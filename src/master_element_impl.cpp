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

#include <celduin/master_element_impl.h>

#include <celduin/binary_element.h>
#include <celduin/exceptions.h>
#include <celduin/id_utils.h>
#include <celduin/vint.h>

using namespace celduin;
using namespace celduin::impl;

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


std::streamsize MasterElementImpl::read_crc(std::istream& i)
{
    BinaryElement crc_bin(ids::Null, std::vector<char>());
    std::streamsize result;

    result = crc_bin.read(i);
    long int crc;
    memcpy(&crc, crc_bin.data(), 4);

    if (false)//(crc != calculated crc)
    {
        throw BadCRC() << err_expected_crc(crc) <<
            err_calc_crc(0); // TODO
    }

    return result;
}


std::streamsize MasterElementImpl::write_crc(std::ostream& o)
{
    if (use_crc_)
    {
        // TODO
        throw NotImplemented();
    }
    else
    {
        return 0;
    }
}
