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
#include <celduin/el_ids.h>
#include <celduin/exceptions.h>
#include <celduin/metaseek.h>
#include <celduin/uint_element.h>
#include <celduin/vint.h>

#include "test_utils.h"


TEST(SeekHead, Create)
{
    celduin::SeekHead e;
    EXPECT_EQ(celduin::ids::SeekHead, e.id());
    EXPECT_TRUE(e.empty());
}


TEST(SeekHead, Assignment)
{
    celduin::SeekHead e1;
    celduin::SeekHead e2;
    e1.insert(std::make_pair(celduin::ids::Tracks, 42));
    e1.insert(std::make_pair(celduin::ids::Cluster, 84));

    EXPECT_TRUE(e2.empty());
    e2 = e1;
    EXPECT_FALSE(e2.empty());
    EXPECT_EQ(e2.begin()->first, celduin::ids::Tracks);
    EXPECT_EQ(e2.begin()->second, 42);
    EXPECT_EQ((++e2.begin())->first, celduin::ids::Cluster);
    EXPECT_EQ((++e2.begin())->second, 84);
}


TEST(SeekHead, BeginEnd)
{
    celduin::SeekHead e;

    EXPECT_TRUE(e.begin() == e.end());
    EXPECT_TRUE(e.rbegin() == e.rend());
    e.insert(std::make_pair(celduin::ids::Tracks, 42));
    EXPECT_FALSE(e.begin() == e.end());
    EXPECT_FALSE(e.rbegin() == e.rend());
}


TEST(SeekHead, Counts)
{
    celduin::SeekHead e;

    EXPECT_TRUE(e.empty());
    e.insert(std::make_pair(celduin::ids::Tracks, 42));
    EXPECT_FALSE(e.empty());
    EXPECT_EQ(1, e.count());
    e.insert(std::make_pair(celduin::ids::Cluster, 84));
    EXPECT_EQ(2, e.count());
}


TEST(SeekHead, Clear)
{
    celduin::SeekHead e;
    e.insert(std::make_pair(celduin::ids::Tracks, 42));
    EXPECT_FALSE(e.empty());
    e.clear();
    EXPECT_TRUE(e.empty());
}


TEST(SeekHead, Insert)
{
    celduin::SeekHead e1;

    // Single insert
    EXPECT_TRUE(e1.empty());
    celduin::SeekHead::iterator res = e1.insert(std::make_pair(celduin::ids::Tracks, 42));
    EXPECT_TRUE(res == e1.begin());
    EXPECT_FALSE(e1.empty());
    EXPECT_EQ(1, e1.count());
    EXPECT_EQ(e1.begin()->first, celduin::ids::Tracks);
    res = e1.insert(std::make_pair(celduin::ids::Cluster, 84));
    EXPECT_TRUE(res != e1.begin());
    EXPECT_FALSE(e1.empty());
    EXPECT_EQ(2, e1.count());
    EXPECT_EQ((++e1.begin())->first, celduin::ids::Cluster);
    EXPECT_EQ((++e1.begin())->second, 84);
    // Same ID
    res = e1.insert(std::make_pair(celduin::ids::Cluster, 168));
    EXPECT_TRUE(res != e1.begin());
    EXPECT_TRUE(res == ++(++e1.begin()));
    EXPECT_TRUE(res == ++(e1.find(celduin::ids::Cluster)));

    // Range insert
    celduin::SeekHead e2;
    EXPECT_TRUE(e2.empty());
    e2.insert(e1.begin(), e1.end());
    EXPECT_FALSE(e2.empty());
    EXPECT_EQ(3, e2.count());
    EXPECT_EQ(e2.begin()->first, celduin::ids::Tracks);
}


TEST(SeekHead, Erase)
{
    celduin::SeekHead e;
    e.insert(std::make_pair(celduin::ids::Tracks, 42));

    EXPECT_FALSE(e.empty());
    e.erase(e.begin());
    EXPECT_TRUE(e.empty());

    e.insert(std::make_pair(celduin::ids::Tracks, 42));
    EXPECT_FALSE(e.empty());
    e.erase(e.begin(), e.end());
    EXPECT_TRUE(e.empty());

    e.insert(std::make_pair(celduin::ids::Tracks, 42));
    EXPECT_FALSE(e.empty());
    e.erase(celduin::ids::Tracks);
    EXPECT_TRUE(e.empty());
    EXPECT_NO_THROW(e.erase(celduin::ids::Null));
}


TEST(SeekHead, Swap)
{
    celduin::SeekHead e1, e2;
    e1.insert(std::make_pair(celduin::ids::Tracks, 42));

    EXPECT_FALSE(e1.empty());
    EXPECT_TRUE(e2.empty());
    e2.swap(e1);
    EXPECT_TRUE(e1.empty());
    EXPECT_FALSE(e2.empty());
    EXPECT_EQ(e2.begin()->first, celduin::ids::Tracks);
}


TEST(SeekHead, Find)
{
    celduin::SeekHead e;
    e.insert(std::make_pair(celduin::ids::Tracks, 42));

    EXPECT_TRUE(e.find(celduin::ids::Tracks) == e.begin());
    EXPECT_TRUE(e.find(celduin::ids::Null) == e.end());
}


TEST(SeekHead, Equality)
{
    celduin::SeekHead e1;
    celduin::SeekHead e2;
    EXPECT_TRUE(e1 == e2);
    EXPECT_FALSE(e1 != e2);

    e2.insert(std::make_pair(celduin::ids::Tracks, 42));
    EXPECT_FALSE(e1 == e2);
    EXPECT_TRUE(e1 != e2);
}


TEST(SeekHead, Size)
{
    celduin::SeekHead ms;

    EXPECT_EQ(celduin::ids::size(celduin::ids::SeekHead) +
            celduin::vint::size(0),
            ms.size());

    std::vector<celduin::SeekElement> children;
    children.push_back(celduin::SeekElement(celduin::ids::Tracks, 0x76FB));
    children.push_back(celduin::SeekElement(celduin::ids::Cues, 0x7F));
    children.push_back(celduin::SeekElement(celduin::ids::Cluster, 0x10203040));

    std::streamsize body_size(0);
    BOOST_FOREACH(celduin::SeekElement e, children)
    {
        body_size += e.size();
        ms.insert(std::make_pair(e.indexed_id(), e.offset()));
    }
    EXPECT_EQ(celduin::ids::size(celduin::ids::SeekHead) +
            celduin::vint::size(body_size) + body_size,
            ms.size());
}


TEST(SeekHead, Write)
{
    std::ostringstream output;
    std::stringstream expected;

    celduin::SeekHead ms;

    std::vector<celduin::SeekElement> children;
    children.push_back(celduin::SeekElement(celduin::ids::Tracks, 0x76FB));
    children.push_back(celduin::SeekElement(celduin::ids::Cues, 0x7F));
    children.push_back(celduin::SeekElement(celduin::ids::Cluster, 0x10203040));

    std::streamsize body_size(0);
    BOOST_FOREACH(celduin::SeekElement e, children)
    {
        body_size += e.size();
        ms.insert(std::make_pair(e.indexed_id(), e.offset()));
    }
    celduin::ids::write(celduin::ids::SeekHead, expected);
    celduin::vint::write(body_size, expected);
    BOOST_FOREACH(celduin::SeekElement e, children)
    {
        e.write(expected);
    }

    EXPECT_EQ(celduin::ids::size(celduin::ids::SeekHead) +
            celduin::vint::size(body_size) + body_size,
            ms.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected.str());
}


TEST(SeekHead, Read)
{
    std::stringstream input;

    std::vector<celduin::SeekElement> children;
    children.push_back(celduin::SeekElement(celduin::ids::Tracks, 0x76FB));
    children.push_back(celduin::SeekElement(celduin::ids::Cues, 0x7F));
    children.push_back(celduin::SeekElement(celduin::ids::Cluster, 0x10203040));

    std::streamsize body_size(0);
    BOOST_FOREACH(celduin::SeekElement e, children)
    {
        body_size += e.size();
    }
    celduin::vint::write(body_size, input);
    BOOST_FOREACH(celduin::SeekElement e, children)
    {
        e.write(input);
    }

    celduin::SeekHead ms;
    EXPECT_EQ(celduin::vint::size(body_size) + body_size,
            ms.read(input));
    EXPECT_EQ(3, ms.count());
    celduin::SeekHead::iterator res = ms.find(celduin::ids::Tracks);
    EXPECT_TRUE(res != ms.end());
    EXPECT_EQ(res->first, celduin::ids::Tracks);
    EXPECT_EQ(res->second, 0x76FB);
    res = ms.find(celduin::ids::Cues);
    EXPECT_TRUE(res != ms.end());
    EXPECT_EQ(res->first, celduin::ids::Cues);
    EXPECT_EQ(res->second, 0x7F);
    res = ms.find(celduin::ids::Cluster);
    EXPECT_TRUE(res != ms.end());
    EXPECT_EQ(res->first, celduin::ids::Cluster);
    EXPECT_EQ(res->second, 0x10203040);

    // No children at all
    input.str(std::string());
    celduin::vint::write(0, input);
    EXPECT_EQ(celduin::vint::size(0), ms.read(input));
    // SeekHead should be clearing its stored index before reading, so the
    // previous test's index should not affect this result
    EXPECT_TRUE(ms.empty());
    // Body size value wrong (too small)
    input.str(std::string());
    celduin::vint::write(2, input);
    children[0].write(input);
    EXPECT_THROW(ms.read(input), celduin::BadBodySize);
    // Invalid child
    input.str(std::string());
    celduin::UIntElement ue(celduin::ids::EBML, 0xFFFF);
    celduin::vint::write(ue.size(), input);
    ue.write(input);
    EXPECT_THROW(ms.read(input), celduin::InvalidChildID);
}

