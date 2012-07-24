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
#include <jonen/cluster.h>
#include <jonen/el_ids.h>
#include <jonen/exceptions.h>
#include <jonen/vint.h>

#include "test_utils.h"


// Fake Cluster implementation
class FakeCluster : public jonen::Cluster
{
    public:
        FakeCluster(uint64_t tc=0)
            : jonen::Cluster(tc)
        {
        }

        bool empty() const
        {
            return true;
        }

        jonen::Cluster::size_type count() const
        {
            return 0;
        }

        void clear()
        {
        }

        void push_back(jonen::Cluster::value_type const&)
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
    e.silent_tracks().push_back(jonen::SilentTrackNumber(15));
    EXPECT_FALSE(e.silent_tracks().empty());
    EXPECT_EQ(e.silent_tracks()[0], 15);
}


TEST(BaseCluster, Position)
{
    FakeCluster e;
    EXPECT_THROW(e.position(), jonen::NotImplemented);
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
    jonen::UIntElement tc(jonen::ids::Timecode, 0);
    std::streamsize body_size(tc.size());
    EXPECT_EQ(jonen::ids::size(jonen::ids::Cluster) + 8 + body_size, e.size());

    jonen::UIntElement st1(jonen::ids::SilentTrackNumber, 1);
    jonen::UIntElement st2(jonen::ids::SilentTrackNumber, 2);
    body_size += jonen::ids::size(jonen::ids::SilentTracks) +
        jonen::vint::size(st1.size() + st2.size()) +
        st1.size() + st2.size();
    e.silent_tracks().push_back(jonen::SilentTrackNumber(1));
    e.silent_tracks().push_back(jonen::SilentTrackNumber(2));
    EXPECT_EQ(jonen::ids::size(jonen::ids::Cluster) + 8 + body_size, e.size());

    jonen::UIntElement ps(jonen::ids::PrevSize, 0x1234);
    body_size += ps.size();
    e.previous_size(0x1234);
    EXPECT_EQ(jonen::ids::size(jonen::ids::Cluster) + 8 + body_size, e.size());
}


TEST(BaseCluster, Write)
{
    std::ostringstream output;
    std::stringstream expected;
    jonen::UIntElement tc(jonen::ids::Timecode, 0);
    jonen::UIntElement st1(jonen::ids::SilentTrackNumber, 1);
    jonen::UIntElement st2(jonen::ids::SilentTrackNumber, 2);
    jonen::UIntElement ps(jonen::ids::PrevSize, 0x1234);

    FakeCluster e;
    std::streamsize expected_size(tc.size());
    jonen::ids::write(jonen::ids::Cluster, expected);
    jonen::vint::write(expected_size, expected, 8);
    tc.write(expected);
    EXPECT_EQ(jonen::ids::size(jonen::ids::Cluster) + 8 + expected_size,
            e.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());

    expected_size += jonen::ids::size(jonen::ids::SilentTracks) +
        jonen::vint::size(st1.size() + st2.size()) +
        st1.size() + st2.size() + ps.size();
    e.silent_tracks().push_back(jonen::SilentTrackNumber(1));
    e.silent_tracks().push_back(jonen::SilentTrackNumber(2));
    e.previous_size(0x1234);
    output.str(std::string());
    expected.str(std::string());
    jonen::ids::write(jonen::ids::Cluster, expected);
    jonen::vint::write(expected_size, expected, 8);
    tc.write(expected);
    jonen::ids::write(jonen::ids::SilentTracks, expected);
    jonen::vint::write(st1.size() + st2.size(), expected);
    st1.write(expected);
    st2.write(expected);
    ps.write(expected);
    EXPECT_EQ(jonen::ids::size(jonen::ids::Cluster) + 8 + expected_size,
            e.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());
}


TEST(BaseCluster, Read)
{
    std::stringstream input;
    jonen::UIntElement tc(jonen::ids::Timecode, 42);
    jonen::UIntElement st1(jonen::ids::SilentTrackNumber, 1);
    jonen::UIntElement st2(jonen::ids::SilentTrackNumber, 2);
    jonen::UIntElement ps(jonen::ids::PrevSize, 0x1234);

    FakeCluster e;
    std::streamsize body_size(tc.size());
    jonen::vint::write(body_size, input);
    tc.write(input);
    EXPECT_EQ(jonen::vint::size(body_size) + body_size,
            e.read(input));
    EXPECT_EQ(42, e.timecode());
    EXPECT_TRUE(e.silent_tracks().empty());
    EXPECT_EQ(0, e.previous_size());

    body_size += jonen::ids::size(jonen::ids::SilentTracks) +
        jonen::vint::size(st1.size() + st2.size()) +
        st1.size() + st2.size() + ps.size();
    jonen::vint::write(body_size, input);
    tc.write(input);
    jonen::ids::write(jonen::ids::SilentTracks, input);
    jonen::vint::write(st1.size() + st2.size(), input);
    st1.write(input);
    st2.write(input);
    ps.write(input);
    EXPECT_EQ(jonen::vint::size(body_size) + body_size,
            e.read(input));
    EXPECT_EQ(42, e.timecode());
    EXPECT_FALSE(e.silent_tracks().empty());
    EXPECT_EQ(0x1234, e.previous_size());

    // Body size value wrong (too small)
    input.str(std::string());
    jonen::vint::write(2, input);
    tc.write(input);
    ps.write(input);
    EXPECT_THROW(e.read(input), jonen::BadBodySize);
    // Invalid child
    input.str(std::string());
    jonen::UIntElement ue(jonen::ids::EBML, 0xFFFF);
    jonen::vint::write(ue.size(), input);
    ue.write(input);
    EXPECT_THROW(e.read(input), jonen::InvalidChildID);
    // Missing timecode
    input.str(std::string());
    jonen::vint::write(ps.size(), input);
    ps.write(input);
    EXPECT_THROW(e.read(input), jonen::MissingChild);
}

