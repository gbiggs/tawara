/* Tide
 *
 * Base element tests.
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
#include <tide/element.h>
#include <tide/exceptions.h>
#include <tide/uint_element.h>
#include <tide/vint.h>

#include "test_consts.h"
#include "test_utils.h"


// Fake Element implementation
class FakeElement : public tide::Element
{
    public:
        FakeElement(uint32_t id)
            : Element(id)
        {
        }

        std::streamsize body_size() const
        {
            return 0;
        }

        std::streamsize write_id(std::ostream& output)
        {
            return 0;
        }

        std::streamsize write_body(std::ostream& output)
        {
            return 0;
        }

        std::streamsize read_body(std::istream& input, std::streamsize size)
        {
            return size;
        }
}; // class FakeElement


TEST(Element, Construction)
{
    EXPECT_EQ(1234, FakeElement(1234).id());
    EXPECT_THROW(FakeElement(0x00), tide::InvalidElementID);
    EXPECT_THROW(FakeElement(0xFF), tide::InvalidElementID);
    EXPECT_THROW(FakeElement(0xFFFF), tide::InvalidElementID);
    EXPECT_THROW(FakeElement(0xFFFFFF), tide::InvalidElementID);
    EXPECT_THROW(FakeElement(0xFFFFFFFF), tide::InvalidElementID);
}


TEST(Element, CopyConstruction)
{
    EXPECT_EQ(1234, FakeElement(FakeElement(1234)).id());
    // The exception actually comes from the inner constructor, but just to be
    // sure it makes it out...
    EXPECT_THROW(FakeElement(FakeElement(0x00)), tide::InvalidElementID);
}


TEST(Element, Assignment)
{
    FakeElement e1(1), e2(2);
    e2 = e1;
    EXPECT_EQ(e1.id(), e2.id());
}


TEST(Element, Size)
{
    FakeElement e(tide::ids::EBML);
    EXPECT_EQ(tide::ids::size(tide::ids::EBML) +
            tide::vint::size(0), e.size());
}


TEST(Element, Write)
{
    std::ostringstream output;
    std::stringstream expected;
    unsigned int dummy(0xFFFF);

    // Place some dummy data at the start to test the element recording its
    // write position.
    tide::vint::write(dummy, output);
    tide::vint::write(dummy, expected);

    FakeElement e(tide::ids::EBML);
    tide::ids::write(tide::ids::EBML, expected);
    tide::vint::write(0, expected);
    EXPECT_EQ(tide::ids::size(tide::ids::EBML) +
            tide::vint::size(0), e.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());
    EXPECT_EQ(tide::vint::size(dummy), e.offset());
}


TEST(Element, Read)
{
    std::stringstream input;
    unsigned int dummy(0xFFFF);
    FakeElement e(tide::ids::EBML);

    tide::vint::write(dummy, input);
    tide::ids::write(tide::ids::Info, input);
    tide::vint::write(0, input);
    input.seekg(tide::vint::size(dummy) +
            tide::ids::size(tide::ids::Info), std::ios::beg);

    EXPECT_EQ(tide::vint::size(0), e.read(input));
    EXPECT_EQ(tide::vint::size(dummy),
            e.offset());
}


TEST(ElementUtils, SkipRead)
{
    std::stringstream input;

    tide::UIntElement ue1(tide::ids::Null, 0xFFFFFFFF);
    tide::UIntElement ue2(tide::ids::Null, 0xFFFFFFFF);
    tide::UIntElement ue3(tide::ids::Null, 0xFFFFFFFF);
    ue1.write(input);
    ue2.write(input);
    ue3.write(input);
    input.seekp(0);
    input.seekg(0);

    tide::skip_read(input, true);
    EXPECT_EQ(ue1.size(), input.tellg());
    EXPECT_EQ(0, input.tellp());
    tide::ids::read(input);
    tide::skip_read(input, false);
    EXPECT_EQ(ue1.size() + ue2.size(), input.tellg());
    EXPECT_EQ(0, input.tellp());
}


TEST(ElementUtils, SkipWrite)
{
    std::stringstream input;

    tide::UIntElement ue1(tide::ids::Null, 0xFFFFFFFF);
    tide::UIntElement ue2(tide::ids::Null, 0xFFFFFFFF);
    tide::UIntElement ue3(tide::ids::Null, 0xFFFFFFFF);
    ue1.write(input);
    ue2.write(input);
    ue3.write(input);
    input.seekp(0);
    input.seekg(0);

    tide::skip_write(input, true);
    EXPECT_EQ(ue1.size(), input.tellp());
    EXPECT_EQ(0, input.tellg());
    input.seekp(tide::ids::size(tide::ids::Null), std::ios::cur);
    tide::skip_write(input, false);
    EXPECT_EQ(ue1.size() + ue2.size(), input.tellp());
    EXPECT_EQ(0, input.tellg());
}

