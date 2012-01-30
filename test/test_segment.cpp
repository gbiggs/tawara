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
#include <tide/el_ids.h>
#include <tide/exceptions.h>
#include <tide/memory_cluster.h>
#include <tide/segment.h>
#include <tide/segment_info.h>
#include <tide/track_entry.h>
#include <tide/tracks.h>
#include <tide/vint.h>
#include <tide/void_element.h>

#include "test_utils.h"


TEST(Segment, Create)
{
    EXPECT_NO_THROW(tide::Segment s);
}


TEST(Segment, Offsetting)
{
    tide::Segment s;
    // For a new segment, the offset is zero
    EXPECT_EQ(42 - tide::ids::size(tide::ids::Segment) - 8,
            s.to_segment_offset(42));
    EXPECT_EQ(42 + tide::ids::size(tide::ids::Segment) + 8,
            s.to_stream_offset(42));
}


TEST(Segment, Size)
{
    tide::Segment s;
    // Segment size always 8 bytes
    EXPECT_EQ(tide::ids::size(tide::ids::Segment) + 8 +
            s.pad_size(), s.size());
}


TEST(Segment, Write)
{
    std::stringstream output;
    std::stringstream start_write;
    std::stringstream expected;
    tide::Segment s;
    std::streamsize body_size(0), expected_size(0);

    tide::SeekHead index;
    index.insert(std::make_pair(tide::ids::Tracks, s.pad_size()));
    index.insert(std::make_pair(tide::ids::Cluster, 5000));
    tide::SegmentInfo info;
    info.date(12345);
    info.title("test segment");
    body_size = index.size() + info.size();

    s.index.insert(std::make_pair(tide::ids::Tracks, s.pad_size()));
    s.index.insert(std::make_pair(tide::ids::Cluster, 5000));
    s.info.date(12345);
    s.info.title("test segment");

    tide::VoidElement ve(s.pad_size(), true);
    tide::ids::write(tide::ids::Segment, start_write);
    tide::vint::write(s.pad_size(), start_write, 8);
    ve.write(start_write);
    EXPECT_EQ(tide::ids::size(tide::ids::Segment) + 8 + s.pad_size(),
            s.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, start_write.str(),
            output.str());

    output.str(std::string());
    expected_size = tide::ids::write(tide::ids::Segment, expected);
    expected_size += tide::vint::write(s.pad_size(), expected, 8);
    expected_size += s.pad_size();
    ve.write(expected); // Need to have a void element to overwrite
    expected.seekp(tide::ids::size(tide::ids::Segment) + 8);
    index.write(expected);
    info.write(expected);
    // The output doesnt just start as blank this time
    s.write(output);
    EXPECT_EQ(tide::ids::size(tide::ids::Segment) + 8 + s.pad_size(),
            s.finalise(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            output.str());

    // Not writing
    EXPECT_THROW(s.finalise(output), tide::NotWriting);

    // TODO: test the case where there is not enough padding for the SeekHead
    // or the SegmentInfo or both.
}


TEST(Segment, Read)
{
    std::stringstream input;

    // Reading with a complete index
    tide::SeekHead index;
    index.insert(std::make_pair(tide::ids::Tracks, 4500));
    index.insert(std::make_pair(tide::ids::Cluster, 5000));
    tide::SegmentInfo info;
    info.date(12345);
    info.title("test segment");
    tide::VoidElement ve(2000, true);
    tide::Tracks tracks;
    tide::TrackEntry::Ptr track_entry(new tide::TrackEntry(1, 2, "MDCC"));
    tracks.insert(track_entry);
    tide::MemoryCluster cluster;
    std::streamsize body_size(index.size() + info.size() + ve.size() +
            tracks.size() + cluster.size());

    tide::ids::write(tide::ids::Segment, input);
    input.seekg(tide::ids::size(tide::ids::Segment));
    tide::vint::write(body_size, input, 8);
    index.write(input);
    info.write(input);
    ve.write(input);
    tracks.write(input);
    cluster.write(input);
    tide::Segment s;
    EXPECT_EQ(8 + index.size() + info.size(), s.read(input));
    EXPECT_TRUE(s.index.find(tide::ids::Tracks) != s.index.end());
    EXPECT_EQ(4500, s.index.find(tide::ids::Tracks)->second);
    EXPECT_TRUE(s.index.find(tide::ids::Info) != s.index.end());
    EXPECT_EQ(index.size(), s.index.find(tide::ids::Info)->second);

    // Reading without an index
    input.str(std::string());
    tide::ids::write(tide::ids::Segment, input);
    input.seekg(tide::ids::size(tide::ids::Segment));
    body_size -= index.size();
    tide::vint::write(body_size, input, 8);
    info.write(input);
    ve.write(input);
    tracks.write(input);
    cluster.write(input);
    EXPECT_EQ(8 + info.size(), s.read(input));
    EXPECT_TRUE(s.index.find(tide::ids::Tracks) != s.index.end());
    EXPECT_EQ(info.size() + ve.size(),
            s.index.find(tide::ids::Tracks)->second);
    EXPECT_TRUE(s.index.find(tide::ids::Info) != s.index.end());
    EXPECT_EQ(0, s.index.find(tide::ids::Info)->second);
    EXPECT_TRUE(s.index.find(tide::ids::Cluster) != s.index.end());
    EXPECT_EQ(info.size() + ve.size() + tracks.size(),
            s.index.find(tide::ids::Cluster)->second);

    // No SegmentInfo
    input.str(std::string());
    tide::ids::write(tide::ids::Segment, input);
    input.seekg(tide::ids::size(tide::ids::Segment));
    tide::vint::write(body_size - info.size(), input, 8);
    ve.write(input);
    tracks.write(input);
    cluster.write(input);
    EXPECT_THROW(s.read(input), tide::NoSegmentInfo);
    // No Tracks
    input.str(std::string());
    tide::ids::write(tide::ids::Segment, input);
    input.seekg(tide::ids::size(tide::ids::Segment));
    tide::vint::write(body_size - tracks.size(), input, 8);
    info.write(input);
    ve.write(input);
    cluster.write(input);
    EXPECT_THROW(s.read(input), tide::NoTracks);
    // No clusters
    input.str(std::string());
    tide::ids::write(tide::ids::Segment, input);
    input.seekg(tide::ids::size(tide::ids::Segment));
    tide::vint::write(body_size - cluster.size(), input, 8);
    info.write(input);
    ve.write(input);
    tracks.write(input);
    EXPECT_THROW(s.read(input), tide::NoClusters);
    // Size too small
    input.str(std::string());
    tide::ids::write(tide::ids::Segment, input);
    input.seekg(tide::ids::size(tide::ids::Segment));
    tide::vint::write(4, input, 8);
    EXPECT_THROW(s.read(input), tide::BadBodySize);
}

