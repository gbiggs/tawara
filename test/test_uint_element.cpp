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

TEST(Element, Construction)
{
    EXPECT_EQ(1234, tide::Element(1234).get_id());
    EXPECT_THROW(tide::Element(0x00), tide::InvalidElementID);
    EXPECT_THROW(tide::Element(0xFF), tide::InvalidElementID);
    EXPECT_THROW(tide::Element(0xFFFF), tide::InvalidElementID);
    EXPECT_THROW(tide::Element(0xFFFFFF), tide::InvalidElementID);
    EXPECT_THROW(tide::Element(0xFFFFFFFF), tide::InvalidElementID);
}


TEST(Element, CopyConstruction)
{
    EXPECT_EQ(1234, tide::Element(tide::Element(1234)).get_id());
    // The exception actually comes from the inner constructor, but just to be
    // sure it makes it out...
    EXPECT_THROW(tide::Element(tide::Element(0x00)), tide::InvalidElementID);
}


TEST(Element, SetID)
{
    tide::Element e(1234);
    e.set_id(9999999);
    EXPECT_EQ(9999999, e.get_id());
    EXPECT_THROW(tide::Element(1).set_id(0x00), tide::InvalidElementID);
    EXPECT_THROW(tide::Element(1).set_id(0xFF), tide::InvalidElementID);
    EXPECT_THROW(tide::Element(1).set_id(0xFFFF), tide::InvalidElementID);
    EXPECT_THROW(tide::Element(1).set_id(0xFFFFFF), tide::InvalidElementID);
    EXPECT_THROW(tide::Element(1).set_id(0xFFFFFFFF), tide::InvalidElementID);
}


TEST(Element, Assignment)
{
    tide::Element e1(1), e2(2);
    e2 = e1;
    EXPECT_EQ(e1.get_id(), e2.get_id());
}

