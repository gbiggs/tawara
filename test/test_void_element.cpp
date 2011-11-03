/* TIDE
 *
 * Void element tests.
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

#include <tide/void_element.h>

#include <gtest/gtest.h>
#include <tide/exceptions.h>
#include <tide/string_element.h>

#include "test_consts.h"


TEST(VoidElement, Construction)
{
    tide::VoidElement ve1(8);
    EXPECT_EQ(0xEC, ve1.id());
    EXPECT_EQ(8, ve1.size());
    EXPECT_FALSE(ve1.fill());

    tide::VoidElement ve2(8, true);
    EXPECT_EQ(0xEC, ve2.id());
    EXPECT_EQ(8, ve2.size());
    EXPECT_TRUE(ve2.fill());

    tide::VoidElement ve3(8, false);
    EXPECT_EQ(0xEC, ve3.id());
    EXPECT_EQ(8, ve3.size());
    EXPECT_FALSE(ve3.fill());
}


TEST(VoidElement, CopyConstruction)
{
    tide::VoidElement ve1(8);
    EXPECT_EQ(0xEC, tide::VoidElement(ve1).id());
    EXPECT_EQ(8, tide::VoidElement(ve1).size());
    EXPECT_FALSE(tide::VoidElement(ve1).fill());

    tide::VoidElement ve2(8, true);
    EXPECT_EQ(0xEC, tide::VoidElement(ve2).id());
    EXPECT_EQ(8, tide::VoidElement(ve2).size());
    EXPECT_TRUE(tide::VoidElement(ve2).fill());

    tide::VoidElement ve3(8, false);
    EXPECT_EQ(0xEC, tide::VoidElement(ve3).id());
    EXPECT_EQ(8, tide::VoidElement(ve3).size());
    EXPECT_FALSE(tide::VoidElement(ve3).fill());
}


TEST(VoidElement, CopyElement)
{
    tide::StringElement se(1, "12345");

    size_t se_size(se.total_size());
    VoidElement ve1(se);
    EXPECT_EQ(se_size, ve1.total_size());
    EXPECT_EQ(se_size - 1, ve1.size());
    EXPECT_FALSE(ve1.fill());

    VoidElement ve2(se, true);
    EXPECT_EQ(se_size, ve2.total_size());
    EXPECT_EQ(se_size - 1, ve2.size());
    EXPECT_TRUE(ve2.fill());
}


TEST(VoidElement, Assignment)
{
    tide::VoidElement e1(8), e2(16);
    e2 = e1;
    EXPECT_EQ(e1.id(), e2.id());
    EXPECT_EQ(e1.size(), e2.size());
    EXPECT_EQ(e1.fill(), e2.fill());

    tide::VoidElement e3(8, true), e4(16, false);
    e3 = e4;
    EXPECT_EQ(e3.id(), e4.id());
    EXPECT_EQ(e3.size(), e4.size());
    EXPECT_EQ(e3.fill(), e4.fill());

    tide::VoidElement e5(8, false), e6(16, true);
    e5 = e6;
    EXPECT_EQ(e5.id(), e6.id());
    EXPECT_EQ(e5.size(), e6.size());
    EXPECT_EQ(e5.fill(), e6.fill());
}


TEST(VoidElement, Size)
{
    tide::VoidElement e1(8);
    EXPECT_EQ(8, e1.size());
    EXPECT_EQ(10, e1.total_size());
    e1.size(16000);
    EXPECT_EQ(16000, e1.size());
    EXPECT_EQ(16003, e1.total_size());
    e1.size(4);
    EXPECT_EQ(4, e1.size());
    EXPECT_EQ(6, e1.total_size());
}


TEST(VoidElement, Fill)
{
    tide::VoidElement e1(8);
    EXPECT_FALSE(e1.fill());
    e1.fill(true);
    EXPECT_TRUE(e1.fill());
    e1.fill(false);
    EXPECT_FALSE(e1.fill());

    tide::VoidElement e2(8, true);
    EXPECT_TRUE(e2.fill());
    e2.fill(false);
    EXPECT_FALSE(e2.fill());

    tide::VoidElement e3(8, false);
    EXPECT_FALSE(e3.fill());
    e3.fill(true);
    EXPECT_TRUE(e3.fill());
}


TEST(VoidElement, Write)
{
    std::stringstream output;
}


TEST(VoidElement, Read)
{
}

