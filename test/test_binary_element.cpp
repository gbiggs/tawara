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

#include <gtest/gtest.h>
#include <tide/binary_element.h>
#include <tide/ebml_integer.h>
#include <tide/exceptions.h>

#include "test_utilities.h"

using namespace tide;

namespace test_binary_el
{
    class BinaryElementTest : public ::testing::Test
    {
        public:
            virtual void SetUp()
            {
                for (char ii(0); ii < 8; ++ii)
                {
                    b1.push_back('a' + ii);
                    b4.push_back('z' - ii);
                }
                for (char ii(0); ii < 12; ++ii)
                {
                    b2.push_back('m' - ii);
                    b3.push_back('n' + ii);
                }
            }

            std::vector<char> b1;
            std::vector<char> b2;
            std::vector<char> b3;
            std::vector<char> b4;
    }; // class TestBinaryElement


    ///////////////////////////////////////////////////////////////////////////
    // BinaryElement interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST_F(BinaryElementTest, Construction)
    {
        EXPECT_EQ(ids::Null, BinaryElement(ids::Null, b1).id());
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b1,
                BinaryElement(ids::Null, b1).value());
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b1,
                BinaryElement(ids::Null, b1, b2).value());
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b2,
                BinaryElement(ids::Null, b1, b2).get_default());
        EXPECT_FALSE(BinaryElement(ids::Null, b1).has_default());
        EXPECT_TRUE(BinaryElement(ids::Null, b1, b2).has_default());

        EXPECT_THROW(BinaryElement(0x00, b1), InvalidElementID);
        EXPECT_THROW(BinaryElement(0xFF, b1), InvalidElementID);
        EXPECT_THROW(BinaryElement(0xFFFF, b1), InvalidElementID);
        EXPECT_THROW(BinaryElement(0xFFFFFF, b1), InvalidElementID);
        EXPECT_THROW(BinaryElement(0xFFFFFFFF, b1), InvalidElementID);

        EXPECT_THROW(BinaryElement(0x00, b1, b2), InvalidElementID);
        EXPECT_THROW(BinaryElement(0xFF, b1, b2), InvalidElementID);
        EXPECT_THROW(BinaryElement(0xFFFF, b1, b2), InvalidElementID);
        EXPECT_THROW(BinaryElement(0xFFFFFF, b1, b2), InvalidElementID);
        EXPECT_THROW(BinaryElement(0xFFFFFFFF, b1, b2), InvalidElementID);
    }

    TEST_F(BinaryElementTest, Swap)
    {
        BinaryElement ee1(0x21, b1, b2);
        BinaryElement ee2(0x42, b3, b4);
        swap(ee1, ee2);
        EXPECT_EQ(0x42, ee1.id());
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b3, ee1.value());
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b4, ee1.get_default());
        EXPECT_EQ(0x21, ee2.id());
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b1, ee2.value());
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b2, ee2.get_default());

        ee1.swap(ee2);
        EXPECT_EQ(0x21, ee1.id());
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b1, ee1.value());
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b2, ee1.get_default());
        EXPECT_EQ(0x42, ee2.id());
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b3, ee2.value());
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b4, ee2.get_default());
    }

    TEST_F(BinaryElementTest, LessThan)
    {
        BinaryElement ee1(ids::Null, b1);
        BinaryElement ee2(ids::Null, b4);
        EXPECT_TRUE(ee1 < ee2);
        EXPECT_TRUE(ee1 < b4);
        EXPECT_TRUE(b1 < ee2);
        EXPECT_FALSE(ee2 < ee1);
        EXPECT_FALSE(ee2 < b1);
        EXPECT_FALSE(b4 < ee1);
    }

    TEST_F(BinaryElementTest, LessThanEqual)
    {
        BinaryElement ee1(ids::Null, b1);
        BinaryElement ee2(ids::Null, b1);
        BinaryElement ee3(ids::Null, b2);
        EXPECT_TRUE(ee1 <= ee2);
        EXPECT_TRUE(ee1 <= ee3);
        EXPECT_TRUE(ee1 <= b1);
        EXPECT_TRUE(ee1 <= b2);
        EXPECT_TRUE(b1 <= ee3);
        EXPECT_TRUE(b2 <= ee3);
        EXPECT_FALSE(ee3 <= ee2);
        EXPECT_FALSE(ee3 <= b1);
        EXPECT_FALSE(b3 <= ee3);
    }

    TEST_F(BinaryElementTest, NotEqual)
    {
        BinaryElement ee1(ids::Null, b1);
        BinaryElement ee2(ids::Null, b1);
        BinaryElement ee3(ids::Null, b2);
        EXPECT_FALSE(ee1 != ee2);
        EXPECT_FALSE(ee1 != b1);
        EXPECT_FALSE(b1 != ee1);
        EXPECT_TRUE(ee1 != ee3);
        EXPECT_TRUE(ee1 != b2);
        EXPECT_TRUE(b2 != ee1);
    }

    TEST_F(BinaryElementTest, GreaterThanEqual)
    {
        BinaryElement ee1(ids::Null, b3);
        BinaryElement ee2(ids::Null, b3);
        BinaryElement ee3(ids::Null, b2);
        EXPECT_TRUE(ee1 >= ee2);
        EXPECT_TRUE(ee1 >= ee3);
        EXPECT_TRUE(ee1 >= b2);
        EXPECT_TRUE(ee1 >= b3);
        EXPECT_TRUE(b2 >= ee3);
        EXPECT_TRUE(b3 >= ee3);
        EXPECT_FALSE(ee3 >= ee2);
        EXPECT_FALSE(ee3 >= b3);
        EXPECT_FALSE(b1 >= ee3);
    }

    TEST_F(BinaryElementTest, GreaterThan)
    {
        BinaryElement ee1(ids::Null, b2);
        BinaryElement ee2(ids::Null, b1);
        EXPECT_TRUE(ee1 > ee2);
        EXPECT_TRUE(ee1 > b1);
        EXPECT_TRUE(b2 > ee2);
        EXPECT_FALSE(ee2 > ee1);
        EXPECT_FALSE(ee2 > b2);
        EXPECT_FALSE(b1 > ee1);
    }

    ///////////////////////////////////////////////////////////////////////////
    // PrimitiveElement interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST_F(BinaryElementTest, Value)
    {
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b2,
                BinaryElement(ids::Null, b2).value());
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b2,
                BinaryElement(ids::Null, b2, b1).value());

        BinaryElement ee1(ids::Null, b2);
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b2, ee1.value());
        ee1.value(b1);
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b1, ee1.value());

        BinaryElement ee2(ids::Null, b2, b1);
        ee2.value(b3);
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b3, ee2.value());
    }

    TEST_F(BinaryElementTest, Default)
    {
        EXPECT_FALSE(BinaryElement(ids::Null, b2).has_default());
        EXPECT_TRUE(BinaryElement(ids::Null, b2, b1).has_default());

        BinaryElement ee1(ids::Null, b2, b1);
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b1, ee1.get_default());
        EXPECT_TRUE(ee1.has_default());
        ee1.remove_default();
        EXPECT_FALSE(ee1.has_default());
        ee1.set_default(b3);
        EXPECT_TRUE(ee1.has_default());
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b3, ee1.get_default());

        BinaryElement ee2(ids::Null, b2);
        EXPECT_FALSE(ee2.has_default());
        ee2.set_default(b1);
        EXPECT_TRUE(ee2.has_default());
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b1, ee2.get_default());
        ee2.remove_default();
        EXPECT_FALSE(ee2.has_default());

        BinaryElement ee3(ids::Null, b2);
        EXPECT_FALSE(ee3.is_default());
        ee3.set_default(b2);
        EXPECT_TRUE(ee3.is_default());
        ee3.set_default(b1);
        EXPECT_FALSE(ee3.is_default());
        ee3.value(b1);
        EXPECT_TRUE(ee3.is_default());
    }

    TEST_F(BinaryElementTest, Equal)
    {
        BinaryElement ee1(ids::Null, b2);
        BinaryElement ee2(ids::Null, b2);
        BinaryElement ee3(ids::Null, b1);
        EXPECT_TRUE(ee1 == ee2);
        EXPECT_TRUE(ee1 == b2);
        EXPECT_TRUE(b2 == ee1);
        EXPECT_FALSE(ee1 == ee3);
        EXPECT_FALSE(ee1 == b1);
        EXPECT_FALSE(b1 == ee1);
    }


    ///////////////////////////////////////////////////////////////////////////
    // Element interface tests
    ///////////////////////////////////////////////////////////////////////////

    std::streamsize fill_buffer(std::string& b, ids::ID id,
            std::vector<char> const& data, bool write_id, bool write_size,
            bool write_body)
    {
        std::streamsize total(0);
        if (write_id)
        {
            // Cheating on the IDs a bit - there is no protection here against
            // invalid IDs
            std::vector<char> tmp(ebml_int::encode_u(id));
            b.append(&tmp[0], 0, tmp.size());
            total += tmp.size();
        }
        if (write_size)
        {
            std::vector<char> tmp(vint::encode(data.size()));
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

    TEST_F(BinaryElementTest, ID)
    {
        BinaryElement ee(42, b1);
        EXPECT_EQ(42, ee.id());
    }

    TEST_F(BinaryElementTest, Offset)
    {
        std::stringstream ss;
        std::string input_val;

        BinaryElement ee1(ids::Null, b1);
        BinaryElement ee2(ids::Null, std::vector<char>());
        EXPECT_EQ(0, ee1.offset());

        // Set when read test
        write(ee1, ss);
        fill_buffer(input_val, ids::Null, b3, true, true, true);
        ss << input_val;
        ss.seekg(ee1.stored_size() + ids::size(ids::Null));
        ee2.read(ss);
        EXPECT_EQ(ee1.stored_size(), ee2.offset());

        // Set when written test
        write(ee2, ss);
        EXPECT_EQ(ee1.stored_size() + ee2.stored_size(), ee2.offset());
    }

    TEST_F(BinaryElementTest, StoredSize)
    {
        BinaryElement ee(ids::Null, b1);
        EXPECT_EQ(ids::size(ids::Null) + vint::size(b1.size()) + b1.size(),
                ee.stored_size());
    }

    TEST_F(BinaryElementTest, Read)
    {
        std::istringstream input;
        std::string input_val;

        BinaryElement ee(ids::Null, std::vector<char>());
        fill_buffer(input_val, ids::Null, b1, false, true, true);
        input.str(input_val);
        EXPECT_EQ(vint::size(b1.size()) + b1.size(), ee.read(input));
        EXPECT_EQ(ids::Null, ee.id());
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b1, ee.value());

        ee.value(b2);
        ee.set_default(b2);
        EXPECT_TRUE(ee.has_default());
        EXPECT_TRUE(ee.is_default());
        std::string().swap(input_val);
        fill_buffer(input_val, ids::Null, b3, false, true, true);
        input.str(input_val);
        EXPECT_EQ(vint::size(b3.size()) + b3.size(), ee.read(input));
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b3, ee.value());
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b2, ee.get_default());
        EXPECT_FALSE(ee.is_default());

        // Zero-length binary
        std::vector<char> empty;
        ee.value(b2);
        ee.set_default(b2);
        EXPECT_TRUE(ee.has_default());
        EXPECT_TRUE(ee.is_default());
        std::string().swap(input_val);
        fill_buffer(input_val, ids::Null, empty, false, true, true);
        input.str(input_val);
        EXPECT_EQ(1, ee.read(input));
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, empty, ee.value());
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b2, ee.get_default());
        EXPECT_FALSE(ee.is_default());

        // Test for ReadError exception
        std::string().swap(input_val);
        fill_buffer(input_val, ids::Null, b4, false, true, true);
        input.str(input_val.substr(0, 4));
        EXPECT_THROW(ee.read(input), ReadError);
    }

    TEST_F(BinaryElementTest, StartWrite)
    {
        std::stringstream output;
        std::string expected;
        tide::BinaryElement ee(ids::Null, b1);

        output.str(std::string());
        std::string().swap(expected);
        fill_buffer(expected, ids::Null, b1, true, true, true);
        EXPECT_EQ(ids::size(ids::Null) + vint::size(b1.size()) + b1.size(),
                ee.start_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
                expected);

        ee.value(b3);
        output.str(std::string());
        std::string().swap(expected);
        fill_buffer(expected, ids::Null, b3, true, true, true);
        EXPECT_EQ(tide::ids::size(ids::Null) + 1 + b3.size(),
                ee.start_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
                expected);

        // Zero-length binary
        std::vector<char> empty;
        ee.value(empty);
        output.str(std::string());
        std::string().swap(expected);
        fill_buffer(expected, ids::Null, empty, true, true, true);
        EXPECT_EQ(tide::ids::size(ids::Null) + vint::size(empty.size()) +
                empty.size(), ee.start_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
                expected);
    }

    TEST_F(BinaryElementTest, FinishWrite)
    {
        std::stringstream output;
        std::string expected;
        BinaryElement ee(ids::Null, b1);

        // Test for exception and no change to output when finishing without
        // starting
        EXPECT_THROW(ee.finish_write(output), NotWriting);
        EXPECT_EQ(0, output.tellp());

        output.str(std::string());
        std::string().swap(expected);
        fill_buffer(expected, ids::Null, b1, true, true, true);
        ee.start_write(output);
        EXPECT_EQ(ids::size(ids::Null) + vint::size(b1.size()) + b1.size(),
                ee.finish_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
                expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Null) + vint::size(b1.size()) + b1.size(),
                output.tellp());
        // Test for double-finish raising an exception
        EXPECT_THROW(ee.finish_write(output), NotWriting);

        ee.value(b3);
        output.str(std::string());
        std::string().swap(expected);
        fill_buffer(expected, ids::Null, b3, true, true, true);
        ee.start_write(output);
        EXPECT_EQ(tide::ids::size(ids::Null) + 1 + b3.size(),
                ee.finish_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
                expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Null) + vint::size(b3.size()) + b3.size(),
                output.tellp());

        // Zero-length binary
        std::vector<char> empty;
        ee.value(empty);
        output.str(std::string());
        std::string().swap(expected);
        fill_buffer(expected, ids::Null, empty, true, true, true);
        ee.start_write(output);
        EXPECT_EQ(tide::ids::size(ids::Null) + vint::size(empty.size()) +
                empty.size(), ee.finish_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
                expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Null) + vint::size(empty.size()) +
                empty.size(), output.tellp());
    }

    TEST_F(BinaryElementTest, Write)
    {
        std::stringstream output;
        std::string expected;
        BinaryElement ee(ids::Null, b1);

        // Test for exception and no change to output when finishing without
        // starting
        EXPECT_THROW(ee.finish_write(output), NotWriting);
        EXPECT_EQ(0, output.tellp());

        output.str(std::string());
        std::string().swap(expected);
        fill_buffer(expected, ids::Null, b1, true, true, true);
        EXPECT_EQ(ids::size(ids::Null) + vint::size(b1.size()) + b1.size(),
                write(ee, output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
                expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Null) + vint::size(b1.size()) + b1.size(),
                output.tellp());
        // Test for double-finish raising an exception
        EXPECT_THROW(ee.finish_write(output), NotWriting);

        ee.value(b3);
        output.str(std::string());
        std::string().swap(expected);
        fill_buffer(expected, ids::Null, b3, true, true, true);
        EXPECT_EQ(tide::ids::size(ids::Null) + 1 + b3.size(),
                write(ee, output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
                expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Null) + vint::size(b3.size()) + b3.size(),
                output.tellp());

        // Zero-length binary
        std::vector<char> empty;
        ee.value(empty);
        output.str(std::string());
        std::string().swap(expected);
        fill_buffer(expected, ids::Null, empty, true, true, true);
        EXPECT_EQ(tide::ids::size(ids::Null) + vint::size(empty.size()) +
                empty.size(), write(ee, output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
                expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Null) + vint::size(empty.size()) +
                empty.size(), output.tellp());
    }
}; // namespace test_binary_el

