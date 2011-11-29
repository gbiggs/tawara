/* Tide
 *
 * Test utilities header file.
 *
 * Copyright 2011 Geoffrey Biggs geoffrey.biggs@aist.go.jp
 *     RT-Synthesis Research Group
 *     Intelligent Systems Research Institute,
 *     National Institute of Advanced Industrial Science and Technology (AIST),
 *     Japan
 *     All rights reserved.
 *
 * This file is part of Tide.
 *
 * Tide is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * Tide is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Tide. If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined(TIDE_TEST_UTILS_H_)
#define TIDE_TEST_UTILS_H_

#include <boost/shared_ptr.hpp>
#include <functional>
#include <gtest/gtest.h>
#include <stdint.h>
#include <string>
#include <tide/element.h>
#include <tide/prim_element.h>


namespace test_utils
{

typedef boost::shared_ptr<tide::Element> ElPtr;

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

}; // test_utils

#endif // TIDE_TEST_UTILS_H_

