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
#include <tawara/el_ids.h>
#include <tawara/exceptions.h>
#include <tawara/track_entry.h>
#include <tawara/tracks.h>
#include <tawara/vint.h>

#include "test_utils.h"


TEST(Tracks, Create)
{
    tawara::Tracks e;
    EXPECT_TRUE(e.empty());
    EXPECT_TRUE(e.begin() == e.end());
}


TEST(Tracks, Assignment)
{
    tawara::Tracks e1;
    tawara::Tracks e2;
    tawara::TrackEntry::Ptr entry(new tawara::TrackEntry(1, 2, "MDCC"));
    e1.insert(entry);

    EXPECT_TRUE(e2.empty());
    e2 = e1;
    EXPECT_FALSE(e2.empty());
    EXPECT_EQ(e2[1]->codec_id(), "MDCC");
}


TEST(Tracks, At)
{
    tawara::Tracks e;
    tawara::TrackEntry::Ptr entry(new tawara::TrackEntry(1, 2, "MDCC"));
    e.insert(entry);
    EXPECT_EQ(e[1], e.at(1));
    EXPECT_EQ(e.at(1)->codec_id(), "MDCC");
    EXPECT_THROW(e.at(2), std::out_of_range);
}


TEST(Tracks, SubscriptOperator)
{
    tawara::Tracks e;
    tawara::TrackEntry::Ptr entry(new tawara::TrackEntry(1, 2, "MDCC"));
    e.insert(entry);
    EXPECT_EQ(e[1], e.at(1));
    EXPECT_EQ(e[1]->codec_id(), "MDCC");
    EXPECT_THROW(e[2], std::out_of_range);

    tawara::TrackEntry::Ptr entry2(new tawara::TrackEntry(1, 2, "Codec"));
    e[1] = entry2;
    EXPECT_EQ(e[1], entry2);
    EXPECT_EQ(e[1]->codec_id(), "Codec");
}


TEST(Tracks, BeginEnd)
{
    tawara::Tracks e;
    tawara::TrackEntry::Ptr entry(new tawara::TrackEntry(1, 2, "MDCC"));

    EXPECT_TRUE(e.begin() == e.end());
    EXPECT_TRUE(e.rbegin() == e.rend());
    e.insert(entry);
    EXPECT_FALSE(e.begin() == e.end());
    EXPECT_FALSE(e.rbegin() == e.rend());
}


TEST(Tracks, Counts)
{
    tawara::Tracks e;
    tawara::TrackEntry::Ptr entry(new tawara::TrackEntry(1, 2, "MDCC"));

    EXPECT_TRUE(e.empty());
    e.insert(entry);
    EXPECT_FALSE(e.empty());
    EXPECT_EQ(1, e.count());
}


TEST(Tracks, Clear)
{
    tawara::Tracks e;
    tawara::TrackEntry::Ptr entry(new tawara::TrackEntry(1, 2, "MDCC"));
    e.insert(entry);
    EXPECT_FALSE(e.empty());
    e.clear();
    EXPECT_TRUE(e.empty());
}


TEST(Tracks, Insert)
{
    tawara::Tracks e1;
    tawara::TrackEntry::Ptr entry1(new tawara::TrackEntry(1, 2, "MDCC"));
    tawara::TrackEntry::Ptr entry2(new tawara::TrackEntry(42, 14, "Codec"));

    // Single insert
    EXPECT_TRUE(e1.empty());
    std::pair<tawara::Tracks::iterator, bool> res;
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
    tawara::TrackEntry::Ptr entry3(new tawara::TrackEntry(1, 3, "Codec"));
    EXPECT_THROW(e1.insert(entry3), tawara::DuplicateTrackNumber);
    // UID collision
    tawara::TrackEntry::Ptr entry4(new tawara::TrackEntry(6, 2, "Codec"));
    EXPECT_THROW(e1.insert(entry4), tawara::DuplicateUID);

    // Range insert
    tawara::Tracks e2;
    EXPECT_TRUE(e2.empty());
    e2.insert(e1.begin(), e1.end());
    EXPECT_FALSE(e2.empty());
    EXPECT_EQ(2, e2.count());
    EXPECT_EQ(e2[1]->codec_id(), "MDCC");
    // Key collision
    EXPECT_THROW(e2.insert(e1.begin(), e1.end()), tawara::DuplicateTrackNumber);
    // UID collision
    tawara::Tracks e3;
    tawara::TrackEntry::Ptr entry5(new tawara::TrackEntry(5, 14, "Codec"));
    e3.insert(entry5);
    EXPECT_THROW(e2.insert(e3.begin(), e3.end()), tawara::DuplicateUID);
}


TEST(Tracks, Erase)
{
    tawara::Tracks e;
    tawara::TrackEntry::Ptr entry(new tawara::TrackEntry(1, 2, "MDCC"));
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
    tawara::Tracks e1, e2;
    tawara::TrackEntry::Ptr entry(new tawara::TrackEntry(1, 2, "MDCC"));
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
    tawara::Tracks e;
    tawara::TrackEntry::Ptr entry(new tawara::TrackEntry(1, 2, "MDCC"));
    e.insert(entry);

    EXPECT_TRUE(e.find(1) == e.begin());
    EXPECT_TRUE(e.find(2) == e.end());
}


TEST(Tracks, Equality)
{
    tawara::Tracks e1;
    tawara::Tracks e2;
    EXPECT_TRUE(e1 == e2);
    EXPECT_FALSE(e1 != e2);

    tawara::TrackEntry::Ptr entry(new tawara::TrackEntry(1, 2, "MDCC"));
    e2.insert(entry);
    EXPECT_FALSE(e1 == e2);
    EXPECT_TRUE(e1 != e2);
}


TEST(Tracks, Size)
{
    tawara::Tracks e;
    EXPECT_EQ(tawara::ids::size(tawara::ids::Tracks) +
            tawara::vint::size(0),
            e.size());

    tawara::TrackEntry::Ptr entry1(new tawara::TrackEntry(1, 2, "MDCC"));
    e.insert(entry1);
    EXPECT_EQ(tawara::ids::size(tawara::ids::Tracks) +
            tawara::vint::size(entry1->size()) +
            entry1->size(),
            e.size());

    tawara::TrackEntry::Ptr entry2(new tawara::TrackEntry(2, 3, "MDCC"));
    e.insert(entry2);
    EXPECT_EQ(tawara::ids::size(tawara::ids::Tracks) +
            tawara::vint::size(entry1->size() +
            entry2->size()) + entry1->size() +
            entry2->size(), e.size());
}


TEST(Tracks, Write)
{
    std::ostringstream output;
    std::stringstream expected;
    tawara::Tracks e;

    // No track entries
    EXPECT_THROW(e.write(output), tawara::EmptyTracksElement);

    output.str(std::string());
    tawara::TrackEntry::Ptr entry1(new tawara::TrackEntry(1, 2, "MDCC"));
    e.insert(entry1);
    tawara::TrackEntry::Ptr entry2(new tawara::TrackEntry(2, 3, "MDCC"));
    e.insert(entry2);
    std::streamsize body_size(entry1->size() + entry2->size());
    tawara::ids::write(tawara::ids::Tracks, expected);
    tawara::vint::write(body_size, expected);
    entry1->write(expected);
    entry2->write(expected);
    EXPECT_EQ(tawara::ids::size(tawara::ids::Tracks) +
            tawara::vint::size(body_size) + entry1->size() +
            entry2->size(), e.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());

    // Key collision
    entry2->number(1);
    EXPECT_THROW(e.write(output), tawara::DuplicateTrackNumber);
    // UID collision
    entry2->number(2);
    entry2->uid(2);
    EXPECT_THROW(e.write(output), tawara::DuplicateUID);
}


TEST(Tracks, Read)
{
    std::stringstream input;
    tawara::Tracks e;
    tawara::TrackEntry entry1(1, 2, "Codec1");
    tawara::TrackEntry entry2(2, 3, "Codec2");

    input.str(std::string());
    std::streamsize body_size(entry1.size() + entry2.size());
    tawara::vint::write(body_size, input);
    entry1.write(input);
    entry2.write(input);
    EXPECT_EQ(tawara::vint::size(body_size) + body_size,
            e.read(input));
    EXPECT_EQ(e[1]->codec_id(), "Codec1");
    EXPECT_EQ(e[2]->codec_id(), "Codec2");

    // No track entries
    tawara::vint::write(0, input);
    EXPECT_THROW(e.read(input), tawara::EmptyTracksElement);
    // Body size value wrong (too small)
    input.str(std::string());
    tawara::vint::write(2, input);
    entry1.write(input);
    EXPECT_THROW(e.read(input), tawara::BadBodySize);
    // Invalid child
    input.str(std::string());
    tawara::UIntElement ue(tawara::ids::EBML, 0xFFFF);
    tawara::vint::write(ue.size(), input);
    ue.write(input);
    EXPECT_THROW(e.read(input), tawara::InvalidChildID);
    // Key collision
    input.str(std::string());
    tawara::vint::write(2 * entry1.size(), input);
    entry1.write(input);
    entry1.write(input);
    EXPECT_THROW(e.read(input), tawara::DuplicateTrackNumber);
    // UID collision
    entry2.uid(2);
    input.str(std::string());
    tawara::vint::write(entry1.size() + entry2.size(), input);
    entry1.write(input);
    entry2.write(input);
    EXPECT_THROW(e.read(input), tawara::DuplicateUID);
}

