/* Tide
 *
 * Date element tests.
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

#include <tide/date_element.h>

#include <gtest/gtest.h>
#include <tide/el_ids.h>
#include <tide/ebml_int.h>
#include <tide/exceptions.h>
#include <tide/vint.h>

#include "test_consts.h"
#include "test_utils.h"


namespace test_datel
{

std::streamsize fill_buffer(std::string& b, tide::ids::ID id, int64_t data,
        bool write_id, bool write_size, bool write_body)
{
    std::streamsize total(0);
    if (write_id)
    {
        // Cheating on the IDs a bit - there is no protection here against
        // invalid IDs
        std::vector<char> tmp(tide::ebml_int::encode_u(id));
        b.append(&tmp[0], 0, tmp.size());
        total += tmp.size();
    }
    if (write_size)
    {
        std::vector<char> tmp(tide::vint::encode(8));
        b.append(&tmp[0], 0, tmp.size());
        total += tmp.size();
    }
    if (write_body)
    {
        for (std::streamsize ii(0); ii < 8; ++ii)
        {
            b.push_back(reinterpret_cast<uint8_t*>(&data)[ii]);
        }
        total += 8;
    }
    return total;
}

}; // namespace test_datel


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

    int v(-15000);
    e8 = v;
    EXPECT_EQ(v, e8.value());
    e8 = 320000;
    EXPECT_EQ(320000, e8.value());
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
    EXPECT_EQ(1, tide::DateElement(1234, 1));
    EXPECT_EQ(1, tide::DateElement(1234, 1, 2).value());
    EXPECT_EQ(1, tide::DateElement(1234, 1, 2));

    tide::DateElement e1(1234, 1);
    EXPECT_EQ(1, e1.value());
    e1.value(2);
    EXPECT_EQ(2, e1.value());

    tide::DateElement e2(1234, 1, 2);
    e2.value(3);
    EXPECT_EQ(3, e2.value());
}


TEST(DateElement, Write)
{
    std::ostringstream output;
    std::string expected;
    int64_t value(2);

    tide::DateElement e1(0x80, value);

    test_datel::fill_buffer(expected, 0x80, value, false, false, true);
    EXPECT_EQ(8, e1.write_body(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::string());
    std::string().swap(expected);
    test_datel::fill_buffer(expected, 0x80, value, false, true, false);
    EXPECT_EQ(1, e1.write_size(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::string());
    std::string().swap(expected);
    test_datel::fill_buffer(expected, 0x80, value, true, false, false);
    EXPECT_EQ(tide::ids::coded_size(0x80), e1.write_id(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::string());
    std::string().swap(expected);
    test_datel::fill_buffer(expected, 0x80, value, true, true, true);
    EXPECT_EQ(tide::ids::coded_size(0x80) + 1 + 8, e1.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    value = -0x839F18AAl;
    e1.value(value);

    test_datel::fill_buffer(expected, 0x80, value, false, false, true);
    EXPECT_EQ(8, e1.write_body(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::string());
    std::string().swap(expected);
    test_datel::fill_buffer(expected, 0x80, value, false, true, false);
    EXPECT_EQ(1, e1.write_size(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::string());
    std::string().swap(expected);
    test_datel::fill_buffer(expected, 0x80, value, true, false, false);
    EXPECT_EQ(tide::ids::coded_size(0x80), e1.write_id(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::string());
    std::string().swap(expected);
    test_datel::fill_buffer(expected, 0x80, value, true, true, true);
    EXPECT_EQ(tide::ids::coded_size(0x80) + 1 + 8, e1.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
}


TEST(DateElement, Read)
{
    std::istringstream input;
    std::string input_val;
    int64_t value(5);

    tide::DateElement e(0x80, 0);
    test_datel::fill_buffer(input_val, 0x80, value, false, true, true);
    input.str(input_val);
    EXPECT_EQ(1 + 8, e.read_body(input));
    EXPECT_EQ(0x80, e.id());
    EXPECT_EQ(value, e.value());

    value = 0x3A958BCD99l;
    e.value(0);
    e.set_default(0);
    EXPECT_TRUE(e.has_default());
    EXPECT_TRUE(e.is_default());
    std::string().swap(input_val);
    test_datel::fill_buffer(input_val, 0x80, value, false, true, true);
    input.str(input_val);
    EXPECT_EQ(1 + 8, e.read_body(input));
    EXPECT_EQ(value, e.value());
    EXPECT_EQ(0, e.get_default());
    EXPECT_FALSE(e.is_default());

    // Test for BadElementLength exception
    std::string().swap(input_val);
    test_datel::fill_buffer(input_val, 0x80, value, false, true, true);
    input_val[0] = 0x05;
    input.str(input_val);
    EXPECT_THROW(e.read_body(input), tide::BadElementLength);
    // Test for ReadError exception
    std::string().swap(input_val);
    test_datel::fill_buffer(input_val, 0x80, value, false, true, true);
    input.str(input_val.substr(0, 4));
    EXPECT_THROW(e.read_body(input), tide::ReadError);
}


TEST(DateElement, Size)
{
    tide::DateElement e(0x80, 1);
    EXPECT_EQ(8, e.size());
    EXPECT_EQ(10, e.total_size());

    e.value(0x7FFFFF);
    EXPECT_EQ(8, e.size());
    EXPECT_EQ(10, e.total_size());

    e.value(0xFFFFFF);
    EXPECT_EQ(8, e.size());
    EXPECT_EQ(10, e.total_size());
}

