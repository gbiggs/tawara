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
#include <celduin/cues.h>
#include <celduin/el_ids.h>
#include <celduin/exceptions.h>
#include <celduin/vint.h>

#include "test_utils.h"


///////////////////////////////////////////////////////////////////////////////
// CueTrackPosition tests
///////////////////////////////////////////////////////////////////////////////

TEST(CueTrackPosition, Create)
{
    celduin::CueTrackPosition p1;
    EXPECT_EQ(1, p1.track());
    EXPECT_EQ(0, p1.cluster_pos());
    EXPECT_EQ(1, p1.block_num());
    EXPECT_EQ(0, p1.codec_state());
    EXPECT_TRUE(p1.reference_times().empty());

    celduin::CueTrackPosition p2(4, 2);
    EXPECT_EQ(4, p2.track());
    EXPECT_EQ(2, p2.cluster_pos());
    EXPECT_EQ(1, p2.block_num());
    EXPECT_EQ(0, p2.codec_state());
    EXPECT_TRUE(p2.reference_times().empty());
}


TEST(CueTrackPosition, Assignment)
{
    celduin::CueTrackPosition p1(4, 2);
    celduin::CueTrackPosition p2;
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
    celduin::CueTrackPosition p;
    EXPECT_EQ(1, p.track());
    p.track(21);
    EXPECT_EQ(21, p.track());
}


TEST(CueTrackPosition, ClusterPos)
{
    celduin::CueTrackPosition p;
    EXPECT_EQ(0, p.cluster_pos());
    p.cluster_pos(21);
    EXPECT_EQ(21, p.cluster_pos());
}


TEST(CueTrackPosition, BlockNum)
{
    celduin::CueTrackPosition p;
    EXPECT_EQ(1, p.block_num());
    p.block_num(21);
    EXPECT_EQ(21, p.block_num());
}


TEST(CueTrackPosition, CodecState)
{
    celduin::CueTrackPosition p;
    EXPECT_EQ(0, p.codec_state());
    p.codec_state(21);
    EXPECT_EQ(21, p.codec_state());
}


TEST(CueTrackPosition, ReferenceTimes)
{
    celduin::CueTrackPosition p;
    EXPECT_TRUE(p.reference_times().empty());
    p.reference_times().push_back(42);
    EXPECT_FALSE(p.reference_times().empty());
    EXPECT_EQ(42, p.reference_times()[0]);
}


TEST(CueTrackPosition, Equality)
{
    celduin::CueTrackPosition p1(4, 2);
    celduin::CueTrackPosition p2;
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
    celduin::UIntElement track(celduin::ids::CueTrack, 1);
    celduin::UIntElement cluster_pos(celduin::ids::CueClusterPosition, 0);
    celduin::UIntElement block_num(celduin::ids::CueBlockNumber, 2);
    celduin::UIntElement codec_state(celduin::ids::CueCodecState, 2);
    celduin::UIntElement ref_time(celduin::ids::CueRefTime, 42);

    celduin::CueTrackPosition p;
    std::streamsize body_size(track.size() + cluster_pos.size());
    EXPECT_EQ(celduin::ids::size(celduin::ids::CueTrackPosition) +
            celduin::vint::size(body_size) + body_size, p.size());

    p.block_num(block_num);
    p.codec_state(codec_state);
    p.reference_times().push_back(42);
    body_size += block_num.size() + codec_state.size() +
        celduin::ids::size(celduin::ids::CueReference) +
        celduin::vint::size(ref_time.size()) + ref_time.size();
    EXPECT_EQ(celduin::ids::size(celduin::ids::CueTrackPosition) +
            celduin::vint::size(body_size) + body_size, p.size());
}


TEST(CueTrackPosition, Write)
{
    std::ostringstream output;
    std::stringstream expected;
    celduin::UIntElement track(celduin::ids::CueTrack, 1);
    celduin::UIntElement cluster_pos(celduin::ids::CueClusterPosition, 0);
    celduin::UIntElement block_num(celduin::ids::CueBlockNumber, 2);
    celduin::UIntElement codec_state(celduin::ids::CueCodecState, 2);
    celduin::UIntElement ref_time(celduin::ids::CueRefTime, 42);

    celduin::CueTrackPosition p;
    std::streamsize body_size(track.size() + cluster_pos.size());
    celduin::ids::write(celduin::ids::CueTrackPosition, expected);
    celduin::vint::write(body_size, expected);
    track.write(expected);
    cluster_pos.write(expected);
    EXPECT_EQ(celduin::ids::size(celduin::ids::CueTrackPosition) +
            celduin::vint::size(body_size) + body_size, p.write(output));
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
        codec_state.size() + celduin::ids::size(celduin::ids::CueReference) +
        celduin::vint::size(ref_time.size()) + ref_time.size();
    celduin::ids::write(celduin::ids::CueTrackPosition, expected);
    celduin::vint::write(body_size, expected);
    track.write(expected);
    cluster_pos.write(expected);
    block_num.write(expected);
    codec_state.write(expected);
    celduin::ids::write(celduin::ids::CueReference, expected);
    celduin::vint::write(ref_time.size(), expected);
    ref_time.write(expected);
    EXPECT_EQ(celduin::ids::size(celduin::ids::CueTrackPosition) +
            celduin::vint::size(body_size) + body_size, p.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            output.str());
}


TEST(CueTrackPosition, Read)
{
    std::stringstream input;
    celduin::UIntElement track(celduin::ids::CueTrack, 1);
    celduin::UIntElement cluster_pos(celduin::ids::CueClusterPosition, 0);
    celduin::UIntElement block_num(celduin::ids::CueBlockNumber, 2);
    celduin::UIntElement codec_state(celduin::ids::CueCodecState, 2);
    celduin::UIntElement ref_time(celduin::ids::CueRefTime, 42);

    celduin::CueTrackPosition p;
    std::streamsize body_size(track.size() + cluster_pos.size());
    celduin::vint::write(body_size, input);
    track.write(input);
    cluster_pos.write(input);
    EXPECT_EQ(celduin::vint::size(body_size) + body_size, p.read(input));
    EXPECT_EQ(1, p.track());
    EXPECT_EQ(0, p.cluster_pos());
    EXPECT_EQ(1, p.block_num());
    EXPECT_EQ(0, p.codec_state());
    EXPECT_TRUE(p.reference_times().empty());

    track = 5;
    cluster_pos = 10;

    input.str(std::string());
    body_size = track.size() + cluster_pos.size() + block_num.size() +
        codec_state.size() + celduin::ids::size(celduin::ids::CueReference) +
        celduin::vint::size(ref_time.size()) + ref_time.size();
    celduin::vint::write(body_size, input);
    track.write(input);
    cluster_pos.write(input);
    block_num.write(input);
    codec_state.write(input);
    celduin::ids::write(celduin::ids::CueReference, input);
    celduin::vint::write(ref_time.size(), input);
    ref_time.write(input);
    EXPECT_EQ(celduin::vint::size(body_size) + body_size, p.read(input));
    EXPECT_EQ(5, p.track());
    EXPECT_EQ(10, p.cluster_pos());
    EXPECT_EQ(2, p.block_num());
    EXPECT_EQ(2, p.codec_state());
    EXPECT_EQ(1, p.reference_times().size());
    EXPECT_EQ(42, p.reference_times()[0]);

    // Body size value wrong (too small)
    input.str(std::string());
    celduin::vint::write(2, input);
    track.write(input);
    cluster_pos.write(input);
    EXPECT_THROW(p.read(input), celduin::BadBodySize);
    // Invalid child
    input.str(std::string());
    celduin::UIntElement ue(celduin::ids::EBML, 0xFFFF);
    celduin::vint::write(ue.size(), input);
    ue.write(input);
    EXPECT_THROW(p.read(input), celduin::InvalidChildID);
    // No track
    input.str(std::string());
    celduin::vint::write(cluster_pos.size(), input);
    cluster_pos.write(input);
    EXPECT_THROW(p.read(input), celduin::MissingChild);
    // No cluster position
    input.str(std::string());
    celduin::vint::write(track.size(), input);
    track.write(input);
    EXPECT_THROW(p.read(input), celduin::MissingChild);
    // Track is zero
    input.str(std::string());
    track = 0;
    body_size = track.size() + cluster_pos.size();
    celduin::vint::write(body_size, input);
    track.write(input);
    cluster_pos.write(input);
    EXPECT_THROW(p.read(input), celduin::ValueOutOfRange);
    // Block number is zero
    input.str(std::string());
    track = 1;
    block_num = 0;
    body_size = track.size() + cluster_pos.size() + block_num.size();
    celduin::vint::write(body_size, input);
    track.write(input);
    cluster_pos.write(input);
    block_num.write(input);
    EXPECT_THROW(p.read(input), celduin::ValueOutOfRange);
}


///////////////////////////////////////////////////////////////////////////////
// CuePoint tests
///////////////////////////////////////////////////////////////////////////////

TEST(CuePoint, Create)
{
    celduin::CuePoint c;
    EXPECT_EQ(0, c.timecode());
    EXPECT_TRUE(c.empty());
}


TEST(CuePoint, Timecode)
{
    celduin::CuePoint c;
    EXPECT_EQ(0, c.timecode());
    c.timecode(42);
    EXPECT_EQ(42, c.timecode());
}


TEST(CuePoint, Assignment)
{
    celduin::CuePoint c1;
    celduin::CuePoint c2;
    celduin::CueTrackPosition p(2, 4);
    c1.push_back(p);

    EXPECT_TRUE(c2.empty());
    c2 = c1;
    EXPECT_FALSE(c2.empty());
    EXPECT_TRUE(c2[0] == p);
}


TEST(CuePoint, Empty)
{
    celduin::CuePoint c;
    EXPECT_TRUE(c.empty());
    celduin::CueTrackPosition p;
    c.push_back(p);
    EXPECT_FALSE(c.empty());
}


TEST(CuePoint, Count)
{
    celduin::CuePoint c;
    EXPECT_EQ(0, c.count());
    celduin::CueTrackPosition p1;
    celduin::CueTrackPosition p2;
    c.push_back(p1);
    c.push_back(p2);
    EXPECT_EQ(2, c.count());
}


TEST(CuePoint, Clear)
{
    celduin::CuePoint c;
    celduin::CueTrackPosition p;
    c.push_back(p);
    EXPECT_FALSE(c.empty());
    c.clear();
    EXPECT_TRUE(c.empty());
}


TEST(CuePoint, Erase)
{
    celduin::CuePoint c;
    celduin::CueTrackPosition p1(1, 2);
    celduin::CueTrackPosition p2(2, 4);
    c.push_back(p1);
    c.push_back(p2);
    EXPECT_EQ(2, c.count());
    EXPECT_TRUE(p1 == *c.begin());
    c.erase(c.begin());
    EXPECT_EQ(1, c.count());
    EXPECT_TRUE(p2 == *c.begin());

    celduin::CueTrackPosition p3(4, 8);
    celduin::CueTrackPosition p4(8, 16);
    c.push_back(p3);
    c.push_back(p4);
    EXPECT_EQ(3, c.count());
    c.erase(c.begin(), ++(++c.begin()));
    EXPECT_EQ(1, c.count());
    EXPECT_TRUE(p4 == *c.begin());
}


TEST(CuePoint, PushBack)
{
    celduin::CuePoint c;
    celduin::CueTrackPosition p1(1, 2);
    celduin::CueTrackPosition p2(2, 4);
    c.push_back(p1);
    EXPECT_TRUE(p1 == *c.begin());
    c.push_back(p2);
    EXPECT_TRUE(p2 == *(++c.begin()));
}


TEST(CuePoint, Iterators)
{
    celduin::CuePoint c;
    EXPECT_TRUE(c.begin() == c.end());

    celduin::CueTrackPosition p1(1, 2);
    celduin::CueTrackPosition p2(2, 4);
    c.push_back(p1);
    EXPECT_FALSE(c.begin() == c.end());
    EXPECT_TRUE(p1 == *c.begin());
    c.push_back(p2);
    EXPECT_TRUE(p2 == *(++c.begin()));
}


TEST(CuePoint, Equality)
{
    celduin::CuePoint c1;
    celduin::CuePoint c2;
    EXPECT_TRUE(c1 == c2);
    EXPECT_FALSE(c1 != c2);

    celduin::CueTrackPosition p(1, 2);
    c1.push_back(p);
    EXPECT_FALSE(c1 == c2);
    EXPECT_TRUE(c1 != c2);
}


TEST(CuePoint, Size)
{
    celduin::CuePoint c;
    celduin::UIntElement tc(celduin::ids::Timecode, 0);
    std::streamsize body_size(tc.size());
    EXPECT_EQ(celduin::ids::size(celduin::ids::CuePoint) +
            celduin::vint::size(body_size) + body_size, c.size());

    celduin::CueTrackPosition p;
    body_size += p.size();
    c.push_back(p);
    EXPECT_EQ(celduin::ids::size(celduin::ids::CuePoint) +
            celduin::vint::size(body_size) + body_size, c.size());
}


TEST(CuePoint, Write)
{
    std::ostringstream output;
    std::stringstream expected;
    celduin::UIntElement tc(celduin::ids::CueTime, 0);

    celduin::CuePoint c;
    EXPECT_THROW(c.write(output), celduin::EmptyCuePointElement);

    expected.str(std::string());
    output.str(std::string());
    celduin::CueTrackPosition p1(1, 2);
    celduin::CueTrackPosition p2(2, 4);
    c.push_back(p1);
    c.push_back(p2);
    std::streamsize body_size(tc.size() + p1.size() + p2.size());
    celduin::ids::write(celduin::ids::CuePoint, expected);
    celduin::vint::write(body_size, expected);
    tc.write(expected);
    p1.write(expected);
    p2.write(expected);
    EXPECT_EQ(celduin::ids::size(celduin::ids::CuePoint) +
            celduin::vint::size(body_size) + body_size, c.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());
}


TEST(CuePoint, Read)
{
    std::stringstream input;
    celduin::UIntElement tc(celduin::ids::CueTime, 42);
    celduin::CueTrackPosition p1(1, 2);
    celduin::CueTrackPosition p2(2, 4);

    celduin::CuePoint c;
    std::streamsize body_size(tc.size() + p1.size() + p2.size());
    celduin::vint::write(body_size, input);
    tc.write(input);
    p1.write(input);
    p2.write(input);
    EXPECT_EQ(celduin::vint::size(body_size) + body_size,
            c.read(input));
    EXPECT_EQ(42, c.timecode());
    EXPECT_EQ(2, c.count());
    EXPECT_TRUE(p1 == c[0]);
    EXPECT_TRUE(p2 == c[1]);

    // Body size value wrong (too small)
    input.str(std::string());
    celduin::vint::write(2, input);
    tc.write(input);
    EXPECT_THROW(c.read(input), celduin::BadBodySize);
    // Invalid child
    input.str(std::string());
    celduin::UIntElement ue(celduin::ids::EBML, 0xFFFF);
    celduin::vint::write(ue.size(), input);
    ue.write(input);
    EXPECT_THROW(c.read(input), celduin::InvalidChildID);
    // No timecode
    input.str(std::string());
    celduin::vint::write(p1.size(), input);
    p1.write(input);
    EXPECT_THROW(c.read(input), celduin::MissingChild);
    // No CueTrackPositions
    input.str(std::string());
    celduin::vint::write(tc.size(), input);
    tc.write(input);
    EXPECT_THROW(c.read(input), celduin::EmptyCuePointElement);
}


///////////////////////////////////////////////////////////////////////////////
// Cues tests
///////////////////////////////////////////////////////////////////////////////

TEST(Cues, Create)
{
    celduin::Cues c;
    EXPECT_TRUE(c.empty());
}


TEST(Cues, Assignment)
{
    celduin::Cues c1;
    celduin::Cues c2;
    celduin::CuePoint cp(42);
    c1.insert(cp);

    EXPECT_TRUE(c2.empty());
    c2 = c1;
    EXPECT_FALSE(c2.empty());
    EXPECT_TRUE(c2[42] == cp);
}


TEST(Cues, At)
{
    celduin::Cues c;
    celduin::CuePoint cp(42);
    c.insert(cp);
    EXPECT_TRUE(c[42] == c.at(42));
    EXPECT_TRUE(c.at(42) == cp);
    EXPECT_THROW(c.at(1), std::out_of_range);
}


TEST(Cues, SubscriptOperator)
{
    celduin::Cues c;
    celduin::CuePoint cp(42);
    c.insert(cp);
    EXPECT_TRUE(c[42] == c.at(42));
    EXPECT_TRUE(c[42] == cp);
    EXPECT_EQ(0, c[42].count());
    EXPECT_NO_THROW(c[2]);

    celduin::CuePoint cp2(42);
    celduin::CueTrackPosition ctp;
    cp2.push_back(ctp);
    c[42] = cp2;
    EXPECT_TRUE(c[42] == cp2);
    EXPECT_EQ(1, c[42].count());
}


TEST(Cues, BeginEnd)
{
    celduin::Cues c;
    celduin::CuePoint cp(42);

    EXPECT_TRUE(c.begin() == c.end());
    EXPECT_TRUE(c.rbegin() == c.rend());
    c.insert(cp);
    EXPECT_FALSE(c.begin() == c.end());
    EXPECT_FALSE(c.rbegin() == c.rend());
}


TEST(Cues, Counts)
{
    celduin::Cues c;
    celduin::CuePoint cp(42);

    EXPECT_TRUE(c.empty());
    c.insert(cp);
    EXPECT_FALSE(c.empty());
    EXPECT_EQ(1, c.count());
}


TEST(Cues, Clear)
{
    celduin::Cues c;
    celduin::CuePoint cp(42);
    c.insert(cp);
    EXPECT_FALSE(c.empty());
    c.clear();
    EXPECT_TRUE(c.empty());
}


TEST(Cues, Insert)
{
    celduin::Cues c1;
    celduin::CuePoint cp1(42);
    celduin::CuePoint cp2(84);

    // Single insert
    EXPECT_TRUE(c1.empty());
    std::pair<celduin::Cues::iterator, bool> res;
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
    celduin::Cues c2;
    EXPECT_TRUE(c2.empty());
    c2.insert(c1.begin(), c1.end());
    EXPECT_FALSE(c2.empty());
    EXPECT_EQ(2, c2.count());
    EXPECT_TRUE(c2[42] == cp1);
}


TEST(Cues, Erase)
{
    celduin::Cues c;
    celduin::CuePoint cp(42);
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
    celduin::Cues c1, c2;
    celduin::CuePoint cp(42);
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
    celduin::Cues c;
    celduin::CuePoint cp(42);
    c.insert(cp);

    EXPECT_TRUE(c.find(42) == c.begin());
    EXPECT_TRUE(c.find(1) == c.end());
}


TEST(Cues, Equality)
{
    celduin::Cues c1;
    celduin::Cues c2;
    EXPECT_TRUE(c1 == c2);
    EXPECT_FALSE(c1 != c2);

    celduin::CuePoint cp(42);
    c1.insert(cp);
    EXPECT_FALSE(c1 == c2);
    EXPECT_TRUE(c1 != c2);
}


TEST(Cues, Size)
{
    celduin::Cues c;
    EXPECT_EQ(celduin::ids::size(celduin::ids::Cues) +
            celduin::vint::size(0),
            c.size());

    celduin::CuePoint cp(42);
    c.insert(cp);
    EXPECT_EQ(celduin::ids::size(celduin::ids::Cues) + celduin::vint::size(cp.size()) +
            cp.size(), c.size());
}


TEST(Cues, Write)
{
    std::ostringstream output;
    std::stringstream expected;
    celduin::CueTrackPosition p1(1, 2);
    celduin::CueTrackPosition p2(2, 4);
    celduin::Cues c;

    // No cue points
    EXPECT_THROW(c.write(output), celduin::EmptyCuesElement);

    output.str(std::string());
    celduin::CuePoint cp1(42);
    cp1.push_back(p1);
    celduin::CuePoint cp2(84);
    cp2.push_back(p2);
    c.insert(cp1);
    c.insert(cp2);
    std::streamsize body_size(cp1.size() + cp2.size());
    celduin::ids::write(celduin::ids::Cues, expected);
    celduin::vint::write(body_size, expected);
    cp1.write(expected);
    cp2.write(expected);
    EXPECT_EQ(celduin::ids::size(celduin::ids::Cues) +
            celduin::vint::size(body_size) + body_size, c.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());
}


TEST(Cues, Read)
{
    std::stringstream input;
    celduin::CueTrackPosition p1(1, 2);
    celduin::CueTrackPosition p2(2, 4);
    celduin::CuePoint cp1(42);
    cp1.push_back(p1);
    celduin::CuePoint cp2(84);
    cp2.push_back(p2);
    celduin::Cues c;

    std::streamsize body_size(cp1.size() + cp2.size());
    celduin::vint::write(body_size, input);
    cp1.write(input);
    cp2.write(input);
    EXPECT_EQ(celduin::vint::size(body_size) + body_size,
            c.read(input));
    EXPECT_TRUE(c[42] == cp1);
    EXPECT_TRUE(c[84] == cp2);

    // No cue points
    celduin::vint::write(0, input);
    EXPECT_THROW(c.read(input), celduin::EmptyCuesElement);
    // Body size value wrong (too small)
    input.str(std::string());
    celduin::vint::write(2, input);
    cp1.write(input);
    EXPECT_THROW(c.read(input), celduin::BadBodySize);
    // Invalid child
    input.str(std::string());
    celduin::UIntElement ue(celduin::ids::EBML, 0xFFFF);
    celduin::vint::write(ue.size(), input);
    ue.write(input);
    EXPECT_THROW(c.read(input), celduin::InvalidChildID);
}

