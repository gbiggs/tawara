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
#include <tawara/el_ids.h>
#include <tawara/exceptions.h>
#include <tawara/seek_element.h>
#include <tawara/uint_element.h>
#include <tawara/vint.h>

#include "test_utils.h"


TEST(Seek, Create)
{
    tawara::SeekElement e(0x80, 2);
    EXPECT_EQ(tawara::ids::Seek, e.id());
    EXPECT_EQ(0x80, e.indexed_id());
    EXPECT_EQ(2, e.offset());
}


TEST(Seek, ID)
{
    tawara::SeekElement e(0x80, 0);
    EXPECT_EQ(0x80, e.indexed_id());
    e.indexed_id(0x9F);
    EXPECT_EQ(0x9F, e.indexed_id());
    EXPECT_EQ(0, e.offset());
    EXPECT_EQ(tawara::ids::Seek, e.id());
}


TEST(Seek, Offset)
{
    tawara::SeekElement e(0x80, 0);
    EXPECT_EQ(0, e.offset());
    e.offset(12345);
    EXPECT_EQ(12345, e.offset());
    EXPECT_EQ(0x80, e.indexed_id());
}


TEST(Seek, Size)
{
    tawara::BinaryElement be(tawara::ids::SeekID,
            // SeekHead is a nice, long ID to test with
            tawara::ids::encode(tawara::ids::SeekHead));
    tawara::UIntElement ue(tawara::ids::SeekPosition, 0x1010);
    std::streamsize body_size(be.size() + ue.size());

    tawara::SeekElement se(tawara::ids::SeekHead, 0x1010);
    EXPECT_EQ(tawara::ids::size(tawara::ids::Seek) +
            tawara::vint::size(body_size) + body_size,
            se.size());
}


TEST(Seek, Write)
{
    std::ostringstream output;
    std::stringstream expected;

    tawara::BinaryElement be(tawara::ids::SeekID,
            // SeekHead is a nice, long ID to test with
            tawara::ids::encode(tawara::ids::SeekHead));
    tawara::UIntElement ue(tawara::ids::SeekPosition, 12345);
    std::streamsize expected_size(be.size() + ue.size());
    tawara::SeekElement se(tawara::ids::SeekHead, 12345);
    tawara::ids::write(tawara::ids::Seek, expected);
    tawara::vint::write(expected_size, expected);
    be.write(expected);
    ue.write(expected);
    EXPECT_EQ(tawara::ids::size(tawara::ids::Seek) +
            tawara::vint::size(expected_size) + expected_size,
            se.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected.str());
}


TEST(Seek, Read)
{
    std::stringstream input;

    tawara::BinaryElement be(tawara::ids::SeekID,
            // SeekHead is a nice, long ID to test with
            tawara::ids::encode(tawara::ids::SeekHead));
    tawara::UIntElement ue(tawara::ids::SeekPosition, 12345);
    std::streamsize body_size(be.size() + ue.size());

    tawara::vint::write(body_size, input);
    be.write(input);
    ue.write(input);

    tawara::SeekElement e(0x80, 0);
    EXPECT_EQ(tawara::vint::size(body_size) + body_size,
        e.read(input));
    EXPECT_EQ(tawara::ids::SeekHead, e.indexed_id());
    EXPECT_EQ(12345, e.offset());

    input.str(std::string());
    be.value(tawara::ids::encode(tawara::ids::EBML));
    ue.value(54321);
    tawara::vint::write(body_size, input);
    // Note the reversed order this time
    ue.write(input);
    be.write(input);

    EXPECT_EQ(tawara::vint::size(body_size) + body_size,
        e.read(input));
    EXPECT_EQ(tawara::ids::EBML, e.indexed_id());
    EXPECT_EQ(54321, e.offset());

    // No SeekID child
    input.str(std::string());
    tawara::vint::write(ue.size(), input);
    ue.write(input);
    EXPECT_THROW(e.read(input), tawara::MissingChild);
    // No SeekPosition child
    input.str(std::string());
    tawara::vint::write(be.size(), input);
    be.write(input);
    EXPECT_THROW(e.read(input), tawara::MissingChild);
    // No children at all
    input.str(std::string());
    tawara::vint::write(0, input);
    EXPECT_THROW(e.read(input), tawara::MissingChild);
    // Body size value wrong (too big)
    input.str(std::string());
    tawara::vint::write(ue.size() + be.size() + 5, input);
    ue.write(input);
    be.write(input);
    EXPECT_THROW(e.read(input), tawara::BadBodySize);
    // Body size value wrong (too small)
    input.str(std::string());
    tawara::vint::write(2, input);
    ue.write(input);
    be.write(input);
    EXPECT_THROW(e.read(input), tawara::BadBodySize);
    // Invalid child
    input.str(std::string());
    tawara::UIntElement ue2(tawara::ids::EBML, 0xFFFF);
    tawara::vint::write(ue2.size(), input);
    ue2.write(input);
    EXPECT_THROW(e.read(input), tawara::InvalidChildID);
}

