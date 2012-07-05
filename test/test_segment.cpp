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
#include <celduin/el_ids.h>
#include <celduin/exceptions.h>
#include <celduin/memory_cluster.h>
#include <celduin/segment.h>
#include <celduin/segment_info.h>
#include <celduin/track_entry.h>
#include <celduin/tracks.h>
#include <celduin/vint.h>
#include <celduin/void_element.h>

#include "test_utils.h"


TEST(Segment, Create)
{
    EXPECT_NO_THROW(celduin::Segment s);
}


TEST(Segment, Offsetting)
{
    celduin::Segment s;
    // For a new segment, the offset is zero
    EXPECT_EQ(42 - celduin::ids::size(celduin::ids::Segment) - 8,
            s.to_segment_offset(42));
    EXPECT_EQ(42 + celduin::ids::size(celduin::ids::Segment) + 8,
            s.to_stream_offset(42));
}


TEST(Segment, Size)
{
    celduin::Segment s;
    // Segment size always 8 bytes
    EXPECT_EQ(celduin::ids::size(celduin::ids::Segment) + 8 +
            s.pad_size(), s.size());
}


TEST(Segment, Write)
{
    std::stringstream output;
    std::stringstream start_write;
    std::stringstream expected;
    celduin::Segment s;
    std::streamsize body_size(0), expected_size(0);

    celduin::SeekHead index;
    index.insert(std::make_pair(celduin::ids::Tracks, s.pad_size()));
    index.insert(std::make_pair(celduin::ids::Cluster, 5000));
    celduin::SegmentInfo info;
    info.date(12345);
    info.title("test segment");
    body_size = index.size() + info.size();

    s.index.insert(std::make_pair(celduin::ids::Tracks, s.pad_size()));
    s.index.insert(std::make_pair(celduin::ids::Cluster, 5000));
    s.info.date(12345);
    s.info.title("test segment");

    celduin::VoidElement ve(s.pad_size(), true);
    celduin::ids::write(celduin::ids::Segment, start_write);
    celduin::vint::write(s.pad_size(), start_write, 8);
    ve.write(start_write);
    EXPECT_EQ(celduin::ids::size(celduin::ids::Segment) + 8 + s.pad_size(),
            s.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, start_write.str(),
            output.str());

    output.str(std::string());
    expected_size = celduin::ids::write(celduin::ids::Segment, expected);
    expected_size += celduin::vint::write(s.pad_size(), expected, 8);
    expected_size += s.pad_size();
    ve.write(expected); // Need to have a void element to overwrite
    expected.seekp(celduin::ids::size(celduin::ids::Segment) + 8);
    index.write(expected);
    info.write(expected);
    // The output doesnt just start as blank this time
    s.write(output);
    EXPECT_EQ(celduin::ids::size(celduin::ids::Segment) + 8 + s.pad_size(),
            s.finalise(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            output.str());

    // Not writing
    EXPECT_THROW(s.finalise(output), celduin::NotWriting);

    // TODO: test the case where there is not enough padding for the SeekHead
    // or the SegmentInfo or both.
}


TEST(Segment, Read)
{
    std::stringstream input;

    // Reading with a complete index
    celduin::SeekHead index;
    index.insert(std::make_pair(celduin::ids::Tracks, 4500));
    index.insert(std::make_pair(celduin::ids::Cluster, 5000));
    celduin::SegmentInfo info;
    info.date(12345);
    info.title("test segment");
    celduin::VoidElement ve(2000, true);
    celduin::Tracks tracks;
    celduin::TrackEntry::Ptr track_entry(new celduin::TrackEntry(1, 2, "MDCC"));
    tracks.insert(track_entry);
    celduin::MemoryCluster cluster;
    std::streamsize body_size(index.size() + info.size() + ve.size() +
            tracks.size() + cluster.size());

    celduin::ids::write(celduin::ids::Segment, input);
    input.seekg(celduin::ids::size(celduin::ids::Segment));
    celduin::vint::write(body_size, input, 8);
    index.write(input);
    info.write(input);
    ve.write(input);
    tracks.write(input);
    cluster.write(input);
    celduin::Segment s;
    EXPECT_EQ(8 + index.size() + info.size(), s.read(input));
    EXPECT_TRUE(s.index.find(celduin::ids::Tracks) != s.index.end());
    EXPECT_EQ(4500, s.index.find(celduin::ids::Tracks)->second);
    EXPECT_TRUE(s.index.find(celduin::ids::Info) != s.index.end());
    EXPECT_EQ(index.size(), s.index.find(celduin::ids::Info)->second);

    // Reading without an index
    input.str(std::string());
    celduin::ids::write(celduin::ids::Segment, input);
    input.seekg(celduin::ids::size(celduin::ids::Segment));
    body_size -= index.size();
    celduin::vint::write(body_size, input, 8);
    info.write(input);
    ve.write(input);
    tracks.write(input);
    cluster.write(input);
    EXPECT_EQ(8 + info.size(), s.read(input));
    EXPECT_TRUE(s.index.find(celduin::ids::Tracks) != s.index.end());
    EXPECT_EQ(info.size() + ve.size(),
            s.index.find(celduin::ids::Tracks)->second);
    EXPECT_TRUE(s.index.find(celduin::ids::Info) != s.index.end());
    EXPECT_EQ(0, s.index.find(celduin::ids::Info)->second);
    EXPECT_TRUE(s.index.find(celduin::ids::Cluster) != s.index.end());
    EXPECT_EQ(info.size() + ve.size() + tracks.size(),
            s.index.find(celduin::ids::Cluster)->second);

    // No SegmentInfo
    input.str(std::string());
    celduin::ids::write(celduin::ids::Segment, input);
    input.seekg(celduin::ids::size(celduin::ids::Segment));
    celduin::vint::write(body_size - info.size(), input, 8);
    ve.write(input);
    tracks.write(input);
    cluster.write(input);
    EXPECT_THROW(s.read(input), celduin::NoSegmentInfo);
    // No Tracks
    input.str(std::string());
    celduin::ids::write(celduin::ids::Segment, input);
    input.seekg(celduin::ids::size(celduin::ids::Segment));
    celduin::vint::write(body_size - tracks.size(), input, 8);
    info.write(input);
    ve.write(input);
    cluster.write(input);
    EXPECT_THROW(s.read(input), celduin::NoTracks);
    // No clusters
    input.str(std::string());
    celduin::ids::write(celduin::ids::Segment, input);
    input.seekg(celduin::ids::size(celduin::ids::Segment));
    celduin::vint::write(body_size - cluster.size(), input, 8);
    info.write(input);
    ve.write(input);
    tracks.write(input);
    EXPECT_THROW(s.read(input), celduin::NoClusters);
    // Size too small
    input.str(std::string());
    celduin::ids::write(celduin::ids::Segment, input);
    input.seekg(celduin::ids::size(celduin::ids::Segment));
    celduin::vint::write(4, input, 8);
    EXPECT_THROW(s.read(input), celduin::BadBodySize);
}

