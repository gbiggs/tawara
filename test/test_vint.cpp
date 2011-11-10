/* Tide
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

#include <gtest/gtest.h>
#include <tide/exceptions.h>
#include <tide/vint.h>

#include "test_consts.h"
#include "test_utils.h"


TEST(VInt, Encode)
{
    std::vector<char> expected(1);
    // 1xxxxxxx
    expected[0] = 0x80;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::vint::encode(0x00));
    expected[0] = 0x81;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::vint::encode(0x01));
    expected[0] = 0x97;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::vint::encode(0x17));
    expected[0] = 0xC0;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::vint::encode(0x40));
    expected[0] = 0xFF;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::vint::encode(0x7F));
    // 01xxxxxx xxxxxxxx
    expected.assign(2, 0);
    expected[0] = 0x4B; expected[1] = 0x35;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::vint::encode(0x0B35));
    expected[0] = 0x60; expected[1] = 0x00;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::vint::encode(0x2000));
    expected[0] = 0x7F; expected[1] = 0xFF;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::vint::encode(0x3FFF));
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    expected.assign(8, 0xFF);
    expected[0] = 0x01;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::vint::encode(0xFFFFFFFFFFFFFF));
    // EBML tag
    expected.assign(4, 0);
    expected[0] = 0x1A; expected[1] = 0x45; expected[2] = 0xDF;
    expected[3] = 0xA3;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::vint::encode(0x0A45DFA3));
    // The remainder are done in the EncodeDecode test for simplicity
}


TEST(VInt, EncodeWithSize)
{
    std::vector<char> expected;

    // 1-byte integer in 2 bytes
    expected.assign(2, 0);
    expected[0] = 0x40; expected[1] = 0x09;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::vint::encode(0x09, 2));
    EXPECT_EQ(2, tide::vint::encode(0x09, 2).size());
    // 1-byte integer in 8 bytes
    expected.assign(8, 0);
    expected[0] = 0x01; expected[1] = 0x00; expected[7] = 0x09;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::vint::encode(0x09, 8));
    EXPECT_EQ(8, tide::vint::encode(0x09, 8).size());
    // 3-byte integer in 5 bytes
    expected.assign(5, 0x01);
    expected[0] = 0x08; expected[1] = 0x00;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::vint::encode(0x010101, 5));
    EXPECT_EQ(5, tide::vint::encode(0x010101, 5).size());
    // 7-byte integer in 8 bytes
    expected.assign(8, 0x02);
    expected[0] = 0x01;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::vint::encode(0x02020202020202, 8));
    EXPECT_EQ(8, tide::vint::encode(0x02020202020202, 8).size());

    // Test throwing
    EXPECT_THROW(tide::vint::encode(0x0101, 1), tide::SpecSizeTooSmall);
}


TEST(VInt, Decode)
{
    std::vector<char> buffer(2);
    tide::vint::DecodeResult r;
    // 1xxxxxxx
    buffer[0] = 0x80;
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0x00);
    EXPECT_TRUE(r.second == buffer.begin() + 1) << "Iterator not after data";
    buffer[0] = 0x81;
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0x01);
    EXPECT_TRUE(r.second == buffer.begin() + 1) << "Iterator not after data";
    buffer[0] = 0x97;
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0x17);
    EXPECT_TRUE(r.second == buffer.begin() + 1) << "Iterator not after data";
    buffer[0] = 0xC0;
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0x40);
    EXPECT_TRUE(r.second == buffer.begin() + 1) << "Iterator not after data";
    buffer[0] = 0xFF;
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0x7F);
    EXPECT_TRUE(r.second == buffer.begin() + 1) << "Iterator not after data";
    // 01xxxxxx xxxxxxxx
    buffer[0] = 0x40; buffer[1] = 0x00;
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0x0000);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer[0] = 0x40; buffer[1] = 0x01;
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0x0001);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer[0] = 0x4B; buffer[1] = 0x35;
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0x0B35);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer[0] = 0x60; buffer[1] = 0x00;
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0x2000);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer[0] = 0x7F; buffer[1] = 0xFF;
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0x3FFF);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.assign(8, 0);
    buffer[0] = 0x01;
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0x00);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer[7] = 0x01;
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0x01);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer.assign(8, 0xFF);
    buffer[0] = 0x01;
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0xFFFFFFFFFFFFFF);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    // EBML tag
    buffer.assign(4, 0);
    buffer[0] = 0x1A; buffer[1] = 0x45; buffer[2] = 0xDF; buffer[3] = 0xA3;
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0x0A45DFA3);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    // The remainder are done in the EncodeDecode test for simplicity
}


TEST(VInt, EncodeDecode)
{
    std::vector<char> buffer;
    tide::vint::DecodeResult r;
    // 1xxxxxxx
    buffer = tide::vint::encode(0x00);
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0x00);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = tide::vint::encode(0x01);
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0x01);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = tide::vint::encode(0x7F);
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0x7F);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    // 01xxxxxx xxxxxxxx
    buffer = tide::vint::encode(0x80);
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0x80);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = tide::vint::encode(0x81);
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0x81);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = tide::vint::encode(0x3FFF);
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0x3FFF);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    // 001xxxxx xxxxxxxx xxxxxxxx
    buffer = tide::vint::encode(0x4000);
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0x4000);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = tide::vint::encode(0x60000);
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0x60000);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = tide::vint::encode(0x1FFFFF);
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0x1FFFFF);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer = tide::vint::encode(0x200000);
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0x200000);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = tide::vint::encode(0xFFFFFFF);
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0xFFFFFFF);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer = tide::vint::encode(0x10000000);
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0x10000000);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = tide::vint::encode(0x7FFFFFFFFl);
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0x7FFFFFFFFl);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer = tide::vint::encode(0x800000000l);
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0x800000000l);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = tide::vint::encode(0X3FFFFFFFFFFl);
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0X3FFFFFFFFFFl);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer = tide::vint::encode(0x40000000000l);
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0x40000000000l);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = tide::vint::encode(0X1FFFFFFFFFFFFl);
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0X1FFFFFFFFFFFFl);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer = tide::vint::encode(0x2000000000000l);
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0x2000000000000l);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = tide::vint::encode(0X0FFFFFFFFFFFFFFl);
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0X0FFFFFFFFFFFFFFl);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    // Integers written with extra bytes
    buffer = tide::vint::encode(0x01, 4);
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0x01);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = tide::vint::encode(0x200000, 5);
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0x200000);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = tide::vint::encode(0X1FFFFFFFFFFFFl, 8);
    r = tide::vint::decode(buffer);
    EXPECT_EQ(r.first, 0X1FFFFFFFFFFFFl);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
}


TEST(VInt, NoTail)
{
    std::vector<char> buffer(1);
    // 1xxxxxxx - No tail necessary
    buffer[0] = 0x80;
    EXPECT_NO_THROW(tide::vint::decode(buffer));
    // 01xxxxxx xxxxxxxx
    buffer[0] = 0x40;
    EXPECT_THROW(tide::vint::decode(buffer), tide::BufferTooSmall);
    // 001xxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x20;
    EXPECT_THROW(tide::vint::decode(buffer), tide::BufferTooSmall);
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x10;
    EXPECT_THROW(tide::vint::decode(buffer), tide::BufferTooSmall);
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x08;
    EXPECT_THROW(tide::vint::decode(buffer), tide::BufferTooSmall);
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x04;
    EXPECT_THROW(tide::vint::decode(buffer), tide::BufferTooSmall);
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x02;
    EXPECT_THROW(tide::vint::decode(buffer), tide::BufferTooSmall);
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x01;
    EXPECT_THROW(tide::vint::decode(buffer), tide::BufferTooSmall);
}


TEST(VInt, TailTooShort)
{
    std::vector<char> buffer(1);
    // 1xxxxxxx - No tail necessary
    buffer[0] = 0x80;
    EXPECT_NO_THROW(tide::vint::decode(buffer));
    // 01xxxxxx xxxxxxxx
    buffer[0] = 0x40;
    EXPECT_THROW(tide::vint::decode(buffer), tide::BufferTooSmall);
    // 001xxxxx xxxxxxxx xxxxxxxx
    buffer.assign(2, 0);
    buffer[0] = 0x20;
    EXPECT_THROW(tide::vint::decode(buffer), tide::BufferTooSmall);
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.assign(3, 0);
    buffer[0] = 0x10;
    EXPECT_THROW(tide::vint::decode(buffer), tide::BufferTooSmall);
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.assign(4, 0);
    buffer[0] = 0x08;
    EXPECT_THROW(tide::vint::decode(buffer), tide::BufferTooSmall);
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.assign(5, 0);
    buffer[0] = 0x04;
    EXPECT_THROW(tide::vint::decode(buffer), tide::BufferTooSmall);
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.assign(6, 0);
    buffer[0] = 0x02;
    EXPECT_THROW(tide::vint::decode(buffer), tide::BufferTooSmall);
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.assign(7, 0);
    buffer[0] = 0x01;
    EXPECT_THROW(tide::vint::decode(buffer), tide::BufferTooSmall);
}


TEST(VInt, NoMarker)
{
    std::vector<char> buffer(1);
    // 1xxxxxxx - Success
    buffer[0] = 0x80;
    EXPECT_NO_THROW(tide::vint::decode(buffer));
    // 00000000 xxxxxxxx xxxxxxxx
    buffer[0] = 0x00;
    EXPECT_THROW(tide::vint::decode(buffer), tide::InvalidVarInt);
}


TEST(VInt, TooBig)
{
    EXPECT_THROW(tide::vint::encode(0x100000000000001),
            tide::VarIntTooBig);
    EXPECT_THROW(tide::vint::encode(0xFFFFFFFFFFFFFFFF),
            tide::VarIntTooBig);
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
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    expected.put(0x81);
    EXPECT_EQ(1, tide::vint::write(0x01, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    expected.put(0x97);
    EXPECT_EQ(1, tide::vint::write(0x17, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    expected.put(0xC0);
    EXPECT_EQ(1, tide::vint::write(0x40, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    expected.put(0xFF);
    EXPECT_EQ(1, tide::vint::write(0x7F, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    // 01xxxxxx xxxxxxxx
    expected.put(0x80);
    EXPECT_EQ(1, tide::vint::write(0x0000, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    expected.put(0x81);
    EXPECT_EQ(1, tide::vint::write(0x0001, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    expected.put(0x4B); expected.put(0x35);
    EXPECT_EQ(2, tide::vint::write(0x0B35, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    expected.put(0x60); expected.put(0x00);
    EXPECT_EQ(2, tide::vint::write(0x2000, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    expected.put(0x7F); expected.put(0xFF);
    EXPECT_EQ(2, tide::vint::write(0x3FFF, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    expected.put(0x80);
    EXPECT_EQ(1, tide::vint::write(0x0000000000000000, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    expected.put(0x81);
    EXPECT_EQ(1, tide::vint::write(0x0000000000000001, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    expected.put(0x01);
    for (int ii(0); ii < 7; ii++)
    {
        expected.put(0xFF);
    }
    EXPECT_EQ(8, tide::vint::write(0xFFFFFFFFFFFFFF, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    // EBML tag
    expected.put(0x1A); expected.put(0x45); expected.put(0xDF);
    expected.put(0xA3);
    EXPECT_EQ(4, tide::vint::write(0x0A45DFA3, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    // The remainder are done in the EncodeDecode test for simplicity
}


TEST(VInt, WriteWithSize)
{
    std::ostringstream buffer;
    std::ostringstream expected;

    // 1-byte integer in 2 bytes
    expected.put(0x40); expected.put(0x09);
    EXPECT_EQ(2, tide::vint::write(0x09, buffer, 2));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    // 1-byte integer in 8 bytes
    expected.put(0x01);
    for(int ii(0); ii < 6; ++ii) { expected.put(0x00); }
    expected.put(0x09);
    EXPECT_EQ(8, tide::vint::write(0x09, buffer, 8));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    // 3-byte integer in 5 bytes
    expected.put(0x08); expected.put(0x00);
    expected.put(0x01); expected.put(0x01); expected.put(0x01);
    EXPECT_EQ(5, tide::vint::write(0x010101, buffer, 5));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    // 7-byte integer in 8 bytes
    expected.put(0x01);
    for(int ii(0); ii < 7; ++ii) { expected.put(0x02); }
    EXPECT_EQ(8, tide::vint::write(0x02020202020202, buffer, 8));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());

    // Test throwing
    EXPECT_THROW(tide::vint::write(0x0101, buffer, 1), tide::SpecSizeTooSmall);
}


TEST(VIntStream, Read)
{
    std::stringstream buffer;
    tide::vint::ReadResult r;
    // 1xxxxxxx
    buffer.put(0x80);
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x00);
    EXPECT_EQ(r.second, 1);
    buffer.put(0x81);
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x01);
    EXPECT_EQ(r.second, 1);
    buffer.put(0x97);
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x17);
    EXPECT_EQ(r.second, 1);
    buffer.put(0xC0);
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x40);
    EXPECT_EQ(r.second, 1);
    buffer.put(0xFF);
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x7F);
    EXPECT_EQ(r.second, 1);
    // 01xxxxxx xxxxxxxx
    buffer.put(0x40); buffer.put(0x00);;
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x0000);
    EXPECT_EQ(r.second, 2);
    buffer.put(0x40); buffer.put(0x01);;
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x0001);
    EXPECT_EQ(r.second, 2);
    buffer.put(0x4B); buffer.put(0x35);;
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x0B35);
    EXPECT_EQ(r.second, 2);
    buffer.put(0x60); buffer.put(0x00);;
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x2000);
    EXPECT_EQ(r.second, 2);
    buffer.put(0x7F); buffer.put(0xFF);;
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x3FFF);
    EXPECT_EQ(r.second, 2);
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x01);
    for (int ii(0); ii < 7; ii++)
    {
        buffer.put(0x00);
    }
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x00000000);
    EXPECT_EQ(r.second, 8);
    buffer.put(0x01);
    for (int ii(0); ii < 6; ii++)
    {
        buffer.put(0x00);
    }
    buffer.put(0x01);
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x00000001);
    EXPECT_EQ(r.second, 8);
    buffer.put(0x01);
    for (int ii(0); ii < 7; ii++)
    {
        buffer.put(0xFF);
    }
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0xFFFFFFFFFFFFFF);
    EXPECT_EQ(r.second, 8);
    // EBML tag
    buffer.put(0x1A); buffer.put(0x45); buffer.put(0xDF); buffer.put(0xA3);
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x0A45DFA3);
    EXPECT_EQ(r.second, 4);
    // The remainder are done in the EncodeDecode test for simplicity
}


TEST(VIntStream, WriteRead)
{
    std::stringstream buffer;
    tide::vint::ReadResult r;
    // 1xxxxxxx
    EXPECT_EQ(1, tide::vint::write(0x00, buffer));
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x00);
    EXPECT_EQ(r.second, 1);
    EXPECT_EQ(1, tide::vint::write(0x01, buffer));
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x01);
    EXPECT_EQ(r.second, 1);
    EXPECT_EQ(1, tide::vint::write(0x7F, buffer));
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x7F);
    EXPECT_EQ(r.second, 1);
    // 01xxxxxx xxxxxxxx
    EXPECT_EQ(2, tide::vint::write(0x80, buffer));
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x80);
    EXPECT_EQ(r.second, 2);
    EXPECT_EQ(2, tide::vint::write(0x81, buffer));
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x81);
    EXPECT_EQ(r.second, 2);
    EXPECT_EQ(2, tide::vint::write(0x3FFF, buffer));
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x3FFF);
    EXPECT_EQ(r.second, 2);
    // 001xxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(3, tide::vint::write(0x4000, buffer));
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x4000);
    EXPECT_EQ(r.second, 3);
    EXPECT_EQ(3, tide::vint::write(0x60000, buffer));
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x60000);
    EXPECT_EQ(r.second, 3);
    EXPECT_EQ(3, tide::vint::write(0x1FFFFF, buffer));
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x1FFFFF);
    EXPECT_EQ(r.second, 3);
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(4, tide::vint::write(0x200000, buffer));
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x200000);
    EXPECT_EQ(r.second, 4);
    EXPECT_EQ(4, tide::vint::write(0xFFFFFFF, buffer));
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0xFFFFFFF);
    EXPECT_EQ(r.second, 4);
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(5, tide::vint::write(0x10000000, buffer));
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x10000000);
    EXPECT_EQ(r.second, 5);
    EXPECT_EQ(5, tide::vint::write(0x7FFFFFFFFl, buffer));
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x7FFFFFFFFl);
    EXPECT_EQ(r.second, 5);
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(6, tide::vint::write(0x800000000l, buffer));
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x800000000l);
    EXPECT_EQ(r.second, 6);
    EXPECT_EQ(6, tide::vint::write(0X3FFFFFFFFFFl, buffer));
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0X3FFFFFFFFFFl);
    EXPECT_EQ(r.second, 6);
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(7, tide::vint::write(0x40000000000l, buffer));
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x40000000000l);
    EXPECT_EQ(r.second, 7);
    EXPECT_EQ(7, tide::vint::write(0X1FFFFFFFFFFFFl, buffer));
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0X1FFFFFFFFFFFFl);
    EXPECT_EQ(r.second, 7);
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(8, tide::vint::write(0x2000000000000l, buffer));
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x2000000000000l);
    EXPECT_EQ(r.second, 8);
    EXPECT_EQ(8, tide::vint::write(0X0FFFFFFFFFFFFFFl, buffer));
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0X0FFFFFFFFFFFFFFl);
    EXPECT_EQ(r.second, 8);
    // Integers written with extra bytes
    EXPECT_EQ(4, tide::vint::write(0x01, buffer, 4));
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x01);
    EXPECT_EQ(r.second, 4);
    EXPECT_EQ(7, tide::vint::write(0x10000000, buffer, 7));
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x10000000);
    EXPECT_EQ(r.second, 7);
    EXPECT_EQ(8, tide::vint::write(0x40000000000l, buffer, 8));
    r = tide::vint::read(buffer);
    EXPECT_EQ(r.first, 0x40000000000l);
    EXPECT_EQ(r.second, 8);
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

