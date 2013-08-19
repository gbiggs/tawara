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

#include <tawara/binary_element.h>

#include <gtest/gtest.h>
#include <string>
#include <tawara/el_ids.h>
#include <tawara/ebml_int.h>
#include <tawara/exceptions.h>
#include <tawara/vint.h>

#include "test_consts.h"
#include "test_utils.h"


namespace test_binel
{

std::streamsize fill_buffer(std::string& b, tawara::ids::ID id, std::vector<char> data,
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
        std::vector<char> tmp(tawara::vint::encode(data.size()));
        b.append(&tmp[0], 0, tmp.size());
        total += tmp.size();
    }
    if (write_body)
    {
        for (unsigned int ii(0); ii < data.size(); ++ii)
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
            for (char ii(0); ii < 8; ++ii)
            {
                b1.push_back(ii);
                b2.push_back(0xFF - ii);
            }
            for (char ii(0); ii < 15; ++ii)
            {
                b3.push_back(0x7F - ii);
                b4.push_back(0x7F + ii);
            }
        }

        std::vector<char> b1;
        std::vector<char> b2;
        std::vector<char> b3;
        std::vector<char> b4;
}; // class TestBinaryElement


TEST_F(BinaryElementTest, Construction)
{
    EXPECT_EQ(tawara::ids::Null, tawara::BinaryElement(tawara::ids::Null, b1).id());
    EXPECT_THROW(tawara::BinaryElement(0x00, b1), tawara::InvalidElementID);
    EXPECT_THROW(tawara::BinaryElement(0xFF, b1), tawara::InvalidElementID);
    EXPECT_THROW(tawara::BinaryElement(0xFFFF, b1), tawara::InvalidElementID);
    EXPECT_THROW(tawara::BinaryElement(0xFFFFFF, b1), tawara::InvalidElementID);
    EXPECT_THROW(tawara::BinaryElement(0xFFFFFFFF, b1), tawara::InvalidElementID);
    // Test with a default as well
    EXPECT_THROW(tawara::BinaryElement(0x00, b1, b1), tawara::InvalidElementID);
}


TEST_F(BinaryElementTest, CopyConstruction)
{
    EXPECT_EQ(tawara::ids::Null, tawara::BinaryElement(tawara::BinaryElement(tawara::ids::Null, b1)).id());
    EXPECT_EQ(tawara::ids::Null, tawara::BinaryElement(tawara::BinaryElement(tawara::ids::Null, b1, b2)).id());
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b1,
            tawara::BinaryElement(tawara::BinaryElement(tawara::ids::Null, b1, b2)).value());
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b2,
            tawara::BinaryElement(tawara::BinaryElement(tawara::ids::Null, b1, b2)).get_default());
    // The exception actually comes from the inner constructor, but just to be
    // sure it makes it out...
    EXPECT_THROW(tawara::BinaryElement(tawara::BinaryElement(0x00, b1)),
            tawara::InvalidElementID);
}


TEST_F(BinaryElementTest, SetID)
{
    tawara::BinaryElement e(tawara::ids::Null, b1);
    e.id(9999999);
    EXPECT_EQ(9999999, e.id());
    EXPECT_THROW(tawara::BinaryElement(1, b1).id(0x00), tawara::InvalidElementID);
    EXPECT_THROW(tawara::BinaryElement(1, b1).id(0xFF), tawara::InvalidElementID);
    EXPECT_THROW(tawara::BinaryElement(1, b1).id(0xFFFF),
            tawara::InvalidElementID);
    EXPECT_THROW(tawara::BinaryElement(1, b1).id(0xFFFFFF),
            tawara::InvalidElementID);
    EXPECT_THROW(tawara::BinaryElement(1, b1).id(0xFFFFFFFF),
            tawara::InvalidElementID);
}


TEST_F(BinaryElementTest, Assignment)
{
    tawara::BinaryElement e1(1, b1), e2(2, b2);
    e2 = e1;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, e1.value(), e2.value());
    EXPECT_EQ(e1.id(), e2.id());

    tawara::BinaryElement e3(1, b1, b1), e4(2, b2, b2);
    e4 = e3;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, e1.value(), e2.value());
    EXPECT_EQ(e3.id(), e4.id());
    EXPECT_EQ(e3.has_default(), e4.has_default());
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, e3.get_default(), e4.get_default());

    tawara::BinaryElement e5(1, b1, b1), e6(2, b2);
    e6 = e5;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, e1.value(), e2.value());
    EXPECT_EQ(e5.id(), e6.id());
    EXPECT_EQ(e5.has_default(), e6.has_default());
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, e5.get_default(), e6.get_default());

    tawara::BinaryElement e7(1, b1), e8(2, b2, b2);
    e8 = e7;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, e1.value(), e2.value());
    EXPECT_EQ(e7.id(), e8.id());
    EXPECT_EQ(e7.has_default(), e8.has_default());
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, e7.get_default(), e8.get_default());

    e8 = b4;
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b4, e8.value());
}


TEST_F(BinaryElementTest, Default)
{
    EXPECT_FALSE(tawara::BinaryElement(tawara::ids::Null, b1).has_default());
    EXPECT_TRUE(tawara::BinaryElement(tawara::ids::Null, b1, b1).has_default());

    tawara::BinaryElement e1(tawara::ids::Null, b1, b1);
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b1, e1.get_default());
    EXPECT_TRUE(e1.has_default());
    e1.remove_default();
    EXPECT_FALSE(e1.has_default());
    e1.set_default(b2);
    EXPECT_TRUE(e1.has_default());
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b2, e1.get_default());

    tawara::BinaryElement e2(tawara::ids::Null, b1);
    EXPECT_FALSE(e2.has_default());
    e2.set_default(b1);
    EXPECT_TRUE(e2.has_default());
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b1, e2.get_default());
    e2.remove_default();
    EXPECT_FALSE(e2.has_default());

    tawara::BinaryElement e3(tawara::ids::Null, b1);
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
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b1,
            tawara::BinaryElement(tawara::ids::Null, b1).value());
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b1,
            tawara::BinaryElement(tawara::ids::Null, b1));
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b1,
            tawara::BinaryElement(tawara::ids::Null, b1, b2).value());
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b1,
            tawara::BinaryElement(tawara::ids::Null, b1, b2));

    tawara::BinaryElement e1(tawara::ids::Null, b1);
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b1, e1.value());
    e1.value(b2);
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b2, e1.value());
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b2, e1);

    tawara::BinaryElement e2(tawara::ids::Null, b1, b2);
    e2.value(b3);
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b3, e2.value());
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b3, e2);
}


TEST_F(BinaryElementTest, Equality)
{
    tawara::BinaryElement e1(tawara::ids::Null, b1);
    tawara::BinaryElement e2(tawara::ids::Null, b1);

    EXPECT_TRUE(e1 == e2);
    e2.value(b2);
    EXPECT_TRUE(e1 != e2);
}


TEST_F(BinaryElementTest, Write)
{
    std::ostringstream output;
    std::string expected;

    tawara::BinaryElement e1(0x80, b1);

    output.str(std::string());
    std::string().swap(expected);
    test_binel::fill_buffer(expected, 0x80, b1, true, true, true);
    EXPECT_EQ(tawara::ids::size(0x80) + 1 + b1.size(),
            e1.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    e1.value(b3);
    output.str(std::string());
    std::string().swap(expected);
    test_binel::fill_buffer(expected, 0x80, b3, true, true, true);
    EXPECT_EQ(tawara::ids::size(0x80) + 1 + b3.size(),
            e1.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    // Zero-length binary
    std::vector<char> value;
    e1.value(value);
    output.str(std::string());
    std::string().swap(expected);
    test_binel::fill_buffer(expected, 0x80, value, true, true, true);
    EXPECT_EQ(tawara::ids::size(0x80) + 1 + value.size(), e1.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
}


TEST_F(BinaryElementTest, Read)
{
    std::istringstream input;
    std::string input_val;

    tawara::BinaryElement e(0x80, std::vector<char>());
    test_binel::fill_buffer(input_val, 0x80, b1, false, true, true);
    input.str(input_val);
    EXPECT_EQ(1 + b1.size(), e.read(input));
    EXPECT_EQ(0x80, e.id());
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b1, e.value());

    e.value(b2);
    e.set_default(b2);
    EXPECT_TRUE(e.has_default());
    EXPECT_TRUE(e.is_default());
    std::string().swap(input_val);
    test_binel::fill_buffer(input_val, 0x80, b3, false, true, true);
    input.str(input_val);
    EXPECT_EQ(1 + b3.size(), e.read(input));
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b3, e.value());
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b2, e.get_default());
    EXPECT_FALSE(e.is_default());

    // Zero-length binary
    std::vector<char> empty;
    e.value(b2);
    e.set_default(b2);
    EXPECT_TRUE(e.has_default());
    EXPECT_TRUE(e.is_default());
    std::string().swap(input_val);
    test_binel::fill_buffer(input_val, 0x80, empty, false, true, true);
    input.str(input_val);
    EXPECT_EQ(1, e.read(input));
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, empty, e.value());
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b2, e.get_default());
    EXPECT_FALSE(e.is_default());

    // Test for ReadError exception
    std::string().swap(input_val);
    test_binel::fill_buffer(input_val, 0x80, b4, false, true, true);
    input.str(input_val.substr(0, 4));
    EXPECT_THROW(e.read(input), tawara::ReadError);
}


TEST_F(BinaryElementTest, Size)
{
    tawara::BinaryElement e(0x80, b1);
    EXPECT_EQ(2 + b1.size(), e.size());

    e.value(b3);
    EXPECT_EQ(2 + b3.size(), e.size());
}

