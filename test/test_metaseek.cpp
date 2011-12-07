/* Tide
 *
 * Test the SeekHead (metaseek) element.
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
#include <tide/metaseek.h>
#include <tide/uint_element.h>
#include <tide/vint.h>

#include "test_utils.h"


TEST(SeekHead, Create)
{
    tide::SeekHead e;
    EXPECT_EQ(tide::ids::SeekHead, e.id());
    EXPECT_TRUE(e.empty());
}


TEST(SeekHead, Assignment)
{
    tide::SeekHead e1;
    tide::SeekHead e2;
    e1.insert(std::make_pair(tide::ids::Tracks, 42));
    e1.insert(std::make_pair(tide::ids::Cluster, 84));

    EXPECT_TRUE(e2.empty());
    e2 = e1;
    EXPECT_FALSE(e2.empty());
    EXPECT_EQ(e2.begin()->first, tide::ids::Tracks);
    EXPECT_EQ(e2.begin()->second, 42);
    EXPECT_EQ((++e2.begin())->first, tide::ids::Cluster);
    EXPECT_EQ((++e2.begin())->second, 84);
}


TEST(SeekHead, BeginEnd)
{
    tide::SeekHead e;

    EXPECT_TRUE(e.begin() == e.end());
    EXPECT_TRUE(e.rbegin() == e.rend());
    e.insert(std::make_pair(tide::ids::Tracks, 42));
    EXPECT_FALSE(e.begin() == e.end());
    EXPECT_FALSE(e.rbegin() == e.rend());
}


TEST(SeekHead, Counts)
{
    tide::SeekHead e;

    EXPECT_TRUE(e.empty());
    e.insert(std::make_pair(tide::ids::Tracks, 42));
    EXPECT_FALSE(e.empty());
    EXPECT_EQ(1, e.count());
    e.insert(std::make_pair(tide::ids::Cluster, 84));
    EXPECT_EQ(2, e.count());
}


TEST(SeekHead, Clear)
{
    tide::SeekHead e;
    e.insert(std::make_pair(tide::ids::Tracks, 42));
    EXPECT_FALSE(e.empty());
    e.clear();
    EXPECT_TRUE(e.empty());
}


TEST(SeekHead, Insert)
{
    tide::SeekHead e1;

    // Single insert
    EXPECT_TRUE(e1.empty());
    tide::SeekHead::iterator res = e1.insert(std::make_pair(tide::ids::Tracks, 42));
    EXPECT_TRUE(res == e1.begin());
    EXPECT_FALSE(e1.empty());
    EXPECT_EQ(1, e1.count());
    EXPECT_EQ(e1.begin()->first, tide::ids::Tracks);
    res = e1.insert(std::make_pair(tide::ids::Cluster, 84));
    EXPECT_TRUE(res != e1.begin());
    EXPECT_FALSE(e1.empty());
    EXPECT_EQ(2, e1.count());
    EXPECT_EQ((++e1.begin())->first, tide::ids::Cluster);
    EXPECT_EQ((++e1.begin())->second, 84);
    // Same ID
    res = e1.insert(std::make_pair(tide::ids::Cluster, 168));
    EXPECT_TRUE(res != e1.begin());
    EXPECT_TRUE(res == ++(++e1.begin()));
    EXPECT_TRUE(res == ++(e1.find(tide::ids::Cluster)));

    // Range insert
    tide::SeekHead e2;
    EXPECT_TRUE(e2.empty());
    e2.insert(e1.begin(), e1.end());
    EXPECT_FALSE(e2.empty());
    EXPECT_EQ(3, e2.count());
    EXPECT_EQ(e2.begin()->first, tide::ids::Tracks);
}


TEST(SeekHead, Erase)
{
    tide::SeekHead e;
    e.insert(std::make_pair(tide::ids::Tracks, 42));

    EXPECT_FALSE(e.empty());
    e.erase(e.begin());
    EXPECT_TRUE(e.empty());

    e.insert(std::make_pair(tide::ids::Tracks, 42));
    EXPECT_FALSE(e.empty());
    e.erase(e.begin(), e.end());
    EXPECT_TRUE(e.empty());

    e.insert(std::make_pair(tide::ids::Tracks, 42));
    EXPECT_FALSE(e.empty());
    e.erase(tide::ids::Tracks);
    EXPECT_TRUE(e.empty());
    EXPECT_NO_THROW(e.erase(tide::ids::Null));
}


TEST(SeekHead, Swap)
{
    tide::SeekHead e1, e2;
    e1.insert(std::make_pair(tide::ids::Tracks, 42));

    EXPECT_FALSE(e1.empty());
    EXPECT_TRUE(e2.empty());
    e2.swap(e1);
    EXPECT_TRUE(e1.empty());
    EXPECT_FALSE(e2.empty());
    EXPECT_EQ(e2.begin()->first, tide::ids::Tracks);
}


TEST(SeekHead, Find)
{
    tide::SeekHead e;
    e.insert(std::make_pair(tide::ids::Tracks, 42));

    EXPECT_TRUE(e.find(tide::ids::Tracks) == e.begin());
    EXPECT_TRUE(e.find(tide::ids::Null) == e.end());
}


TEST(SeekHead, Equality)
{
    tide::SeekHead e1;
    tide::SeekHead e2;
    EXPECT_TRUE(e1 == e2);
    EXPECT_FALSE(e1 != e2);

    e2.insert(std::make_pair(tide::ids::Tracks, 42));
    EXPECT_FALSE(e1 == e2);
    EXPECT_TRUE(e1 != e2);
}


TEST(SeekHead, Size)
{
    tide::SeekHead ms;

    EXPECT_EQ(tide::ids::size(tide::ids::SeekHead) +
            tide::vint::size(0),
            ms.size());

    std::vector<tide::SeekElement> children;
    children.push_back(tide::SeekElement(tide::ids::Tracks, 0x76FB));
    children.push_back(tide::SeekElement(tide::ids::Cues, 0x7F));
    children.push_back(tide::SeekElement(tide::ids::Cluster, 0x10203040));

    std::streamsize body_size(0);
    BOOST_FOREACH(tide::SeekElement e, children)
    {
        body_size += e.size();
        ms.insert(std::make_pair(e.indexed_id(), e.offset()));
    }
    EXPECT_EQ(tide::ids::size(tide::ids::SeekHead) +
            tide::vint::size(body_size) + body_size,
            ms.size());
}


TEST(SeekHead, Write)
{
    std::ostringstream output;
    std::stringstream expected;

    tide::SeekHead ms;

    std::vector<tide::SeekElement> children;
    children.push_back(tide::SeekElement(tide::ids::Tracks, 0x76FB));
    children.push_back(tide::SeekElement(tide::ids::Cues, 0x7F));
    children.push_back(tide::SeekElement(tide::ids::Cluster, 0x10203040));

    std::streamsize body_size(0);
    BOOST_FOREACH(tide::SeekElement e, children)
    {
        body_size += e.size();
        ms.insert(std::make_pair(e.indexed_id(), e.offset()));
    }
    tide::ids::write(tide::ids::SeekHead, expected);
    tide::vint::write(body_size, expected);
    BOOST_FOREACH(tide::SeekElement e, children)
    {
        e.write(expected);
    }

    EXPECT_EQ(tide::ids::size(tide::ids::SeekHead) +
            tide::vint::size(body_size) + body_size,
            ms.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected.str());
}


TEST(SeekHead, Read)
{
    std::stringstream input;

    std::vector<tide::SeekElement> children;
    children.push_back(tide::SeekElement(tide::ids::Tracks, 0x76FB));
    children.push_back(tide::SeekElement(tide::ids::Cues, 0x7F));
    children.push_back(tide::SeekElement(tide::ids::Cluster, 0x10203040));

    std::streamsize body_size(0);
    BOOST_FOREACH(tide::SeekElement e, children)
    {
        body_size += e.size();
    }
    tide::vint::write(body_size, input);
    BOOST_FOREACH(tide::SeekElement e, children)
    {
        e.write(input);
    }

    tide::SeekHead ms;
    EXPECT_EQ(tide::vint::size(body_size) + body_size,
            ms.read(input));
    EXPECT_EQ(3, ms.count());
    tide::SeekHead::iterator res = ms.find(tide::ids::Tracks);
    EXPECT_TRUE(res != ms.end());
    EXPECT_EQ(res->first, tide::ids::Tracks);
    EXPECT_EQ(res->second, 0x76FB);
    res = ms.find(tide::ids::Cues);
    EXPECT_TRUE(res != ms.end());
    EXPECT_EQ(res->first, tide::ids::Cues);
    EXPECT_EQ(res->second, 0x7F);
    res = ms.find(tide::ids::Cluster);
    EXPECT_TRUE(res != ms.end());
    EXPECT_EQ(res->first, tide::ids::Cluster);
    EXPECT_EQ(res->second, 0x10203040);

    // No children at all
    input.str(std::string());
    tide::vint::write(0, input);
    EXPECT_EQ(tide::vint::size(0), ms.read(input));
    // SeekHead should be clearing its stored index before reading, so the
    // previous test's index should not affect this result
    EXPECT_TRUE(ms.empty());
    // Body size value wrong (too small)
    input.str(std::string());
    tide::vint::write(2, input);
    children[0].write(input);
    EXPECT_THROW(ms.read(input), tide::BadBodySize);
    // Invalid child
    input.str(std::string());
    tide::UIntElement ue(tide::ids::EBML, 0xFFFF);
    tide::vint::write(ue.size(), input);
    ue.write(input);
    EXPECT_THROW(ms.read(input), tide::InvalidChildID);
}

