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

#include <jonen/master_element_impl.h>

#include <jonen/binary_element.h>
#include <jonen/exceptions.h>
#include <jonen/id_utils.h>
#include <jonen/vint.h>

#include <boost/crc.hpp>

using namespace jonen;
using namespace jonen::impl;

///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

MasterElementImpl::MasterElementImpl(bool use_crc)
    : use_crc_(use_crc)
{
}


void MasterElementImpl::swap(MasterElementImpl& other)
{
    using std::swap;

    swap(use_crc_, other.use_crc_);
}


std::streamsize MasterElementImpl::crc_size() const
{
    if (use_crc_)
    {
        return ids::size(ids::CRC32) +
            vint::size(4) + // Binary element size bytes
            4; // Binary element body size (CRC32 = 4 bytes)
    }
    else
    {
        return 0;
    }
}


std::streamsize MasterElementImpl::read_crc(std::istream& i,
        std::streamsize size)
{
    std::streamsize read_bytes(0);
    // Read the entire body and check the CRC
    std::vector<char> body;
    read_bytes = read_with_crc(body, i, size);
    // Rewind to the first element after the CRC
    i.seekg(-body.size(), std::ios::cur);
    // The number of bytes read is the size of the CRC element, or zero if
    // there wasn't one.
    return read_bytes - body.size();
}


std::streamsize MasterElementImpl::read_with_crc(std::vector<char>& body,
        std::istream& i, std::streamsize size)
{
    if (size == 0)
    {
        // Nothing to do for an empty element
        return 0;
    }
    std::streamsize read_bytes(0);
    // Read an ID and check if it is a CRC32 element
    ids::ReadResult id_res = ids::read(i);
    read_bytes = id_res.second;
    if (id_res.first == ids::CRC32)
    {
        use_crc_ = true;
        // Have a CRC value, so read it
        BinaryElement stored_crc_el(ids::CRC32, std::vector<char>());
        read_bytes += stored_crc_el.read(i);
        long int stored_crc;
        memcpy(&stored_crc, stored_crc_el.data(), 4);
        size -= read_bytes;
        // Read the rest of the body
        body.resize(size, 0);
        i.read(&body[0], size);
        if (!i)
        {
            throw ReadError() << err_pos(i.tellg());
        }
        read_bytes += size;
        boost::crc_32_type calc_crc;
        calc_crc.process_bytes(&body[0], body.size());
        if (stored_crc != calc_crc.checksum())
        {
            throw BadCRC() << err_expected_crc(calc_crc.checksum()) <<
                err_stored_crc(stored_crc);
        }
    }
    else
    {
        use_crc_ = false;
        // No CRC, so rewind and read the entire body
        i.seekg(-id_res.second, std::ios::cur);
        body.resize(size, 0);
        i.read(&body[0], size);
        if (!i)
        {
            throw ReadError() << err_pos(i.tellg());
        }
        read_bytes = size;
    }
    return read_bytes;
}


std::streamsize MasterElementImpl::write_crc(std::vector<char> const& body,
        std::iostream& io)
{
    std::streamsize result(0);
    if (use_crc_)
    {
        boost::crc_32_type crc;
        crc.process_bytes(&body[0], body.size());
        BinaryElement crc_el(ids::CRC32, std::vector<char>());
        crc_el.push_back((crc.checksum() & 0x000000FF));
        crc_el.push_back((crc.checksum() & 0x0000FF00) >> 8);
        crc_el.push_back((crc.checksum() & 0x00FF0000) >> 16);
        crc_el.push_back((crc.checksum() & 0xFF000000) >> 24);
        result = write(crc_el, io);
    }
    return result;
}


std::streamsize MasterElementImpl::write_with_crc(
        std::vector<char> const& body, std::iostream& io)
{
    std::streamsize result(0);
    result = write_crc(body, io);
    io.write(&body[0], body.size());
    result += body.size();
    return result;
}
