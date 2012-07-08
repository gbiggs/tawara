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

#include <celduin/ebml_integer.h>
#include <celduin/exceptions.h>


std::streamsize celduin::ebml_int::size_u(unsigned long long int integer)
{
    if (integer == 0)
    {
        return 0;
    }
    else if (integer <= 0xFF)
    {
        return 1;
    }
    else if (integer <= 0xFFFF)
    {
        return 2;
    }
    else if (integer <= 0xFFFFFF)
    {
        return 3;
    }
    else if (integer <= 0xFFFFFFFFl)
    {
        return 4;
    }
    else if (integer <= 0xFFFFFFFFFFl)
    {
        return 5;
    }
    else if (integer <= 0xFFFFFFFFFFFFl)
    {
        return 6;
    }
    else if (integer <= 0xFFFFFFFFFFFFFFl)
    {
        return 7;
    }
    else
    {
        return 8;
    }
}


std::streamsize celduin::ebml_int::size_s(long long int integer)
{
    if (integer == 0)
    {
        return 0;
    }
    else if (integer >= -0x80 && integer <= 0x7F)
    {
        return 1;
    }
    else if (integer >= -0x8000 && integer <= 0x7FFF)
    {
        return 2;
    }
    else if (integer >= -0x800000 && integer <= 0x7FFFFF)
    {
        return 3;
    }
    else if (integer >= -0x80000000l && integer <= 0x7FFFFFFFl)
    {
        return 4;
    }
    else if (integer >= -0x8000000000l && integer <= 0x7FFFFFFFFFl)
    {
        return 5;
    }
    else if (integer >= -0x800000000000l && integer <= 0x7FFFFFFFFFFFl)
    {
        return 6;
    }
    else if (integer >= -0x80000000000000l && integer <= 0x7FFFFFFFFFFFFFl)
    {
        return 7;
    }
    else
    {
        return 8;
    }
}


std::vector<char> celduin::ebml_int::encode_u(unsigned long long int integer)
{
    std::vector<char> buffer;
    if (integer == 0)
    {
        // Zero values are encoded as nothing
        return buffer;
    }
    std::streamsize size(size_u(integer));
    buffer.assign(size, 0);
    for (std::streamsize ii(0); ii < size; ++ii)
    {
        buffer[size - ii - 1] = integer & 0xFF;
        integer >>= 8;
    }

    return buffer;
}


std::vector<char> celduin::ebml_int::encode_s(long long int integer)
{
    std::vector<char> buffer;
    if (integer == 0)
    {
        // Zero values are encoded as nothing
        return buffer;
    }
    std::streamsize size(size_s(integer));
    buffer.assign(size, 0);
    for (std::streamsize ii(0); ii < size; ++ii)
    {
        buffer[size - ii - 1] = integer & 0xFF;
        integer >>= 8;
    }

    return buffer;
}


std::streamsize celduin::ebml_int::write_u(unsigned long long int integer,
        std::ostream& o)
{
    std::vector<char> buffer(encode_u(integer));

    if (buffer.size() != 0)
    {
        o.write(&buffer[0], buffer.size());
        if (!o)
        {
            throw celduin::WriteError() << celduin::err_pos(o.tellp());
        }
    }
    return buffer.size();
}


std::streamsize celduin::ebml_int::write_s(long long int integer, std::ostream& o)
{
    std::vector<char> buffer(encode_s(integer));
    if (buffer.size() != 0)
    {
        o.write(&buffer[0], buffer.size());
        if (!o)
        {
            throw celduin::WriteError() << celduin::err_pos(o.tellp());
        }
    }
    return buffer.size();
}


unsigned long long int celduin::ebml_int::decode_u(std::vector<char> const& buffer)
{
    assert(buffer.size() <= 8);

    if (buffer.empty())
    {
        // Zero-length value means a zero-value integer
        return 0;
    }

    unsigned long long int result(0);
    for (unsigned int ii(0); ii < buffer.size(); ++ii)
    {
        result <<= 8;
        (reinterpret_cast<char*>(&result))[0] |= buffer[ii];
    }
    return result;
}


long long int celduin::ebml_int::decode_s(std::vector<char> const& buffer)
{
    assert(buffer.size() <= 8);

    if (buffer.empty())
    {
        // Zero-length value means a zero-value integer
        return 0;
    }

    long long int result(0);
    if (buffer[0] & 0x80)
    {
        // Negative value
        result = -1;
    }
    for (unsigned int ii(0); ii < buffer.size(); ++ii)
    {
        result <<= 8;
        (reinterpret_cast<char*>(&result))[0] |= buffer[ii];
    }
    return result;
}


unsigned long long int celduin::ebml_int::read_u(std::istream& i,
        std::streamsize n)
{
    assert(n <= 8);

    std::vector<char> tmp(n, 0);
    i.read(&tmp[0], n);
    if (!i)
    {
        throw celduin::ReadError() << celduin::err_pos(i.tellg());
    }
    return celduin::ebml_int::decode_u(tmp);
}


long long int celduin::ebml_int::read_s(std::istream& i, std::streamsize n)
{
    assert(n <= 8);

    std::vector<char> tmp(n, 0);
    i.read(&tmp[0], n);
    if (!i)
    {
        throw celduin::ReadError() << celduin::err_pos(i.tellg());
    }
    return celduin::ebml_int::decode_s(tmp);
}

