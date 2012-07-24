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

#include <jonen/uint_element.h>

#include <gtest/gtest.h>
#include <jonen/ebml_int.h>
#include <jonen/el_ids.h>
#include <jonen/exceptions.h>
#include <jonen/vint.h>

#include "test_consts.h"
#include "test_utils.h"


namespace test_uintel
{

std::streamsize fill_buffer(std::string& b, jonen::ids::ID id, uint64_t data,
        bool write_id, bool write_size, bool write_body)
{
    std::streamsize size(0), total(0);
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
        size = jonen::ebml_int::size_u(data);
        std::vector<char> tmp(jonen::vint::encode(size));
        b.append(&tmp[0], 0, tmp.size());
        total += tmp.size();
    }
    if (write_body)
    {
        std::vector<char> tmp(jonen::ebml_int::encode_u(data));
        b.append(&tmp[0], 0, tmp.size());
        total += tmp.size();
    }
    return total;
}

}; // namespace test_uintel


TEST(UIntElement, Construction)
{
    EXPECT_EQ(jonen::ids::Null, jonen::UIntElement(jonen::ids::Null, 1).id());
    EXPECT_THROW(jonen::UIntElement(0x00, 1), jonen::InvalidElementID);
    EXPECT_THROW(jonen::UIntElement(0xFF, 1), jonen::InvalidElementID);
    EXPECT_THROW(jonen::UIntElement(0xFFFF, 1), jonen::InvalidElementID);
    EXPECT_THROW(jonen::UIntElement(0xFFFFFF, 1), jonen::InvalidElementID);
    EXPECT_THROW(jonen::UIntElement(0xFFFFFFFF, 1), jonen::InvalidElementID);
    // Test with a default as well
    EXPECT_THROW(jonen::UIntElement(0x00, 1, 1), jonen::InvalidElementID);
}


TEST(UIntElement, CopyConstruction)
{
    EXPECT_EQ(jonen::ids::Null, jonen::UIntElement(jonen::UIntElement(jonen::ids::Null, 1)).id());
    EXPECT_EQ(jonen::ids::Null, jonen::UIntElement(jonen::UIntElement(jonen::ids::Null, 1, 1)).id());
    EXPECT_EQ(1, jonen::UIntElement(jonen::UIntElement(jonen::ids::Null, 1, 2)).value());
    EXPECT_EQ(2, jonen::UIntElement(jonen::UIntElement(jonen::ids::Null, 1, 2)).get_default());
    // The exception actually comes from the inner constructor, but just to be
    // sure it makes it out...
    EXPECT_THROW(jonen::UIntElement(jonen::UIntElement(0x00, 1)),
            jonen::InvalidElementID);
}


TEST(UIntElement, SetID)
{
    jonen::UIntElement e(jonen::ids::Null, 1);
    e.id(9999999);
    EXPECT_EQ(9999999, e.id());
    EXPECT_THROW(jonen::UIntElement(1, 1).id(0x00), jonen::InvalidElementID);
    EXPECT_THROW(jonen::UIntElement(1, 1).id(0xFF), jonen::InvalidElementID);
    EXPECT_THROW(jonen::UIntElement(1, 1).id(0xFFFF),
            jonen::InvalidElementID);
    EXPECT_THROW(jonen::UIntElement(1, 1).id(0xFFFFFF),
            jonen::InvalidElementID);
    EXPECT_THROW(jonen::UIntElement(1, 1).id(0xFFFFFFFF),
            jonen::InvalidElementID);
}


TEST(UIntElement, Assignment)
{
    jonen::UIntElement e1(1, 1), e2(2, 2);
    e2 = e1;
    EXPECT_EQ(e1.value(), e2.value());
    EXPECT_EQ(e1.id(), e2.id());

    jonen::UIntElement e3(1, 1, 1), e4(2, 2, 2);
    e4 = e3;
    EXPECT_EQ(e3.value(), e4.value());
    EXPECT_EQ(e3.id(), e4.id());
    EXPECT_EQ(e3.has_default(), e4.has_default());
    EXPECT_EQ(e3.get_default(), e4.get_default());

    jonen::UIntElement e5(1, 1, 1), e6(2, 2);
    e6 = e5;
    EXPECT_EQ(e5.value(), e6.value());
    EXPECT_EQ(e5.id(), e6.id());
    EXPECT_EQ(e5.has_default(), e6.has_default());
    EXPECT_EQ(e5.get_default(), e6.get_default());

    jonen::UIntElement e7(1, 1), e8(2, 2, 2);
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
    EXPECT_FALSE(jonen::UIntElement(jonen::ids::Null, 1).has_default());
    EXPECT_TRUE(jonen::UIntElement(jonen::ids::Null, 1, 1).has_default());

    jonen::UIntElement e1(jonen::ids::Null, 1, 1);
    EXPECT_EQ(1, e1.get_default());
    EXPECT_TRUE(e1.has_default());
    e1.remove_default();
    EXPECT_FALSE(e1.has_default());
    e1.set_default(2);
    EXPECT_TRUE(e1.has_default());
    EXPECT_EQ(2, e1.get_default());

    jonen::UIntElement e2(jonen::ids::Null, 1);
    EXPECT_FALSE(e2.has_default());
    e2.set_default(1);
    EXPECT_TRUE(e2.has_default());
    EXPECT_EQ(1, e2.get_default());
    e2.remove_default();
    EXPECT_FALSE(e2.has_default());

    jonen::UIntElement e3(jonen::ids::Null, 1);
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
    EXPECT_EQ(1, jonen::UIntElement(jonen::ids::Null, 1).value());
    EXPECT_EQ(1, jonen::UIntElement(jonen::ids::Null, 1));
    EXPECT_EQ(1, jonen::UIntElement(jonen::ids::Null, 1, 2).value());
    EXPECT_EQ(1, jonen::UIntElement(jonen::ids::Null, 1, 2));

    jonen::UIntElement e1(jonen::ids::Null, 1);
    EXPECT_EQ(1, e1.value());
    e1.value(2);
    EXPECT_EQ(2, e1.value());

    jonen::UIntElement e2(jonen::ids::Null, 1, 2);
    e2.value(3);
    EXPECT_EQ(3, e2.value());
}


TEST(UIntElement, Equality)
{
    jonen::UIntElement e1(jonen::ids::Null, 1);
    jonen::UIntElement e2(jonen::ids::Null, 1);

    EXPECT_TRUE(e1 == e2);
    e2.value(2);
    EXPECT_TRUE(e1 != e2);
}


TEST(UIntElement, Write)
{
    std::ostringstream output;
    std::string expected;
    int64_t value(2);
    std::streamsize val_size(jonen::ebml_int::size_u(value));

    jonen::UIntElement e1(0x80, value);
    test_uintel::fill_buffer(expected, 0x80, value, true, true, true);
    EXPECT_EQ(jonen::ids::size(0x80) + jonen::vint::size(val_size) +
            val_size, e1.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    value = 0x839F18AAl;
    val_size = jonen::ebml_int::size_u(value);
    e1.value(value);
    output.str(std::string());
    std::string().swap(expected);
    test_uintel::fill_buffer(expected, 0x80, value, true, true, true);
    EXPECT_EQ(jonen::ids::size(0x80) + jonen::vint::size(val_size) +
            val_size, e1.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
}


TEST(UIntElement, Read)
{
    std::istringstream input;
    std::string input_val;
    int64_t value(5);
    std::streamsize val_size(jonen::ebml_int::size_u(value));

    jonen::UIntElement e(jonen::ids::Null, 0);
    test_uintel::fill_buffer(input_val, jonen::ids::Null, value, false, true,
            true);
    input.str(input_val);
    EXPECT_EQ(jonen::vint::size(val_size) + val_size, e.read(input));
    EXPECT_EQ(jonen::ids::Null, e.id());
    EXPECT_EQ(value, e.value());

    value = 0x3A958BCD99l;
    val_size = jonen::ebml_int::size_u(value);
    e.value(0);
    e.set_default(0);
    EXPECT_TRUE(e.has_default());
    EXPECT_TRUE(e.is_default());
    std::string().swap(input_val);
    test_uintel::fill_buffer(input_val, jonen::ids::Null, value, false, true,
            true);
    input.str(input_val);
    EXPECT_EQ(jonen::vint::size(val_size) + val_size, e.read(input));
    EXPECT_EQ(value, e.value());
    EXPECT_EQ(0, e.get_default());
    EXPECT_FALSE(e.is_default());

    // Test for ReadError exception
    std::string().swap(input_val);
    test_uintel::fill_buffer(input_val, jonen::ids::Null, value, false, true,
            true);
    input.str(input_val.substr(0, 4));
    EXPECT_THROW(e.read(input), jonen::ReadError);
}


TEST(UIntElement, Size)
{
    jonen::UIntElement e(jonen::ids::Null, 1);
    EXPECT_EQ(3, e.size());

    e.value(0x7FFFFF);
    EXPECT_EQ(5, e.size());

    e.value(0xFFFFFF);
    EXPECT_EQ(5, e.size());
}

