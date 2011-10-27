/* TIDE
 *
 * EBML integer tests.
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

#include <gtest/gtest.h>
#include <tide/exceptions.h>
#include <tide/ebml_int.h>

#include "test_consts.h"


namespace tide
{
namespace ebml_int
{

::testing::AssertionResult buffers_eq(char const* b1_expr, char const* b2_expr,
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


TEST(EBMLInt, CodedSizeUnsigned)
{
    // 1 byte
    EXPECT_EQ(1, tide::ebml_int::coded_size_u(0x00));
    EXPECT_EQ(1, tide::ebml_int::coded_size_u(0x01));
    EXPECT_EQ(1, tide::ebml_int::coded_size_u(0x7F));
    EXPECT_EQ(1, tide::ebml_int::coded_size_u(0xFF));
    // 2 bytes
    EXPECT_EQ(2, tide::ebml_int::coded_size_u(0x0100));
    EXPECT_EQ(2, tide::ebml_int::coded_size_u(0x0101));
    EXPECT_EQ(2, tide::ebml_int::coded_size_u(0x7FFF));
    EXPECT_EQ(2, tide::ebml_int::coded_size_u(0xFFFF));
    // 3 bytes
    EXPECT_EQ(3, tide::ebml_int::coded_size_u(0x010000));
    EXPECT_EQ(3, tide::ebml_int::coded_size_u(0x010001));
    EXPECT_EQ(3, tide::ebml_int::coded_size_u(0x7FFFFF));
    EXPECT_EQ(3, tide::ebml_int::coded_size_u(0xFFFFFF));
    // 4 bytes
    EXPECT_EQ(4, tide::ebml_int::coded_size_u(0x01000000));
    EXPECT_EQ(4, tide::ebml_int::coded_size_u(0x01000001));
    EXPECT_EQ(4, tide::ebml_int::coded_size_u(0x7FFFFFFF));
    EXPECT_EQ(4, tide::ebml_int::coded_size_u(0xFFFFFFFF));
    // 5 bytes
    EXPECT_EQ(5, tide::ebml_int::coded_size_u(0x0100000000));
    EXPECT_EQ(5, tide::ebml_int::coded_size_u(0x0100000001));
    EXPECT_EQ(5, tide::ebml_int::coded_size_u(0x7FFFFFFFFF));
    EXPECT_EQ(5, tide::ebml_int::coded_size_u(0xFFFFFFFFFF));
    // 6 bytes
    EXPECT_EQ(6, tide::ebml_int::coded_size_u(0x010000000000));
    EXPECT_EQ(6, tide::ebml_int::coded_size_u(0x010000000001));
    EXPECT_EQ(6, tide::ebml_int::coded_size_u(0x7FFFFFFFFFFF));
    EXPECT_EQ(6, tide::ebml_int::coded_size_u(0xFFFFFFFFFFFF));
    // 7 bytes
    EXPECT_EQ(7, tide::ebml_int::coded_size_u(0x01000000000000));
    EXPECT_EQ(7, tide::ebml_int::coded_size_u(0x01000000000001));
    EXPECT_EQ(7, tide::ebml_int::coded_size_u(0x7FFFFFFFFFFFFF));
    EXPECT_EQ(7, tide::ebml_int::coded_size_u(0xFFFFFFFFFFFFFF));
    // 8 bytes
    EXPECT_EQ(8, tide::ebml_int::coded_size_u(0x0100000000000000));
    EXPECT_EQ(8, tide::ebml_int::coded_size_u(0x0100000000000001));
    EXPECT_EQ(8, tide::ebml_int::coded_size_u(0x7FFFFFFFFFFFFFFF));
    EXPECT_EQ(8, tide::ebml_int::coded_size_u(0xFFFFFFFFFFFFFFFF));
}


TEST(EBMLInt, CodedSizeSigned)
{
    // 1 byte
    EXPECT_EQ(1, tide::ebml_int::coded_size_s(-1));
    EXPECT_EQ(1, tide::ebml_int::coded_size_s(0));
    EXPECT_EQ(1, tide::ebml_int::coded_size_s(1));
    EXPECT_EQ(1, tide::ebml_int::coded_size_s(-0x80));
    EXPECT_EQ(1, tide::ebml_int::coded_size_s(0x7F));
    // 2 bytes
    EXPECT_EQ(2, tide::ebml_int::coded_size_s(-0x81));
    EXPECT_EQ(2, tide::ebml_int::coded_size_s(0x80));
    EXPECT_EQ(2, tide::ebml_int::coded_size_s(-0x8000));
    EXPECT_EQ(2, tide::ebml_int::coded_size_s(0x7FFF));
    // 3 bytes
    EXPECT_EQ(3, tide::ebml_int::coded_size_s(-0x8001));
    EXPECT_EQ(3, tide::ebml_int::coded_size_s(0x8000));
    EXPECT_EQ(3, tide::ebml_int::coded_size_s(-0x800000));
    EXPECT_EQ(3, tide::ebml_int::coded_size_s(0x7FFFFF));
    // 4 bytes
    EXPECT_EQ(4, tide::ebml_int::coded_size_s(-0x800001));
    EXPECT_EQ(4, tide::ebml_int::coded_size_s(0x800000));
    EXPECT_EQ(4, tide::ebml_int::coded_size_s(-0x80000000l));
    EXPECT_EQ(4, tide::ebml_int::coded_size_s(0x7FFFFFFFl));
    // 5 bytes
    EXPECT_EQ(5, tide::ebml_int::coded_size_s(-0x80000001l));
    EXPECT_EQ(5, tide::ebml_int::coded_size_s(0x80000000l));
    EXPECT_EQ(5, tide::ebml_int::coded_size_s(-0x8000000000l));
    EXPECT_EQ(5, tide::ebml_int::coded_size_s(0x7FFFFFFFFFl));
    // 6 bytes
    EXPECT_EQ(6, tide::ebml_int::coded_size_s(-0x8000000001l));
    EXPECT_EQ(6, tide::ebml_int::coded_size_s(0x8000000000l));
    EXPECT_EQ(6, tide::ebml_int::coded_size_s(-0x800000000000l));
    EXPECT_EQ(6, tide::ebml_int::coded_size_s(0x7FFFFFFFFFFFl));
    // 7 bytes
    EXPECT_EQ(7, tide::ebml_int::coded_size_s(-0x800000000001l));
    EXPECT_EQ(7, tide::ebml_int::coded_size_s(0x800000000000l));
    EXPECT_EQ(7, tide::ebml_int::coded_size_s(-0x80000000000000l));
    EXPECT_EQ(7, tide::ebml_int::coded_size_s(0x7FFFFFFFFFFFFFl));
    // 8 bytes
    EXPECT_EQ(8, tide::ebml_int::coded_size_s(-0x80000000000001l));
    EXPECT_EQ(8, tide::ebml_int::coded_size_s(0x80000000000000l));
    EXPECT_EQ(8, tide::ebml_int::coded_size_s(-0x8000000000000000l));
    EXPECT_EQ(8, tide::ebml_int::coded_size_s(0x7FFFFFFFFFFFFFFFl));
}


TEST(EBMLInt, EncodeUnsigned)
{
    uint8_t buffer[8], expected[8];
    memset(buffer, 0, sizeof(buffer));
    memset(expected, 0, sizeof(expected));
    // 1 byte
    expected[0] = 0x00;
    EXPECT_EQ(1, tide::ebml_int::encode_u(0x00, buffer, 1));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x01;
    EXPECT_EQ(1, tide::ebml_int::encode_u(0x01, buffer, 1));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x7F;
    EXPECT_EQ(1, tide::ebml_int::encode_u(0x7F, buffer, 1));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0xFF;
    EXPECT_EQ(1, tide::ebml_int::encode_u(0xFF, buffer, 1));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    // 2 bytes
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    expected[0] = 0x01;
    EXPECT_EQ(2, tide::ebml_int::encode_u(0x0100, buffer, 2));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x01; expected[1] = 0x01;
    EXPECT_EQ(2, tide::ebml_int::encode_u(0x0101, buffer, 2));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x7F; expected[1] = 0xFF;
    EXPECT_EQ(2, tide::ebml_int::encode_u(0x7FFF, buffer, 2));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0xFF; expected[1] = 0xFF;
    EXPECT_EQ(2, tide::ebml_int::encode_u(0xFFFF, buffer, 2));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    // 3 bytes
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    expected[0] = 0x01;
    EXPECT_EQ(3, tide::ebml_int::encode_u(0x010000, buffer, 3));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x01; expected[2] = 0x01;
    EXPECT_EQ(3, tide::ebml_int::encode_u(0x010001, buffer, 3));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x7F; expected[1] = 0xFF; expected[2] = 0xFF;
    EXPECT_EQ(3, tide::ebml_int::encode_u(0x7FFFFF, buffer, 3));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0xFF;
    EXPECT_EQ(3, tide::ebml_int::encode_u(0xFFFFFF, buffer, 3));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    // 4 bytes
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    expected[0] = 0x01;
    EXPECT_EQ(4, tide::ebml_int::encode_u(0x01000000, buffer, 4));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x01; expected[3] = 0x01;
    EXPECT_EQ(4, tide::ebml_int::encode_u(0x01000001, buffer, 4));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    memset(expected, 0xFF, 4);
    expected[0] = 0x7F;
    EXPECT_EQ(4, tide::ebml_int::encode_u(0x7FFFFFFF, buffer, 4));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0xFF;
    EXPECT_EQ(4, tide::ebml_int::encode_u(0xFFFFFFFF, buffer, 4));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    // 5 bytes
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    expected[0] = 0x01;
    EXPECT_EQ(5, tide::ebml_int::encode_u(0x0100000000l, buffer, 5));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x01; expected[4] = 0x01;
    EXPECT_EQ(5, tide::ebml_int::encode_u(0x0100000001l, buffer, 5));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    memset(expected, 0xFF, 5);
    expected[0] = 0x7F;
    EXPECT_EQ(5, tide::ebml_int::encode_u(0x7FFFFFFFFFl, buffer, 5));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0xFF;
    EXPECT_EQ(5, tide::ebml_int::encode_u(0xFFFFFFFFFFl, buffer, 5));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    // 6 bytes
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    expected[0] = 0x01;
    EXPECT_EQ(6, tide::ebml_int::encode_u(0x010000000000l, buffer, 6));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x01; expected[5] = 0x01;
    EXPECT_EQ(6, tide::ebml_int::encode_u(0x010000000001l, buffer, 6));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    memset(expected, 0xFF, 6);
    expected[0] = 0x7F;
    EXPECT_EQ(6, tide::ebml_int::encode_u(0x7FFFFFFFFFFFl, buffer, 6));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0xFF;
    EXPECT_EQ(6, tide::ebml_int::encode_u(0xFFFFFFFFFFFFl, buffer, 6));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    // 7 bytes
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    expected[0] = 0x01;
    EXPECT_EQ(7, tide::ebml_int::encode_u(0x01000000000000l, buffer, 7));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x01; expected[6] = 0x01;
    EXPECT_EQ(7, tide::ebml_int::encode_u(0x01000000000001l, buffer, 7));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    memset(expected, 0xFF, 7);
    expected[0] = 0x7F;
    EXPECT_EQ(7, tide::ebml_int::encode_u(0x7FFFFFFFFFFFFFl, buffer, 7));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0xFF;
    EXPECT_EQ(7, tide::ebml_int::encode_u(0xFFFFFFFFFFFFFFl, buffer, 7));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    // 8 bytes
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    expected[0] = 0x01;
    EXPECT_EQ(8, tide::ebml_int::encode_u(0x0100000000000000l, buffer, 8));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x01; expected[7] = 0x01;
    EXPECT_EQ(8, tide::ebml_int::encode_u(0x0100000000000001l, buffer, 8));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    memset(expected, 0xFF, 8);
    expected[0] = 0x7F;
    EXPECT_EQ(8, tide::ebml_int::encode_u(0x7FFFFFFFFFFFFFFFl, buffer, 8));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0xFF;
    EXPECT_EQ(8, tide::ebml_int::encode_u(0xFFFFFFFFFFFFFFFFl, buffer, 8));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    // Test that buffer size is not dictating the outcome
    expected[0] = 0x00;
    EXPECT_EQ(1, tide::ebml_int::encode_u(0x00, buffer, 8));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x01;
    EXPECT_EQ(1, tide::ebml_int::encode_u(0x01, buffer, 8));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
}


TEST(EBMLInt, EncodeSigned)
{
    uint8_t buffer[8], expected[8];
    memset(buffer, 0, sizeof(buffer));
    memset(expected, 0, sizeof(expected));
    // 1 byte
    expected[0] = 0xFF;
    EXPECT_EQ(1, tide::ebml_int::encode_u(-1, buffer, 1));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x00;
    EXPECT_EQ(1, tide::ebml_int::encode_u(0, buffer, 1));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x01;
    EXPECT_EQ(1, tide::ebml_int::encode_u(1, buffer, 1));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x80;
    EXPECT_EQ(1, tide::ebml_int::encode_u(-0x80, buffer, 1));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x7F;
    EXPECT_EQ(1, tide::ebml_int::encode_u(0x7F, buffer, 1));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    // 2 bytes
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    expected[0] = 0x01; expected[1] = 0x00;
    EXPECT_EQ(2, tide::ebml_int::encode_u(-0x81, buffer, 2));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x80; expected[1] = 0x00;
    EXPECT_EQ(2, tide::ebml_int::encode_u(0x0101, buffer, 2));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x7F; expected[1] = 0xFF;
    EXPECT_EQ(2, tide::ebml_int::encode_u(0x7FFF, buffer, 2));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0xFF; expected[1] = 0xFF;
    EXPECT_EQ(2, tide::ebml_int::encode_u(0xFFFF, buffer, 2));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    // 3 bytes
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    expected[0] = 0x01;
    EXPECT_EQ(3, tide::ebml_int::encode_u(0x010000, buffer, 3));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x01; expected[2] = 0x01;
    EXPECT_EQ(3, tide::ebml_int::encode_u(0x010001, buffer, 3));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x7F; expected[1] = 0xFF; expected[2] = 0xFF;
    EXPECT_EQ(3, tide::ebml_int::encode_u(0x7FFFFF, buffer, 3));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0xFF;
    EXPECT_EQ(3, tide::ebml_int::encode_u(0xFFFFFF, buffer, 3));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    // 4 bytes
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    expected[0] = 0x01;
    EXPECT_EQ(4, tide::ebml_int::encode_u(0x01000000, buffer, 4));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x01; expected[3] = 0x01;
    EXPECT_EQ(4, tide::ebml_int::encode_u(0x01000001, buffer, 4));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    memset(expected, 0xFF, 4);
    expected[0] = 0x7F;
    EXPECT_EQ(4, tide::ebml_int::encode_u(0x7FFFFFFF, buffer, 4));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0xFF;
    EXPECT_EQ(4, tide::ebml_int::encode_u(0xFFFFFFFF, buffer, 4));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    // 5 bytes
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    expected[0] = 0x01;
    EXPECT_EQ(5, tide::ebml_int::encode_u(0x0100000000l, buffer, 5));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x01; expected[4] = 0x01;
    EXPECT_EQ(5, tide::ebml_int::encode_u(0x0100000001l, buffer, 5));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    memset(expected, 0xFF, 5);
    expected[0] = 0x7F;
    EXPECT_EQ(5, tide::ebml_int::encode_u(0x7FFFFFFFFFl, buffer, 5));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0xFF;
    EXPECT_EQ(5, tide::ebml_int::encode_u(0xFFFFFFFFFFl, buffer, 5));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    // 6 bytes
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    expected[0] = 0x01;
    EXPECT_EQ(6, tide::ebml_int::encode_u(0x010000000000l, buffer, 6));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x01; expected[5] = 0x01;
    EXPECT_EQ(6, tide::ebml_int::encode_u(0x010000000001l, buffer, 6));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    memset(expected, 0xFF, 6);
    expected[0] = 0x7F;
    EXPECT_EQ(6, tide::ebml_int::encode_u(0x7FFFFFFFFFFFl, buffer, 6));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0xFF;
    EXPECT_EQ(6, tide::ebml_int::encode_u(0xFFFFFFFFFFFFl, buffer, 6));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    // 7 bytes
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    expected[0] = 0x01;
    EXPECT_EQ(7, tide::ebml_int::encode_u(0x01000000000000l, buffer, 7));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x01; expected[6] = 0x01;
    EXPECT_EQ(7, tide::ebml_int::encode_u(0x01000000000001l, buffer, 7));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    memset(expected, 0xFF, 7);
    expected[0] = 0x7F;
    EXPECT_EQ(7, tide::ebml_int::encode_u(0x7FFFFFFFFFFFFFl, buffer, 7));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0xFF;
    EXPECT_EQ(7, tide::ebml_int::encode_u(0xFFFFFFFFFFFFFFl, buffer, 7));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    // 8 bytes
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    expected[0] = 0x01;
    EXPECT_EQ(8, tide::ebml_int::encode_u(0x0100000000000000l, buffer, 8));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x01; expected[7] = 0x01;
    EXPECT_EQ(8, tide::ebml_int::encode_u(0x0100000000000001l, buffer, 8));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    memset(expected, 0xFF, 8);
    expected[0] = 0x7F;
    EXPECT_EQ(8, tide::ebml_int::encode_u(0x7FFFFFFFFFFFFFFFl, buffer, 8));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0xFF;
    EXPECT_EQ(8, tide::ebml_int::encode_u(0xFFFFFFFFFFFFFFFFl, buffer, 8));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    // Test that buffer size is not dictating the outcome
    expected[0] = 0x00;
    EXPECT_EQ(1, tide::ebml_int::encode_u(0x00, buffer, 8));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x01;
    EXPECT_EQ(1, tide::ebml_int::encode_u(0x01, buffer, 8));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
}


TEST(EBMLInt, DecodeUnsigned)
{
}


TEST(EBMLInt, DecodeSigned)
{
}


TEST(EBMLInt, EncodeDecodeUnsigned)
{
}


TEST(EBMLInt, EncodeDecodeSigned)
{
}

}; // namespace ebml_int
}; // namespace tide

