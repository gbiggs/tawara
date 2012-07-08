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

#include <celduin/element_utils.h>
#include <celduin/exceptions.h>
#include <celduin/id_utils.h>
#include <celduin/vint.h>

using namespace celduin;


void celduin::validate_id(ids::ID id)
{
    if (id == 0 ||
            id == 0xFF ||
            id == 0xFFFF ||
            id == 0xFFFFFF ||
            id == 0xFFFFFFFF)
    {
        throw InvalidElementID() << err_id(id);
    }
}


std::streamsize celduin::skip_read(std::iostream& io, bool and_id)
{
    std::streamsize skipped_bytes(0);
    if (and_id)
    {
        skipped_bytes += ids::read(io).second;
    }
    vint::ReadResult size_res(vint::read(io));
    skipped_bytes += size_res.second;
    io.seekg(size_res.first, std::ios::cur);
    skipped_bytes += size_res.first;
    return skipped_bytes;
}


std::streamsize celduin::skip_write(std::iostream& io, bool and_id)
{
    std::streampos cur_read(io.tellg());
    io.seekg(io.tellp());
    std::streamsize skipped_bytes = celduin::skip_read(io, and_id);
    io.seekp(io.tellg());
    io.seekg(cur_read);
    return skipped_bytes;
}

