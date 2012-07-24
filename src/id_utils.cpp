/*
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2011, 2012, Geoffrey Biggs, geoffrey.biggs@aist.go.jp
 *     RT-Synthesis Research Group
 *     Intelligent Systems Research Institute,
 *     National Institute of Advanced Industrial Science and Technology (AIST),
 *     Japan
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Geoffrey Biggs nor AIST, nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#include <jonen/id_utils.h>

#include <jonen/exceptions.h>

using namespace jonen;


bool jonen::ids::validate(ids::ID id)
{
    if (id == 0 ||
            id == 0xFF ||
            id == 0xFFFF ||
            id == 0xFFFFFF ||
            id == 0xFFFFFFFF)
    {
        return false;
    }
    return true;
}


std::streamsize jonen::ids::size(ids::ID id)
{
    if (id >= 0x80 && id <= 0xFE)
    {
        return 1;
    }
    else if (id >= 0x4000 && id <= 0x7FFE)
    {
        return 2;
    }
    else if (id >= 0x200000 && id <= 0x3FFFFE)
    {
        return 3;
    }
    else if (id >= 0x10000000 && id <= 0x1FFFFFFE)
    {
        return 4;
    }
    /* Uncomment this if EBML IDs expand to 64 bits.
    else if (id >= 0x0800000000 && id <= 0x0FFFFFFFFE)
    {
        return 5;
    }
    else if (id >= 0x040000000000 && id <= 0x07FFFFFFFFFE)
    {
        return 6;
    }
    else if (id >= 0x02000000000000 && id <= 0x03FFFFFFFFFFFE)
    {
        return 7;
    }
    else if (id >= 0x0100000000000000 && id <= 0x01FFFFFFFFFFFFFE)
    {
        return 8;
    }*/
    else
    {
        throw InvalidEBMLID() << err_varint(id);
    }
}


std::vector<char> jonen::ids::encode(ID id)
{
    std::streamsize c_size(size(id));
    std::vector<char> buffer(c_size, 0);
    // Write the remaining bytes
    for (unsigned int ii(0); ii < c_size; ++ii)
    {
        buffer[c_size - ii - 1] = id & 0xFF;
        id >>= 8;
    }
    return buffer;
}


std::streamsize jonen::ids::write(ids::ID id, std::ostream& o)
{
    std::streamsize c_size(size(id));
    // Write the remaining bytes
    for (unsigned int ii(0); ii < c_size; ++ii)
    {
        o.put((id >> (c_size - ii - 1) * 8) & 0xFF);
    }
    if (!o)
    {
        throw WriteError() << err_pos(o.tellp());
    }

    return c_size;
}


ids::DecodeResult jonen::ids::decode(std::vector<char> const& buffer)
{
    assert(buffer.size() > 0);

    unsigned int to_copy(0);
    ids::ID result(0);

    reinterpret_cast<char*>(&result)[0] = buffer[0];
    // Check the size
    if (static_cast<unsigned char>(buffer[0]) >= 0x80) // 1 byte
    {
        to_copy = 0;
    }
    else if (static_cast<unsigned char>(buffer[0]) >= 0x40) // 2 bytes
    {
        to_copy = 1;
    }
    else if (static_cast<unsigned char>(buffer[0]) >= 0x20) // 3 bytes
    {
        to_copy = 2;
    }
    else if (static_cast<unsigned char>(buffer[0]) >= 0x10) // 4 bytes
    {
        to_copy = 3;
    }
    else if (static_cast<unsigned char>(buffer[0]) >= 0x08) // 5 bytes
    {
        to_copy = 4;
    }
    else if (static_cast<unsigned char>(buffer[0]) >= 0x04) // 6 bytes
    {
        to_copy = 5;
    }
    else if (static_cast<unsigned char>(buffer[0]) >= 0x02) // 7 bytes
    {
        to_copy = 6;
    }
    else if (static_cast<unsigned char>(buffer[0]) == 0x01) // 8 bytes
    {
        // The first byte is not part of the result in this case
        to_copy = 7;
    }
    else
    {
        // All bits zero is invalid
        throw InvalidVarInt();
    }

    if (buffer.size() < to_copy + 1)
    {
        throw BufferTooSmall() << err_bufsize(buffer.size()) <<
            err_reqsize(to_copy + 1);
    }

    // Copy the remaining bytes
    for (std::streamsize ii(1); ii < to_copy + 1; ++ii)
    {
        result <<= 8;
        reinterpret_cast<char*>(&result)[0] = buffer[ii];
    }

    // Calling size provides a check on the value of the ID, throwing
    // InvalidEBMLID if it is not in one of the allowable ranges.
    size(result);
    return std::make_pair(result, buffer.begin() + to_copy + 1);
}


ids::ReadResult jonen::ids::read(std::istream& i)
{
    ids::ID result(0);
    std::streamsize to_copy(0);
    uint8_t buffer[8];

    // Read the first byte
    i.read(reinterpret_cast<char*>(buffer), 1);
    if (!i)
    {
        throw ReadError() << err_pos(i.tellg());
    }
    result = buffer[0];
    // Check the size
    if (buffer[0] >= 0x80) // 1 byte
    {
        to_copy = 0;
    }
    else if (buffer[0] >= 0x40) // 2 bytes
    {
        to_copy = 1;
    }
    else if (buffer[0] >= 0x20) // 3 bytes
    {
        to_copy = 2;
    }
    else if (buffer[0] >= 0x10) // 4 bytes
    {
        to_copy = 3;
    }
    else if (buffer[0] >= 0x08) // 5 bytes
    {
        to_copy = 4;
    }
    else if (buffer[0] >= 0x04) // 6 bytes
    {
        to_copy = 5;
    }
    else if (buffer[0] >= 0x02) // 7 bytes
    {
        to_copy = 6;
    }
    else if (buffer[0] == 0x01) // 8 bytes
    {
        // The first byte is not part of the result in this case
        to_copy = 7;
    }
    else
    {
        // All bits zero is invalid
        throw InvalidVarInt();
    }

    // Copy the remaining bytes
    i.read(reinterpret_cast<char*>(&buffer[1]), to_copy);
    if (i.fail())
    {
        throw ReadError() << err_pos(i.tellg());
    }

    for (std::streamsize ii(1); ii < to_copy + 1; ++ii)
    {
        result <<= 8;
        result += buffer[ii];
    }

    // Calling size provides a check on the value of the ID, throwing
    // InvalidEBMLID if it is not in one of the allowable ranges.
    try
    {
        size(result);
    }
    catch(boost::exception& e)
    {
        e << err_pos(i.tellg());
        throw;
    }
    return std::make_pair(result, to_copy + 1);
}

