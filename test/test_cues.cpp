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
#include <tide/cues.h>
#include <tide/el_ids.h>
#include <tide/exceptions.h>
#include <tide/vint.h>

#include "test_utils.h"


///////////////////////////////////////////////////////////////////////////////
// CueTrackPosition tests
///////////////////////////////////////////////////////////////////////////////

TEST(CueTrackPosition, Create)
{
    tide::CueTrackPosition p1;
    EXPECT_EQ(1, p1.track());
    EXPECT_EQ(0, p1.cluster_pos());
    EXPECT_EQ(1, p1.block_num());
    EXPECT_EQ(0, p1.codec_state());
    EXPECT_TRUE(p1.reference_times().empty());

    tide::CueTrackPosition p2(4, 2);
    EXPECT_EQ(4, p2.track());
    EXPECT_EQ(2, p2.cluster_pos());
    EXPECT_EQ(1, p2.block_num());
    EXPECT_EQ(0, p2.codec_state());
    EXPECT_TRUE(p2.reference_times().empty());
}


TEST(CueTrackPosition, Assignment)
{
    tide::CueTrackPosition p1(4, 2);
    tide::CueTrackPosition p2;
    p1.block_num(8);
    p1.codec_state(16);
    p1.reference_times().push_back(42);

    EXPECT_TRUE(p2 != p1);
    p2 = p1;
    EXPECT_TRUE(p2 == p1);
    EXPECT_TRUE(42 == p2.reference_times()[0]);
}


TEST(CueTrackPosition, Track)
{
    tide::CueTrackPosition p;
    EXPECT_EQ(1, p.track());
    p.track(21);
    EXPECT_EQ(21, p.track());
}


TEST(CueTrackPosition, ClusterPos)
{
    tide::CueTrackPosition p;
    EXPECT_EQ(0, p.cluster_pos());
    p.cluster_pos(21);
    EXPECT_EQ(21, p.cluster_pos());
}


TEST(CueTrackPosition, BlockNum)
{
    tide::CueTrackPosition p;
    EXPECT_EQ(1, p.block_num());
    p.block_num(21);
    EXPECT_EQ(21, p.block_num());
}


TEST(CueTrackPosition, CodecState)
{
    tide::CueTrackPosition p;
    EXPECT_EQ(0, p.codec_state());
    p.codec_state(21);
    EXPECT_EQ(21, p.codec_state());
}


TEST(CueTrackPosition, ReferenceTimes)
{
    tide::CueTrackPosition p;
    EXPECT_TRUE(p.reference_times().empty());
    p.reference_times().push_back(42);
    EXPECT_FALSE(p.reference_times().empty());
    EXPECT_EQ(42, p.reference_times()[0]);
}


TEST(CueTrackPosition, Equality)
{
    tide::CueTrackPosition p1(4, 2);
    tide::CueTrackPosition p2;
    p1.block_num(8);
    p1.codec_state(16);

    EXPECT_TRUE(p2 != p1);
    p2.track(4);
    p2.cluster_pos(2);
    p2.block_num(8);
    p2.codec_state(16);
    EXPECT_TRUE(p2 == p1);
}


TEST(CueTrackPosition, Size)
{
    tide::UIntElement track(tide::ids::CueTrack, 1);
    tide::UIntElement cluster_pos(tide::ids::CueClusterPosition, 0);
    tide::UIntElement block_num(tide::ids::CueBlockNumber, 2);
    tide::UIntElement codec_state(tide::ids::CueCodecState, 2);
    tide::UIntElement ref_time(tide::ids::CueRefTime, 42);

    tide::CueTrackPosition p;
    std::streamsize body_size(track.size() + cluster_pos.size());
    EXPECT_EQ(tide::ids::size(tide::ids::CueTrackPosition) +
            tide::vint::size(body_size) + body_size, p.size());

    p.block_num(block_num);
    p.codec_state(codec_state);
    p.reference_times().push_back(42);
    body_size += block_num.size() + codec_state.size() +
        tide::ids::size(tide::ids::CueReference) +
        tide::vint::size(ref_time.size()) + ref_time.size();
    EXPECT_EQ(tide::ids::size(tide::ids::CueTrackPosition) +
            tide::vint::size(body_size) + body_size, p.size());
}


TEST(CueTrackPosition, Write)
{
    std::ostringstream output;
    std::stringstream expected;
    tide::UIntElement track(tide::ids::CueTrack, 1);
    tide::UIntElement cluster_pos(tide::ids::CueClusterPosition, 0);
    tide::UIntElement block_num(tide::ids::CueBlockNumber, 2);
    tide::UIntElement codec_state(tide::ids::CueCodecState, 2);
    tide::UIntElement ref_time(tide::ids::CueRefTime, 42);

    tide::CueTrackPosition p;
    std::streamsize body_size(track.size() + cluster_pos.size());
    tide::ids::write(tide::ids::CueTrackPosition, expected);
    tide::vint::write(body_size, expected);
    track.write(expected);
    cluster_pos.write(expected);
    EXPECT_EQ(tide::ids::size(tide::ids::CueTrackPosition) +
            tide::vint::size(body_size) + body_size, p.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            output.str());

    track = 5;
    cluster_pos = 10;
    p.track(track);
    p.cluster_pos(cluster_pos);
    p.block_num(block_num);
    p.codec_state(codec_state);
    p.reference_times().push_back(ref_time);

    expected.str(std::string());
    output.str(std::string());
    body_size = track.size() + cluster_pos.size() + block_num.size() +
        codec_state.size() + tide::ids::size(tide::ids::CueReference) +
        tide::vint::size(ref_time.size()) + ref_time.size();
    tide::ids::write(tide::ids::CueTrackPosition, expected);
    tide::vint::write(body_size, expected);
    track.write(expected);
    cluster_pos.write(expected);
    block_num.write(expected);
    codec_state.write(expected);
    tide::ids::write(tide::ids::CueReference, expected);
    tide::vint::write(ref_time.size(), expected);
    ref_time.write(expected);
    EXPECT_EQ(tide::ids::size(tide::ids::CueTrackPosition) +
            tide::vint::size(body_size) + body_size, p.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            output.str());
}


TEST(CueTrackPosition, Read)
{
    std::stringstream input;
    tide::UIntElement track(tide::ids::CueTrack, 1);
    tide::UIntElement cluster_pos(tide::ids::CueClusterPosition, 0);
    tide::UIntElement block_num(tide::ids::CueBlockNumber, 2);
    tide::UIntElement codec_state(tide::ids::CueCodecState, 2);
    tide::UIntElement ref_time(tide::ids::CueRefTime, 42);

    tide::CueTrackPosition p;
    std::streamsize body_size(track.size() + cluster_pos.size());
    tide::vint::write(body_size, input);
    track.write(input);
    cluster_pos.write(input);
    EXPECT_EQ(tide::vint::size(body_size) + body_size, p.read(input));
    EXPECT_EQ(1, p.track());
    EXPECT_EQ(0, p.cluster_pos());
    EXPECT_EQ(1, p.block_num());
    EXPECT_EQ(0, p.codec_state());
    EXPECT_TRUE(p.reference_times().empty());

    track = 5;
    cluster_pos = 10;

    input.str(std::string());
    body_size = track.size() + cluster_pos.size() + block_num.size() +
        codec_state.size() + tide::ids::size(tide::ids::CueReference) +
        tide::vint::size(ref_time.size()) + ref_time.size();
    tide::vint::write(body_size, input);
    track.write(input);
    cluster_pos.write(input);
    block_num.write(input);
    codec_state.write(input);
    tide::ids::write(tide::ids::CueReference, input);
    tide::vint::write(ref_time.size(), input);
    ref_time.write(input);
    EXPECT_EQ(tide::vint::size(body_size) + body_size, p.read(input));
    EXPECT_EQ(5, p.track());
    EXPECT_EQ(10, p.cluster_pos());
    EXPECT_EQ(2, p.block_num());
    EXPECT_EQ(2, p.codec_state());
    EXPECT_EQ(1, p.reference_times().size());
    EXPECT_EQ(42, p.reference_times()[0]);

    // Body size value wrong (too small)
    input.str(std::string());
    tide::vint::write(2, input);
    track.write(input);
    cluster_pos.write(input);
    EXPECT_THROW(p.read(input), tide::BadBodySize);
    // Invalid child
    input.str(std::string());
    tide::UIntElement ue(tide::ids::EBML, 0xFFFF);
    tide::vint::write(ue.size(), input);
    ue.write(input);
    EXPECT_THROW(p.read(input), tide::InvalidChildID);
    // No track
    input.str(std::string());
    tide::vint::write(cluster_pos.size(), input);
    cluster_pos.write(input);
    EXPECT_THROW(p.read(input), tide::MissingChild);
    // No cluster position
    input.str(std::string());
    tide::vint::write(track.size(), input);
    track.write(input);
    EXPECT_THROW(p.read(input), tide::MissingChild);
    // Track is zero
    input.str(std::string());
    track = 0;
    body_size = track.size() + cluster_pos.size();
    tide::vint::write(body_size, input);
    track.write(input);
    cluster_pos.write(input);
    EXPECT_THROW(p.read(input), tide::ValueOutOfRange);
    // Block number is zero
    input.str(std::string());
    track = 1;
    block_num = 0;
    body_size = track.size() + cluster_pos.size() + block_num.size();
    tide::vint::write(body_size, input);
    track.write(input);
    cluster_pos.write(input);
    block_num.write(input);
    EXPECT_THROW(p.read(input), tide::ValueOutOfRange);
}


///////////////////////////////////////////////////////////////////////////////
// CuePoint tests
///////////////////////////////////////////////////////////////////////////////

TEST(CuePoint, Create)
{
    tide::CuePoint c;
    EXPECT_EQ(0, c.timecode());
    EXPECT_TRUE(c.empty());
}


TEST(CuePoint, Timecode)
{
    tide::CuePoint c;
    EXPECT_EQ(0, c.timecode());
    c.timecode(42);
    EXPECT_EQ(42, c.timecode());
}


TEST(CuePoint, Assignment)
{
    tide::CuePoint c1;
    tide::CuePoint c2;
    tide::CueTrackPosition p(2, 4);
    c1.push_back(p);

    EXPECT_TRUE(c2.empty());
    c2 = c1;
    EXPECT_FALSE(c2.empty());
    EXPECT_TRUE(c2[0] == p);
}


TEST(CuePoint, Empty)
{
    tide::CuePoint c;
    EXPECT_TRUE(c.empty());
    tide::CueTrackPosition p;
    c.push_back(p);
    EXPECT_FALSE(c.empty());
}


TEST(CuePoint, Count)
{
    tide::CuePoint c;
    EXPECT_EQ(0, c.count());
    tide::CueTrackPosition p1;
    tide::CueTrackPosition p2;
    c.push_back(p1);
    c.push_back(p2);
    EXPECT_EQ(2, c.count());
}


TEST(CuePoint, Clear)
{
    tide::CuePoint c;
    tide::CueTrackPosition p;
    c.push_back(p);
    EXPECT_FALSE(c.empty());
    c.clear();
    EXPECT_TRUE(c.empty());
}


TEST(CuePoint, Erase)
{
    tide::CuePoint c;
    tide::CueTrackPosition p1(1, 2);
    tide::CueTrackPosition p2(2, 4);
    c.push_back(p1);
    c.push_back(p2);
    EXPECT_EQ(2, c.count());
    EXPECT_TRUE(p1 == *c.begin());
    c.erase(c.begin());
    EXPECT_EQ(1, c.count());
    EXPECT_TRUE(p2 == *c.begin());

    tide::CueTrackPosition p3(4, 8);
    tide::CueTrackPosition p4(8, 16);
    c.push_back(p3);
    c.push_back(p4);
    EXPECT_EQ(3, c.count());
    c.erase(c.begin(), ++(++c.begin()));
    EXPECT_EQ(1, c.count());
    EXPECT_TRUE(p4 == *c.begin());
}


TEST(CuePoint, PushBack)
{
    tide::CuePoint c;
    tide::CueTrackPosition p1(1, 2);
    tide::CueTrackPosition p2(2, 4);
    c.push_back(p1);
    EXPECT_TRUE(p1 == *c.begin());
    c.push_back(p2);
    EXPECT_TRUE(p2 == *(++c.begin()));
}


TEST(CuePoint, Iterators)
{
    tide::CuePoint c;
    EXPECT_TRUE(c.begin() == c.end());

    tide::CueTrackPosition p1(1, 2);
    tide::CueTrackPosition p2(2, 4);
    c.push_back(p1);
    EXPECT_FALSE(c.begin() == c.end());
    EXPECT_TRUE(p1 == *c.begin());
    c.push_back(p2);
    EXPECT_TRUE(p2 == *(++c.begin()));
}


TEST(CuePoint, Equality)
{
    tide::CuePoint c1;
    tide::CuePoint c2;
    EXPECT_TRUE(c1 == c2);
    EXPECT_FALSE(c1 != c2);

    tide::CueTrackPosition p(1, 2);
    c1.push_back(p);
    EXPECT_FALSE(c1 == c2);
    EXPECT_TRUE(c1 != c2);
}


TEST(CuePoint, Size)
{
    tide::CuePoint c;
    tide::UIntElement tc(tide::ids::Timecode, 0);
    std::streamsize body_size(tc.size());
    EXPECT_EQ(tide::ids::size(tide::ids::CuePoint) +
            tide::vint::size(body_size) + body_size, c.size());

    tide::CueTrackPosition p;
    body_size += p.size();
    c.push_back(p);
    EXPECT_EQ(tide::ids::size(tide::ids::CuePoint) +
            tide::vint::size(body_size) + body_size, c.size());
}


TEST(CuePoint, Write)
{
    std::ostringstream output;
    std::stringstream expected;
    tide::UIntElement tc(tide::ids::CueTime, 0);

    tide::CuePoint c;
    EXPECT_THROW(c.write(output), tide::EmptyCuePointElement);

    expected.str(std::string());
    output.str(std::string());
    tide::CueTrackPosition p1(1, 2);
    tide::CueTrackPosition p2(2, 4);
    c.push_back(p1);
    c.push_back(p2);
    std::streamsize body_size(tc.size() + p1.size() + p2.size());
    tide::ids::write(tide::ids::CuePoint, expected);
    tide::vint::write(body_size, expected);
    tc.write(expected);
    p1.write(expected);
    p2.write(expected);
    EXPECT_EQ(tide::ids::size(tide::ids::CuePoint) +
            tide::vint::size(body_size) + body_size, c.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());
}


TEST(CuePoint, Read)
{
    std::stringstream input;
    tide::UIntElement tc(tide::ids::CueTime, 42);
    tide::CueTrackPosition p1(1, 2);
    tide::CueTrackPosition p2(2, 4);

    tide::CuePoint c;
    std::streamsize body_size(tc.size() + p1.size() + p2.size());
    tide::vint::write(body_size, input);
    tc.write(input);
    p1.write(input);
    p2.write(input);
    EXPECT_EQ(tide::vint::size(body_size) + body_size,
            c.read(input));
    EXPECT_EQ(42, c.timecode());
    EXPECT_EQ(2, c.count());
    EXPECT_TRUE(p1 == c[0]);
    EXPECT_TRUE(p2 == c[1]);

    // Body size value wrong (too small)
    input.str(std::string());
    tide::vint::write(2, input);
    tc.write(input);
    EXPECT_THROW(c.read(input), tide::BadBodySize);
    // Invalid child
    input.str(std::string());
    tide::UIntElement ue(tide::ids::EBML, 0xFFFF);
    tide::vint::write(ue.size(), input);
    ue.write(input);
    EXPECT_THROW(c.read(input), tide::InvalidChildID);
    // No timecode
    input.str(std::string());
    tide::vint::write(p1.size(), input);
    p1.write(input);
    EXPECT_THROW(c.read(input), tide::MissingChild);
    // No CueTrackPositions
    input.str(std::string());
    tide::vint::write(tc.size(), input);
    tc.write(input);
    EXPECT_THROW(c.read(input), tide::EmptyCuePointElement);
}


///////////////////////////////////////////////////////////////////////////////
// Cues tests
///////////////////////////////////////////////////////////////////////////////

TEST(Cues, Create)
{
    tide::Cues c;
    EXPECT_TRUE(c.empty());
}


TEST(Cues, Assignment)
{
    tide::Cues c1;
    tide::Cues c2;
    tide::CuePoint cp(42);
    c1.insert(cp);

    EXPECT_TRUE(c2.empty());
    c2 = c1;
    EXPECT_FALSE(c2.empty());
    EXPECT_TRUE(c2[42] == cp);
}


TEST(Cues, At)
{
    tide::Cues c;
    tide::CuePoint cp(42);
    c.insert(cp);
    EXPECT_TRUE(c[42] == c.at(42));
    EXPECT_TRUE(c.at(42) == cp);
    EXPECT_THROW(c.at(1), std::out_of_range);
}


TEST(Cues, SubscriptOperator)
{
    tide::Cues c;
    tide::CuePoint cp(42);
    c.insert(cp);
    EXPECT_TRUE(c[42] == c.at(42));
    EXPECT_TRUE(c[42] == cp);
    EXPECT_EQ(0, c[42].count());
    EXPECT_NO_THROW(c[2]);

    tide::CuePoint cp2(42);
    tide::CueTrackPosition ctp;
    cp2.push_back(ctp);
    c[42] = cp2;
    EXPECT_TRUE(c[42] == cp2);
    EXPECT_EQ(1, c[42].count());
}


TEST(Cues, BeginEnd)
{
    tide::Cues c;
    tide::CuePoint cp(42);

    EXPECT_TRUE(c.begin() == c.end());
    EXPECT_TRUE(c.rbegin() == c.rend());
    c.insert(cp);
    EXPECT_FALSE(c.begin() == c.end());
    EXPECT_FALSE(c.rbegin() == c.rend());
}


TEST(Cues, Counts)
{
    tide::Cues c;
    tide::CuePoint cp(42);

    EXPECT_TRUE(c.empty());
    c.insert(cp);
    EXPECT_FALSE(c.empty());
    EXPECT_EQ(1, c.count());
}


TEST(Cues, Clear)
{
    tide::Cues c;
    tide::CuePoint cp(42);
    c.insert(cp);
    EXPECT_FALSE(c.empty());
    c.clear();
    EXPECT_TRUE(c.empty());
}


TEST(Cues, Insert)
{
    tide::Cues c1;
    tide::CuePoint cp1(42);
    tide::CuePoint cp2(84);

    // Single insert
    EXPECT_TRUE(c1.empty());
    std::pair<tide::Cues::iterator, bool> res;
    res = c1.insert(cp1);
    EXPECT_TRUE(res.first == c1.begin());
    EXPECT_TRUE(res.second);
    EXPECT_FALSE(c1.empty());
    EXPECT_EQ(1, c1.count());
    EXPECT_TRUE(c1[42] == cp1);
    res = c1.insert(cp2);
    EXPECT_TRUE(res.first != c1.begin());
    EXPECT_TRUE(res.second);
    EXPECT_FALSE(c1.empty());
    EXPECT_EQ(2, c1.count());
    EXPECT_TRUE(c1[84] == cp2);

    // Failed insert
    res = c1.insert(cp1);
    EXPECT_FALSE(res.second);
    EXPECT_EQ(2, c1.count());

    // Range insert
    tide::Cues c2;
    EXPECT_TRUE(c2.empty());
    c2.insert(c1.begin(), c1.end());
    EXPECT_FALSE(c2.empty());
    EXPECT_EQ(2, c2.count());
    EXPECT_TRUE(c2[42] == cp1);
}


TEST(Cues, Erase)
{
    tide::Cues c;
    tide::CuePoint cp(42);
    c.insert(cp);

    EXPECT_FALSE(c.empty());
    c.erase(c.begin());
    EXPECT_TRUE(c.empty());

    c.insert(cp);
    EXPECT_FALSE(c.empty());
    c.erase(c.begin(), c.end());
    EXPECT_TRUE(c.empty());

    c.insert(cp);
    EXPECT_FALSE(c.empty());
    c.erase(42);
    EXPECT_TRUE(c.empty());
    EXPECT_NO_THROW(c.erase(1));
}


TEST(Cues, Swap)
{
    tide::Cues c1, c2;
    tide::CuePoint cp(42);
    c1.insert(cp);

    EXPECT_FALSE(c1.empty());
    EXPECT_TRUE(c2.empty());
    c2.swap(c1);
    EXPECT_TRUE(c1.empty());
    EXPECT_FALSE(c2.empty());
    EXPECT_TRUE(c2[42] == cp);
}


TEST(Cues, Find)
{
    tide::Cues c;
    tide::CuePoint cp(42);
    c.insert(cp);

    EXPECT_TRUE(c.find(42) == c.begin());
    EXPECT_TRUE(c.find(1) == c.end());
}


TEST(Cues, Equality)
{
    tide::Cues c1;
    tide::Cues c2;
    EXPECT_TRUE(c1 == c2);
    EXPECT_FALSE(c1 != c2);

    tide::CuePoint cp(42);
    c1.insert(cp);
    EXPECT_FALSE(c1 == c2);
    EXPECT_TRUE(c1 != c2);
}


TEST(Cues, Size)
{
    tide::Cues c;
    EXPECT_EQ(tide::ids::size(tide::ids::Cues) +
            tide::vint::size(0),
            c.size());

    tide::CuePoint cp(42);
    c.insert(cp);
    EXPECT_EQ(tide::ids::size(tide::ids::Cues) + tide::vint::size(cp.size()) +
            cp.size(), c.size());
}


TEST(Cues, Write)
{
    std::ostringstream output;
    std::stringstream expected;
    tide::CueTrackPosition p1(1, 2);
    tide::CueTrackPosition p2(2, 4);
    tide::Cues c;

    // No cue points
    EXPECT_THROW(c.write(output), tide::EmptyCuesElement);

    output.str(std::string());
    tide::CuePoint cp1(42);
    cp1.push_back(p1);
    tide::CuePoint cp2(84);
    cp2.push_back(p2);
    c.insert(cp1);
    c.insert(cp2);
    std::streamsize body_size(cp1.size() + cp2.size());
    tide::ids::write(tide::ids::Cues, expected);
    tide::vint::write(body_size, expected);
    cp1.write(expected);
    cp2.write(expected);
    EXPECT_EQ(tide::ids::size(tide::ids::Cues) +
            tide::vint::size(body_size) + body_size, c.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());
}


TEST(Cues, Read)
{
    std::stringstream input;
    tide::CueTrackPosition p1(1, 2);
    tide::CueTrackPosition p2(2, 4);
    tide::CuePoint cp1(42);
    cp1.push_back(p1);
    tide::CuePoint cp2(84);
    cp2.push_back(p2);
    tide::Cues c;

    std::streamsize body_size(cp1.size() + cp2.size());
    tide::vint::write(body_size, input);
    cp1.write(input);
    cp2.write(input);
    EXPECT_EQ(tide::vint::size(body_size) + body_size,
            c.read(input));
    EXPECT_TRUE(c[42] == cp1);
    EXPECT_TRUE(c[84] == cp2);

    // No cue points
    tide::vint::write(0, input);
    EXPECT_THROW(c.read(input), tide::EmptyCuesElement);
    // Body size value wrong (too small)
    input.str(std::string());
    tide::vint::write(2, input);
    cp1.write(input);
    EXPECT_THROW(c.read(input), tide::BadBodySize);
    // Invalid child
    input.str(std::string());
    tide::UIntElement ue(tide::ids::EBML, 0xFFFF);
    tide::vint::write(ue.size(), input);
    ue.write(input);
    EXPECT_THROW(c.read(input), tide::InvalidChildID);
}

