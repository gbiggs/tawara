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

#include "test_utilities.h"

#include <boost/crc.hpp>
#include <boost/foreach.hpp>
#include <gtest/gtest.h>
#include <jonen/binary_element.h>
#include <jonen/ebml_integer.h>
#include <jonen/vint.h>

using namespace jonen;
using namespace test_utils;


::testing::AssertionResult test_utils::std_strings_eq(char const* b1_expr,
        char const* b2_expr, std::string const& b1, std::string const& b2)
{
    return test_utils::std_buffers_eq(b1_expr, b2_expr, b1, b2);
}


::testing::AssertionResult test_utils::std_buffers_eq(char const* b1_expr,
        char const* b2_expr, std::string const& b1, std::string const& b2)
{
    if (b1.size() != b2.size())
    {
        std::stringstream b1_str;
        for (unsigned int ii(0); ii < b1.size(); ++ii)
        {
            b1_str << std::hex << std::setw(2) << std::setfill('0') <<
                +(b1[ii] & 0xFF) << ' ';
        }
        std::stringstream b2_str;
        for (unsigned int ii(0); ii < b2.size(); ++ii)
        {
            b2_str << std::hex << std::setw(2) << std::setfill('0') <<
                +(b2[ii] & 0xFF) << ' ';
        }
        return ::testing::AssertionFailure() << b1_expr << " length != " <<
            b2_expr << " length:\n" << b1_expr << " (" << b1.size() <<
            "):\t0x" << b1_str.str() << '\n' << b2_expr << " (" << b2.size() <<
            "):\t0x" << b2_str.str();
    }
    if (strncmp(reinterpret_cast<char const*>(b1.c_str()),
            reinterpret_cast<char const*>(b2.c_str()), b1.size()) == 0)
    {
        return ::testing::AssertionSuccess();
    }
    else
    {
        std::stringstream b1_str;
        for (unsigned int ii(0); ii < b1.size(); ++ii)
        {
            b1_str << std::hex << std::setw(2) << std::setfill('0') <<
                +(b1[ii] & 0xFF) << ' ';
        }
        std::stringstream b2_str;
        for (unsigned int ii(0); ii < b2.size(); ++ii)
        {
            b2_str << std::hex << std::setw(2) << std::setfill('0') <<
                +(b2[ii] & 0xFF) << ' ';
        }
        return ::testing::AssertionFailure() << b1_expr << " (" << b1.size() <<
            "):\t0x" << b1_str.str() << '\n' << b2_expr << " (" << b2.size() <<
            "):\t0x" << b2_str.str();
    }
}


::testing::AssertionResult test_utils::std_vectors_eq(char const* v1_expr,
        char const* v2_expr, std::vector<char> const& v1,
        std::vector<char> const& v2)
{
    if (v1.size() != v2.size())
    {
        return ::testing::AssertionFailure() << v1_expr << " length (" <<
            v1.size() << ") != " << v2_expr << " length (" << v2.size() << ')';
    }
    if (v1 == v2)
    {
        return ::testing::AssertionSuccess();
    }
    else
    {
        std::stringstream v1_str;
        for (unsigned int ii(0); ii < v1.size(); ++ii)
        {
            v1_str << std::hex << std::setw(2) << std::setfill('0') <<
                +(v1[ii] & 0xFF);
        }
        std::stringstream v2_str;
        for (unsigned int ii(0); ii < v2.size(); ++ii)
        {
            v2_str << std::hex << std::setw(2) << std::setfill('0') <<
                +(v2[ii] & 0xFF);
        }
        return ::testing::AssertionFailure() << v1_expr << ": 0x" <<
            v1_str.str() << '\t' << v2_expr << ": 0x" << v2_str.str();
    }
}


std::streamsize test_utils::children_size(std::vector<ElPtr> const& children)
{
    std::streamsize body_size(0);
    BOOST_FOREACH(ElPtr el, children)
    {
        body_size += el->stored_size();
    }
    return body_size;
}


std::streamsize test_utils::fill_buffer(std::string& b, jonen::ids::ID id,
        std::vector<ElPtr> const& children, bool write_id, bool write_size,
        bool write_body, bool use_crc)
{
    char temp[8];
    std::streamsize total(0);
    if (write_id)
    {
        // Cheating on the IDs a bit - there is no protection here against
        // invalid IDs
        std::vector<char> tmp(ebml_int::encode_u(id));
        b.append(&tmp[0], 0, tmp.size());
        total += tmp.size();
    }
    if (write_size)
    {
        int crc_size(0);
        if (use_crc)
        {
            crc_size = 6;
        }
        std::vector<char> tmp(vint::encode(children_size(children) +
                    crc_size));
        b.append(&tmp[0], 0, tmp.size());
        total += tmp.size();
    }
    if (write_body)
    {
        std::stringstream ss;
        BOOST_FOREACH(ElPtr el, children)
        {
            total += write(*el, ss);
        }
        if (use_crc)
        {
            boost::crc_32_type crc;
            crc.process_bytes(ss.str().c_str(), ss.str().length());
            BinaryElement crc_el(ids::CRC32, std::vector<char>());
            crc_el.push_back((crc.checksum() & 0x000000FF));
            crc_el.push_back((crc.checksum() & 0x0000FF00) >> 8);
            crc_el.push_back((crc.checksum() & 0x00FF0000) >> 16);
            crc_el.push_back((crc.checksum() & 0xFF000000) >> 24);
            std::stringstream crc_ss;
            total += write(crc_el, crc_ss);
            b += crc_ss.str();
        }
        b += ss.str();
    }
    return total;
}


std::streamsize test_utils::write_except(std::stringstream& ss,
        std::vector<ElPtr> const& children, int except)
{
    std::streamsize body_size(0);
    for (int ii(0); ii < children.size(); ++ii)
    {
        if (ii == except)
        {
            continue;
        }
        body_size += write(*children[ii], ss);
    }
    return body_size;
}

