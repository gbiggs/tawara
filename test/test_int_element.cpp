/* TIDE
 *
 * Signed integer element tests.
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
#include <tide/ebml_int.h>
#include <tide/exceptions.h>
#include <tide/int_element.h>
#include <tide/vint.h>

#include "test_consts.h"
#include "test_utils.h"


namespace test_intel
{

size_t fill_buffer(std::basic_string<uint8_t>& b, uint32_t id, int64_t data,
        bool write_id, bool write_body)
{
    uint8_t temp[8];
    size_t n(0), size(0), total(0);
    if (write_id)
    {
        n = tide::vint::encode(id, temp, 8);
        b.append(temp, 0, n);
        total += n;
    }
    if (write_body)
    {
        size = tide::ebml_int::coded_size_s(data);
        n = tide::vint::encode(size, temp, 8);
        b.append(temp, 0, n);
        total += n;
        n = tide::ebml_int::encode_s(data, temp, 8);
        b.append(temp, 0, n);
        total += n;
    }
    return total;
}

}; // namespace test_intel


TEST(IntElement, Construction)
{
    EXPECT_EQ(1234, tide::IntElement(1234, 1).id());
    EXPECT_THROW(tide::IntElement(0x00, 1), tide::InvalidElementID);
    EXPECT_THROW(tide::IntElement(0xFF, 1), tide::InvalidElementID);
    EXPECT_THROW(tide::IntElement(0xFFFF, 1), tide::InvalidElementID);
    EXPECT_THROW(tide::IntElement(0xFFFFFF, 1), tide::InvalidElementID);
    EXPECT_THROW(tide::IntElement(0xFFFFFFFF, 1), tide::InvalidElementID);
    // Test with a default as well
    EXPECT_THROW(tide::IntElement(0x00, 1, 1), tide::InvalidElementID);
}


TEST(IntElement, CopyConstruction)
{
    EXPECT_EQ(1234, tide::IntElement(tide::IntElement(1234, 1)).id());
    EXPECT_EQ(1234, tide::IntElement(tide::IntElement(1234, 1, 2)).id());
    EXPECT_EQ(1, tide::IntElement(tide::IntElement(1234, 1, 2)).value());
    EXPECT_EQ(2, tide::IntElement(tide::IntElement(1234, 1, 2)).get_default());
    // The exception actually comes from the inner constructor, but just to be
    // sure it makes it out...
    EXPECT_THROW(tide::IntElement(tide::IntElement(0x00, 1)),
            tide::InvalidElementID);
}


TEST(IntElement, SetID)
{
    tide::IntElement e(1234, 1);
    e.id(9999999);
    EXPECT_EQ(9999999, e.id());
    EXPECT_THROW(tide::IntElement(1, 1).id(0x00), tide::InvalidElementID);
    EXPECT_THROW(tide::IntElement(1, 1).id(0xFF), tide::InvalidElementID);
    EXPECT_THROW(tide::IntElement(1, 1).id(0xFFFF),
            tide::InvalidElementID);
    EXPECT_THROW(tide::IntElement(1, 1).id(0xFFFFFF),
            tide::InvalidElementID);
    EXPECT_THROW(tide::IntElement(1, 1).id(0xFFFFFFFF),
            tide::InvalidElementID);
}


TEST(IntElement, Assignment)
{
    tide::IntElement e1(1, 1), e2(2, 2);
    e2 = e1;
    EXPECT_EQ(e1.value(), e2.value());
    EXPECT_EQ(e1.id(), e2.id());

    tide::IntElement e3(1, 1, 1), e4(2, 2, 2);
    e4 = e3;
    EXPECT_EQ(e3.value(), e4.value());
    EXPECT_EQ(e3.id(), e4.id());
    EXPECT_EQ(e3.has_default(), e4.has_default());
    EXPECT_EQ(e3.get_default(), e4.get_default());

    tide::IntElement e5(1, 1, 1), e6(2, 2);
    e6 = e5;
    EXPECT_EQ(e5.value(), e6.value());
    EXPECT_EQ(e5.id(), e6.id());
    EXPECT_EQ(e5.has_default(), e6.has_default());
    EXPECT_EQ(e5.get_default(), e6.get_default());

    tide::IntElement e7(1, 1), e8(2, 2, 2);
    e8 = e7;
    EXPECT_EQ(e7.value(), e8.value());
    EXPECT_EQ(e7.id(), e8.id());
    EXPECT_EQ(e7.has_default(), e8.has_default());
    EXPECT_EQ(e7.get_default(), e8.get_default());

    int v(-15000);
    e8 = v;
    EXPECT_EQ(v, e8.value());
    e8 = 320000;
    EXPECT_EQ(320000, e8.value());
}


TEST(IntElement, Default)
{
    EXPECT_FALSE(tide::IntElement(1234, 1).has_default());
    EXPECT_TRUE(tide::IntElement(1234, 1, 1).has_default());

    tide::IntElement e1(1234, 1, 1);
    EXPECT_EQ(1, e1.get_default());
    EXPECT_TRUE(e1.has_default());
    e1.remove_default();
    EXPECT_FALSE(e1.has_default());
    e1.set_default(2);
    EXPECT_TRUE(e1.has_default());
    EXPECT_EQ(2, e1.get_default());

    tide::IntElement e2(1234, 1);
    EXPECT_FALSE(e2.has_default());
    e2.set_default(1);
    EXPECT_TRUE(e2.has_default());
    EXPECT_EQ(1, e2.get_default());
    e2.remove_default();
    EXPECT_FALSE(e2.has_default());

    tide::IntElement e3(1234, 1);
    EXPECT_FALSE(e3.is_default());
    e3.set_default(1);
    EXPECT_TRUE(e3.is_default());
    e3.set_default(2);
    EXPECT_FALSE(e3.is_default());
    e3.value(2);
    EXPECT_TRUE(e3.is_default());
}


TEST(IntElement, Value)
{
    EXPECT_EQ(1, tide::IntElement(1234, 1).value());
    EXPECT_EQ(1, tide::IntElement(1234, 1, 2).value());

    tide::IntElement e1(1234, 1);
    EXPECT_EQ(1, e1.value());
    e1.value(2);
    EXPECT_EQ(2, e1.value());

    tide::IntElement e2(1234, 1, 2);
    e2.value(3);
    EXPECT_EQ(3, e2.value());
}


TEST(IntElement, Write)
{
    std::basic_ostringstream<uint8_t> output;
    std::basic_string<uint8_t> expected;
    int64_t value(2);
    size_t val_size(tide::ebml_int::coded_size_s(value));

    tide::IntElement e1(0x01, value);

    test_intel::fill_buffer(expected, 0x01, value, false, true);
    EXPECT_EQ(tide::vint::coded_size(val_size) + val_size,
            e1.write_body(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::basic_string<uint8_t>());
    std::basic_string<uint8_t>().swap(expected);
    test_intel::fill_buffer(expected, 0x01, value, true, false);
    EXPECT_EQ(tide::vint::coded_size(1), e1.write_id(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::basic_string<uint8_t>());
    std::basic_string<uint8_t>().swap(expected);
    test_intel::fill_buffer(expected, 0x01, value, true, true);
    EXPECT_EQ(tide::vint::coded_size(1) + tide::vint::coded_size(val_size) + val_size,
            e1.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    value = -0x839F18AAl;
    val_size = tide::ebml_int::coded_size_s(value);
    e1.value(value);

    test_intel::fill_buffer(expected, 0x01, value, false, true);
    EXPECT_EQ(tide::vint::coded_size(val_size) + val_size,
            e1.write_body(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::basic_string<uint8_t>());
    std::basic_string<uint8_t>().swap(expected);
    test_intel::fill_buffer(expected, 0x01, value, true, false);
    EXPECT_EQ(tide::vint::coded_size(1), e1.write_id(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::basic_string<uint8_t>());
    std::basic_string<uint8_t>().swap(expected);
    test_intel::fill_buffer(expected, 0x01, value, true, true);
    EXPECT_EQ(tide::vint::coded_size(1) + tide::vint::coded_size(val_size) + val_size,
            e1.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
}


TEST(IntElement, Read)
{
    std::basic_istringstream<uint8_t> input;
    std::basic_string<uint8_t> input_val;
    int64_t value(5);
    size_t val_size(tide::ebml_int::coded_size_s(value));

    tide::IntElement e(0x01, 0);
    test_intel::fill_buffer(input_val, 0x01, value, false, true);
    input.str(input_val);
    EXPECT_EQ(tide::vint::coded_size(val_size) + val_size, e.read_body(input));
    EXPECT_EQ(0x01, e.id());
    EXPECT_EQ(value, e.value());

    value = 0x3A958BCD99l;
    val_size = tide::ebml_int::coded_size_s(value);
    e.value(0);
    e.set_default(0);
    EXPECT_TRUE(e.has_default());
    EXPECT_TRUE(e.is_default());
    std::basic_string<uint8_t>().swap(input_val);
    test_intel::fill_buffer(input_val, 0x01, value, false, true);
    input.str(input_val);
    EXPECT_EQ(tide::vint::coded_size(val_size) + val_size, e.read_body(input));
    EXPECT_EQ(value, e.value());
    EXPECT_EQ(0, e.get_default());
    EXPECT_FALSE(e.is_default());

    // Test for ReadError exception
    std::basic_string<uint8_t>().swap(input_val);
    test_intel::fill_buffer(input_val, 0x01, value, false, true);
    input.str(input_val.substr(0, 4));
    EXPECT_THROW(e.read_body(input), tide::ReadError);
}


TEST(IntElement, Size)
{
    tide::IntElement e(1, 1);
    EXPECT_EQ(1, e.size());
    EXPECT_EQ(3, e.total_size());

    e.id(-70000);
    EXPECT_EQ(1, e.size());
    EXPECT_EQ(7, e.total_size());

    e.value(0x7FFFFF);
    EXPECT_EQ(3, e.size());
    EXPECT_EQ(9, e.total_size());

    e.value(0xFFFFFF);
    EXPECT_EQ(4, e.size());
    EXPECT_EQ(10, e.total_size());
}

