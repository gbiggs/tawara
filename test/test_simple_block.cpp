/* Tide
 *
 * Test the SimpleBlock element.
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
#include <tide/simple_block.h>
#include <tide/vint.h>

#include "test_utils.h"


TEST(SimpleBlock, Create)
{
    tide::SimpleBlock b1(1, 12345);
    EXPECT_EQ(1, b1.track_number());
    EXPECT_EQ(12345, b1.timecode());
    EXPECT_FALSE(b1.invisible());
    EXPECT_EQ(tide::Block::LACING_NONE, b1.lacing());
    EXPECT_TRUE(b1.empty());

    tide::SimpleBlock b2(2, 22222, tide::Block::LACING_EBML);
    EXPECT_EQ(2, b2.track_number());
    EXPECT_EQ(22222, b2.timecode());
    EXPECT_FALSE(b2.invisible());
    EXPECT_EQ(tide::Block::LACING_EBML, b2.lacing());
    EXPECT_TRUE(b2.empty());

}


TEST(SimpleBlock, Keyframe)
{
    tide::SimpleBlock b1(1, 12345);
    EXPECT_FALSE(b1.keyframe());
    b1.keyframe(true);
    EXPECT_TRUE(b1.keyframe());
}


TEST(SimpleBlock, Discardable)
{
    tide::SimpleBlock b1(1, 12345);
    EXPECT_FALSE(b1.discardable());
    b1.discardable(true);
    EXPECT_TRUE(b1.discardable());
}


TEST(SimpleBlock, TrackNumber)
{
    tide::SimpleBlock b1(1, 12345);
    EXPECT_EQ(1, b1.track_number());
    b1.track_number(42);
    EXPECT_EQ(42, b1.track_number());
}


TEST(SimpleBlock, Timecode)
{
    tide::SimpleBlock b1(1, 12345);
    EXPECT_EQ(12345, b1.timecode());
    b1.timecode(22222);
    EXPECT_EQ(22222, b1.timecode());
}


TEST(SimpleBlock, Invisible)
{
    tide::SimpleBlock b1(2, 22222, tide::Block::LACING_EBML);
    EXPECT_FALSE(b1.invisible());
    b1.invisible(true);
    EXPECT_TRUE(b1.invisible());
}


TEST(SimpleBlock, Lacing)
{
    tide::SimpleBlock b1(1, 12345);
    EXPECT_EQ(tide::Block::LACING_NONE, b1.lacing());
    b1.lacing(tide::Block::LACING_EBML);
    EXPECT_EQ(tide::Block::LACING_EBML, b1.lacing());
    b1.lacing(tide::Block::LACING_FIXED);
    EXPECT_EQ(tide::Block::LACING_FIXED, b1.lacing());
}


TEST(SimpleBlock, Assignment)
{
    tide::SimpleBlock b1(1, 12345, tide::Block::LACING_EBML);
    tide::SimpleBlock b2(2, 22222, tide::Block::LACING_FIXED);
    tide::SimpleBlock::value_type f1(test_utils::make_frame());
    tide::SimpleBlock::value_type f2(test_utils::make_frame());
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


TEST(SimpleBlock, At)
{
    tide::SimpleBlock b(2, 22222, tide::Block::LACING_EBML);
    tide::SimpleBlock::value_type f1(test_utils::make_frame());
    tide::SimpleBlock::value_type f2(test_utils::make_frame());
    b.push_back(f1);
    b.push_back(f2);
    EXPECT_EQ(b[1], b.at(1));
    EXPECT_EQ(b.at(1)->size(), f2->size());
    EXPECT_THROW(b.at(2), std::out_of_range);
}


TEST(SimpleBlock, SubscriptOperator)
{
    tide::SimpleBlock b(2, 22222, tide::Block::LACING_EBML);
    tide::SimpleBlock::value_type f1(test_utils::make_frame());
    tide::SimpleBlock::value_type f2(test_utils::make_frame());
    b.push_back(f1);
    b.push_back(f2);
    EXPECT_EQ(b[1], b.at(1));
    EXPECT_EQ(b.at(1)->size(), f2->size());
    EXPECT_NO_THROW(b[2]);

    tide::SimpleBlock::value_type f3(test_utils::make_frame());
    b[1] = f3;
    EXPECT_EQ(b[1], f3);
    EXPECT_EQ(b[1]->size(), f3->size());
}


TEST(SimpleBlock, BeginEnd)
{
    tide::SimpleBlock b(2, 22222, tide::Block::LACING_EBML);
    tide::SimpleBlock::value_type f1(test_utils::make_frame());

    EXPECT_TRUE(b.begin() == b.end());
    EXPECT_TRUE(b.rbegin() == b.rend());
    b.push_back(f1);
    EXPECT_FALSE(b.begin() == b.end());
    EXPECT_FALSE(b.rbegin() == b.rend());
}


TEST(SimpleBlock, Counts)
{
    tide::SimpleBlock b(2, 22222, tide::Block::LACING_EBML);
    tide::SimpleBlock::value_type f1(test_utils::make_frame());
    tide::SimpleBlock::value_type f2(test_utils::make_frame());

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


TEST(SimpleBlock, Clear)
{
    tide::SimpleBlock b(2, 22222, tide::Block::LACING_EBML);
    tide::SimpleBlock::value_type f1(test_utils::make_frame());
    b.push_back(f1);
    EXPECT_FALSE(b.empty());
    b.clear();
    EXPECT_TRUE(b.empty());
}


TEST(SimpleBlock, PushBack)
{
    tide::SimpleBlock b(1, 12345, tide::Block::LACING_NONE);
    tide::SimpleBlock::value_type f1(test_utils::make_frame());
    tide::SimpleBlock::value_type f2(test_utils::make_frame());
    tide::SimpleBlock::value_type f3(test_utils::make_frame());
    tide::SimpleBlock::value_type empty_frame;

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
    EXPECT_NO_THROW(b.push_back(f3));
    EXPECT_EQ(3, b.count());

    b.lacing(tide::Block::LACING_NONE);
    EXPECT_THROW(b.push_back(f1), tide::MaxLaceSizeExceeded);
}


TEST(SimpleBlock, Erase)
{
    tide::SimpleBlock b(1, 12345, tide::Block::LACING_NONE);
    tide::SimpleBlock::value_type f1(test_utils::make_frame());
    b.push_back(f1);

    EXPECT_FALSE(b.empty());
    b.erase(b.begin());
    EXPECT_TRUE(b.empty());

    b.push_back(f1);
    EXPECT_FALSE(b.empty());
    b.erase(b.begin(), b.end());
    EXPECT_TRUE(b.empty());
}


TEST(SimpleBlock, Resize)
{
    tide::SimpleBlock b(1, 12345, tide::Block::LACING_NONE);
    tide::SimpleBlock::value_type f1(test_utils::make_frame());
    tide::SimpleBlock::value_type f2(test_utils::make_frame());
    tide::SimpleBlock::value_type f3(test_utils::make_frame());

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


TEST(SimpleBlock, Swap)
{
    tide::SimpleBlock b1(1, 12345, tide::Block::LACING_NONE);
    tide::SimpleBlock b2(2, 22222, tide::Block::LACING_EBML);
    tide::SimpleBlock::value_type f1(test_utils::make_frame());
    tide::SimpleBlock::value_type f2(test_utils::make_frame());
    tide::SimpleBlock::value_type f3(test_utils::make_frame());
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


TEST(SimpleBlock, Equality)
{
    tide::SimpleBlock b1(1, 12345, tide::Block::LACING_NONE);
    tide::SimpleBlock b2(1, 12345, tide::Block::LACING_NONE);
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

    tide::SimpleBlock::value_type f1(test_utils::make_frame());
    b1.push_back(f1);
    EXPECT_FALSE(b1 == b2);
    EXPECT_TRUE(b1 != b2);
}


