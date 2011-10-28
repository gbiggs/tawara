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
    EXPECT_EQ(1, tide::ebml_int::encode_s(-1, buffer, 1));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x00;
    EXPECT_EQ(1, tide::ebml_int::encode_s(0, buffer, 1));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x01;
    EXPECT_EQ(1, tide::ebml_int::encode_s(1, buffer, 1));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x80;
    EXPECT_EQ(1, tide::ebml_int::encode_s(-0x80, buffer, 1));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x7F;
    EXPECT_EQ(1, tide::ebml_int::encode_s(0x7F, buffer, 1));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    // 2 bytes
    memset(buffer, 0, sizeof(buffer));
    memset(expected, 0, sizeof(expected));
    expected[0] = 0xFF; expected[1] = 0x7F;
    EXPECT_EQ(2, tide::ebml_int::encode_s(-0x81, buffer, 2));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x00; expected[1] = 0x80;
    EXPECT_EQ(2, tide::ebml_int::encode_s(0x80, buffer, 2));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x80; expected[1] = 0x00;
    EXPECT_EQ(2, tide::ebml_int::encode_s(-0x8000, buffer, 2));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x7F; expected[1] = 0xFF;
    EXPECT_EQ(2, tide::ebml_int::encode_s(0x7FFF, buffer, 2));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    // 3 bytes
    memset(buffer, 0, sizeof(buffer));
    memset(expected, 0, sizeof(expected));
    memset(expected, 0xFF, 3);
    expected[1] = 0x7F;
    EXPECT_EQ(3, tide::ebml_int::encode_s(-0x8001, buffer, 3));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    memset(expected, 0, sizeof(expected));
    expected[1] = 0x80;
    EXPECT_EQ(3, tide::ebml_int::encode_s(0x8000, buffer, 3));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    memset(expected, 0, sizeof(expected));
    expected[0] = 0x80;
    EXPECT_EQ(3, tide::ebml_int::encode_s(-0x800000, buffer, 3));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    memset(expected, 0xFF, 3);
    expected[0] = 0x7F;
    EXPECT_EQ(3, tide::ebml_int::encode_s(0x7FFFFF, buffer, 3));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    // 4 bytes
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    memset(expected, 0xFF, 4);
    expected[1] = 0x7F;
    EXPECT_EQ(4, tide::ebml_int::encode_s(-0x800001, buffer, 4));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    memset(expected, 0, sizeof(expected));
    expected[1] = 0x80;
    EXPECT_EQ(4, tide::ebml_int::encode_s(0x800000, buffer, 4));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    memset(expected, 0, sizeof(expected));
    expected[0] = 0x80;
    EXPECT_EQ(4, tide::ebml_int::encode_s(-0x80000000l, buffer, 4));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    memset(expected, 0xFF, 4);
    expected[0] = 0x7F;
    EXPECT_EQ(4, tide::ebml_int::encode_s(0x7FFFFFFFl, buffer, 4));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    // 5 bytes
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    memset(expected, 0xFF, 5);
    expected[1] = 0x7F;
    EXPECT_EQ(5, tide::ebml_int::encode_s(-0x80000001l, buffer, 5));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    memset(expected, 0, sizeof(expected));
    expected[1] = 0x80;
    EXPECT_EQ(5, tide::ebml_int::encode_s(0x80000000l, buffer, 5));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    memset(expected, 0, sizeof(expected));
    expected[0] = 0x80;
    EXPECT_EQ(5, tide::ebml_int::encode_s(-0x8000000000l, buffer, 5));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    memset(expected, 0xFF, 5);
    expected[0] = 0x7F;
    EXPECT_EQ(5, tide::ebml_int::encode_s(0x7FFFFFFFFFl, buffer, 5));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    // 6 bytes
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    memset(expected, 0xFF, 6);
    expected[1] = 0x7F;
    EXPECT_EQ(6, tide::ebml_int::encode_s(-0x8000000001, buffer, 6));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    memset(expected, 0, sizeof(expected));
    expected[1] = 0x80;
    EXPECT_EQ(6, tide::ebml_int::encode_s(0x8000000000, buffer, 6));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    memset(expected, 0, sizeof(expected));
    expected[0] = 0x80;
    EXPECT_EQ(6, tide::ebml_int::encode_s(-0x800000000000l, buffer, 6));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    memset(expected, 0xFF, 6);
    expected[0] = 0x7F;
    EXPECT_EQ(6, tide::ebml_int::encode_s(0x7FFFFFFFFFFFl, buffer, 6));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    // 7 bytes
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    memset(expected, 0xFF, 7);
    expected[1] = 0x7F;
    EXPECT_EQ(7, tide::ebml_int::encode_s(-0x800000000001, buffer, 7));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    memset(expected, 0, sizeof(expected));
    expected[1] = 0x80;
    EXPECT_EQ(7, tide::ebml_int::encode_s(0x800000000000, buffer, 7));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    memset(expected, 0, sizeof(expected));
    expected[0] = 0x80;
    EXPECT_EQ(7, tide::ebml_int::encode_s(-0x80000000000000l, buffer, 7));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    memset(expected, 0xFF, 7);
    expected[0] = 0x7F;
    EXPECT_EQ(7, tide::ebml_int::encode_s(0x7FFFFFFFFFFFFFl, buffer, 7));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    // 8 bytes
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    memset(expected, 0xFF, 8);
    expected[1] = 0x7F;
    EXPECT_EQ(8, tide::ebml_int::encode_s(-0x80000000000001, buffer, 8));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    memset(expected, 0, sizeof(expected));
    expected[1] = 0x80;
    EXPECT_EQ(8, tide::ebml_int::encode_s(0x80000000000000, buffer, 8));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    memset(expected, 0, sizeof(expected));
    expected[0] = 0x80;
    EXPECT_EQ(8, tide::ebml_int::encode_s(-0x8000000000000000l, buffer, 8));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
    memset(expected, 0xFF, 8);
    expected[0] = 0x7F;
    EXPECT_EQ(8, tide::ebml_int::encode_s(0x7FFFFFFFFFFFFFFFl, buffer, 8));
    EXPECT_PRED_FORMAT3(buffers_eq, expected, buffer, 8);
}


TEST(EBMLInt, DecodeUnsigned)
{
    uint8_t buffer[8];
    // 1 byte
    memset(buffer, 0x00, sizeof(buffer));
    EXPECT_EQ(0, tide::ebml_int::decode_u(buffer, 1));
    buffer[0] = 0x01;
    EXPECT_EQ(1, tide::ebml_int::decode_u(buffer, 1));
    buffer[0] = 0x7F;
    EXPECT_EQ(0x7F, tide::ebml_int::decode_u(buffer, 1));
    buffer[0] = 0xFF;
    EXPECT_EQ(0xFF, tide::ebml_int::decode_u(buffer, 1));
    // 2 bytes
    memset(buffer, 0x00, sizeof(buffer));
    buffer[0] = 0x01;
    EXPECT_EQ(0x0100, tide::ebml_int::decode_u(buffer, 2));
    buffer[0] = 0x01; buffer[1] = 0x01;
    EXPECT_EQ(0x0101, tide::ebml_int::decode_u(buffer, 2));
    buffer[0] = 0x7F; buffer[1] = 0xFF;
    EXPECT_EQ(0x7FFF, tide::ebml_int::decode_u(buffer, 2));
    buffer[0] = 0xFF; buffer[1] = 0xFF;
    EXPECT_EQ(0xFFFF, tide::ebml_int::decode_u(buffer, 2));
    // 3 bytes
    memset(buffer, 0x00, sizeof(buffer));
    buffer[0] = 0x01;
    EXPECT_EQ(0x010000, tide::ebml_int::decode_u(buffer, 3));
    buffer[0] = 0x01; buffer[2] = 0x01;
    EXPECT_EQ(0x010001, tide::ebml_int::decode_u(buffer, 3));
    memset(buffer, 0xFF, 3);
    buffer[0] = 0x7F;
    EXPECT_EQ(0x7FFFFF, tide::ebml_int::decode_u(buffer, 3));
    buffer[0] = 0xFF;
    EXPECT_EQ(0xFFFFFF, tide::ebml_int::decode_u(buffer, 3));
    // 4 bytes
    memset(buffer, 0x00, sizeof(buffer));
    buffer[0] = 0x01;
    EXPECT_EQ(0x01000000, tide::ebml_int::decode_u(buffer, 4));
    buffer[0] = 0x01; buffer[3] = 0x01;
    EXPECT_EQ(0x01000001, tide::ebml_int::decode_u(buffer, 4));
    memset(buffer, 0xFF, 4);
    buffer[0] = 0x7F;
    EXPECT_EQ(0x7FFFFFFF, tide::ebml_int::decode_u(buffer, 4));
    buffer[0] = 0xFF;
    EXPECT_EQ(0xFFFFFFFF, tide::ebml_int::decode_u(buffer, 4));
    // 5 bytes
    memset(buffer, 0x00, sizeof(buffer));
    buffer[0] = 0x01;
    EXPECT_EQ(0x0100000000, tide::ebml_int::decode_u(buffer, 5));
    buffer[0] = 0x01; buffer[4] = 0x01;
    EXPECT_EQ(0x0100000001, tide::ebml_int::decode_u(buffer, 5));
    memset(buffer, 0xFF, 5);
    buffer[0] = 0x7F;
    EXPECT_EQ(0x7FFFFFFFFF, tide::ebml_int::decode_u(buffer, 5));
    buffer[0] = 0xFF;
    EXPECT_EQ(0xFFFFFFFFFF, tide::ebml_int::decode_u(buffer, 5));
    // 6 bytes
    memset(buffer, 0x00, sizeof(buffer));
    buffer[0] = 0x01;
    EXPECT_EQ(0x010000000000, tide::ebml_int::decode_u(buffer, 6));
    buffer[0] = 0x01; buffer[5] = 0x01;
    EXPECT_EQ(0x010000000001, tide::ebml_int::decode_u(buffer, 6));
    memset(buffer, 0xFF, 6);
    buffer[0] = 0x7F;
    EXPECT_EQ(0x7FFFFFFFFFFF, tide::ebml_int::decode_u(buffer, 6));
    buffer[0] = 0xFF;
    EXPECT_EQ(0xFFFFFFFFFFFF, tide::ebml_int::decode_u(buffer, 6));
    // 7 bytes
    memset(buffer, 0x00, sizeof(buffer));
    buffer[0] = 0x01;
    EXPECT_EQ(0x01000000000000, tide::ebml_int::decode_u(buffer, 7));
    buffer[0] = 0x01; buffer[6] = 0x01;
    EXPECT_EQ(0x01000000000001, tide::ebml_int::decode_u(buffer, 7));
    memset(buffer, 0xFF, 7);
    buffer[0] = 0x7F;
    EXPECT_EQ(0x7FFFFFFFFFFFFF, tide::ebml_int::decode_u(buffer, 7));
    buffer[0] = 0xFF;
    EXPECT_EQ(0xFFFFFFFFFFFFFF, tide::ebml_int::decode_u(buffer, 7));
    // 8 bytes
    memset(buffer, 0x00, sizeof(buffer));
    buffer[0] = 0x01;
    EXPECT_EQ(0x0100000000000000, tide::ebml_int::decode_u(buffer, 8));
    buffer[0] = 0x01; buffer[7] = 0x01;
    EXPECT_EQ(0x0100000000000001, tide::ebml_int::decode_u(buffer, 8));
    memset(buffer, 0xFF, 8);
    buffer[0] = 0x7F;
    EXPECT_EQ(0x7FFFFFFFFFFFFFFF, tide::ebml_int::decode_u(buffer, 8));
    buffer[0] = 0xFF;
    EXPECT_EQ(0xFFFFFFFFFFFFFFFF, tide::ebml_int::decode_u(buffer, 8));
}


TEST(EBMLInt, DecodeSigned)
{
    uint8_t buffer[8];
    // 1 byte
    memset(buffer, 0x00, sizeof(buffer));
    buffer[0] = 0x80;
    EXPECT_EQ(-0x80, tide::ebml_int::decode_s(buffer, 1));
    buffer[0] = 0xFF;
    EXPECT_EQ(-1, tide::ebml_int::decode_s(buffer, 1));
    buffer[0] = 0x00;
    EXPECT_EQ(0, tide::ebml_int::decode_s(buffer, 1));
    buffer[0] = 0x7F;
    EXPECT_EQ(0x7F, tide::ebml_int::decode_s(buffer, 1));
    // 2 bytes
    memset(buffer, 0x00, sizeof(buffer));
    memset(buffer, 0xFF, 2);
    buffer[1] = 0x7F;
    EXPECT_EQ(-0x81, tide::ebml_int::decode_s(buffer, 2));
    memset(buffer, 0x00, sizeof(buffer));
    buffer[1] = 0x80;
    EXPECT_EQ(0x80, tide::ebml_int::decode_s(buffer, 2));
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = 0x80;
    EXPECT_EQ(-0x8000, tide::ebml_int::decode_s(buffer, 2));
    memset(buffer, 0xFF, 2);
    buffer[0] = 0x7F;
    EXPECT_EQ(0x7FFF, tide::ebml_int::decode_s(buffer, 2));
    // 3 bytes
    memset(buffer, 0x00, sizeof(buffer));
    memset(buffer, 0xFF, 3);
    buffer[1] = 0x7F;
    EXPECT_EQ(-0x8001, tide::ebml_int::decode_s(buffer, 3));
    memset(buffer, 0x00, sizeof(buffer));
    buffer[1] = 0x80;
    EXPECT_EQ(0x8000, tide::ebml_int::decode_s(buffer, 3));
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = 0x80;
    EXPECT_EQ(-0x800000, tide::ebml_int::decode_s(buffer, 3));
    memset(buffer, 0xFF, 3);
    buffer[0] = 0x7F;
    EXPECT_EQ(0x7FFFFF, tide::ebml_int::decode_s(buffer, 3));
    // 4 bytes
    memset(buffer, 0x00, sizeof(buffer));
    memset(buffer, 0xFF, 4);
    buffer[1] = 0x7F;
    EXPECT_EQ(-0x800001, tide::ebml_int::decode_s(buffer, 4));
    memset(buffer, 0x00, sizeof(buffer));
    buffer[1] = 0x80;
    EXPECT_EQ(0x800000, tide::ebml_int::decode_s(buffer, 4));
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = 0x80;
    EXPECT_EQ(-0x80000000l, tide::ebml_int::decode_s(buffer, 4));
    memset(buffer, 0xFF, 4);
    buffer[0] = 0x7F;
    EXPECT_EQ(0x7FFFFFFFl, tide::ebml_int::decode_s(buffer, 4));
    // 5 bytes
    memset(buffer, 0x00, sizeof(buffer));
    memset(buffer, 0xFF, 5);
    buffer[1] = 0x7F;
    EXPECT_EQ(-0x80000001l, tide::ebml_int::decode_s(buffer, 5));
    memset(buffer, 0x00, sizeof(buffer));
    buffer[1] = 0x80;
    EXPECT_EQ(0x80000000l, tide::ebml_int::decode_s(buffer, 5));
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = 0x80;
    EXPECT_EQ(-0x8000000000l, tide::ebml_int::decode_s(buffer, 5));
    memset(buffer, 0xFF, 5);
    buffer[0] = 0x7F;
    EXPECT_EQ(0x7FFFFFFFFFl, tide::ebml_int::decode_s(buffer, 5));
    // 6 bytes
    memset(buffer, 0x00, sizeof(buffer));
    memset(buffer, 0xFF, 6);
    buffer[1] = 0x7F;
    EXPECT_EQ(-0x8000000001l, tide::ebml_int::decode_s(buffer, 6));
    memset(buffer, 0x00, sizeof(buffer));
    buffer[1] = 0x80;
    EXPECT_EQ(0x8000000000l, tide::ebml_int::decode_s(buffer, 6));
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = 0x80;
    EXPECT_EQ(-0x800000000000l, tide::ebml_int::decode_s(buffer, 6));
    memset(buffer, 0xFF, 6);
    buffer[0] = 0x7F;
    EXPECT_EQ(0x7FFFFFFFFFFFl, tide::ebml_int::decode_s(buffer, 6));
    // 7 bytes
    memset(buffer, 0x00, sizeof(buffer));
    memset(buffer, 0xFF, 7);
    buffer[1] = 0x7F;
    EXPECT_EQ(-0x800000000001l, tide::ebml_int::decode_s(buffer, 7));
    memset(buffer, 0x00, sizeof(buffer));
    buffer[1] = 0x80;
    EXPECT_EQ(0x800000000000l, tide::ebml_int::decode_s(buffer, 7));
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = 0x80;
    EXPECT_EQ(-0x80000000000000l, tide::ebml_int::decode_s(buffer, 7));
    memset(buffer, 0xFF, 7);
    buffer[0] = 0x7F;
    EXPECT_EQ(0x7FFFFFFFFFFFFFl, tide::ebml_int::decode_s(buffer, 7));
    // 8 bytes
    memset(buffer, 0x00, sizeof(buffer));
    memset(buffer, 0xFF, 8);
    buffer[1] = 0x7F;
    EXPECT_EQ(-0x80000000000001l, tide::ebml_int::decode_s(buffer, 8));
    memset(buffer, 0x00, sizeof(buffer));
    buffer[1] = 0x80;
    EXPECT_EQ(0x80000000000000l, tide::ebml_int::decode_s(buffer, 8));
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = 0x80;
    EXPECT_EQ(-0x8000000000000000l, tide::ebml_int::decode_s(buffer, 8));
    memset(buffer, 0xFF, 8);
    buffer[0] = 0x7F;
    EXPECT_EQ(0x7FFFFFFFFFFFFFFFl, tide::ebml_int::decode_s(buffer, 8));
}


TEST(EBMLInt, EncodeDecodeUnsigned)
{
    uint8_t buffer[8];
    memset(buffer, 0, sizeof(8));
    // 1 byte
    tide::ebml_int::encode_u(0, buffer, 1);
    EXPECT_EQ(0, tide::ebml_int::decode_u(buffer, 1));
    tide::ebml_int::encode_u(1, buffer, 1);
    EXPECT_EQ(1, tide::ebml_int::decode_u(buffer, 1));
    tide::ebml_int::encode_u(0x7F, buffer, 1);
    EXPECT_EQ(0x7F, tide::ebml_int::decode_u(buffer, 1));
    tide::ebml_int::encode_u(0xFF, buffer, 1);
    EXPECT_EQ(0xFF, tide::ebml_int::decode_u(buffer, 1));
    // 2 bytes
    tide::ebml_int::encode_u(0x100, buffer, 2);
    EXPECT_EQ(0x100, tide::ebml_int::decode_u(buffer, 2));
    tide::ebml_int::encode_u(0x101, buffer, 2);
    EXPECT_EQ(0x101, tide::ebml_int::decode_u(buffer, 2));
    tide::ebml_int::encode_u(0x7FFF, buffer, 2);
    EXPECT_EQ(0x7FFF, tide::ebml_int::decode_u(buffer, 2));
    tide::ebml_int::encode_u(0xFFFF, buffer, 2);
    EXPECT_EQ(0xFFFF, tide::ebml_int::decode_u(buffer, 2));
    // 3 bytes
    tide::ebml_int::encode_u(0x10000, buffer, 3);
    EXPECT_EQ(0x10000, tide::ebml_int::decode_u(buffer, 3));
    tide::ebml_int::encode_u(0x10001, buffer, 3);
    EXPECT_EQ(0x10001, tide::ebml_int::decode_u(buffer, 3));
    tide::ebml_int::encode_u(0x7FFFFF, buffer, 3);
    EXPECT_EQ(0x7FFFFF, tide::ebml_int::decode_u(buffer, 3));
    tide::ebml_int::encode_u(0xFFFFFF, buffer, 3);
    EXPECT_EQ(0xFFFFFF, tide::ebml_int::decode_u(buffer, 3));
    // 4 bytes
    tide::ebml_int::encode_u(0x1000000l, buffer, 4);
    EXPECT_EQ(0x1000000l, tide::ebml_int::decode_u(buffer, 4));
    tide::ebml_int::encode_u(0x1000001l, buffer, 4);
    EXPECT_EQ(0x1000001l, tide::ebml_int::decode_u(buffer, 4));
    tide::ebml_int::encode_u(0x7FFFFFFFl, buffer, 4);
    EXPECT_EQ(0x7FFFFFFFl, tide::ebml_int::decode_u(buffer, 4));
    tide::ebml_int::encode_u(0xFFFFFFFFl, buffer, 4);
    EXPECT_EQ(0xFFFFFFFFl, tide::ebml_int::decode_u(buffer, 4));
    // 5 bytes
    tide::ebml_int::encode_u(0x100000000l, buffer, 5);
    EXPECT_EQ(0x100000000l, tide::ebml_int::decode_u(buffer, 5));
    tide::ebml_int::encode_u(0x100000001l, buffer, 5);
    EXPECT_EQ(0x100000001l, tide::ebml_int::decode_u(buffer, 5));
    tide::ebml_int::encode_u(0x7FFFFFFFFFl, buffer, 5);
    EXPECT_EQ(0x7FFFFFFFFFl, tide::ebml_int::decode_u(buffer, 5));
    tide::ebml_int::encode_u(0xFFFFFFFFFFl, buffer, 5);
    EXPECT_EQ(0xFFFFFFFFFFl, tide::ebml_int::decode_u(buffer, 5));
    // 6 bytes
    tide::ebml_int::encode_u(0x10000000000l, buffer, 6);
    EXPECT_EQ(0x10000000000l, tide::ebml_int::decode_u(buffer, 6));
    tide::ebml_int::encode_u(0x10000000001l, buffer, 6);
    EXPECT_EQ(0x10000000001l, tide::ebml_int::decode_u(buffer, 6));
    tide::ebml_int::encode_u(0x7FFFFFFFFFFFl, buffer, 6);
    EXPECT_EQ(0x7FFFFFFFFFFFl, tide::ebml_int::decode_u(buffer, 6));
    tide::ebml_int::encode_u(0xFFFFFFFFFFFFl, buffer, 6);
    EXPECT_EQ(0xFFFFFFFFFFFFl, tide::ebml_int::decode_u(buffer, 6));
    // 7 bytes
    tide::ebml_int::encode_u(0x1000000000000l, buffer, 7);
    EXPECT_EQ(0x1000000000000l, tide::ebml_int::decode_u(buffer, 7));
    tide::ebml_int::encode_u(0x1000000000001l, buffer, 7);
    EXPECT_EQ(0x1000000000001l, tide::ebml_int::decode_u(buffer, 7));
    tide::ebml_int::encode_u(0x7FFFFFFFFFFFFFl, buffer, 7);
    EXPECT_EQ(0x7FFFFFFFFFFFFFl, tide::ebml_int::decode_u(buffer, 7));
    tide::ebml_int::encode_u(0xFFFFFFFFFFFFFFl, buffer, 7);
    EXPECT_EQ(0xFFFFFFFFFFFFFFl, tide::ebml_int::decode_u(buffer, 7));
    // 8 bytes
    tide::ebml_int::encode_u(0x100000000000000l, buffer, 8);
    EXPECT_EQ(0x100000000000000l, tide::ebml_int::decode_u(buffer, 8));
    tide::ebml_int::encode_u(0x100000000000001l, buffer, 8);
    EXPECT_EQ(0x100000000000001l, tide::ebml_int::decode_u(buffer, 8));
    tide::ebml_int::encode_u(0x7FFFFFFFFFFFFFFFl, buffer, 8);
    EXPECT_EQ(0x7FFFFFFFFFFFFFFFl, tide::ebml_int::decode_u(buffer, 8));
    tide::ebml_int::encode_u(0xFFFFFFFFFFFFFFFFl, buffer, 8);
    EXPECT_EQ(0xFFFFFFFFFFFFFFFFl, tide::ebml_int::decode_u(buffer, 8));
}


TEST(EBMLInt, EncodeDecodeSigned)
{
    uint8_t buffer[8];
    memset(buffer, 0, sizeof(8));
    // 1 byte
    tide::ebml_int::encode_s(-0x80, buffer, 1);
    EXPECT_EQ(-0x80, tide::ebml_int::decode_s(buffer, 1));
    tide::ebml_int::encode_s(-1, buffer, 1);
    EXPECT_EQ(-1, tide::ebml_int::decode_s(buffer, 1));
    tide::ebml_int::encode_s(0, buffer, 1);
    EXPECT_EQ(0, tide::ebml_int::decode_s(buffer, 1));
    tide::ebml_int::encode_s(0x7F, buffer, 1);
    EXPECT_EQ(0x7F, tide::ebml_int::decode_s(buffer, 1));
    // 2 bytes
    tide::ebml_int::encode_s(-0x81, buffer, 2);
    EXPECT_EQ(-0x81, tide::ebml_int::decode_s(buffer, 2));
    tide::ebml_int::encode_s(0x80, buffer, 2);
    EXPECT_EQ(0x80, tide::ebml_int::decode_s(buffer, 2));
    tide::ebml_int::encode_s(-0x8000, buffer, 2);
    EXPECT_EQ(-0x8000, tide::ebml_int::decode_s(buffer, 2));
    tide::ebml_int::encode_s(0x7FFF, buffer, 2);
    EXPECT_EQ(0x7FFF, tide::ebml_int::decode_s(buffer, 2));
    // 3 bytes
    tide::ebml_int::encode_s(-0x8001, buffer, 3);
    EXPECT_EQ(-0x8001, tide::ebml_int::decode_s(buffer, 3));
    tide::ebml_int::encode_s(0x8000, buffer, 3);
    EXPECT_EQ(0x8000, tide::ebml_int::decode_s(buffer, 3));
    tide::ebml_int::encode_s(-0x800000, buffer, 3);
    EXPECT_EQ(-0x800000, tide::ebml_int::decode_s(buffer, 3));
    tide::ebml_int::encode_s(0x7FFFFF, buffer, 3);
    EXPECT_EQ(0x7FFFFF, tide::ebml_int::decode_s(buffer, 3));
    // 4 bytes
    tide::ebml_int::encode_s(-0x800001l, buffer, 4);
    EXPECT_EQ(-0x800001l, tide::ebml_int::decode_s(buffer, 4));
    tide::ebml_int::encode_s(0x800000l, buffer, 4);
    EXPECT_EQ(0x800000l, tide::ebml_int::decode_s(buffer, 4));
    tide::ebml_int::encode_s(-0x80000000l, buffer, 4);
    EXPECT_EQ(-0x80000000l, tide::ebml_int::decode_s(buffer, 4));
    tide::ebml_int::encode_s(0x7FFFFFFFl, buffer, 4);
    EXPECT_EQ(0x7FFFFFFFl, tide::ebml_int::decode_s(buffer, 4));
    // 5 bytes
    tide::ebml_int::encode_s(-0x80000001l, buffer, 5);
    EXPECT_EQ(-0x80000001l, tide::ebml_int::decode_s(buffer, 5));
    tide::ebml_int::encode_s(0x80000000l, buffer, 5);
    EXPECT_EQ(0x80000000l, tide::ebml_int::decode_s(buffer, 5));
    tide::ebml_int::encode_s(-0x8000000000l, buffer, 5);
    EXPECT_EQ(-0x8000000000l, tide::ebml_int::decode_s(buffer, 5));
    tide::ebml_int::encode_s(0x7FFFFFFFFFl, buffer, 5);
    EXPECT_EQ(0x7FFFFFFFFFl, tide::ebml_int::decode_s(buffer, 5));
    // 6 bytes
    tide::ebml_int::encode_s(-0x8000000001l, buffer, 6);
    EXPECT_EQ(-0x8000000001l, tide::ebml_int::decode_s(buffer, 6));
    tide::ebml_int::encode_s(0x8000000000l, buffer, 6);
    EXPECT_EQ(0x8000000000l, tide::ebml_int::decode_s(buffer, 6));
    tide::ebml_int::encode_s(-0x800000000000l, buffer, 6);
    EXPECT_EQ(-0x800000000000l, tide::ebml_int::decode_s(buffer, 6));
    tide::ebml_int::encode_s(0x7FFFFFFFFFFFl, buffer, 6);
    EXPECT_EQ(0x7FFFFFFFFFFFl, tide::ebml_int::decode_s(buffer, 6));
    // 7 bytes
    tide::ebml_int::encode_s(-0x800000000001l, buffer, 7);
    EXPECT_EQ(-0x800000000001l, tide::ebml_int::decode_s(buffer, 7));
    tide::ebml_int::encode_s(0x800000000000l, buffer, 7);
    EXPECT_EQ(0x800000000000l, tide::ebml_int::decode_s(buffer, 7));
    tide::ebml_int::encode_s(-0x80000000000000l, buffer, 7);
    EXPECT_EQ(-0x80000000000000l, tide::ebml_int::decode_s(buffer, 7));
    tide::ebml_int::encode_s(0x7FFFFFFFFFFFFFl, buffer, 7);
    EXPECT_EQ(0x7FFFFFFFFFFFFFl, tide::ebml_int::decode_s(buffer, 7));
    // 8 bytes
    tide::ebml_int::encode_s(-0x80000000000001l, buffer, 8);
    EXPECT_EQ(-0x80000000000001l, tide::ebml_int::decode_s(buffer, 8));
    tide::ebml_int::encode_s(0x80000000000000l, buffer, 8);
    EXPECT_EQ(0x80000000000000l, tide::ebml_int::decode_s(buffer, 8));
    tide::ebml_int::encode_s(-0x8000000000000000l, buffer, 8);
    EXPECT_EQ(-0x8000000000000000l, tide::ebml_int::decode_s(buffer, 8));
    tide::ebml_int::encode_s(0x7FFFFFFFFFFFFFFFl, buffer, 8);
    EXPECT_EQ(0x7FFFFFFFFFFFFFFFl, tide::ebml_int::decode_s(buffer, 8));
}


TEST(EBMLInt, BufferTooSmall)
{
    // 1 byte unsigned
    EXPECT_THROW(encode_u(0x100, 0, 1), tide::BufferTooSmall);
    // 2 bytes unsigned
    EXPECT_THROW(encode_u(0x10000, 0, 2), tide::BufferTooSmall);
    // 3 bytes unsigned
    EXPECT_THROW(encode_u(0x1000000, 0, 3), tide::BufferTooSmall);
    // 4 bytes unsigned
    EXPECT_THROW(encode_u(0x100000000l, 0, 4), tide::BufferTooSmall);
    // 5 bytes unsigned
    EXPECT_THROW(encode_u(0x10000000000l, 0, 5), tide::BufferTooSmall);
    // 6 bytes unsigned
    EXPECT_THROW(encode_u(0x1000000000000l, 0, 6), tide::BufferTooSmall);
    // 7 bytes unsigned
    EXPECT_THROW(encode_u(0x100000000000000l, 0, 7), tide::BufferTooSmall);
    // 8 byte values always fit

    // 1 byte signed
    EXPECT_THROW(encode_s(-0x81, 0, 1), tide::BufferTooSmall);
    EXPECT_THROW(encode_s(0x80, 0, 1), tide::BufferTooSmall);
    // 2 bytes signed
    EXPECT_THROW(encode_s(-0x8001, 0, 2), tide::BufferTooSmall);
    EXPECT_THROW(encode_s(0x8000, 0, 2), tide::BufferTooSmall);
    // 3 bytes signed
    EXPECT_THROW(encode_s(-0x800001, 0, 3), tide::BufferTooSmall);
    EXPECT_THROW(encode_s(0x800000, 0, 3), tide::BufferTooSmall);
    // 4 bytes signed
    EXPECT_THROW(encode_s(-0x80000001l, 0, 4), tide::BufferTooSmall);
    EXPECT_THROW(encode_s(0x80000000l, 0, 4), tide::BufferTooSmall);
    // 5 bytes signed
    EXPECT_THROW(encode_s(-0x8000000001l, 0, 5), tide::BufferTooSmall);
    EXPECT_THROW(encode_s(0x8000000000l, 0, 5), tide::BufferTooSmall);
    // 6 bytes signed
    EXPECT_THROW(encode_s(-0x800000000001l, 0, 6), tide::BufferTooSmall);
    EXPECT_THROW(encode_s(0x800000000000l, 0, 6), tide::BufferTooSmall);
    // 7 bytes signed
    EXPECT_THROW(encode_s(-0x80000000000001l, 0, 7), tide::BufferTooSmall);
    EXPECT_THROW(encode_s(0x80000000000000l, 0, 7), tide::BufferTooSmall);
    // 8 byte values always fit
}

}; // namespace ebml_int
}; // namespace tide

