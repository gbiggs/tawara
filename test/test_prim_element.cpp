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

#include <tide/prim_element.h>

#include <gtest/gtest.h>
#include <tide/exceptions.h>

#include "test_consts.h"


// Fake PrimitiveElement implementation
class FakePrimElement : public tide::PrimitiveElement<int>
{
    public:
        FakePrimElement(uint32_t id)
            : Element(id)
        {
        }

        std::streamsize write_id(std::ostream& output)
        {
            return 0;
        }

        std::streamsize write_body(std::ostream& output)
        {
            return 0;
        }

        std::streamsize read_body(std::istream& input)
        {
            return 0;
        }

        size_t size() const
        {
            return 0;
        }

        size_t total_size() const
        {
            return 0;
        }
}; // class FakePrimElement


TEST(PrimElement, Construction)
{
    EXPECT_EQ(1234, FakePrimElement(1234).id());
    EXPECT_THROW(FakePrimElement(0x00), tide::InvalidElementID);
    EXPECT_THROW(FakePrimElement(0xFF), tide::InvalidElementID);
    EXPECT_THROW(FakePrimElement(0xFFFF), tide::InvalidElementID);
    EXPECT_THROW(FakePrimElement(0xFFFFFF), tide::InvalidElementID);
    EXPECT_THROW(FakePrimElement(0xFFFFFFFF), tide::InvalidElementID);
}


TEST(PrimElement, CopyConstruction)
{
    EXPECT_EQ(1234, FakePrimElement(FakePrimElement(1234, 16)).id());
    EXPECT_EQ(16, FakePrimElement(FakePrimElement(1234, 16)).value());
    EXPECT_EQ(55, FakePrimElement(FakePrimElement(1234, 16, 55)).get_default());
    // The exception actually comes from the inner constructor, but just to be
    // sure it makes it out...
    EXPECT_THROW(FakePrimElement(FakePrimElement(0x00)), tide::InvalidElementID);
}


TEST(PrimElement, SetID)
{
    FakePrimElement e(1234);
    e.id(9999999);
    EXPECT_EQ(9999999, e.id());
    EXPECT_THROW(FakePrimElement(1).id(0x00), tide::InvalidElementID);
    EXPECT_THROW(FakePrimElement(1).id(0xFF), tide::InvalidElementID);
    EXPECT_THROW(FakePrimElement(1).id(0xFFFF), tide::InvalidElementID);
    EXPECT_THROW(FakePrimElement(1).id(0xFFFFFF), tide::InvalidElementID);
    EXPECT_THROW(FakePrimElement(1).id(0xFFFFFFFF), tide::InvalidElementID);
}


TEST(PrimElement, Assignment)
{
    FakePrimElement e1(1, 1), e2(2, 2);
    e2 = e1;
    EXPECT_EQ(e1.id(), e2.id());
    EXPECT_EQ(e1.value(), e2.value());
}

