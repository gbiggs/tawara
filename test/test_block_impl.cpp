/* Tide
 *
 * Test the BlockImpl object.
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
#include <tide/block_impl.h>
#include <tide/el_ids.h>
#include <tide/exceptions.h>
#include <tide/vint.h>

#include "test_utils.h"


TEST(BlockImpl, Create)
{
    tide::BlockImpl b1(1, 12345);
    EXPECT_EQ(1, b1.track_number());
    EXPECT_EQ(12345, b1.timecode());
    EXPECT_FALSE(b1.invisible());
    EXPECT_EQ(tide::Block::LACING_NONE, b1.lacing());
    EXPECT_TRUE(b1.empty());

    tide::BlockImpl b2(2, 22222, tide::Block::LACING_EBML);
    EXPECT_EQ(2, b2.track_number());
    EXPECT_EQ(22222, b2.timecode());
    EXPECT_FALSE(b2.invisible());
    EXPECT_EQ(tide::Block::LACING_EBML, b2.lacing());
    EXPECT_TRUE(b2.empty());

}


TEST(BlockImpl, TrackNumber)
{
    tide::BlockImpl b1(1, 12345);
    EXPECT_EQ(1, b1.track_number());
    b1.track_number(42);
    EXPECT_EQ(42, b1.track_number());
}


TEST(BlockImpl, Timecode)
{
    tide::BlockImpl b1(1, 12345);
    EXPECT_EQ(12345, b1.timecode());
    b1.timecode(22222);
    EXPECT_EQ(22222, b1.timecode());
}


TEST(BlockImpl, Invisible)
{
    tide::BlockImpl b1(2, 22222, tide::Block::LACING_EBML);
    EXPECT_FALSE(b1.invisible());
    b1.invisible(true);
    EXPECT_TRUE(b1.invisible());
}


TEST(BlockImpl, Lacing)
{
    tide::BlockImpl b1(1, 12345);
    EXPECT_EQ(tide::Block::LACING_NONE, b1.lacing());
    b1.lacing(tide::Block::LACING_EBML);
    EXPECT_EQ(tide::Block::LACING_EBML, b1.lacing());
    b1.lacing(tide::Block::LACING_FIXED);
    EXPECT_EQ(tide::Block::LACING_FIXED, b1.lacing());
}


TEST(BlockImpl, Assignment)
{
    tide::BlockImpl b1(1, 12345, tide::Block::LACING_EBML);
    tide::BlockImpl b2(2, 22222, tide::Block::LACING_FIXED);
    tide::Block::value_type f1(test_utils::make_frame());
    tide::Block::value_type f2(test_utils::make_frame());
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
    tide::BlockImpl b(2, 22222, tide::Block::LACING_EBML);
    tide::Block::value_type f1(test_utils::make_frame());
    tide::Block::value_type f2(test_utils::make_frame());
    b.push_back(f1);
    b.push_back(f2);
    EXPECT_EQ(b[1], b.at(1));
    EXPECT_EQ(b.at(1)->size(), f2->size());
    EXPECT_THROW(b.at(2), std::out_of_range);
}


TEST(BlockImpl, SubscriptOperator)
{
    tide::BlockImpl b(2, 22222, tide::Block::LACING_EBML);
    tide::Block::value_type f1(test_utils::make_frame());
    tide::Block::value_type f2(test_utils::make_frame());
    b.push_back(f1);
    b.push_back(f2);
    EXPECT_EQ(b[1], b.at(1));
    EXPECT_EQ(b.at(1)->size(), f2->size());
    EXPECT_NO_THROW(b[2]);

    tide::Block::value_type f3(test_utils::make_frame());
    b[1] = f3;
    EXPECT_EQ(b[1], f3);
    EXPECT_EQ(b[1]->size(), f3->size());
}


TEST(BlockImpl, BeginEnd)
{
    tide::BlockImpl b(2, 22222, tide::Block::LACING_EBML);
    tide::Block::value_type f1(test_utils::make_frame());

    EXPECT_TRUE(b.begin() == b.end());
    EXPECT_TRUE(b.rbegin() == b.rend());
    b.push_back(f1);
    EXPECT_FALSE(b.begin() == b.end());
    EXPECT_FALSE(b.rbegin() == b.rend());
}


TEST(BlockImpl, Counts)
{
    tide::BlockImpl b(2, 22222, tide::Block::LACING_EBML);
    tide::Block::value_type f1(test_utils::make_frame());
    tide::Block::value_type f2(test_utils::make_frame());

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


TEST(BlockImpl, Clear)
{
    tide::BlockImpl b(2, 22222, tide::Block::LACING_EBML);
    tide::Block::value_type f1(test_utils::make_frame());
    b.push_back(f1);
    EXPECT_FALSE(b.empty());
    b.clear();
    EXPECT_TRUE(b.empty());
}


TEST(BlockImpl, PushBack)
{
    tide::BlockImpl b(1, 12345, tide::Block::LACING_NONE);
    tide::Block::value_type f1(test_utils::make_frame());
    tide::Block::value_type f2(test_utils::make_frame());
    tide::Block::value_type f3(test_utils::make_frame());
    tide::Block::value_type empty_frame;

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
    b.clear();
    b.push_back(f1);
    EXPECT_NO_THROW(b.push_back(f1));
    EXPECT_THROW(b.push_back(f2), tide::BadLacedFrameSize);
    EXPECT_EQ(2, b.count());

    b.lacing(tide::Block::LACING_NONE);
    EXPECT_THROW(b.push_back(f1), tide::MaxLaceSizeExceeded);
}


TEST(BlockImpl, Erase)
{
    tide::BlockImpl b(1, 12345, tide::Block::LACING_NONE);
    tide::Block::value_type f1(test_utils::make_frame());
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
    tide::BlockImpl b(1, 12345, tide::Block::LACING_NONE);
    tide::Block::value_type f1(test_utils::make_frame());
    tide::Block::value_type f2(test_utils::make_frame());
    tide::Block::value_type f3(test_utils::make_frame());

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


TEST(BlockImpl, Swap)
{
    tide::BlockImpl b1(1, 12345, tide::Block::LACING_NONE);
    tide::BlockImpl b2(2, 22222, tide::Block::LACING_EBML);
    tide::Block::value_type f1(test_utils::make_frame());
    tide::Block::value_type f2(test_utils::make_frame());
    tide::Block::value_type f3(test_utils::make_frame());
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
    EXPECT_EQ(tide::Block::LACING_EBML, b1.lacing());
    EXPECT_EQ(tide::Block::LACING_NONE, b2.lacing());
    EXPECT_EQ(2, b1.count());
    EXPECT_EQ(1, b2.count());
    EXPECT_TRUE(f2 == b1[0]);
    EXPECT_TRUE(f1 == b2[0]);
}


TEST(BlockImpl, Equality)
{
    tide::BlockImpl b1(1, 12345, tide::Block::LACING_NONE);
    tide::BlockImpl b2(1, 12345, tide::Block::LACING_NONE);
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

    tide::Block::value_type f1(test_utils::make_frame());
    b1.push_back(f1);
    EXPECT_FALSE(b1 == b2);
    EXPECT_TRUE(b1 != b2);
}


TEST(BlockImpl, FrameDataSize)
{
    tide::BlockImpl b(1, 12345, tide::Block::LACING_NONE);
    tide::Block::value_type f1(test_utils::make_frame());
    tide::Block::value_type f2(test_utils::make_frame());
    tide::Block::value_type f3(test_utils::make_frame());

    b.push_back(f1);
    // The 3 bytes are for the timecode and flags
    EXPECT_EQ(tide::vint::size(1) + 3 + f1->size(), b.frame_data_size());

    b.lacing(tide::Block::LACING_EBML);
    std::streamsize expected_size = f1->size() + f2->size() + f3->size();
    b.push_back(f2);
    b.push_back(f3);
    // Extra 1 byte for number of frames in the lace
    EXPECT_EQ(tide::vint::size(1) + 3 + 1 + tide::vint::size_u(f1->size()) +
            tide::vint::size_u(f2->size() - f1->size() + 0x1F) + expected_size,
            b.frame_data_size());

    b.lacing(tide::Block::LACING_FIXED);
    b.clear();
    b.push_back(f1);
    b.push_back(f1);
    b.push_back(f1);
    // Extra 1 byte for number of frames in the lace
    EXPECT_EQ(tide::vint::size(1) + 3 + 1 + expected_size,
            b.frame_data_size());
}

