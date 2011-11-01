/* TIDE
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
 * This file is part of TIDE.
 *
 * TIDE is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * TIDE is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with TIDE. If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined(TIDE_TEST_UTILS_H_)
#define TIDE_TEST_UTILS_H_

#include <gtest/gtest.h>

#include <stdint.h>
#include <string>

namespace test_utils
{

// Tests if to buffers are equal. The length of the buffers must be specified.
::testing::AssertionResult buffers_eq(char const* b1_expr, char const* b2_expr,
        char const* n_expr, uint8_t const* b1, uint8_t const* b2, size_t n);

// Tests if two std::basic_string<uint8_t> byte buffers are equal.
::testing::AssertionResult std_buffers_eq(char const* b1_expr,
        char const* b2_expr,
        std::basic_string<uint8_t> const& b1,
        std::basic_string<uint8_t> const& b2);

// Tests if two std::string buffers are equal.
::testing::AssertionResult std_buffers_eq(char const* b1_expr,
        char const* b2_expr, std::string const& b1, std::string const& b2);

// Tests if two std::pair<uint64_t, size_t> values are equal because gtest
// can't figure it out for itself nor print the result.
::testing::AssertionResult int_pairs_eq(char const* p1_expr,
        char const* p2_expr, std::pair<uint64_t, size_t> const& p1,
        std::pair<uint64_t, size_t> const& p2);

}; // test_utils

#endif // TIDE_TEST_UTILS_H_

