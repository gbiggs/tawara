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
#include <tawara/block_impl.h>
#include <tawara/el_ids.h>
#include <tawara/exceptions.h>
#include <tawara/vint.h>

#include "test_utils.h"


TEST(BlockImpl, Create)
{
    tawara::BlockImpl b1(1, 12345);
    EXPECT_EQ(1, b1.track_number());
    EXPECT_EQ(12345, b1.timecode());
    EXPECT_FALSE(b1.invisible());
    EXPECT_EQ(tawara::Block::LACING_NONE, b1.lacing());
    EXPECT_TRUE(b1.empty());

    tawara::BlockImpl b2(2, 22222, tawara::Block::LACING_EBML);
    EXPECT_EQ(2, b2.track_number());
    EXPECT_EQ(22222, b2.timecode());
    EXPECT_FALSE(b2.invisible());
    EXPECT_EQ(tawara::Block::LACING_EBML, b2.lacing());
    EXPECT_TRUE(b2.empty());

}


TEST(BlockImpl, TrackNumber)
{
    tawara::BlockImpl b1(1, 12345);
    EXPECT_EQ(1, b1.track_number());
    b1.track_number(42);
    EXPECT_EQ(42, b1.track_number());
}


TEST(BlockImpl, Timecode)
{
    tawara::BlockImpl b1(1, 12345);
    EXPECT_EQ(12345, b1.timecode());
    b1.timecode(22222);
    EXPECT_EQ(22222, b1.timecode());
}


TEST(BlockImpl, Invisible)
{
    tawara::BlockImpl b1(2, 22222, tawara::Block::LACING_EBML);
    EXPECT_FALSE(b1.invisible());
    b1.invisible(true);
    EXPECT_TRUE(b1.invisible());
}


TEST(BlockImpl, Lacing)
{
    tawara::BlockImpl b1(1, 12345);
    EXPECT_EQ(tawara::Block::LACING_NONE, b1.lacing());
    b1.lacing(tawara::Block::LACING_EBML);
    EXPECT_EQ(tawara::Block::LACING_EBML, b1.lacing());
    b1.lacing(tawara::Block::LACING_FIXED);
    EXPECT_EQ(tawara::Block::LACING_FIXED, b1.lacing());
}


TEST(BlockImpl, Assignment)
{
    tawara::BlockImpl b1(1, 12345, tawara::Block::LACING_EBML);
    tawara::BlockImpl b2(2, 22222, tawara::Block::LACING_FIXED);
    tawara::Block::value_type f1(test_utils::make_blob(5));
    tawara::Block::value_type f2(test_utils::make_blob(10));
    b1.push_back(f1);
    b1.push_back(f2);

    EXPECT_TRUE(b2.empty());
    b2 = b1;
    EXPECT_EQ(b1.track_number(), b2.track_number());
    EXPECT_EQ(b1.timecode(), b2.timecode());
    EXPECT_EQ(b1.invisible(), b2.invisible());
    EXPECT_EQ(b1.lacing(), b2.lacing());
    EXPECT_FALSE(b2.empty());
    EXPECT_EQ(b2[1]->size(), f2->size());
}


TEST(BlockImpl, At)
{
    tawara::BlockImpl b(2, 22222, tawara::Block::LACING_EBML);
    tawara::Block::value_type f1(test_utils::make_blob(5));
    tawara::Block::value_type f2(test_utils::make_blob(10));
    b.push_back(f1);
    b.push_back(f2);
    EXPECT_EQ(b[1], b.at(1));
    EXPECT_EQ(b.at(1)->size(), f2->size());
    EXPECT_THROW(b.at(2), std::out_of_range);
}


TEST(BlockImpl, SubscriptOperator)
{
    tawara::BlockImpl b(2, 22222, tawara::Block::LACING_EBML);
    tawara::Block::value_type f1(test_utils::make_blob(5));
    tawara::Block::value_type f2(test_utils::make_blob(10));
    b.push_back(f1);
    b.push_back(f2);
    EXPECT_EQ(b[1], b.at(1));
    EXPECT_EQ(b.at(1)->size(), f2->size());
    EXPECT_NO_THROW(b[2]);

    tawara::Block::value_type f3(test_utils::make_blob(15));
    b[1] = f3;
    EXPECT_EQ(b[1], f3);
    EXPECT_EQ(b[1]->size(), f3->size());
}


TEST(BlockImpl, BeginEnd)
{
    tawara::BlockImpl b(2, 22222, tawara::Block::LACING_EBML);
    tawara::Block::value_type f1(test_utils::make_blob(5));

    EXPECT_TRUE(b.begin() == b.end());
    EXPECT_TRUE(b.rbegin() == b.rend());
    b.push_back(f1);
    EXPECT_FALSE(b.begin() == b.end());
    EXPECT_FALSE(b.rbegin() == b.rend());
}


TEST(BlockImpl, Counts)
{
    tawara::BlockImpl b(2, 22222, tawara::Block::LACING_EBML);
    tawara::Block::value_type f1(test_utils::make_blob(5));
    tawara::Block::value_type f2(test_utils::make_blob(10));

    EXPECT_TRUE(b.empty());
    b.push_back(f1);
    b.push_back(f2);
    EXPECT_FALSE(b.empty());
    EXPECT_EQ(2, b.count());

    b.lacing(tawara::Block::LACING_NONE);
    EXPECT_EQ(1, b.max_count());
    b.lacing(tawara::Block::LACING_EBML);
    EXPECT_NE(1, b.max_count());
    b.lacing(tawara::Block::LACING_FIXED);
    EXPECT_NE(1, b.max_count());
}


TEST(BlockImpl, Clear)
{
    tawara::BlockImpl b(2, 22222, tawara::Block::LACING_EBML);
    tawara::Block::value_type f1(test_utils::make_blob(5));
    b.push_back(f1);
    EXPECT_FALSE(b.empty());
    b.clear();
    EXPECT_TRUE(b.empty());
}


TEST(BlockImpl, PushBack)
{
    tawara::BlockImpl b(1, 12345, tawara::Block::LACING_NONE);
    tawara::Block::value_type f1(test_utils::make_blob(5));
    tawara::Block::value_type f2(test_utils::make_blob(10));
    tawara::Block::value_type f3(test_utils::make_blob(15));
    tawara::Block::value_type empty_frame;

    EXPECT_THROW(b.push_back(empty_frame), tawara::EmptyFrame);

    EXPECT_TRUE(b.empty());
    EXPECT_NO_THROW(b.push_back(f1));
    EXPECT_FALSE(b.empty());
    EXPECT_EQ(1, b.count());
    EXPECT_THROW(b.push_back(f2), tawara::MaxLaceSizeExceeded);

    b.lacing(tawara::Block::LACING_EBML);
    EXPECT_NO_THROW(b.push_back(f2));
    EXPECT_EQ(2, b.count());

    b.lacing(tawara::Block::LACING_FIXED);
    b.clear();
    b.push_back(f1);
    EXPECT_NO_THROW(b.push_back(f1));
    EXPECT_THROW(b.push_back(f2), tawara::BadLacedFrameSize);
    EXPECT_EQ(2, b.count());

    b.lacing(tawara::Block::LACING_NONE);
    EXPECT_THROW(b.push_back(f1), tawara::MaxLaceSizeExceeded);
}


TEST(BlockImpl, Erase)
{
    tawara::BlockImpl b(1, 12345, tawara::Block::LACING_NONE);
    tawara::Block::value_type f1(test_utils::make_blob(5));
    b.push_back(f1);

    EXPECT_FALSE(b.empty());
    b.erase(b.begin());
    EXPECT_TRUE(b.empty());

    b.push_back(f1);
    EXPECT_FALSE(b.empty());
    b.erase(b.begin(), b.end());
    EXPECT_TRUE(b.empty());
}


TEST(BlockImpl, Resize)
{
    tawara::BlockImpl b(1, 12345, tawara::Block::LACING_NONE);
    tawara::Block::value_type f1(test_utils::make_blob(5));
    tawara::Block::value_type f2(test_utils::make_blob(10));
    tawara::Block::value_type f3(test_utils::make_blob(15));

    EXPECT_THROW(b.resize(2), tawara::MaxLaceSizeExceeded);
    b.lacing(tawara::Block::LACING_EBML);
    EXPECT_NO_THROW(b.resize(2));
    EXPECT_EQ(2, b.count());
    EXPECT_TRUE(!b[1]);
    b.lacing(tawara::Block::LACING_FIXED);
    EXPECT_NO_THROW(b.resize(3));
    EXPECT_EQ(3, b.count());
    b.lacing(tawara::Block::LACING_NONE);
    EXPECT_NO_THROW(b.resize(1));
    EXPECT_EQ(1, b.count());
}


TEST(BlockImpl, Swap)
{
    tawara::BlockImpl b1(1, 12345, tawara::Block::LACING_NONE);
    tawara::BlockImpl b2(2, 22222, tawara::Block::LACING_EBML);
    tawara::Block::value_type f1(test_utils::make_blob(5));
    tawara::Block::value_type f2(test_utils::make_blob(10));
    tawara::Block::value_type f3(test_utils::make_blob(15));
    b1.invisible(true);
    b1.push_back(f1);
    b2.push_back(f2);
    b2.push_back(f3);

    b2.swap(b1);
    EXPECT_EQ(2, b1.track_number());
    EXPECT_EQ(1, b2.track_number());
    EXPECT_EQ(22222, b1.timecode());
    EXPECT_EQ(12345, b2.timecode());
    EXPECT_FALSE(b1.invisible());
    EXPECT_TRUE(b2.invisible());
    EXPECT_EQ(tawara::Block::LACING_EBML, b1.lacing());
    EXPECT_EQ(tawara::Block::LACING_NONE, b2.lacing());
    EXPECT_EQ(2, b1.count());
    EXPECT_EQ(1, b2.count());
    EXPECT_TRUE(f2 == b1[0]);
    EXPECT_TRUE(f1 == b2[0]);
}


TEST(BlockImpl, Equality)
{
    tawara::BlockImpl b1(1, 12345, tawara::Block::LACING_NONE);
    tawara::BlockImpl b2(1, 12345, tawara::Block::LACING_NONE);
    EXPECT_TRUE(b1 == b2);
    EXPECT_FALSE(b1 != b2);

    b1.track_number(2);
    EXPECT_FALSE(b1 == b2);
    EXPECT_TRUE(b1 != b2);
    b1.track_number(1);

    b1.timecode(22222);
    EXPECT_FALSE(b1 == b2);
    EXPECT_TRUE(b1 != b2);
    b1.timecode(12345);

    b1.invisible(true);
    EXPECT_FALSE(b1 == b2);
    EXPECT_TRUE(b1 != b2);
    b1.track_number(false);

    b1.lacing(tawara::Block::LACING_EBML);
    EXPECT_FALSE(b1 == b2);
    EXPECT_TRUE(b1 != b2);
    b1.lacing(tawara::Block::LACING_NONE);

    tawara::Block::value_type f1(test_utils::make_blob(5));
    b1.push_back(f1);
    EXPECT_FALSE(b1 == b2);
    EXPECT_TRUE(b1 != b2);
}


TEST(BlockImpl, Size)
{
    tawara::BlockImpl b(1, 12345, tawara::Block::LACING_NONE);
    tawara::Block::value_type f1(test_utils::make_blob(5));
    tawara::Block::value_type f2(test_utils::make_blob(10));
    tawara::Block::value_type f3(test_utils::make_blob(15));
    std::streamsize frames_size = f1->size() + f2->size() + f3->size();

    b.push_back(f1);
    // The 3 bytes are for the timecode and flags
    EXPECT_EQ(tawara::vint::size(1) + 3 + f1->size(), b.size());

    b.lacing(tawara::Block::LACING_EBML);
    b.push_back(f2);
    b.push_back(f3);
    // Extra 1 byte for number of frames in the lace
    EXPECT_EQ(tawara::vint::size(1) + 3 + 1 + tawara::vint::size(f1->size()) +
            tawara::vint::s_to_u(f2->size() - f1->size()).second +
            frames_size, b.size());

    b.lacing(tawara::Block::LACING_FIXED);
    b.clear();
    b.push_back(f1);
    b.push_back(f1);
    b.push_back(f1);
    // Extra 1 byte for number of frames in the lace
    EXPECT_EQ(tawara::vint::size(1) + 3 + 1 + 3 * f1->size(), b.size());
}


TEST(BlockImpl, Write)
{
    std::ostringstream output;
    std::stringstream expected;
    std::streamsize expected_size(0);

    unsigned int track_num(1);
    unsigned int timecode(12345);

    tawara::BlockImpl b(track_num, timecode);
    tawara::Block::value_type f1(test_utils::make_blob(5));
    tawara::Block::value_type f2(test_utils::make_blob(10));
    tawara::Block::value_type f3(test_utils::make_blob(15));
    std::streamsize frame_size = f1->size() + f2->size() + f3->size();
    tawara::vint::OffsetInt f2_lace_size(tawara::vint::s_to_u(f2->size() -
                f1->size()));

    // No lacing
    b.lacing(tawara::Block::LACING_NONE);
    b.push_back(f1);
    tawara::vint::write(track_num, expected);
    expected.put(timecode >> 8);
    expected.put(timecode & 0xFF);
    expected.put(0); // Flags
    expected.write(&(*f1)[0], f1->size());
    expected_size = tawara::vint::size(track_num) + 3 + f1->size();
    EXPECT_EQ(expected_size, b.write(output, 0));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());
    EXPECT_EQ(0, output.str()[tawara::vint::size(track_num) + 2]);

    // EBML lacing
    output.str(std::string());
    expected.str(std::string());
    b.lacing(tawara::Block::LACING_EBML);
    b.push_back(f2);
    b.push_back(f3);
    tawara::vint::write(track_num, expected);
    expected.put(timecode >> 8);
    expected.put(timecode & 0xFF);
    expected.put(0x60); // Flags
    expected.put(3); // Lace header - number of frames
    tawara::vint::write(f1->size(), expected);
    tawara::vint::write(f2_lace_size.first, expected, f2_lace_size.second);
    expected.write(&(*f1)[0], f1->size());
    expected.write(&(*f2)[0], f2->size());
    expected.write(&(*f3)[0], f3->size());
    expected_size = tawara::vint::size(track_num) + 3 + 1 +
        tawara::vint::size(f1->size()) +
        tawara::vint::s_to_u(f2->size() - f1->size()).second + frame_size;
    EXPECT_EQ(expected_size, b.write(output, 0));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());
    EXPECT_EQ(0x60, output.str()[tawara::vint::size(track_num) + 2]);

    // Fixed lacing
    output.str(std::string());
    expected.str(std::string());
    b.lacing(tawara::Block::LACING_FIXED);
    b.clear();
    b.push_back(f1);
    b.push_back(f1);
    b.push_back(f1);
    tawara::vint::write(track_num, expected);
    expected.put(timecode >> 8);
    expected.put(timecode & 0xFF);
    expected.put(0x40); // Flags
    expected.put(3); // Lace header - number of frames
    expected.write(&(*f1)[0], f1->size());
    expected.write(&(*f1)[0], f1->size());
    expected.write(&(*f1)[0], f1->size());
    expected_size = tawara::vint::size(track_num) + 3 + 1 + 3 * f1->size();
    EXPECT_EQ(expected_size, b.write(output, 0));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());
    EXPECT_EQ(0x40, output.str()[tawara::vint::size(track_num) + 2]);

    // Invisible flag set
    output.str(std::string());
    expected.str(std::string());
    b.invisible(true);
    b.lacing(tawara::Block::LACING_NONE);
    b.clear();
    b.push_back(f1);
    tawara::vint::write(track_num, expected);
    expected.put(timecode >> 8);
    expected.put(timecode & 0xFF);
    expected.put(0x10); // Flags
    expected.write(&(*f1)[0], f1->size());
    expected_size = tawara::vint::size(track_num) + 3 + f1->size();
    EXPECT_EQ(expected_size, b.write(output, 0));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());
    EXPECT_EQ(0x10, output.str()[tawara::vint::size(track_num) + 2]);

    // Empty block
    b.clear();
    EXPECT_THROW(b.write(output, 0), tawara::EmptyBlock);

    // Empty frame
    tawara::Block::value_type empty_frame;
    b.clear();
    b.push_back(f1);
    b[0] = empty_frame;
    EXPECT_THROW(b.write(output, 0), tawara::EmptyFrame);

    // Unequal frame sizes
    b.clear();
    b.lacing(tawara::Block::LACING_EBML);
    b.push_back(f1);
    b.push_back(f2);
    b.lacing(tawara::Block::LACING_FIXED);
    EXPECT_THROW(b.write(output, 0), tawara::BadLacedFrameSize);
}


TEST(BlockImpl, Read)
{
    std::stringstream input;
    std::streamsize expected_size(0);

    unsigned int track_num(1);
    unsigned int timecode(12345);

    tawara::BlockImpl b(0, 0);
    b.lacing(tawara::Block::LACING_FIXED);
    b.invisible(true);

    tawara::Block::value_type f1(test_utils::make_blob(5));
    tawara::Block::value_type f2(test_utils::make_blob(8));
    tawara::Block::value_type f3(test_utils::make_blob(6));
    std::streamsize frame_size = f1->size() + f2->size() + f3->size();
    tawara::vint::OffsetInt f2_lace_size(tawara::vint::s_to_u(f2->size() -
                f1->size()));

    // No lacing
    tawara::vint::write(track_num, input);
    input.put(timecode >> 8);
    input.put(timecode & 0xFF);
    input.put(0); // Flags
    input.write(&(*f1)[0], f1->size());
    expected_size = tawara::vint::size(track_num) + 3 + f1->size();
    tawara::BlockImpl::ReadResult res = b.read(input, expected_size);
    EXPECT_EQ(expected_size, res.first);
    EXPECT_EQ(0, res.second);
    EXPECT_EQ(track_num, b.track_number());
    EXPECT_EQ(timecode, b.timecode());
    EXPECT_FALSE(b.invisible());
    EXPECT_EQ(tawara::Block::LACING_NONE, b.lacing());
    EXPECT_EQ(1, b.count());
    EXPECT_EQ(f1->size(), b[0]->size());

    // EBML lacing
    input.str(std::string());
    b.clear();
    b.track_number(0);
    b.timecode(0);
    b.lacing(tawara::Block::LACING_FIXED);
    b.invisible(true);
    tawara::vint::write(track_num, input);
    input.put(timecode >> 8);
    input.put(timecode & 0xFF);
    input.put(0x60); // Flags
    input.put(3); // Lace header - number of frames
    tawara::vint::write(f1->size(), input);
    tawara::vint::write(f2_lace_size.first, input, f2_lace_size.second);
    input.write(&(*f1)[0], f1->size());
    input.write(&(*f2)[0], f2->size());
    input.write(&(*f3)[0], f3->size());
    expected_size = tawara::vint::size(track_num) + 3 + 1 +
        tawara::vint::size(f1->size()) +
        tawara::vint::s_to_u(f2->size() - f1->size()).second + frame_size;
    res = b.read(input, expected_size);
    EXPECT_EQ(expected_size, res.first);
    EXPECT_EQ(0, res.second);
    EXPECT_EQ(track_num, b.track_number());
    EXPECT_EQ(timecode, b.timecode());
    EXPECT_FALSE(b.invisible());
    EXPECT_EQ(tawara::Block::LACING_EBML, b.lacing());
    EXPECT_EQ(3, b.count());
    EXPECT_EQ(f1->size(), b[0]->size());
    EXPECT_EQ(f2->size(), b[1]->size());
    EXPECT_EQ(f3->size(), b[2]->size());

    // Fixed lacing
    input.str(std::string());
    b.clear();
    b.track_number(0);
    b.timecode(0);
    b.lacing(tawara::Block::LACING_NONE);
    b.invisible(true);
    tawara::vint::write(track_num, input);
    input.put(timecode >> 8);
    input.put(timecode & 0xFF);
    input.put(0x40); // Flags
    input.put(3); // Lace header - number of frames
    input.write(&(*f1)[0], f1->size());
    input.write(&(*f1)[0], f1->size());
    input.write(&(*f1)[0], f1->size());
    expected_size = tawara::vint::size(track_num) + 3 + 1 + 3 * f1->size();
    res = b.read(input, expected_size);
    EXPECT_EQ(expected_size, res.first);
    EXPECT_EQ(0, res.second);
    EXPECT_EQ(track_num, b.track_number());
    EXPECT_EQ(timecode, b.timecode());
    EXPECT_FALSE(b.invisible());
    EXPECT_EQ(tawara::Block::LACING_FIXED, b.lacing());
    EXPECT_EQ(3, b.count());
    EXPECT_EQ(f1->size(), b[0]->size());
    EXPECT_EQ(f1->size(), b[1]->size());
    EXPECT_EQ(f1->size(), b[2]->size());

    // Invisible flag set
    input.str(std::string());
    b.clear();
    b.track_number(0);
    b.timecode(0);
    b.lacing(tawara::Block::LACING_NONE);
    b.invisible(false);
    tawara::vint::write(track_num, input);
    input.put(timecode >> 8);
    input.put(timecode & 0xFF);
    input.put(0x10); // Flags
    input.write(&(*f1)[0], f1->size());
    expected_size = tawara::vint::size(track_num) + 3 + f1->size();
    res = b.read(input, expected_size);
    EXPECT_EQ(expected_size, res.first);
    EXPECT_EQ(0x00, res.second);
    EXPECT_TRUE(b.invisible());

    // Read error
    input.str(std::string());
    b.lacing(tawara::Block::LACING_EBML);
    b.push_back(f2);
    b.push_back(f3);
    tawara::vint::write(track_num, input);
    input.put(timecode >> 8);
    input.put(timecode & 0xFF);
    input.put(0x60); // Flags
    input.put(3); // Lace header - number of frames
    tawara::vint::write(f1->size(), input);
    tawara::vint::write(f2_lace_size.first, input, f2_lace_size.second);
    input.write(&(*f1)[0], f1->size());
    input.write(&(*f2)[0], f2->size());
    expected_size = tawara::vint::size(track_num) + 3 + 1 +
        tawara::vint::size(f1->size()) +
        tawara::vint::s_to_u(f2->size() - f1->size()).second + frame_size;
    EXPECT_THROW(b.read(input, expected_size), tawara::ReadError);
    input.clear();

    // Bad body size
    input.str(std::string());
    tawara::vint::write(track_num, input);
    input.put(timecode >> 8);
    input.put(timecode & 0xFF);
    input.put(0x00); // Flags
    expected_size = tawara::vint::size(track_num) + 3;
    EXPECT_THROW(b.read(input, expected_size), tawara::BadBodySize);

    // Bad frame size (due to missing data)
    input.str(std::string());
    tawara::vint::write(track_num, input);
    input.put(timecode >> 8);
    input.put(timecode & 0xFF);
    input.put(0x60); // Flags - EBML lacing
    input.put(3); // Lace header - number of frames
    tawara::vint::write(f1->size(), input);
    tawara::vint::write(f2_lace_size.first, input, f2_lace_size.second);
    expected_size = tawara::vint::size(track_num) + 3 + 1 +
        tawara::vint::size(f1->size()) +
        tawara::vint::s_to_u(f2->size() - f1->size()).second;
    EXPECT_THROW(b.read(input, expected_size), tawara::BadLacedFrameSize);

    // Missing frame - EBML lacing
    input.str(std::string());
    tawara::vint::write(track_num, input);
    input.put(timecode >> 8);
    input.put(timecode & 0xFF);
    input.put(0x60); // Flags - EBML lacing
    input.put(3); // Lace header - number of frames
    tawara::vint::write(f1->size(), input);
    tawara::vint::write(f2_lace_size.first, input, f2_lace_size.second);
    input.write(&(*f1)[0], f1->size());
    input.write(&(*f2)[0], f2->size());
    // No 3rd frame
    expected_size = tawara::vint::size(track_num) + 3 + 1 +
        tawara::vint::size(f1->size()) +
        tawara::vint::s_to_u(f2->size() - f1->size()).second + f1->size() +
        f2->size();
    EXPECT_THROW(b.read(input, expected_size), tawara::EmptyFrame);

    // Missing frame - fixed lacing
    input.str(std::string());
    tawara::vint::write(track_num, input);
    input.put(timecode >> 8);
    input.put(timecode & 0xFF);
    input.put(0x40); // Flags - fixed lacing
    input.put(3); // Lace header - number of frames
    input.write(&(*f1)[0], f1->size());
    input.write(&(*f1)[0], f1->size());
    // No 3rd frame
    expected_size = tawara::vint::size(track_num) + 3 + 1 +
        f1->size() + f1->size();
    EXPECT_THROW(b.read(input, expected_size), tawara::BadLacedFrameSize);

    // Unequal frame sizes
    input.str(std::string());
    tawara::vint::write(track_num, input);
    input.put(timecode >> 8);
    input.put(timecode & 0xFF);
    input.put(0x40); // Flags
    input.put(2); // Lace header - number of frames
    input.write(&(*f1)[0], f1->size());
    input.write(&(*f2)[0], f1->size());
    expected_size = tawara::vint::size(track_num) + 3 + 1 + f1->size() +
        f2->size();
    EXPECT_THROW(b.read(input, expected_size), tawara::BadLacedFrameSize);
}

