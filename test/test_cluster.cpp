/* Tide
 *
 * Test the base Cluster class.
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
#include <tide/cluster.h>
#include <tide/el_ids.h>
#include <tide/exceptions.h>
#include <tide/vint.h>

#include "test_utils.h"


// Fake Cluster implementation
class FakeCluster : public tide::Cluster
{
    public:
        FakeCluster(uint64_t tc=0)
            : tide::Cluster(tc)
        {
        }

        std::streamsize blocks_size() const
        {
            return 0;
        }

        std::streamsize write_blocks(std::ostream& output)
        {
            return 0;
        }

        std::streamsize read_blocks(std::istream& input)
        {
            return 0;
        }
}; // class FakeCluster


TEST(BaseCluster, Create)
{
    FakeCluster e;
    EXPECT_TRUE(e.silent_tracks().empty());
    //EXPECT_EQ(0, e.position());
    EXPECT_EQ(0, e.previous_size());
}


TEST(BaseCluster, Timecode)
{
    FakeCluster e(42);
    EXPECT_EQ(42, e.timecode());
    e.timecode(84);
    EXPECT_EQ(84, e.timecode());
}


TEST(BaseCluster, SilentTracks)
{
    FakeCluster e;
    EXPECT_TRUE(e.silent_tracks().empty());
    e.silent_tracks().push_back(tide::SilentTrackNumber(15));
    EXPECT_FALSE(e.silent_tracks().empty());
    EXPECT_EQ(e.silent_tracks()[0], 15);
}


TEST(BaseCluster, Position)
{
    FakeCluster e;
    EXPECT_THROW(e.position(), tide::NotImplemented);
}


TEST(BaseCluster, PreviousSize)
{
    FakeCluster e;
    EXPECT_EQ(0, e.previous_size());
    e.previous_size(0x1234);
    EXPECT_EQ(0x1234, e.previous_size());
}


TEST(BaseCluster, Size)
{
    FakeCluster e;
    tide::UIntElement tc(tide::ids::Timecode, 0);
    std::streamsize body_size(tc.total_size());
    EXPECT_EQ(body_size, e.size());
    EXPECT_EQ(tide::ids::coded_size(tide::ids::Cluster) +
            tide::vint::coded_size(body_size) + body_size,
            e.total_size());

    tide::UIntElement st1(tide::ids::SilentTrackNumber, 1);
    tide::UIntElement st2(tide::ids::SilentTrackNumber, 2);
    body_size += tide::ids::coded_size(tide::ids::SilentTracks) +
        tide::vint::coded_size(st1.total_size() + st2.total_size()) +
        st1.total_size() + st2.total_size();
    e.silent_tracks().push_back(tide::SilentTrackNumber(1));
    e.silent_tracks().push_back(tide::SilentTrackNumber(2));
    EXPECT_EQ(body_size, e.size());
    EXPECT_EQ(tide::ids::coded_size(tide::ids::Cluster) +
            tide::vint::coded_size(body_size) + body_size,
            e.total_size());

    tide::UIntElement ps(tide::ids::PrevSize, 0x1234);
    body_size += ps.total_size();
    e.previous_size(0x1234);
    EXPECT_EQ(body_size, e.size());
    EXPECT_EQ(tide::ids::coded_size(tide::ids::Cluster) +
            tide::vint::coded_size(body_size) + body_size,
            e.total_size());
}


TEST(BaseCluster, Write)
{
    std::ostringstream output;
    std::stringstream expected;
    tide::UIntElement tc(tide::ids::Timecode, 0);
    tide::UIntElement st1(tide::ids::SilentTrackNumber, 1);
    tide::UIntElement st2(tide::ids::SilentTrackNumber, 2);
    tide::UIntElement ps(tide::ids::PrevSize, 0x1234);

    FakeCluster e;
    std::streamsize expected_size(tc.total_size());
    tc.write(expected);
    EXPECT_EQ(expected_size, e.write_body(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());
    output.str(std::string());
    expected.str(std::string());
    tide::ids::write(tide::ids::Cluster, expected);
    tide::vint::write(expected_size, expected);
    tc.write(expected);
    EXPECT_EQ(tide::ids::coded_size(tide::ids::Cluster) +
            tide::vint::coded_size(expected_size) + expected_size,
            e.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());

    expected_size += tide::ids::coded_size(tide::ids::SilentTracks) +
        tide::vint::coded_size(st1.total_size() + st2.total_size()) +
        st1.total_size() + st2.total_size() + ps.total_size();
    e.silent_tracks().push_back(tide::SilentTrackNumber(1));
    e.silent_tracks().push_back(tide::SilentTrackNumber(2));
    e.previous_size(0x1234);
    output.str(std::string());
    expected.str(std::string());
    tc.write(expected);
    tide::ids::write(tide::ids::SilentTracks, expected);
    tide::vint::write(st1.total_size() + st2.total_size(), expected);
    st1.write(expected);
    st2.write(expected);
    ps.write(expected);
    EXPECT_EQ(expected_size, e.write_body(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());
    output.str(std::string());
    expected.str(std::string());
    tide::ids::write(tide::ids::Cluster, expected);
    tide::vint::write(expected_size, expected);
    tc.write(expected);
    tide::ids::write(tide::ids::SilentTracks, expected);
    tide::vint::write(st1.total_size() + st2.total_size(), expected);
    st1.write(expected);
    st2.write(expected);
    ps.write(expected);
    EXPECT_EQ(tide::ids::coded_size(tide::ids::Cluster) +
            tide::vint::coded_size(expected_size) + expected_size,
            e.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());
}


TEST(BaseCluster, Read)
{
    std::stringstream input;
    tide::UIntElement tc(tide::ids::Timecode, 42);
    tide::UIntElement st1(tide::ids::SilentTrackNumber, 1);
    tide::UIntElement st2(tide::ids::SilentTrackNumber, 2);
    tide::UIntElement ps(tide::ids::PrevSize, 0x1234);

    FakeCluster e;
    std::streamsize body_size(tc.total_size());
    tide::vint::write(body_size, input);
    tc.write(input);
    EXPECT_EQ(tide::vint::coded_size(body_size) + body_size,
            e.read(input));
    EXPECT_EQ(42, e.timecode());
    EXPECT_TRUE(e.silent_tracks().empty());
    EXPECT_EQ(0, e.previous_size());

    body_size += tide::ids::coded_size(tide::ids::SilentTracks) +
        tide::vint::coded_size(st1.total_size() + st2.total_size()) +
        st1.total_size() + st2.total_size() + ps.total_size();
    tide::vint::write(body_size, input);
    tc.write(input);
    tide::ids::write(tide::ids::SilentTracks, input);
    tide::vint::write(st1.total_size() + st2.total_size(), input);
    st1.write(input);
    st2.write(input);
    ps.write(input);
    EXPECT_EQ(tide::vint::coded_size(body_size) + body_size,
            e.read(input));
    EXPECT_EQ(42, e.timecode());
    EXPECT_FALSE(e.silent_tracks().empty());
    EXPECT_EQ(0x1234, e.previous_size());

    // Body size value wrong (too small)
    input.str(std::string());
    tide::vint::write(2, input);
    tc.write(input);
    ps.write(input);
    EXPECT_THROW(e.read(input), tide::BadBodySize);
    // Invalid child
    input.str(std::string());
    tide::UIntElement ue(tide::ids::EBML, 0xFFFF);
    tide::vint::write(ue.total_size(), input);
    ue.write(input);
    EXPECT_THROW(e.read(input), tide::InvalidChildID);
    // Missing timecode
    input.str(std::string());
    tide::vint::write(ps.total_size(), input);
    ps.write(input);
    EXPECT_THROW(e.read(input), tide::MissingChild);
}

