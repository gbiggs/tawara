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
#include <jonen/block_element.h>
#include <jonen/el_ids.h>
#include <jonen/exceptions.h>
#include <jonen/memory_cluster.h>
#include <jonen/simple_block.h>
#include <jonen/vint.h>

#include "test_utils.h"


TEST(MemoryCluster, Create)
{
    jonen::MemoryCluster e;
    EXPECT_TRUE(e.empty());
}


TEST(MemoryCluster, Empty)
{
    jonen::MemoryCluster c;
    EXPECT_TRUE(c.empty());
    jonen::BlockElement::Ptr b(new jonen::SimpleBlock(1, 12345,
                jonen::Block::LACING_NONE));
    c.push_back(b);
    EXPECT_FALSE(c.empty());
}


TEST(MemoryCluster, Count)
{
    jonen::MemoryCluster c;
    EXPECT_EQ(0, c.count());
    jonen::BlockElement::Ptr b1(new jonen::SimpleBlock(1, 12345,
                jonen::Block::LACING_NONE));
    jonen::BlockElement::Ptr b2(new jonen::SimpleBlock(2, 26262,
                jonen::Block::LACING_NONE));
    c.push_back(b1);
    c.push_back(b2);
    EXPECT_EQ(2, c.count());
}


TEST(MemoryCluster, Clear)
{
    jonen::MemoryCluster c;
    jonen::BlockElement::Ptr b(new jonen::SimpleBlock(1, 12345,
                jonen::Block::LACING_NONE));
    c.push_back(b);
    EXPECT_FALSE(c.empty());
    c.clear();
    EXPECT_TRUE(c.empty());
}


TEST(MemoryCluster, Erase)
{
    jonen::MemoryCluster c;
    jonen::BlockElement::Ptr b1(new jonen::SimpleBlock(1, 12345,
                jonen::Block::LACING_NONE));
    jonen::BlockElement::Ptr b2(new jonen::SimpleBlock(2, 26262,
                jonen::Block::LACING_NONE));
    c.push_back(b1);
    c.push_back(b2);
    EXPECT_EQ(2, c.count());
    EXPECT_TRUE(b1 == *c.begin());
    c.erase(c.begin());
    EXPECT_EQ(1, c.count());
    EXPECT_TRUE(b2 == *c.begin());

    jonen::BlockElement::Ptr b3(new jonen::SimpleBlock(3, 12345,
                jonen::Block::LACING_NONE));
    jonen::BlockElement::Ptr b4(new jonen::SimpleBlock(4, 26262,
                jonen::Block::LACING_NONE));
    c.push_back(b3);
    c.push_back(b4);
    EXPECT_EQ(3, c.count());
    c.erase(c.begin(), ++(++c.begin()));
    EXPECT_EQ(1, c.count());
    EXPECT_TRUE(b4 == *c.begin());
}


TEST(MemoryCluster, PushBack)
{
    jonen::MemoryCluster c;
    jonen::BlockElement::Ptr b1(new jonen::SimpleBlock(1, 12345,
                jonen::Block::LACING_NONE));
    jonen::BlockElement::Ptr b2(new jonen::SimpleBlock(2, 26262,
                jonen::Block::LACING_NONE));
    c.push_back(b1);
    EXPECT_TRUE(b1 == *c.begin());
    c.push_back(b2);
    EXPECT_TRUE(b2 == *(++c.begin()));
}


TEST(MemoryCluster, Iterators)
{
    jonen::MemoryCluster c;
    EXPECT_TRUE(c.begin() == c.end());

    jonen::BlockElement::Ptr b1(new jonen::SimpleBlock(1, 12345,
                jonen::Block::LACING_NONE));
    jonen::BlockElement::Ptr b2(new jonen::SimpleBlock(2, 26262,
                jonen::Block::LACING_NONE));
    c.push_back(b1);
    EXPECT_FALSE(c.begin() == c.end());
    EXPECT_TRUE(b1 == *c.begin());
    c.push_back(b2);
    EXPECT_TRUE(b2 == *(++c.begin()));
}


TEST(MemoryCluster, Size)
{
    jonen::MemoryCluster c;
    jonen::UIntElement tc(jonen::ids::Timecode, 0);
    std::streamsize body_size(tc.size());
    EXPECT_EQ(jonen::ids::size(jonen::ids::Cluster) + 8 + body_size, c.size());
}


TEST(MemoryCluster, Write)
{
    std::ostringstream output;
    std::stringstream expected;
    jonen::UIntElement tc(jonen::ids::Timecode, 0);
    jonen::BlockElement::Ptr b1(new jonen::SimpleBlock(1, 12345,
                jonen::Block::LACING_NONE));
    jonen::BlockElement::Ptr b2(new jonen::SimpleBlock(2, 26262,
                jonen::Block::LACING_NONE));
    jonen::Block::value_type f1(test_utils::make_blob(5));
    b1->push_back(f1);
    jonen::Block::value_type f2(test_utils::make_blob(10));
    b2->push_back(f2);
    jonen::MemoryCluster c;

    std::streamsize expected_size(tc.size());
    jonen::ids::write(jonen::ids::Cluster, expected);
    jonen::vint::write(expected_size, expected, 8);
    tc.write(expected);
    EXPECT_EQ(jonen::ids::size(jonen::ids::Cluster) + 8 + expected_size,
            c.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());

    output.str(std::string());
    expected.str(std::string());
    c.push_back(b1);
    c.push_back(b2);
    jonen::ids::write(jonen::ids::Cluster, expected);
    jonen::vint::write(expected_size, expected, 8);
    tc.write(expected);
    EXPECT_EQ(jonen::ids::size(jonen::ids::Cluster) + 8 + expected_size,
            c.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());
    expected_size += b1->size() + b2->size();
    b1->write(expected);
    b2->write(expected);
    EXPECT_EQ(jonen::ids::size(jonen::ids::Cluster) + 8 + expected_size,
            c.finalise(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());
}


TEST(MemoryCluster, Read)
{
    std::stringstream input;
    jonen::UIntElement tc(jonen::ids::Timecode, 42);
    jonen::BlockElement::Ptr b1(new jonen::SimpleBlock(1, 12345,
                jonen::Block::LACING_NONE));
    jonen::BlockElement::Ptr b2(new jonen::SimpleBlock(2, 26262,
                jonen::Block::LACING_NONE));
    jonen::Block::value_type f1(test_utils::make_blob(5));
    b1->push_back(f1);
    jonen::Block::value_type f2(test_utils::make_blob(10));
    b2->push_back(f2);
    jonen::MemoryCluster c;

    std::streamsize body_size(tc.size());
    jonen::vint::write(body_size, input);
    tc.write(input);
    EXPECT_EQ(jonen::vint::size(body_size) + body_size,
            c.read(input));
    EXPECT_EQ(42, c.timecode());

    input.str(std::string());
    body_size += b1->size() + b2->size();
    jonen::vint::write(body_size, input);
    tc.write(input);
    b1->write(input);
    b2->write(input);
    EXPECT_EQ(jonen::vint::size(body_size) + body_size,
            c.read(input));
    EXPECT_EQ(42, c.timecode());
    EXPECT_EQ(2, c.count());
    EXPECT_TRUE((*boost::static_pointer_cast<jonen::SimpleBlock>(b1)) ==
            (*boost::static_pointer_cast<jonen::SimpleBlock>(*c.begin())));
    EXPECT_TRUE((*boost::static_pointer_cast<jonen::SimpleBlock>(b2)) ==
            (*boost::static_pointer_cast<jonen::SimpleBlock>(*(++c.begin()))));
}

