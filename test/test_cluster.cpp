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
#include <tawara/cluster.h>
#include <tawara/el_ids.h>
#include <tawara/exceptions.h>
#include <tawara/vint.h>

#include "test_utils.h"


// Fake Cluster implementation
class FakeCluster : public tawara::Cluster
{
    public:
        FakeCluster(uint64_t tc=0)
            : tawara::Cluster(tc)
        {
        }

        bool empty() const
        {
            return true;
        }

        tawara::Cluster::size_type count() const
        {
            return 0;
        }

        void clear()
        {
        }

        void push_back(tawara::Cluster::value_type const&)
        {
        }

        std::streamsize finalise(std::ostream& output)
        {
            return 0;
        }

        std::streamsize blocks_size() const
        {
            return 0;
        }

        std::streamsize read_blocks(std::istream& input, std::streamsize size)
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
    e.silent_tracks().push_back(tawara::SilentTrackNumber(15));
    EXPECT_FALSE(e.silent_tracks().empty());
    EXPECT_EQ(e.silent_tracks()[0], 15);
}


TEST(BaseCluster, Position)
{
    FakeCluster e;
    EXPECT_THROW(e.position(), tawara::NotImplemented);
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
    tawara::UIntElement tc(tawara::ids::Timecode, 0);
    std::streamsize body_size(tc.size());
    EXPECT_EQ(tawara::ids::size(tawara::ids::Cluster) + 8 + body_size, e.size());

    tawara::UIntElement st1(tawara::ids::SilentTrackNumber, 1);
    tawara::UIntElement st2(tawara::ids::SilentTrackNumber, 2);
    body_size += tawara::ids::size(tawara::ids::SilentTracks) +
        tawara::vint::size(st1.size() + st2.size()) +
        st1.size() + st2.size();
    e.silent_tracks().push_back(tawara::SilentTrackNumber(1));
    e.silent_tracks().push_back(tawara::SilentTrackNumber(2));
    EXPECT_EQ(tawara::ids::size(tawara::ids::Cluster) + 8 + body_size, e.size());

    tawara::UIntElement ps(tawara::ids::PrevSize, 0x1234);
    body_size += ps.size();
    e.previous_size(0x1234);
    EXPECT_EQ(tawara::ids::size(tawara::ids::Cluster) + 8 + body_size, e.size());
}


TEST(BaseCluster, Write)
{
    std::ostringstream output;
    std::stringstream expected;
    tawara::UIntElement tc(tawara::ids::Timecode, 0);
    tawara::UIntElement st1(tawara::ids::SilentTrackNumber, 1);
    tawara::UIntElement st2(tawara::ids::SilentTrackNumber, 2);
    tawara::UIntElement ps(tawara::ids::PrevSize, 0x1234);

    FakeCluster e;
    std::streamsize expected_size(tc.size());
    tawara::ids::write(tawara::ids::Cluster, expected);
    tawara::vint::write(expected_size, expected, 8);
    tc.write(expected);
    EXPECT_EQ(tawara::ids::size(tawara::ids::Cluster) + 8 + expected_size,
            e.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());

    expected_size += tawara::ids::size(tawara::ids::SilentTracks) +
        tawara::vint::size(st1.size() + st2.size()) +
        st1.size() + st2.size() + ps.size();
    e.silent_tracks().push_back(tawara::SilentTrackNumber(1));
    e.silent_tracks().push_back(tawara::SilentTrackNumber(2));
    e.previous_size(0x1234);
    output.str(std::string());
    expected.str(std::string());
    tawara::ids::write(tawara::ids::Cluster, expected);
    tawara::vint::write(expected_size, expected, 8);
    tc.write(expected);
    tawara::ids::write(tawara::ids::SilentTracks, expected);
    tawara::vint::write(st1.size() + st2.size(), expected);
    st1.write(expected);
    st2.write(expected);
    ps.write(expected);
    EXPECT_EQ(tawara::ids::size(tawara::ids::Cluster) + 8 + expected_size,
            e.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());
}


TEST(BaseCluster, Read)
{
    std::stringstream input;
    tawara::UIntElement tc(tawara::ids::Timecode, 42);
    tawara::UIntElement st1(tawara::ids::SilentTrackNumber, 1);
    tawara::UIntElement st2(tawara::ids::SilentTrackNumber, 2);
    tawara::UIntElement ps(tawara::ids::PrevSize, 0x1234);

    FakeCluster e;
    std::streamsize body_size(tc.size());
    tawara::vint::write(body_size, input);
    tc.write(input);
    EXPECT_EQ(tawara::vint::size(body_size) + body_size,
            e.read(input));
    EXPECT_EQ(42, e.timecode());
    EXPECT_TRUE(e.silent_tracks().empty());
    EXPECT_EQ(0, e.previous_size());

    body_size += tawara::ids::size(tawara::ids::SilentTracks) +
        tawara::vint::size(st1.size() + st2.size()) +
        st1.size() + st2.size() + ps.size();
    tawara::vint::write(body_size, input);
    tc.write(input);
    tawara::ids::write(tawara::ids::SilentTracks, input);
    tawara::vint::write(st1.size() + st2.size(), input);
    st1.write(input);
    st2.write(input);
    ps.write(input);
    EXPECT_EQ(tawara::vint::size(body_size) + body_size,
            e.read(input));
    EXPECT_EQ(42, e.timecode());
    EXPECT_FALSE(e.silent_tracks().empty());
    EXPECT_EQ(0x1234, e.previous_size());

    // Body size value wrong (too small)
    input.str(std::string());
    tawara::vint::write(2, input);
    tc.write(input);
    ps.write(input);
    EXPECT_THROW(e.read(input), tawara::BadBodySize);
    // Invalid child
    input.str(std::string());
    tawara::UIntElement ue(tawara::ids::EBML, 0xFFFF);
    tawara::vint::write(ue.size(), input);
    ue.write(input);
    EXPECT_THROW(e.read(input), tawara::InvalidChildID);
    // Missing timecode
    input.str(std::string());
    tawara::vint::write(ps.size(), input);
    ps.write(input);
    EXPECT_THROW(e.read(input), tawara::MissingChild);
}

