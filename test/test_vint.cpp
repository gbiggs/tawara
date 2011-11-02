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
#include "test_utils.h"


TEST(VInt, Encode)
{
    uint8_t expected[8], buffer[8];
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    // 1xxxxxxx
    expected[0] = 0x80;
    EXPECT_EQ(1, tide::vint::encode(0x00, buffer, 1));
    EXPECT_PRED_FORMAT3(test_utils::buffers_eq, expected, buffer, 1);
    expected[0] = 0x81;
    EXPECT_EQ(1, tide::vint::encode(0x01, buffer, 1));
    EXPECT_PRED_FORMAT3(test_utils::buffers_eq, expected, buffer, 1);
    expected[0] = 0x97;
    EXPECT_EQ(1, tide::vint::encode(0x17, buffer, 1));
    EXPECT_PRED_FORMAT3(test_utils::buffers_eq, expected, buffer, 1);
    expected[0] = 0xC0;
    EXPECT_EQ(1, tide::vint::encode(0x40, buffer, 1));
    EXPECT_PRED_FORMAT3(test_utils::buffers_eq, expected, buffer, 1);
    expected[0] = 0xFF;
    EXPECT_EQ(1, tide::vint::encode(0x7F, buffer, 1));
    EXPECT_PRED_FORMAT3(test_utils::buffers_eq, expected, buffer, 1);
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    // 01xxxxxx xxxxxxxx
    expected[0] = 0x80; expected[1] = 0x00;
    EXPECT_EQ(1, tide::vint::encode(0x0000, buffer, 2));
    EXPECT_PRED_FORMAT3(test_utils::buffers_eq, expected, buffer, 2);
    expected[0] = 0x81; expected[1] = 0x00;
    EXPECT_EQ(1, tide::vint::encode(0x0001, buffer, 2));
    EXPECT_PRED_FORMAT3(test_utils::buffers_eq, expected, buffer, 2);
    expected[0] = 0x4B; expected[1] = 0x35;
    EXPECT_EQ(2, tide::vint::encode(0x0B35, buffer, 2));
    EXPECT_PRED_FORMAT3(test_utils::buffers_eq, expected, buffer, 2);
    expected[0] = 0x60; expected[1] = 0x00;
    EXPECT_EQ(2, tide::vint::encode(0x2000, buffer, 2));
    EXPECT_PRED_FORMAT3(test_utils::buffers_eq, expected, buffer, 2);
    expected[0] = 0x7F; expected[1] = 0xFF;
    EXPECT_EQ(2, tide::vint::encode(0x3FFF, buffer, 2));
    EXPECT_PRED_FORMAT3(test_utils::buffers_eq, expected, buffer, 2);
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    expected[0] = 0x80; expected[1] = 0x00;
    EXPECT_EQ(1, tide::vint::encode(0x0000000000000000, buffer, 8));
    EXPECT_PRED_FORMAT3(test_utils::buffers_eq, expected, buffer, 8);
    expected[0] = 0x81; expected[1] = 0x00;
    EXPECT_EQ(1, tide::vint::encode(0x0000000000000001, buffer, 8));
    EXPECT_PRED_FORMAT3(test_utils::buffers_eq, expected, buffer, 8);
    memset(expected, 0xFF, sizeof(expected));
    expected[0] = 0x01;
    EXPECT_EQ(8, tide::vint::encode(0xFFFFFFFFFFFFFF, buffer, 8));
    EXPECT_PRED_FORMAT3(test_utils::buffers_eq, expected, buffer, 8);
    // EBML tag
    memset(expected, 0, sizeof(expected));
    memset(buffer, 0, sizeof(buffer));
    expected[0] = 0x1A; expected[1] = 0x45; expected[2] = 0xDF;
    expected[3] = 0xA3;
    EXPECT_EQ(4, tide::vint::encode(0x0A45DFA3, buffer, 4));
    EXPECT_PRED_FORMAT3(test_utils::buffers_eq, expected, buffer, 4);
    // The remainder are done in the EncodeDecode test for simplicity
}


TEST(VInt, Decode)
{
    uint8_t buffer[8];
    memset(buffer, 0, sizeof(buffer));
    // 1xxxxxxx
    buffer[0] = 0x80;
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x00, 1),
            tide::vint::decode(buffer, 1));
    buffer[0] = 0x81;
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x01, 1),
            tide::vint::decode(buffer, 1));
    buffer[0] = 0x97;
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x17, 1),
            tide::vint::decode(buffer, 1));
    buffer[0] = 0xC0;
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x40, 1),
            tide::vint::decode(buffer, 1));
    buffer[0] = 0xFF;
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x7F, 1),
            tide::vint::decode(buffer, 1));
    // 01xxxxxx xxxxxxxx
    buffer[0] = 0x40; buffer[1] = 0x00;
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x0000, 2),
            tide::vint::decode(buffer, 2));
    buffer[0] = 0x40; buffer[1] = 0x01;
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x0001, 2),
            tide::vint::decode(buffer, 2));
    buffer[0] = 0x4B; buffer[1] = 0x35;
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x0B35, 2),
            tide::vint::decode(buffer, 2));
    buffer[0] = 0x60; buffer[1] = 0x00;
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x2000, 2),
            tide::vint::decode(buffer, 2));
    buffer[0] = 0x7F; buffer[1] = 0xFF;
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x3FFF, 2),
            tide::vint::decode(buffer, 2));
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = 0x01;
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x00000000, 8),
            tide::vint::decode(buffer, 8));
    buffer[7] = 0x01;
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x00000001, 8),
            tide::vint::decode(buffer, 8));
    memset(buffer, 0xFF, sizeof(buffer));
    buffer[0] = 0x01;
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0xFFFFFFFFFFFFFF, 8),
            tide::vint::decode(buffer, 8));
    // EBML tag
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = 0x1A; buffer[1] = 0x45; buffer[2] = 0xDF; buffer[3] = 0xA3;
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x0A45DFA3, 4),
            tide::vint::decode(buffer, 4));
    // The remainder are done in the EncodeDecode test for simplicity
}


TEST(VInt, EncodeDecode)
{
    uint8_t buffer[8];
    memset(buffer, 0, sizeof(buffer));
    // 1xxxxxxx
    EXPECT_EQ(1, tide::vint::encode(0x00, buffer, 8));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x00, 1),
            tide::vint::decode(buffer, 8));
    EXPECT_EQ(1, tide::vint::encode(0x01, buffer, 8));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x01, 1),
            tide::vint::decode(buffer, 8));
    EXPECT_EQ(1, tide::vint::encode(0x7F, buffer, 8));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x7F, 1),
            tide::vint::decode(buffer, 8));
    // 01xxxxxx xxxxxxxx
    EXPECT_EQ(2, tide::vint::encode(0x80, buffer, 8));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x80, 2),
            tide::vint::decode(buffer, 8));
    EXPECT_EQ(2, tide::vint::encode(0x81, buffer, 8));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x81, 2),
            tide::vint::decode(buffer, 8));
    EXPECT_EQ(2, tide::vint::encode(0x3FFF, buffer, 8));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x3FFF, 2),
            tide::vint::decode(buffer, 8));
    // 001xxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(3, tide::vint::encode(0x4000, buffer, 8));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x4000, 3),
            tide::vint::decode(buffer, 8));
    EXPECT_EQ(3, tide::vint::encode(0x60000, buffer, 8));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x60000, 3),
            tide::vint::decode(buffer, 8));
    EXPECT_EQ(3, tide::vint::encode(0x1FFFFF, buffer, 8));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x1FFFFF, 3),
            tide::vint::decode(buffer, 8));
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(4, tide::vint::encode(0x200000, buffer, 8));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x200000, 4),
            tide::vint::decode(buffer, 8));
    EXPECT_EQ(4, tide::vint::encode(0xFFFFFFF, buffer, 8));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0xFFFFFFF, 4),
            tide::vint::decode(buffer, 8));
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(5, tide::vint::encode(0x10000000, buffer, 8));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x10000000, 5),
            tide::vint::decode(buffer, 8));
    EXPECT_EQ(5, tide::vint::encode(0x7FFFFFFFFl, buffer, 8));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq,
            std::make_pair(0x7FFFFFFFFl, 5), tide::vint::decode(buffer, 8));
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(6, tide::vint::encode(0x800000000l, buffer, 8));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x800000000l, 6),
            tide::vint::decode(buffer, 8));
    EXPECT_EQ(6, tide::vint::encode(0X3FFFFFFFFFFl, buffer, 8));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0X3FFFFFFFFFFl, 6),
            tide::vint::decode(buffer, 8));
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(7, tide::vint::encode(0x40000000000l, buffer, 8));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x40000000000l, 7),
            tide::vint::decode(buffer, 8));
    EXPECT_EQ(7, tide::vint::encode(0X1FFFFFFFFFFFFl, buffer, 8));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0X1FFFFFFFFFFFFl, 7),
            tide::vint::decode(buffer, 8));
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(8, tide::vint::encode(0x2000000000000l, buffer, 8));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x2000000000000l, 8),
            tide::vint::decode(buffer, 8));
    EXPECT_EQ(8, tide::vint::encode(0X0FFFFFFFFFFFFFFl, buffer, 8));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0X0FFFFFFFFFFFFFFl, 8),
            tide::vint::decode(buffer, 8));
}


TEST(VInt, NoTail)
{
    uint8_t buffer[8];
    // 1xxxxxxx - No tail necessary
    buffer[0] = 0x80;
    EXPECT_NO_THROW(tide::vint::decode(buffer, 1));
    // 01xxxxxx xxxxxxxx
    buffer[0] = 0x40;
    EXPECT_THROW(tide::vint::decode(buffer, 1), tide::BufferTooSmall);
    // 001xxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x20;
    EXPECT_THROW(tide::vint::decode(buffer, 1), tide::BufferTooSmall);
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x10;
    EXPECT_THROW(tide::vint::decode(buffer, 1), tide::BufferTooSmall);
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x08;
    EXPECT_THROW(tide::vint::decode(buffer, 1), tide::BufferTooSmall);
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x04;
    EXPECT_THROW(tide::vint::decode(buffer, 1), tide::BufferTooSmall);
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x02;
    EXPECT_THROW(tide::vint::decode(buffer, 1), tide::BufferTooSmall);
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x01;
    EXPECT_THROW(tide::vint::decode(buffer, 1), tide::BufferTooSmall);
}


TEST(VInt, BufferTooSmall)
{
    uint8_t buffer[8];
    // 1xxxxxxx will assert
    // 01xxxxxx xxxxxxxx
    EXPECT_THROW(tide::vint::encode(0x80, buffer, 1), tide::BufferTooSmall);
    // 001xxxxx xxxxxxxx xxxxxxxx
    EXPECT_THROW(tide::vint::encode(0x4000, buffer, 2), tide::BufferTooSmall);
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_THROW(tide::vint::encode(0x200000, buffer, 3), tide::BufferTooSmall);
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_THROW(tide::vint::encode(0x10000000, buffer, 4), tide::BufferTooSmall);
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_THROW(tide::vint::encode(0x800000000, buffer, 5), tide::BufferTooSmall);
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_THROW(tide::vint::encode(0x40000000000, buffer, 6), tide::BufferTooSmall);
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_THROW(tide::vint::encode(0x2000000000000, buffer, 7), tide::BufferTooSmall);
}


TEST(VInt, TailTooShort)
{
    uint8_t buffer[8];
    // 1xxxxxxx - No tail necessary
    buffer[0] = 0x80;
    EXPECT_NO_THROW(tide::vint::decode(buffer, 1));
    // 01xxxxxx xxxxxxxx
    buffer[0] = 0x40;
    EXPECT_THROW(tide::vint::decode(buffer, 1), tide::BufferTooSmall);
    // 001xxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x20;
    EXPECT_THROW(tide::vint::decode(buffer, 2), tide::BufferTooSmall);
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x10;
    EXPECT_THROW(tide::vint::decode(buffer, 3), tide::BufferTooSmall);
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x08;
    EXPECT_THROW(tide::vint::decode(buffer, 4), tide::BufferTooSmall);
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x04;
    EXPECT_THROW(tide::vint::decode(buffer, 5), tide::BufferTooSmall);
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x02;
    EXPECT_THROW(tide::vint::decode(buffer, 6), tide::BufferTooSmall);
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x01;
    EXPECT_THROW(tide::vint::decode(buffer, 7), tide::BufferTooSmall);
}


TEST(VInt, NoMarker)
{
    uint8_t buffer[8];
    // 1xxxxxxx - Success
    buffer[0] = 0x80;
    EXPECT_NO_THROW(tide::vint::decode(buffer, 1));
    // 00000000 xxxxxxxx xxxxxxxx
    buffer[0] = 0x00;
    EXPECT_THROW(tide::vint::decode(buffer, 3), tide::InvalidVarInt);
}


TEST(VInt, TooBig)
{
    uint8_t buffer[8], empty[8];
    memset(buffer, 0, sizeof(buffer));
    memset(empty, 0, sizeof(empty));
    EXPECT_THROW(tide::vint::encode(0x100000000000001, buffer, 8),
            tide::VarIntTooBig);
    EXPECT_PRED_FORMAT3(test_utils::buffers_eq, buffer, empty, 8);
    EXPECT_THROW(tide::vint::encode(0xFFFFFFFFFFFFFFFF, buffer, 8),
            tide::VarIntTooBig);
    EXPECT_PRED_FORMAT3(test_utils::buffers_eq, buffer, empty, 8);
}


TEST(VInt, CodedSize)
{
    EXPECT_EQ(1, tide::vint::coded_size(0x00));
    EXPECT_EQ(1, tide::vint::coded_size(0x01));
    EXPECT_EQ(1, tide::vint::coded_size(0x7F));
    // 01xxxxxx xxxxxxxx
    EXPECT_EQ(2, tide::vint::coded_size(0x80));
    EXPECT_EQ(2, tide::vint::coded_size(0x81));
    EXPECT_EQ(2, tide::vint::coded_size(0x3FFF));
    // 001xxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(3, tide::vint::coded_size(0x4000));
    EXPECT_EQ(3, tide::vint::coded_size(0x60000));
    EXPECT_EQ(3, tide::vint::coded_size(0x1FFFFF));
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(4, tide::vint::coded_size(0x200000));
    EXPECT_EQ(4, tide::vint::coded_size(0xFFFFFFF));
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(5, tide::vint::coded_size(0x10000000));
    EXPECT_EQ(5, tide::vint::coded_size(0x7FFFFFFFF));
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(6, tide::vint::coded_size(0x800000000));
    EXPECT_EQ(6, tide::vint::coded_size(0X3FFFFFFFFFF));
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(7, tide::vint::coded_size(0x40000000000));
    EXPECT_EQ(7, tide::vint::coded_size(0X1FFFFFFFFFFFF));
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(8, tide::vint::coded_size(0x2000000000000));
    EXPECT_EQ(8, tide::vint::coded_size(0X0FFFFFFFFFFFFFF));
    // Oversize
    EXPECT_THROW(tide::vint::coded_size(0x100000000000000), tide::VarIntTooBig);
    EXPECT_THROW(tide::vint::coded_size(0xFFFFFFFFFFFFFFFF), tide::VarIntTooBig);
}


TEST(VIntStream, Write)
{
    std::ostringstream buffer;
    std::ostringstream expected;
    // 1xxxxxxx
    expected.put(0x80);
    EXPECT_EQ(1, tide::vint::write(0x00, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(), buffer.str());
    expected.put(0x81);
    EXPECT_EQ(1, tide::vint::write(0x01, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(), buffer.str());
    expected.put(0x97);
    EXPECT_EQ(1, tide::vint::write(0x17, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(), buffer.str());
    expected.put(0xC0);
    EXPECT_EQ(1, tide::vint::write(0x40, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(), buffer.str());
    expected.put(0xFF);
    EXPECT_EQ(1, tide::vint::write(0x7F, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(), buffer.str());
    // 01xxxxxx xxxxxxxx
    expected.put(0x80);
    EXPECT_EQ(1, tide::vint::write(0x0000, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(), buffer.str());
    expected.put(0x81);
    EXPECT_EQ(1, tide::vint::write(0x0001, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(), buffer.str());
    expected.put(0x4B); expected.put(0x35);
    EXPECT_EQ(2, tide::vint::write(0x0B35, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(), buffer.str());
    expected.put(0x60); expected.put(0x00);
    EXPECT_EQ(2, tide::vint::write(0x2000, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(), buffer.str());
    expected.put(0x7F); expected.put(0xFF);
    EXPECT_EQ(2, tide::vint::write(0x3FFF, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(), buffer.str());
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    expected.put(0x80);
    EXPECT_EQ(1, tide::vint::write(0x0000000000000000, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(), buffer.str());
    expected.put(0x81);
    EXPECT_EQ(1, tide::vint::write(0x0000000000000001, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(), buffer.str());
    expected.put(0x01);
    for (int ii(0); ii < 7; ii++)
    {
        expected.put(0xFF);
    }
    EXPECT_EQ(8, tide::vint::write(0xFFFFFFFFFFFFFF, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(), buffer.str());
    // EBML tag
    expected.put(0x1A); expected.put(0x45); expected.put(0xDF);
    expected.put(0xA3);
    EXPECT_EQ(4, tide::vint::write(0x0A45DFA3, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(), buffer.str());
    // The remainder are done in the EncodeDecode test for simplicity
}


TEST(VIntStream, Read)
{
    std::stringstream buffer;
    // 1xxxxxxx
    buffer.put(0x80);
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x00, 1),
            tide::vint::read(buffer));
    buffer.put(0x81);
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x01, 1),
            tide::vint::read(buffer));
    buffer.put(0x97);
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x17, 1),
            tide::vint::read(buffer));
    buffer.put(0xC0);
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x40, 1),
            tide::vint::read(buffer));
    buffer.put(0xFF);
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x7F, 1),
            tide::vint::read(buffer));
    // 01xxxxxx xxxxxxxx
    buffer.put(0x40); buffer.put(0x00);;
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x0000, 2),
            tide::vint::read(buffer));
    buffer.put(0x40); buffer.put(0x01);;
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x0001, 2),
            tide::vint::read(buffer));
    buffer.put(0x4B); buffer.put(0x35);;
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x0B35, 2),
            tide::vint::read(buffer));
    buffer.put(0x60); buffer.put(0x00);;
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x2000, 2),
            tide::vint::read(buffer));
    buffer.put(0x7F); buffer.put(0xFF);;
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x3FFF, 2),
            tide::vint::read(buffer));
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x01);
    for (int ii(0); ii < 7; ii++)
    {
        buffer.put(0x00);
    }
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x00000000, 8),
            tide::vint::read(buffer));
    buffer.put(0x01);
    for (int ii(0); ii < 6; ii++)
    {
        buffer.put(0x00);
    }
    buffer.put(0x01);
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x00000001, 8),
            tide::vint::read(buffer));
    buffer.put(0x01);
    for (int ii(0); ii < 7; ii++)
    {
        buffer.put(0xFF);
    }
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0xFFFFFFFFFFFFFF, 8),
            tide::vint::read(buffer));
    // EBML tag
    buffer.put(0x1A); buffer.put(0x45); buffer.put(0xDF); buffer.put(0xA3);
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x0A45DFA3, 4),
            tide::vint::read(buffer));
    // The remainder are done in the EncodeDecode test for simplicity
}


TEST(VIntStream, WriteRead)
{
    std::stringstream buffer;
    // 1xxxxxxx
    EXPECT_EQ(1, tide::vint::write(0x00, buffer));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x00, 1),
            tide::vint::read(buffer));
    EXPECT_EQ(1, tide::vint::write(0x01, buffer));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x01, 1),
            tide::vint::read(buffer));
    EXPECT_EQ(1, tide::vint::write(0x7F, buffer));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x7F, 1),
            tide::vint::read(buffer));
    // 01xxxxxx xxxxxxxx
    EXPECT_EQ(2, tide::vint::write(0x80, buffer));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x80, 2),
            tide::vint::read(buffer));
    EXPECT_EQ(2, tide::vint::write(0x81, buffer));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x81, 2),
            tide::vint::read(buffer));
    EXPECT_EQ(2, tide::vint::write(0x3FFF, buffer));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x3FFF, 2),
            tide::vint::read(buffer));
    // 001xxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(3, tide::vint::write(0x4000, buffer));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x4000, 3),
            tide::vint::read(buffer));
    EXPECT_EQ(3, tide::vint::write(0x60000, buffer));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x60000, 3),
            tide::vint::read(buffer));
    EXPECT_EQ(3, tide::vint::write(0x1FFFFF, buffer));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x1FFFFF, 3),
            tide::vint::read(buffer));
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(4, tide::vint::write(0x200000, buffer));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x200000, 4),
            tide::vint::read(buffer));
    EXPECT_EQ(4, tide::vint::write(0xFFFFFFF, buffer));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0xFFFFFFF, 4),
            tide::vint::read(buffer));
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(5, tide::vint::write(0x10000000, buffer));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x10000000, 5),
            tide::vint::read(buffer));
    EXPECT_EQ(5, tide::vint::write(0x7FFFFFFFFl, buffer));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x7FFFFFFFFl, 5),
            tide::vint::read(buffer));
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(6, tide::vint::write(0x800000000l, buffer));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x800000000l, 6),
            tide::vint::read(buffer));
    EXPECT_EQ(6, tide::vint::write(0X3FFFFFFFFFFl, buffer));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0X3FFFFFFFFFFl, 6),
            tide::vint::read(buffer));
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(7, tide::vint::write(0x40000000000l, buffer));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x40000000000l, 7),
            tide::vint::read(buffer));
    EXPECT_EQ(7, tide::vint::write(0X1FFFFFFFFFFFFl, buffer));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0X1FFFFFFFFFFFFl, 7),
            tide::vint::read(buffer));
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(8, tide::vint::write(0x2000000000000l, buffer));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0x2000000000000l, 8),
            tide::vint::read(buffer));
    EXPECT_EQ(8, tide::vint::write(0X0FFFFFFFFFFFFFFl, buffer));
    EXPECT_PRED_FORMAT2(test_utils::int_pairs_eq, std::make_pair(0X0FFFFFFFFFFFFFFl, 8),
            tide::vint::read(buffer));
}


TEST(VIntStream, NoTail)
{
    std::stringstream buffer;
    // 1xxxxxxx - No tail necessary
    buffer.put(0x80);
    EXPECT_NO_THROW(tide::vint::read(buffer));
    // 01xxxxxx xxxxxxxx
    buffer.put(0x40);
    EXPECT_THROW(tide::vint::read(buffer), tide::ReadError);
    // 001xxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x20);
    EXPECT_THROW(tide::vint::read(buffer), tide::ReadError);
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x10);
    EXPECT_THROW(tide::vint::read(buffer), tide::ReadError);
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x08);
    EXPECT_THROW(tide::vint::read(buffer), tide::ReadError);
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x04);
    EXPECT_THROW(tide::vint::read(buffer), tide::ReadError);
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x02);
    EXPECT_THROW(tide::vint::read(buffer), tide::ReadError);
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x01);
    EXPECT_THROW(tide::vint::read(buffer), tide::ReadError);
}


TEST(VIntStream, TailTooShort)
{
    std::stringstream buffer;
    // 1xxxxxxx - No tail necessary
    buffer.put(0x80);
    EXPECT_NO_THROW(tide::vint::read(buffer));
    // 01xxxxxx xxxxxxxx
    buffer.put(0x40);
    EXPECT_THROW(tide::vint::read(buffer), tide::ReadError);
    // 001xxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x20); buffer.put(0x00);
    EXPECT_THROW(tide::vint::read(buffer), tide::ReadError);
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x10); buffer.put(0x00); buffer.put(0x00);
    EXPECT_THROW(tide::vint::read(buffer), tide::ReadError);
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x08); buffer.put(0x00); buffer.put(0x00); buffer.put(0x00);
    EXPECT_THROW(tide::vint::read(buffer), tide::ReadError);
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x04); buffer.put(0x00); buffer.put(0x00); buffer.put(0x00);
    buffer.put(0x00);
    EXPECT_THROW(tide::vint::read(buffer), tide::ReadError);
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x02); buffer.put(0x00); buffer.put(0x00); buffer.put(0x00);
    buffer.put(0x00); buffer.put(0x00);
    EXPECT_THROW(tide::vint::read(buffer), tide::ReadError);
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x01); buffer.put(0x00); buffer.put(0x00); buffer.put(0x00);
    buffer.put(0x00); buffer.put(0x00); buffer.put(0x00);
    EXPECT_THROW(tide::vint::read(buffer), tide::ReadError);
}


TEST(VIntStream, NoMarker)
{
    std::stringstream buffer;
    // 1xxxxxxx - No tail necessary
    buffer.put(0x80);
    EXPECT_NO_THROW(tide::vint::read(buffer));
    // 00000000 xxxxxxxx xxxxxxxx
    buffer.put(0x00);
    EXPECT_THROW(tide::vint::read(buffer), tide::InvalidVarInt);
}


TEST(VIntStream, TooBig)
{
    std::stringstream buffer;
    EXPECT_THROW(tide::vint::write(0x100000000000001, buffer),
            tide::VarIntTooBig);
    EXPECT_EQ(0, buffer.str().size());
    EXPECT_THROW(tide::vint::write(0xFFFFFFFFFFFFFFFF, buffer),
            tide::VarIntTooBig);
    EXPECT_EQ(0, buffer.str().size());
}

