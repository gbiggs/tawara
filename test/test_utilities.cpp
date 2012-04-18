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

#include <gtest/gtest.h>

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


::testing::AssertionResult test_utils::std_vectors_eq(char const* b1_expr,
        char const* b2_expr, std::vector<char> const& b1,
        std::vector<char> const& b2)
{
    if (b1.size() != b2.size())
    {
        return ::testing::AssertionFailure() << b1_expr << " length (" <<
            b1.size() << ") != " << b2_expr << " length (" << b2.size() << ')';
    }
    if (b1 == b2)
    {
        return ::testing::AssertionSuccess();
    }
    else
    {
        std::stringstream b1_str;
        for (unsigned int ii(0); ii < b1.size(); ++ii)
        {
            b1_str << std::hex << std::setw(2) << std::setfill('0') <<
                +(b1[ii] & 0xFF);
        }
        std::stringstream b2_str;
        for (unsigned int ii(0); ii < b2.size(); ++ii)
        {
            b2_str << std::hex << std::setw(2) << std::setfill('0') <<
                +(b2[ii] & 0xFF);
        }
        return ::testing::AssertionFailure() << b1_expr << ": 0x" <<
            b1_str.str() << '\t' << b2_expr << ": 0x" << b2_str.str();
    }
}

