/* Tide
 *
 * Test the Segment object.
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

