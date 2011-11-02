/* TIDE
 *
 * String element tests.
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

#include <tide/string_element.h>

#include <gtest/gtest.h>
#include <tide/exceptions.h>
#include <tide/vint.h>

#include "test_consts.h"
#include "test_utils.h"


namespace test_strel
{

size_t fill_buffer(std::basic_string<uint8_t>& b, uint32_t id,
        std::string data, uint64_t padding, bool write_id, bool write_body)
{
    uint8_t temp[8];
    size_t n(0), total(0);
    if (write_id)
    {
        n = tide::vint::encode(id, temp, 8);
        b.append(temp, 0, n);
        total += n;
    }
    if (write_body)
    {
        n = tide::vint::encode(data.size(), temp, 8);
        b.append(temp, 0, n);
        total += n;
        for (size_t ii(0); ii < data.size(); ++ii)
        {
            b.push_back(data[ii]);
        }
        total += data.size();
        for (size_t ii(0); ii < padding; ++ii)
        {
            b.push_back(0x00);
        }
        total += padding;
    }
    return total;
}

}; // namespace test_strel


TEST(StringElement, Construction)
{
    EXPECT_EQ(1234, tide::StringElement(1234, "string 1").id());
    EXPECT_THROW(tide::StringElement(0x00, "string 1"),
            tide::InvalidElementID);
    EXPECT_THROW(tide::StringElement(0xFF, "string 1"),
            tide::InvalidElementID);
    EXPECT_THROW(tide::StringElement(0xFFFF, "string 1"),
            tide::InvalidElementID);
    EXPECT_THROW(tide::StringElement(0xFFFFFF, "string 1"),
            tide::InvalidElementID);
    EXPECT_THROW(tide::StringElement(0xFFFFFFFF, "string 1"),
            tide::InvalidElementID);
    // Test with a default as well
    EXPECT_THROW(tide::StringElement(0x00, "string 1", "default string"),
            tide::InvalidElementID);
}


TEST(StringElement, CopyConstruction)
{
    EXPECT_EQ(1234, tide::StringElement(tide::StringElement(1234,
                    "string 1")).id());
    EXPECT_EQ(1234, tide::StringElement(tide::StringElement(1234, "string 1",
                    "string 2")).id());
    EXPECT_EQ("string 1", tide::StringElement(tide::StringElement(1234,
                    "string 1", "string 2")).value());
    EXPECT_EQ("string 2", tide::StringElement(tide::StringElement(1234,
                    "string 1", "string 2")).get_default());
    // The exception actually comes from the inner constructor, but just to be
    // sure it makes it out...
    EXPECT_THROW(tide::StringElement(tide::StringElement(0x00, "string 1")),
            tide::InvalidElementID);
}


TEST(StringElement, SetID)
{
    tide::StringElement e(1234, "string 1");
    e.id(9999999);
    EXPECT_EQ(9999999, e.id());
    EXPECT_THROW(tide::StringElement(1, "string 1").id(0x00),
            tide::InvalidElementID);
    EXPECT_THROW(tide::StringElement(1, "string 1").id(0xFF),
            tide::InvalidElementID);
    EXPECT_THROW(tide::StringElement(1, "string 1").id(0xFFFF),
            tide::InvalidElementID);
    EXPECT_THROW(tide::StringElement(1, "string 1").id(0xFFFFFF),
            tide::InvalidElementID);
    EXPECT_THROW(tide::StringElement(1, "string 1").id(0xFFFFFFFF),
            tide::InvalidElementID);
}


TEST(StringElement, Assignment)
{
    tide::StringElement e1(1, "string 1"), e2(2, "string 2");
    e2 = e1;
    EXPECT_EQ(e1.value(), e2.value());
    EXPECT_EQ(e1.id(), e2.id());

    tide::StringElement e3(1, "string 1", "string 1"), e4(2, "string 2",
            "string 2");
    e4 = e3;
    EXPECT_EQ(e1.value(), e2.value());
    EXPECT_EQ(e3.id(), e4.id());
    EXPECT_EQ(e3.has_default(), e4.has_default());
    EXPECT_EQ(e3.get_default(), e4.get_default());

    tide::StringElement e5(1, "string 1", "string 1"), e6(2, "string 2");
    e6 = e5;
    EXPECT_EQ(e1.value(), e2.value());
    EXPECT_EQ(e5.id(), e6.id());
    EXPECT_EQ(e5.has_default(), e6.has_default());
    EXPECT_EQ(e5.get_default(), e6.get_default());

    tide::StringElement e7(1, "string 1"), e8(2, "string 2", "string 2");
    e8 = e7;
    EXPECT_EQ(e1.value(), e2.value());
    EXPECT_EQ(e7.id(), e8.id());
    EXPECT_EQ(e7.has_default(), e8.has_default());
    EXPECT_EQ(e7.get_default(), e8.get_default());

    std::string v("another string");
    e8 = v;
    EXPECT_EQ(v, e8.value());
    e8 = "yet another string";
    EXPECT_EQ("yet another string", e8.value());
}


TEST(StringElement, Default)
{
    EXPECT_FALSE(tide::StringElement(1234, "string 1").has_default());
    EXPECT_TRUE(tide::StringElement(1234, "string 1",
                "string 1").has_default());

    tide::StringElement e1(1234, "string 1", "string 1");
    EXPECT_EQ("string 1", e1.get_default());
    EXPECT_TRUE(e1.has_default());
    e1.remove_default();
    EXPECT_FALSE(e1.has_default());
    e1.set_default("string 2");
    EXPECT_TRUE(e1.has_default());
    EXPECT_EQ("string 2", e1.get_default());

    tide::StringElement e2(1234, "string 1");
    EXPECT_FALSE(e2.has_default());
    e2.set_default("string 1");
    EXPECT_TRUE(e2.has_default());
    EXPECT_EQ("string 1", e2.get_default());
    e2.remove_default();
    EXPECT_FALSE(e2.has_default());

    tide::StringElement e3(1234, "string 1");
    EXPECT_FALSE(e3.is_default());
    e3.set_default("string 1");
    EXPECT_TRUE(e3.is_default());
    e3.set_default("string 2");
    EXPECT_FALSE(e3.is_default());
    e3.value("string 2");
    EXPECT_TRUE(e3.is_default());
}


TEST(StringElement, Value)
{
    EXPECT_EQ("string 1", tide::StringElement(1234, "string 1").value());
    EXPECT_EQ("string 1", tide::StringElement(1234, "string 1",
                "string 2").value());

    tide::StringElement e1(1234, "string 1");
    EXPECT_EQ("string 1", e1.value());
    e1.value("string 2");
    EXPECT_EQ("string 2", e1.value());

    tide::StringElement e2(1234, "string 1", "string 2");
    e2.value("string 3");
    EXPECT_EQ("string 3", e2.value());
}


TEST(StringElement, Padding)
{
    EXPECT_EQ(0, tide::StringElement(1234, "string 1").padding());
    EXPECT_EQ(0, tide::StringElement(1234, "string 1", "string 2").padding());

    tide::StringElement e1(1234, "string 1");
    EXPECT_EQ(0, e1.padding());
    e1.padding(30);
    EXPECT_EQ(30, e1.padding());
}


TEST(StringElement, Write)
{
    std::basic_ostringstream<uint8_t> output;
    std::basic_string<uint8_t> expected;
    std::string value("12345");
    uint64_t padding(0);

    tide::StringElement e1(0x01, value);
    e1.padding(padding);

    test_strel::fill_buffer(expected, 0x01, value, padding, false, true);
    EXPECT_EQ(1 + value.size() + padding, e1.write_body(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::basic_string<uint8_t>());
    std::basic_string<uint8_t>().swap(expected);
    test_strel::fill_buffer(expected, 0x01, value, padding, true, false);
    EXPECT_EQ(tide::vint::coded_size(1), e1.write_id(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::basic_string<uint8_t>());
    std::basic_string<uint8_t>().swap(expected);
    test_strel::fill_buffer(expected, 0x01, value, padding, true, true);
    EXPECT_EQ(tide::vint::coded_size(1) + 1 + value.size() + padding,
            e1.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    value = "1234567890";
    padding = 5;
    e1.value(value);
    e1.padding(padding);

    test_strel::fill_buffer(expected, 0x01, value, padding, false, true);
    EXPECT_EQ(1 + value.size() + padding, e1.write_body(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::basic_string<uint8_t>());
    std::basic_string<uint8_t>().swap(expected);
    test_strel::fill_buffer(expected, 0x01, value, padding, true, false);
    EXPECT_EQ(tide::vint::coded_size(1), e1.write_id(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::basic_string<uint8_t>());
    std::basic_string<uint8_t>().swap(expected);
    test_strel::fill_buffer(expected, 0x01, value, padding, true, true);
    EXPECT_EQ(tide::vint::coded_size(1) + 1 + value.size() + padding,
            e1.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    // Zero-length string
    value = "";
    padding = 0;
    e1.value(value);
    e1.padding(padding);

    test_strel::fill_buffer(expected, 0x01, value, padding, false, true);
    EXPECT_EQ(1 + value.size() + padding, e1.write_body(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::basic_string<uint8_t>());
    std::basic_string<uint8_t>().swap(expected);
    test_strel::fill_buffer(expected, 0x01, value, padding, true, false);
    EXPECT_EQ(tide::vint::coded_size(1), e1.write_id(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::basic_string<uint8_t>());
    std::basic_string<uint8_t>().swap(expected);
    test_strel::fill_buffer(expected, 0x01, value, padding, true, true);
    EXPECT_EQ(tide::vint::coded_size(1) + 1 + value.size() + padding,
            e1.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    // Zero-length string with padding
    value = "";
    padding = 5;
    e1.value(value);
    e1.padding(padding);

    test_strel::fill_buffer(expected, 0x01, value, padding, false, true);
    EXPECT_EQ(1 + value.size() + padding, e1.write_body(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::basic_string<uint8_t>());
    std::basic_string<uint8_t>().swap(expected);
    test_strel::fill_buffer(expected, 0x01, value, padding, true, false);
    EXPECT_EQ(tide::vint::coded_size(1), e1.write_id(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::basic_string<uint8_t>());
    std::basic_string<uint8_t>().swap(expected);
    test_strel::fill_buffer(expected, 0x01, value, padding, true, true);
    EXPECT_EQ(tide::vint::coded_size(1) + 1 + value.size() + padding,
            e1.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
}


TEST(StringElement, Read)
{
    std::basic_istringstream<uint8_t> input;
    std::basic_string<uint8_t> input_val;
    std::string value("12345");
    uint64_t padding(0);

    tide::StringElement e(0x01, "");
    test_strel::fill_buffer(input_val, 0x01, value, padding, false, true);
    input.str(input_val);
    EXPECT_EQ(1 + value.size() + padding, e.read_body(input));
    EXPECT_EQ(0x01, e.id());
    EXPECT_EQ(value, e.value());
    EXPECT_EQ(padding, e.padding());

    // String without padding
    value = "1234567890";
    padding = 0;
    e.value("1");
    e.padding(0);
    e.set_default("1");
    EXPECT_TRUE(e.has_default());
    EXPECT_TRUE(e.is_default());
    std::basic_string<uint8_t>().swap(input_val);
    test_strel::fill_buffer(input_val, 0x01, value, padding, false, true);
    input.str(input_val);
    EXPECT_EQ(1 + value.size() + padding, e.read_body(input));
    EXPECT_EQ(value, e.value());
    EXPECT_EQ(padding, e.padding());
    EXPECT_EQ("1", e.get_default());
    EXPECT_FALSE(e.is_default());

    // String with padding
    value = "1234567890";
    padding = 5;
    e.value("1");
    e.padding(0);
    e.set_default("1");
    EXPECT_TRUE(e.has_default());
    EXPECT_TRUE(e.is_default());
    std::basic_string<uint8_t>().swap(input_val);
    test_strel::fill_buffer(input_val, 0x01, value, padding, false, true);
    input.str(input_val);
    EXPECT_EQ(1 + value.size() + padding, e.read_body(input));
    EXPECT_EQ(value, e.value());
    EXPECT_EQ(padding, e.padding());
    EXPECT_EQ("1", e.get_default());
    EXPECT_FALSE(e.is_default());

    // Zero-length string
    value = "";
    padding = 0;
    e.value("1");
    e.padding(0);
    e.set_default("1");
    EXPECT_TRUE(e.has_default());
    EXPECT_TRUE(e.is_default());
    std::basic_string<uint8_t>().swap(input_val);
    test_strel::fill_buffer(input_val, 0x01, value, padding, false, true);
    input.str(input_val);
    EXPECT_EQ(1 + value.size() + padding, e.read_body(input));
    EXPECT_EQ(value, e.value());
    EXPECT_EQ(padding, e.padding());
    EXPECT_EQ("1", e.get_default());
    EXPECT_FALSE(e.is_default());

    // Zero-length string with padding
    value = "";
    padding = 5;
    e.value("1");
    e.padding(0);
    e.set_default("1");
    EXPECT_TRUE(e.has_default());
    EXPECT_TRUE(e.is_default());
    std::basic_string<uint8_t>().swap(input_val);
    test_strel::fill_buffer(input_val, 0x01, value, padding, false, true);
    input.str(input_val);
    EXPECT_EQ(1 + value.size() + padding, e.read_body(input));
    EXPECT_EQ(value, e.value());
    EXPECT_EQ(padding, e.padding());
    EXPECT_EQ("1", e.get_default());
    EXPECT_FALSE(e.is_default());

    // Test for ReadError exception
    value = "12345";
    std::basic_string<uint8_t>().swap(input_val);
    test_strel::fill_buffer(input_val, 0x01, value, padding, false, true);
    input.str(input_val.substr(0, 4));
    EXPECT_THROW(e.read_body(input), tide::ReadError);
}


TEST(StringElement, Size)
{
    tide::StringElement e(1, "12345");
    EXPECT_EQ(5, e.size());
    EXPECT_EQ(7, e.total_size());

    e.id(0x1234567);
    EXPECT_EQ(5, e.size());
    EXPECT_EQ(10, e.total_size());

    e.value("1234567890");
    EXPECT_EQ(10, e.size());
    EXPECT_EQ(15, e.total_size());

    e.padding(5);
    EXPECT_EQ(15, e.size());
    EXPECT_EQ(20, e.total_size());
}

