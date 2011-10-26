/* TIDE
 *
 * Variable-length integer tests.
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
#include <tide/vint.h>

#include "test_consts.h"


bool buffers_eq(uint8_t const* b1, uint8_t const* b2, size_t n)
{
    /*std::cout << "Comparing " << std::hex << std::right;
    std::cout.width(2);
    std::cout.fill('0');
    for (int ii(0); ii < n; ii++)
        std::cout << static_cast<int>(b1[ii]);
    std::cout << " with ";
    for (int ii(0); ii < n; ii++)
        std::cout << static_cast<int>(b2[ii]);
    std::cout << '\n';*/
    return strncmp(reinterpret_cast<char const*>(b1),
            reinterpret_cast<char const*>(b2), n) == 0;
}


TEST(VInt, Encode)
{
    uint8_t expected[8], data[8];
    memset(expected, 0, sizeof(expected));
    memset(data, 0, sizeof(data));
    // 1xxxxxxx
    expected[0] = 0x80;
    tide::encode_vint(0x00, data, 1);
    EXPECT_PRED3(buffers_eq, expected, data, 1);
    expected[0] = 0x81;
    tide::encode_vint(0x01, data, 1);
    EXPECT_PRED3(buffers_eq, expected, data, 1);
    expected[0] = 0x97;
    tide::encode_vint(0x17, data, 1);
    EXPECT_PRED3(buffers_eq, expected, data, 1);
    expected[0] = 0xC0;
    tide::encode_vint(0x40, data, 1);
    EXPECT_PRED3(buffers_eq, expected, data, 1);
    expected[0] = 0xFF;
    tide::encode_vint(0x7F, data, 1);
    EXPECT_PRED3(buffers_eq, expected, data, 1);
    memset(expected, 0, sizeof(expected));
    memset(data, 0, sizeof(data));
    // 01xxxxxx xxxxxxxx
    expected[0] = 0x80; expected[1] = 0x00;
    tide::encode_vint(0x0000, data, 2);
    EXPECT_PRED3(buffers_eq, expected, data, 2);
    expected[0] = 0x81; expected[1] = 0x00;
    tide::encode_vint(0x0001, data, 2);
    EXPECT_PRED3(buffers_eq, expected, data, 2);
    expected[0] = 0x4B; expected[1] = 0x35;
    tide::encode_vint(0x0B35, data, 2);
    EXPECT_PRED3(buffers_eq, expected, data, 2);
    expected[0] = 0x60; expected[1] = 0x00;
    tide::encode_vint(0x2000, data, 2);
    EXPECT_PRED3(buffers_eq, expected, data, 2);
    expected[0] = 0x7F; expected[1] = 0xFF;
    tide::encode_vint(0x3FFF, data, 2);
    EXPECT_PRED3(buffers_eq, expected, data, 2);
    memset(expected, 0, sizeof(expected));
    memset(data, 0, sizeof(data));
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    expected[0] = 0x80; expected[1] = 0x00;
    tide::encode_vint(0x0000000000000000, data, 8);
    EXPECT_PRED3(buffers_eq, expected, data, 8);
    expected[0] = 0x81; expected[1] = 0x00;
    tide::encode_vint(0x0000000000000001, data, 8);
    EXPECT_PRED3(buffers_eq, expected, data, 8);
    memset(expected, 0xFF, sizeof(expected));
    expected[0] = 0x01;
    tide::encode_vint(0xFFFFFFFFFFFFFF, data, 8);
    EXPECT_PRED3(buffers_eq, expected, data, 8);
    // EBML tag
    memset(expected, 0, sizeof(expected));
    memset(data, 0, sizeof(data));
    expected[0] = 0x1A; expected[1] = 0x45; expected[2] = 0xDF;
    expected[3] = 0xA3;
    tide::encode_vint(0x0A45DFA3, data, 4);
    EXPECT_PRED3(buffers_eq, expected, data, 4);
    // The remainder are done in the EncodeDecode test for simplicity
}


TEST(VInt, Decode)
{
    uint8_t data[8];
    memset(data, 0, sizeof(data));
    // 1xxxxxxx
    data[0] = 0x80;
    EXPECT_EQ(0x00, tide::decode_vint(data));
    data[0] = 0x81;
    EXPECT_EQ(0x01, tide::decode_vint(data));
    data[0] = 0x97;
    EXPECT_EQ(0x17, tide::decode_vint(data));
    data[0] = 0xC0;
    EXPECT_EQ(0x40, tide::decode_vint(data));
    data[0] = 0xFF;
    EXPECT_EQ(0x7F, tide::decode_vint(data));
    // 01xxxxxx xxxxxxxx
    data[0] = 0x40; data[1] = 0x00;
    EXPECT_EQ(0x0000, tide::decode_vint(data));
    data[0] = 0x40; data[1] = 0x01;
    EXPECT_EQ(0x0001, tide::decode_vint(data));
    data[0] = 0x4B; data[1] = 0x35;
    EXPECT_EQ(0x0B35, tide::decode_vint(data));
    data[0] = 0x60; data[1] = 0x00;
    EXPECT_EQ(0x2000, tide::decode_vint(data));
    data[0] = 0x7F; data[1] = 0xFF;
    EXPECT_EQ(0x3FFF, tide::decode_vint(data));
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    memset(data, 0, sizeof(data));
    data[0] = 0x01;
    EXPECT_EQ(0x00000000, tide::decode_vint(data));
    data[7] = 0x01;
    EXPECT_EQ(0x00000001, tide::decode_vint(data));
    memset(data, 0xFF, sizeof(data));
    data[0] = 0x01;
    EXPECT_EQ(0xFFFFFFFFFFFFFF, tide::decode_vint(data));
    // EBML tag
    memset(data, 0, sizeof(data));
    data[0] = 0x1A; data[1] = 0x45; data[2] = 0xDF; data[3] = 0xA3;
    EXPECT_EQ(0x0A45DFA3, tide::decode_vint(data));
    // The remainder are done in the EncodeDecode test for simplicity
}


TEST(VInt, EncodeDecode)
{
    uint8_t buffer[8];
    memset(buffer, 0, sizeof(buffer));
    // 1xxxxxxx
    EXPECT_EQ(0x00, tide::decode_vint(tide::encode_vint(0x00, buffer, 8)));
    EXPECT_EQ(0x01, tide::decode_vint(tide::encode_vint(0x01, buffer, 8)));
    EXPECT_EQ(0x7F, tide::decode_vint(tide::encode_vint(0x7F, buffer, 8)));
    // 01xxxxxx xxxxxxxx
    EXPECT_EQ(0x80, tide::decode_vint(tide::encode_vint(0x80, buffer, 8)));
    EXPECT_EQ(0x81, tide::decode_vint(tide::encode_vint(0x81, buffer, 8)));
    EXPECT_EQ(0x3FFF, tide::decode_vint(tide::encode_vint(0x3FFF, buffer, 8)));
    // 001xxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(0x4000, tide::decode_vint(tide::encode_vint(0x4000, buffer, 8)));
    EXPECT_EQ(0x60000, tide::decode_vint(tide::encode_vint(0x60000, buffer, 8)));
    EXPECT_EQ(0x1FFFFF, tide::decode_vint(tide::encode_vint(0x1FFFFF, buffer, 8)));
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(0x200000, tide::decode_vint(tide::encode_vint(0x200000, buffer, 8)));
    EXPECT_EQ(0xFFFFFFF, tide::decode_vint(tide::encode_vint(0xFFFFFFF, buffer, 8)));
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(0x10000000, tide::decode_vint(tide::encode_vint(0x10000000, buffer, 8)));
    EXPECT_EQ(0x7FFFFFFFF, tide::decode_vint(tide::encode_vint(0x7FFFFFFFF, buffer, 8)));
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(0x800000000, tide::decode_vint(tide::encode_vint(0x800000000, buffer, 8)));
    EXPECT_EQ(0X3FFFFFFFFFF, tide::decode_vint(tide::encode_vint(0X3FFFFFFFFFF, buffer, 8)));
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(0x40000000000, tide::decode_vint(tide::encode_vint(0x40000000000, buffer, 8)));
    EXPECT_EQ(0X1FFFFFFFFFFFF, tide::decode_vint(tide::encode_vint(0X1FFFFFFFFFFFF, buffer, 8)));
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(0x2000000000000, tide::decode_vint(tide::encode_vint(0x2000000000000, buffer, 8)));
    EXPECT_EQ(0X0FFFFFFFFFFFFFF, tide::decode_vint(tide::encode_vint(0X0FFFFFFFFFFFFFF, buffer, 8)));
}


TEST(VInt, NoTail)
{
    // 1xxxxxxx - No tail necessary
    // 01xxxxxx xxxxxxxx
    // 001xxxxx xxxxxxxx xxxxxxxx
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
}


TEST(VInt, TailTooShort)
{
    // Test for each possible marker bit position
    // 1xxxxxxx
    // 01xxxxxx xxxxxxxx
    // 001xxxxx xxxxxxxx xxxxxxxx
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
}


TEST(VInt, NoMarker)
{
    // Test detecting if there is a bit set in the first byte
    // 1xxxxxxx - Success
    // 00000000
    // 00000000 xxxxxxxx xxxxxxxx
}


TEST(VInt, NoWidth)
{
}


TEST(VInt, TooBig)
{
}

