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

#if !defined(JONEN_TEST_UTILS_H_)
#define JONEN_TEST_UTILS_H_

#include <boost/shared_ptr.hpp>
#include <functional>
#include <gtest/gtest.h>
#include <stdint.h>
#include <string>
#include <jonen/element.h>
#include <jonen/prim_element.h>
#include <vector>


namespace test_utils
{

typedef boost::shared_ptr<jonen::Element> ElPtr;

// Adds the total size of an element to a value
struct TotalSizeOp
    : public std::binary_function<uint64_t, ElPtr, uint64_t>
{
    uint64_t operator()(uint64_t total, ElPtr el)
    {
        return total + el->size();
    }
};


// Tests if two std::basic_string<uint8_t> byte buffers are equal.
::testing::AssertionResult std_buffers_eq(char const* b1_expr,
        char const* b2_expr,
        std::basic_string<uint8_t> const& b1,
        std::basic_string<uint8_t> const& b2);

// Tests if two std::string buffers are equal.
::testing::AssertionResult std_buffers_eq(char const* b1_expr,
        char const* b2_expr, std::string const& b1, std::string const& b2);

// Tests if two std::vector<char> buffers are equal.
::testing::AssertionResult std_vectors_eq(char const* b1_expr,
        char const* b2_expr, std::vector<char> const& b1,
        std::vector<char> const& b2);

// Tests if two std::pair values are equal because gtest can't figure it out
// for itself nor print the result.
template<typename T1, typename T2>
::testing::AssertionResult pairs_eq(char const* p1_expr,
        char const* p2_expr, typename std::pair<uint64_t, T1> const& p1,
        typename std::pair<uint64_t, T2> const& p2)
{
    if (p1.first != p2.first || p1.second != p2.second)
    {
        return ::testing::AssertionFailure() << p1_expr << " (" << p1.first <<
            ", " << p1.second << ") != " << p2_expr << " (" << p2.first <<
            ", " << p2.second << ")";
    }
    return ::testing::AssertionSuccess();
}

// Returns a blob of binary data, with a length that increases by 5 bytes
// each time this function is called.
boost::shared_ptr<std::vector<char> > make_blob(size_t size);

}; // test_utils

#endif // JONEN_TEST_UTILS_H_

