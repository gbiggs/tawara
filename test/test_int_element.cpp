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
#include <tide/exceptions.h>
#include <tide/integer_elements.h>

#include "test_utilities.h"

using namespace tide;

namespace test_int_el
{
    ///////////////////////////////////////////////////////////////////////////
    // IntegerElement interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST(IntElement, Construction)
    {
        EXPECT_EQ(ids::Null, IntElement(ids::Null, 0).id());
        EXPECT_EQ(42, IntElement(ids::Null, 42).value());
        EXPECT_EQ(42, IntElement(ids::Null, 42, 0).value());
        EXPECT_EQ(0, IntElement(ids::Null, 42, 0).get_default());
        EXPECT_EQ(false, IntElement(ids::Null, 42).has_default());
        EXPECT_EQ(true, IntElement(ids::Null, 42, 0).has_default());

        EXPECT_THROW(IntElement(0x00, 1), InvalidElementID);
        EXPECT_THROW(IntElement(0xFF, 1), InvalidElementID);
        EXPECT_THROW(IntElement(0xFFFF, 1), InvalidElementID);
        EXPECT_THROW(IntElement(0xFFFFFF, 1), InvalidElementID);
        EXPECT_THROW(IntElement(0xFFFFFFFF, 1), InvalidElementID);

        EXPECT_THROW(IntElement(0x00, 1, 2), InvalidElementID);
        EXPECT_THROW(IntElement(0xFF, 1, 2), InvalidElementID);
        EXPECT_THROW(IntElement(0xFFFF, 1, 2), InvalidElementID);
        EXPECT_THROW(IntElement(0xFFFFFF, 1, 2), InvalidElementID);
        EXPECT_THROW(IntElement(0xFFFFFFFF, 1, 2), InvalidElementID);
    }

    TEST(IntElement, Swap)
    {
        IntElement ee1(0x21, 12, 1);
        IntElement ee2(0x42, 24, 2);
        swap(ee1, ee2);
        EXPECT_EQ(0x42, ee1.id());
        EXPECT_EQ(24, ee1.value());
        EXPECT_EQ(2, ee1.get_default());
        EXPECT_EQ(0x21, ee2.id());
        EXPECT_EQ(12, ee2.value());
        EXPECT_EQ(1, ee2.get_default());

        ee1.swap(ee2);
        EXPECT_EQ(0x21, ee1.id());
        EXPECT_EQ(12, ee1.value());
        EXPECT_EQ(1, ee1.get_default());
        EXPECT_EQ(0x42, ee2.id());
        EXPECT_EQ(24, ee2.value());
        EXPECT_EQ(2, ee2.get_default());
    }

    TEST(IntElement, LessThan)
    {
        IntElement ee1(ids::Null, 1);
        IntElement ee2(ids::Null, 2);
        EXPECT_TRUE(ee1 < ee2);
        EXPECT_TRUE(ee1 < 2);
        EXPECT_TRUE(1 < ee2);
        EXPECT_FALSE(ee2 < ee1);
        EXPECT_FALSE(ee2 < 1);
        EXPECT_FALSE(2 < ee1);
    }

    TEST(IntElement, LessThanEqual)
    {
        IntElement ee1(ids::Null, 1);
        IntElement ee2(ids::Null, 1);
        IntElement ee3(ids::Null, 2);
        EXPECT_TRUE(ee1 <= ee2);
        EXPECT_TRUE(ee1 <= ee3);
        EXPECT_TRUE(ee1 <= 1);
        EXPECT_TRUE(ee1 <= 2);
        EXPECT_TRUE(1 <= ee3);
        EXPECT_TRUE(2 <= ee3);
        EXPECT_FALSE(ee3 <= ee2);
        EXPECT_FALSE(ee3 <= 1);
        EXPECT_FALSE(3 <= ee3);
    }

    TEST(IntElement, NotEqual)
    {
        IntElement ee1(ids::Null, 1);
        IntElement ee2(ids::Null, 1);
        IntElement ee3(ids::Null, 2);
        EXPECT_FALSE(ee1 != ee2);
        EXPECT_FALSE(ee1 != 1);
        EXPECT_FALSE(1 != ee1);
        EXPECT_TRUE(ee1 != ee3);
        EXPECT_TRUE(ee1 != 2);
        EXPECT_TRUE(2 != ee1);
    }

    TEST(IntElement, GreaterThanEqual)
    {
        IntElement ee1(ids::Null, 2);
        IntElement ee2(ids::Null, 2);
        IntElement ee3(ids::Null, 1);
        EXPECT_TRUE(ee1 >= ee2);
        EXPECT_TRUE(ee1 >= ee3);
        EXPECT_TRUE(ee1 >= 1);
        EXPECT_TRUE(ee1 >= 2);
        EXPECT_TRUE(1 >= ee3);
        EXPECT_TRUE(2 >= ee3);
        EXPECT_FALSE(ee3 >= ee2);
        EXPECT_FALSE(ee3 >= 2);
        EXPECT_FALSE(0 >= ee3);
    }

    TEST(IntElement, GreaterThan)
    {
        IntElement ee1(ids::Null, 2);
        IntElement ee2(ids::Null, 1);
        EXPECT_TRUE(ee1 > ee2);
        EXPECT_TRUE(ee1 > 1);
        EXPECT_TRUE(2 > ee2);
        EXPECT_FALSE(ee2 > ee1);
        EXPECT_FALSE(ee2 > 2);
        EXPECT_FALSE(1 > ee1);
    }

    TEST(IntElement, Addition)
    {
        IntElement ee1(ids::Null, -1);
        IntElement ee2(ids::Null, 2);
        EXPECT_EQ(1, (ee1 + ee2).value());
        EXPECT_EQ(1, (ee1 + 2).value());
        EXPECT_EQ(1, -1 + ee2);
        ee1 += ee2;
        EXPECT_EQ(1, ee1.value());
        ee2 += 1;
        EXPECT_EQ(3, ee2.value());
    }

    TEST(IntElement, Subtraction)
    {
        IntElement ee1(ids::Null, 1);
        IntElement ee2(ids::Null, 2);
        EXPECT_EQ(-1, (ee1 - ee2).value());
        EXPECT_EQ(-1, (ee1 - 2).value());
        EXPECT_EQ(-1, 1 - ee2);
        ee1 -= ee2;
        EXPECT_EQ(-1, ee1.value());
        ee2 -= 1;
        EXPECT_EQ(1, ee2.value());
    }

    TEST(IntElement, Multiplication)
    {
        IntElement ee1(ids::Null, 2);
        IntElement ee2(ids::Null, -3);
        EXPECT_EQ(-6, (ee1 * ee2).value());
        EXPECT_EQ(-6, (ee1 * -3).value());
        EXPECT_EQ(-6, 2 * ee2);
        ee1 *= ee2;
        EXPECT_EQ(-6, ee1.value());
        ee2 *= 2;
        EXPECT_EQ(-6, ee2.value());
    }

    TEST(IntElement, Division)
    {
        IntElement ee1(ids::Null, 8);
        IntElement ee2(ids::Null, -4);
        EXPECT_EQ(-2, (ee1 / ee2).value());
        EXPECT_EQ(-2, (ee1 / -4).value());
        EXPECT_EQ(-2, 8 / ee2);
        ee1 /= ee2;
        EXPECT_EQ(-2, ee1.value());
        ee2 /= -2;
        EXPECT_EQ(2, ee2.value());
    }

    TEST(IntElement, Modulus)
    {
        IntElement ee1(ids::Null, 4);
        IntElement ee2(ids::Null, 2);
        IntElement ee3(ids::Null, 3);
        EXPECT_EQ(0, (ee1 % ee2).value());
        EXPECT_EQ(0, (ee1 % 2).value());
        EXPECT_EQ(0, 4 % ee2);
        EXPECT_EQ(1, (ee1 % ee3).value());
        EXPECT_EQ(1, (ee1 % 3).value());
        EXPECT_EQ(1, 4 % ee3);
        ee1 %= ee2;
        EXPECT_EQ(0, ee1.value());
        ee1.value(4);
        ee1 %= ee3;
        EXPECT_EQ(1, ee1.value());
    }

    TEST(IntElement, LogicalOr)
    {
        IntElement ee1(ids::Null, 0x01);
        IntElement ee2(ids::Null, 0x10);
        EXPECT_EQ(0x11, (ee1 | ee2).value());
        EXPECT_EQ(0x11, (ee1 | 0x10).value());
        EXPECT_EQ(0x11, (0x01 | ee2).value());
        ee1 |= ee2;
        EXPECT_EQ(0x11, ee1.value());
        ee1 |= 0x100;
        EXPECT_EQ(0x111, ee1.value());
    }

    TEST(IntElement, LogicalAnd)
    {
        IntElement ee1(ids::Null, 0x101);
        IntElement ee2(ids::Null, 0x100);
        EXPECT_EQ(0x100, (ee1 & ee2).value());
        EXPECT_EQ(0x100, (ee1 & 0x100).value());
        EXPECT_EQ(0x100, (0x101 & ee2).value());
        ee1 &= ee2;
        EXPECT_EQ(0x100, ee1.value());
        ee1 &= 0x000;
        EXPECT_EQ(0x000, ee1.value());
    }

    TEST(IntElement, LogicalXor)
    {
        IntElement ee1(ids::Null, 0x101);
        IntElement ee2(ids::Null, 0x100);
        EXPECT_EQ(0x001, (ee1 ^ ee2).value());
        EXPECT_EQ(0x001, (ee1 ^ 0x100).value());
        EXPECT_EQ(0x001, (0x101 ^ ee2).value());
        ee1 ^= ee2;
        EXPECT_EQ(0x001, ee1.value());
        ee1 ^= 0x001;
        EXPECT_EQ(0x000, ee1.value());
    }

    TEST(IntElement, LogicalShiftLeft)
    {
        IntElement ee1(ids::Null, 1);
        IntElement ee2(ids::Null, 2);
        EXPECT_EQ(4, (ee1 << ee2).value());
        EXPECT_EQ(4, (ee1 << 2).value());
        ee1 <<= ee2;
        EXPECT_EQ(4, ee1.value());
        ee1.value(1);
        ee1 <<= 2;
        EXPECT_EQ(4, ee1.value());
    }

    TEST(IntElement, LogicalShiftRight)
    {
        IntElement ee1(ids::Null, 4);
        IntElement ee2(ids::Null, 2);
        EXPECT_EQ(1, (ee1 >> ee2).value());
        EXPECT_EQ(1, (ee1 >> 2).value());
        ee1 >>= ee2;
        EXPECT_EQ(1, ee1.value());
        ee1.value(4);
        ee1 >>= 2;
        EXPECT_EQ(1, ee1.value());
    }

    TEST(IntElement, Increment)
    {
        IntElement ee1(ids::Null, 1);
        EXPECT_EQ(2, (++ee1).value());
        ee1++;
        EXPECT_EQ(3, ee1.value());
    }

    TEST(IntElement, Decrement)
    {
        IntElement ee1(ids::Null, 2);
        EXPECT_EQ(1, (--ee1).value());
        ee1--;
        EXPECT_EQ(0, ee1.value());
    }


    ///////////////////////////////////////////////////////////////////////////
    // PrimitiveElement interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST(IntElement, Value)
    {
        EXPECT_EQ(1, IntElement(ids::Null, 1).value());
        EXPECT_EQ(1, IntElement(ids::Null, 1, 2).value());

        IntElement ee1(ids::Null, 1);
        EXPECT_EQ(1, ee1.value());
        ee1.value(2);
        EXPECT_EQ(2, ee1.value());

        IntElement ee2(ids::Null, 1, 2);
        ee2.value(3);
        EXPECT_EQ(3, ee2.value());
    }

    TEST(IntElement, Default)
    {
        EXPECT_FALSE(IntElement(ids::Null, 1).has_default());
        EXPECT_TRUE(IntElement(ids::Null, 1, 1).has_default());

        IntElement ee1(ids::Null, 1, 1);
        EXPECT_EQ(1, ee1.get_default());
        EXPECT_TRUE(ee1.has_default());
        ee1.remove_default();
        EXPECT_FALSE(ee1.has_default());
        ee1.set_default(2);
        EXPECT_TRUE(ee1.has_default());
        EXPECT_EQ(2, ee1.get_default());

        IntElement ee2(ids::Null, 1);
        EXPECT_FALSE(ee2.has_default());
        ee2.set_default(1);
        EXPECT_TRUE(ee2.has_default());
        EXPECT_EQ(1, ee2.get_default());
        ee2.remove_default();
        EXPECT_FALSE(ee2.has_default());

        IntElement ee3(ids::Null, 1);
        EXPECT_FALSE(ee3.is_default());
        ee3.set_default(1);
        EXPECT_TRUE(ee3.is_default());
        ee3.set_default(2);
        EXPECT_FALSE(ee3.is_default());
        ee3.value(2);
        EXPECT_TRUE(ee3.is_default());
    }

    TEST(IntElement, Equal)
    {
        IntElement ee1(ids::Null, 1);
        IntElement ee2(ids::Null, 1);
        IntElement ee3(ids::Null, 2);
        EXPECT_TRUE(ee1 == ee2);
        EXPECT_TRUE(ee1 == 1);
        EXPECT_TRUE(1 == ee1);
        EXPECT_FALSE(ee1 == ee3);
        EXPECT_FALSE(ee1 == 2);
        EXPECT_FALSE(2 == ee1);
    }


    ///////////////////////////////////////////////////////////////////////////
    // Element interface tests
    ///////////////////////////////////////////////////////////////////////////

    std::streamsize fill_buffer(std::string& b, ids::ID id, int64_t data,
            bool write_id, bool write_size, bool write_body)
    {
        std::streamsize size(0), total(0);
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
            size = ebml_int::size_s(data);
            std::vector<char> tmp(vint::encode(size));
            b.append(&tmp[0], 0, tmp.size());
            total += tmp.size();
        }
        if (write_body)
        {
            std::vector<char> tmp(ebml_int::encode_s(data));
            b.append(&tmp[0], 0, tmp.size());
            total += tmp.size();
        }
        return total;
    }

    TEST(IntElement, ID)
    {
        IntElement ee(42, 1);
        EXPECT_EQ(42, ee.id());
    }

    TEST(IntElement, Offset)
    {
        IntElement ee(ids::Null, 1);
        EXPECT_EQ(0, ee.offset());

        std::stringstream ss;
        std::string input_val;
        int64_t value1(424242), value2(42);

        // Set when read test
        IntElement ee1(ids::Null, value1), ee2(ids::Null, 0);
        write(ee1, ss);
        ids::write(ids::Null, ss);
        fill_buffer(input_val, ids::Null, value2, false, true, true);
        ss << input_val;
        ss.seekg(ee1.stored_size() + ids::size(ids::Null));
        ee2.read(ss);
        EXPECT_EQ(ee1.stored_size(), ee2.offset());

        // Set when written test
        write(ee2, ss);
        EXPECT_EQ(ee1.stored_size() + ee2.stored_size(), ee2.offset());
    }

    TEST(IntElement, StoredSize)
    {
        IntElement ee(ids::Null, 1);
        EXPECT_EQ(3, ee.stored_size());

        ee.value(0x7FFFFF);
        EXPECT_EQ(5, ee.stored_size());

        ee.value(0xFFFFFF);
        EXPECT_EQ(6, ee.stored_size());
    }

    TEST(IntElement, Read)
    {
        std::istringstream input;
        std::string input_val;
        int64_t value(5);
        std::streamsize val_size(ebml_int::size_s(value));

        IntElement ee(ids::Null, 0);
        fill_buffer(input_val, ids::Null, value, false, true, true);
        input.str(input_val);
        EXPECT_EQ(vint::size(val_size) + val_size, ee.read(input));
        EXPECT_EQ(ids::Null, ee.id());
        EXPECT_EQ(value, ee.value());

        value = 0x3A958BCD99l;
        val_size = ebml_int::size_s(value);
        ee.value(0);
        ee.set_default(0);
        EXPECT_TRUE(ee.has_default());
        EXPECT_TRUE(ee.is_default());
        std::string().swap(input_val);
        fill_buffer(input_val, ids::Null, value, false, true, true);
        input.str(input_val);
        EXPECT_EQ(vint::size(val_size) + val_size, ee.read(input));
        EXPECT_EQ(value, ee.value());
        EXPECT_EQ(0, ee.get_default());
        EXPECT_FALSE(ee.is_default());

        // Test for ReadError exception
        std::string().swap(input_val);
        fill_buffer(input_val, ids::Null, value, false, true, true);
        input.str(input_val.substr(0, 4));
        EXPECT_THROW(ee.read(input), ReadError);
        // Test for ReadWhileWriting exception
        std::stringstream output;
        ee.start_write(output);
        EXPECT_THROW(ee.read(output), ReadWhileWriting);
    }

    TEST(IntElement, StartWrite)
    {
        std::stringstream output;
        std::string expected;
        int64_t value(2);
        std::streamsize val_size(ebml_int::size_s(value));

        IntElement ee(ids::Null, value);
        output.str(std::string());
        std::string().swap(expected);
        fill_buffer(expected, ids::Null, value, true, true, true);
        EXPECT_EQ(ids::size(ids::Null) + vint::size(val_size) +
                val_size, ee.start_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

        value = -0x839F18AAl;
        val_size = ebml_int::size_s(value);
        ee.value(value);
        output.str(std::string());
        std::string().swap(expected);
        fill_buffer(expected, ids::Null, value, true, true, true);
        EXPECT_EQ(ids::size(ids::Null) + vint::size(val_size) +
                val_size, ee.start_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
    }

    TEST(IntElement, FinishWrite)
    {
        std::stringstream output;
        std::string expected;
        int64_t value(2);
        std::streamsize val_size(ebml_int::size_s(value));
        IntElement ee(ids::Null, value);

        // Test for exception and no change to output when finishing without
        // starting
        output.str(std::string());
        std::string().swap(expected);
        EXPECT_THROW(ee.finish_write(output), NotWriting);
        EXPECT_EQ(0, output.tellp());

        output.str(std::string());
        std::string().swap(expected);
        fill_buffer(expected, ids::Null, value, true, true, true);
        ee.start_write(output);
        EXPECT_EQ(ids::size(ids::Null) + vint::size(val_size) +
                val_size, ee.finish_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

        value = -0x839F18AAl;
        val_size = ebml_int::size_s(value);
        ee.value(value);
        output.str(std::string());
        std::string().swap(expected);
        fill_buffer(expected, ids::Null, value, true, true, true);
        ee.start_write(output);
        EXPECT_EQ(ids::size(ids::Null) + vint::size(val_size) +
                val_size, ee.finish_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

        // Test for double-finish raising an exception
        EXPECT_THROW(ee.finish_write(output), NotWriting);
    }

    TEST(IntElement, Write)
    {
        std::stringstream output;
        std::string expected;
        int64_t value(2);
        std::streamsize val_size(ebml_int::size_s(value));

        IntElement ee(ids::Null, value);
        output.str(std::string());
        std::string().swap(expected);
        fill_buffer(expected, ids::Null, value, true, true, true);
        EXPECT_EQ(ids::size(ids::Null) + vint::size(val_size) +
                val_size, write(ee, output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

        value = -0x839F18AAl;
        val_size = ebml_int::size_s(value);
        ee.value(value);
        output.str(std::string());
        std::string().swap(expected);
        fill_buffer(expected, ids::Null, value, true, true, true);
        EXPECT_EQ(ids::size(ids::Null) + vint::size(val_size) +
                val_size, write(ee, output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
    }
}; // namespace test_int_el

