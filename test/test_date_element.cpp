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

#include <tawara/date_element.h>

#include <gtest/gtest.h>
#include <tawara/el_ids.h>
#include <tawara/ebml_int.h>
#include <tawara/exceptions.h>
#include <tawara/vint.h>

#include "test_consts.h"
#include "test_utils.h"


namespace test_datel
{

std::streamsize fill_buffer(std::string& b, tawara::ids::ID id, int64_t data,
        bool write_id, bool write_size, bool write_body)
{
    std::streamsize total(0);
    if (write_id)
    {
        // Cheating on the IDs a bit - there is no protection here against
        // invalid IDs
        std::vector<char> tmp(tawara::ebml_int::encode_u(id));
        b.append(&tmp[0], 0, tmp.size());
        total += tmp.size();
    }
    if (write_size)
    {
        std::vector<char> tmp(tawara::vint::encode(8));
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
    EXPECT_EQ(tawara::ids::Null, tawara::DateElement(tawara::ids::Null, 1).id());
    EXPECT_THROW(tawara::DateElement(0x00, 1), tawara::InvalidElementID);
    EXPECT_THROW(tawara::DateElement(0xFF, 1), tawara::InvalidElementID);
    EXPECT_THROW(tawara::DateElement(0xFFFF, 1), tawara::InvalidElementID);
    EXPECT_THROW(tawara::DateElement(0xFFFFFF, 1), tawara::InvalidElementID);
    EXPECT_THROW(tawara::DateElement(0xFFFFFFFF, 1), tawara::InvalidElementID);
    // Test with a default as well
    EXPECT_THROW(tawara::DateElement(0x00, 1, 1), tawara::InvalidElementID);
}


TEST(DateElement, CopyConstruction)
{
    EXPECT_EQ(tawara::ids::Null, tawara::DateElement(tawara::DateElement(tawara::ids::Null, 1)).id());
    EXPECT_EQ(tawara::ids::Null, tawara::DateElement(tawara::DateElement(tawara::ids::Null, 1, 2)).id());
    EXPECT_EQ(1, tawara::DateElement(tawara::DateElement(tawara::ids::Null, 1, 2)).value());
    EXPECT_EQ(2, tawara::DateElement(tawara::DateElement(tawara::ids::Null, 1, 2)).get_default());
    // The exception actually comes from the inner constructor, but just to be
    // sure it makes it out...
    EXPECT_THROW(tawara::DateElement(tawara::DateElement(0x00, 1)),
            tawara::InvalidElementID);
}


TEST(DateElement, SetID)
{
    tawara::DateElement e(tawara::ids::Null, 1);
    e.id(9999999);
    EXPECT_EQ(9999999, e.id());
    EXPECT_THROW(tawara::DateElement(1, 1).id(0x00), tawara::InvalidElementID);
    EXPECT_THROW(tawara::DateElement(1, 1).id(0xFF), tawara::InvalidElementID);
    EXPECT_THROW(tawara::DateElement(1, 1).id(0xFFFF),
            tawara::InvalidElementID);
    EXPECT_THROW(tawara::DateElement(1, 1).id(0xFFFFFF),
            tawara::InvalidElementID);
    EXPECT_THROW(tawara::DateElement(1, 1).id(0xFFFFFFFF),
            tawara::InvalidElementID);
}


TEST(DateElement, Assignment)
{
    tawara::DateElement e1(1, 1), e2(2, 2);
    e2 = e1;
    EXPECT_EQ(e1.value(), e2.value());
    EXPECT_EQ(e1.id(), e2.id());

    tawara::DateElement e3(1, 1, 1), e4(2, 2, 2);
    e4 = e3;
    EXPECT_EQ(e1.value(), e2.value());
    EXPECT_EQ(e3.id(), e4.id());
    EXPECT_EQ(e3.has_default(), e4.has_default());
    EXPECT_EQ(e3.get_default(), e4.get_default());

    tawara::DateElement e5(1, 1, 1), e6(2, 2);
    e6 = e5;
    EXPECT_EQ(e1.value(), e2.value());
    EXPECT_EQ(e5.id(), e6.id());
    EXPECT_EQ(e5.has_default(), e6.has_default());
    EXPECT_EQ(e5.get_default(), e6.get_default());

    tawara::DateElement e7(1, 1), e8(2, 2, 2);
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
    EXPECT_FALSE(tawara::DateElement(tawara::ids::Null, 1).has_default());
    EXPECT_TRUE(tawara::DateElement(tawara::ids::Null, 1, 1).has_default());

    tawara::DateElement e1(tawara::ids::Null, 1, 1);
    EXPECT_EQ(1, e1.get_default());
    EXPECT_TRUE(e1.has_default());
    e1.remove_default();
    EXPECT_FALSE(e1.has_default());
    e1.set_default(2);
    EXPECT_TRUE(e1.has_default());
    EXPECT_EQ(2, e1.get_default());

    tawara::DateElement e2(tawara::ids::Null, 1);
    EXPECT_FALSE(e2.has_default());
    e2.set_default(1);
    EXPECT_TRUE(e2.has_default());
    EXPECT_EQ(1, e2.get_default());
    e2.remove_default();
    EXPECT_FALSE(e2.has_default());

    tawara::DateElement e3(tawara::ids::Null, 1);
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
    EXPECT_EQ(1, tawara::DateElement(tawara::ids::Null, 1).value());
    EXPECT_EQ(1, tawara::DateElement(tawara::ids::Null, 1));
    EXPECT_EQ(1, tawara::DateElement(tawara::ids::Null, 1, 2).value());
    EXPECT_EQ(1, tawara::DateElement(tawara::ids::Null, 1, 2));

    tawara::DateElement e1(tawara::ids::Null, 1);
    EXPECT_EQ(1, e1.value());
    e1.value(2);
    EXPECT_EQ(2, e1.value());

    tawara::DateElement e2(tawara::ids::Null, 1, 2);
    e2.value(3);
    EXPECT_EQ(3, e2.value());
}


TEST(DateElement, Equality)
{
    tawara::DateElement e1(tawara::ids::Null, 1);
    tawara::DateElement e2(tawara::ids::Null, 1);

    EXPECT_TRUE(e1 == e2);
    e2.value(2);
    EXPECT_TRUE(e1 != e2);
}


TEST(DateElement, Write)
{
    std::ostringstream output;
    std::string expected;
    int64_t value(2);

    tawara::DateElement e1(0x80, value);

    test_datel::fill_buffer(expected, 0x80, value, true, true, true);
    EXPECT_EQ(tawara::ids::size(0x80) + 1 + 8, e1.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    value = -0x839F18AAl;
    e1.value(value);
    output.str(std::string());
    std::string().swap(expected);
    test_datel::fill_buffer(expected, 0x80, value, true, true, true);
    EXPECT_EQ(tawara::ids::size(0x80) + 1 + 8, e1.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
}


TEST(DateElement, Read)
{
    std::istringstream input;
    std::string input_val;
    int64_t value(5);

    tawara::DateElement e(0x80, 0);
    test_datel::fill_buffer(input_val, 0x80, value, false, true, true);
    input.str(input_val);
    EXPECT_EQ(1 + 8, e.read(input));
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
    EXPECT_EQ(1 + 8, e.read(input));
    EXPECT_EQ(value, e.value());
    EXPECT_EQ(0, e.get_default());
    EXPECT_FALSE(e.is_default());

    // Test for BadElementLength exception
    std::string().swap(input_val);
    test_datel::fill_buffer(input_val, 0x80, value, false, true, true);
    input_val[0] = 0x05;
    input.str(input_val);
    EXPECT_THROW(e.read(input), tawara::BadElementLength);
    // Test for ReadError exception
    std::string().swap(input_val);
    test_datel::fill_buffer(input_val, 0x80, value, false, true, true);
    input.str(input_val.substr(0, 4));
    EXPECT_THROW(e.read(input), tawara::ReadError);
}


TEST(DateElement, Size)
{
    tawara::DateElement e(0x80, 1);
    EXPECT_EQ(10, e.size());

    e.value(0x7FFFFF);
    EXPECT_EQ(10, e.size());

    e.value(0xFFFFFF);
    EXPECT_EQ(10, e.size());
}

