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

#include <jonen/float_element.h>

#include <gtest/gtest.h>
#include <jonen/el_ids.h>
#include <jonen/ebml_int.h>
#include <jonen/exceptions.h>
#include <jonen/vint.h>

#include "test_consts.h"
#include "test_utils.h"


namespace test_flel
{

std::streamsize fill_buffer(std::string& b, jonen::ids::ID id, double data,
        bool write_id, bool write_size, bool write_body, bool double_prec)
{
    char temp[8];
    std::streamsize total(0);
    if (write_id)
    {
        // Cheating on the IDs a bit - there is no protection here against
        // invalid IDs
        std::vector<char> tmp(jonen::ebml_int::encode_u(id));
        b.append(&tmp[0], 0, tmp.size());
        total += tmp.size();
    }
    if (write_size)
    {
        if (double_prec)
        {
            std::vector<char> tmp(jonen::vint::encode(8));
            b.append(&tmp[0], 0, tmp.size());
            total += tmp.size();
        }
        else
        {
            std::vector<char> tmp(jonen::vint::encode(4));
            b.append(&tmp[0], 0, tmp.size());
            total += tmp.size();
        }
    }
    if (write_body)
    {
        if (double_prec)
        {
            memcpy(temp, reinterpret_cast<char*>(&data), 8);
            b.append(temp, 0, 8);
            total += 8;
        }
        else
        {
            float data_fl(data);
            memcpy(temp, reinterpret_cast<char*>(&data_fl), 4);
            b.append(temp, 0, 4);
            total += 4;
        }
    }
    return total;
}

}; // namespace test_flel


TEST(FloatElement, Construction)
{
    EXPECT_EQ(jonen::ids::Null, jonen::FloatElement(jonen::ids::Null, 1.0).id());
    EXPECT_THROW(jonen::FloatElement(0x00, 1.0), jonen::InvalidElementID);
    EXPECT_THROW(jonen::FloatElement(0xFF, 1.0), jonen::InvalidElementID);
    EXPECT_THROW(jonen::FloatElement(0xFFFF, 1.0), jonen::InvalidElementID);
    EXPECT_THROW(jonen::FloatElement(0xFFFFFF, 1.0), jonen::InvalidElementID);
    EXPECT_THROW(jonen::FloatElement(0xFFFFFFFF, 1.0), jonen::InvalidElementID);
    // Test with a default as well
    EXPECT_THROW(jonen::FloatElement(0x00, 1.0, 1.0), jonen::InvalidElementID);
}


TEST(FloatElement, CopyConstruction)
{
    EXPECT_EQ(jonen::ids::Null, jonen::FloatElement(jonen::FloatElement(jonen::ids::Null, 1.0)).id());
    EXPECT_EQ(jonen::ids::Null,
        jonen::FloatElement(jonen::FloatElement(jonen::ids::Null, 1.0, 2.0)).id());
    EXPECT_DOUBLE_EQ(1.0,
        jonen::FloatElement(jonen::FloatElement(jonen::ids::Null, 1.0, 2.0)).value());
    EXPECT_EQ(2.0,
        jonen::FloatElement(jonen::FloatElement(jonen::ids::Null, 1.0, 2.0)).get_default());
    EXPECT_EQ(jonen::EBML_FLOAT_PREC_SINGLE,
            jonen::FloatElement(jonen::FloatElement(jonen::ids::Null, 1.0, 2.0,
                    jonen::EBML_FLOAT_PREC_SINGLE)).precision());
    // The exception actually comes from the inner constructor, but just to be
    // sure it makes it out...
    EXPECT_THROW(jonen::FloatElement(jonen::FloatElement(0x00, 1.0)),
            jonen::InvalidElementID);
}


TEST(FloatElement, SetID)
{
    jonen::FloatElement e(jonen::ids::Null, 1.0);
    e.id(9999999);
    EXPECT_EQ(9999999, e.id());
    EXPECT_THROW(jonen::FloatElement(1, 1.0).id(0x00), jonen::InvalidElementID);
    EXPECT_THROW(jonen::FloatElement(1, 1.0).id(0xFF), jonen::InvalidElementID);
    EXPECT_THROW(jonen::FloatElement(1, 1.0).id(0xFFFF), jonen::InvalidElementID);
    EXPECT_THROW(jonen::FloatElement(1, 1.0).id(0xFFFFFF),
            jonen::InvalidElementID);
    EXPECT_THROW(jonen::FloatElement(1, 1.0).id(0xFFFFFFFF),
            jonen::InvalidElementID);
}


TEST(FloatElement, Assignment)
{
    jonen::FloatElement e1(1, 1.0), e2(2, 2.0);
    e2 = e1;
    EXPECT_DOUBLE_EQ(e1.value(), e2.value());
    EXPECT_EQ(e1.id(), e2.id());
    EXPECT_EQ(e1.has_default(), e2.has_default());
    EXPECT_EQ(e1.get_default(), e2.get_default());
    EXPECT_EQ(e1.precision(), e2.precision());

    jonen::FloatElement e3(1, 1.0, 1.0), e4(2, 2.0, 2.0);
    e4 = e3;
    EXPECT_DOUBLE_EQ(e1.value(), e2.value());
    EXPECT_EQ(e3.id(), e4.id());
    EXPECT_EQ(e3.has_default(), e4.has_default());
    EXPECT_EQ(e3.get_default(), e4.get_default());
    EXPECT_EQ(e3.precision(), e4.precision());

    jonen::FloatElement e5(1, 1.0, 1.0), e6(2, 2.0);
    e6 = e5;
    EXPECT_DOUBLE_EQ(e1.value(), e2.value());
    EXPECT_EQ(e5.id(), e6.id());
    EXPECT_EQ(e5.has_default(), e6.has_default());
    EXPECT_EQ(e5.get_default(), e6.get_default());
    EXPECT_EQ(e5.precision(), e6.precision());

    jonen::FloatElement e7(1, 1.0), e8(2, 2.0, 2.0);
    e8 = e7;
    EXPECT_DOUBLE_EQ(e1.value(), e2.value());
    EXPECT_EQ(e7.id(), e8.id());
    EXPECT_EQ(e7.has_default(), e8.has_default());
    EXPECT_EQ(e7.get_default(), e8.get_default());
    EXPECT_EQ(e7.precision(), e8.precision());

    jonen::FloatElement e9(1, 1.0, jonen::EBML_FLOAT_PREC_SINGLE),
        e10(2, 2.0, jonen::EBML_FLOAT_PREC_DOUBLE);
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
    EXPECT_FALSE(jonen::FloatElement(jonen::ids::Null, 1.0).has_default());
    EXPECT_TRUE(jonen::FloatElement(jonen::ids::Null, 1.0, 1.0).has_default());

    jonen::FloatElement e1(jonen::ids::Null, 1.0, 1.0);
    EXPECT_EQ(1, e1.get_default());
    EXPECT_TRUE(e1.has_default());
    e1.remove_default();
    EXPECT_FALSE(e1.has_default());
    e1.set_default(2);
    EXPECT_TRUE(e1.has_default());
    EXPECT_EQ(2, e1.get_default());

    jonen::FloatElement e2(jonen::ids::Null, 1.0);
    EXPECT_FALSE(e2.has_default());
    e2.set_default(1);
    EXPECT_TRUE(e2.has_default());
    EXPECT_EQ(1, e2.get_default());
    e2.remove_default();
    EXPECT_FALSE(e2.has_default());

    jonen::FloatElement e3(jonen::ids::Null, 1.0);
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
    EXPECT_DOUBLE_EQ(1.0, jonen::FloatElement(jonen::ids::Null, 1.0).value());
    EXPECT_DOUBLE_EQ(1.0, jonen::FloatElement(jonen::ids::Null, 1.0));
    EXPECT_DOUBLE_EQ(1.0, jonen::FloatElement(jonen::ids::Null, 1.0, 2.0).value());
    EXPECT_DOUBLE_EQ(1.0, jonen::FloatElement(jonen::ids::Null, 1.0, 2.0));

    jonen::FloatElement e1(jonen::ids::Null, 1.0);
    EXPECT_DOUBLE_EQ(1.0, e1.value());
    e1.value(294.2984e4);
    EXPECT_DOUBLE_EQ(294.2984e4, e1.value());

    jonen::FloatElement e2(jonen::ids::Null, 1.0, 2.0);
    e2.value(3.03489529);
    EXPECT_DOUBLE_EQ(3.03489529, e2.value());
}


TEST(FloatElement, Precision)
{
    EXPECT_EQ(jonen::EBML_FLOAT_PREC_SINGLE,
            jonen::FloatElement(jonen::ids::Null, 1.0,
                jonen::EBML_FLOAT_PREC_SINGLE).precision());
    EXPECT_EQ(jonen::EBML_FLOAT_PREC_DOUBLE,
            jonen::FloatElement(jonen::ids::Null, 1.0,
                jonen::EBML_FLOAT_PREC_DOUBLE).precision());

    jonen::FloatElement e1(jonen::ids::Null, 1.0);
    EXPECT_EQ(jonen::EBML_FLOAT_PREC_DOUBLE, e1.precision());
    e1.precision(jonen::EBML_FLOAT_PREC_SINGLE);
    EXPECT_EQ(jonen::EBML_FLOAT_PREC_SINGLE, e1.precision());

    jonen::FloatElement e2(jonen::ids::Null, 1.0, 2.0);
    EXPECT_EQ(jonen::EBML_FLOAT_PREC_DOUBLE, e2.precision());
    e2.precision(jonen::EBML_FLOAT_PREC_SINGLE);
    EXPECT_EQ(jonen::EBML_FLOAT_PREC_SINGLE, e2.precision());

    jonen::FloatElement e3(jonen::ids::Null, 1.0, jonen::EBML_FLOAT_PREC_SINGLE);
    EXPECT_EQ(jonen::EBML_FLOAT_PREC_SINGLE, e3.precision());
    e3.precision(jonen::EBML_FLOAT_PREC_DOUBLE);
    EXPECT_EQ(jonen::EBML_FLOAT_PREC_DOUBLE, e3.precision());

    jonen::FloatElement e4(jonen::ids::Null, 1.0, 2.0, jonen::EBML_FLOAT_PREC_SINGLE);
    EXPECT_EQ(jonen::EBML_FLOAT_PREC_SINGLE, e4.precision());
    e4.precision(jonen::EBML_FLOAT_PREC_DOUBLE);
    EXPECT_EQ(jonen::EBML_FLOAT_PREC_DOUBLE, e4.precision());
}


TEST(FloatElement, Equality)
{
    jonen::FloatElement e1(jonen::ids::Null, 1.0);
    jonen::FloatElement e2(jonen::ids::Null, 1.0);

    EXPECT_TRUE(e1 == e2);
    e2.value(2.0);
    EXPECT_TRUE(e1 != e2);
}


TEST(FloatElement, Write)
{
    std::ostringstream output;
    std::string expected;
    double value(2.7182818284590451);

    jonen::FloatElement e1(0x80, value, jonen::EBML_FLOAT_PREC_DOUBLE);
    test_flel::fill_buffer(expected, 0x80, value, true, true, true, true);
    EXPECT_EQ(jonen::ids::size(0x80) + 9, e1.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    value = 3.14159;
    e1.value(value);
    e1.precision(jonen::EBML_FLOAT_PREC_SINGLE);
    output.str(std::string());
    std::string().swap(expected);
    test_flel::fill_buffer(expected, 0x80, value, true, true, true, false);
    EXPECT_EQ(jonen::ids::size(0x80) + 5, e1.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
}


TEST(FloatElement, Read)
{
    std::istringstream input;
    std::string input_val;
    double value(23.14069);

    jonen::FloatElement e(0x80, 0);
    test_flel::fill_buffer(input_val, 0x01, value, false, true, true, true);
    input.str(input_val);
    EXPECT_EQ(9, e.read(input));
    EXPECT_EQ(0x80, e.id());
    EXPECT_DOUBLE_EQ(value, e.value());
    EXPECT_EQ(jonen::EBML_FLOAT_PREC_DOUBLE, e.precision());

    value = 22.45916;
    e.value(0);
    e.set_default(0);
    EXPECT_TRUE(e.has_default());
    EXPECT_TRUE(e.is_default());
    std::string().swap(input_val);
    test_flel::fill_buffer(input_val, 0x01, value, false, true, true, false);
    input.str(input_val);
    EXPECT_EQ(5, e.read(input));
    EXPECT_FLOAT_EQ(value, e.value());
    EXPECT_EQ(0, e.get_default());
    EXPECT_FALSE(e.is_default());
    EXPECT_EQ(jonen::EBML_FLOAT_PREC_SINGLE, e.precision());

    // Test for ReadError exception
    std::string().swap(input_val);
    test_flel::fill_buffer(input_val, 0x01, value, false, true, true, true);
    input.str(input_val.substr(0, 4));
    EXPECT_THROW(e.read(input), jonen::ReadError);
    // Test for ReadError exception
    std::string().swap(input_val);
    test_flel::fill_buffer(input_val, 0x01, value, false, true, true, true);
    input.str(input_val.substr(0, 4));
    EXPECT_THROW(e.read(input), jonen::ReadError);
}


TEST(FloatElement, Size)
{
    jonen::FloatElement e(0x80, 1.0, jonen::EBML_FLOAT_PREC_SINGLE);
    EXPECT_EQ(6, e.size());
    e.precision(jonen::EBML_FLOAT_PREC_DOUBLE);
    EXPECT_EQ(10, e.size());

    e.precision(jonen::EBML_FLOAT_PREC_SINGLE);
    e.value(3.15149);
    EXPECT_EQ(6, e.size());
    e.precision(jonen::EBML_FLOAT_PREC_DOUBLE);
    EXPECT_EQ(10, e.size());
}

