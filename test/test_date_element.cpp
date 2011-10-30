/* TIDE
 *
 * Unsigned integer element tests.
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
#include <tide/date_element.h>
#include <tide/exceptions.h>

#include "test_consts.h"

TEST(DateElement, Construction)
{
    EXPECT_EQ(1234, tide::DateElement(1234, 1).id());
    EXPECT_THROW(tide::DateElement(0x00, 1), tide::InvalidElementID);
    EXPECT_THROW(tide::DateElement(0xFF, 1), tide::InvalidElementID);
    EXPECT_THROW(tide::DateElement(0xFFFF, 1), tide::InvalidElementID);
    EXPECT_THROW(tide::DateElement(0xFFFFFF, 1), tide::InvalidElementID);
    EXPECT_THROW(tide::DateElement(0xFFFFFFFF, 1), tide::InvalidElementID);
    // Test with a default as well
    EXPECT_THROW(tide::DateElement(0x00, 1, 1), tide::InvalidElementID);
}


TEST(DateElement, CopyConstruction)
{
    EXPECT_EQ(1234, tide::DateElement(tide::DateElement(1234, 1)).id());
    EXPECT_EQ(1234, tide::DateElement(tide::DateElement(1234, 1, 2)).id());
    EXPECT_EQ(1, tide::DateElement(tide::DateElement(1234, 1, 2)).value());
    EXPECT_EQ(2, tide::DateElement(tide::DateElement(1234, 1, 2)).get_default());
    // The exception actually comes from the inner constructor, but just to be
    // sure it makes it out...
    EXPECT_THROW(tide::DateElement(tide::DateElement(0x00, 1)),
            tide::InvalidElementID);
}


TEST(DateElement, SetID)
{
    tide::DateElement e(1234, 1);
    e.id(9999999);
    EXPECT_EQ(9999999, e.id());
    EXPECT_THROW(tide::DateElement(1, 1).id(0x00), tide::InvalidElementID);
    EXPECT_THROW(tide::DateElement(1, 1).id(0xFF), tide::InvalidElementID);
    EXPECT_THROW(tide::DateElement(1, 1).id(0xFFFF),
            tide::InvalidElementID);
    EXPECT_THROW(tide::DateElement(1, 1).id(0xFFFFFF),
            tide::InvalidElementID);
    EXPECT_THROW(tide::DateElement(1, 1).id(0xFFFFFFFF),
            tide::InvalidElementID);
}


TEST(DateElement, Assignment)
{
    tide::DateElement e1(1, 1), e2(2, 2);
    e2 = e1;
    EXPECT_EQ(e1.value(), e2.value());
    EXPECT_EQ(e1.id(), e2.id());

    tide::DateElement e3(1, 1, 1), e4(2, 2, 2);
    e4 = e3;
    EXPECT_EQ(e1.value(), e2.value());
    EXPECT_EQ(e3.id(), e4.id());
    EXPECT_EQ(e3.has_default(), e4.has_default());
    EXPECT_EQ(e3.get_default(), e4.get_default());

    tide::DateElement e5(1, 1, 1), e6(2, 2);
    e6 = e5;
    EXPECT_EQ(e1.value(), e2.value());
    EXPECT_EQ(e5.id(), e6.id());
    EXPECT_EQ(e5.has_default(), e6.has_default());
    EXPECT_EQ(e5.get_default(), e6.get_default());

    tide::DateElement e7(1, 1), e8(2, 2, 2);
    e8 = e7;
    EXPECT_EQ(e1.value(), e2.value());
    EXPECT_EQ(e7.id(), e8.id());
    EXPECT_EQ(e7.has_default(), e8.has_default());
    EXPECT_EQ(e7.get_default(), e8.get_default());
}


TEST(DateElement, Default)
{
    EXPECT_FALSE(tide::DateElement(1234, 1).has_default());
    EXPECT_TRUE(tide::DateElement(1234, 1, 1).has_default());

    tide::DateElement e1(1234, 1, 1);
    EXPECT_EQ(1, e1.get_default());
    EXPECT_TRUE(e1.has_default());
    e1.remove_default();
    EXPECT_FALSE(e1.has_default());
    e1.set_default(2);
    EXPECT_TRUE(e1.has_default());
    EXPECT_EQ(2, e1.get_default());

    tide::DateElement e2(1234, 1);
    EXPECT_FALSE(e2.has_default());
    e2.set_default(1);
    EXPECT_TRUE(e2.has_default());
    EXPECT_EQ(1, e2.get_default());
    e2.remove_default();
    EXPECT_FALSE(e2.has_default());

    tide::DateElement e3(1234, 1);
    EXPECT_FALSE(e3.is_default());
    e3.set_default(1);
    EXPECT_TRUE(e3.is_default());
    e3.set_default(2);
    EXPECT_FALSE(e3.is_default());
    e3.value(2);
    EXPECT_TRUE(e3.is_default());
}


TEST(DateElement, Value)
{
    EXPECT_EQ(1, tide::DateElement(1234, 1).value());
    EXPECT_EQ(1, tide::DateElement(1234, 1, 2).value());

    tide::DateElement e1(1234, 1);
    EXPECT_EQ(1, e1.value());
    e1.value(2);
    EXPECT_EQ(2, e1.value());

    tide::DateElement e2(1234, 1, 2);
    e2.value(3);
    EXPECT_EQ(3, e2.value());
}

