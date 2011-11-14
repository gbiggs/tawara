/* Tide
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

#include <tide/uint_element.h>

#include <gtest/gtest.h>
#include <tide/ebml_int.h>
#include <tide/el_ids.h>
#include <tide/exceptions.h>
#include <tide/vint.h>

#include "test_consts.h"
#include "test_utils.h"


namespace test_uintel
{

std::streamsize fill_buffer(std::string& b, tide::ids::ID id, uint64_t data,
        bool write_id, bool write_size, bool write_body)
{
    std::streamsize size(0), total(0);
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
        size = tide::ebml_int::coded_size_u(data);
        std::vector<char> tmp(tide::vint::encode(size));
        b.append(&tmp[0], 0, tmp.size());
        total += tmp.size();
    }
    if (write_body)
    {
        std::vector<char> tmp(tide::ebml_int::encode_u(data));
        b.append(&tmp[0], 0, tmp.size());
        total += tmp.size();
    }
    return total;
}

}; // namespace test_uintel


TEST(UIntElement, Construction)
{
    EXPECT_EQ(1234, tide::UIntElement(1234, 1).id());
    EXPECT_THROW(tide::UIntElement(0x00, 1), tide::InvalidElementID);
    EXPECT_THROW(tide::UIntElement(0xFF, 1), tide::InvalidElementID);
    EXPECT_THROW(tide::UIntElement(0xFFFF, 1), tide::InvalidElementID);
    EXPECT_THROW(tide::UIntElement(0xFFFFFF, 1), tide::InvalidElementID);
    EXPECT_THROW(tide::UIntElement(0xFFFFFFFF, 1), tide::InvalidElementID);
    // Test with a default as well
    EXPECT_THROW(tide::UIntElement(0x00, 1, 1), tide::InvalidElementID);
}


TEST(UIntElement, CopyConstruction)
{
    EXPECT_EQ(1234, tide::UIntElement(tide::UIntElement(1234, 1)).id());
    EXPECT_EQ(1234, tide::UIntElement(tide::UIntElement(1234, 1, 1)).id());
    EXPECT_EQ(1, tide::UIntElement(tide::UIntElement(1234, 1, 2)).value());
    EXPECT_EQ(2, tide::UIntElement(tide::UIntElement(1234, 1, 2)).get_default());
    // The exception actually comes from the inner constructor, but just to be
    // sure it makes it out...
    EXPECT_THROW(tide::UIntElement(tide::UIntElement(0x00, 1)),
            tide::InvalidElementID);
}


TEST(UIntElement, SetID)
{
    tide::UIntElement e(1234, 1);
    e.id(9999999);
    EXPECT_EQ(9999999, e.id());
    EXPECT_THROW(tide::UIntElement(1, 1).id(0x00), tide::InvalidElementID);
    EXPECT_THROW(tide::UIntElement(1, 1).id(0xFF), tide::InvalidElementID);
    EXPECT_THROW(tide::UIntElement(1, 1).id(0xFFFF),
            tide::InvalidElementID);
    EXPECT_THROW(tide::UIntElement(1, 1).id(0xFFFFFF),
            tide::InvalidElementID);
    EXPECT_THROW(tide::UIntElement(1, 1).id(0xFFFFFFFF),
            tide::InvalidElementID);
}


TEST(UIntElement, Assignment)
{
    tide::UIntElement e1(1, 1), e2(2, 2);
    e2 = e1;
    EXPECT_EQ(e1.value(), e2.value());
    EXPECT_EQ(e1.id(), e2.id());

    tide::UIntElement e3(1, 1, 1), e4(2, 2, 2);
    e4 = e3;
    EXPECT_EQ(e3.value(), e4.value());
    EXPECT_EQ(e3.id(), e4.id());
    EXPECT_EQ(e3.has_default(), e4.has_default());
    EXPECT_EQ(e3.get_default(), e4.get_default());

    tide::UIntElement e5(1, 1, 1), e6(2, 2);
    e6 = e5;
    EXPECT_EQ(e5.value(), e6.value());
    EXPECT_EQ(e5.id(), e6.id());
    EXPECT_EQ(e5.has_default(), e6.has_default());
    EXPECT_EQ(e5.get_default(), e6.get_default());

    tide::UIntElement e7(1, 1), e8(2, 2, 2);
    e8 = e7;
    EXPECT_EQ(e7.value(), e8.value());
    EXPECT_EQ(e7.id(), e8.id());
    EXPECT_EQ(e7.has_default(), e8.has_default());
    EXPECT_EQ(e7.get_default(), e8.get_default());

    unsigned int v(15000);
    e8 = v;
    EXPECT_EQ(v, e8.value());
    e8 = 320000;
    EXPECT_EQ(320000, e8.value());
}


TEST(UIntElement, Default)
{
    EXPECT_FALSE(tide::UIntElement(1234, 1).has_default());
    EXPECT_TRUE(tide::UIntElement(1234, 1, 1).has_default());

    tide::UIntElement e1(1234, 1, 1);
    EXPECT_EQ(1, e1.get_default());
    EXPECT_TRUE(e1.has_default());
    e1.remove_default();
    EXPECT_FALSE(e1.has_default());
    e1.set_default(2);
    EXPECT_TRUE(e1.has_default());
    EXPECT_EQ(2, e1.get_default());

    tide::UIntElement e2(1234, 1);
    EXPECT_FALSE(e2.has_default());
    e2.set_default(1);
    EXPECT_TRUE(e2.has_default());
    EXPECT_EQ(1, e2.get_default());
    e2.remove_default();
    EXPECT_FALSE(e2.has_default());

    tide::UIntElement e3(1234, 1);
    EXPECT_FALSE(e3.is_default());
    e3.set_default(1);
    EXPECT_TRUE(e3.is_default());
    e3.set_default(2);
    EXPECT_FALSE(e3.is_default());
    e3.value(2);
    EXPECT_TRUE(e3.is_default());
}


TEST(UIntElement, Value)
{
    EXPECT_EQ(1, tide::UIntElement(1234, 1).value());
    EXPECT_EQ(1, tide::UIntElement(1234, 1, 2).value());

    tide::UIntElement e1(1234, 1);
    EXPECT_EQ(1, e1.value());
    e1.value(2);
    EXPECT_EQ(2, e1.value());

    tide::UIntElement e2(1234, 1, 2);
    e2.value(3);
    EXPECT_EQ(3, e2.value());
}


TEST(UIntElement, Write)
{
    std::ostringstream output;
    std::string expected;
    int64_t value(2);
    std::streamsize val_size(tide::ebml_int::coded_size_u(value));

    tide::UIntElement e1(0x80, value);

    test_uintel::fill_buffer(expected, 0x80, value, false, false, true);
    EXPECT_EQ(val_size, e1.write_body(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::string());
    std::string().swap(expected);
    test_uintel::fill_buffer(expected, 0x80, value, false, true, false);
    EXPECT_EQ(tide::vint::coded_size(val_size), e1.write_size(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::string());
    std::string().swap(expected);
    test_uintel::fill_buffer(expected, 0x80, value, true, false, false);
    EXPECT_EQ(tide::ids::coded_size(0x80), e1.write_id(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::string());
    std::string().swap(expected);
    test_uintel::fill_buffer(expected, 0x80, value, true, true, true);
    EXPECT_EQ(tide::ids::coded_size(0x80) + tide::vint::coded_size(val_size) +
            val_size, e1.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    value = 0x839F18AAl;
    val_size = tide::ebml_int::coded_size_u(value);
    e1.value(value);

    test_uintel::fill_buffer(expected, 0x80, value, false, false, true);
    EXPECT_EQ(val_size, e1.write_body(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::string());
    std::string().swap(expected);
    test_uintel::fill_buffer(expected, 0x80, value, false, true, false);
    EXPECT_EQ(tide::vint::coded_size(val_size), e1.write_size(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::string());
    std::string().swap(expected);
    test_uintel::fill_buffer(expected, 0x80, value, true, false, false);
    EXPECT_EQ(tide::ids::coded_size(0x80), e1.write_id(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::string());
    std::string().swap(expected);
    test_uintel::fill_buffer(expected, 0x80, value, true, true, true);
    EXPECT_EQ(tide::ids::coded_size(0x80) + tide::vint::coded_size(val_size) +
            val_size, e1.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
}


TEST(UIntElement, Read)
{
    std::istringstream input;
    std::string input_val;
    int64_t value(5);
    std::streamsize val_size(tide::ebml_int::coded_size_u(value));

    tide::UIntElement e(0x01, 0);
    test_uintel::fill_buffer(input_val, 0x01, value, false, true, true);
    input.str(input_val);
    EXPECT_EQ(tide::vint::coded_size(val_size) + val_size, e.read_body(input));
    EXPECT_EQ(0x01, e.id());
    EXPECT_EQ(value, e.value());

    value = 0x3A958BCD99l;
    val_size = tide::ebml_int::coded_size_u(value);
    e.value(0);
    e.set_default(0);
    EXPECT_TRUE(e.has_default());
    EXPECT_TRUE(e.is_default());
    std::string().swap(input_val);
    test_uintel::fill_buffer(input_val, 0x01, value, false, true, true);
    input.str(input_val);
    EXPECT_EQ(tide::vint::coded_size(val_size) + val_size, e.read_body(input));
    EXPECT_EQ(value, e.value());
    EXPECT_EQ(0, e.get_default());
    EXPECT_FALSE(e.is_default());

    // Test for ReadError exception
    std::string().swap(input_val);
    test_uintel::fill_buffer(input_val, 0x01, value, false, true, true);
    input.str(input_val.substr(0, 4));
    EXPECT_THROW(e.read_body(input), tide::ReadError);
}


TEST(UIntElement, Size)
{
    tide::UIntElement e(0x80, 1);
    EXPECT_EQ(1, e.size());
    EXPECT_EQ(3, e.total_size());

    e.value(0x7FFFFF);
    EXPECT_EQ(3, e.size());
    EXPECT_EQ(5, e.total_size());

    e.value(0xFFFFFF);
    EXPECT_EQ(3, e.size());
    EXPECT_EQ(5, e.total_size());
}

