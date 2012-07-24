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
#include <jonen/el_ids.h>
#include <jonen/exceptions.h>
#include <jonen/seek_element.h>
#include <jonen/uint_element.h>
#include <jonen/vint.h>

#include "test_utils.h"


TEST(Seek, Create)
{
    jonen::SeekElement e(0x80, 2);
    EXPECT_EQ(jonen::ids::Seek, e.id());
    EXPECT_EQ(0x80, e.indexed_id());
    EXPECT_EQ(2, e.offset());
}


TEST(Seek, ID)
{
    jonen::SeekElement e(0x80, 0);
    EXPECT_EQ(0x80, e.indexed_id());
    e.indexed_id(0x9F);
    EXPECT_EQ(0x9F, e.indexed_id());
    EXPECT_EQ(0, e.offset());
    EXPECT_EQ(jonen::ids::Seek, e.id());
}


TEST(Seek, Offset)
{
    jonen::SeekElement e(0x80, 0);
    EXPECT_EQ(0, e.offset());
    e.offset(12345);
    EXPECT_EQ(12345, e.offset());
    EXPECT_EQ(0x80, e.indexed_id());
}


TEST(Seek, Size)
{
    jonen::BinaryElement be(jonen::ids::SeekID,
            // SeekHead is a nice, long ID to test with
            jonen::ids::encode(jonen::ids::SeekHead));
    jonen::UIntElement ue(jonen::ids::SeekPosition, 0x1010);
    std::streamsize body_size(be.size() + ue.size());

    jonen::SeekElement se(jonen::ids::SeekHead, 0x1010);
    EXPECT_EQ(jonen::ids::size(jonen::ids::Seek) +
            jonen::vint::size(body_size) + body_size,
            se.size());
}


TEST(Seek, Write)
{
    std::ostringstream output;
    std::stringstream expected;

    jonen::BinaryElement be(jonen::ids::SeekID,
            // SeekHead is a nice, long ID to test with
            jonen::ids::encode(jonen::ids::SeekHead));
    jonen::UIntElement ue(jonen::ids::SeekPosition, 12345);
    std::streamsize expected_size(be.size() + ue.size());
    jonen::SeekElement se(jonen::ids::SeekHead, 12345);
    jonen::ids::write(jonen::ids::Seek, expected);
    jonen::vint::write(expected_size, expected);
    be.write(expected);
    ue.write(expected);
    EXPECT_EQ(jonen::ids::size(jonen::ids::Seek) +
            jonen::vint::size(expected_size) + expected_size,
            se.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected.str());
}


TEST(Seek, Read)
{
    std::stringstream input;

    jonen::BinaryElement be(jonen::ids::SeekID,
            // SeekHead is a nice, long ID to test with
            jonen::ids::encode(jonen::ids::SeekHead));
    jonen::UIntElement ue(jonen::ids::SeekPosition, 12345);
    std::streamsize body_size(be.size() + ue.size());

    jonen::vint::write(body_size, input);
    be.write(input);
    ue.write(input);

    jonen::SeekElement e(0x80, 0);
    EXPECT_EQ(jonen::vint::size(body_size) + body_size,
        e.read(input));
    EXPECT_EQ(jonen::ids::SeekHead, e.indexed_id());
    EXPECT_EQ(12345, e.offset());

    input.str(std::string());
    be.value(jonen::ids::encode(jonen::ids::EBML));
    ue.value(54321);
    jonen::vint::write(body_size, input);
    // Note the reversed order this time
    ue.write(input);
    be.write(input);

    EXPECT_EQ(jonen::vint::size(body_size) + body_size,
        e.read(input));
    EXPECT_EQ(jonen::ids::EBML, e.indexed_id());
    EXPECT_EQ(54321, e.offset());

    // No SeekID child
    input.str(std::string());
    jonen::vint::write(ue.size(), input);
    ue.write(input);
    EXPECT_THROW(e.read(input), jonen::MissingChild);
    // No SeekPosition child
    input.str(std::string());
    jonen::vint::write(be.size(), input);
    be.write(input);
    EXPECT_THROW(e.read(input), jonen::MissingChild);
    // No children at all
    input.str(std::string());
    jonen::vint::write(0, input);
    EXPECT_THROW(e.read(input), jonen::MissingChild);
    // Body size value wrong (too big)
    input.str(std::string());
    jonen::vint::write(ue.size() + be.size() + 5, input);
    ue.write(input);
    be.write(input);
    EXPECT_THROW(e.read(input), jonen::BadBodySize);
    // Body size value wrong (too small)
    input.str(std::string());
    jonen::vint::write(2, input);
    ue.write(input);
    be.write(input);
    EXPECT_THROW(e.read(input), jonen::BadBodySize);
    // Invalid child
    input.str(std::string());
    jonen::UIntElement ue2(jonen::ids::EBML, 0xFFFF);
    jonen::vint::write(ue2.size(), input);
    ue2.write(input);
    EXPECT_THROW(e.read(input), jonen::InvalidChildID);
}

