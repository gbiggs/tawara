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

#if !defined(TEST_UTILITIES_H_)
#define TEST_UTILITIES_H_

#include <boost/shared_ptr.hpp>
#include <gtest/gtest.h>
#include <jonen/element.h>
#include <jonen/ids.h>
#include <vector>


namespace test_utils
{
    // Tests if two std::strings are equal.
    ::testing::AssertionResult std_strings_eq(char const* b1_expr,
            char const* b2_expr, std::string const& b1, std::string const& b2);


    // Tests if two std::string buffers are equal.
    ::testing::AssertionResult std_buffers_eq(char const* b1_expr,
            char const* b2_expr, std::string const& b1, std::string const& b2);


    // Tests if two std::vector<char> buffers are equal.
    ::testing::AssertionResult std_vectors_eq(char const* v1_expr,
            char const* v2_expr, std::vector<char> const& v1,
            std::vector<char> const& v2);


    // A pointer to an element
    typedef boost::shared_ptr<jonen::Element> ElPtr;


    // Calculates the size of a bunch of elements
    std::streamsize children_size(std::vector<ElPtr> const& children);


    // Fills a buffer with the equivalent of a master element
    std::streamsize fill_buffer(std::string& b, jonen::ids::ID id,
            std::vector<ElPtr> const& children, bool write_id, bool write_size,
            bool write_body, bool use_crc=false);


    // Writes all elements in a list to a buffer except for one
    std::streamsize write_except(std::stringstream& ss,
            std::vector<ElPtr> const& children, int except);
}; // test_utils

#endif // !defined(TEST_UTILITIES_H_)

