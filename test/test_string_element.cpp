/*
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2011, 2012, Geoffrey Biggs, geoffrey.biggs@aist.go.jp
 *     RT-Synthesis Research Group
 *     Intelligent Systems Research Institute,
 *     National Institute of Advanced Industrial Science and Technology (AIST),
 *     Japan
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Geoffrey Biggs nor AIST, nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#include <tide/string_element.h>

#include <gtest/gtest.h>
#include <tide/el_ids.h>
#include <tide/ebml_int.h>
#include <tide/exceptions.h>
#include <tide/vint.h>

#include "test_consts.h"
#include "test_utils.h"


namespace test_strel
{

std::streamsize fill_buffer(std::string& b, tide::ids::ID id, std::string data,
        uint64_t padding, bool write_id, bool write_size, bool write_body)
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
        std::vector<char> tmp(tide::vint::encode(data.size() + padding));
        b.append(&tmp[0], 0, tmp.size());
        total += tmp.size();
    }
    if (write_body)
    {
        b.append(data);
        total += data.size();
        for (unsigned int ii(0); ii < padding; ++ii)
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
    EXPECT_EQ(tide::ids::Null, tide::StringElement(tide::ids::Null, "string 1").id());
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
    EXPECT_EQ(tide::ids::Null, tide::StringElement(tide::StringElement(tide::ids::Null,
                    "string 1")).id());
    EXPECT_EQ(tide::ids::Null, tide::StringElement(tide::StringElement(tide::ids::Null, "string 1",
                    "string 2")).id());
    EXPECT_EQ("string 1", tide::StringElement(tide::StringElement(tide::ids::Null,
                    "string 1", "string 2")).value());
    EXPECT_EQ("string 2", tide::StringElement(tide::StringElement(tide::ids::Null,
                    "string 1", "string 2")).get_default());
    // The exception actually comes from the inner constructor, but just to be
    // sure it makes it out...
    EXPECT_THROW(tide::StringElement(tide::StringElement(0x00, "string 1")),
            tide::InvalidElementID);
}


TEST(StringElement, SetID)
{
    tide::StringElement e(tide::ids::Null, "string 1");
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
    EXPECT_FALSE(tide::StringElement(tide::ids::Null, "string 1").has_default());
    EXPECT_TRUE(tide::StringElement(tide::ids::Null, "string 1",
                "string 1").has_default());

    tide::StringElement e1(tide::ids::Null, "string 1", "string 1");
    EXPECT_EQ("string 1", e1.get_default());
    EXPECT_TRUE(e1.has_default());
    e1.remove_default();
    EXPECT_FALSE(e1.has_default());
    e1.set_default("string 2");
    EXPECT_TRUE(e1.has_default());
    EXPECT_EQ("string 2", e1.get_default());

    tide::StringElement e2(tide::ids::Null, "string 1");
    EXPECT_FALSE(e2.has_default());
    e2.set_default("string 1");
    EXPECT_TRUE(e2.has_default());
    EXPECT_EQ("string 1", e2.get_default());
    e2.remove_default();
    EXPECT_FALSE(e2.has_default());

    tide::StringElement e3(tide::ids::Null, "string 1");
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
    EXPECT_EQ("string 1", tide::StringElement(tide::ids::Null, "string 1").value());
    EXPECT_EQ("string 1", tide::StringElement(tide::ids::Null, "string 1",
                "string 2").value());

    tide::StringElement e1(tide::ids::Null, "string 1");
    EXPECT_EQ("string 1", e1.value());
    e1.value("string 2");
    EXPECT_EQ("string 2", e1.value());

    tide::StringElement e2(tide::ids::Null, "string 1", "string 2");
    e2.value("string 3");
    EXPECT_EQ("string 3", e2.value());
}


TEST(StringElement, Padding)
{
    EXPECT_EQ(0, tide::StringElement(tide::ids::Null, "string 1").padding());
    EXPECT_EQ(0, tide::StringElement(tide::ids::Null, "string 1", "string 2").padding());

    tide::StringElement e1(tide::ids::Null, "string 1");
    EXPECT_EQ(0, e1.padding());
    e1.padding(30);
    EXPECT_EQ(30, e1.padding());
}


TEST(StringElement, Equality)
{
    tide::StringElement e1(tide::ids::Null, "string 1");
    tide::StringElement e2(tide::ids::Null, "string 1");

    EXPECT_TRUE(e1 == e2);
    e2.value("string 2");
    EXPECT_TRUE(e1 != e2);
}


TEST(StringElement, Write)
{
    std::ostringstream output;
    std::string expected;
    std::string value("tide::ids::Null5");
    uint64_t padding(0);

    tide::StringElement e1(tide::ids::Null, value);
    e1.padding(padding);
    test_strel::fill_buffer(expected, tide::ids::Null, value, padding, true, true, true);
    EXPECT_EQ(tide::ids::size(tide::ids::Null) + 1 + value.size() + padding,
            e1.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    value = "1234567890";
    padding = 5;
    e1.value(value);
    e1.padding(padding);
    output.str(std::string());
    std::string().swap(expected);
    test_strel::fill_buffer(expected, tide::ids::Null, value, padding, true,
            true, true);
    EXPECT_EQ(tide::ids::size(tide::ids::Null) + 1 + value.size() +
            padding, e1.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    // Zero-length string
    value = "";
    padding = 0;
    e1.value(value);
    e1.padding(padding);
    output.str(std::string());
    std::string().swap(expected);
    test_strel::fill_buffer(expected, tide::ids::Null, value, padding, true,
            true, true);
    EXPECT_EQ(tide::ids::size(tide::ids::Null) + 1 + value.size() +
            padding, e1.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    // Zero-length string with padding
    value = "";
    padding = 5;
    e1.value(value);
    e1.padding(padding);
    output.str(std::string());
    std::string().swap(expected);
    test_strel::fill_buffer(expected, tide::ids::Null, value, padding, true,
            true, true);
    EXPECT_EQ(tide::ids::size(tide::ids::Null) + 1 + value.size() +
            padding, e1.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
}


TEST(StringElement, Read)
{
    std::istringstream input;
    std::string input_val;
    std::string value("tide::ids::Null5");
    uint64_t padding(0);

    tide::StringElement e(0x80, "");
    test_strel::fill_buffer(input_val, 0x80, value, padding, false, true, true);
    input.str(input_val);
    EXPECT_EQ(1 + value.size() + padding, e.read(input));
    EXPECT_EQ(0x80, e.id());
    EXPECT_EQ(value, e.value());
    EXPECT_EQ(padding, e.padding());

    // String without padding
    value = "tide::ids::Null567890";
    padding = 0;
    std::string value_padded(value);
    value_padded.append(padding, '\0');
    e.value("1");
    e.padding(0);
    e.set_default("1");
    EXPECT_TRUE(e.has_default());
    EXPECT_TRUE(e.is_default());
    std::string().swap(input_val);
    test_strel::fill_buffer(input_val, 0x80, value, padding, false, true, true);
    input.str(input_val);
    EXPECT_EQ(1 + value.size() + padding, e.read(input));
    EXPECT_EQ(value_padded, e.value());
    EXPECT_EQ(padding, e.padding());
    EXPECT_EQ("1", e.get_default());
    EXPECT_FALSE(e.is_default());

    // String with padding
    value = "tide::ids::Null567890";
    padding = 5;
    value_padded = value;
    value_padded.append(padding, '\0');
    e.value("1");
    e.padding(0);
    e.set_default("1");
    EXPECT_TRUE(e.has_default());
    EXPECT_TRUE(e.is_default());
    std::string().swap(input_val);
    test_strel::fill_buffer(input_val, 0x80, value, padding, false, true, true);
    input.str(input_val);
    EXPECT_EQ(1 + value.size() + padding, e.read(input));
    EXPECT_EQ(value_padded, e.value());
    EXPECT_EQ(0, e.padding());
    EXPECT_EQ("1", e.get_default());
    EXPECT_FALSE(e.is_default());

    // Zero-length string
    value = "";
    padding = 0;
    value_padded = value;
    value_padded.append(padding, '\0');
    e.value("1");
    e.padding(0);
    e.set_default("1");
    EXPECT_TRUE(e.has_default());
    EXPECT_TRUE(e.is_default());
    std::string().swap(input_val);
    test_strel::fill_buffer(input_val, 0x80, value, padding, false, true, true);
    input.str(input_val);
    EXPECT_EQ(1 + value.size() + padding, e.read(input));
    EXPECT_EQ(value_padded, e.value());
    EXPECT_EQ(0, e.padding());
    EXPECT_EQ("1", e.get_default());
    EXPECT_FALSE(e.is_default());

    // Zero-length string with padding
    value = "";
    padding = 5;
    value_padded = value;
    value_padded.append(padding, '\0');
    e.value("1");
    e.padding(0);
    e.set_default("1");
    EXPECT_TRUE(e.has_default());
    EXPECT_TRUE(e.is_default());
    std::string().swap(input_val);
    test_strel::fill_buffer(input_val, 0x80, value, padding, false, true, true);
    input.str(input_val);
    EXPECT_EQ(1 + value.size() + padding, e.read(input));
    EXPECT_EQ(value_padded, e.value());
    EXPECT_EQ(0, e.padding());
    EXPECT_EQ("1", e.get_default());
    EXPECT_FALSE(e.is_default());

    // Test for ReadError exception
    value = "tide::ids::Null5";
    std::string().swap(input_val);
    test_strel::fill_buffer(input_val, 0x80, value, padding, false, true, true);
    input.str(input_val.substr(0, 4));
    EXPECT_THROW(e.read(input), tide::ReadError);
}


TEST(StringElement, Size)
{
    tide::StringElement e(0x80, "12345");
    EXPECT_EQ(7, e.size());

    e.value("1234567890");
    EXPECT_EQ(12, e.size());

    e.padding(5);
    EXPECT_EQ(17, e.size());
}

