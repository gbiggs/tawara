/* Tide
 *
 * Test the Seek element.
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
#include <tide/seek_element.h>
#include <tide/uint_element.h>
#include <tide/vint.h>

#include "test_utils.h"


TEST(Seek, Create)
{
    tide::SeekElement e(0x80, 2);
    EXPECT_EQ(tide::ids::Seek, e.id());
    EXPECT_EQ(0x80, e.indexed_id());
    EXPECT_EQ(2, e.offset());
}


TEST(Seek, ID)
{
    tide::SeekElement e(0x80, 0);
    EXPECT_EQ(0x80, e.indexed_id());
    e.indexed_id(0x9F);
    EXPECT_EQ(0x9F, e.indexed_id());
    EXPECT_EQ(0, e.offset());
    EXPECT_EQ(tide::ids::Seek, e.id());
}


TEST(Seek, Offset)
{
    tide::SeekElement e(0x80, 0);
    EXPECT_EQ(0, e.offset());
    e.offset(12345);
    EXPECT_EQ(12345, e.offset());
    EXPECT_EQ(0x80, e.indexed_id());
}


TEST(Seek, Size)
{
    tide::BinaryElement be(tide::ids::SeekID,
            // SeekHead is a nice, long ID to test with
            tide::ids::encode(tide::ids::SeekHead));
    tide::UIntElement ue(tide::ids::SeekPosition, 0x1010);
    std::streamsize body_size(be.size() + ue.size());

    tide::SeekElement se(tide::ids::SeekHead, 0x1010);
    EXPECT_EQ(tide::ids::coded_size(tide::ids::Seek) +
            tide::vint::coded_size(body_size) + body_size,
            se.size());
}


TEST(Seek, Write)
{
    std::ostringstream output;
    std::stringstream expected;

    tide::BinaryElement be(tide::ids::SeekID,
            // SeekHead is a nice, long ID to test with
            tide::ids::encode(tide::ids::SeekHead));
    tide::UIntElement ue(tide::ids::SeekPosition, 12345);
    std::streamsize expected_size(be.size() + ue.size());
    tide::SeekElement se(tide::ids::SeekHead, 12345);
    tide::ids::write(tide::ids::Seek, expected);
    tide::vint::write(expected_size, expected);
    be.write(expected);
    ue.write(expected);
    EXPECT_EQ(tide::ids::coded_size(tide::ids::Seek) +
            tide::vint::coded_size(expected_size) + expected_size,
            se.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected.str());
}


TEST(Seek, Read)
{
    std::stringstream input;

    tide::BinaryElement be(tide::ids::SeekID,
            // SeekHead is a nice, long ID to test with
            tide::ids::encode(tide::ids::SeekHead));
    tide::UIntElement ue(tide::ids::SeekPosition, 12345);
    std::streamsize body_size(be.size() + ue.size());

    tide::vint::write(body_size, input);
    be.write(input);
    ue.write(input);

    tide::SeekElement e(0x80, 0);
    EXPECT_EQ(tide::vint::coded_size(body_size) + body_size,
        e.read(input));
    EXPECT_EQ(tide::ids::SeekHead, e.indexed_id());
    EXPECT_EQ(12345, e.offset());

    input.str(std::string());
    be.value(tide::ids::encode(tide::ids::EBML));
    ue.value(54321);
    tide::vint::write(body_size, input);
    // Note the reversed order this time
    ue.write(input);
    be.write(input);

    EXPECT_EQ(tide::vint::coded_size(body_size) + body_size,
        e.read(input));
    EXPECT_EQ(tide::ids::EBML, e.indexed_id());
    EXPECT_EQ(54321, e.offset());

    // No SeekID child
    input.str(std::string());
    tide::vint::write(ue.size(), input);
    ue.write(input);
    EXPECT_THROW(e.read(input), tide::MissingChild);
    // No SeekPosition child
    input.str(std::string());
    tide::vint::write(be.size(), input);
    be.write(input);
    EXPECT_THROW(e.read(input), tide::MissingChild);
    // No children at all
    input.str(std::string());
    tide::vint::write(0, input);
    EXPECT_THROW(e.read(input), tide::MissingChild);
    // Body size value wrong (too big)
    input.str(std::string());
    tide::vint::write(ue.size() + be.size() + 5, input);
    ue.write(input);
    be.write(input);
    EXPECT_THROW(e.read(input), tide::BadBodySize);
    // Body size value wrong (too small)
    input.str(std::string());
    tide::vint::write(2, input);
    ue.write(input);
    be.write(input);
    EXPECT_THROW(e.read(input), tide::BadBodySize);
    // Invalid child
    input.str(std::string());
    tide::UIntElement ue2(tide::ids::EBML, 0xFFFF);
    tide::vint::write(ue2.size(), input);
    ue2.write(input);
    EXPECT_THROW(e.read(input), tide::InvalidChildID);
}

