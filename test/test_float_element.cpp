/* TIDE
 *
 * Float element tests.
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

#include <tide/float_element.h>

#include <gtest/gtest.h>
#include <tide/exceptions.h>
#include <tide/vint.h>

#include "test_consts.h"
#include "test_utils.h"


namespace test_flel
{

size_t fill_buffer(std::basic_string<uint8_t>& b, uint32_t id, double data,
        bool write_id, bool write_body, bool double_prec)
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
        if (double_prec)
        {
            n = tide::vint::encode(8, temp, 8);
            b.append(temp, 0, n);
            memcpy(temp, reinterpret_cast<uint8_t*>(&data), 8);
            b.append(temp, 0, 8);
            total += n + 8;
        }
        else
        {
            n = tide::vint::encode(4, temp, 8);
            b.append(temp, 0, n);
            float data_fl(data);
            memcpy(temp, reinterpret_cast<uint8_t*>(&data_fl), 4);
            b.append(temp, 0, 4);
            total += n + 4;
        }
    }
    return total;
}

}; // namespace test_flel


TEST(FloatElement, Construction)
{
    EXPECT_EQ(1234, tide::FloatElement(1234, 1.0).id());
    EXPECT_THROW(tide::FloatElement(0x00, 1.0), tide::InvalidElementID);
    EXPECT_THROW(tide::FloatElement(0xFF, 1.0), tide::InvalidElementID);
    EXPECT_THROW(tide::FloatElement(0xFFFF, 1.0), tide::InvalidElementID);
    EXPECT_THROW(tide::FloatElement(0xFFFFFF, 1.0), tide::InvalidElementID);
    EXPECT_THROW(tide::FloatElement(0xFFFFFFFF, 1.0), tide::InvalidElementID);
    // Test with a default as well
    EXPECT_THROW(tide::FloatElement(0x00, 1.0, 1.0), tide::InvalidElementID);
}


TEST(FloatElement, CopyConstruction)
{
    EXPECT_EQ(1234, tide::FloatElement(tide::FloatElement(1234, 1.0)).id());
    EXPECT_EQ(1234,
        tide::FloatElement(tide::FloatElement(1234, 1.0, 2.0)).id());
    EXPECT_DOUBLE_EQ(1.0,
        tide::FloatElement(tide::FloatElement(1234, 1.0, 2.0)).value());
    EXPECT_EQ(2.0,
        tide::FloatElement(tide::FloatElement(1234, 1.0, 2.0)).get_default());
    EXPECT_EQ(tide::EBML_FLOAT_PREC_SINGLE,
            tide::FloatElement(tide::FloatElement(1234, 1.0, 2.0,
                    tide::EBML_FLOAT_PREC_SINGLE)).precision());
    // The exception actually comes from the inner constructor, but just to be
    // sure it makes it out...
    EXPECT_THROW(tide::FloatElement(tide::FloatElement(0x00, 1.0)),
            tide::InvalidElementID);
}


TEST(FloatElement, SetID)
{
    tide::FloatElement e(1234, 1.0);
    e.id(9999999);
    EXPECT_EQ(9999999, e.id());
    EXPECT_THROW(tide::FloatElement(1, 1.0).id(0x00), tide::InvalidElementID);
    EXPECT_THROW(tide::FloatElement(1, 1.0).id(0xFF), tide::InvalidElementID);
    EXPECT_THROW(tide::FloatElement(1, 1.0).id(0xFFFF), tide::InvalidElementID);
    EXPECT_THROW(tide::FloatElement(1, 1.0).id(0xFFFFFF),
            tide::InvalidElementID);
    EXPECT_THROW(tide::FloatElement(1, 1.0).id(0xFFFFFFFF),
            tide::InvalidElementID);
}


TEST(FloatElement, Assignment)
{
    tide::FloatElement e1(1, 1.0), e2(2, 2.0);
    e2 = e1;
    EXPECT_DOUBLE_EQ(e1.value(), e2.value());
    EXPECT_EQ(e1.id(), e2.id());
    EXPECT_EQ(e1.has_default(), e2.has_default());
    EXPECT_EQ(e1.get_default(), e2.get_default());
    EXPECT_EQ(e1.precision(), e2.precision());

    tide::FloatElement e3(1, 1.0, 1.0), e4(2, 2.0, 2.0);
    e4 = e3;
    EXPECT_DOUBLE_EQ(e1.value(), e2.value());
    EXPECT_EQ(e3.id(), e4.id());
    EXPECT_EQ(e3.has_default(), e4.has_default());
    EXPECT_EQ(e3.get_default(), e4.get_default());
    EXPECT_EQ(e3.precision(), e4.precision());

    tide::FloatElement e5(1, 1.0, 1.0), e6(2, 2.0);
    e6 = e5;
    EXPECT_DOUBLE_EQ(e1.value(), e2.value());
    EXPECT_EQ(e5.id(), e6.id());
    EXPECT_EQ(e5.has_default(), e6.has_default());
    EXPECT_EQ(e5.get_default(), e6.get_default());
    EXPECT_EQ(e5.precision(), e6.precision());

    tide::FloatElement e7(1, 1.0), e8(2, 2.0, 2.0);
    e8 = e7;
    EXPECT_DOUBLE_EQ(e1.value(), e2.value());
    EXPECT_EQ(e7.id(), e8.id());
    EXPECT_EQ(e7.has_default(), e8.has_default());
    EXPECT_EQ(e7.get_default(), e8.get_default());
    EXPECT_EQ(e7.precision(), e8.precision());

    tide::FloatElement e9(1, 1.0, tide::EBML_FLOAT_PREC_SINGLE),
        e10(2, 2.0, tide::EBML_FLOAT_PREC_DOUBLE);
    e10 = e9;
    EXPECT_DOUBLE_EQ(e9.value(), e10.value());
    EXPECT_EQ(e9.id(), e10.id());
    EXPECT_EQ(e9.has_default(), e10.has_default());
    EXPECT_EQ(e9.get_default(), e10.get_default());
    EXPECT_EQ(e9.precision(), e10.precision());

    double v(-3.14159);
    e8 = v;
    EXPECT_DOUBLE_EQ(v, e8.value());
    e8 = 2.71828e10;
    EXPECT_DOUBLE_EQ(2.71828e10, e8.value());
}


TEST(FloatElement, Default)
{
    EXPECT_FALSE(tide::FloatElement(1234, 1.0).has_default());
    EXPECT_TRUE(tide::FloatElement(1234, 1.0, 1.0).has_default());

    tide::FloatElement e1(1234, 1.0, 1.0);
    EXPECT_EQ(1, e1.get_default());
    EXPECT_TRUE(e1.has_default());
    e1.remove_default();
    EXPECT_FALSE(e1.has_default());
    e1.set_default(2);
    EXPECT_TRUE(e1.has_default());
    EXPECT_EQ(2, e1.get_default());

    tide::FloatElement e2(1234, 1.0);
    EXPECT_FALSE(e2.has_default());
    e2.set_default(1);
    EXPECT_TRUE(e2.has_default());
    EXPECT_EQ(1, e2.get_default());
    e2.remove_default();
    EXPECT_FALSE(e2.has_default());

    tide::FloatElement e3(1234, 1.0);
    EXPECT_FALSE(e3.is_default());
    e3.set_default(1);
    EXPECT_TRUE(e3.is_default());
    e3.set_default(2);
    EXPECT_FALSE(e3.is_default());
    e3.value(2);
    EXPECT_TRUE(e3.is_default());
}


TEST(FloatElement, Value)
{
    EXPECT_DOUBLE_EQ(1.0, tide::FloatElement(1234, 1.0).value());
    EXPECT_DOUBLE_EQ(1.0, tide::FloatElement(1234, 1.0, 2.0).value());

    tide::FloatElement e1(1234, 1.0);
    EXPECT_DOUBLE_EQ(1.0, e1.value());
    e1.value(294.2984e4);
    EXPECT_DOUBLE_EQ(294.2984e4, e1.value());

    tide::FloatElement e2(1234, 1.0, 2.0);
    e2.value(3.03489529);
    EXPECT_DOUBLE_EQ(3.03489529, e2.value());
}


TEST(FloatElement, Precision)
{
    EXPECT_EQ(tide::EBML_FLOAT_PREC_SINGLE,
            tide::FloatElement(1234, 1.0,
                tide::EBML_FLOAT_PREC_SINGLE).precision());
    EXPECT_EQ(tide::EBML_FLOAT_PREC_DOUBLE,
            tide::FloatElement(1234, 1.0,
                tide::EBML_FLOAT_PREC_DOUBLE).precision());

    tide::FloatElement e1(1234, 1.0);
    EXPECT_EQ(tide::EBML_FLOAT_PREC_DOUBLE, e1.precision());
    e1.precision(tide::EBML_FLOAT_PREC_SINGLE);
    EXPECT_EQ(tide::EBML_FLOAT_PREC_SINGLE, e1.precision());

    tide::FloatElement e2(1234, 1.0, 2.0);
    EXPECT_EQ(tide::EBML_FLOAT_PREC_DOUBLE, e2.precision());
    e2.precision(tide::EBML_FLOAT_PREC_SINGLE);
    EXPECT_EQ(tide::EBML_FLOAT_PREC_SINGLE, e2.precision());

    tide::FloatElement e3(1234, 1.0, tide::EBML_FLOAT_PREC_SINGLE);
    EXPECT_EQ(tide::EBML_FLOAT_PREC_SINGLE, e3.precision());
    e3.precision(tide::EBML_FLOAT_PREC_DOUBLE);
    EXPECT_EQ(tide::EBML_FLOAT_PREC_DOUBLE, e3.precision());

    tide::FloatElement e4(1234, 1.0, 2.0, tide::EBML_FLOAT_PREC_SINGLE);
    EXPECT_EQ(tide::EBML_FLOAT_PREC_SINGLE, e4.precision());
    e4.precision(tide::EBML_FLOAT_PREC_DOUBLE);
    EXPECT_EQ(tide::EBML_FLOAT_PREC_DOUBLE, e4.precision());
}


TEST(FloatElement, Write)
{
    std::basic_ostringstream<uint8_t> output;
    std::basic_string<uint8_t> expected;
    double value(2.7182818284590451);

    tide::FloatElement e1(0x01, value, tide::EBML_FLOAT_PREC_DOUBLE);

    test_flel::fill_buffer(expected, 0x01, value, false, true, true);
    EXPECT_EQ(9, e1.write_body(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::basic_string<uint8_t>());
    std::basic_string<uint8_t>().swap(expected);
    test_flel::fill_buffer(expected, 0x01, value, true, false, true);
    EXPECT_EQ(tide::vint::coded_size(1), e1.write_id(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::basic_string<uint8_t>());
    std::basic_string<uint8_t>().swap(expected);
    test_flel::fill_buffer(expected, 0x01, value, true, true, true);
    EXPECT_EQ(tide::vint::coded_size(1) + 9, e1.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    value = 3.14159;
    e1.value(value);
    e1.precision(tide::EBML_FLOAT_PREC_SINGLE);

    test_flel::fill_buffer(expected, 0x01, value, false, true, false);
    EXPECT_EQ(5, e1.write_body(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::basic_string<uint8_t>());
    std::basic_string<uint8_t>().swap(expected);
    test_flel::fill_buffer(expected, 0x01, value, true, false, false);
    EXPECT_EQ(tide::vint::coded_size(1), e1.write_id(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::basic_string<uint8_t>());
    std::basic_string<uint8_t>().swap(expected);
    test_flel::fill_buffer(expected, 0x01, value, true, true, false);
    EXPECT_EQ(tide::vint::coded_size(1) + 5, e1.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
}


TEST(FloatElement, Read)
{
    std::basic_istringstream<uint8_t> input;
    std::basic_string<uint8_t> input_val;
    double value(23.14069);

    tide::FloatElement e(0x01, 0);
    test_flel::fill_buffer(input_val, 0x01, value, false, true, true);
    input.str(input_val);
    EXPECT_EQ(9, e.read_body(input));
    EXPECT_EQ(0x01, e.id());
    EXPECT_DOUBLE_EQ(value, e.value());
    EXPECT_EQ(tide::EBML_FLOAT_PREC_DOUBLE, e.precision());

    value = 22.45916;
    e.value(0);
    e.set_default(0);
    EXPECT_TRUE(e.has_default());
    EXPECT_TRUE(e.is_default());
    std::basic_string<uint8_t>().swap(input_val);
    test_flel::fill_buffer(input_val, 0x01, value, false, true, false);
    input.str(input_val);
    EXPECT_EQ(5, e.read_body(input));
    EXPECT_FLOAT_EQ(value, e.value());
    EXPECT_EQ(0, e.get_default());
    EXPECT_FALSE(e.is_default());
    EXPECT_EQ(tide::EBML_FLOAT_PREC_SINGLE, e.precision());

    // Test for ReadError exception
    std::basic_string<uint8_t>().swap(input_val);
    test_flel::fill_buffer(input_val, 0x01, value, false, true, true);
    input.str(input_val.substr(0, 4));
    EXPECT_THROW(e.read_body(input), tide::ReadError);
    // Test for ReadError exception
    std::basic_string<uint8_t>().swap(input_val);
    test_flel::fill_buffer(input_val, 0x01, value, false, true, true);
    input.str(input_val.substr(0, 4));
    EXPECT_THROW(e.read_body(input), tide::ReadError);
}


TEST(FloatElement, Size)
{
    tide::FloatElement e(1, 1.0, tide::EBML_FLOAT_PREC_SINGLE);
    EXPECT_EQ(4, e.size());
    EXPECT_EQ(6, e.total_size());
    e.precision(tide::EBML_FLOAT_PREC_DOUBLE);
    EXPECT_EQ(8, e.size());
    EXPECT_EQ(10, e.total_size());

    e.precision(tide::EBML_FLOAT_PREC_SINGLE);
    e.id(-70000);
    EXPECT_EQ(4, e.size());
    EXPECT_EQ(10, e.total_size());
    e.precision(tide::EBML_FLOAT_PREC_DOUBLE);
    EXPECT_EQ(8, e.size());
    EXPECT_EQ(14, e.total_size());

    e.precision(tide::EBML_FLOAT_PREC_SINGLE);
    e.value(3.15149);
    EXPECT_EQ(4, e.size());
    EXPECT_EQ(10, e.total_size());
    e.precision(tide::EBML_FLOAT_PREC_DOUBLE);
    EXPECT_EQ(8, e.size());
    EXPECT_EQ(14, e.total_size());
}

