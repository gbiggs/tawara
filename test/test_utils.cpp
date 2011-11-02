/* TIDE
 *
 * Test utilities source file.
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

#include "test_utils.h"

#include <gtest/gtest.h>


::testing::AssertionResult test_utils::buffers_eq(char const* b1_expr, char const* b2_expr,
        char const* n_expr, uint8_t const* b1, uint8_t const* b2, size_t n)
{
    if (strncmp(reinterpret_cast<char const*>(b1),
            reinterpret_cast<char const*>(b2), n) == 0)
    {
        return ::testing::AssertionSuccess();
    }
    else
    {
        std::stringstream b1_str;
        b1_str << std::hex;
        b1_str.width(2);
        b1_str.fill('0');
        for (size_t ii(0); ii < n; ++ii)
        {
            b1_str << static_cast<int>(b1[ii]);
        }
        std::stringstream b2_str;
        b2_str << std::hex;
        b2_str.width(2);
        b2_str.fill('0');
        for (size_t ii(0); ii < n; ++ii)
        {
            b2_str << static_cast<int>(b2[ii]);
        }
        return ::testing::AssertionFailure() << b1_expr << ": 0x" <<
            b1_str.str() << '\t' << b2_expr << ": 0x" << b2_str.str();
    }
}


::testing::AssertionResult test_utils::std_buffers_eq(char const* b1_expr,
        char const* b2_expr,
        std::basic_string<uint8_t> const& b1,
        std::basic_string<uint8_t> const& b2)
{
    if (b1.size() != b2.size())
    {
        return ::testing::AssertionFailure() << b1_expr << " length (" <<
            b1.size() << ") != " << b2_expr << " length (" << b2.size() << ')';
    }
    if (strncmp(reinterpret_cast<char const*>(b1.c_str()),
            reinterpret_cast<char const*>(b2.c_str()), b1.size()) == 0)
    {
        return ::testing::AssertionSuccess();
    }
    else
    {
        std::stringstream b1_str;
        for (size_t ii(0); ii < b1.size(); ++ii)
        {
            b1_str << std::hex << std::setw(2) << std::setfill('0') <<
                +(b1[ii] & 0xFF) << ' ';
        }
        std::stringstream b2_str;
        for (size_t ii(0); ii < b2.size(); ++ii)
        {
            b2_str << std::hex << std::setw(2) << std::setfill('0') <<
                +(b2[ii] & 0xFF) << ' ';
        }
        return ::testing::AssertionFailure() << b1_expr << " (" << b1.size() <<
            "):\t0x" << b1_str.str() << '\n' << b2_expr << " (" << b2.size() <<
            "):\t\t0x" << b2_str.str();
    }
}


::testing::AssertionResult test_utils::std_buffers_eq(char const* b1_expr,
        char const* b2_expr, std::string const& b1, std::string const& b2)
{
    if (b1.size() != b2.size())
    {
        return ::testing::AssertionFailure() << b1_expr << " length (" <<
            b1.size() << ") != " << b2_expr << " length (" << b2.size() << ')';
    }
    if (strncmp(reinterpret_cast<char const*>(b1.c_str()),
            reinterpret_cast<char const*>(b2.c_str()), b1.size()) == 0)
    {
        return ::testing::AssertionSuccess();
    }
    else
    {
        std::stringstream b1_str;
        for (size_t ii(0); ii < b1.size(); ++ii)
        {
            b1_str << std::hex << std::setw(2) << std::setfill('0') <<
                +(b1[ii] & 0xFF) << ' ';
        }
        std::stringstream b2_str;
        for (size_t ii(0); ii < b2.size(); ++ii)
        {
            b2_str << std::hex << std::setw(2) << std::setfill('0') <<
                +(b2[ii] & 0xFF) << ' ';
        }
        return ::testing::AssertionFailure() << b1_expr << " (" << b1.size() <<
            "):\t0x" << b1_str.str() << '\n' << b2_expr << " (" << b2.size() <<
            "):\t\t0x" << b2_str.str();
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
        for (size_t ii(0); ii < b1.size(); ++ii)
        {
            b1_str << std::hex << std::setw(2) << std::setfill('0') <<
                +(b1[ii] & 0xFF);
        }
        std::stringstream b2_str;
        for (size_t ii(0); ii < b2.size(); ++ii)
        {
            b2_str << std::hex << std::setw(2) << std::setfill('0') <<
                +(b2[ii] & 0xFF);
        }
        return ::testing::AssertionFailure() << b1_expr << ": 0x" <<
            b1_str.str() << '\t' << b2_expr << ": 0x" << b2_str.str();
    }
}


::testing::AssertionResult test_utils::int_pairs_eq(char const* p1_expr,
        char const* p2_expr, std::pair<uint64_t, size_t> const& p1,
        std::pair<uint64_t, size_t> const& p2)
{
    if (p1 != p2)
    {
        return ::testing::AssertionFailure() << p1_expr << " (" << p1.first <<
            ", " << p1.second << ") != " << p2_expr << " (" << p2.first <<
            ", " << p2.second << ")";
    }
    return ::testing::AssertionSuccess();
}

