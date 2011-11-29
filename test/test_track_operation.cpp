/* Tide
 *
 * Test the TrackOperation and related elements.
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

#include <boost/foreach.hpp>
#include <gtest/gtest.h>
#include <tide/el_ids.h>
#include <tide/exceptions.h>
#include <tide/track_operation.h>
#include <tide/uint_element.h>
#include <tide/vint.h>

#include "test_utils.h"


TEST(TrackJoinBlocks, Create)
{
    tide::TrackJoinBlocks e;
    EXPECT_EQ(tide::ids::TrackJoinBlocks, e.id());
    EXPECT_EQ(0, e.count());
}


TEST(TrackJoinBlocks, Type)
{
    tide::TrackJoinBlocks e;
    EXPECT_EQ(e.type(), "joinblocks");
}


TEST(TrackJoinBlocks, Append)
{
    tide::TrackJoinBlocks e;
    e.append(0xFFFF);
    ASSERT_EQ(1, e.count());
    e.append(0xC0C0);
    ASSERT_EQ(2, e.count());
    EXPECT_EQ(0xFFFF, e[0]);
    EXPECT_EQ(0xC0C0, e[1]);

    EXPECT_THROW(e.append(0), tide::ValueOutOfRange);
}


TEST(TrackJoinBlocks, Remove)
{
    tide::TrackJoinBlocks e;
    e.append(0xFFFF);
    e.append(0xC0C0);
    ASSERT_EQ(2, e.count());
    e.remove(0);
    ASSERT_EQ(1, e.count());
    EXPECT_EQ(0xC0C0, e[0]);
    e.append(0x0101);
    e.remove(1);
    EXPECT_EQ(0xC0C0, e[0]);
}


TEST(TrackJoinBlocks, IndexOperator)
{
    tide::TrackJoinBlocks e;
    e.append(0xFFFF);
    e.append(0xC0C0);
    e.append(0x0101);
    EXPECT_EQ(0xFFFF, e[0]);
    EXPECT_EQ(0x0101, e[2]);
}


TEST(TrackJoinBlocks, Equality)
{
    tide::TrackJoinBlocks e1;
    e1.append(0xFFFF);
    tide::TrackJoinBlocks e2;
    e2.append(0xFFFF);
    EXPECT_TRUE(e1 == e2);
    EXPECT_FALSE(e1 != e2);

    e2.append(0x7777);
    EXPECT_FALSE(e1 == e2);
    EXPECT_TRUE(e1 != e2);
}


TEST(TrackJoinBlocks, Size)
{
    tide::TrackJoinBlocks e;

    EXPECT_EQ(tide::ids::coded_size(tide::ids::TrackJoinBlocks) +
            tide::vint::coded_size(0),
            e.size());

    std::vector<tide::UIntElement> children;
    children.push_back(tide::UIntElement(tide::ids::TrackJoinUID, 0xFFFF));
    children.push_back(tide::UIntElement(tide::ids::TrackJoinUID, 0xC0C0));
    children.push_back(tide::UIntElement(tide::ids::TrackJoinUID, 0x0101));

    std::streamsize body_size(0);
    BOOST_FOREACH(tide::UIntElement child, children)
    {
        body_size += child.size();
        e.append(child.value());
    }

    EXPECT_EQ(tide::ids::coded_size(tide::ids::TrackJoinBlocks) +
            tide::vint::coded_size(body_size) + body_size,
            e.size());
}


TEST(TrackJoinBlocks, Write)
{
    std::ostringstream output;
    std::stringstream expected;

    tide::TrackJoinBlocks e;

    std::vector<tide::UIntElement> children;
    children.push_back(tide::UIntElement(tide::ids::TrackJoinUID, 0xFFFF));
    children.push_back(tide::UIntElement(tide::ids::TrackJoinUID, 0xC0C0));
    children.push_back(tide::UIntElement(tide::ids::TrackJoinUID, 0x0101));

    std::streamsize body_size(0);
    BOOST_FOREACH(tide::UIntElement child, children)
    {
        body_size += child.size();
        e.append(child.value());
    }
    tide::ids::write(tide::ids::TrackJoinBlocks, expected);
    tide::vint::write(body_size, expected);
    BOOST_FOREACH(tide::UIntElement child, children)
    {
        child.write(expected);
    }

    EXPECT_EQ(tide::ids::coded_size(tide::ids::TrackJoinBlocks) +
            tide::vint::coded_size(body_size) + body_size,
            e.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected.str());
}


TEST(TrackJoinBlocks, Read)
{
    std::stringstream input;

    std::vector<tide::UIntElement> children;
    children.push_back(tide::UIntElement(tide::ids::TrackJoinUID, 0xFFFF));
    children.push_back(tide::UIntElement(tide::ids::TrackJoinUID, 0xC0C0));
    children.push_back(tide::UIntElement(tide::ids::TrackJoinUID, 0x0101));

    std::streamsize body_size(0);
    BOOST_FOREACH(tide::UIntElement child, children)
    {
        body_size += child.size();
    }
    tide::vint::write(body_size, input);
    BOOST_FOREACH(tide::UIntElement child, children)
    {
        child.write(input);
    }

    tide::TrackJoinBlocks e;
    EXPECT_EQ(tide::vint::coded_size(body_size) + body_size,
            e.read(input));
    EXPECT_EQ(3, e.count());
    EXPECT_EQ(0xFFFF, e[0]);
    EXPECT_EQ(0xC0C0, e[1]);
    EXPECT_EQ(0x0101, e[2]);

    // No children at all
    input.str(std::string());
    tide::vint::write(0, input);
    EXPECT_THROW(e.read(input), tide::MissingChild);
    // TrackJoinBlocks should be clearing its stored UIDs before reading, so
    // the previous test's index should not affect this result
    EXPECT_EQ(0, e.count());
    // Body size value wrong (too small)
    input.str(std::string());
    tide::vint::write(2, input);
    children[0].write(input);
    EXPECT_THROW(e.read(input), tide::BadBodySize);
    // Invalid child
    input.str(std::string());
    tide::UIntElement ue(tide::ids::EBML, 0xFFFF);
    tide::vint::write(ue.size(), input);
    ue.write(input);
    EXPECT_THROW(e.read(input), tide::InvalidChildID);
}

