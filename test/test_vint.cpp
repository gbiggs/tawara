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
    uint8_t expected[8], buffer[8];
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    // 1xxxxxxx
    expected[0] = 0x80;
    tide::encode_vint(0x00, buffer, 1);
    EXPECT_PRED3(buffers_eq, expected, buffer, 1);
    expected[0] = 0x81;
    tide::encode_vint(0x01, buffer, 1);
    EXPECT_PRED3(buffers_eq, expected, buffer, 1);
    expected[0] = 0x97;
    tide::encode_vint(0x17, buffer, 1);
    EXPECT_PRED3(buffers_eq, expected, buffer, 1);
    expected[0] = 0xC0;
    tide::encode_vint(0x40, buffer, 1);
    EXPECT_PRED3(buffers_eq, expected, buffer, 1);
    expected[0] = 0xFF;
    tide::encode_vint(0x7F, buffer, 1);
    EXPECT_PRED3(buffers_eq, expected, buffer, 1);
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    // 01xxxxxx xxxxxxxx
    expected[0] = 0x80; expected[1] = 0x00;
    tide::encode_vint(0x0000, buffer, 2);
    EXPECT_PRED3(buffers_eq, expected, buffer, 2);
    expected[0] = 0x81; expected[1] = 0x00;
    tide::encode_vint(0x0001, buffer, 2);
    EXPECT_PRED3(buffers_eq, expected, buffer, 2);
    expected[0] = 0x4B; expected[1] = 0x35;
    tide::encode_vint(0x0B35, buffer, 2);
    EXPECT_PRED3(buffers_eq, expected, buffer, 2);
    expected[0] = 0x60; expected[1] = 0x00;
    tide::encode_vint(0x2000, buffer, 2);
    EXPECT_PRED3(buffers_eq, expected, buffer, 2);
    expected[0] = 0x7F; expected[1] = 0xFF;
    tide::encode_vint(0x3FFF, buffer, 2);
    EXPECT_PRED3(buffers_eq, expected, buffer, 2);
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    expected[0] = 0x80; expected[1] = 0x00;
    tide::encode_vint(0x0000000000000000, buffer, 8);
    EXPECT_PRED3(buffers_eq, expected, buffer, 8);
    expected[0] = 0x81; expected[1] = 0x00;
    tide::encode_vint(0x0000000000000001, buffer, 8);
    EXPECT_PRED3(buffers_eq, expected, buffer, 8);
    memset(expected, 0xFF, sizeof(expected));
    expected[0] = 0x01;
    tide::encode_vint(0xFFFFFFFFFFFFFF, buffer, 8);
    EXPECT_PRED3(buffers_eq, expected, buffer, 8);
    // EBML tag
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    expected[0] = 0x1A; expected[1] = 0x45; expected[2] = 0xDF;
    expected[3] = 0xA3;
    tide::encode_vint(0x0A45DFA3, buffer, 4);
    EXPECT_PRED3(buffers_eq, expected, buffer, 4);
    // The remainder are done in the EncodeDecode test for simplicity
}


TEST(VInt, Decode)
{
    uint8_t buffer[8];
    memset(buffer, 0, sizeof(buffer));
    // 1xxxxxxx
    buffer[0] = 0x80;
    EXPECT_EQ(0x00, tide::decode_vint(buffer, 1));
    buffer[0] = 0x81;
    EXPECT_EQ(0x01, tide::decode_vint(buffer, 1));
    buffer[0] = 0x97;
    EXPECT_EQ(0x17, tide::decode_vint(buffer, 1));
    buffer[0] = 0xC0;
    EXPECT_EQ(0x40, tide::decode_vint(buffer, 1));
    buffer[0] = 0xFF;
    EXPECT_EQ(0x7F, tide::decode_vint(buffer, 1));
    // 01xxxxxx xxxxxxxx
    buffer[0] = 0x40; buffer[1] = 0x00;
    EXPECT_EQ(0x0000, tide::decode_vint(buffer, 2));
    buffer[0] = 0x40; buffer[1] = 0x01;
    EXPECT_EQ(0x0001, tide::decode_vint(buffer, 2));
    buffer[0] = 0x4B; buffer[1] = 0x35;
    EXPECT_EQ(0x0B35, tide::decode_vint(buffer, 2));
    buffer[0] = 0x60; buffer[1] = 0x00;
    EXPECT_EQ(0x2000, tide::decode_vint(buffer, 2));
    buffer[0] = 0x7F; buffer[1] = 0xFF;
    EXPECT_EQ(0x3FFF, tide::decode_vint(buffer, 2));
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = 0x01;
    EXPECT_EQ(0x00000000, tide::decode_vint(buffer, 8));
    buffer[7] = 0x01;
    EXPECT_EQ(0x00000001, tide::decode_vint(buffer, 8));
    memset(buffer, 0xFF, sizeof(buffer));
    buffer[0] = 0x01;
    EXPECT_EQ(0xFFFFFFFFFFFFFF, tide::decode_vint(buffer, 8));
    // EBML tag
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = 0x1A; buffer[1] = 0x45; buffer[2] = 0xDF; buffer[3] = 0xA3;
    EXPECT_EQ(0x0A45DFA3, tide::decode_vint(buffer, 4));
    // The remainder are done in the EncodeDecode test for simplicity
}


TEST(VInt, EncodeDecode)
{
    uint8_t buffer[8];
    memset(buffer, 0, sizeof(buffer));
    // 1xxxxxxx
    EXPECT_EQ(0x00, tide::decode_vint(tide::encode_vint(0x00, buffer, 8), 8));
    EXPECT_EQ(0x01, tide::decode_vint(tide::encode_vint(0x01, buffer, 8), 8));
    EXPECT_EQ(0x7F, tide::decode_vint(tide::encode_vint(0x7F, buffer, 8), 8));
    // 01xxxxxx xxxxxxxx
    EXPECT_EQ(0x80, tide::decode_vint(tide::encode_vint(0x80, buffer, 8), 8));
    EXPECT_EQ(0x81, tide::decode_vint(tide::encode_vint(0x81, buffer, 8), 8));
    EXPECT_EQ(0x3FFF, tide::decode_vint(tide::encode_vint(0x3FFF, buffer, 8), 8));
    // 001xxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(0x4000, tide::decode_vint(tide::encode_vint(0x4000, buffer, 8), 8));
    EXPECT_EQ(0x60000, tide::decode_vint(tide::encode_vint(0x60000, buffer, 8), 8));
    EXPECT_EQ(0x1FFFFF, tide::decode_vint(tide::encode_vint(0x1FFFFF, buffer, 8), 8));
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(0x200000, tide::decode_vint(tide::encode_vint(0x200000, buffer, 8), 8));
    EXPECT_EQ(0xFFFFFFF, tide::decode_vint(tide::encode_vint(0xFFFFFFF, buffer, 8), 8));
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(0x10000000, tide::decode_vint(tide::encode_vint(0x10000000, buffer, 8), 8));
    EXPECT_EQ(0x7FFFFFFFF, tide::decode_vint(tide::encode_vint(0x7FFFFFFFF, buffer, 8), 8));
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(0x800000000, tide::decode_vint(tide::encode_vint(0x800000000, buffer, 8), 8));
    EXPECT_EQ(0X3FFFFFFFFFF, tide::decode_vint(tide::encode_vint(0X3FFFFFFFFFF, buffer, 8), 8));
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(0x40000000000, tide::decode_vint(tide::encode_vint(0x40000000000, buffer, 8), 8));
    EXPECT_EQ(0X1FFFFFFFFFFFF, tide::decode_vint(tide::encode_vint(0X1FFFFFFFFFFFF, buffer, 8), 8));
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(0x2000000000000, tide::decode_vint(tide::encode_vint(0x2000000000000, buffer, 8), 8));
    EXPECT_EQ(0X0FFFFFFFFFFFFFF, tide::decode_vint(tide::encode_vint(0X0FFFFFFFFFFFFFF, buffer, 8), 8));
}


TEST(VInt, NoTail)
{
    uint8_t buffer[8];
    // 1xxxxxxx - No tail necessary
    buffer[0] = 0x80;
    EXPECT_NO_THROW(tide::decode_vint(buffer, 1));
    // 01xxxxxx xxxxxxxx
    buffer[0] = 0x40;
    EXPECT_THROW(tide::decode_vint(buffer, 1), tide::BufferTooSmall);
    // 001xxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x20;
    EXPECT_THROW(tide::decode_vint(buffer, 1), tide::BufferTooSmall);
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x10;
    EXPECT_THROW(tide::decode_vint(buffer, 1), tide::BufferTooSmall);
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x08;
    EXPECT_THROW(tide::decode_vint(buffer, 1), tide::BufferTooSmall);
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x04;
    EXPECT_THROW(tide::decode_vint(buffer, 1), tide::BufferTooSmall);
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x02;
    EXPECT_THROW(tide::decode_vint(buffer, 1), tide::BufferTooSmall);
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x01;
    EXPECT_THROW(tide::decode_vint(buffer, 1), tide::BufferTooSmall);
}


TEST(VInt, BufferTooSmall)
{
    uint8_t buffer[8];
    // 1xxxxxxx will assert
    // 01xxxxxx xxxxxxxx
    EXPECT_THROW(tide::encode_vint(0x80, buffer, 1), tide::BufferTooSmall);
    // 001xxxxx xxxxxxxx xxxxxxxx
    EXPECT_THROW(tide::encode_vint(0x4000, buffer, 2), tide::BufferTooSmall);
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_THROW(tide::encode_vint(0x200000, buffer, 3), tide::BufferTooSmall);
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_THROW(tide::encode_vint(0x10000000, buffer, 4), tide::BufferTooSmall);
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_THROW(tide::encode_vint(0x800000000, buffer, 5), tide::BufferTooSmall);
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_THROW(tide::encode_vint(0x40000000000, buffer, 6), tide::BufferTooSmall);
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_THROW(tide::encode_vint(0x2000000000000, buffer, 7), tide::BufferTooSmall);
}


TEST(VInt, TailTooShort)
{
    uint8_t buffer[8];
    // 1xxxxxxx - No tail necessary
    buffer[0] = 0x80;
    EXPECT_NO_THROW(tide::decode_vint(buffer, 1));
    // 01xxxxxx xxxxxxxx
    buffer[0] = 0x40;
    EXPECT_THROW(tide::decode_vint(buffer, 1), tide::BufferTooSmall);
    // 001xxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x20;
    EXPECT_THROW(tide::decode_vint(buffer, 2), tide::BufferTooSmall);
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x10;
    EXPECT_THROW(tide::decode_vint(buffer, 3), tide::BufferTooSmall);
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x08;
    EXPECT_THROW(tide::decode_vint(buffer, 4), tide::BufferTooSmall);
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x04;
    EXPECT_THROW(tide::decode_vint(buffer, 5), tide::BufferTooSmall);
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x02;
    EXPECT_THROW(tide::decode_vint(buffer, 6), tide::BufferTooSmall);
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x01;
    EXPECT_THROW(tide::decode_vint(buffer, 7), tide::BufferTooSmall);
}


TEST(VInt, NoMarker)
{
    uint8_t buffer[8];
    // 1xxxxxxx - Success
    buffer[0] = 0x80;
    EXPECT_NO_THROW(tide::decode_vint(buffer, 1));
    // 00000000 xxxxxxxx xxxxxxxx
    buffer[0] = 0x00;
    EXPECT_THROW(tide::decode_vint(buffer, 3), tide::InvalidVarInt);
}


TEST(VInt, TooBig)
{
    uint8_t buffer[8], empty[8];
    memset(buffer, 0, sizeof(buffer));
    memset(empty, 0, sizeof(empty));
    EXPECT_THROW(tide::encode_vint(0x100000000000001, buffer, 8),
            tide::VarIntTooBig);
    EXPECT_PRED3(buffers_eq, buffer, empty, 8);
    EXPECT_THROW(tide::encode_vint(0xFFFFFFFFFFFFFFFF, buffer, 8),
            tide::VarIntTooBig);
    EXPECT_PRED3(buffers_eq, buffer, empty, 8);
}

