/* Tide
 *
 * EBML class ID management tests.
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
#include <tide/el_ids.h>
#include <tide/exceptions.h>

#include "test_consts.h"
#include "test_utils.h"


TEST(ElID, CodedSize)
{
    EXPECT_EQ(1, tide::ids::coded_size(0x80));
    EXPECT_EQ(1, tide::ids::coded_size(0x81));
    EXPECT_EQ(1, tide::ids::coded_size(0xFE));
    EXPECT_THROW(tide::ids::coded_size(0xFF), tide::InvalidEBMLID);
    // 01xxxxxx xxxxxxxx
    EXPECT_EQ(2, tide::ids::coded_size(0x4000));
    EXPECT_EQ(2, tide::ids::coded_size(0x7FFE));
    EXPECT_THROW(tide::ids::coded_size(0x7FFF), tide::InvalidEBMLID);
    // 001xxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(3, tide::ids::coded_size(0x200000));
    EXPECT_EQ(3, tide::ids::coded_size(0x3FFFFE));
    EXPECT_THROW(tide::ids::coded_size(0x3FFFFF), tide::InvalidEBMLID);
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(4, tide::ids::coded_size(0x10000000));
    EXPECT_EQ(4, tide::ids::coded_size(0x1FFFFFFE));
    EXPECT_THROW(tide::ids::coded_size(0x1FFFFFFF), tide::InvalidEBMLID);
    /* Uncomment this if EBML IDs expand to 64 bits.
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(5, tide::ids::coded_size(0x0800000000));
    EXPECT_EQ(5, tide::ids::coded_size(0x0FFFFFFFFE));
    EXPECT_THROW(tide::ids::coded_size(0x0FFFFFFFFF), tide::InvalidEBMLID);
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(6, tide::ids::coded_size(0x040000000000));
    EXPECT_EQ(6, tide::ids::coded_size(0X07FFFFFFFFFE));
    EXPECT_THROW(tide::ids::coded_size(0x07FFFFFFFFFF), tide::InvalidEBMLID);
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(7, tide::ids::coded_size(0x02000000000000));
    EXPECT_EQ(7, tide::ids::coded_size(0X03FFFFFFFFFFFE));
    EXPECT_THROW(tide::ids::coded_size(0x03FFFFFFFFFFFF), tide::InvalidEBMLID);
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(8, tide::ids::coded_size(0x0100000000000000));
    EXPECT_EQ(8, tide::ids::coded_size(0X01FFFFFFFFFFFFFE));
    EXPECT_THROW(tide::ids::coded_size(0x01FFFFFFFFFFFFFFF),
            tide::InvalidEBMLID);
    */
}


TEST(ElID, Encode)
{
    std::vector<char> expected;
    // 1xxxxxxx
    expected.assign(1, 0x80);
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ids::encode(0x80));
    expected[0] = 0x81;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ids::encode(0x81));
    expected[0] = 0x97;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ids::encode(0x97));
    expected[0] = 0xC0;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ids::encode(0xC0));
    expected[0] = 0xFE;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ids::encode(0xFE));
    EXPECT_THROW(tide::ids::encode(0xFF), tide::InvalidEBMLID);
    // 01xxxxxx xxxxxxxx
    expected.assign(2, 0);
    expected[0] = 0x40;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ids::encode(0x4000));
    expected[0] = 0x4B; expected[1] = 0x35;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ids::encode(0x4B35));
    expected[0] = 0x7F; expected[1] = 0xFE;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ids::encode(0x7FFE));
    EXPECT_THROW(tide::ids::encode(0x7FFF), tide::InvalidEBMLID);
    /* Uncomment this if EBML IDs expand to 64 bits.
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    expected.assign(7, 0xFF);
    expected[0] = 0x01;
    expected[7] = 0xFE;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ids::encode(0x01FFFFFFFFFFFFFE));
    EXPECT_THROW(tide::ids::encode(0x01FFFFFFFFFFFFFF, buffer),
            tide::InvalidEBMLID);
    */
    // EBML tag
    expected.assign(4, 0x1A);
    expected[1] = 0x45; expected[2] = 0xDF; expected[3] = 0xA3;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            tide::ids::encode(0x1A45DFA3));
}


TEST(ElID, Decode)
{
    std::vector<char> buffer;
    tide::ids::DecodeResult r;
    // 1xxxxxxx
    buffer.assign(1, 0x80);
    r = tide::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x80);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer[0] = 0x81;
    r = tide::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x81);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer[0] = 0x97;
    r = tide::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x97);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer[0] = 0xC0;
    r = tide::ids::decode(buffer);
    EXPECT_EQ(r.first, 0xC0);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer[0] = 0xFE;
    r = tide::ids::decode(buffer);
    EXPECT_EQ(r.first, 0xFE);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer[0] = 0xFF;
    EXPECT_THROW(tide::ids::decode(buffer), tide::InvalidEBMLID);
    // 01xxxxxx xxxxxxxx
    buffer.assign(2, 0);
    buffer[0] = 0x40;
    r = tide::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x4000);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer[0] = 0x40; buffer[1] = 0x01;;
    r = tide::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x4001);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer[0] = 0x4B; buffer[1] = 0x35;;
    r = tide::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x4B35);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer[0] = 0x7F; buffer[1] = 0xFE;;
    r = tide::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x7FFE);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer[0] = 0x7F; buffer[1] = 0xFF;;
    EXPECT_THROW(tide::ids::decode(buffer), tide::InvalidEBMLID);
    /* Uncomment this if EBML IDs expand to 64 bits.
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.assign(8, 0);
    buffer[0] = 0x01;
    r = tide::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x0100000000000000);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer[7] = 0x01;
    r = tide::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x0100000000000001);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer.assign(8, 0xFF);
    buffer[0] = 0x01;
    buffer[7] = 0xFE;
    r = tide::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x01FFFFFFFFFFFFFE);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer[7] = 0xFF;
    EXPECT_THROW(tide::ids::decode(buffer), tide::InvalidEBMLID);
    */
    // EBML tag
    buffer.assign(4, 0x1A);
    buffer[1] = 0x45; buffer[2] = 0xDF; buffer[3] = 0xA3;
    r = tide::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x1A45DFA3);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
}


TEST(ElID, EncodeDecode)
{
    std::vector<char> buffer;
    tide::ids::DecodeResult r;
    // 1xxxxxxx
    buffer = tide::ids::encode(0x80);
    r = tide::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x80);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = tide::ids::encode(0x81);
    r = tide::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x81);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = tide::ids::encode(0xFE);
    r = tide::ids::decode(buffer);
    EXPECT_EQ(r.first, 0xFE);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    // 01xxxxxx xxxxxxxx
    buffer = tide::ids::encode(0x4000);
    r = tide::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x4000);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = tide::ids::encode(0x7FFE);
    r = tide::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x7FFE);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    // 001xxxxx xxxxxxxx xxxxxxxx
    buffer = tide::ids::encode(0x200000);
    r = tide::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x200000);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = tide::ids::encode(0x3FFFFE);
    r = tide::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x3FFFFE);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer = tide::ids::encode(0x10000000);
    r = tide::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x10000000);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = tide::ids::encode(0x1FFFFFFE);
    r = tide::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x1FFFFFFE);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    /* Uncomment this if EBML IDs expand to 64 bits.
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer = tide::ids::encode(0x0800000000);
    r = tide::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x0800000000);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = tide::ids::encode(0x0FFFFFFFFE);
    r = tide::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x0FFFFFFFFE);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer = tide::ids::encode(0x040000000000);
    r = tide::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x040000000000);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = tide::ids::encode(0X07FFFFFFFFFE);
    r = tide::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x07FFFFFFFFFE);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer = tide::ids::encode(0x02000000000000);
    r = tide::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x02000000000000);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = tide::ids::encode(0X03FFFFFFFFFFFE);
    r = tide::ids::decode(buffer);
    EXPECT_EQ(r.first, 0X03FFFFFFFFFFFE);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer = tide::ids::encode(0x0100000000000000);
    r = tide::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x0100000000000000);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = tide::ids::encode(0X01FFFFFFFFFFFFFE);
    r = tide::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x01FFFFFFFFFFFFFE);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    */
}


TEST(ElID, NoTail)
{
    std::vector<char> buffer(1);
    // 1xxxxxxx - No tail necessary
    buffer[0] = 0x80;
    EXPECT_NO_THROW(tide::ids::decode(buffer));
    // 01xxxxxx xxxxxxxx
    buffer[0] = 0x40;
    EXPECT_THROW(tide::ids::decode(buffer), tide::BufferTooSmall);
    // 001xxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x20;
    EXPECT_THROW(tide::ids::decode(buffer), tide::BufferTooSmall);
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x10;
    EXPECT_THROW(tide::ids::decode(buffer), tide::BufferTooSmall);
    /* Uncomment this if EBML IDs expand to 64 bits.
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x08;
    EXPECT_THROW(tide::ids::decode(buffer), tide::BufferTooSmall);
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x04;
    EXPECT_THROW(tide::ids::decode(buffer), tide::BufferTooSmall);
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x02;
    EXPECT_THROW(tide::ids::decode(buffer), tide::BufferTooSmall);
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x01;
    EXPECT_THROW(tide::ids::decode(buffer), tide::BufferTooSmall);
    */
}


TEST(ElID, TailTooShort)
{
    std::vector<char> buffer(1);
    // 1xxxxxxx - No tail necessary
    buffer[0] = 0x80;
    EXPECT_NO_THROW(tide::ids::decode(buffer));
    // 01xxxxxx xxxxxxxx
    buffer[0] = 0x40;
    EXPECT_THROW(tide::ids::decode(buffer), tide::BufferTooSmall);
    // 001xxxxx xxxxxxxx xxxxxxxx
    buffer.assign(2, 0);
    buffer[0] = 0x20;
    EXPECT_THROW(tide::ids::decode(buffer), tide::BufferTooSmall);
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.assign(3, 0);
    buffer[0] = 0x10;
    EXPECT_THROW(tide::ids::decode(buffer), tide::BufferTooSmall);
    /* Uncomment this if EBML IDs expand to 64 bits.
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.assign(4, 0);
    buffer[0] = 0x08;
    EXPECT_THROW(tide::ids::decode(buffer), tide::BufferTooSmall);
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.assign(5, 0);
    buffer[0] = 0x04;
    EXPECT_THROW(tide::ids::decode(buffer), tide::BufferTooSmall);
    // [0] = 01x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxx
    buffer.assign(6, 0);
    buffer[0] = 0x02;
    EXPECT_THROW[0] = ::ids::decode(buffer), tide::BufferTooSmall;
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.assign(7, 0);
    buffer[0] = 0x01;
    EXPECT_THROW(tide::ids::decode(buffer), tide::BufferTooSmall);
    */
}


TEST(ElID, NoMarker)
{
    std::vector<char> buffer(1);
    // 1xxxxxxx - Success
    buffer[0] = 0x80;
    EXPECT_NO_THROW(tide::ids::decode(buffer));
    // 00000000 xxxxxxxx xxxxxxxx
    buffer[0] = 0x00;
    EXPECT_THROW(tide::ids::decode(buffer), tide::InvalidVarInt);
}


TEST(ElIDStream, Write)
{
    std::ostringstream buffer;
    std::ostringstream expected;
    // 1xxxxxxx
    expected.put(0x80);
    EXPECT_EQ(1, tide::ids::write(0x80, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    expected.put(0x81);
    EXPECT_EQ(1, tide::ids::write(0x81, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    expected.put(0x97);
    EXPECT_EQ(1, tide::ids::write(0x97, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    expected.put(0xC0);
    EXPECT_EQ(1, tide::ids::write(0xC0, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    expected.put(0xFE);
    EXPECT_EQ(1, tide::ids::write(0xFE, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    EXPECT_THROW(tide::ids::write(0xFF, buffer), tide::InvalidEBMLID);
    // 01xxxxxx xxxxxxxx
    expected.put(0x40); expected.put(0x00);
    EXPECT_EQ(2, tide::ids::write(0x4000, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    expected.put(0x4B); expected.put(0x35);
    EXPECT_EQ(2, tide::ids::write(0x4B35, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    expected.put(0x7F); expected.put(0xFE);
    EXPECT_EQ(2, tide::ids::write(0x7FFE, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    EXPECT_THROW(tide::ids::write(0x7FFF, buffer), tide::InvalidEBMLID);
    /* Uncomment this if EBML IDs expand to 64 bits.
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    expected.put(0x01);
    for (int ii(0); ii < 6; ii++)
    {
        expected.put(0xFF);
    }
    expected.put(0xFE);
    EXPECT_EQ(8, tide::ids::write(0x01FFFFFFFFFFFFFE, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    EXPECT_THROW(tide::ids::write(0x01FFFFFFFFFFFFFF, buffer),
            tide::InvalidEBMLID);
    */
    // EBML tag
    expected.put(0x1A); expected.put(0x45); expected.put(0xDF);
    expected.put(0xA3);
    EXPECT_EQ(4, tide::ids::write(0x1A45DFA3, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
}


TEST(ElIDStream, Read)
{
    std::stringstream buffer;
    tide::ids::ReadResult r;
    // 1xxxxxxx
    buffer.put(0x80);
    r = tide::ids::read(buffer);
    EXPECT_EQ(r.first, 0x80);
    EXPECT_EQ(r.second, 1);
    buffer.put(0x81);
    r = tide::ids::read(buffer);
    EXPECT_EQ(r.first, 0x81);
    EXPECT_EQ(r.second, 1);
    buffer.put(0x97);
    r = tide::ids::read(buffer);
    EXPECT_EQ(r.first, 0x97);
    EXPECT_EQ(r.second, 1);
    buffer.put(0xC0);
    r = tide::ids::read(buffer);
    EXPECT_EQ(r.first, 0xC0);
    EXPECT_EQ(r.second, 1);
    buffer.put(0xFE);
    r = tide::ids::read(buffer);
    EXPECT_EQ(r.first, 0xFE);
    EXPECT_EQ(r.second, 1);
    buffer.put(0xFF);
    EXPECT_THROW(tide::ids::read(buffer), tide::InvalidEBMLID);
    // 01xxxxxx xxxxxxxx
    buffer.put(0x40); buffer.put(0x00);;
    r = tide::ids::read(buffer);
    EXPECT_EQ(r.first, 0x4000);
    EXPECT_EQ(r.second, 2);
    buffer.put(0x40); buffer.put(0x01);;
    r = tide::ids::read(buffer);
    EXPECT_EQ(r.first, 0x4001);
    EXPECT_EQ(r.second, 2);
    buffer.put(0x4B); buffer.put(0x35);;
    r = tide::ids::read(buffer);
    EXPECT_EQ(r.first, 0x4B35);
    EXPECT_EQ(r.second, 2);
    buffer.put(0x7F); buffer.put(0xFE);;
    r = tide::ids::read(buffer);
    EXPECT_EQ(r.first, 0x7FFE);
    EXPECT_EQ(r.second, 2);
    buffer.put(0x7F); buffer.put(0xFF);;
    EXPECT_THROW(tide::ids::read(buffer), tide::InvalidEBMLID);
    /* Uncomment this if EBML IDs expand to 64 bits.
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x01);
    for (int ii(0); ii < 7; ii++)
    {
        buffer.put(0x00);
    }
    r = tide::ids::read(buffer);
    EXPECT_EQ(r.first, 0x0100000000000000);
    EXPECT_EQ(r.second, 8);
    buffer.put(0x01);
    for (int ii(0); ii < 6; ii++)
    {
        buffer.put(0x00);
    }
    buffer.put(0x01);
    r = tide::ids::read(buffer);
    EXPECT_EQ(r.first, 0x0100000000000001);
    EXPECT_EQ(r.second, 8);
    buffer.put(0x01);
    for (int ii(0); ii < 6; ii++)
    {
        buffer.put(0xFF);
    }
    buffer.put(0xFE);
    r = tide::ids::read(buffer);
    EXPECT_EQ(r.first, 0x01FFFFFFFFFFFFFE);
    EXPECT_EQ(r.second, 8);
    buffer.put(0x01);
    for (int ii(0); ii < 7; ii++)
    {
        buffer.put(0xFF);
    }
    EXPECT_THROW(tide::ids::read(buffer), tide::InvalidEBMLID);
    */
    // EBML tag
    buffer.put(0x1A); buffer.put(0x45); buffer.put(0xDF); buffer.put(0xA3);
    r = tide::ids::read(buffer);
    EXPECT_EQ(r.first, 0x1A45DFA3);
    EXPECT_EQ(r.second, 4);
}


TEST(ElIDStream, WriteRead)
{
    std::stringstream buffer;
    tide::ids::ReadResult r;
    // 1xxxxxxx
    EXPECT_EQ(1, tide::ids::write(0x80, buffer));
    r = tide::ids::read(buffer);
    EXPECT_EQ(r.first, 0x80);
    EXPECT_EQ(r.second, 1);
    EXPECT_EQ(1, tide::ids::write(0x81, buffer));
    r = tide::ids::read(buffer);
    EXPECT_EQ(r.first, 0x81);
    EXPECT_EQ(r.second, 1);
    EXPECT_EQ(1, tide::ids::write(0xFE, buffer));
    r = tide::ids::read(buffer);
    EXPECT_EQ(r.first, 0xFE);
    EXPECT_EQ(r.second, 1);
    // 01xxxxxx xxxxxxxx
    EXPECT_EQ(2, tide::ids::write(0x4000, buffer));
    r = tide::ids::read(buffer);
    EXPECT_EQ(r.first, 0x4000);
    EXPECT_EQ(r.second, 2);
    EXPECT_EQ(2, tide::ids::write(0x7FFE, buffer));
    r = tide::ids::read(buffer);
    EXPECT_EQ(r.first, 0x7FFE);
    EXPECT_EQ(r.second, 2);
    // 001xxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(3, tide::ids::write(0x200000, buffer));
    r = tide::ids::read(buffer);
    EXPECT_EQ(r.first, 0x200000);
    EXPECT_EQ(r.second, 3);
    EXPECT_EQ(3, tide::ids::write(0x3FFFFE, buffer));
    r = tide::ids::read(buffer);
    EXPECT_EQ(r.first, 0x3FFFFE);
    EXPECT_EQ(r.second, 3);
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(4, tide::ids::write(0x10000000, buffer));
    r = tide::ids::read(buffer);
    EXPECT_EQ(r.first, 0x10000000);
    EXPECT_EQ(r.second, 4);
    EXPECT_EQ(4, tide::ids::write(0x1FFFFFFE, buffer));
    r = tide::ids::read(buffer);
    EXPECT_EQ(r.first, 0x1FFFFFFE);
    EXPECT_EQ(r.second, 4);
    /* Uncomment this if EBML IDs expand to 64 bits.
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(5, tide::ids::write(0x0800000000, buffer));
    r = tide::ids::read(buffer);
    EXPECT_EQ(r.first, 0x0800000000);
    EXPECT_EQ(r.second, 5);
    EXPECT_EQ(5, tide::ids::write(0x0FFFFFFFFE, buffer));
    r = tide::ids::read(buffer);
    EXPECT_EQ(r.first, 0x0FFFFFFFFE);
    EXPECT_EQ(r.second, 5);
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(6, tide::ids::write(0x040000000000, buffer));
    r = tide::ids::read(buffer);
    EXPECT_EQ(r.first, 0x040000000000);
    EXPECT_EQ(r.second, 6);
    EXPECT_EQ(6, tide::ids::write(0X07FFFFFFFFFE, buffer));
    r = tide::ids::read(buffer);
    EXPECT_EQ(r.first, 0x07FFFFFFFFFE);
    EXPECT_EQ(r.second, 6);
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(7, tide::ids::write(0x02000000000000, buffer));
    r = tide::ids::read(buffer);
    EXPECT_EQ(r.first, 0x02000000000000);
    EXPECT_EQ(r.second, 7);
    EXPECT_EQ(7, tide::ids::write(0X03FFFFFFFFFFFE, buffer));
    r = tide::ids::read(buffer);
    EXPECT_EQ(r.first, 0X03FFFFFFFFFFFE);
    EXPECT_EQ(r.second, 7);
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(8, tide::ids::write(0x0100000000000000, buffer));
    r = tide::ids::read(buffer);
    EXPECT_EQ(r.first, 0x0100000000000000);
    EXPECT_EQ(r.second, 8);
    EXPECT_EQ(8, tide::ids::write(0X01FFFFFFFFFFFFFE, buffer));
    r = tide::ids::read(buffer);
    EXPECT_EQ(r.first, 0x01FFFFFFFFFFFFFE);
    EXPECT_EQ(r.second, 8);
    */
}


TEST(ElIDStream, NoTail)
{
    std::stringstream buffer;
    // 1xxxxxxx - No tail necessary
    buffer.put(0x80);
    EXPECT_NO_THROW(tide::ids::read(buffer));
    // 01xxxxxx xxxxxxxx
    buffer.put(0x40);
    EXPECT_THROW(tide::ids::read(buffer), tide::ReadError);
    // 001xxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x20);
    EXPECT_THROW(tide::ids::read(buffer), tide::ReadError);
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x10);
    EXPECT_THROW(tide::ids::read(buffer), tide::ReadError);
    /* Uncomment this if EBML IDs expand to 64 bits.
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x08);
    EXPECT_THROW(tide::ids::read(buffer), tide::ReadError);
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x04);
    EXPECT_THROW(tide::ids::read(buffer), tide::ReadError);
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x02);
    EXPECT_THROW(tide::ids::read(buffer), tide::ReadError);
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x01);
    EXPECT_THROW(tide::ids::read(buffer), tide::ReadError);
    */
}


TEST(ElIDStream, TailTooShort)
{
    std::stringstream buffer;
    // 1xxxxxxx - No tail necessary
    buffer.put(0x80);
    EXPECT_NO_THROW(tide::ids::read(buffer));
    // 01xxxxxx xxxxxxxx
    buffer.put(0x40);
    EXPECT_THROW(tide::ids::read(buffer), tide::ReadError);
    // 001xxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x20); buffer.put(0x00);
    EXPECT_THROW(tide::ids::read(buffer), tide::ReadError);
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x10); buffer.put(0x00); buffer.put(0x00);
    EXPECT_THROW(tide::ids::read(buffer), tide::ReadError);
    /* Uncomment this if EBML IDs expand to 64 bits.
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x08); buffer.put(0x00); buffer.put(0x00); buffer.put(0x00);
    EXPECT_THROW(tide::ids::read(buffer), tide::ReadError);
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x04); buffer.put(0x00); buffer.put(0x00); buffer.put(0x00);
    buffer.put(0x00);
    EXPECT_THROW(tide::ids::read(buffer), tide::ReadError);
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x02); buffer.put(0x00); buffer.put(0x00); buffer.put(0x00);
    buffer.put(0x00); buffer.put(0x00);
    EXPECT_THROW(tide::ids::read(buffer), tide::ReadError);
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x01); buffer.put(0x00); buffer.put(0x00); buffer.put(0x00);
    buffer.put(0x00); buffer.put(0x00); buffer.put(0x00);
    EXPECT_THROW(tide::ids::read(buffer), tide::ReadError);
    */
}


TEST(ElIDStream, NoMarker)
{
    std::stringstream buffer;
    // 1xxxxxxx - No tail necessary
    buffer.put(0x80);
    EXPECT_NO_THROW(tide::ids::read(buffer));
    // 00000000 xxxxxxxx ...
    buffer.put(0x00);
    EXPECT_THROW(tide::ids::read(buffer), tide::InvalidVarInt);
}

