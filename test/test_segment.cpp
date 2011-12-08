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
#include <tide/tracks.h>
#include <tide/vint.h>
#include <tide/void_element.h>

#include "test_utils.h"


TEST(Segment, Create)
{
    EXPECT_NO_THROW(tide::Segment s);
}


TEST(Segment, OffsetIn)
{
    tide::Segment s;
    // For a new segment, the offset is zero
    EXPECT_EQ(s.offset_in(42), 42);
}


TEST(Segment, Size)
{
    tide::Segment s;
    // Segment size always 8 bytes
    EXPECT_EQ(tide::ids::size(tide::ids::Segment) + 8 +
            4096, s.size());
}


TEST(Segment, Write)
{
    std::stringstream output;
    std::stringstream expected;
    tide::Segment s;
    std::streamsize body_size(0), expected_size(0);

    tide::SeekHead index;
    index.insert(std::make_pair(tide::ids::Tracks, 4096));
    index.insert(std::make_pair(tide::ids::Cluster, 5000));
    tide::SegmentInfo info;
    info.date(12345);
    info.title("test segment");
    body_size = index.size() + info.size();

    s.index.insert(std::make_pair(tide::ids::Tracks, 4096));
    s.index.insert(std::make_pair(tide::ids::Cluster, 5000));
    s.info.date(12345);
    s.info.title("test segment");

    expected_size = tide::ids::write(tide::ids::Segment, expected);
    expected_size += tide::vint::write(4096, expected);
    expected_size += 4096;
    index.write(expected);
    info.write(expected);
    EXPECT_EQ(tide::ids::size(tide::ids::Segment) + 8 + 4096,
            s.write(output));
    EXPECT_EQ(tide::ids::size(tide::ids::Segment) + 8 + 4096,
            s.finalise(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            output.str());

    // TODO: test the case where there is not enough padding for the SeekHead
    // or the SegmentInfo or both.
}


TEST(Segment, Read)
{
    std::stringstream input;

    // Reading with a complete index
    tide::SeekHead index;
    index.insert(std::make_pair(tide::ids::Tracks, 4096));
    index.insert(std::make_pair(tide::ids::Cluster, 5000));
    tide::SegmentInfo info;
    info.date(12345);
    info.title("test segment");
    tide::VoidElement ve(2000, true);
    tide::Tracks tracks;
    tide::MemoryCluster cluster;
    std::streamsize body_size(index.size() + info.size() + ve.size() +
            tracks.size() + cluster.size());

    tide::vint::write(body_size, input);
    index.write(input);
    info.write(input);
    ve.write(input);
    tracks.write(input);
    cluster.write(input);
    tide::Segment s;
    EXPECT_EQ(tide::vint::size(body_size) + body_size,
            s.read(input));
    EXPECT_TRUE(s.index.find(tide::ids::Tracks) != s.index.end());
    EXPECT_EQ(s.index.find(tide::ids::Tracks)->second, 4096);
    EXPECT_TRUE(s.index.find(tide::ids::Info) != s.index.end());
    EXPECT_EQ(s.index.find(tide::ids::Info)->second, index.size());

    // Reading without an index
    input.str(std::string());
    body_size -= index.size();
    tide::vint::write(body_size, input);
    info.write(input);
    ve.write(input);
    tracks.write(input);
    cluster.write(input);
    EXPECT_EQ(tide::vint::size(body_size) + body_size,
            s.read(input));
    EXPECT_TRUE(s.index.find(tide::ids::Tracks) != s.index.end());
    EXPECT_EQ(s.index.find(tide::ids::Tracks)->second, info.size() + ve.size());
    EXPECT_TRUE(s.index.find(tide::ids::Info) != s.index.end());
    EXPECT_EQ(s.index.find(tide::ids::Info)->second, 0);
    EXPECT_TRUE(s.index.find(tide::ids::Cluster) != s.index.end());
    EXPECT_EQ(s.index.find(tide::ids::Cluster)->second,
            info.size() + ve.size() + tracks.size());

    // No SegmentInfo
    input.str(std::string());
    tide::vint::write(body_size, input);
    ve.write(input);
    tracks.write(input);
    cluster.write(input);
    EXPECT_THROW(s.read(input), tide::NoSegmentInfo);
    // No Tracks
    input.str(std::string());
    tide::vint::write(body_size, input);
    info.write(input);
    ve.write(input);
    cluster.write(input);
    EXPECT_THROW(s.read(input), tide::NoTracks);
    // No clusters
    input.str(std::string());
    tide::vint::write(body_size, input);
    info.write(input);
    ve.write(input);
    tracks.write(input);
    EXPECT_THROW(s.read(input), tide::NoClusters);
    // Size too small
    input.str(std::string());
    tide::vint::write(4, input);
    EXPECT_THROW(s.read(input), tide::BadBodySize);
}

