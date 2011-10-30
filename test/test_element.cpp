/* TIDE
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
 * This file is part of TIDE.
 *
 * TIDE is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * TIDE is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with TIDE. If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtest/gtest.h>
#include <tide/element.h>
#include <tide/exceptions.h>

#include "test_consts.h"


// Fake Element implementation
class FakeElement : public tide::Element
{
    public:
        FakeElement(uint32_t id)
            : Element(id)
        {
        }

        std::streamsize write(std::ostream& output)
        {
            return 0;
        }

        std::streamsize read(std::istream& input)
        {
            return 0;
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


TEST(Element, SetID)
{
    FakeElement e(1234);
    e.id(9999999);
    EXPECT_EQ(9999999, e.id());
    EXPECT_THROW(FakeElement(1).id(0x00), tide::InvalidElementID);
    EXPECT_THROW(FakeElement(1).id(0xFF), tide::InvalidElementID);
    EXPECT_THROW(FakeElement(1).id(0xFFFF), tide::InvalidElementID);
    EXPECT_THROW(FakeElement(1).id(0xFFFFFF), tide::InvalidElementID);
    EXPECT_THROW(FakeElement(1).id(0xFFFFFFFF), tide::InvalidElementID);
}


TEST(Element, Assignment)
{
    FakeElement e1(1), e2(2);
    e2 = e1;
    EXPECT_EQ(e1.id(), e2.id());
}

