/* TIDE
 *
 * Binary element tests.
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

#include <tide/binary_element.h>

#include <gtest/gtest.h>
#include <string>
#include <tide/exceptions.h>

#include "test_consts.h"
#include "test_utils.h"


namespace test_binel
{

size_t fill_buffer(std::basic_string<uint8_t>& b, uint32_t id,
        std::basic_string<uint8_t> data, bool write_id, bool write_body)
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
    }
    return total;
}

}; // namespace test_binel


class BinaryElementTest : public ::testing::Test
{
    public:
        virtual void SetUp()
        {
            for (uint8_t ii(0); ii < 8; ++ii)
            {
                b1.push_back(ii);
                b2.push_back(0xFF - ii);
            }
            for (uint8_t ii(0); ii < 15; ++ii)
            {
                b3.push_back(0x7F - ii);
                b4.push_back(0x7F + ii);
            }
        }

        std::basic_string<uint8_t> b1;
        std::basic_string<uint8_t> b2;
        std::basic_string<uint8_t> b3;
        std::basic_string<uint8_t> b4;
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
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, b1,
            tide::BinaryElement(tide::BinaryElement(1234, b1, b2)).value());
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, b2,
            tide::BinaryElement(tide::BinaryElement(1234, b1, b2)).get_default());
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
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, e1.value(), e2.value());
    EXPECT_EQ(e1.id(), e2.id());

    tide::BinaryElement e3(1, b1, b1), e4(2, b2, b2);
    e4 = e3;
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, e1.value(), e2.value());
    EXPECT_EQ(e3.id(), e4.id());
    EXPECT_EQ(e3.has_default(), e4.has_default());
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, e3.get_default(), e4.get_default());

    tide::BinaryElement e5(1, b1, b1), e6(2, b2);
    e6 = e5;
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, e1.value(), e2.value());
    EXPECT_EQ(e5.id(), e6.id());
    EXPECT_EQ(e5.has_default(), e6.has_default());
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, e5.get_default(), e6.get_default());

    tide::BinaryElement e7(1, b1), e8(2, b2, b2);
    e8 = e7;
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, e1.value(), e2.value());
    EXPECT_EQ(e7.id(), e8.id());
    EXPECT_EQ(e7.has_default(), e8.has_default());
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, e7.get_default(), e8.get_default());

    e8 = b4;
    EXPECT_EQ(b4, e8.value());
}


TEST_F(BinaryElementTest, Default)
{
    EXPECT_FALSE(tide::BinaryElement(1234, b1).has_default());
    EXPECT_TRUE(tide::BinaryElement(1234, b1, b1).has_default());

    tide::BinaryElement e1(1234, b1, b1);
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, b1, e1.get_default());
    EXPECT_TRUE(e1.has_default());
    e1.remove_default();
    EXPECT_FALSE(e1.has_default());
    e1.set_default(b2);
    EXPECT_TRUE(e1.has_default());
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, b2, e1.get_default());

    tide::BinaryElement e2(1234, b1);
    EXPECT_FALSE(e2.has_default());
    e2.set_default(b1);
    EXPECT_TRUE(e2.has_default());
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, b1, e2.get_default());
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
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, b1,
            tide::BinaryElement(1234, b1).value());
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, b1,
            tide::BinaryElement(1234, b1, b2).value());

    tide::BinaryElement e1(1234, b1);
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, b1, e1.value());
    e1.value(b2);
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, b2, e1.value());

    tide::BinaryElement e2(1234, b1, b2);
    e2.value(b3);
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, b3, e2.value());
}


TEST_F(BinaryElementTest, Write)
{
    std::basic_ostringstream<uint8_t> output;
    std::basic_string<uint8_t> expected;

    tide::BinaryElement e1(0x01, b1);

    test_binel::fill_buffer(expected, 0x01, b1, false, true);
    EXPECT_EQ(1 + b1.size(), e1.write_body(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::basic_string<uint8_t>());
    std::basic_string<uint8_t>().swap(expected);
    test_binel::fill_buffer(expected, 0x01, b1, true, false);
    EXPECT_EQ(tide::vint::coded_size(1), e1.write_id(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::basic_string<uint8_t>());
    std::basic_string<uint8_t>().swap(expected);
    test_binel::fill_buffer(expected, 0x01, b1, true, true);
    EXPECT_EQ(tide::vint::coded_size(1) + 1 + b1.size() + padding,
            e1.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    e1.value(b3);

    test_binel::fill_buffer(expected, 0x01, b3, false, true);
    EXPECT_EQ(1 + b3.size(), e1.write_body(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::basic_string<uint8_t>());
    std::basic_string<uint8_t>().swap(expected);
    test_binel::fill_buffer(expected, 0x01, b3, true, false);
    EXPECT_EQ(tide::vint::coded_size(1), e1.write_id(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::basic_string<uint8_t>());
    std::basic_string<uint8_t>().swap(expected);
    test_binel::fill_buffer(expected, 0x01, b3, true, true);
    EXPECT_EQ(tide::vint::coded_size(1) + 1 + b3.size() + padding,
            e1.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    // Zero-length binary
    std::basic_string<uint8_t> value;
    e1.value(value);

    test_binel::fill_buffer(expected, 0x01, value, false, true);
    EXPECT_EQ(1 + value.size(), e1.write_body(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::basic_string<uint8_t>());
    std::basic_string<uint8_t>().swap(expected);
    test_binel::fill_buffer(expected, 0x01, value, true, false);
    EXPECT_EQ(tide::vint::coded_size(1), e1.write_id(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.str(std::basic_string<uint8_t>());
    std::basic_string<uint8_t>().swap(expected);
    test_binel::fill_buffer(expected, 0x01, value, true, true);
    EXPECT_EQ(tide::vint::coded_size(1) + 1 + value.size(), e1.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
}


TEST_F(BinaryElementTest, Read)
{
    std::basic_istringstream<uint8_t> input;
    std::basic_string<uint8_t> input_val;
    std::string value("12345");
    uint64_t padding(0);

    tide::BinaryElement e(0x01, "");
    test_binel::fill_buffer(input_val, 0x01, value, padding, false, true);
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
    test_binel::fill_buffer(input_val, 0x01, value, padding, false, true);
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
    test_binel::fill_buffer(input_val, 0x01, value, padding, false, true);
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
    test_binel::fill_buffer(input_val, 0x01, value, padding, false, true);
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
    test_binel::fill_buffer(input_val, 0x01, value, padding, false, true);
    input.str(input_val);
    EXPECT_EQ(1 + value.size() + padding, e.read_body(input));
    EXPECT_EQ(value, e.value());
    EXPECT_EQ(padding, e.padding());
    EXPECT_EQ("1", e.get_default());
    EXPECT_FALSE(e.is_default());

    // Test for ReadError exception
    value = "12345";
    std::basic_string<uint8_t>().swap(input_val);
    test_binel::fill_buffer(input_val, 0x01, value, padding, false, true);
    input.str(input_val.substr(0, 4));
    EXPECT_THROW(e.read_body(input), tide::ReadError);
}


TEST_F(BinaryElementTest, Size)
{
    tide::BinaryElement e(1, b1);
    EXPECT_EQ(b1.size(), e.size());
    EXPECT_EQ(2 + b1.size(), e.total_size());

    e.id(0x1234567);
    EXPECT_EQ(b1.size(), e.size());
    EXPECT_EQ(5 + b1.size(), e.total_size());

    e.value(b3);
    EXPECT_EQ(b3.size(), e.size());
    EXPECT_EQ(5 + b3.size(), e.total_size());
}

