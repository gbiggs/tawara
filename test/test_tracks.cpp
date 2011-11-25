/* Tide
 *
 * Test the Tracks element.
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
#include <tide/track_entry.h>
#include <tide/tracks.h>
#include <tide/vint.h>

#include "test_utils.h"


TEST(Tracks, Create)
{
    tide::Tracks e;
    EXPECT_TRUE(e.empty());
    EXPECT_TRUE(e.begin() == e.end());
}


TEST(Tracks, Assignment)
{
    tide::Tracks e1;
    tide::Tracks e2;
    tide::TrackEntry::Ptr entry(new tide::TrackEntry(1, 2, "MDCC"));
    e1.insert(entry);

    EXPECT_TRUE(e2.empty());
    e2 = e1;
    EXPECT_FALSE(e2.empty());
    EXPECT_EQ(e2[1]->codec_id(), "MDCC");
}


TEST(Tracks, At)
{
    tide::Tracks e;
    tide::TrackEntry::Ptr entry(new tide::TrackEntry(1, 2, "MDCC"));
    e.insert(entry);
    EXPECT_EQ(e[1], e.at(1));
    EXPECT_EQ(e.at(1)->codec_id(), "MDCC");
    EXPECT_THROW(e.at(2), std::out_of_range);
}


TEST(Tracks, SubscriptOperator)
{
    tide::Tracks e;
    tide::TrackEntry::Ptr entry(new tide::TrackEntry(1, 2, "MDCC"));
    e.insert(entry);
    EXPECT_EQ(e[1], e.at(1));
    EXPECT_EQ(e[1]->codec_id(), "MDCC");
    EXPECT_THROW(e[2], std::out_of_range);

    tide::TrackEntry::Ptr entry2(new tide::TrackEntry(1, 2, "Codec"));
    e[1] = entry2;
    EXPECT_EQ(e[1], entry2);
    EXPECT_EQ(e[1]->codec_id(), "Codec");
}


TEST(Tracks, BeginEnd)
{
    tide::Tracks e;
    tide::TrackEntry::Ptr entry(new tide::TrackEntry(1, 2, "MDCC"));

    EXPECT_TRUE(e.begin() == e.end());
    EXPECT_TRUE(e.rbegin() == e.rend());
    e.insert(entry);
    EXPECT_FALSE(e.begin() == e.end());
    EXPECT_FALSE(e.rbegin() == e.rend());
}


TEST(Tracks, Counts)
{
    tide::Tracks e;
    tide::TrackEntry::Ptr entry(new tide::TrackEntry(1, 2, "MDCC"));

    EXPECT_TRUE(e.empty());
    e.insert(entry);
    EXPECT_FALSE(e.empty());
    EXPECT_EQ(1, e.count());
}


TEST(Tracks, Clear)
{
    tide::Tracks e;
    tide::TrackEntry::Ptr entry(new tide::TrackEntry(1, 2, "MDCC"));
    e.insert(entry);
    EXPECT_FALSE(e.empty());
    e.clear();
    EXPECT_TRUE(e.empty());
}


TEST(Tracks, Insert)
{
    tide::Tracks e1;
    tide::TrackEntry::Ptr entry1(new tide::TrackEntry(1, 2, "MDCC"));
    tide::TrackEntry::Ptr entry2(new tide::TrackEntry(42, 14, "Codec"));

    // Single insert
    EXPECT_TRUE(e1.empty());
    std::pair<tide::Tracks::iterator, bool> res;
    res = e1.insert(entry1);
    EXPECT_TRUE(res.first == e1.begin());
    EXPECT_TRUE(res.second);
    EXPECT_FALSE(e1.empty());
    EXPECT_EQ(1, e1.count());
    EXPECT_EQ(e1[1]->codec_id(), "MDCC");
    res = e1.insert(entry2);
    EXPECT_TRUE(res.first != e1.begin());
    EXPECT_TRUE(res.second);
    EXPECT_FALSE(e1.empty());
    EXPECT_EQ(2, e1.count());
    EXPECT_EQ(e1[42]->number(), 42);
    EXPECT_EQ(e1[42]->codec_id(), "Codec");
    // Key collision
    tide::TrackEntry::Ptr entry3(new tide::TrackEntry(1, 3, "Codec"));
    EXPECT_THROW(e1.insert(entry3), tide::DuplicateTrackNumber);
    // UID collision
    tide::TrackEntry::Ptr entry4(new tide::TrackEntry(6, 2, "Codec"));
    EXPECT_THROW(e1.insert(entry4), tide::DuplicateUID);

    // Range insert
    tide::Tracks e2;
    EXPECT_TRUE(e2.empty());
    e2.insert(e1.begin(), e1.end());
    EXPECT_FALSE(e2.empty());
    EXPECT_EQ(2, e2.count());
    EXPECT_EQ(e2[1]->codec_id(), "MDCC");
    // Key collision
    EXPECT_THROW(e2.insert(e1.begin(), e1.end()), tide::DuplicateTrackNumber);
    // UID collision
    tide::Tracks e3;
    tide::TrackEntry::Ptr entry5(new tide::TrackEntry(5, 14, "Codec"));
    e3.insert(entry5);
    EXPECT_THROW(e2.insert(e3.begin(), e3.end()), tide::DuplicateUID);
}


TEST(Tracks, Erase)
{
    tide::Tracks e;
    tide::TrackEntry::Ptr entry(new tide::TrackEntry(1, 2, "MDCC"));
    e.insert(entry);

    EXPECT_FALSE(e.empty());
    e.erase(e.begin());
    EXPECT_TRUE(e.empty());

    e.insert(entry);
    EXPECT_FALSE(e.empty());
    e.erase(e.begin(), e.end());
    EXPECT_TRUE(e.empty());

    e.insert(entry);
    EXPECT_FALSE(e.empty());
    e.erase(1);
    EXPECT_TRUE(e.empty());
    EXPECT_NO_THROW(e.erase(2));
}


TEST(Tracks, Swap)
{
    tide::Tracks e1, e2;
    tide::TrackEntry::Ptr entry(new tide::TrackEntry(1, 2, "MDCC"));
    e1.insert(entry);

    EXPECT_FALSE(e1.empty());
    EXPECT_TRUE(e2.empty());
    e2.swap(e1);
    EXPECT_TRUE(e1.empty());
    EXPECT_FALSE(e2.empty());
    EXPECT_EQ(e2[1]->codec_id(), "MDCC");
}


TEST(Tracks, Find)
{
    tide::Tracks e;
    tide::TrackEntry::Ptr entry(new tide::TrackEntry(1, 2, "MDCC"));
    e.insert(entry);

    EXPECT_TRUE(e.find(1) == e.begin());
    EXPECT_TRUE(e.find(2) == e.end());
}


TEST(Tracks, Equality)
{
    tide::Tracks e1;
    tide::Tracks e2;
    EXPECT_TRUE(e1 == e2);
    EXPECT_FALSE(e1 != e2);

    tide::TrackEntry::Ptr entry(new tide::TrackEntry(1, 2, "MDCC"));
    e2.insert(entry);
    EXPECT_FALSE(e1 == e2);
    EXPECT_TRUE(e1 != e2);
}


TEST(Tracks, Size)
{
    tide::Tracks e;
    EXPECT_EQ(0, e.size());
    EXPECT_EQ(tide::ids::coded_size(tide::ids::Tracks) +
            tide::vint::coded_size(0),
            e.total_size());

    tide::TrackEntry::Ptr entry1(new tide::TrackEntry(1, 2, "MDCC"));
    e.insert(entry1);
    EXPECT_EQ(entry1->total_size(), e.size());
    EXPECT_EQ(tide::ids::coded_size(tide::ids::Tracks) +
            tide::vint::coded_size(entry1->total_size()) +
            entry1->total_size(),
            e.total_size());

    tide::TrackEntry::Ptr entry2(new tide::TrackEntry(2, 3, "MDCC"));
    e.insert(entry2);
    EXPECT_EQ(entry1->total_size() + entry2->total_size(), e.size());
    EXPECT_EQ(tide::ids::coded_size(tide::ids::Tracks) +
            tide::vint::coded_size(entry1->total_size() +
            entry2->total_size()) + entry1->total_size() +
            entry2->total_size(), e.total_size());
}


TEST(Tracks, Write)
{
    std::ostringstream output;
    std::stringstream expected;
    tide::Tracks e;

    // No track entries
    EXPECT_THROW(e.write_body(output), tide::EmptyTracksElement);
    EXPECT_THROW(e.write(output), tide::EmptyTracksElement);

    output.str(std::string());
    tide::TrackEntry::Ptr entry1(new tide::TrackEntry(1, 2, "MDCC"));
    e.insert(entry1);
    tide::TrackEntry::Ptr entry2(new tide::TrackEntry(2, 3, "MDCC"));
    e.insert(entry2);
    std::streamsize body_size(entry1->total_size() + entry2->total_size());
    entry1->write(expected);
    entry2->write(expected);
    EXPECT_EQ(body_size, e.write_body(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());
    output.str(std::string());
    expected.str(std::string());
    tide::ids::write(tide::ids::Tracks, expected);
    tide::vint::write(body_size, expected);
    entry1->write(expected);
    entry2->write(expected);
    EXPECT_EQ(tide::ids::coded_size(tide::ids::Tracks) +
            tide::vint::coded_size(body_size) + entry1->total_size() +
            entry2->total_size(), e.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());

    // Key collision
    entry2->number(1);
    EXPECT_THROW(e.write_body(output), tide::DuplicateTrackNumber);
    // UID collision
    entry2->number(2);
    entry2->uid(2);
    EXPECT_THROW(e.write_body(output), tide::DuplicateUID);
}


TEST(Tracks, Read)
{
    std::stringstream input;
    tide::Tracks e;
    tide::TrackEntry entry1(1, 2, "Codec1");
    tide::TrackEntry entry2(2, 3, "Codec2");

    input.str(std::string());
    std::streamsize body_size(entry1.total_size() + entry2.total_size());
    tide::vint::write(body_size, input);
    entry1.write(input);
    entry2.write(input);
    EXPECT_EQ(tide::vint::coded_size(body_size) + body_size,
            e.read_body(input));
    EXPECT_EQ(e[1]->codec_id(), "Codec1");
    EXPECT_EQ(e[2]->codec_id(), "Codec2");

    // No track entries
    tide::vint::write(0, input);
    EXPECT_THROW(e.read_body(input), tide::EmptyTracksElement);
    // Body size value wrong (too small)
    input.str(std::string());
    tide::vint::write(2, input);
    entry1.write(input);
    EXPECT_THROW(e.read_body(input), tide::BadBodySize);
    // Invalid child
    input.str(std::string());
    tide::UIntElement ue(tide::ids::EBML, 0xFFFF);
    tide::vint::write(ue.total_size(), input);
    ue.write(input);
    EXPECT_THROW(e.read_body(input), tide::InvalidChildID);
    // Key collision
    input.str(std::string());
    tide::vint::write(2 * entry1.total_size(), input);
    entry1.write(input);
    entry1.write(input);
    EXPECT_THROW(e.read_body(input), tide::DuplicateTrackNumber);
    // UID collision
    entry2.uid(2);
    input.str(std::string());
    tide::vint::write(entry1.total_size() + entry2.total_size(), input);
    entry1.write(input);
    entry2.write(input);
    EXPECT_THROW(e.read_body(input), tide::DuplicateUID);
}

