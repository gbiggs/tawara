/*
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2011, 2012, Geoffrey Biggs, geoffrey.biggs@aist.go.jp
 *     RT-Synthesis Research Group
 *     Intelligent Systems Research Institute,
 *     National Institute of Advanced Industrial Science and Technology (AIST),
 *     Japan
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Geoffrey Biggs nor AIST, nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#include <gtest/gtest.h>
#include <celduin/el_ids.h>
#include <celduin/exceptions.h>

#include "test_consts.h"
#include "test_utils.h"


TEST(ElID, CodedSize)
{
    EXPECT_EQ(1, celduin::ids::size(0x80));
    EXPECT_EQ(1, celduin::ids::size(0x81));
    EXPECT_EQ(1, celduin::ids::size(0xFE));
    EXPECT_THROW(celduin::ids::size(0xFF), celduin::InvalidEBMLID);
    // 01xxxxxx xxxxxxxx
    EXPECT_EQ(2, celduin::ids::size(0x4000));
    EXPECT_EQ(2, celduin::ids::size(0x7FFE));
    EXPECT_THROW(celduin::ids::size(0x7FFF), celduin::InvalidEBMLID);
    // 001xxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(3, celduin::ids::size(0x200000));
    EXPECT_EQ(3, celduin::ids::size(0x3FFFFE));
    EXPECT_THROW(celduin::ids::size(0x3FFFFF), celduin::InvalidEBMLID);
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(4, celduin::ids::size(0x10000000));
    EXPECT_EQ(4, celduin::ids::size(0x1FFFFFFE));
    EXPECT_THROW(celduin::ids::size(0x1FFFFFFF), celduin::InvalidEBMLID);
    /* Uncomment this if EBML IDs expand to 64 bits.
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(5, celduin::ids::size(0x0800000000));
    EXPECT_EQ(5, celduin::ids::size(0x0FFFFFFFFE));
    EXPECT_THROW(celduin::ids::size(0x0FFFFFFFFF), celduin::InvalidEBMLID);
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(6, celduin::ids::size(0x040000000000));
    EXPECT_EQ(6, celduin::ids::size(0X07FFFFFFFFFE));
    EXPECT_THROW(celduin::ids::size(0x07FFFFFFFFFF), celduin::InvalidEBMLID);
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(7, celduin::ids::size(0x02000000000000));
    EXPECT_EQ(7, celduin::ids::size(0X03FFFFFFFFFFFE));
    EXPECT_THROW(celduin::ids::size(0x03FFFFFFFFFFFF), celduin::InvalidEBMLID);
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(8, celduin::ids::size(0x0100000000000000));
    EXPECT_EQ(8, celduin::ids::size(0X01FFFFFFFFFFFFFE));
    EXPECT_THROW(celduin::ids::size(0x01FFFFFFFFFFFFFFF),
            celduin::InvalidEBMLID);
    */
}


TEST(ElID, Encode)
{
    std::vector<char> expected;
    // 1xxxxxxx
    expected.assign(1, 0x80);
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            celduin::ids::encode(0x80));
    expected[0] = 0x81;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            celduin::ids::encode(0x81));
    expected[0] = 0x97;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            celduin::ids::encode(0x97));
    expected[0] = 0xC0;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            celduin::ids::encode(0xC0));
    expected[0] = 0xFE;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            celduin::ids::encode(0xFE));
    EXPECT_THROW(celduin::ids::encode(0xFF), celduin::InvalidEBMLID);
    // 01xxxxxx xxxxxxxx
    expected.assign(2, 0);
    expected[0] = 0x40;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            celduin::ids::encode(0x4000));
    expected[0] = 0x4B; expected[1] = 0x35;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            celduin::ids::encode(0x4B35));
    expected[0] = 0x7F; expected[1] = 0xFE;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            celduin::ids::encode(0x7FFE));
    EXPECT_THROW(celduin::ids::encode(0x7FFF), celduin::InvalidEBMLID);
    /* Uncomment this if EBML IDs expand to 64 bits.
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    expected.assign(7, 0xFF);
    expected[0] = 0x01;
    expected[7] = 0xFE;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            celduin::ids::encode(0x01FFFFFFFFFFFFFE));
    EXPECT_THROW(celduin::ids::encode(0x01FFFFFFFFFFFFFF, buffer),
            celduin::InvalidEBMLID);
    */
    // EBML tag
    expected.assign(4, 0x1A);
    expected[1] = 0x45; expected[2] = 0xDF; expected[3] = 0xA3;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
            celduin::ids::encode(0x1A45DFA3));
}


TEST(ElID, Decode)
{
    std::vector<char> buffer;
    celduin::ids::DecodeResult r;
    // 1xxxxxxx
    buffer.assign(1, 0x80);
    r = celduin::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x80);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer[0] = 0x81;
    r = celduin::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x81);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer[0] = 0x97;
    r = celduin::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x97);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer[0] = 0xC0;
    r = celduin::ids::decode(buffer);
    EXPECT_EQ(r.first, 0xC0);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer[0] = 0xFE;
    r = celduin::ids::decode(buffer);
    EXPECT_EQ(r.first, 0xFE);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer[0] = 0xFF;
    EXPECT_THROW(celduin::ids::decode(buffer), celduin::InvalidEBMLID);
    // 01xxxxxx xxxxxxxx
    buffer.assign(2, 0);
    buffer[0] = 0x40;
    r = celduin::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x4000);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer[0] = 0x40; buffer[1] = 0x01;;
    r = celduin::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x4001);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer[0] = 0x4B; buffer[1] = 0x35;;
    r = celduin::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x4B35);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer[0] = 0x7F; buffer[1] = 0xFE;;
    r = celduin::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x7FFE);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer[0] = 0x7F; buffer[1] = 0xFF;;
    EXPECT_THROW(celduin::ids::decode(buffer), celduin::InvalidEBMLID);
    /* Uncomment this if EBML IDs expand to 64 bits.
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.assign(8, 0);
    buffer[0] = 0x01;
    r = celduin::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x0100000000000000);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer[7] = 0x01;
    r = celduin::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x0100000000000001);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer.assign(8, 0xFF);
    buffer[0] = 0x01;
    buffer[7] = 0xFE;
    r = celduin::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x01FFFFFFFFFFFFFE);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer[7] = 0xFF;
    EXPECT_THROW(celduin::ids::decode(buffer), celduin::InvalidEBMLID);
    */
    // EBML tag
    buffer.assign(4, 0x1A);
    buffer[1] = 0x45; buffer[2] = 0xDF; buffer[3] = 0xA3;
    r = celduin::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x1A45DFA3);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
}


TEST(ElID, EncodeDecode)
{
    std::vector<char> buffer;
    celduin::ids::DecodeResult r;
    // 1xxxxxxx
    buffer = celduin::ids::encode(0x80);
    r = celduin::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x80);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = celduin::ids::encode(0x81);
    r = celduin::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x81);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = celduin::ids::encode(0xFE);
    r = celduin::ids::decode(buffer);
    EXPECT_EQ(r.first, 0xFE);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    // 01xxxxxx xxxxxxxx
    buffer = celduin::ids::encode(0x4000);
    r = celduin::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x4000);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = celduin::ids::encode(0x7FFE);
    r = celduin::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x7FFE);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    // 001xxxxx xxxxxxxx xxxxxxxx
    buffer = celduin::ids::encode(0x200000);
    r = celduin::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x200000);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = celduin::ids::encode(0x3FFFFE);
    r = celduin::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x3FFFFE);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer = celduin::ids::encode(0x10000000);
    r = celduin::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x10000000);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = celduin::ids::encode(0x1FFFFFFE);
    r = celduin::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x1FFFFFFE);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    /* Uncomment this if EBML IDs expand to 64 bits.
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer = celduin::ids::encode(0x0800000000);
    r = celduin::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x0800000000);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = celduin::ids::encode(0x0FFFFFFFFE);
    r = celduin::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x0FFFFFFFFE);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer = celduin::ids::encode(0x040000000000);
    r = celduin::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x040000000000);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = celduin::ids::encode(0X07FFFFFFFFFE);
    r = celduin::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x07FFFFFFFFFE);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer = celduin::ids::encode(0x02000000000000);
    r = celduin::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x02000000000000);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = celduin::ids::encode(0X03FFFFFFFFFFFE);
    r = celduin::ids::decode(buffer);
    EXPECT_EQ(r.first, 0X03FFFFFFFFFFFE);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer = celduin::ids::encode(0x0100000000000000);
    r = celduin::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x0100000000000000);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    buffer = celduin::ids::encode(0X01FFFFFFFFFFFFFE);
    r = celduin::ids::decode(buffer);
    EXPECT_EQ(r.first, 0x01FFFFFFFFFFFFFE);
    EXPECT_TRUE(r.second == buffer.end()) << "Iterator not after data";
    */
}


TEST(ElID, NoTail)
{
    std::vector<char> buffer(1);
    // 1xxxxxxx - No tail necessary
    buffer[0] = 0x80;
    EXPECT_NO_THROW(celduin::ids::decode(buffer));
    // 01xxxxxx xxxxxxxx
    buffer[0] = 0x40;
    EXPECT_THROW(celduin::ids::decode(buffer), celduin::BufferTooSmall);
    // 001xxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x20;
    EXPECT_THROW(celduin::ids::decode(buffer), celduin::BufferTooSmall);
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x10;
    EXPECT_THROW(celduin::ids::decode(buffer), celduin::BufferTooSmall);
    /* Uncomment this if EBML IDs expand to 64 bits.
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x08;
    EXPECT_THROW(celduin::ids::decode(buffer), celduin::BufferTooSmall);
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x04;
    EXPECT_THROW(celduin::ids::decode(buffer), celduin::BufferTooSmall);
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x02;
    EXPECT_THROW(celduin::ids::decode(buffer), celduin::BufferTooSmall);
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer[0] = 0x01;
    EXPECT_THROW(celduin::ids::decode(buffer), celduin::BufferTooSmall);
    */
}


TEST(ElID, TailTooShort)
{
    std::vector<char> buffer(1);
    // 1xxxxxxx - No tail necessary
    buffer[0] = 0x80;
    EXPECT_NO_THROW(celduin::ids::decode(buffer));
    // 01xxxxxx xxxxxxxx
    buffer[0] = 0x40;
    EXPECT_THROW(celduin::ids::decode(buffer), celduin::BufferTooSmall);
    // 001xxxxx xxxxxxxx xxxxxxxx
    buffer.assign(2, 0);
    buffer[0] = 0x20;
    EXPECT_THROW(celduin::ids::decode(buffer), celduin::BufferTooSmall);
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.assign(3, 0);
    buffer[0] = 0x10;
    EXPECT_THROW(celduin::ids::decode(buffer), celduin::BufferTooSmall);
    /* Uncomment this if EBML IDs expand to 64 bits.
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.assign(4, 0);
    buffer[0] = 0x08;
    EXPECT_THROW(celduin::ids::decode(buffer), celduin::BufferTooSmall);
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.assign(5, 0);
    buffer[0] = 0x04;
    EXPECT_THROW(celduin::ids::decode(buffer), celduin::BufferTooSmall);
    // [0] = 01x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxx
    buffer.assign(6, 0);
    buffer[0] = 0x02;
    EXPECT_THROW[0] = ::ids::decode(buffer), celduin::BufferTooSmall;
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.assign(7, 0);
    buffer[0] = 0x01;
    EXPECT_THROW(celduin::ids::decode(buffer), celduin::BufferTooSmall);
    */
}


TEST(ElID, NoMarker)
{
    std::vector<char> buffer(1);
    // 1xxxxxxx - Success
    buffer[0] = 0x80;
    EXPECT_NO_THROW(celduin::ids::decode(buffer));
    // 00000000 xxxxxxxx xxxxxxxx
    buffer[0] = 0x00;
    EXPECT_THROW(celduin::ids::decode(buffer), celduin::InvalidVarInt);
}


TEST(ElIDStream, Write)
{
    std::ostringstream buffer;
    std::ostringstream expected;
    // 1xxxxxxx
    expected.put(0x80);
    EXPECT_EQ(1, celduin::ids::write(0x80, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    expected.put(0x81);
    EXPECT_EQ(1, celduin::ids::write(0x81, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    expected.put(0x97);
    EXPECT_EQ(1, celduin::ids::write(0x97, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    expected.put(0xC0);
    EXPECT_EQ(1, celduin::ids::write(0xC0, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    expected.put(0xFE);
    EXPECT_EQ(1, celduin::ids::write(0xFE, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    EXPECT_THROW(celduin::ids::write(0xFF, buffer), celduin::InvalidEBMLID);
    // 01xxxxxx xxxxxxxx
    expected.put(0x40); expected.put(0x00);
    EXPECT_EQ(2, celduin::ids::write(0x4000, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    expected.put(0x4B); expected.put(0x35);
    EXPECT_EQ(2, celduin::ids::write(0x4B35, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    expected.put(0x7F); expected.put(0xFE);
    EXPECT_EQ(2, celduin::ids::write(0x7FFE, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    EXPECT_THROW(celduin::ids::write(0x7FFF, buffer), celduin::InvalidEBMLID);
    /* Uncomment this if EBML IDs expand to 64 bits.
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    expected.put(0x01);
    for (int ii(0); ii < 6; ii++)
    {
        expected.put(0xFF);
    }
    expected.put(0xFE);
    EXPECT_EQ(8, celduin::ids::write(0x01FFFFFFFFFFFFFE, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
    EXPECT_THROW(celduin::ids::write(0x01FFFFFFFFFFFFFF, buffer),
            celduin::InvalidEBMLID);
    */
    // EBML tag
    expected.put(0x1A); expected.put(0x45); expected.put(0xDF);
    expected.put(0xA3);
    EXPECT_EQ(4, celduin::ids::write(0x1A45DFA3, buffer));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            buffer.str());
}


TEST(ElIDStream, Read)
{
    std::stringstream buffer;
    celduin::ids::ReadResult r;
    // 1xxxxxxx
    buffer.put(0x80);
    r = celduin::ids::read(buffer);
    EXPECT_EQ(r.first, 0x80);
    EXPECT_EQ(r.second, 1);
    buffer.put(0x81);
    r = celduin::ids::read(buffer);
    EXPECT_EQ(r.first, 0x81);
    EXPECT_EQ(r.second, 1);
    buffer.put(0x97);
    r = celduin::ids::read(buffer);
    EXPECT_EQ(r.first, 0x97);
    EXPECT_EQ(r.second, 1);
    buffer.put(0xC0);
    r = celduin::ids::read(buffer);
    EXPECT_EQ(r.first, 0xC0);
    EXPECT_EQ(r.second, 1);
    buffer.put(0xFE);
    r = celduin::ids::read(buffer);
    EXPECT_EQ(r.first, 0xFE);
    EXPECT_EQ(r.second, 1);
    buffer.put(0xFF);
    EXPECT_THROW(celduin::ids::read(buffer), celduin::InvalidEBMLID);
    // 01xxxxxx xxxxxxxx
    buffer.put(0x40); buffer.put(0x00);;
    r = celduin::ids::read(buffer);
    EXPECT_EQ(r.first, 0x4000);
    EXPECT_EQ(r.second, 2);
    buffer.put(0x40); buffer.put(0x01);;
    r = celduin::ids::read(buffer);
    EXPECT_EQ(r.first, 0x4001);
    EXPECT_EQ(r.second, 2);
    buffer.put(0x4B); buffer.put(0x35);;
    r = celduin::ids::read(buffer);
    EXPECT_EQ(r.first, 0x4B35);
    EXPECT_EQ(r.second, 2);
    buffer.put(0x7F); buffer.put(0xFE);;
    r = celduin::ids::read(buffer);
    EXPECT_EQ(r.first, 0x7FFE);
    EXPECT_EQ(r.second, 2);
    buffer.put(0x7F); buffer.put(0xFF);;
    EXPECT_THROW(celduin::ids::read(buffer), celduin::InvalidEBMLID);
    /* Uncomment this if EBML IDs expand to 64 bits.
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x01);
    for (int ii(0); ii < 7; ii++)
    {
        buffer.put(0x00);
    }
    r = celduin::ids::read(buffer);
    EXPECT_EQ(r.first, 0x0100000000000000);
    EXPECT_EQ(r.second, 8);
    buffer.put(0x01);
    for (int ii(0); ii < 6; ii++)
    {
        buffer.put(0x00);
    }
    buffer.put(0x01);
    r = celduin::ids::read(buffer);
    EXPECT_EQ(r.first, 0x0100000000000001);
    EXPECT_EQ(r.second, 8);
    buffer.put(0x01);
    for (int ii(0); ii < 6; ii++)
    {
        buffer.put(0xFF);
    }
    buffer.put(0xFE);
    r = celduin::ids::read(buffer);
    EXPECT_EQ(r.first, 0x01FFFFFFFFFFFFFE);
    EXPECT_EQ(r.second, 8);
    buffer.put(0x01);
    for (int ii(0); ii < 7; ii++)
    {
        buffer.put(0xFF);
    }
    EXPECT_THROW(celduin::ids::read(buffer), celduin::InvalidEBMLID);
    */
    // EBML tag
    buffer.put(0x1A); buffer.put(0x45); buffer.put(0xDF); buffer.put(0xA3);
    r = celduin::ids::read(buffer);
    EXPECT_EQ(r.first, 0x1A45DFA3);
    EXPECT_EQ(r.second, 4);
}


TEST(ElIDStream, WriteRead)
{
    std::stringstream buffer;
    celduin::ids::ReadResult r;
    // 1xxxxxxx
    EXPECT_EQ(1, celduin::ids::write(0x80, buffer));
    r = celduin::ids::read(buffer);
    EXPECT_EQ(r.first, 0x80);
    EXPECT_EQ(r.second, 1);
    EXPECT_EQ(1, celduin::ids::write(0x81, buffer));
    r = celduin::ids::read(buffer);
    EXPECT_EQ(r.first, 0x81);
    EXPECT_EQ(r.second, 1);
    EXPECT_EQ(1, celduin::ids::write(0xFE, buffer));
    r = celduin::ids::read(buffer);
    EXPECT_EQ(r.first, 0xFE);
    EXPECT_EQ(r.second, 1);
    // 01xxxxxx xxxxxxxx
    EXPECT_EQ(2, celduin::ids::write(0x4000, buffer));
    r = celduin::ids::read(buffer);
    EXPECT_EQ(r.first, 0x4000);
    EXPECT_EQ(r.second, 2);
    EXPECT_EQ(2, celduin::ids::write(0x7FFE, buffer));
    r = celduin::ids::read(buffer);
    EXPECT_EQ(r.first, 0x7FFE);
    EXPECT_EQ(r.second, 2);
    // 001xxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(3, celduin::ids::write(0x200000, buffer));
    r = celduin::ids::read(buffer);
    EXPECT_EQ(r.first, 0x200000);
    EXPECT_EQ(r.second, 3);
    EXPECT_EQ(3, celduin::ids::write(0x3FFFFE, buffer));
    r = celduin::ids::read(buffer);
    EXPECT_EQ(r.first, 0x3FFFFE);
    EXPECT_EQ(r.second, 3);
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(4, celduin::ids::write(0x10000000, buffer));
    r = celduin::ids::read(buffer);
    EXPECT_EQ(r.first, 0x10000000);
    EXPECT_EQ(r.second, 4);
    EXPECT_EQ(4, celduin::ids::write(0x1FFFFFFE, buffer));
    r = celduin::ids::read(buffer);
    EXPECT_EQ(r.first, 0x1FFFFFFE);
    EXPECT_EQ(r.second, 4);
    /* Uncomment this if EBML IDs expand to 64 bits.
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(5, celduin::ids::write(0x0800000000, buffer));
    r = celduin::ids::read(buffer);
    EXPECT_EQ(r.first, 0x0800000000);
    EXPECT_EQ(r.second, 5);
    EXPECT_EQ(5, celduin::ids::write(0x0FFFFFFFFE, buffer));
    r = celduin::ids::read(buffer);
    EXPECT_EQ(r.first, 0x0FFFFFFFFE);
    EXPECT_EQ(r.second, 5);
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(6, celduin::ids::write(0x040000000000, buffer));
    r = celduin::ids::read(buffer);
    EXPECT_EQ(r.first, 0x040000000000);
    EXPECT_EQ(r.second, 6);
    EXPECT_EQ(6, celduin::ids::write(0X07FFFFFFFFFE, buffer));
    r = celduin::ids::read(buffer);
    EXPECT_EQ(r.first, 0x07FFFFFFFFFE);
    EXPECT_EQ(r.second, 6);
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(7, celduin::ids::write(0x02000000000000, buffer));
    r = celduin::ids::read(buffer);
    EXPECT_EQ(r.first, 0x02000000000000);
    EXPECT_EQ(r.second, 7);
    EXPECT_EQ(7, celduin::ids::write(0X03FFFFFFFFFFFE, buffer));
    r = celduin::ids::read(buffer);
    EXPECT_EQ(r.first, 0X03FFFFFFFFFFFE);
    EXPECT_EQ(r.second, 7);
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    EXPECT_EQ(8, celduin::ids::write(0x0100000000000000, buffer));
    r = celduin::ids::read(buffer);
    EXPECT_EQ(r.first, 0x0100000000000000);
    EXPECT_EQ(r.second, 8);
    EXPECT_EQ(8, celduin::ids::write(0X01FFFFFFFFFFFFFE, buffer));
    r = celduin::ids::read(buffer);
    EXPECT_EQ(r.first, 0x01FFFFFFFFFFFFFE);
    EXPECT_EQ(r.second, 8);
    */
}


TEST(ElIDStream, NoTail)
{
    std::stringstream buffer;
    // 1xxxxxxx - No tail necessary
    buffer.put(0x80);
    EXPECT_NO_THROW(celduin::ids::read(buffer));
    // 01xxxxxx xxxxxxxx
    buffer.put(0x40);
    EXPECT_THROW(celduin::ids::read(buffer), celduin::ReadError);
    // 001xxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x20);
    EXPECT_THROW(celduin::ids::read(buffer), celduin::ReadError);
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x10);
    EXPECT_THROW(celduin::ids::read(buffer), celduin::ReadError);
    /* Uncomment this if EBML IDs expand to 64 bits.
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x08);
    EXPECT_THROW(celduin::ids::read(buffer), celduin::ReadError);
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x04);
    EXPECT_THROW(celduin::ids::read(buffer), celduin::ReadError);
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x02);
    EXPECT_THROW(celduin::ids::read(buffer), celduin::ReadError);
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x01);
    EXPECT_THROW(celduin::ids::read(buffer), celduin::ReadError);
    */
}


TEST(ElIDStream, TailTooShort)
{
    std::stringstream buffer;
    // 1xxxxxxx - No tail necessary
    buffer.put(0x80);
    EXPECT_NO_THROW(celduin::ids::read(buffer));
    // 01xxxxxx xxxxxxxx
    buffer.put(0x40);
    EXPECT_THROW(celduin::ids::read(buffer), celduin::ReadError);
    // 001xxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x20); buffer.put(0x00);
    EXPECT_THROW(celduin::ids::read(buffer), celduin::ReadError);
    // 0001xxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x10); buffer.put(0x00); buffer.put(0x00);
    EXPECT_THROW(celduin::ids::read(buffer), celduin::ReadError);
    /* Uncomment this if EBML IDs expand to 64 bits.
    // 00001xxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x08); buffer.put(0x00); buffer.put(0x00); buffer.put(0x00);
    EXPECT_THROW(celduin::ids::read(buffer), celduin::ReadError);
    // 000001xx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x04); buffer.put(0x00); buffer.put(0x00); buffer.put(0x00);
    buffer.put(0x00);
    EXPECT_THROW(celduin::ids::read(buffer), celduin::ReadError);
    // 0000001x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x02); buffer.put(0x00); buffer.put(0x00); buffer.put(0x00);
    buffer.put(0x00); buffer.put(0x00);
    EXPECT_THROW(celduin::ids::read(buffer), celduin::ReadError);
    // 00000001 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    buffer.put(0x01); buffer.put(0x00); buffer.put(0x00); buffer.put(0x00);
    buffer.put(0x00); buffer.put(0x00); buffer.put(0x00);
    EXPECT_THROW(celduin::ids::read(buffer), celduin::ReadError);
    */
}


TEST(ElIDStream, NoMarker)
{
    std::stringstream buffer;
    // 1xxxxxxx - No tail necessary
    buffer.put(0x80);
    EXPECT_NO_THROW(celduin::ids::read(buffer));
    // 00000000 xxxxxxxx ...
    buffer.put(0x00);
    EXPECT_THROW(celduin::ids::read(buffer), celduin::InvalidVarInt);
}

