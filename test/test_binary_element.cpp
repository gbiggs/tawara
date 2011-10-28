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

#include "test_consts.h"

#include <gtest/gtest.h>
#include <string>
#include <tide/prim_element.h>
#include <tide/exceptions.h>


::testing::AssertionResult std_buffers_eq(char const* b1_expr,
        char const* b2_expr, char const* n_expr,
        std::basic_string<uint8_t> const& b1,
        std::basic_string<uint8_t> const& b2, size_t n)
{
    if (strncmp(reinterpret_cast<char const*>(b1.c_str()),
            reinterpret_cast<char const*>(b2.c_str()), n) == 0)
    {
        return ::testing::AssertionSuccess();
    }
    else
    {
        std::stringstream b1_str;
        b1_str << std::hex;
        b1_str.width(2);
        b1_str.fill('0');
        for (size_t ii(0); ii < n; ++ii)
        {
            b1_str << static_cast<int>(b1[ii]);
        }
        std::stringstream b2_str;
        b2_str << std::hex;
        b2_str.width(2);
        b2_str.fill('0');
        for (size_t ii(0); ii < n; ++ii)
        {
            b2_str << static_cast<int>(b2[ii]);
        }
        return ::testing::AssertionFailure() << b1_expr << ": 0x" <<
            b1_str.str() << '\t' << b2_expr << ": 0x" << b2_str.str();
    }
}


class BinaryElementTest : public ::testing::Test
{
    public:
        virtual void SetUp()
        {
            for (uint8_t ii(0); ii < 8; ++ii)
            {
                b1.push_back(ii);
                b2.push_back(0xFF - ii);
                b3.push_back(0x7F - ii);
            }
        }

        std::basic_string<uint8_t> b1;
        std::basic_string<uint8_t> b2;
        std::basic_string<uint8_t> b3;
}; // class TestBinaryElement


TEST_F(BinaryElementTest, Construction)
{
    EXPECT_EQ(1234, tide::BinaryElement(1234, b1).id());
    EXPECT_THROW(tide::BinaryElement(0x00, b1), tide::InvalidElementID);
    EXPECT_THROW(tide::BinaryElement(0xFF, b1), tide::InvalidElementID);
    EXPECT_THROW(tide::BinaryElement(0xFFFF, b1), tide::InvalidElementID);
    EXPECT_THROW(tide::BinaryElement(0xFFFFFF, b1), tide::InvalidElementID);
    EXPECT_THROW(tide::BinaryElement(0xFFFFFFFF, b1), tide::InvalidElementID);
    // Test with a default as well
    EXPECT_THROW(tide::BinaryElement(0x00, b1, b1), tide::InvalidElementID);
}


TEST_F(BinaryElementTest, CopyConstruction)
{
    EXPECT_EQ(1234, tide::BinaryElement(tide::BinaryElement(1234, b1)).id());
    EXPECT_EQ(1234, tide::BinaryElement(tide::BinaryElement(1234, b1, b2)).id());
    EXPECT_PRED_FORMAT3(std_buffers_eq, b1,
            tide::BinaryElement(tide::BinaryElement(1234, b1, b2)).value(), 8);
    EXPECT_PRED_FORMAT3(std_buffers_eq, b2,
            tide::BinaryElement(tide::BinaryElement(1234, b1, b2)).get_default(),
            8);
    // The exception actually comes from the inner constructor, but just to be
    // sure it makes it out...
    EXPECT_THROW(tide::BinaryElement(tide::BinaryElement(0x00, b1)),
            tide::InvalidElementID);
}


TEST_F(BinaryElementTest, SetID)
{
    tide::BinaryElement e(1234, b1);
    e.id(9999999);
    EXPECT_EQ(9999999, e.id());
    EXPECT_THROW(tide::BinaryElement(1, b1).id(0x00), tide::InvalidElementID);
    EXPECT_THROW(tide::BinaryElement(1, b1).id(0xFF), tide::InvalidElementID);
    EXPECT_THROW(tide::BinaryElement(1, b1).id(0xFFFF),
            tide::InvalidElementID);
    EXPECT_THROW(tide::BinaryElement(1, b1).id(0xFFFFFF),
            tide::InvalidElementID);
    EXPECT_THROW(tide::BinaryElement(1, b1).id(0xFFFFFFFF),
            tide::InvalidElementID);
}


TEST_F(BinaryElementTest, Assignment)
{
    tide::BinaryElement e1(1, b1), e2(2, b2);
    e2 = e1;
    EXPECT_PRED_FORMAT3(std_buffers_eq, e1.value(), e2.value(), 8);
    EXPECT_EQ(e1.id(), e2.id());

    tide::BinaryElement e3(1, b1, b1), e4(2, b2, b2);
    e4 = e3;
    EXPECT_PRED_FORMAT3(std_buffers_eq, e1.value(), e2.value(), 8);
    EXPECT_EQ(e3.id(), e4.id());
    EXPECT_EQ(e3.has_default(), e4.has_default());
    EXPECT_PRED_FORMAT3(std_buffers_eq, e3.get_default(), e4.get_default(), 8);

    tide::BinaryElement e5(1, b1, b1), e6(2, b2);
    e6 = e5;
    EXPECT_PRED_FORMAT3(std_buffers_eq, e1.value(), e2.value(), 8);
    EXPECT_EQ(e5.id(), e6.id());
    EXPECT_EQ(e5.has_default(), e6.has_default());
    EXPECT_PRED_FORMAT3(std_buffers_eq, e5.get_default(), e6.get_default(), 8);

    tide::BinaryElement e7(1, b1), e8(2, b2, b2);
    e8 = e7;
    EXPECT_PRED_FORMAT3(std_buffers_eq, e1.value(), e2.value(), 8);
    EXPECT_EQ(e7.id(), e8.id());
    EXPECT_EQ(e7.has_default(), e8.has_default());
    EXPECT_PRED_FORMAT3(std_buffers_eq, e7.get_default(), e8.get_default(), 8);
}


TEST_F(BinaryElementTest, Default)
{
    EXPECT_FALSE(tide::BinaryElement(1234, b1).has_default());
    EXPECT_TRUE(tide::BinaryElement(1234, b1, b1).has_default());

    tide::BinaryElement e1(1234, b1, b1);
    EXPECT_PRED_FORMAT3(std_buffers_eq, b1, e1.get_default(), 8);
    EXPECT_TRUE(e1.has_default());
    e1.remove_default();
    EXPECT_FALSE(e1.has_default());
    e1.set_default(b2);
    EXPECT_TRUE(e1.has_default());
    EXPECT_PRED_FORMAT3(std_buffers_eq, b2, e1.get_default(), 8);

    tide::BinaryElement e2(1234, b1);
    EXPECT_FALSE(e2.has_default());
    e2.set_default(b1);
    EXPECT_TRUE(e2.has_default());
    EXPECT_PRED_FORMAT3(std_buffers_eq, b1, e2.get_default(), 8);
    e2.remove_default();
    EXPECT_FALSE(e2.has_default());

    tide::BinaryElement e3(1234, b1);
    EXPECT_FALSE(e3.is_default());
    e3.set_default(b1);
    EXPECT_TRUE(e3.is_default());
    e3.set_default(b2);
    EXPECT_FALSE(e3.is_default());
    e3.value(b2);
    EXPECT_TRUE(e3.is_default());
}


TEST_F(BinaryElementTest, Value)
{
    EXPECT_PRED_FORMAT3(std_buffers_eq, b1, tide::BinaryElement(1234, b1).value(), 8);
    EXPECT_PRED_FORMAT3(std_buffers_eq, b1, tide::BinaryElement(1234, b1, b2).value(), 8);

    tide::BinaryElement e1(1234, b1);
    EXPECT_PRED_FORMAT3(std_buffers_eq, b1, e1.value(), 8);
    e1.value(b2);
    EXPECT_PRED_FORMAT3(std_buffers_eq, b2, e1.value(), 8);

    tide::BinaryElement e2(1234, b1, b2);
    e2.value(b3);
    EXPECT_PRED_FORMAT3(std_buffers_eq, b3, e2.value(), 8);
}

