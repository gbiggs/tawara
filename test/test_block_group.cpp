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
#include <tide/block_additions.h>
#include <tide/block_group.h>
#include <tide/el_ids.h>
#include <tide/exceptions.h>
#include <tide/int_element.h>
#include <tide/uint_element.h>
#include <tide/vint.h>

#include "test_utils.h"


TEST(BlockGroup, Create)
{
    tide::BlockGroup b1(1, 12345);
    EXPECT_EQ(1, b1.track_number());
    EXPECT_EQ(12345, b1.timecode());
    EXPECT_FALSE(b1.invisible());
    EXPECT_EQ(tide::Block::LACING_NONE, b1.lacing());
    EXPECT_TRUE(b1.empty());

    tide::BlockGroup b2(2, 22222, tide::Block::LACING_EBML, 42, 84);
    EXPECT_EQ(2, b2.track_number());
    EXPECT_EQ(22222, b2.timecode());
    EXPECT_FALSE(b2.invisible());
    EXPECT_EQ(tide::Block::LACING_EBML, b2.lacing());
    EXPECT_TRUE(b2.empty());
    EXPECT_EQ(42, b2.duration());
    EXPECT_EQ(84, b2.ref_priority());
}


TEST(BlockGroup, TrackNumber)
{
    tide::BlockGroup b1(1, 12345);
    EXPECT_EQ(1, b1.track_number());
    b1.track_number(42);
    EXPECT_EQ(42, b1.track_number());
}


TEST(BlockGroup, Timecode)
{
    tide::BlockGroup b1(1, 12345);
    EXPECT_EQ(12345, b1.timecode());
    b1.timecode(22222);
    EXPECT_EQ(22222, b1.timecode());
}


TEST(BlockGroup, Invisible)
{
    tide::BlockGroup b1(2, 22222, tide::Block::LACING_EBML);
    EXPECT_FALSE(b1.invisible());
    b1.invisible(true);
    EXPECT_TRUE(b1.invisible());
}


TEST(BlockGroup, Lacing)
{
    tide::BlockGroup b1(1, 12345);
    EXPECT_EQ(tide::Block::LACING_NONE, b1.lacing());
    b1.lacing(tide::Block::LACING_EBML);
    EXPECT_EQ(tide::Block::LACING_EBML, b1.lacing());
    b1.lacing(tide::Block::LACING_FIXED);
    EXPECT_EQ(tide::Block::LACING_FIXED, b1.lacing());
}


TEST(BlockGroup, Additions)
{
    tide::BlockGroup b1(1, 12345);

    EXPECT_TRUE(b1.additions().empty());
    tide::BlockAdditions::value_type addition(new
            tide::BlockAdditions::Addition(2,
                *test_utils::make_blob(5)));
    b1.additions().push_back(addition);
    EXPECT_EQ(b1.additions()[0], addition);
}


TEST(BlockGroup, Duration)
{
    tide::BlockGroup b1(1, 12345);
    EXPECT_EQ(0, b1.duration());
    b1.duration(42);
    EXPECT_EQ(42, b1.duration());
}


TEST(BlockGroup, RefPriority)
{
    tide::BlockGroup b1(1, 12345);
    EXPECT_EQ(0, b1.ref_priority());
    b1.ref_priority(42);
    EXPECT_EQ(42, b1.ref_priority());
}


TEST(BlockGroup, RefBlocks)
{
    tide::BlockGroup b1(1, 12345);
    EXPECT_TRUE(b1.ref_blocks().empty());
    b1.ref_blocks().push_back(42);
    EXPECT_FALSE(b1.ref_blocks().empty());
    EXPECT_EQ(42, b1.ref_blocks()[0]);
}


TEST(BlockGroup, CodecState)
{
    tide::BlockGroup b1(1, 12345);
    EXPECT_TRUE(b1.codec_state().empty());
    std::vector<char> blob(*test_utils::make_blob(10));
    b1.codec_state(blob);
    EXPECT_TRUE(b1.codec_state() == blob);
}


TEST(BlockGroup, Assignment)
{
    tide::BlockGroup b1(1, 12345, tide::Block::LACING_EBML);
    tide::BlockGroup b2(2, 22222, tide::Block::LACING_FIXED);
    tide::BlockGroup::value_type f1(test_utils::make_blob(5));
    tide::BlockGroup::value_type f2(test_utils::make_blob(10));
    b1.push_back(f1);
    b1.push_back(f2);
    tide::BlockAdditions::value_type addition(new
            tide::BlockAdditions::Addition(2,
                *test_utils::make_blob(5)));
    b1.additions().push_back(addition);
    b1.duration(42);
    b1.ref_priority(84);
    b1.ref_blocks().push_back(168);
    std::vector<char> blob(*test_utils::make_blob(10));
    b1.codec_state(blob);

    EXPECT_TRUE(b2.empty());
    b2 = b1;
    EXPECT_EQ(b1.track_number(), b2.track_number());
    EXPECT_EQ(b1.timecode(), b2.timecode());
    EXPECT_EQ(b1.invisible(), b2.invisible());
    EXPECT_EQ(b1.lacing(), b2.lacing());
    EXPECT_FALSE(b2.empty());
    EXPECT_EQ(b2[1]->size(), f2->size());
    EXPECT_TRUE(b2.additions() == b1.additions());
    EXPECT_EQ(b2.duration(), b1.duration());
    EXPECT_EQ(b2.ref_priority(), b1.ref_priority());
    EXPECT_TRUE(b2.ref_blocks() == b1.ref_blocks());
    EXPECT_TRUE(b2.codec_state() == b1.codec_state());
}


TEST(BlockGroup, At)
{
    tide::BlockGroup b(2, 22222, tide::Block::LACING_EBML);
    tide::BlockGroup::value_type f1(test_utils::make_blob(5));
    tide::BlockGroup::value_type f2(test_utils::make_blob(10));
    b.push_back(f1);
    b.push_back(f2);
    EXPECT_EQ(b[1], b.at(1));
    EXPECT_EQ(b.at(1)->size(), f2->size());
    EXPECT_THROW(b.at(2), std::out_of_range);
}


TEST(BlockGroup, SubscriptOperator)
{
    tide::BlockGroup b(2, 22222, tide::Block::LACING_EBML);
    tide::BlockGroup::value_type f1(test_utils::make_blob(5));
    tide::BlockGroup::value_type f2(test_utils::make_blob(10));
    b.push_back(f1);
    b.push_back(f2);
    EXPECT_EQ(b[1], b.at(1));
    EXPECT_EQ(b.at(1)->size(), f2->size());
    EXPECT_NO_THROW(b[2]);

    tide::BlockGroup::value_type f3(test_utils::make_blob(15));
    b[1] = f3;
    EXPECT_EQ(b[1], f3);
    EXPECT_EQ(b[1]->size(), f3->size());
}


TEST(BlockGroup, BeginEnd)
{
    tide::BlockGroup b(2, 22222, tide::Block::LACING_EBML);
    tide::BlockGroup::value_type f1(test_utils::make_blob(5));

    EXPECT_TRUE(b.begin() == b.end());
    EXPECT_TRUE(b.rbegin() == b.rend());
    b.push_back(f1);
    EXPECT_FALSE(b.begin() == b.end());
    EXPECT_FALSE(b.rbegin() == b.rend());
}


TEST(BlockGroup, Counts)
{
    tide::BlockGroup b(2, 22222, tide::Block::LACING_EBML);
    tide::BlockGroup::value_type f1(test_utils::make_blob(5));
    tide::BlockGroup::value_type f2(test_utils::make_blob(10));

    EXPECT_TRUE(b.empty());
    b.push_back(f1);
    b.push_back(f2);
    EXPECT_FALSE(b.empty());
    EXPECT_EQ(2, b.count());

    b.lacing(tide::Block::LACING_NONE);
    EXPECT_EQ(1, b.max_count());
    b.lacing(tide::Block::LACING_EBML);
    EXPECT_NE(1, b.max_count());
    b.lacing(tide::Block::LACING_FIXED);
    EXPECT_NE(1, b.max_count());
}


TEST(BlockGroup, Clear)
{
    tide::BlockGroup b(2, 22222, tide::Block::LACING_EBML);
    tide::BlockGroup::value_type f1(test_utils::make_blob(5));
    b.push_back(f1);
    EXPECT_FALSE(b.empty());
    b.clear();
    EXPECT_TRUE(b.empty());
}


TEST(BlockGroup, PushBack)
{
    tide::BlockGroup b(1, 12345, tide::Block::LACING_NONE);
    tide::BlockGroup::value_type f1(test_utils::make_blob(5));
    tide::BlockGroup::value_type f2(test_utils::make_blob(10));
    tide::BlockGroup::value_type f3(test_utils::make_blob(15));
    tide::BlockGroup::value_type empty_frame;

    EXPECT_THROW(b.push_back(empty_frame), tide::EmptyFrame);

    EXPECT_TRUE(b.empty());
    EXPECT_NO_THROW(b.push_back(f1));
    EXPECT_FALSE(b.empty());
    EXPECT_EQ(1, b.count());
    EXPECT_THROW(b.push_back(f2), tide::MaxLaceSizeExceeded);

    b.lacing(tide::Block::LACING_EBML);
    EXPECT_NO_THROW(b.push_back(f2));
    EXPECT_EQ(2, b.count());

    b.lacing(tide::Block::LACING_FIXED);
    EXPECT_THROW(b.push_back(f3), tide::BadLacedFrameSize);
    b.clear();
    b.push_back(f1);
    EXPECT_NO_THROW(b.push_back(f1));
    EXPECT_EQ(2, b.count());

    b.lacing(tide::Block::LACING_NONE);
    EXPECT_THROW(b.push_back(f1), tide::MaxLaceSizeExceeded);
}


TEST(BlockGroup, Erase)
{
    tide::BlockGroup b(1, 12345, tide::Block::LACING_NONE);
    tide::BlockGroup::value_type f1(test_utils::make_blob(5));
    b.push_back(f1);

    EXPECT_FALSE(b.empty());
    b.erase(b.begin());
    EXPECT_TRUE(b.empty());

    b.push_back(f1);
    EXPECT_FALSE(b.empty());
    b.erase(b.begin(), b.end());
    EXPECT_TRUE(b.empty());
}


TEST(BlockGroup, Resize)
{
    tide::BlockGroup b(1, 12345, tide::Block::LACING_NONE);
    tide::BlockGroup::value_type f1(test_utils::make_blob(5));
    tide::BlockGroup::value_type f2(test_utils::make_blob(10));
    tide::BlockGroup::value_type f3(test_utils::make_blob(15));

    EXPECT_THROW(b.resize(2), tide::MaxLaceSizeExceeded);
    b.lacing(tide::Block::LACING_EBML);
    EXPECT_NO_THROW(b.resize(2));
    EXPECT_EQ(2, b.count());
    EXPECT_TRUE(!b[1]);
    b.lacing(tide::Block::LACING_FIXED);
    EXPECT_NO_THROW(b.resize(3));
    EXPECT_EQ(3, b.count());
    b.lacing(tide::Block::LACING_NONE);
    EXPECT_NO_THROW(b.resize(1));
    EXPECT_EQ(1, b.count());
}


TEST(BlockGroup, Swap)
{
    tide::BlockGroup b1(1, 12345, tide::Block::LACING_NONE);
    tide::BlockGroup b2(2, 22222, tide::Block::LACING_EBML);
    tide::BlockGroup::value_type f1(test_utils::make_blob(5));
    tide::BlockGroup::value_type f2(test_utils::make_blob(10));
    tide::BlockGroup::value_type f3(test_utils::make_blob(15));
    b1.invisible(true);
    b1.push_back(f1);
    b2.push_back(f2);
    b2.push_back(f3);
    tide::BlockAdditions::value_type addition(new
            tide::BlockAdditions::Addition(2,
                *test_utils::make_blob(5)));
    b1.additions().push_back(addition);
    b1.duration(42);
    b1.ref_priority(84);
    b1.ref_blocks().push_back(168);
    std::vector<char> blob(*test_utils::make_blob(10));
    b1.codec_state(blob);

    b2.swap(b1);
    EXPECT_EQ(2, b1.track_number());
    EXPECT_EQ(1, b2.track_number());
    EXPECT_EQ(22222, b1.timecode());
    EXPECT_EQ(12345, b2.timecode());
    EXPECT_FALSE(b1.invisible());
    EXPECT_TRUE(b2.invisible());
    EXPECT_EQ(tide::Block::LACING_EBML, b1.lacing());
    EXPECT_EQ(tide::Block::LACING_NONE, b2.lacing());
    EXPECT_EQ(2, b1.count());
    EXPECT_EQ(1, b2.count());
    EXPECT_TRUE(f2 == b1[0]);
    EXPECT_TRUE(f1 == b2[0]);
    EXPECT_TRUE(b1.additions().empty());
    EXPECT_FALSE(b2.additions().empty());
    EXPECT_EQ(0, b1.duration());
    EXPECT_EQ(42, b2.duration());
    EXPECT_EQ(0, b1.ref_priority());
    EXPECT_EQ(84, b2.ref_priority());
    EXPECT_TRUE(b1.ref_blocks().empty());
    EXPECT_EQ(168, b2.ref_blocks()[0]);
    EXPECT_TRUE(b1.codec_state().empty());
    EXPECT_TRUE(b2.codec_state() == blob);
}


TEST(BlockGroup, Equality)
{
    tide::BlockGroup b1(1, 12345, tide::Block::LACING_NONE);
    tide::BlockGroup b2(1, 12345, tide::Block::LACING_NONE);
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

    b1.lacing(tide::Block::LACING_EBML);
    EXPECT_FALSE(b1 == b2);
    EXPECT_TRUE(b1 != b2);
    b1.lacing(tide::Block::LACING_NONE);

    tide::BlockAdditions::value_type addition(new
            tide::BlockAdditions::Addition(2,
                *test_utils::make_blob(5)));
    b1.additions().push_back(addition);
    EXPECT_FALSE(b1 == b2);
    EXPECT_TRUE(b1 != b2);
    b1.additions().clear();

    b1.duration(42);
    EXPECT_FALSE(b1 == b2);
    EXPECT_TRUE(b1 != b2);
    b1.duration(0);

    b1.ref_priority(42);
    EXPECT_FALSE(b1 == b2);
    EXPECT_TRUE(b1 != b2);
    b1.ref_priority(0);

    b1.ref_blocks().push_back(42);
    EXPECT_FALSE(b1 == b2);
    EXPECT_TRUE(b1 != b2);
    b1.ref_blocks().clear();

    std::vector<char> blob(*test_utils::make_blob(10));
    b1.codec_state(blob);
    EXPECT_FALSE(b1 == b2);
    EXPECT_TRUE(b1 != b2);
    b1.codec_state(std::vector<char>());

    tide::BlockGroup::value_type f1(test_utils::make_blob(5));
    b1.push_back(f1);
    EXPECT_FALSE(b1 == b2);
    EXPECT_TRUE(b1 != b2);
}


TEST(BlockGroup, Size)
{
    tide::BlockGroup b(1, 12345, tide::Block::LACING_NONE);
    tide::Block::value_type f1(test_utils::make_blob(5));
    tide::Block::value_type f2(test_utils::make_blob(10));
    tide::Block::value_type f3(test_utils::make_blob(15));
    std::streamsize frames_size = f1->size() + f2->size() + f3->size();

    b.push_back(f1);
    // The 3 bytes are for the timecode and flags
    std::streamsize body_size(tide::vint::size(1) + 3 + f1->size());
    body_size += tide::ids::size(tide::ids::Block) +
        tide::vint::size(body_size);
    EXPECT_EQ(tide::ids::size(tide::ids::BlockGroup) +
            tide::vint::size(body_size) + body_size, b.size());

    b.lacing(tide::Block::LACING_EBML);
    b.push_back(f2);
    b.push_back(f3);
    // Extra 1 byte for number of frames in the lace
    std::streamsize block_size = tide::vint::size(1) + 3 + 1 +
        tide::vint::size(f1->size()) + tide::vint::s_to_u(f2->size() -
                f1->size()).second + frames_size;
    block_size += tide::ids::size(tide::ids::Block) +
        tide::vint::size(block_size);
    EXPECT_EQ(tide::ids::size(tide::ids::BlockGroup) +
            tide::vint::size(block_size) + block_size, b.size());

    b.lacing(tide::Block::LACING_FIXED);
    b.clear();
    b.push_back(f1);
    b.push_back(f1);
    b.push_back(f1);
    // Extra 1 byte for number of frames in the lace
    body_size = tide::vint::size(1) + 3 + 1 + 3 * f1->size();
    body_size += tide::ids::size(tide::ids::Block) +
        tide::vint::size(body_size);
    EXPECT_EQ(tide::ids::size(tide::ids::BlockGroup) +
            tide::vint::size(body_size) + body_size, b.size());

    // Add in the BlockGroup extras
    tide::BlockAdditions::value_type addition(new
            tide::BlockAdditions::Addition(2,
                *test_utils::make_blob(5)));
    tide::BlockAdditions additions;
    additions.push_back(addition);
    tide::UIntElement duration(tide::ids::BlockDuration, 42);
    tide::UIntElement ref_priority(tide::ids::ReferencePriority, 84);
    tide::IntElement ref_block(tide::ids::ReferenceBlock, 168);
    std::vector<char> blob(*test_utils::make_blob(10));
    tide::BinaryElement codec_state(tide::ids::CodecState, blob);

    b.lacing(tide::Block::LACING_NONE);
    b.clear();
    b.push_back(f1);
    b.additions().push_back(addition);
    b.duration(duration);
    b.ref_priority(ref_priority);
    b.ref_blocks().push_back(ref_block);
    b.codec_state(codec_state);

    block_size = tide::vint::size(1) + 3 + f1->size();
    block_size += tide::ids::size(tide::ids::Block) +
        tide::vint::size(block_size);
    body_size = additions.size() + duration.size() + ref_priority.size() +
        ref_block.size() + codec_state.size() + block_size;
    EXPECT_EQ(tide::ids::size(tide::ids::BlockGroup) +
            tide::vint::size(body_size) + body_size, b.size());
}


TEST(BlockGroup, Write)
{
    std::ostringstream output;
    std::stringstream expected;
    std::streamsize expected_size(0);

    unsigned int track_num(1);
    unsigned int timecode(12345);

    tide::BlockGroup b(track_num, timecode);
    std::streamsize id_size = tide::ids::size(tide::ids::BlockGroup);

    tide::Block::value_type f1(test_utils::make_blob(5));
    tide::Block::value_type f2(test_utils::make_blob(10));
    tide::Block::value_type f3(test_utils::make_blob(15));
    std::streamsize frame_size = f1->size() + f2->size() + f3->size();
    tide::vint::OffsetInt f2_lace_size(tide::vint::s_to_u(f2->size() -
                f1->size()));

    // No lacing
    b.lacing(tide::Block::LACING_NONE);
    b.push_back(f1);
    std::streamsize block_size = tide::vint::size(track_num) + 3 + f1->size();
    expected_size = tide::ids::size(tide::ids::Block) +
        tide::vint::size(block_size) + block_size;
    tide::ids::write(tide::ids::BlockGroup, expected);
    tide::vint::write(expected_size, expected);
    tide::ids::write(tide::ids::Block, expected);
    tide::vint::write(block_size, expected);
    tide::vint::write(track_num, expected);
    expected.put(timecode >> 8);
    expected.put(timecode & 0xFF);
    expected.put(0); // Flags
    expected.write(&(*f1)[0], f1->size());
    EXPECT_EQ(id_size + tide::vint::size(expected_size) + expected_size,
            b.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());

    // EBML lacing
    output.str(std::string());
    expected.str(std::string());
    b.lacing(tide::Block::LACING_EBML);
    b.push_back(f2);
    b.push_back(f3);
    block_size = tide::vint::size(track_num) + 3 + 1 +
        tide::vint::size(f1->size()) +
        tide::vint::s_to_u(f2->size() - f1->size()).second + frame_size;
    expected_size = tide::ids::size(tide::ids::Block) +
        tide::vint::size(block_size) + block_size;
    tide::ids::write(tide::ids::BlockGroup, expected);
    tide::vint::write(expected_size, expected);
    tide::ids::write(tide::ids::Block, expected);
    tide::vint::write(block_size, expected);
    tide::vint::write(track_num, expected);
    expected.put(timecode >> 8);
    expected.put(timecode & 0xFF);
    expected.put(0x60); // Flags
    expected.put(3); // Lace header - number of frames
    tide::vint::write(f1->size(), expected);
    tide::vint::write(f2_lace_size.first, expected, f2_lace_size.second);
    expected.write(&(*f1)[0], f1->size());
    expected.write(&(*f2)[0], f2->size());
    expected.write(&(*f3)[0], f3->size());
    EXPECT_EQ(id_size + tide::vint::size(expected_size) + expected_size,
            b.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());

    // Fixed lacing
    output.str(std::string());
    expected.str(std::string());
    b.lacing(tide::Block::LACING_FIXED);
    b.clear();
    b.push_back(f1);
    b.push_back(f1);
    b.push_back(f1);
    block_size = tide::vint::size(track_num) + 3 + 1 + 3 * f1->size();
    expected_size = tide::ids::size(tide::ids::Block) +
        tide::vint::size(block_size) + block_size;
    tide::ids::write(tide::ids::BlockGroup, expected);
    tide::vint::write(expected_size, expected);
    tide::ids::write(tide::ids::Block, expected);
    tide::vint::write(block_size, expected);
    tide::vint::write(track_num, expected);
    expected.put(timecode >> 8);
    expected.put(timecode & 0xFF);
    expected.put(0x40); // Flags
    expected.put(3); // Lace header - number of frames
    expected.write(&(*f1)[0], f1->size());
    expected.write(&(*f1)[0], f1->size());
    expected.write(&(*f1)[0], f1->size());
    EXPECT_EQ(id_size + tide::vint::size(expected_size) + expected_size,
            b.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());

    // Invisible flag set
    output.str(std::string());
    expected.str(std::string());
    b.invisible(true);
    b.lacing(tide::Block::LACING_NONE);
    b.clear();
    b.push_back(f1);
    block_size = tide::vint::size(track_num) + 3 + f1->size();
    expected_size = tide::ids::size(tide::ids::Block) +
        tide::vint::size(block_size) + block_size;
    tide::ids::write(tide::ids::BlockGroup, expected);
    tide::vint::write(expected_size, expected);
    tide::ids::write(tide::ids::Block, expected);
    tide::vint::write(block_size, expected);
    tide::vint::write(track_num, expected);
    expected.put(timecode >> 8);
    expected.put(timecode & 0xFF);
    expected.put(0x10); // Flags
    expected.write(&(*f1)[0], f1->size());
    EXPECT_EQ(id_size + tide::vint::size(expected_size) + expected_size,
            b.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());

    // Add in the BlockGroup extras
    tide::BlockAdditions::value_type addition(new
            tide::BlockAdditions::Addition(2,
                *test_utils::make_blob(5)));
    tide::BlockAdditions additions;
    additions.push_back(addition);
    tide::UIntElement duration(tide::ids::BlockDuration, 42);
    tide::UIntElement ref_priority(tide::ids::ReferencePriority, 84);
    tide::IntElement ref_block(tide::ids::ReferenceBlock, 168);
    std::vector<char> blob(*test_utils::make_blob(10));
    tide::BinaryElement codec_state(tide::ids::CodecState, blob);
    block_size = tide::vint::size(track_num) + 3 + f1->size();
    expected_size = additions.size() + duration.size() +
        ref_priority.size() + ref_block.size() + codec_state.size() +
        block_size + tide::ids::size(tide::ids::Block) +
        tide::vint::size(block_size);
    b.lacing(tide::Block::LACING_NONE);
    b.clear();
    b.push_back(f1);
    b.additions().push_back(addition);
    b.duration(duration);
    b.ref_priority(ref_priority);
    b.ref_blocks().push_back(ref_block);
    b.codec_state(codec_state);
    tide::ids::write(tide::ids::BlockGroup, expected);
    tide::vint::write(expected_size, expected);
    tide::ids::write(tide::ids::Block, expected);
    tide::vint::write(block_size, expected);
    tide::vint::write(track_num, expected);
    expected.put(timecode >> 8);
    expected.put(timecode & 0xFF);
    expected.put(0); // Flags
    expected.write(&(*f1)[0], f1->size());
    additions.write(expected);
    duration.write(expected);
    ref_priority.write(expected);
    ref_block.write(expected);
    codec_state.write(expected);
    EXPECT_EQ(id_size + tide::vint::size(expected_size) + expected_size,
            b.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());

    // Empty block
    b.clear();
    EXPECT_THROW(b.write(output), tide::EmptyBlock);

    // Empty frame
    tide::Block::value_type empty_frame;
    b.clear();
    b.push_back(f1);
    b[0] = empty_frame;
    EXPECT_THROW(b.write(output), tide::EmptyFrame);

    // Unequal frame sizes
    b.clear();
    b.lacing(tide::Block::LACING_EBML);
    b.push_back(f1);
    b.push_back(f2);
    b.lacing(tide::Block::LACING_FIXED);
    EXPECT_THROW(b.write(output), tide::BadLacedFrameSize);
}


TEST(BlockGroup, Read)
{
    std::stringstream input;
    std::streamsize expected_size(0);

    unsigned int track_num(1);
    unsigned int timecode(12345);

    tide::BlockGroup b(0, 0);
    b.lacing(tide::Block::LACING_FIXED);
    b.invisible(true);

    tide::Block::value_type f1(test_utils::make_blob(5));
    tide::Block::value_type f2(test_utils::make_blob(8));
    tide::Block::value_type f3(test_utils::make_blob(6));
    std::streamsize frame_size = f1->size() + f2->size() + f3->size();
    tide::vint::OffsetInt f2_lace_size(tide::vint::s_to_u(f2->size() -
                f1->size()));

    // No lacing
    std::streamsize block_size = tide::vint::size(track_num) + 3 + f1->size();
    expected_size = tide::ids::size(tide::ids::Block) +
        tide::vint::size(block_size) + block_size;
    tide::vint::write(expected_size, input);
    tide::ids::write(tide::ids::Block, input);
    tide::vint::write(block_size, input);
    tide::vint::write(track_num, input);
    input.put(timecode >> 8);
    input.put(timecode & 0xFF);
    input.put(0); // Flags
    input.write(&(*f1)[0], f1->size());
    EXPECT_EQ(tide::vint::size(expected_size) + expected_size,
            b.read(input));
    EXPECT_EQ(track_num, b.track_number());
    EXPECT_EQ(timecode, b.timecode());
    EXPECT_FALSE(b.invisible());
    EXPECT_EQ(tide::Block::LACING_NONE, b.lacing());
    EXPECT_EQ(1, b.count());
    EXPECT_EQ(f1->size(), b[0]->size());

    // EBML lacing
    input.str(std::string());
    b.clear();
    b.track_number(0);
    b.timecode(0);
    b.lacing(tide::Block::LACING_FIXED);
    b.invisible(true);
    block_size = tide::vint::size(track_num) + 3 + 1 +
        tide::vint::size(f1->size()) +
        tide::vint::s_to_u(f2->size() - f1->size()).second + frame_size;
    expected_size = tide::ids::size(tide::ids::Block) +
        tide::vint::size(block_size) + block_size;
    tide::vint::write(expected_size, input);
    tide::ids::write(tide::ids::Block, input);
    tide::vint::write(block_size, input);
    tide::vint::write(track_num, input);
    input.put(timecode >> 8);
    input.put(timecode & 0xFF);
    input.put(0x60); // Flags
    input.put(3); // Lace header - number of frames
    tide::vint::write(f1->size(), input);
    tide::vint::write(f2_lace_size.first, input, f2_lace_size.second);
    input.write(&(*f1)[0], f1->size());
    input.write(&(*f2)[0], f2->size());
    input.write(&(*f3)[0], f3->size());
    EXPECT_EQ(tide::vint::size(expected_size) + expected_size,
            b.read(input));
    EXPECT_EQ(track_num, b.track_number());
    EXPECT_EQ(timecode, b.timecode());
    EXPECT_FALSE(b.invisible());
    EXPECT_EQ(tide::Block::LACING_EBML, b.lacing());
    EXPECT_EQ(3, b.count());
    EXPECT_EQ(f1->size(), b[0]->size());
    EXPECT_EQ(f2->size(), b[1]->size());
    EXPECT_EQ(f3->size(), b[2]->size());

    // Fixed lacing
    input.str(std::string());
    b.clear();
    b.track_number(0);
    b.timecode(0);
    b.lacing(tide::Block::LACING_NONE);
    b.invisible(true);
    block_size = tide::vint::size(track_num) + 3 + 1 + 3 * f1->size();
    expected_size = tide::ids::size(tide::ids::Block) +
        tide::vint::size(block_size) + block_size;
    tide::vint::write(expected_size, input);
    tide::ids::write(tide::ids::Block, input);
    tide::vint::write(block_size, input);
    tide::vint::write(track_num, input);
    input.put(timecode >> 8);
    input.put(timecode & 0xFF);
    input.put(0x40); // Flags
    input.put(3); // Lace header - number of frames
    input.write(&(*f1)[0], f1->size());
    input.write(&(*f1)[0], f1->size());
    input.write(&(*f1)[0], f1->size());
    EXPECT_EQ(tide::vint::size(expected_size) + expected_size,
            b.read(input));
    EXPECT_EQ(track_num, b.track_number());
    EXPECT_EQ(timecode, b.timecode());
    EXPECT_FALSE(b.invisible());
    EXPECT_EQ(tide::Block::LACING_FIXED, b.lacing());
    EXPECT_EQ(3, b.count());
    EXPECT_EQ(f1->size(), b[0]->size());
    EXPECT_EQ(f1->size(), b[1]->size());
    EXPECT_EQ(f1->size(), b[2]->size());

    // Invisible flag set
    input.str(std::string());
    b.clear();
    b.track_number(0);
    b.timecode(0);
    b.lacing(tide::Block::LACING_NONE);
    b.invisible(false);
    block_size = tide::vint::size(track_num) + 3 + f1->size();
    expected_size = tide::ids::size(tide::ids::Block) +
        tide::vint::size(block_size) + block_size;
    tide::vint::write(expected_size, input);
    tide::ids::write(tide::ids::Block, input);
    tide::vint::write(block_size, input);
    tide::vint::write(track_num, input);
    input.put(timecode >> 8);
    input.put(timecode & 0xFF);
    input.put(0x10); // Flags
    input.write(&(*f1)[0], f1->size());
    EXPECT_EQ(tide::vint::size(expected_size) + expected_size,
            b.read(input));
    EXPECT_TRUE(b.invisible());

    // Read error
    input.str(std::string());
    b.lacing(tide::Block::LACING_EBML);
    b.push_back(f2);
    b.push_back(f3);
    block_size = tide::vint::size(track_num) + 3 + 1 +
        tide::vint::size(f1->size()) +
        tide::vint::s_to_u(f2->size() - f1->size()).second + frame_size;
    expected_size = tide::ids::size(tide::ids::Block) +
        tide::vint::size(block_size) + block_size;
    tide::vint::write(expected_size, input);
    tide::ids::write(tide::ids::Block, input);
    tide::vint::write(block_size, input);
    tide::vint::write(track_num, input);
    input.put(timecode >> 8);
    input.put(timecode & 0xFF);
    input.put(0x60); // Flags
    input.put(3); // Lace header - number of frames
    tide::vint::write(f1->size(), input);
    tide::vint::write(f2_lace_size.first, input, f2_lace_size.second);
    input.write(&(*f1)[0], f1->size());
    input.write(&(*f2)[0], f2->size());
    EXPECT_THROW(b.read(input), tide::ReadError);
    input.clear();

    // Bad body size
    input.str(std::string());
    block_size = tide::vint::size(track_num) + 3;
    expected_size = tide::ids::size(tide::ids::Block) +
        tide::vint::size(block_size) + block_size;
    tide::vint::write(expected_size, input);
    tide::ids::write(tide::ids::Block, input);
    tide::vint::write(block_size, input);
    tide::vint::write(track_num, input);
    input.put(timecode >> 8);
    input.put(timecode & 0xFF);
    input.put(0x00); // Flags
    EXPECT_THROW(b.read(input), tide::BadBodySize);

    // Bad frame size (due to missing data)
    input.str(std::string());
    block_size = tide::vint::size(track_num) + 3 + 1 +
        tide::vint::size(f1->size()) +
        tide::vint::s_to_u(f2->size() - f1->size()).second;
    expected_size = tide::ids::size(tide::ids::Block) +
        tide::vint::size(block_size) + block_size;
    tide::vint::write(expected_size, input);
    tide::ids::write(tide::ids::Block, input);
    tide::vint::write(block_size, input);
    tide::vint::write(track_num, input);
    input.put(timecode >> 8);
    input.put(timecode & 0xFF);
    input.put(0x60); // Flags - EBML lacing
    input.put(3); // Lace header - number of frames
    tide::vint::write(f1->size(), input);
    tide::vint::write(f2_lace_size.first, input, f2_lace_size.second);
    EXPECT_THROW(b.read(input), tide::BadLacedFrameSize);

    // Missing frame - EBML lacing
    input.str(std::string());
    block_size = tide::vint::size(track_num) + 3 + 1 +
        tide::vint::size(f1->size()) +
        tide::vint::s_to_u(f2->size() - f1->size()).second + f1->size() +
        f2->size();
    expected_size = tide::ids::size(tide::ids::Block) +
        tide::vint::size(block_size) + block_size;
    tide::vint::write(expected_size, input);
    tide::ids::write(tide::ids::Block, input);
    tide::vint::write(block_size, input);
    tide::vint::write(track_num, input);
    input.put(timecode >> 8);
    input.put(timecode & 0xFF);
    input.put(0x60); // Flags - EBML lacing
    input.put(3); // Lace header - number of frames
    tide::vint::write(f1->size(), input);
    tide::vint::write(f2_lace_size.first, input, f2_lace_size.second);
    input.write(&(*f1)[0], f1->size());
    input.write(&(*f2)[0], f2->size());
    // No 3rd frame
    EXPECT_THROW(b.read(input), tide::EmptyFrame);

    // Missing frame - fixed lacing
    input.str(std::string());
    block_size = tide::vint::size(track_num) + 3 + 1 +
        f1->size() + f1->size();
    expected_size = tide::ids::size(tide::ids::Block) +
        tide::vint::size(block_size) + block_size;
    tide::vint::write(expected_size, input);
    tide::ids::write(tide::ids::Block, input);
    tide::vint::write(block_size, input);
    tide::vint::write(track_num, input);
    input.put(timecode >> 8);
    input.put(timecode & 0xFF);
    input.put(0x40); // Flags - fixed lacing
    input.put(3); // Lace header - number of frames
    input.write(&(*f1)[0], f1->size());
    input.write(&(*f1)[0], f1->size());
    // No 3rd frame
    EXPECT_THROW(b.read(input), tide::BadLacedFrameSize);

    // Unequal frame sizes
    input.str(std::string());
    block_size = tide::vint::size(track_num) + 3 + 1 + f1->size() +
        f2->size();
    expected_size = tide::ids::size(tide::ids::Block) +
        tide::vint::size(block_size) + block_size;
    tide::vint::write(expected_size, input);
    tide::ids::write(tide::ids::Block, input);
    tide::vint::write(block_size, input);
    tide::vint::write(track_num, input);
    input.put(timecode >> 8);
    input.put(timecode & 0xFF);
    input.put(0x40); // Flags
    input.put(2); // Lace header - number of frames
    input.write(&(*f1)[0], f1->size());
    input.write(&(*f2)[0], f1->size());
    EXPECT_THROW(b.read(input), tide::BadLacedFrameSize);

    // Add in the BlockGroup extras
    tide::BlockAdditions::value_type addition(new
            tide::BlockAdditions::Addition(2,
                *test_utils::make_blob(5)));
    tide::BlockAdditions additions;
    additions.push_back(addition);
    tide::UIntElement duration(tide::ids::BlockDuration, 42);
    tide::UIntElement ref_priority(tide::ids::ReferencePriority, 84);
    tide::IntElement ref_block(tide::ids::ReferenceBlock, 168);
    std::vector<char> blob(*test_utils::make_blob(10));
    tide::BinaryElement codec_state(tide::ids::CodecState, blob);
    block_size = tide::vint::size(track_num) + 3 + f1->size();
    expected_size = additions.size() + duration.size() +
        ref_priority.size() + ref_block.size() + codec_state.size() +
        block_size + tide::ids::size(tide::ids::Block) +
        tide::vint::size(block_size);

    b.lacing(tide::Block::LACING_NONE);
    b.clear();
    b.push_back(f1);
    b.additions().push_back(addition);
    b.duration(duration);
    b.ref_priority(ref_priority);
    b.ref_blocks().push_back(ref_block);
    b.codec_state(codec_state);

    input.str(std::string());
    tide::vint::write(expected_size, input);
    tide::ids::write(tide::ids::Block, input);
    tide::vint::write(block_size, input);
    tide::vint::write(track_num, input);
    input.put(timecode >> 8);
    input.put(timecode & 0xFF);
    input.put(0); // Flags
    input.write(&(*f1)[0], f1->size());
    additions.write(input);
    duration.write(input);
    ref_priority.write(input);
    ref_block.write(input);
    codec_state.write(input);
    EXPECT_EQ(tide::vint::size(expected_size) + expected_size,
            b.read(input));
    EXPECT_EQ(track_num, b.track_number());
    EXPECT_EQ(timecode, b.timecode());
    EXPECT_FALSE(b.invisible());
    EXPECT_EQ(tide::Block::LACING_NONE, b.lacing());
    EXPECT_EQ(1, b.count());
    EXPECT_EQ(f1->size(), b[0]->size());
    EXPECT_EQ(addition->first, b.additions()[0]->first);
    EXPECT_TRUE(addition->second == b.additions()[0]->second);
    EXPECT_EQ(duration, b.duration());
    EXPECT_EQ(ref_priority, b.ref_priority());
    EXPECT_EQ(ref_block, b.ref_blocks()[0]);
    EXPECT_TRUE(codec_state.value() == b.codec_state());
}

