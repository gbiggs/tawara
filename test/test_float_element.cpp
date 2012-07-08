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
#include <celduin/ebml_integer.h>
#include <celduin/exceptions.h>
#include <celduin/float_element.h>

#include "test_utilities.h"

using namespace celduin;

namespace test_float_el
{
    ///////////////////////////////////////////////////////////////////////////
    // FloatElement interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST(FloatElement, Construction)
    {
        EXPECT_EQ(ids::Null, FloatElement(ids::Null, 0).id());
        EXPECT_DOUBLE_EQ(4.2, FloatElement(ids::Null, 4.2).value());
        EXPECT_DOUBLE_EQ(4.2, FloatElement(ids::Null, 4.2, 0).value());
        EXPECT_DOUBLE_EQ(0, FloatElement(ids::Null, 4.2, 0).get_default());
        EXPECT_FALSE(FloatElement(ids::Null, 4.2).has_default());
        EXPECT_TRUE(FloatElement(ids::Null, 4.2, 0).has_default());

        EXPECT_THROW(FloatElement(0x00, 1), InvalidElementID);
        EXPECT_THROW(FloatElement(0xFF, 1), InvalidElementID);
        EXPECT_THROW(FloatElement(0xFFFF, 1), InvalidElementID);
        EXPECT_THROW(FloatElement(0xFFFFFF, 1), InvalidElementID);
        EXPECT_THROW(FloatElement(0xFFFFFFFF, 1), InvalidElementID);

        EXPECT_THROW(FloatElement(0x00, 1, 2), InvalidElementID);
        EXPECT_THROW(FloatElement(0xFF, 1, 2), InvalidElementID);
        EXPECT_THROW(FloatElement(0xFFFF, 1, 2), InvalidElementID);
        EXPECT_THROW(FloatElement(0xFFFFFF, 1, 2), InvalidElementID);
        EXPECT_THROW(FloatElement(0xFFFFFFFF, 1, 2), InvalidElementID);
    }

    TEST(FloatElement, CopyConstruction)
    {
        FloatElement ee1(ids::Null, 4.2);

        FloatElement ee2(ee1);
        EXPECT_EQ(ids::Null, ee2.id());
        EXPECT_EQ(4.2, ee2.value());
        EXPECT_FALSE(ee2.has_default());

        FloatElement ee3(ids::Null, 2.4, 8.4);
        FloatElement ee4(ee3);
        EXPECT_EQ(ids::Null, ee4.id());
        EXPECT_EQ(2.4, ee4.value());
        EXPECT_TRUE(ee4.has_default());
        EXPECT_EQ(8.4, ee4.get_default());
    }

    TEST(FloatElement, Assignment)
    {
        FloatElement ee1(ids::Null, 4.2), ee2(0x81, 0);
        ee2 = ee1;
        EXPECT_EQ(ee2, ee1);

        FloatElement ee3(ids::Null, 2.4, 8.4), ee4(0x81, 0);
        ee4 = ee3;
        EXPECT_EQ(ee4, ee3);

        ee4 = 16.8;
        EXPECT_EQ(16.8, ee4.value());
    }

    TEST(FloatElement, Swap)
    {
        FloatElement ee1(0x21, 1.2, 1);
        FloatElement ee2(0x42, 2.4, 2);
        swap(ee1, ee2);
        EXPECT_EQ(0x42, ee1.id());
        EXPECT_DOUBLE_EQ(2.4, ee1.value());
        EXPECT_DOUBLE_EQ(2, ee1.get_default());
        EXPECT_EQ(0x21, ee2.id());
        EXPECT_DOUBLE_EQ(1.2, ee2.value());
        EXPECT_DOUBLE_EQ(1, ee2.get_default());

        ee1.swap(ee2);
        EXPECT_EQ(0x21, ee1.id());
        EXPECT_DOUBLE_EQ(1.2, ee1.value());
        EXPECT_DOUBLE_EQ(1, ee1.get_default());
        EXPECT_EQ(0x42, ee2.id());
        EXPECT_DOUBLE_EQ(2.4, ee2.value());
        EXPECT_DOUBLE_EQ(2, ee2.get_default());

        double temp(3.14159);
        swap(ee1, temp);
        EXPECT_EQ(0x21, ee1.id());
        EXPECT_DOUBLE_EQ(3.14159, ee1.value());
        EXPECT_EQ(1.2, temp);
        EXPECT_DOUBLE_EQ(1, ee1.get_default());
    }

    TEST(FloatElement, LessThan)
    {
        FloatElement ee1(ids::Null, 2.4);
        FloatElement ee2(ids::Null, 4.2);
        EXPECT_TRUE(ee1 < ee2);
        EXPECT_TRUE(ee1 < 4.2);
        EXPECT_TRUE(2.4 < ee2);
        EXPECT_FALSE(ee2 < ee1);
        EXPECT_FALSE(ee2 < 2.4);
        EXPECT_FALSE(4.2 < ee1);
    }

    TEST(FloatElement, LessThanEqual)
    {
        FloatElement ee1(ids::Null, 2.4);
        FloatElement ee2(ids::Null, 2.4);
        FloatElement ee3(ids::Null, 4.2);
        EXPECT_TRUE(ee1 <= ee2);
        EXPECT_TRUE(ee1 <= ee3);
        EXPECT_TRUE(ee1 <= 2.4);
        EXPECT_TRUE(ee1 <= 4.2);
        EXPECT_TRUE(2.4 <= ee3);
        EXPECT_TRUE(4.2 <= ee3);
        EXPECT_FALSE(ee3 <= ee2);
        EXPECT_FALSE(ee3 <= 2.4);
        EXPECT_FALSE(8.4 <= ee3);
    }

    TEST(FloatElement, NotEqual)
    {
        FloatElement ee1(ids::Null, 2.4);
        FloatElement ee2(ids::Null, 2.4);
        FloatElement ee3(ids::Null, 4.2);
        EXPECT_FALSE(ee1 != ee2);
        EXPECT_FALSE(ee1 != 2.4);
        EXPECT_FALSE(2.4 != ee1);
        EXPECT_TRUE(ee1 != ee3);
        EXPECT_TRUE(ee1 != 4.2);
        EXPECT_TRUE(4.2 != ee1);
    }

    TEST(FloatElement, GreaterThanEqual)
    {
        FloatElement ee1(ids::Null, 4.2);
        FloatElement ee2(ids::Null, 4.2);
        FloatElement ee3(ids::Null, 2.4);
        EXPECT_TRUE(ee1 >= ee2);
        EXPECT_TRUE(ee1 >= ee3);
        EXPECT_TRUE(ee1 >= 2.4);
        EXPECT_TRUE(ee1 >= 4.2);
        EXPECT_TRUE(2.4 >= ee3);
        EXPECT_TRUE(4.2 >= ee3);
        EXPECT_FALSE(ee3 >= ee2);
        EXPECT_FALSE(ee3 >= 4.2);
        EXPECT_FALSE(1.2 >= ee3);
    }

    TEST(FloatElement, GreaterThan)
    {
        FloatElement ee1(ids::Null, 4.2);
        FloatElement ee2(ids::Null, 2.4);
        EXPECT_TRUE(ee1 > ee2);
        EXPECT_TRUE(ee1 > 2.4);
        EXPECT_TRUE(4.2 > ee2);
        EXPECT_FALSE(ee2 > ee1);
        EXPECT_FALSE(ee2 > 4.2);
        EXPECT_FALSE(2.4 > ee1);
    }

    TEST(FloatElement, Addition)
    {
        FloatElement ee1(ids::Null, 2.4);
        FloatElement ee2(ids::Null, 4.2);
        EXPECT_DOUBLE_EQ(6.6, (ee1 + ee2).value());
        EXPECT_DOUBLE_EQ(6.6, (ee1 + 4.2).value());
        EXPECT_DOUBLE_EQ(6.6, (2.4 + ee2).value());
        ee1 += ee2;
        EXPECT_DOUBLE_EQ(6.6, ee1.value());
        ee2 += 2.4;
        EXPECT_DOUBLE_EQ(6.6, ee2.value());
    }

    TEST(FloatElement, Subtraction)
    {
        FloatElement ee1(ids::Null, 2.4);
        FloatElement ee2(ids::Null, 4.2);
        EXPECT_DOUBLE_EQ(-1.8, (ee1 - ee2).value());
        EXPECT_DOUBLE_EQ(-1.8, (ee1 - 4.2).value());
        EXPECT_DOUBLE_EQ(-1.8, 2.4 - ee2);
        ee1 -= ee2;
        EXPECT_DOUBLE_EQ(-1.8, ee1.value());
        ee2 -= 2.4;
        EXPECT_DOUBLE_EQ(1.8, ee2.value());
    }

    TEST(FloatElement, Multiplication)
    {
        FloatElement ee1(ids::Null, 2.4);
        FloatElement ee2(ids::Null, 4.2);
        EXPECT_DOUBLE_EQ(10.08, (ee1 * ee2).value());
        EXPECT_DOUBLE_EQ(10.08, (ee1 * 4.2).value());
        EXPECT_DOUBLE_EQ(10.08, (2.4 * ee2).value());
        ee1 *= ee2;
        EXPECT_DOUBLE_EQ(10.08, ee1.value());
        ee2 *= 2.4;
        EXPECT_DOUBLE_EQ(10.08, ee2.value());
    }

    TEST(FloatElement, Division)
    {
        FloatElement ee1(ids::Null, 4.0);
        FloatElement ee2(ids::Null, 2.5);
        EXPECT_DOUBLE_EQ(1.6, (ee1 / ee2).value());
        EXPECT_DOUBLE_EQ(1.6, (ee1 / 2.5).value());
        EXPECT_DOUBLE_EQ(1.6, 4.0 / ee2);
        ee1 /= ee2;
        EXPECT_DOUBLE_EQ(1.6, ee1.value());
        ee2 /= 2.5;
        EXPECT_DOUBLE_EQ(1.0, ee2.value());
    }

    TEST(FloatElement, Increment)
    {
        FloatElement ee1(ids::Null, 4.2);
        EXPECT_DOUBLE_EQ(5.2, (++ee1).value());
        ee1++;
        EXPECT_DOUBLE_EQ(6.2, ee1.value());
    }

    TEST(FloatElement, Decrement)
    {
        FloatElement ee1(ids::Null, 4.2);
        EXPECT_DOUBLE_EQ(3.2, (--ee1).value());
        ee1--;
        EXPECT_DOUBLE_EQ(2.2, ee1.value());
    }

    TEST(FloatElement, StreamOut)
    {
        std::stringstream ss;
        FloatElement ee1(ids::Null, 2.4);
        ss << ee1;
        EXPECT_EQ("2.4", ss.str());
    }

    TEST(FloatElement, StreamIn)
    {
        std::stringstream ss;
        FloatElement ee1(ids::Null, 0);
        ss << 2.4;
        ss >> ee1;
        EXPECT_EQ(2.4, ee1.value());
    }


    ///////////////////////////////////////////////////////////////////////////
    // PrimitiveElement interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST(FloatElement, Value)
    {
        EXPECT_DOUBLE_EQ(4.2, FloatElement(ids::Null, 4.2).value());
        EXPECT_DOUBLE_EQ(4.2, FloatElement(ids::Null, 4.2, 2.4).value());

        FloatElement ee1(ids::Null, 4.2);
        EXPECT_DOUBLE_EQ(4.2, ee1.value());
        ee1.value(2.4);
        EXPECT_DOUBLE_EQ(2.4, ee1.value());

        FloatElement ee2(ids::Null, 4.2, 2.4);
        ee2.value(8.4);
        EXPECT_DOUBLE_EQ(8.4, ee2.value());
    }

    TEST(FloatElement, Default)
    {
        EXPECT_FALSE(FloatElement(ids::Null, 4.2).has_default());
        EXPECT_TRUE(FloatElement(ids::Null, 4.2, 2.4).has_default());

        FloatElement ee1(ids::Null, 4.2, 2.4);
        EXPECT_DOUBLE_EQ(2.4, ee1.get_default());
        EXPECT_TRUE(ee1.has_default());
        ee1.remove_default();
        EXPECT_FALSE(ee1.has_default());
        ee1.set_default(8.4);
        EXPECT_TRUE(ee1.has_default());
        EXPECT_DOUBLE_EQ(8.4, ee1.get_default());

        FloatElement ee2(ids::Null, 4.2);
        EXPECT_FALSE(ee2.has_default());
        ee2.set_default(2.4);
        EXPECT_TRUE(ee2.has_default());
        EXPECT_DOUBLE_EQ(2.4, ee2.get_default());
        ee2.remove_default();
        EXPECT_FALSE(ee2.has_default());

        FloatElement ee3(ids::Null, 4.2);
        EXPECT_FALSE(ee3.is_default());
        ee3.set_default(4.2);
        EXPECT_TRUE(ee3.is_default());
        ee3.set_default(2.4);
        EXPECT_FALSE(ee3.is_default());
        ee3.value(2.4);
        EXPECT_TRUE(ee3.is_default());
    }

    TEST(FloatElement, Equal)
    {
        FloatElement ee1(ids::Null, 4.2);
        FloatElement ee2(ids::Null, 4.2);
        FloatElement ee3(ids::Null, 2.4);
        EXPECT_TRUE(ee1 == ee2);
        EXPECT_TRUE(ee1 == 4.2);
        EXPECT_TRUE(4.2 == ee1);
        EXPECT_FALSE(ee1 == ee3);
        EXPECT_FALSE(ee1 == 2.4);
        EXPECT_FALSE(2.4 == ee1);
    }


    ///////////////////////////////////////////////////////////////////////////
    // Element interface tests
    ///////////////////////////////////////////////////////////////////////////

    std::streamsize fill_buffer(std::string& b, ids::ID id, double data,
            bool write_id, bool write_size, bool write_body, bool double_prec)
    {
        char temp[8];
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
            if (double_prec)
            {
                std::vector<char> tmp(vint::encode(8));
                b.append(&tmp[0], 0, tmp.size());
                total += tmp.size();
            }
            else
            {
                std::vector<char> tmp(vint::encode(4));
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

    TEST(FloatElement, ID)
    {
        FloatElement ee(42, 1.5);
        EXPECT_EQ(42, ee.id());
    }

    TEST(FloatElement, Offset)
    {
        FloatElement ee(ids::Null, 1.5);
        EXPECT_EQ(0, ee.offset());

        std::stringstream ss;
        std::string input_val;
        double value1(4.2), value2(2.4);

        // Set when read test
        FloatElement ee1(ids::Null, value1), ee2(ids::Null, 0);
        write(ee1, ss);
        fill_buffer(input_val, ids::Null, value2, true, true, true, true);
        ss << input_val;
        ss.seekg(ee1.stored_size() + ids::size(ids::Null));
        ee2.read(ss);
        EXPECT_EQ(ee1.stored_size(), ee2.offset());

        // Set when written test
        write(ee2, ss);
        EXPECT_EQ(ee1.stored_size() + ee2.stored_size(), ee2.offset());
    }

    TEST(FloatElement, StoredSize)
    {
        FloatElement ee(ids::Null, 1.0, EBML_FLOAT_PREC_SINGLE);
        EXPECT_EQ(ids::size(ids::Null) + vint::size(4) + 4, ee.stored_size());
        ee.precision(EBML_FLOAT_PREC_DOUBLE);
        EXPECT_EQ(ids::size(ids::Null) + vint::size(8) + 8, ee.stored_size());

        ee.precision(EBML_FLOAT_PREC_SINGLE);
        ee.value(3.15149);
        EXPECT_EQ(ids::size(ids::Null) + vint::size(4) + 4, ee.stored_size());
        ee.precision(EBML_FLOAT_PREC_DOUBLE);
        EXPECT_EQ(ids::size(ids::Null) + vint::size(8) + 8, ee.stored_size());
    }

    TEST(FloatElement, Read)
    {
        std::istringstream input;
        std::string input_val;
        double value(23.14069);

        FloatElement ee(ids::Null, 0);
        fill_buffer(input_val, 0x01, value, false, true, true, true);
        input.str(input_val);
        EXPECT_EQ(9, ee.read(input));
        EXPECT_EQ(ids::Null, ee.id());
        EXPECT_DOUBLE_EQ(value, ee.value());
        EXPECT_EQ(EBML_FLOAT_PREC_DOUBLE, ee.precision());

        value = 22.45916;
        ee.value(0);
        ee.set_default(0);
        EXPECT_TRUE(ee.has_default());
        EXPECT_TRUE(ee.is_default());
        std::string().swap(input_val);
        fill_buffer(input_val, 0x01, value, false, true, true, false);
        input.str(input_val);
        EXPECT_EQ(5, ee.read(input));
        EXPECT_FLOAT_EQ(value, ee.value());
        EXPECT_EQ(0, ee.get_default());
        EXPECT_FALSE(ee.is_default());
        EXPECT_EQ(EBML_FLOAT_PREC_SINGLE, ee.precision());

        // Test for ReadError exception
        std::string().swap(input_val);
        fill_buffer(input_val, 0x01, value, false, true, true, true);
        input.str(input_val.substr(0, 4));
        EXPECT_THROW(ee.read(input), ReadError);
        // Test for ReadError exception
        std::string().swap(input_val);
        fill_buffer(input_val, 0x01, value, false, true, true, true);
        input.str(input_val.substr(0, 4));
        EXPECT_THROW(ee.read(input), ReadError);
    }

    TEST(FloatElement, StartWrite)
    {
        std::stringstream output;
        std::string expected;
        double value(2.7182818284590451);
        FloatElement ee(ids::Null, value, EBML_FLOAT_PREC_DOUBLE);

        fill_buffer(expected, ids::Null, value, true, true, true, true);
        EXPECT_EQ(ids::size(ids::Null) + 9, ee.start_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Null) + 9, output.tellp());

        value = 3.14159;
        ee.value(value);
        ee.precision(EBML_FLOAT_PREC_SINGLE);
        output.str(std::string());
        std::string().swap(expected);
        fill_buffer(expected, ids::Null, value, true, true, true, false);
        EXPECT_EQ(ids::size(ids::Null) + 5, ee.start_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Null) + 5, output.tellp());
    }

    TEST(FloatElement, FinishWrite)
    {
        std::stringstream output;
        std::string expected;
        double value(2.7182818284590451);
        FloatElement ee(ids::Null, value, EBML_FLOAT_PREC_DOUBLE);

        // Test for exception and no change to output when finishing without
        // starting
        EXPECT_THROW(ee.finish_write(output), NotWriting);
        EXPECT_EQ(0, output.tellp());

        fill_buffer(expected, ids::Null, value, true, true, true, true);
        ee.start_write(output);
        EXPECT_EQ(ids::size(ids::Null) + 9, ee.finish_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Null) + 9, output.tellp());

        value = 3.14159;
        ee.value(value);
        ee.precision(EBML_FLOAT_PREC_SINGLE);
        output.str(std::string());
        std::string().swap(expected);
        fill_buffer(expected, ids::Null, value, true, true, true, false);
        ee.start_write(output);
        EXPECT_EQ(ids::size(ids::Null) + 5, ee.finish_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Null) + 5, output.tellp());

        // Test for double-finish raising an exception
        EXPECT_THROW(ee.finish_write(output), NotWriting);
    }

    TEST(FloatElement, Write)
    {
        std::stringstream output;
        std::string expected;
        double value(2.7182818284590451);
        FloatElement ee(ids::Null, value, EBML_FLOAT_PREC_DOUBLE);

        fill_buffer(expected, ids::Null, value, true, true, true, true);
        EXPECT_EQ(ids::size(ids::Null) + 9, write(ee, output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Null) + 9, output.tellp());

        value = 3.14159;
        ee.value(value);
        ee.precision(EBML_FLOAT_PREC_SINGLE);
        output.str(std::string());
        std::string().swap(expected);
        fill_buffer(expected, ids::Null, value, true, true, true, false);
        EXPECT_EQ(ids::size(ids::Null) + 5, write(ee, output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Null) + 5, output.tellp());
    }
}; // namespace test_float_el

