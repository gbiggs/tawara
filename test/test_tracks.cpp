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
#include <jonen/el_ids.h>
#include <jonen/exceptions.h>
#include <jonen/track_entry.h>
#include <jonen/tracks.h>
#include <jonen/vint.h>

#include "test_utils.h"


TEST(Tracks, Create)
{
    jonen::Tracks e;
    EXPECT_TRUE(e.empty());
    EXPECT_TRUE(e.begin() == e.end());
}


TEST(Tracks, Assignment)
{
    jonen::Tracks e1;
    jonen::Tracks e2;
    jonen::TrackEntry::Ptr entry(new jonen::TrackEntry(1, 2, "MDCC"));
    e1.insert(entry);

    EXPECT_TRUE(e2.empty());
    e2 = e1;
    EXPECT_FALSE(e2.empty());
    EXPECT_EQ(e2[1]->codec_id(), "MDCC");
}


TEST(Tracks, At)
{
    jonen::Tracks e;
    jonen::TrackEntry::Ptr entry(new jonen::TrackEntry(1, 2, "MDCC"));
    e.insert(entry);
    EXPECT_EQ(e[1], e.at(1));
    EXPECT_EQ(e.at(1)->codec_id(), "MDCC");
    EXPECT_THROW(e.at(2), std::out_of_range);
}


TEST(Tracks, SubscriptOperator)
{
    jonen::Tracks e;
    jonen::TrackEntry::Ptr entry(new jonen::TrackEntry(1, 2, "MDCC"));
    e.insert(entry);
    EXPECT_EQ(e[1], e.at(1));
    EXPECT_EQ(e[1]->codec_id(), "MDCC");
    EXPECT_THROW(e[2], std::out_of_range);

    jonen::TrackEntry::Ptr entry2(new jonen::TrackEntry(1, 2, "Codec"));
    e[1] = entry2;
    EXPECT_EQ(e[1], entry2);
    EXPECT_EQ(e[1]->codec_id(), "Codec");
}


TEST(Tracks, BeginEnd)
{
    jonen::Tracks e;
    jonen::TrackEntry::Ptr entry(new jonen::TrackEntry(1, 2, "MDCC"));

    EXPECT_TRUE(e.begin() == e.end());
    EXPECT_TRUE(e.rbegin() == e.rend());
    e.insert(entry);
    EXPECT_FALSE(e.begin() == e.end());
    EXPECT_FALSE(e.rbegin() == e.rend());
}


TEST(Tracks, Counts)
{
    jonen::Tracks e;
    jonen::TrackEntry::Ptr entry(new jonen::TrackEntry(1, 2, "MDCC"));

    EXPECT_TRUE(e.empty());
    e.insert(entry);
    EXPECT_FALSE(e.empty());
    EXPECT_EQ(1, e.count());
}


TEST(Tracks, Clear)
{
    jonen::Tracks e;
    jonen::TrackEntry::Ptr entry(new jonen::TrackEntry(1, 2, "MDCC"));
    e.insert(entry);
    EXPECT_FALSE(e.empty());
    e.clear();
    EXPECT_TRUE(e.empty());
}


TEST(Tracks, Insert)
{
    jonen::Tracks e1;
    jonen::TrackEntry::Ptr entry1(new jonen::TrackEntry(1, 2, "MDCC"));
    jonen::TrackEntry::Ptr entry2(new jonen::TrackEntry(42, 14, "Codec"));

    // Single insert
    EXPECT_TRUE(e1.empty());
    std::pair<jonen::Tracks::iterator, bool> res;
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
    jonen::TrackEntry::Ptr entry3(new jonen::TrackEntry(1, 3, "Codec"));
    EXPECT_THROW(e1.insert(entry3), jonen::DuplicateTrackNumber);
    // UID collision
    jonen::TrackEntry::Ptr entry4(new jonen::TrackEntry(6, 2, "Codec"));
    EXPECT_THROW(e1.insert(entry4), jonen::DuplicateUID);

    // Range insert
    jonen::Tracks e2;
    EXPECT_TRUE(e2.empty());
    e2.insert(e1.begin(), e1.end());
    EXPECT_FALSE(e2.empty());
    EXPECT_EQ(2, e2.count());
    EXPECT_EQ(e2[1]->codec_id(), "MDCC");
    // Key collision
    EXPECT_THROW(e2.insert(e1.begin(), e1.end()), jonen::DuplicateTrackNumber);
    // UID collision
    jonen::Tracks e3;
    jonen::TrackEntry::Ptr entry5(new jonen::TrackEntry(5, 14, "Codec"));
    e3.insert(entry5);
    EXPECT_THROW(e2.insert(e3.begin(), e3.end()), jonen::DuplicateUID);
}


TEST(Tracks, Erase)
{
    jonen::Tracks e;
    jonen::TrackEntry::Ptr entry(new jonen::TrackEntry(1, 2, "MDCC"));
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
    jonen::Tracks e1, e2;
    jonen::TrackEntry::Ptr entry(new jonen::TrackEntry(1, 2, "MDCC"));
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
    jonen::Tracks e;
    jonen::TrackEntry::Ptr entry(new jonen::TrackEntry(1, 2, "MDCC"));
    e.insert(entry);

    EXPECT_TRUE(e.find(1) == e.begin());
    EXPECT_TRUE(e.find(2) == e.end());
}


TEST(Tracks, Equality)
{
    jonen::Tracks e1;
    jonen::Tracks e2;
    EXPECT_TRUE(e1 == e2);
    EXPECT_FALSE(e1 != e2);

    jonen::TrackEntry::Ptr entry(new jonen::TrackEntry(1, 2, "MDCC"));
    e2.insert(entry);
    EXPECT_FALSE(e1 == e2);
    EXPECT_TRUE(e1 != e2);
}


TEST(Tracks, Size)
{
    jonen::Tracks e;
    EXPECT_EQ(jonen::ids::size(jonen::ids::Tracks) +
            jonen::vint::size(0),
            e.size());

    jonen::TrackEntry::Ptr entry1(new jonen::TrackEntry(1, 2, "MDCC"));
    e.insert(entry1);
    EXPECT_EQ(jonen::ids::size(jonen::ids::Tracks) +
            jonen::vint::size(entry1->size()) +
            entry1->size(),
            e.size());

    jonen::TrackEntry::Ptr entry2(new jonen::TrackEntry(2, 3, "MDCC"));
    e.insert(entry2);
    EXPECT_EQ(jonen::ids::size(jonen::ids::Tracks) +
            jonen::vint::size(entry1->size() +
            entry2->size()) + entry1->size() +
            entry2->size(), e.size());
}


TEST(Tracks, Write)
{
    std::ostringstream output;
    std::stringstream expected;
    jonen::Tracks e;

    // No track entries
    EXPECT_THROW(e.write(output), jonen::EmptyTracksElement);

    output.str(std::string());
    jonen::TrackEntry::Ptr entry1(new jonen::TrackEntry(1, 2, "MDCC"));
    e.insert(entry1);
    jonen::TrackEntry::Ptr entry2(new jonen::TrackEntry(2, 3, "MDCC"));
    e.insert(entry2);
    std::streamsize body_size(entry1->size() + entry2->size());
    jonen::ids::write(jonen::ids::Tracks, expected);
    jonen::vint::write(body_size, expected);
    entry1->write(expected);
    entry2->write(expected);
    EXPECT_EQ(jonen::ids::size(jonen::ids::Tracks) +
            jonen::vint::size(body_size) + entry1->size() +
            entry2->size(), e.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());

    // Key collision
    entry2->number(1);
    EXPECT_THROW(e.write(output), jonen::DuplicateTrackNumber);
    // UID collision
    entry2->number(2);
    entry2->uid(2);
    EXPECT_THROW(e.write(output), jonen::DuplicateUID);
}


TEST(Tracks, Read)
{
    std::stringstream input;
    jonen::Tracks e;
    jonen::TrackEntry entry1(1, 2, "Codec1");
    jonen::TrackEntry entry2(2, 3, "Codec2");

    input.str(std::string());
    std::streamsize body_size(entry1.size() + entry2.size());
    jonen::vint::write(body_size, input);
    entry1.write(input);
    entry2.write(input);
    EXPECT_EQ(jonen::vint::size(body_size) + body_size,
            e.read(input));
    EXPECT_EQ(e[1]->codec_id(), "Codec1");
    EXPECT_EQ(e[2]->codec_id(), "Codec2");

    // No track entries
    jonen::vint::write(0, input);
    EXPECT_THROW(e.read(input), jonen::EmptyTracksElement);
    // Body size value wrong (too small)
    input.str(std::string());
    jonen::vint::write(2, input);
    entry1.write(input);
    EXPECT_THROW(e.read(input), jonen::BadBodySize);
    // Invalid child
    input.str(std::string());
    jonen::UIntElement ue(jonen::ids::EBML, 0xFFFF);
    jonen::vint::write(ue.size(), input);
    ue.write(input);
    EXPECT_THROW(e.read(input), jonen::InvalidChildID);
    // Key collision
    input.str(std::string());
    jonen::vint::write(2 * entry1.size(), input);
    entry1.write(input);
    entry1.write(input);
    EXPECT_THROW(e.read(input), jonen::DuplicateTrackNumber);
    // UID collision
    entry2.uid(2);
    input.str(std::string());
    jonen::vint::write(entry1.size() + entry2.size(), input);
    entry1.write(input);
    entry2.write(input);
    EXPECT_THROW(e.read(input), jonen::DuplicateUID);
}

