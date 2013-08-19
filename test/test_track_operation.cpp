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

#include <boost/foreach.hpp>
#include <gtest/gtest.h>
#include <tawara/el_ids.h>
#include <tawara/exceptions.h>
#include <tawara/track_operation.h>
#include <tawara/uint_element.h>
#include <tawara/vint.h>

#include "test_utils.h"


TEST(TrackJoinBlocks, Create)
{
    tawara::TrackJoinBlocks e;
    EXPECT_EQ(tawara::ids::TrackJoinBlocks, e.id());
    EXPECT_EQ(0, e.count());
}


TEST(TrackJoinBlocks, Type)
{
    tawara::TrackJoinBlocks e;
    EXPECT_EQ(e.type(), "joinblocks");
}


TEST(TrackJoinBlocks, Append)
{
    tawara::TrackJoinBlocks e;
    e.append(0xFFFF);
    ASSERT_EQ(1, e.count());
    e.append(0xC0C0);
    ASSERT_EQ(2, e.count());
    EXPECT_EQ(0xFFFF, e[0]);
    EXPECT_EQ(0xC0C0, e[1]);

    EXPECT_THROW(e.append(0), tawara::ValueOutOfRange);
}


TEST(TrackJoinBlocks, Remove)
{
    tawara::TrackJoinBlocks e;
    e.append(0xFFFF);
    e.append(0xC0C0);
    ASSERT_EQ(2, e.count());
    e.remove(0);
    ASSERT_EQ(1, e.count());
    EXPECT_EQ(0xC0C0, e[0]);
    e.append(0x0101);
    e.remove(1);
    EXPECT_EQ(0xC0C0, e[0]);
}


TEST(TrackJoinBlocks, IndexOperator)
{
    tawara::TrackJoinBlocks e;
    e.append(0xFFFF);
    e.append(0xC0C0);
    e.append(0x0101);
    EXPECT_EQ(0xFFFF, e[0]);
    EXPECT_EQ(0x0101, e[2]);
}


TEST(TrackJoinBlocks, Equality)
{
    tawara::TrackJoinBlocks e1;
    e1.append(0xFFFF);
    tawara::TrackJoinBlocks e2;
    e2.append(0xFFFF);
    EXPECT_TRUE(e1 == e2);
    EXPECT_FALSE(e1 != e2);

    e2.append(0x7777);
    EXPECT_FALSE(e1 == e2);
    EXPECT_TRUE(e1 != e2);
}


TEST(TrackJoinBlocks, Size)
{
    tawara::TrackJoinBlocks e;

    EXPECT_EQ(tawara::ids::size(tawara::ids::TrackJoinBlocks) +
            tawara::vint::size(0),
            e.size());

    std::vector<tawara::UIntElement> children;
    children.push_back(tawara::UIntElement(tawara::ids::TrackJoinUID, 0xFFFF));
    children.push_back(tawara::UIntElement(tawara::ids::TrackJoinUID, 0xC0C0));
    children.push_back(tawara::UIntElement(tawara::ids::TrackJoinUID, 0x0101));

    std::streamsize body_size(0);
    BOOST_FOREACH(tawara::UIntElement child, children)
    {
        body_size += child.size();
        e.append(child.value());
    }

    EXPECT_EQ(tawara::ids::size(tawara::ids::TrackJoinBlocks) +
            tawara::vint::size(body_size) + body_size,
            e.size());
}


TEST(TrackJoinBlocks, Write)
{
    std::ostringstream output;
    std::stringstream expected;

    tawara::TrackJoinBlocks e;

    std::vector<tawara::UIntElement> children;
    children.push_back(tawara::UIntElement(tawara::ids::TrackJoinUID, 0xFFFF));
    children.push_back(tawara::UIntElement(tawara::ids::TrackJoinUID, 0xC0C0));
    children.push_back(tawara::UIntElement(tawara::ids::TrackJoinUID, 0x0101));

    std::streamsize body_size(0);
    BOOST_FOREACH(tawara::UIntElement child, children)
    {
        body_size += child.size();
        e.append(child.value());
    }
    tawara::ids::write(tawara::ids::TrackJoinBlocks, expected);
    tawara::vint::write(body_size, expected);
    BOOST_FOREACH(tawara::UIntElement child, children)
    {
        child.write(expected);
    }

    EXPECT_EQ(tawara::ids::size(tawara::ids::TrackJoinBlocks) +
            tawara::vint::size(body_size) + body_size,
            e.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected.str());
}


TEST(TrackJoinBlocks, Read)
{
    std::stringstream input;

    std::vector<tawara::UIntElement> children;
    children.push_back(tawara::UIntElement(tawara::ids::TrackJoinUID, 0xFFFF));
    children.push_back(tawara::UIntElement(tawara::ids::TrackJoinUID, 0xC0C0));
    children.push_back(tawara::UIntElement(tawara::ids::TrackJoinUID, 0x0101));

    std::streamsize body_size(0);
    BOOST_FOREACH(tawara::UIntElement child, children)
    {
        body_size += child.size();
    }
    tawara::vint::write(body_size, input);
    BOOST_FOREACH(tawara::UIntElement child, children)
    {
        child.write(input);
    }

    tawara::TrackJoinBlocks e;
    EXPECT_EQ(tawara::vint::size(body_size) + body_size,
            e.read(input));
    EXPECT_EQ(3, e.count());
    EXPECT_EQ(0xFFFF, e[0]);
    EXPECT_EQ(0xC0C0, e[1]);
    EXPECT_EQ(0x0101, e[2]);

    // No children at all
    input.str(std::string());
    tawara::vint::write(0, input);
    EXPECT_THROW(e.read(input), tawara::MissingChild);
    // TrackJoinBlocks should be clearing its stored UIDs before reading, so
    // the previous test's index should not affect this result
    EXPECT_EQ(0, e.count());
    // Body size value wrong (too small)
    input.str(std::string());
    tawara::vint::write(2, input);
    children[0].write(input);
    EXPECT_THROW(e.read(input), tawara::BadBodySize);
    // Invalid child
    input.str(std::string());
    tawara::UIntElement ue(tawara::ids::EBML, 0xFFFF);
    tawara::vint::write(ue.size(), input);
    ue.write(input);
    EXPECT_THROW(e.read(input), tawara::InvalidChildID);
}

