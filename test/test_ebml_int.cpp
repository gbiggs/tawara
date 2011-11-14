/* Tide
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

#include <tide/ebml_int.h>

#include <gtest/gtest.h>
#include <tide/exceptions.h>

#include "test_consts.h"
#include "test_utils.h"


TEST(EBMLInt, CodedSizeUnsigned)
{
    // 0 bytes
    EXPECT_EQ(0, tide::ebml_int::coded_size_u(0x00));
    // 1 byte
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
    // 0 bytes
    EXPECT_EQ(0, tide::ebml_int::coded_size_s(0));
    // 1 byte
    EXPECT_EQ(1, tide::ebml_int::coded_size_s(-1));
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
    std::vector<char> expected;
    // 0 bytes
    // This is checking that the buffer is untouched for zero
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0x00));
    // 1 byte
    expected.assign(1, 0x01);
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0x01));
    expected[0] = 0x7F;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0x7F));
    expected[0] = 0xFF;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0xFF));
    // 2 bytes
    expected.assign(2, 0);
    expected[0] = 0x01;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0x0100));
    expected[0] = 0x01; expected[1] = 0x01;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0x0101));
    expected[0] = 0x7F; expected[1] = 0xFF;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0x7FFF));
    expected[0] = 0xFF; expected[1] = 0xFF;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0xFFFF));
    // 3 bytes
    expected.assign(3, 0);
    expected[0] = 0x01;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0x010000));
    expected[0] = 0x01; expected[2] = 0x01;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0x010001));
    expected[0] = 0x7F; expected[1] = 0xFF; expected[2] = 0xFF;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0x7FFFFF));
    expected[0] = 0xFF;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0xFFFFFF));
    // 4 bytes
    expected.assign(4, 0);
    expected[0] = 0x01;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0x01000000));
    expected[0] = 0x01; expected[3] = 0x01;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0x01000001));
    expected.assign(4, 0xFF);
    expected[0] = 0x7F;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0x7FFFFFFF));
    expected[0] = 0xFF;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0xFFFFFFFF));
    // 5 bytes
    expected.assign(5, 0);
    expected[0] = 0x01;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0x0100000000l));
    expected[0] = 0x01; expected[4] = 0x01;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0x0100000001l));
    expected.assign(5, 0xFF);
    expected[0] = 0x7F;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0x7FFFFFFFFFl));
    expected[0] = 0xFF;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0xFFFFFFFFFFl));
    // 6 bytes
    expected.assign(6, 0);
    expected[0] = 0x01;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0x010000000000l));
    expected[0] = 0x01; expected[5] = 0x01;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0x010000000001l));
    expected.assign(6, 0xFF);
    expected[0] = 0x7F;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0x7FFFFFFFFFFFl));
    expected[0] = 0xFF;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0xFFFFFFFFFFFFl));
    // 7 bytes
    expected.assign(7, 0);
    expected[0] = 0x01;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0x01000000000000l));
    expected[0] = 0x01; expected[6] = 0x01;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0x01000000000001l));
    expected.assign(7, 0xFF);
    expected[0] = 0x7F;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0x7FFFFFFFFFFFFFl));
    expected[0] = 0xFF;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0xFFFFFFFFFFFFFFl));
    // 8 bytes
    expected.assign(8, 0);
    expected[0] = 0x01;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0x0100000000000000l));
    expected[0] = 0x01; expected[7] = 0x01;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0x0100000000000001l));
    expected.assign(8, 0xFF);
    expected[0] = 0x7F;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0x7FFFFFFFFFFFFFFFl));
    expected[0] = 0xFF;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0xFFFFFFFFFFFFFFFFl));
}


TEST(EBMLInt, EncodeSigned)
{
    std::vector<char> expected;
    // 0 bytes
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_u(0));
    // 1 byte
    expected.assign(1, 0xFF);
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(-1));
    expected[0] = 0x01;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(1));
    expected[0] = 0x80;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(-0x80));
    expected[0] = 0x7F;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(0x7F));
    // 2 bytes
    expected.assign(2, 0);
    expected[0] = 0xFF; expected[1] = 0x7F;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(-0x81));
    expected[0] = 0x00; expected[1] = 0x80;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(0x80));
    expected[0] = 0x80; expected[1] = 0x00;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(-0x8000));
    expected[0] = 0x7F; expected[1] = 0xFF;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(0x7FFF));
    // 3 bytes
    expected.assign(3, 0xFF);
    expected[1] = 0x7F;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(-0x8001));
    expected.assign(3, 0);
    expected[1] = 0x80;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(0x8000));
    expected.assign(3, 0);
    expected[0] = 0x80;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(-0x800000));
    expected.assign(3, 0xFF);
    expected[0] = 0x7F;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(0x7FFFFF));
    // 4 bytes
    expected.assign(4, 0xFF);
    expected[1] = 0x7F;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(-0x800001));
    expected.assign(4, 0);
    expected[1] = 0x80;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(0x800000));
    expected.assign(4, 0);
    expected[0] = 0x80;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(-0x80000000l));
    expected.assign(4, 0xFF);
    expected[0] = 0x7F;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(0x7FFFFFFFl));
    // 5 bytes
    expected.assign(5, 0xFF);
    expected[1] = 0x7F;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(-0x80000001l));
    expected.assign(5, 0);
    expected[1] = 0x80;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(0x80000000l));
    expected.assign(5, 0);
    expected[0] = 0x80;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(-0x8000000000l));
    expected.assign(5, 0xFF);
    expected[0] = 0x7F;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(0x7FFFFFFFFFl));
    // 6 bytes
    expected.assign(6, 0xFF);
    expected[1] = 0x7F;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(-0x8000000001));
    expected.assign(6, 0);
    expected[1] = 0x80;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(0x8000000000));
    expected.assign(6, 0);
    expected[0] = 0x80;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(-0x800000000000l));
    expected.assign(6, 0xFF);
    expected[0] = 0x7F;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(0x7FFFFFFFFFFFl));
    // 7 bytes
    expected.assign(7, 0xFF);
    expected[1] = 0x7F;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(-0x800000000001));
    expected.assign(7, 0);
    expected[1] = 0x80;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(0x800000000000));
    expected.assign(7, 0);
    expected[0] = 0x80;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(-0x80000000000000l));
    expected.assign(7, 0xFF);
    expected[0] = 0x7F;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(0x7FFFFFFFFFFFFFl));
    // 8 bytes
    expected.assign(8, 0xFF);
    expected[1] = 0x7F;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(-0x80000000000001));
    expected.assign(8, 0);
    expected[1] = 0x80;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(0x80000000000000));
    expected.assign(8, 0);
    expected[0] = 0x80;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(-0x8000000000000000l));
    expected.assign(8, 0xFF);
    expected[0] = 0x7F;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ebml_int::encode_s(0x7FFFFFFFFFFFFFFFl));
}


TEST(EBMLInt, DecodeUnsigned)
{
    std::vector<char> buffer;
    // 0 bytes
    EXPECT_EQ(0, tide::ebml_int::decode_u(buffer));
    // 1 byte
    buffer.assign(1, 0x01);
    EXPECT_EQ(1, tide::ebml_int::decode_u(buffer));
    buffer[0] = 0x7F;
    EXPECT_EQ(0x7F, tide::ebml_int::decode_u(buffer));
    buffer[0] = 0xFF;
    EXPECT_EQ(0xFF, tide::ebml_int::decode_u(buffer));
    // 2 bytes
    buffer.assign(2, 0);
    buffer[0] = 0x01;
    EXPECT_EQ(0x0100, tide::ebml_int::decode_u(buffer));
    buffer[0] = 0x01; buffer[1] = 0x01;
    EXPECT_EQ(0x0101, tide::ebml_int::decode_u(buffer));
    buffer[0] = 0x7F; buffer[1] = 0xFF;
    EXPECT_EQ(0x7FFF, tide::ebml_int::decode_u(buffer));
    buffer[0] = 0xFF; buffer[1] = 0xFF;
    EXPECT_EQ(0xFFFF, tide::ebml_int::decode_u(buffer));
    // 3 bytes
    buffer.assign(3, 0);
    buffer[0] = 0x01;
    EXPECT_EQ(0x010000, tide::ebml_int::decode_u(buffer));
    buffer[0] = 0x01; buffer[2] = 0x01;
    EXPECT_EQ(0x010001, tide::ebml_int::decode_u(buffer));
    buffer.assign(3, 0xFF);
    buffer[0] = 0x7F;
    EXPECT_EQ(0x7FFFFF, tide::ebml_int::decode_u(buffer));
    buffer[0] = 0xFF;
    EXPECT_EQ(0xFFFFFF, tide::ebml_int::decode_u(buffer));
    // 4 bytes
    buffer.assign(4, 0);
    buffer[0] = 0x01;
    EXPECT_EQ(0x01000000, tide::ebml_int::decode_u(buffer));
    buffer[0] = 0x01; buffer[3] = 0x01;
    EXPECT_EQ(0x01000001, tide::ebml_int::decode_u(buffer));
    buffer.assign(4, 0xFF);
    buffer[0] = 0x7F;
    EXPECT_EQ(0x7FFFFFFF, tide::ebml_int::decode_u(buffer));
    buffer[0] = 0xFF;
    EXPECT_EQ(0xFFFFFFFF, tide::ebml_int::decode_u(buffer));
    // 5 bytes
    buffer.assign(5, 0);
    buffer[0] = 0x01;
    EXPECT_EQ(0x0100000000, tide::ebml_int::decode_u(buffer));
    buffer[0] = 0x01; buffer[4] = 0x01;
    EXPECT_EQ(0x0100000001, tide::ebml_int::decode_u(buffer));
    buffer.assign(5, 0xFF);
    buffer[0] = 0x7F;
    EXPECT_EQ(0x7FFFFFFFFF, tide::ebml_int::decode_u(buffer));
    buffer[0] = 0xFF;
    EXPECT_EQ(0xFFFFFFFFFF, tide::ebml_int::decode_u(buffer));
    // 6 bytes
    buffer.assign(6, 0);
    buffer[0] = 0x01;
    EXPECT_EQ(0x010000000000, tide::ebml_int::decode_u(buffer));
    buffer[0] = 0x01; buffer[5] = 0x01;
    EXPECT_EQ(0x010000000001, tide::ebml_int::decode_u(buffer));
    buffer.assign(6, 0xFF);
    buffer[0] = 0x7F;
    EXPECT_EQ(0x7FFFFFFFFFFF, tide::ebml_int::decode_u(buffer));
    buffer[0] = 0xFF;
    EXPECT_EQ(0xFFFFFFFFFFFF, tide::ebml_int::decode_u(buffer));
    // 7 bytes
    buffer.assign(7, 0);
    buffer[0] = 0x01;
    EXPECT_EQ(0x01000000000000, tide::ebml_int::decode_u(buffer));
    buffer[0] = 0x01; buffer[6] = 0x01;
    EXPECT_EQ(0x01000000000001, tide::ebml_int::decode_u(buffer));
    buffer.assign(7, 0xFF);
    buffer[0] = 0x7F;
    EXPECT_EQ(0x7FFFFFFFFFFFFF, tide::ebml_int::decode_u(buffer));
    buffer[0] = 0xFF;
    EXPECT_EQ(0xFFFFFFFFFFFFFF, tide::ebml_int::decode_u(buffer));
    // 8 bytes
    buffer.assign(8, 0);
    buffer[0] = 0x01;
    EXPECT_EQ(0x0100000000000000, tide::ebml_int::decode_u(buffer));
    buffer[0] = 0x01; buffer[7] = 0x01;
    EXPECT_EQ(0x0100000000000001, tide::ebml_int::decode_u(buffer));
    buffer.assign(8, 0xFF);
    buffer[0] = 0x7F;
    EXPECT_EQ(0x7FFFFFFFFFFFFFFF, tide::ebml_int::decode_u(buffer));
    buffer[0] = 0xFF;
    EXPECT_EQ(0xFFFFFFFFFFFFFFFF, tide::ebml_int::decode_u(buffer));
}


TEST(EBMLInt, DecodeSigned)
{
    std::vector<char> buffer;
    // 0 bytes
    EXPECT_EQ(0, tide::ebml_int::decode_s(buffer));
    // 1 byte
    buffer.assign(1, 0x80);
    EXPECT_EQ(-0x80, tide::ebml_int::decode_s(buffer));
    buffer[0] = 0xFF;
    EXPECT_EQ(-1, tide::ebml_int::decode_s(buffer));
    buffer[0] = 0x7F;
    EXPECT_EQ(0x7F, tide::ebml_int::decode_s(buffer));
    // 2 bytes
    buffer.assign(2, 0xFF);
    buffer[1] = 0x7F;
    EXPECT_EQ(-0x81, tide::ebml_int::decode_s(buffer));
    buffer.assign(2, 0);
    buffer[1] = 0x80;
    EXPECT_EQ(0x80, tide::ebml_int::decode_s(buffer));
    buffer.assign(2, 0);
    buffer[0] = 0x80;
    EXPECT_EQ(-0x8000, tide::ebml_int::decode_s(buffer));
    buffer.assign(2, 0xFF);
    buffer[0] = 0x7F;
    EXPECT_EQ(0x7FFF, tide::ebml_int::decode_s(buffer));
    // 3 bytes
    buffer.assign(3, 0xFF);
    buffer[1] = 0x7F;
    EXPECT_EQ(-0x8001, tide::ebml_int::decode_s(buffer));
    buffer.assign(3, 0);
    buffer[1] = 0x80;
    EXPECT_EQ(0x8000, tide::ebml_int::decode_s(buffer));
    buffer.assign(3, 0);
    buffer[0] = 0x80;
    EXPECT_EQ(-0x800000, tide::ebml_int::decode_s(buffer));
    buffer.assign(3, 0xFF);
    buffer[0] = 0x7F;
    EXPECT_EQ(0x7FFFFF, tide::ebml_int::decode_s(buffer));
    // 4 bytes
    buffer.assign(4, 0xFF);
    buffer[1] = 0x7F;
    EXPECT_EQ(-0x800001, tide::ebml_int::decode_s(buffer));
    buffer.assign(4, 0);
    buffer[1] = 0x80;
    EXPECT_EQ(0x800000, tide::ebml_int::decode_s(buffer));
    buffer.assign(4, 0);
    buffer[0] = 0x80;
    EXPECT_EQ(-0x80000000l, tide::ebml_int::decode_s(buffer));
    buffer.assign(4, 0xFF);
    buffer[0] = 0x7F;
    EXPECT_EQ(0x7FFFFFFFl, tide::ebml_int::decode_s(buffer));
    // 5 bytes
    buffer.assign(5, 0xFF);
    buffer[1] = 0x7F;
    EXPECT_EQ(-0x80000001l, tide::ebml_int::decode_s(buffer));
    buffer.assign(5, 0);
    buffer[1] = 0x80;
    EXPECT_EQ(0x80000000l, tide::ebml_int::decode_s(buffer));
    buffer.assign(5, 0);
    buffer[0] = 0x80;
    EXPECT_EQ(-0x8000000000l, tide::ebml_int::decode_s(buffer));
    buffer.assign(5, 0xFF);
    buffer[0] = 0x7F;
    EXPECT_EQ(0x7FFFFFFFFFl, tide::ebml_int::decode_s(buffer));
    // 6 bytes
    buffer.assign(6, 0xFF);
    buffer[1] = 0x7F;
    EXPECT_EQ(-0x8000000001l, tide::ebml_int::decode_s(buffer));
    buffer.assign(6, 0);
    buffer[1] = 0x80;
    EXPECT_EQ(0x8000000000l, tide::ebml_int::decode_s(buffer));
    buffer.assign(6, 0);
    buffer[0] = 0x80;
    EXPECT_EQ(-0x800000000000l, tide::ebml_int::decode_s(buffer));
    buffer.assign(6, 0xFF);
    buffer[0] = 0x7F;
    EXPECT_EQ(0x7FFFFFFFFFFFl, tide::ebml_int::decode_s(buffer));
    // 7 bytes
    buffer.assign(7, 0xFF);
    buffer[1] = 0x7F;
    EXPECT_EQ(-0x800000000001l, tide::ebml_int::decode_s(buffer));
    buffer.assign(7, 0);
    buffer[1] = 0x80;
    EXPECT_EQ(0x800000000000l, tide::ebml_int::decode_s(buffer));
    buffer.assign(7, 0);
    buffer[0] = 0x80;
    EXPECT_EQ(-0x80000000000000l, tide::ebml_int::decode_s(buffer));
    buffer.assign(7, 0xFF);
    buffer[0] = 0x7F;
    EXPECT_EQ(0x7FFFFFFFFFFFFFl, tide::ebml_int::decode_s(buffer));
    // 8 bytes
    buffer.assign(8, 0xFF);
    buffer[1] = 0x7F;
    EXPECT_EQ(-0x80000000000001l, tide::ebml_int::decode_s(buffer));
    buffer.assign(8, 0);
    buffer[1] = 0x80;
    EXPECT_EQ(0x80000000000000l, tide::ebml_int::decode_s(buffer));
    buffer.assign(8, 0);
    buffer[0] = 0x80;
    EXPECT_EQ(-0x8000000000000000l, tide::ebml_int::decode_s(buffer));
    buffer.assign(8, 0xFF);
    buffer[0] = 0x7F;
    EXPECT_EQ(0x7FFFFFFFFFFFFFFFl, tide::ebml_int::decode_s(buffer));
}


TEST(EBMLInt, EncodeDecodeUnsigned)
{
    // 0 bytes
    EXPECT_EQ(0, tide::ebml_int::decode_u(tide::ebml_int::encode_u(0)));
    // 1 byte
    EXPECT_EQ(1, tide::ebml_int::decode_u(tide::ebml_int::encode_u(1)));
    EXPECT_EQ(0x7F, tide::ebml_int::decode_u(tide::ebml_int::encode_u(0x7F)));
    EXPECT_EQ(0xFF, tide::ebml_int::decode_u(tide::ebml_int::encode_u(0xFF)));
    // 2 bytes
    EXPECT_EQ(0x100, tide::ebml_int::decode_u(tide::ebml_int::encode_u(0x100)));
    EXPECT_EQ(0x101, tide::ebml_int::decode_u(tide::ebml_int::encode_u(0x101)));
    EXPECT_EQ(0x7FFF,
            tide::ebml_int::decode_u(tide::ebml_int::encode_u(0x7FFF)));
    EXPECT_EQ(0xFFFF,
            tide::ebml_int::decode_u(tide::ebml_int::encode_u(0xFFFF)));
    // 3 bytes
    EXPECT_EQ(0x10000,
            tide::ebml_int::decode_u(tide::ebml_int::encode_u(0x10000)));
    EXPECT_EQ(0x10001,
            tide::ebml_int::decode_u(tide::ebml_int::encode_u(0x10001)));
    EXPECT_EQ(0x7FFFFF,
            tide::ebml_int::decode_u(tide::ebml_int::encode_u(0x7FFFFF)));
    EXPECT_EQ(0xFFFFFF,
            tide::ebml_int::decode_u(tide::ebml_int::encode_u(0xFFFFFF)));
    // 4 bytes
    EXPECT_EQ(0x1000000l,
            tide::ebml_int::decode_u(tide::ebml_int::encode_u(0x1000000l)));
    EXPECT_EQ(0x1000001l,
            tide::ebml_int::decode_u(tide::ebml_int::encode_u(0x1000001l)));
    EXPECT_EQ(0x7FFFFFFFl,
            tide::ebml_int::decode_u(tide::ebml_int::encode_u(0x7FFFFFFFl)));
    EXPECT_EQ(0xFFFFFFFFl,
            tide::ebml_int::decode_u(tide::ebml_int::encode_u(0xFFFFFFFFl)));
    // 5 bytes
    EXPECT_EQ(0x100000000l,
            tide::ebml_int::decode_u(tide::ebml_int::encode_u(0x100000000l)));
    EXPECT_EQ(0x100000001l,
            tide::ebml_int::decode_u(tide::ebml_int::encode_u(0x100000001l)));
    EXPECT_EQ(0x7FFFFFFFFFl,
            tide::ebml_int::decode_u(tide::ebml_int::encode_u(0x7FFFFFFFFFl)));
    EXPECT_EQ(0xFFFFFFFFFFl,
            tide::ebml_int::decode_u(tide::ebml_int::encode_u(0xFFFFFFFFFFl)));
    // 6 bytes
    EXPECT_EQ(0x10000000000l,
            tide::ebml_int::decode_u(tide::ebml_int::encode_u(0x10000000000l)));
    EXPECT_EQ(0x10000000001l,
            tide::ebml_int::decode_u(tide::ebml_int::encode_u(0x10000000001l)));
    EXPECT_EQ(0x7FFFFFFFFFFFl,
            tide::ebml_int::decode_u(tide::ebml_int::encode_u(0x7FFFFFFFFFFFl)));
    EXPECT_EQ(0xFFFFFFFFFFFFl,
            tide::ebml_int::decode_u(tide::ebml_int::encode_u(0xFFFFFFFFFFFFl)));
    // 7 bytes
    EXPECT_EQ(0x1000000000000l,
            tide::ebml_int::decode_u(tide::ebml_int::encode_u(0x1000000000000l)));
    EXPECT_EQ(0x1000000000001l,
            tide::ebml_int::decode_u(tide::ebml_int::encode_u(0x1000000000001l)));
    EXPECT_EQ(0x7FFFFFFFFFFFFFl,
            tide::ebml_int::decode_u(tide::ebml_int::encode_u(0x7FFFFFFFFFFFFFl)));
    EXPECT_EQ(0xFFFFFFFFFFFFFFl,
            tide::ebml_int::decode_u(tide::ebml_int::encode_u(0xFFFFFFFFFFFFFFl)));
    // 8 bytes
    EXPECT_EQ(0x100000000000000l,
            tide::ebml_int::decode_u(tide::ebml_int::encode_u(0x100000000000000l)));
    EXPECT_EQ(0x100000000000001l,
            tide::ebml_int::decode_u(tide::ebml_int::encode_u(0x100000000000001l)));
    EXPECT_EQ(0x7FFFFFFFFFFFFFFFl,
            tide::ebml_int::decode_u(tide::ebml_int::encode_u(0x7FFFFFFFFFFFFFFFl)));
    EXPECT_EQ(0xFFFFFFFFFFFFFFFFl,
            tide::ebml_int::decode_u(tide::ebml_int::encode_u(0xFFFFFFFFFFFFFFFFl)));
}


TEST(EBMLInt, EncodeDecodeSigned)
{
    // 0 bytes
    EXPECT_EQ(0, tide::ebml_int::decode_s(tide::ebml_int::encode_s(0)));
    // 1 byte
    EXPECT_EQ(-0x80, tide::ebml_int::decode_s(tide::ebml_int::encode_s(-0x80)));
    EXPECT_EQ(-1, tide::ebml_int::decode_s(tide::ebml_int::encode_s(-1)));
    EXPECT_EQ(0x7F, tide::ebml_int::decode_s(tide::ebml_int::encode_s(0x7F)));
    // 2 bytes
    EXPECT_EQ(-0x81, tide::ebml_int::decode_s(tide::ebml_int::encode_s(-0x81)));
    EXPECT_EQ(0x80, tide::ebml_int::decode_s(tide::ebml_int::encode_s(0x80)));
    EXPECT_EQ(-0x8000,
            tide::ebml_int::decode_s(tide::ebml_int::encode_s(-0x8000)));
    EXPECT_EQ(0x7FFF,
            tide::ebml_int::decode_s(tide::ebml_int::encode_s(0x7FFF)));
    // 3 bytes
    EXPECT_EQ(-0x8001,
            tide::ebml_int::decode_s(tide::ebml_int::encode_s(-0x8001)));
    EXPECT_EQ(0x8000,
            tide::ebml_int::decode_s(tide::ebml_int::encode_s(0x8000)));
    EXPECT_EQ(-0x800000,
            tide::ebml_int::decode_s(tide::ebml_int::encode_s(-0x800000)));
    EXPECT_EQ(0x7FFFFF,
            tide::ebml_int::decode_s(tide::ebml_int::encode_s(0x7FFFFF)));
    // 4 bytes
    EXPECT_EQ(-0x800001l,
            tide::ebml_int::decode_s(tide::ebml_int::encode_s(-0x800001l)));
    EXPECT_EQ(0x800000l,
            tide::ebml_int::decode_s(tide::ebml_int::encode_s(0x800000l)));
    EXPECT_EQ(-0x80000000l,
            tide::ebml_int::decode_s(tide::ebml_int::encode_s(-0x80000000l)));
    EXPECT_EQ(0x7FFFFFFFl,
            tide::ebml_int::decode_s(tide::ebml_int::encode_s(0x7FFFFFFFl)));
    // 5 bytes
    EXPECT_EQ(-0x80000001l,
            tide::ebml_int::decode_s(tide::ebml_int::encode_s(-0x80000001l)));
    EXPECT_EQ(0x80000000l,
            tide::ebml_int::decode_s(tide::ebml_int::encode_s(0x80000000l)));
    EXPECT_EQ(-0x8000000000l,
            tide::ebml_int::decode_s(tide::ebml_int::encode_s(-0x8000000000l)));
    EXPECT_EQ(0x7FFFFFFFFFl,
            tide::ebml_int::decode_s(tide::ebml_int::encode_s(0x7FFFFFFFFFl)));
    // 6 bytes
    EXPECT_EQ(-0x8000000001l,
            tide::ebml_int::decode_s(tide::ebml_int::encode_s(-0x8000000001l)));
    EXPECT_EQ(0x8000000000l,
            tide::ebml_int::decode_s(tide::ebml_int::encode_s(0x8000000000l)));
    EXPECT_EQ(-0x800000000000l,
            tide::ebml_int::decode_s(tide::ebml_int::encode_s(-0x800000000000l)));
    EXPECT_EQ(0x7FFFFFFFFFFFl,
            tide::ebml_int::decode_s(tide::ebml_int::encode_s(0x7FFFFFFFFFFFl)));
    // 7 bytes
    EXPECT_EQ(-0x800000000001l,
            tide::ebml_int::decode_s(tide::ebml_int::encode_s(-0x800000000001l)));
    EXPECT_EQ(0x800000000000l,
            tide::ebml_int::decode_s(tide::ebml_int::encode_s(0x800000000000l)));
    EXPECT_EQ(-0x80000000000000l,
            tide::ebml_int::decode_s(tide::ebml_int::encode_s(-0x80000000000000l)));
    EXPECT_EQ(0x7FFFFFFFFFFFFFl,
            tide::ebml_int::decode_s(tide::ebml_int::encode_s(0x7FFFFFFFFFFFFFl)));
    // 8 bytes
    EXPECT_EQ(-0x80000000000001l,
            tide::ebml_int::decode_s(tide::ebml_int::encode_s(-0x80000000000001l)));
    EXPECT_EQ(0x80000000000000l,
            tide::ebml_int::decode_s(tide::ebml_int::encode_s(0x80000000000000l)));
    EXPECT_EQ(-0x8000000000000000l,
            tide::ebml_int::decode_s(tide::ebml_int::encode_s(-0x8000000000000000l)));
    EXPECT_EQ(0x7FFFFFFFFFFFFFFFl,
            tide::ebml_int::decode_s(tide::ebml_int::encode_s(0x7FFFFFFFFFFFFFFFl)));
}


void fill_buffer(std::stringstream& buf, uint8_t c, std::streamsize n)
{
    for (std::streamsize ii(0); ii < n; ++ii)
    {
        buf.put(c);
    }
}


TEST(EBMLIntStream, WriteUnsigned)
{
    std::ostringstream buffer;
    std::string expected;
    expected.reserve(8);
    // 0 bytes
    expected.push_back(0xFF);
    buffer.put(0xFF);
    // This is checking that the buffer is untouched for zero
    EXPECT_EQ(0, tide::ebml_int::write_u(0x00, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    // 1 byte
    buffer.str(std::string());
    expected[0] = 0x01;
    EXPECT_EQ(1, tide::ebml_int::write_u(0x01, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected[0] = 0x7F;
    EXPECT_EQ(1, tide::ebml_int::write_u(0x7F, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected[0] = 0xFF;
    EXPECT_EQ(1, tide::ebml_int::write_u(0xFF, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    // 2 bytes
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 2, 0x00);
    expected[0] = 0x01;
    EXPECT_EQ(2, tide::ebml_int::write_u(0x0100, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected[0] = 0x01; expected[1] = 0x01;
    EXPECT_EQ(2, tide::ebml_int::write_u(0x0101, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 2, 0xFF);
    expected[0] = 0x7F;
    EXPECT_EQ(2, tide::ebml_int::write_u(0x7FFF, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected[0] = 0xFF; expected[1] = 0xFF;
    EXPECT_EQ(2, tide::ebml_int::write_u(0xFFFF, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    // 3 bytes
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 3, 0x00);
    expected[0] = 0x01;
    EXPECT_EQ(3, tide::ebml_int::write_u(0x010000, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected[0] = 0x01; expected[2] = 0x01;
    EXPECT_EQ(3, tide::ebml_int::write_u(0x010001, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 3, 0xFF);
    expected[0] = 0x7F;
    EXPECT_EQ(3, tide::ebml_int::write_u(0x7FFFFF, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected[0] = 0xFF;
    EXPECT_EQ(3, tide::ebml_int::write_u(0xFFFFFF, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    // 4 bytes
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 4, 0x00);
    expected[0] = 0x01;
    EXPECT_EQ(4, tide::ebml_int::write_u(0x01000000, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected[0] = 0x01; expected[3] = 0x01;
    EXPECT_EQ(4, tide::ebml_int::write_u(0x01000001, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 4, 0xFF);
    expected[0] = 0x7F;
    EXPECT_EQ(4, tide::ebml_int::write_u(0x7FFFFFFF, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected[0] = 0xFF;
    EXPECT_EQ(4, tide::ebml_int::write_u(0xFFFFFFFF, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    // 5 bytes
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 5, 0x00);
    expected[0] = 0x01;
    EXPECT_EQ(5, tide::ebml_int::write_u(0x0100000000l, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected[0] = 0x01; expected[4] = 0x01;
    EXPECT_EQ(5, tide::ebml_int::write_u(0x0100000001l, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 5, 0xFF);
    expected[0] = 0x7F;
    EXPECT_EQ(5, tide::ebml_int::write_u(0x7FFFFFFFFFl, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected[0] = 0xFF;
    EXPECT_EQ(5, tide::ebml_int::write_u(0xFFFFFFFFFFl, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    // 6 bytes
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 6, 0x00);
    expected[0] = 0x01;
    EXPECT_EQ(6, tide::ebml_int::write_u(0x010000000000l, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected[0] = 0x01; expected[5] = 0x01;
    EXPECT_EQ(6, tide::ebml_int::write_u(0x010000000001l, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 6, 0xFF);
    expected[0] = 0x7F;
    EXPECT_EQ(6, tide::ebml_int::write_u(0x7FFFFFFFFFFFl, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected[0] = 0xFF;
    EXPECT_EQ(6, tide::ebml_int::write_u(0xFFFFFFFFFFFFl, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    // 7 bytes
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 7, 0x00);
    expected[0] = 0x01;
    EXPECT_EQ(7, tide::ebml_int::write_u(0x01000000000000l, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected[0] = 0x01; expected[6] = 0x01;
    EXPECT_EQ(7, tide::ebml_int::write_u(0x01000000000001l, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 7, 0xFF);
    expected[0] = 0x7F;
    EXPECT_EQ(7, tide::ebml_int::write_u(0x7FFFFFFFFFFFFFl, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected[0] = 0xFF;
    EXPECT_EQ(7, tide::ebml_int::write_u(0xFFFFFFFFFFFFFFl, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    // 8 bytes
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 8, 0x00);
    expected[0] = 0x01;
    EXPECT_EQ(8, tide::ebml_int::write_u(0x0100000000000000l, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected[0] = 0x01; expected[7] = 0x01;
    EXPECT_EQ(8, tide::ebml_int::write_u(0x0100000000000001l, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 8, 0xFF);
    expected[0] = 0x7F;
    EXPECT_EQ(8, tide::ebml_int::write_u(0x7FFFFFFFFFFFFFFFl, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected[0] = 0xFF;
    EXPECT_EQ(8, tide::ebml_int::write_u(0xFFFFFFFFFFFFFFFFl, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
}


TEST(EBMLIntStream, WriteSigned)
{
    std::ostringstream buffer;
    std::string expected;
    expected.reserve(8);
    // 0 bytes
    expected.push_back(0xFF);
    buffer.put(0xFF);
    // This is checking that the buffer is untouched for zero
    EXPECT_EQ(0, tide::ebml_int::write_s(0, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    // 1 byte
    buffer.str(std::string());
    expected[0] = 0xFF;
    EXPECT_EQ(1, tide::ebml_int::write_s(-1, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected[0] = 0x01;
    EXPECT_EQ(1, tide::ebml_int::write_s(1, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected[0] = 0x80;
    EXPECT_EQ(1, tide::ebml_int::write_s(-0x80, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected[0] = 0x7F;
    EXPECT_EQ(1, tide::ebml_int::write_s(0x7F, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    // 2 bytes
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 2, 0xFF);
    expected[1] = 0x7F;
    EXPECT_EQ(2, tide::ebml_int::write_s(-0x81, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 2, 0x00);
    expected[1] = 0x80;
    EXPECT_EQ(2, tide::ebml_int::write_s(0x80, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected[0] = 0x80; expected[1] = 0x00;
    EXPECT_EQ(2, tide::ebml_int::write_s(-0x8000, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected[0] = 0x7F; expected[1] = 0xFF;
    EXPECT_EQ(2, tide::ebml_int::write_s(0x7FFF, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    // 3 bytes
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 3, 0xFF);
    expected[1] = 0x7F;
    EXPECT_EQ(3, tide::ebml_int::write_s(-0x8001, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 3, 0x00);
    expected[1] = 0x80;
    EXPECT_EQ(3, tide::ebml_int::write_s(0x8000, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 3, 0x00);
    expected[0] = 0x80;
    EXPECT_EQ(3, tide::ebml_int::write_s(-0x800000, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 3, 0xFF);
    expected[0] = 0x7F;
    EXPECT_EQ(3, tide::ebml_int::write_s(0x7FFFFF, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    // 4 bytes
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 4, 0xFF);
    expected[1] = 0x7F;
    EXPECT_EQ(4, tide::ebml_int::write_s(-0x800001, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 4, 0x00);
    expected[1] = 0x80;
    EXPECT_EQ(4, tide::ebml_int::write_s(0x800000, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 4, 0x00);
    expected[0] = 0x80;
    EXPECT_EQ(4, tide::ebml_int::write_s(-0x80000000l, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 4, 0xFF);
    expected[0] = 0x7F;
    EXPECT_EQ(4, tide::ebml_int::write_s(0x7FFFFFFFl, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    // 5 bytes
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 5, 0xFF);
    expected[1] = 0x7F;
    EXPECT_EQ(5, tide::ebml_int::write_s(-0x80000001l, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 5, 0x00);
    expected[1] = 0x80;
    EXPECT_EQ(5, tide::ebml_int::write_s(0x80000000l, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 5, 0x00);
    expected[0] = 0x80;
    EXPECT_EQ(5, tide::ebml_int::write_s(-0x8000000000l, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 5, 0xFF);
    expected[0] = 0x7F;
    EXPECT_EQ(5, tide::ebml_int::write_s(0x7FFFFFFFFFl, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    // 6 bytes
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 6, 0xFF);
    expected[1] = 0x7F;
    EXPECT_EQ(6, tide::ebml_int::write_s(-0x8000000001l, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 6, 0x00);
    expected[1] = 0x80;
    EXPECT_EQ(6, tide::ebml_int::write_s(0x8000000000l, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 6, 0x00);
    expected[0] = 0x80;
    EXPECT_EQ(6, tide::ebml_int::write_s(-0x800000000000l, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 6, 0xFF);
    expected[0] = 0x7F;
    EXPECT_EQ(6, tide::ebml_int::write_s(0x7FFFFFFFFFFFl, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    // 7 bytes
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 7, 0xFF);
    expected[1] = 0x7F;
    EXPECT_EQ(7, tide::ebml_int::write_s(-0x800000000001l, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 7, 0x00);
    expected[1] = 0x80;
    EXPECT_EQ(7, tide::ebml_int::write_s(0x800000000000l, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 7, 0x00);
    expected[0] = 0x80;
    EXPECT_EQ(7, tide::ebml_int::write_s(-0x80000000000000l, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 7, 0xFF);
    expected[0] = 0x7F;
    EXPECT_EQ(7, tide::ebml_int::write_s(0x7FFFFFFFFFFFFFl, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    // 8 bytes
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 8, 0xFF);
    expected[1] = 0x7F;
    EXPECT_EQ(8, tide::ebml_int::write_s(-0x80000000000001l, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 8, 0x00);
    expected[1] = 0x80;
    EXPECT_EQ(8, tide::ebml_int::write_s(0x80000000000000l, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 8, 0x00);
    expected[0] = 0x80;
    EXPECT_EQ(8, tide::ebml_int::write_s(-0x8000000000000000l, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    buffer.str(std::string());
    expected.replace(expected.begin(), expected.end(), 8, 0xFF);
    expected[0] = 0x7F;
    EXPECT_EQ(8, tide::ebml_int::write_s(0x7FFFFFFFFFFFFFFFl, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
}


TEST(EBMLIntStream, ReadUnsigned)
{
    std::stringstream buffer;
    // 0 bytes
    EXPECT_EQ(0, tide::ebml_int::read_u(buffer, 0));
    // 1 byte
    buffer.put(0x01);
    EXPECT_EQ(1, tide::ebml_int::read_u(buffer, 1));
    buffer.put(0x7F);
    EXPECT_EQ(0x7F, tide::ebml_int::read_u(buffer, 1));
    buffer.put(0xFF);
    EXPECT_EQ(0xFF, tide::ebml_int::read_u(buffer, 1));
    // 2 bytes
    buffer.put(0x01); buffer.put(0x00);
    EXPECT_EQ(0x0100, tide::ebml_int::read_u(buffer, 2));
    buffer.put(0x01); buffer.put(0x01);
    EXPECT_EQ(0x0101, tide::ebml_int::read_u(buffer, 2));
    buffer.put(0x7F); buffer.put(0xFF);
    EXPECT_EQ(0x7FFF, tide::ebml_int::read_u(buffer, 2));
    buffer.put(0xFF); buffer.put(0xFF);
    EXPECT_EQ(0xFFFF, tide::ebml_int::read_u(buffer, 2));
    // 3 bytes
    buffer.put(0x01); fill_buffer(buffer, 0x00, 2);
    EXPECT_EQ(0x010000, tide::ebml_int::read_u(buffer, 3));
    buffer.put(0x01); buffer.put(0x00); buffer.put(0x01);
    EXPECT_EQ(0x010001, tide::ebml_int::read_u(buffer, 3));
    buffer.put(0x7F); buffer.put(0xFF); buffer.put(0xFF);
    EXPECT_EQ(0x7FFFFF, tide::ebml_int::read_u(buffer, 3));
    buffer.put(0xFF); buffer.put(0xFF); buffer.put(0xFF);;
    EXPECT_EQ(0xFFFFFF, tide::ebml_int::read_u(buffer, 3));
    // 4 bytes
    buffer.put(0x01); fill_buffer(buffer, 0x00, 3);
    EXPECT_EQ(0x01000000, tide::ebml_int::read_u(buffer, 4));
    buffer.put(0x01); fill_buffer(buffer, 0x00, 2); buffer.put(0x01);
    EXPECT_EQ(0x01000001, tide::ebml_int::read_u(buffer, 4));
    buffer.put(0x7F); fill_buffer(buffer, 0xFF, 3);
    EXPECT_EQ(0x7FFFFFFF, tide::ebml_int::read_u(buffer, 4));
    fill_buffer(buffer, 0xFF, 4);
    EXPECT_EQ(0xFFFFFFFF, tide::ebml_int::read_u(buffer, 4));
    // 5 bytes
    buffer.put(0x01); fill_buffer(buffer, 0x00, 4);
    EXPECT_EQ(0x0100000000, tide::ebml_int::read_u(buffer, 5));
    buffer.put(0x01); fill_buffer(buffer, 0x00, 3); buffer.put(0x01);
    EXPECT_EQ(0x0100000001, tide::ebml_int::read_u(buffer, 5));
    buffer.put(0x7F); fill_buffer(buffer, 0xFF, 4);
    EXPECT_EQ(0x7FFFFFFFFF, tide::ebml_int::read_u(buffer, 5));
    fill_buffer(buffer, 0xFF, 5);
    EXPECT_EQ(0xFFFFFFFFFF, tide::ebml_int::read_u(buffer, 5));
    // 6 bytes
    buffer.put(0x01); fill_buffer(buffer, 0x00, 5);
    EXPECT_EQ(0x010000000000, tide::ebml_int::read_u(buffer, 6));
    buffer.put(0x01); fill_buffer(buffer, 0x00, 4); buffer.put(0x01);
    EXPECT_EQ(0x010000000001, tide::ebml_int::read_u(buffer, 6));
    buffer.put(0x7F); fill_buffer(buffer, 0xFF, 5);
    EXPECT_EQ(0x7FFFFFFFFFFF, tide::ebml_int::read_u(buffer, 6));
    fill_buffer(buffer, 0xFF, 6);
    EXPECT_EQ(0xFFFFFFFFFFFF, tide::ebml_int::read_u(buffer, 6));
    // 7 bytes
    buffer.put(0x01); fill_buffer(buffer, 0x00, 6);
    EXPECT_EQ(0x01000000000000, tide::ebml_int::read_u(buffer, 7));
    buffer.put(0x01); fill_buffer(buffer, 0x00, 5); buffer.put(0x01);
    EXPECT_EQ(0x01000000000001, tide::ebml_int::read_u(buffer, 7));
    buffer.put(0x7F); fill_buffer(buffer, 0xFF, 6);
    EXPECT_EQ(0x7FFFFFFFFFFFFF, tide::ebml_int::read_u(buffer, 7));
    fill_buffer(buffer, 0xFF, 7);
    EXPECT_EQ(0xFFFFFFFFFFFFFF, tide::ebml_int::read_u(buffer, 7));
    // 8 bytes
    buffer.put(0x01); fill_buffer(buffer, 0x00, 7);
    EXPECT_EQ(0x0100000000000000, tide::ebml_int::read_u(buffer, 8));
    buffer.put(0x01); fill_buffer(buffer, 0x00, 6); buffer.put(0x01);
    EXPECT_EQ(0x0100000000000001, tide::ebml_int::read_u(buffer, 8));
    buffer.put(0x7F); fill_buffer(buffer, 0xFF, 7);
    EXPECT_EQ(0x7FFFFFFFFFFFFFFF, tide::ebml_int::read_u(buffer, 8));
    fill_buffer(buffer, 0xFF, 8);
    EXPECT_EQ(0xFFFFFFFFFFFFFFFF, tide::ebml_int::read_u(buffer, 8));
}


TEST(EBMLIntStream, ReadSigned)
{
    std::stringstream buffer;
    // 0 bytes
    EXPECT_EQ(0, tide::ebml_int::read_s(buffer, 0));
    // 1 byte
    buffer.put(0x80);
    EXPECT_EQ(-0x80, tide::ebml_int::read_s(buffer, 1));
    buffer.put(0xFF);
    EXPECT_EQ(-1, tide::ebml_int::read_s(buffer, 1));
    buffer.put(0x7F);
    EXPECT_EQ(0x7F, tide::ebml_int::read_s(buffer, 1));
    // 2 bytes
    buffer.put(0xFF); buffer.put(0x7F);
    EXPECT_EQ(-0x81, tide::ebml_int::read_s(buffer, 2));
    buffer.put(0x00); buffer.put(0x80);
    EXPECT_EQ(0x80, tide::ebml_int::read_s(buffer, 2));
    buffer.put(0x80); buffer.put(0x00);
    EXPECT_EQ(-0x8000, tide::ebml_int::read_s(buffer, 2));
    buffer.put(0x7F); buffer.put(0xFF);
    EXPECT_EQ(0x7FFF, tide::ebml_int::read_s(buffer, 2));
    // 3 bytes
    buffer.put(0xFF); buffer.put(0x7F); buffer.put(0xFF);
    EXPECT_EQ(-0x8001, tide::ebml_int::read_s(buffer, 3));
    buffer.put(0x00); buffer.put(0x80); buffer.put(0x00);
    EXPECT_EQ(0x8000, tide::ebml_int::read_s(buffer, 3));
    buffer.put(0x80); fill_buffer(buffer, 0x00, 2);
    EXPECT_EQ(-0x800000, tide::ebml_int::read_s(buffer, 3));
    buffer.put(0x7F); fill_buffer(buffer, 0xFF, 2);
    EXPECT_EQ(0x7FFFFF, tide::ebml_int::read_s(buffer, 3));
    // 4 bytes
    buffer.put(0xFF); buffer.put(0x7F); fill_buffer(buffer, 0xFF, 2);
    EXPECT_EQ(-0x800001, tide::ebml_int::read_s(buffer, 4));
    buffer.put(0x00); buffer.put(0x80); fill_buffer(buffer, 0x00, 2);
    EXPECT_EQ(0x800000, tide::ebml_int::read_s(buffer, 4));
    buffer.put(0x80); fill_buffer(buffer, 0x00, 3);
    EXPECT_EQ(-0x80000000l, tide::ebml_int::read_s(buffer, 4));
    buffer.put(0x7F); fill_buffer(buffer, 0xFF, 3);
    EXPECT_EQ(0x7FFFFFFFl, tide::ebml_int::read_s(buffer, 4));
    // 5 bytes
    buffer.put(0xFF); buffer.put(0x7F); fill_buffer(buffer, 0xFF, 3);
    EXPECT_EQ(-0x80000001l, tide::ebml_int::read_s(buffer, 5));
    buffer.put(0x00); buffer.put(0x80); fill_buffer(buffer, 0x00, 3);
    EXPECT_EQ(0x80000000l, tide::ebml_int::read_s(buffer, 5));
    buffer.put(0x80); fill_buffer(buffer, 0x00, 4);
    EXPECT_EQ(-0x8000000000l, tide::ebml_int::read_s(buffer, 5));
    buffer.put(0x7F); fill_buffer(buffer, 0xFF, 4);
    EXPECT_EQ(0x7FFFFFFFFFl, tide::ebml_int::read_s(buffer, 5));
    // 6 bytes
    buffer.put(0xFF); buffer.put(0x7F); fill_buffer(buffer, 0xFF, 4);
    EXPECT_EQ(-0x8000000001l, tide::ebml_int::read_s(buffer, 6));
    buffer.put(0x00); buffer.put(0x80); fill_buffer(buffer, 0x00, 4);
    EXPECT_EQ(0x8000000000l, tide::ebml_int::read_s(buffer, 6));
    buffer.put(0x80); fill_buffer(buffer, 0x00, 5);
    EXPECT_EQ(-0x800000000000l, tide::ebml_int::read_s(buffer, 6));
    buffer.put(0x7F); fill_buffer(buffer, 0xFF, 5);
    EXPECT_EQ(0x7FFFFFFFFFFFl, tide::ebml_int::read_s(buffer, 6));
    // 7 bytes
    buffer.put(0xFF); buffer.put(0x7F); fill_buffer(buffer, 0xFF, 5);
    EXPECT_EQ(-0x800000000001l, tide::ebml_int::read_s(buffer, 7));
    buffer.put(0x00); buffer.put(0x80); fill_buffer(buffer, 0x00, 5);
    EXPECT_EQ(0x800000000000l, tide::ebml_int::read_s(buffer, 7));
    buffer.put(0x80); fill_buffer(buffer, 0x00, 6);
    EXPECT_EQ(-0x80000000000000l, tide::ebml_int::read_s(buffer, 7));
    buffer.put(0x7F); fill_buffer(buffer, 0xFF, 6);
    EXPECT_EQ(0x7FFFFFFFFFFFFFl, tide::ebml_int::read_s(buffer, 7));
    // 8 bytes
    buffer.put(0xFF); buffer.put(0x7F); fill_buffer(buffer, 0xFF, 6);
    EXPECT_EQ(-0x80000000000001l, tide::ebml_int::read_s(buffer, 8));
    buffer.put(0x00); buffer.put(0x80); fill_buffer(buffer, 0x00, 6);
    EXPECT_EQ(0x80000000000000l, tide::ebml_int::read_s(buffer, 8));
    buffer.put(0x80); fill_buffer(buffer, 0x00, 7);
    EXPECT_EQ(-0x8000000000000000l, tide::ebml_int::read_s(buffer, 8));
    buffer.put(0x7F); fill_buffer(buffer, 0xFF, 7);
    EXPECT_EQ(0x7FFFFFFFFFFFFFFFl, tide::ebml_int::read_s(buffer, 8));
}


TEST(EBMLIntStream, WriteReadUnsigned)
{
    std::stringstream buffer;
    // 0 bytes
    tide::ebml_int::write_u(0, buffer);
    EXPECT_EQ(0, tide::ebml_int::read_u(buffer, 0));
    // 1 byte
    tide::ebml_int::write_u(1, buffer);
    EXPECT_EQ(1, tide::ebml_int::read_u(buffer, 1));
    tide::ebml_int::write_u(0x7F, buffer);
    EXPECT_EQ(0x7F, tide::ebml_int::read_u(buffer, 1));
    tide::ebml_int::write_u(0xFF, buffer);
    EXPECT_EQ(0xFF, tide::ebml_int::read_u(buffer, 1));
    // 2 bytes
    tide::ebml_int::write_u(0x100, buffer);
    EXPECT_EQ(0x100, tide::ebml_int::read_u(buffer, 2));
    tide::ebml_int::write_u(0x101, buffer);
    EXPECT_EQ(0x101, tide::ebml_int::read_u(buffer, 2));
    tide::ebml_int::write_u(0x7FFF, buffer);
    EXPECT_EQ(0x7FFF, tide::ebml_int::read_u(buffer, 2));
    tide::ebml_int::write_u(0xFFFF, buffer);
    EXPECT_EQ(0xFFFF, tide::ebml_int::read_u(buffer, 2));
    // 3 bytes
    tide::ebml_int::write_u(0x10000, buffer);
    EXPECT_EQ(0x10000, tide::ebml_int::read_u(buffer, 3));
    tide::ebml_int::write_u(0x10001, buffer);
    EXPECT_EQ(0x10001, tide::ebml_int::read_u(buffer, 3));
    tide::ebml_int::write_u(0x7FFFFF, buffer);
    EXPECT_EQ(0x7FFFFF, tide::ebml_int::read_u(buffer, 3));
    tide::ebml_int::write_u(0xFFFFFF, buffer);
    EXPECT_EQ(0xFFFFFF, tide::ebml_int::read_u(buffer, 3));
    // 4 bytes
    tide::ebml_int::write_u(0x1000000l, buffer);
    EXPECT_EQ(0x1000000l, tide::ebml_int::read_u(buffer, 4));
    tide::ebml_int::write_u(0x1000001l, buffer);
    EXPECT_EQ(0x1000001l, tide::ebml_int::read_u(buffer, 4));
    tide::ebml_int::write_u(0x7FFFFFFFl, buffer);
    EXPECT_EQ(0x7FFFFFFFl, tide::ebml_int::read_u(buffer, 4));
    tide::ebml_int::write_u(0xFFFFFFFFl, buffer);
    EXPECT_EQ(0xFFFFFFFFl, tide::ebml_int::read_u(buffer, 4));
    // 5 bytes
    tide::ebml_int::write_u(0x100000000l, buffer);
    EXPECT_EQ(0x100000000l, tide::ebml_int::read_u(buffer, 5));
    tide::ebml_int::write_u(0x100000001l, buffer);
    EXPECT_EQ(0x100000001l, tide::ebml_int::read_u(buffer, 5));
    tide::ebml_int::write_u(0x7FFFFFFFFFl, buffer);
    EXPECT_EQ(0x7FFFFFFFFFl, tide::ebml_int::read_u(buffer, 5));
    tide::ebml_int::write_u(0xFFFFFFFFFFl, buffer);
    EXPECT_EQ(0xFFFFFFFFFFl, tide::ebml_int::read_u(buffer, 5));
    // 6 bytes
    tide::ebml_int::write_u(0x10000000000l, buffer);
    EXPECT_EQ(0x10000000000l, tide::ebml_int::read_u(buffer, 6));
    tide::ebml_int::write_u(0x10000000001l, buffer);
    EXPECT_EQ(0x10000000001l, tide::ebml_int::read_u(buffer, 6));
    tide::ebml_int::write_u(0x7FFFFFFFFFFFl, buffer);
    EXPECT_EQ(0x7FFFFFFFFFFFl, tide::ebml_int::read_u(buffer, 6));
    tide::ebml_int::write_u(0xFFFFFFFFFFFFl, buffer);
    EXPECT_EQ(0xFFFFFFFFFFFFl, tide::ebml_int::read_u(buffer, 6));
    // 7 bytes
    tide::ebml_int::write_u(0x1000000000000l, buffer);
    EXPECT_EQ(0x1000000000000l, tide::ebml_int::read_u(buffer, 7));
    tide::ebml_int::write_u(0x1000000000001l, buffer);
    EXPECT_EQ(0x1000000000001l, tide::ebml_int::read_u(buffer, 7));
    tide::ebml_int::write_u(0x7FFFFFFFFFFFFFl, buffer);
    EXPECT_EQ(0x7FFFFFFFFFFFFFl, tide::ebml_int::read_u(buffer, 7));
    tide::ebml_int::write_u(0xFFFFFFFFFFFFFFl, buffer);
    EXPECT_EQ(0xFFFFFFFFFFFFFFl, tide::ebml_int::read_u(buffer, 7));
    // 8 bytes
    tide::ebml_int::write_u(0x100000000000000l, buffer);
    EXPECT_EQ(0x100000000000000l, tide::ebml_int::read_u(buffer, 8));
    tide::ebml_int::write_u(0x100000000000001l, buffer);
    EXPECT_EQ(0x100000000000001l, tide::ebml_int::read_u(buffer, 8));
    tide::ebml_int::write_u(0x7FFFFFFFFFFFFFFFl, buffer);
    EXPECT_EQ(0x7FFFFFFFFFFFFFFFl, tide::ebml_int::read_u(buffer, 8));
    tide::ebml_int::write_u(0xFFFFFFFFFFFFFFFFl, buffer);
    EXPECT_EQ(0xFFFFFFFFFFFFFFFFl, tide::ebml_int::read_u(buffer, 8));
}


TEST(EBMLIntStream, WriteReadSigned)
{
    std::stringstream buffer;
    // 0 bytes
    tide::ebml_int::write_s(0, buffer);
    EXPECT_EQ(0, tide::ebml_int::read_s(buffer, 0));
    // 1 byte
    tide::ebml_int::write_s(-0x80, buffer);
    EXPECT_EQ(-0x80, tide::ebml_int::read_s(buffer, 1));
    tide::ebml_int::write_s(-1, buffer);
    EXPECT_EQ(-1, tide::ebml_int::read_s(buffer, 1));
    tide::ebml_int::write_s(0x7F, buffer);
    EXPECT_EQ(0x7F, tide::ebml_int::read_s(buffer, 1));
    // 2 bytes
    tide::ebml_int::write_s(-0x81, buffer);
    EXPECT_EQ(-0x81, tide::ebml_int::read_s(buffer, 2));
    tide::ebml_int::write_s(0x80, buffer);
    EXPECT_EQ(0x80, tide::ebml_int::read_s(buffer, 2));
    tide::ebml_int::write_s(-0x8000, buffer);
    EXPECT_EQ(-0x8000, tide::ebml_int::read_s(buffer, 2));
    tide::ebml_int::write_s(0x7FFF, buffer);
    EXPECT_EQ(0x7FFF, tide::ebml_int::read_s(buffer, 2));
    // 3 bytes
    tide::ebml_int::write_s(-0x8001, buffer);
    EXPECT_EQ(-0x8001, tide::ebml_int::read_s(buffer, 3));
    tide::ebml_int::write_s(0x8000, buffer);
    EXPECT_EQ(0x8000, tide::ebml_int::read_s(buffer, 3));
    tide::ebml_int::write_s(-0x800000, buffer);
    EXPECT_EQ(-0x800000, tide::ebml_int::read_s(buffer, 3));
    tide::ebml_int::write_s(0x7FFFFF, buffer);
    EXPECT_EQ(0x7FFFFF, tide::ebml_int::read_s(buffer, 3));
    // 4 bytes
    tide::ebml_int::write_s(-0x800001l, buffer);
    EXPECT_EQ(-0x800001l, tide::ebml_int::read_s(buffer, 4));
    tide::ebml_int::write_s(0x800000l, buffer);
    EXPECT_EQ(0x800000l, tide::ebml_int::read_s(buffer, 4));
    tide::ebml_int::write_s(-0x80000000l, buffer);
    EXPECT_EQ(-0x80000000l, tide::ebml_int::read_s(buffer, 4));
    tide::ebml_int::write_s(0x7FFFFFFFl, buffer);
    EXPECT_EQ(0x7FFFFFFFl, tide::ebml_int::read_s(buffer, 4));
    // 5 bytes
    tide::ebml_int::write_s(-0x80000001l, buffer);
    EXPECT_EQ(-0x80000001l, tide::ebml_int::read_s(buffer, 5));
    tide::ebml_int::write_s(0x80000000l, buffer);
    EXPECT_EQ(0x80000000l, tide::ebml_int::read_s(buffer, 5));
    tide::ebml_int::write_s(-0x8000000000l, buffer);
    EXPECT_EQ(-0x8000000000l, tide::ebml_int::read_s(buffer, 5));
    tide::ebml_int::write_s(0x7FFFFFFFFFl, buffer);
    EXPECT_EQ(0x7FFFFFFFFFl, tide::ebml_int::read_s(buffer, 5));
    // 6 bytes
    tide::ebml_int::write_s(-0x8000000001l, buffer);
    EXPECT_EQ(-0x8000000001l, tide::ebml_int::read_s(buffer, 6));
    tide::ebml_int::write_s(0x8000000000l, buffer);
    EXPECT_EQ(0x8000000000l, tide::ebml_int::read_s(buffer, 6));
    tide::ebml_int::write_s(-0x800000000000l, buffer);
    EXPECT_EQ(-0x800000000000l, tide::ebml_int::read_s(buffer, 6));
    tide::ebml_int::write_s(0x7FFFFFFFFFFFl, buffer);
    EXPECT_EQ(0x7FFFFFFFFFFFl, tide::ebml_int::read_s(buffer, 6));
    // 7 bytes
    tide::ebml_int::write_s(-0x800000000001l, buffer);
    EXPECT_EQ(-0x800000000001l, tide::ebml_int::read_s(buffer, 7));
    tide::ebml_int::write_s(0x800000000000l, buffer);
    EXPECT_EQ(0x800000000000l, tide::ebml_int::read_s(buffer, 7));
    tide::ebml_int::write_s(-0x80000000000000l, buffer);
    EXPECT_EQ(-0x80000000000000l, tide::ebml_int::read_s(buffer, 7));
    tide::ebml_int::write_s(0x7FFFFFFFFFFFFFl, buffer);
    EXPECT_EQ(0x7FFFFFFFFFFFFFl, tide::ebml_int::read_s(buffer, 7));
    // 8 bytes
    tide::ebml_int::write_s(-0x80000000000001l, buffer);
    EXPECT_EQ(-0x80000000000001l, tide::ebml_int::read_s(buffer, 8));
    tide::ebml_int::write_s(0x80000000000000l, buffer);
    EXPECT_EQ(0x80000000000000l, tide::ebml_int::read_s(buffer, 8));
    tide::ebml_int::write_s(-0x8000000000000000l, buffer);
    EXPECT_EQ(-0x8000000000000000l, tide::ebml_int::read_s(buffer, 8));
    tide::ebml_int::write_s(0x7FFFFFFFFFFFFFFFl, buffer);
    EXPECT_EQ(0x7FFFFFFFFFFFFFFFl, tide::ebml_int::read_s(buffer, 8));
}


TEST(EBMLIntStream, ReadError)
{
    std::stringstream buffer;
    EXPECT_THROW(tide::ebml_int::read_s(buffer, 8), tide::ReadError);
    buffer.put(0x7F); buffer.put(0x29);
    EXPECT_THROW(tide::ebml_int::read_s(buffer, 8), tide::ReadError);
}

