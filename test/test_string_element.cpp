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
#include <tide/ebml_integer.h>
#include <tide/exceptions.h>
#include <tide/string_element.h>
#include <tide/tide_config.h>

#include <limits>

#include "test_utilities.h"

using namespace tide;

namespace test_string_el
{
    class StringElementTest : public ::testing::Test
    {
        public:
            virtual void SetUp()
            {
                for (char ii(0); ii < 8; ++ii)
                {
                    s1.push_back('a' + ii);
                    s4.push_back('z' - ii);
                }
                for (char ii(0); ii < 12; ++ii)
                {
                    s2.push_back('m' - ii);
                    s3.push_back('n' + ii);
                }
            }

            std::string s1;
            std::string s2;
            std::string s3;
            std::string s4;
    }; // class TestStringElement


    ///////////////////////////////////////////////////////////////////////////
    // StringElement interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST_F(StringElementTest, Construction)
    {
        EXPECT_EQ(ids::Null, StringElement(ids::Null, s1).id());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s1,
                StringElement(ids::Null, s1).value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s1,
                StringElement(ids::Null, s1, s2).value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s2,
                StringElement(ids::Null, s1, s2).get_default());
        EXPECT_FALSE(StringElement(ids::Null, s1).has_default());
        EXPECT_TRUE(StringElement(ids::Null, s1, s2).has_default());

        EXPECT_THROW(StringElement(0x00, s1), InvalidElementID);
        EXPECT_THROW(StringElement(0xFF, s1), InvalidElementID);
        EXPECT_THROW(StringElement(0xFFFF, s1), InvalidElementID);
        EXPECT_THROW(StringElement(0xFFFFFF, s1), InvalidElementID);
        EXPECT_THROW(StringElement(0xFFFFFFFF, s1), InvalidElementID);

        EXPECT_THROW(StringElement(0x00, s1, s2), InvalidElementID);
        EXPECT_THROW(StringElement(0xFF, s1, s2), InvalidElementID);
        EXPECT_THROW(StringElement(0xFFFF, s1, s2), InvalidElementID);
        EXPECT_THROW(StringElement(0xFFFFFF, s1, s2), InvalidElementID);
        EXPECT_THROW(StringElement(0xFFFFFFFF, s1, s2), InvalidElementID);
    }

    TEST_F(StringElementTest, CopyConstruction)
    {
        StringElement ee1(ids::Null, s1);

        StringElement ee2(ee1);
        EXPECT_EQ(ids::Null, ee2.id());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s1, ee2.value());
        EXPECT_FALSE(ee2.has_default());

        StringElement ee3(ids::Null, s2, s3);
        StringElement ee4(ee3);
        EXPECT_EQ(ids::Null, ee4.id());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s2, ee4.value());
        EXPECT_TRUE(ee4.has_default());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s3, ee4.get_default());
    }

    TEST_F(StringElementTest, Assignment)
    {
        StringElement ee1(ids::Null, s1), ee2(0x81, std::string());
        ee2 = ee1;
        EXPECT_EQ(ee2, ee1);

        StringElement ee3(ids::Null, s2, s3), ee4(0x81, std::string());
        ee4 = ee3;
        EXPECT_EQ(ee4, ee3);

        ee4 = s4;
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s4, ee4.value());
    }

    TEST_F(StringElementTest, Swap)
    {
        StringElement ee1(0x21, s1, s2);
        StringElement ee2(0x42, s3, s4);
        swap(ee1, ee2);
        EXPECT_EQ(0x42, ee1.id());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s3, ee1.value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s4, ee1.get_default());
        EXPECT_EQ(0x21, ee2.id());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s1, ee2.value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s2, ee2.get_default());

        ee1.swap(ee2);
        EXPECT_EQ(0x21, ee1.id());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s1, ee1.value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s2, ee1.get_default());
        EXPECT_EQ(0x42, ee2.id());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s3, ee2.value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s4, ee2.get_default());

        std::string s2_copy(s2);
        swap(ee1, s2_copy);
        EXPECT_EQ(0x21, ee1.id());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s2, ee1.value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s2_copy, s1);
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s2, ee1.get_default());
    }

    TEST_F(StringElementTest, LessThan)
    {
        StringElement ee1(ids::Null, s1);
        StringElement ee2(ids::Null, s4);
        EXPECT_TRUE(ee1 < ee2);
        EXPECT_TRUE(ee1 < s4);
        EXPECT_TRUE(s1 < ee2);
        EXPECT_FALSE(ee2 < ee1);
        EXPECT_FALSE(ee2 < s1);
        EXPECT_FALSE(s4 < ee1);
    }

    TEST_F(StringElementTest, LessThanEqual)
    {
        StringElement ee1(ids::Null, s1);
        StringElement ee2(ids::Null, s1);
        StringElement ee3(ids::Null, s2);
        EXPECT_TRUE(ee1 <= ee2);
        EXPECT_TRUE(ee1 <= ee3);
        EXPECT_TRUE(ee1 <= s1);
        EXPECT_TRUE(ee1 <= s2);
        EXPECT_TRUE(s1 <= ee3);
        EXPECT_TRUE(s2 <= ee3);
        EXPECT_FALSE(ee3 <= ee2);
        EXPECT_FALSE(ee3 <= s1);
        EXPECT_FALSE(s3 <= ee3);
    }

    TEST_F(StringElementTest, NotEqual)
    {
        StringElement ee1(ids::Null, s1);
        StringElement ee2(ids::Null, s1);
        StringElement ee3(ids::Null, s2);
        EXPECT_FALSE(ee1 != ee2);
        EXPECT_FALSE(ee1 != s1);
        EXPECT_FALSE(s1 != ee1);
        EXPECT_TRUE(ee1 != ee3);
        EXPECT_TRUE(ee1 != s2);
        EXPECT_TRUE(s2 != ee1);
    }

    TEST_F(StringElementTest, GreaterThanEqual)
    {
        StringElement ee1(ids::Null, s3);
        StringElement ee2(ids::Null, s3);
        StringElement ee3(ids::Null, s2);
        EXPECT_TRUE(ee1 >= ee2);
        EXPECT_TRUE(ee1 >= ee3);
        EXPECT_TRUE(ee1 >= s2);
        EXPECT_TRUE(ee1 >= s3);
        EXPECT_TRUE(s2 >= ee3);
        EXPECT_TRUE(s3 >= ee3);
        EXPECT_FALSE(ee3 >= ee2);
        EXPECT_FALSE(ee3 >= s3);
        EXPECT_FALSE(s1 >= ee3);
    }

    TEST_F(StringElementTest, GreaterThan)
    {
        StringElement ee1(ids::Null, s2);
        StringElement ee2(ids::Null, s1);
        EXPECT_TRUE(ee1 > ee2);
        EXPECT_TRUE(ee1 > s1);
        EXPECT_TRUE(s2 > ee2);
        EXPECT_FALSE(ee2 > ee1);
        EXPECT_FALSE(ee2 > s2);
        EXPECT_FALSE(s1 > ee1);
    }

    TEST_F(StringElementTest, StreamOut)
    {
        std::stringstream ss;
        StringElement ee1(ids::Null, s1);
        ss << ee1;
        EXPECT_EQ("\"abcdefgh\"", ss.str());
    }


    TEST_F(StringElementTest, Padding)
    {
        StringElement ee1(ids::Null, s1);
        EXPECT_EQ(0, ee1.padding());
        EXPECT_EQ(ids::size(ids::Null) + vint::size(s1.size()) + s1.size(),
                ee1.stored_size());
        ee1.padding(10);
        EXPECT_EQ(10, ee1.padding());
        EXPECT_EQ(ids::size(ids::Null) + vint::size(s1.size()) + s1.size() +
                10, ee1.stored_size());
    }


    ///////////////////////////////////////////////////////////////////////////
    // Vector interface tests
    ///////////////////////////////////////////////////////////////////////////

    /*TEST_F(StringElementTest, Assign)
    {
        // Copies-of-a-value assign
        StringElement ee(ids::Null, s1);
        std::string aaa(10, 'a');
        ee.assign(10, 'a');
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, aaa,
                ee.value());

        // Iterator range assign
        std::string bbb(10, 'b');
        ee.assign(bbb.begin() + 5, bbb.end());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq,
                std::string(5, 'b'), ee.value());
    }


    TEST_F(StringElementTest, GetAllocator)
    {
        // TODO: How can this be tested?
    }


    TEST_F(StringElementTest, At)
    {
        StringElement ee(ids::Null, s1);
        EXPECT_EQ('a', ee.at(0));
        EXPECT_THROW(ee.at(20), std::out_of_range);

        StringElement const ee_const(ids::Null, s1);
        EXPECT_EQ('e', ee_const.at(4));
        EXPECT_THROW(ee_const.at(20), std::out_of_range);
    }


    TEST_F(StringElementTest, IndexOperator)
    {
        StringElement ee(ids::Null, s1);
        EXPECT_EQ('a', ee[0]);
        EXPECT_NO_THROW(ee[20]);

        StringElement const ee_const(ids::Null, s1);
        EXPECT_EQ('e', ee_const[4]);
        EXPECT_NO_THROW(ee_const[20]);
    }


    TEST_F(StringElementTest, Front)
    {
        StringElement ee(ids::Null, s1);
        EXPECT_EQ('a', ee.front());
        StringElement const ee_const(ids::Null, s1);
        EXPECT_EQ('a', ee_const.front());
    }


    TEST_F(StringElementTest, Back)
    {
        StringElement ee(ids::Null, s1);
        EXPECT_EQ('h', ee.back());
        StringElement const ee_const(ids::Null, s1);
        EXPECT_EQ('h', ee_const.back());
    }


    TEST_F(StringElementTest, Data)
    {
        StringElement ee(ids::Null, s1);
        EXPECT_EQ('a', *ee.data());
        StringElement const ee_const(ids::Null, s1);
        EXPECT_EQ('a', *ee_const.data());
    }


    TEST_F(StringElementTest, Begin)
    {
        StringElement ee(ids::Null, s1);
        EXPECT_EQ('a', *(ee.begin()));
#if defined(TIDE_CPLUSPLUS11_SUPPORT)
        EXPECT_EQ('a', *(ee.cbegin()));
#endif // defined(TIDE_CPLUSPLUS11_SUPPORT)
        StringElement ee_const(ids::Null, s1);
        EXPECT_EQ('a', *(ee_const.begin()));
    }


    TEST_F(StringElementTest, End)
    {
        StringElement ee(ids::Null, s1);
        EXPECT_EQ('h', *(ee.end() - 1));
#if defined(TIDE_CPLUSPLUS11_SUPPORT)
        EXPECT_EQ('h', *(ee.cend() - 1));
#endif // defined(TIDE_CPLUSPLUS11_SUPPORT)
        StringElement ee_const(ids::Null, s1);
        EXPECT_EQ('h', *(ee_const.end() - 1));
    }


    TEST_F(StringElementTest, RBegin)
    {
        StringElement ee(ids::Null, s1);
        EXPECT_EQ('h', *(ee.rbegin()));
#if defined(TIDE_CPLUSPLUS11_SUPPORT)
        EXPECT_EQ('h', *(ee.crbegin()));
#endif // defined(TIDE_CPLUSPLUS11_SUPPORT)
        StringElement ee_const(ids::Null, s1);
        EXPECT_EQ('h', *(ee_const.rbegin()));
    }


    TEST_F(StringElementTest, REnd)
    {
        StringElement ee(ids::Null, s1);
        EXPECT_EQ('a', *(ee.rend() - 1));
#if defined(TIDE_CPLUSPLUS11_SUPPORT)
        EXPECT_EQ('a', *(ee.crend() - 1));
#endif // defined(TIDE_CPLUSPLUS11_SUPPORT)
        StringElement ee_const(ids::Null, s1);
        EXPECT_EQ('a', *(ee_const.rend() - 1));
    }


    TEST_F(StringElementTest, Empty)
    {
        StringElement ee(ids::Null, std::string());
        EXPECT_TRUE(ee.empty());
        ee.value(s1);
        EXPECT_FALSE(ee.empty());
    }


    TEST_F(StringElementTest, Size)
    {
        StringElement ee(ids::Null, std::string());
        EXPECT_EQ(0, ee.size());
        ee.value(s1);
        EXPECT_EQ(8, ee.size());
    }


    TEST_F(StringElementTest, MaxSize)
    {
        // This value differs based on platform and standard library
        // implementation, so the best that can be done is test that the
        // underlying vector's value is being passed correctly.
        StringElement ee(ids::Null, std::string());
        EXPECT_EQ(std::string().max_size(), ee.max_size());
    }


    TEST_F(StringElementTest, Reserve)
    {
        StringElement ee(ids::Null, s1);
        EXPECT_EQ(8, ee.capacity());
        ee.reserve(32);
        EXPECT_EQ(32, ee.capacity());
    }


    TEST_F(StringElementTest, Capacity)
    {
        StringElement ee(ids::Null, s1);
        EXPECT_EQ(8, ee.capacity());
    }


#if defined(TIDE_CPLUSPLUS11_SUPPORT)
    TEST_F(StringElementTest, ShrinkToFit)
    {
        StringElement ee(ids::Null, s1);
        ee.capacity(32);
        EXPECT_EQ(32, ee.capacity());
        ee.shrink_to_fit();
        EXPECT_EQ(8, ee.capacity());
    }
#endif // defined(TIDE_CPLUSPLUS11_SUPPORT)


    TEST_F(StringElementTest, Clear)
    {
        StringElement ee(ids::Null, s1);
        ee.clear();
        EXPECT_EQ(8, ee.capacity());
        // Because the vector does not contain constructed values, the stored
        // contents will not change.
    }


    TEST_F(StringElementTest, Insert)
    {
        StringElement ee(ids::Null, s1);
        char z('z');
        ee.insert(ee.begin(), z);
        EXPECT_EQ('z', ee[0]);

#if defined(TIDE_CPLUSPLUS11_SUPPORT)
        ee.insert(ee.cbegin(), 'y');
        EXPECT_EQ('y', ee[0]);
#endif // defined(TIDE_CPLUSPLUS11_SUPPORT)

        ee.insert(ee.begin(), 3, 'x');
        EXPECT_EQ('x', ee[0]);
        EXPECT_EQ('x', ee[1]);
        EXPECT_EQ('x', ee[2]);

        ee.insert(ee.begin(), s3.begin(), s3.begin() + 2);
        EXPECT_EQ('n', ee[0]);
        EXPECT_EQ('o', ee[1]);
        EXPECT_EQ('x', ee[2]); // Only two elements should have been inserted

#if defined(TIDE_CPLUSPLUS11_SUPPORT)
        ee.insert(ee.begin(), {'1', '2', '3'});
        EXPECT_EQ('1', ee[0]);
        EXPECT_EQ('2', ee[1]);
        EXPECT_EQ('3', ee[2]);
#endif // defined(TIDE_CPLUSPLUS11_SUPPORT)
    }


    TEST_F(StringElementTest, Emplace)
    {
#if defined(TIDE_CPLUSPLUS11_SUPPORT)
        StringElement ee(ids::Null, s1);
        ee.emplace(ee.begin(), 'm');
        EXPECT_EQ('m', ee[0]);
#endif // defined(TIDE_CPLUSPLUS11_SUPPORT)
    }


    TEST_F(StringElementTest, Erase)
    {
        StringElement ee(ids::Null, s1);
        ee.erase(ee.begin());
        EXPECT_EQ('b', ee[0]);
        EXPECT_EQ(7, ee.size());
        ee.erase(ee.begin(), ee.begin() + 2);
        EXPECT_EQ('d', ee[0]);
        EXPECT_EQ(5, ee.size());
    }


    TEST_F(StringElementTest, PushBack)
    {
        StringElement ee(ids::Null, s1);
        ee.push_back('i');
        EXPECT_EQ('i', ee[8]);
        EXPECT_EQ(9, ee.size());
    }


    TEST_F(StringElementTest, EmplaceBack)
    {
#if defined(TIDE_CPLUSPLUS11_SUPPORT)
        StringElement ee(ids::Null, s1);
        ee.emplace_back('i');
        EXPECT_EQ('i', ee[8]);
        EXPECT_EQ(9, ee.size());
#endif // defined(TIDE_CPLUSPLUS11_SUPPORT)
    }


    TEST_F(StringElementTest, PopBack)
    {
        StringElement ee(ids::Null, s1);
        ee.pop_back();
        EXPECT_EQ(7, ee.size());
        EXPECT_EQ('g', ee.back());
    }


    TEST_F(StringElementTest, Resize)
    {
        StringElement ee(ids::Null, s1);
        ee.resize(10, 'z');
        EXPECT_EQ('z', ee[8]);
        EXPECT_EQ('z', ee[9]);
        EXPECT_EQ(10, ee.size());

        ee.resize(3);
        EXPECT_EQ('c', ee.back());
        EXPECT_EQ(3, ee.size());
    }*/


    ///////////////////////////////////////////////////////////////////////////
    // PrimitiveElement interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST_F(StringElementTest, Value)
    {
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s2,
                StringElement(ids::Null, s2).value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s2,
                StringElement(ids::Null, s2, s1).value());

        StringElement ee1(ids::Null, s2);
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s2, ee1.value());
        ee1.value(s1);
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s1, ee1.value());

        StringElement ee2(ids::Null, s2, s1);
        ee2.value(s3);
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s3, ee2.value());
        ee2.value("12345");
        std::string numbers("12345");
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, numbers, ee2.value());
    }

    TEST_F(StringElementTest, Default)
    {
        EXPECT_FALSE(StringElement(ids::Null, s2).has_default());
        EXPECT_TRUE(StringElement(ids::Null, s2, s1).has_default());

        StringElement ee1(ids::Null, s2, s1);
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s1, ee1.get_default());
        EXPECT_TRUE(ee1.has_default());
        ee1.remove_default();
        EXPECT_FALSE(ee1.has_default());
        ee1.set_default(s3);
        EXPECT_TRUE(ee1.has_default());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s3, ee1.get_default());

        StringElement ee2(ids::Null, s2);
        EXPECT_FALSE(ee2.has_default());
        ee2.set_default(s1);
        EXPECT_TRUE(ee2.has_default());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s1, ee2.get_default());
        ee2.remove_default();
        EXPECT_FALSE(ee2.has_default());

        StringElement ee3(ids::Null, s2);
        EXPECT_FALSE(ee3.is_default());
        ee3.set_default(s2);
        EXPECT_TRUE(ee3.is_default());
        ee3.set_default(s1);
        EXPECT_FALSE(ee3.is_default());
        ee3.value(s1);
        EXPECT_TRUE(ee3.is_default());
    }

    TEST_F(StringElementTest, Equal)
    {
        StringElement ee1(ids::Null, s2);
        StringElement ee2(ids::Null, s2);
        StringElement ee3(ids::Null, s1);
        EXPECT_TRUE(ee1 == ee2);
        EXPECT_TRUE(ee1 == s2);
        EXPECT_TRUE(s2 == ee1);
        EXPECT_FALSE(ee1 == ee3);
        EXPECT_FALSE(ee1 == s1);
        EXPECT_FALSE(s1 == ee1);
    }


    ///////////////////////////////////////////////////////////////////////////
    // Element interface tests
    ///////////////////////////////////////////////////////////////////////////

    std::streamsize fill_buffer(std::string& b, ids::ID id,
            std::string const& data, bool write_id, bool write_size,
            bool write_body, unsigned int padding=0)
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
            std::vector<char> tmp(vint::encode(data.size() + padding));
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

    TEST_F(StringElementTest, ID)
    {
        StringElement ee(42, s1);
        EXPECT_EQ(42, ee.id());
    }

    TEST_F(StringElementTest, Offset)
    {
        std::stringstream ss;
        std::string input_val;

        StringElement ee1(ids::Null, s1);
        StringElement ee2(ids::Null, std::string());
        EXPECT_EQ(0, ee1.offset());

        // Set when read test
        write(ee1, ss);
        fill_buffer(input_val, ids::Null, s3, true, true, true);
        ss << input_val;
        ss.seekg(ee1.stored_size() + ids::size(ids::Null));
        ee2.read(ss);
        EXPECT_EQ(ee1.stored_size(), ee2.offset());

        // Set when written test
        write(ee2, ss);
        EXPECT_EQ(ee1.stored_size() + ee2.stored_size(), ee2.offset());
    }

    TEST_F(StringElementTest, StoredSize)
    {
        StringElement ee(ids::Null, s1);
        EXPECT_EQ(ids::size(ids::Null) + vint::size(s1.size()) + s1.size(),
                ee.stored_size());
    }

    TEST_F(StringElementTest, Read)
    {
        std::istringstream input;
        std::string input_val;
        unsigned int padding(0);

        StringElement ee(ids::Null, std::string());
        fill_buffer(input_val, ids::Null, s1, false, true, true);
        input.str(input_val);
        EXPECT_EQ(vint::size(s1.size() + padding) + s1.size() + padding,
                ee.read(input));
        EXPECT_EQ(ids::Null, ee.id());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s1, ee.value());

        // String without padding
        ee.value(s2);
        ee.set_default(s2);
        EXPECT_TRUE(ee.has_default());
        EXPECT_TRUE(ee.is_default());
        std::string().swap(input_val);
        fill_buffer(input_val, ids::Null, s3, false, true, true);
        input.str(input_val);
        EXPECT_EQ(vint::size(s3.size() + padding) + s3.size() + padding,
                ee.read(input));
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s3, ee.value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s2, ee.get_default());
        EXPECT_FALSE(ee.is_default());
        EXPECT_EQ(0, ee.padding());

        // String with padding
        padding = 5;
        ee.value(s2);
        ee.set_default(s2);
        EXPECT_TRUE(ee.has_default());
        EXPECT_TRUE(ee.is_default());
        std::string().swap(input_val);
        fill_buffer(input_val, ids::Null, s3, false, true, true, padding);
        input.str(input_val);
        EXPECT_EQ(vint::size(s3.size() + padding) + s3.size() + padding,
                ee.read(input));
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s3, ee.value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s2, ee.get_default());
        EXPECT_FALSE(ee.is_default());
        EXPECT_EQ(5, ee.padding());

        // Zero-length string
        padding = 0;
        ee.padding(0);
        std::string empty;
        ee.value(s2);
        ee.set_default(s2);
        EXPECT_TRUE(ee.has_default());
        EXPECT_TRUE(ee.is_default());
        std::string().swap(input_val);
        fill_buffer(input_val, ids::Null, empty, false, true, true, padding);
        input.str(input_val);
        EXPECT_EQ(vint::size(empty.size() + padding) + empty.size() + padding,
                ee.read(input));
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, empty, ee.value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s2, ee.get_default());
        EXPECT_FALSE(ee.is_default());
        EXPECT_EQ(0, ee.padding());

        // Zero-length string
        padding = 5;
        ee.padding(0);
        ee.value(s2);
        ee.set_default(s2);
        EXPECT_TRUE(ee.has_default());
        EXPECT_TRUE(ee.is_default());
        std::string().swap(input_val);
        fill_buffer(input_val, ids::Null, empty, false, true, true, padding);
        input.str(input_val);
        EXPECT_EQ(vint::size(empty.size() + padding) + empty.size() + padding,
                ee.read(input));
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, empty, ee.value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, s2, ee.get_default());
        EXPECT_FALSE(ee.is_default());

        // Test for ReadError exception
        std::string().swap(input_val);
        fill_buffer(input_val, ids::Null, s4, false, true, true);
        input.str(input_val.substr(0, 4));
        EXPECT_THROW(ee.read(input), ReadError);
    }

    TEST_F(StringElementTest, StartWrite)
    {
        std::stringstream output;
        std::string expected;
        tide::StringElement ee(ids::Null, s1);
        unsigned int padding(0);

        output.str(std::string());
        std::string().swap(expected);
        fill_buffer(expected, ids::Null, s1, true, true, true);
        EXPECT_EQ(ids::size(ids::Null) + vint::size(s1.size()) + s1.size(),
                ee.start_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
                expected);

        padding = 5;
        ee.padding(padding);
        ee.value(s3);
        output.str(std::string());
        std::string().swap(expected);
        fill_buffer(expected, ids::Null, s3, true, true, true, padding);
        EXPECT_EQ(tide::ids::size(ids::Null) + vint::size(s3.size() + padding)
                + s3.size() + padding, ee.start_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
                expected);

        // Zero-length string
        padding = 0;
        ee.padding(0);
        std::string empty;
        ee.value(empty);
        output.str(std::string());
        std::string().swap(expected);
        fill_buffer(expected, ids::Null, empty, true, true, true);
        EXPECT_EQ(tide::ids::size(ids::Null) + vint::size(empty.size()) +
                empty.size(), ee.start_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
                expected);

        // Zero-length string with padding
        padding = 5;
        ee.value(empty);
        ee.padding(padding);
        output.str(std::string());
        std::string().swap(expected);
        fill_buffer(expected, ids::Null, empty, true, true, true, padding);
        EXPECT_EQ(ids::size(ids::Null) + vint::size(empty.size() + padding) +
                empty.size() + padding, ee.start_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, output.str(), expected);
    }

    TEST_F(StringElementTest, FinishWrite)
    {
        std::stringstream output;
        std::string expected;
        StringElement ee(ids::Null, s1);
        unsigned int padding(0);

        // Test for exception and no change to output when finishing without
        // starting
        EXPECT_THROW(ee.finish_write(output), NotWriting);
        EXPECT_EQ(0, output.tellp());

        output.str(std::string());
        std::string().swap(expected);
        fill_buffer(expected, ids::Null, s1, true, true, true);
        ee.start_write(output);
        EXPECT_EQ(ids::size(ids::Null) + vint::size(s1.size()) + s1.size(),
                ee.finish_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
                expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Null) + vint::size(s1.size()) + s1.size(),
                output.tellp());
        // Test for double-finish raising an exception
        EXPECT_THROW(ee.finish_write(output), NotWriting);

        padding = 5;
        ee.padding(padding);
        ee.value(s3);
        output.str(std::string());
        std::string().swap(expected);
        fill_buffer(expected, ids::Null, s3, true, true, true, padding);
        ee.start_write(output);
        EXPECT_EQ(tide::ids::size(ids::Null) + vint::size(s3.size() + padding)
                + s3.size() + padding, ee.finish_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
                expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Null) + vint::size(s3.size() + padding) +
                s3.size() + padding, output.tellp());

        // Zero-length string
        padding = 0;
        ee.padding(padding);
        std::string empty;
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
        EXPECT_EQ(ids::size(ids::Null) + vint::size(empty.size() + padding) +
                empty.size() + padding, output.tellp());

        // Zero-length string with padding
        padding = 5;
        ee.value(empty);
        ee.padding(padding);
        output.str(std::string());
        std::string().swap(expected);
        fill_buffer(expected, ids::Null, empty, true, true, true, padding);
        ee.start_write(output);
        EXPECT_EQ(ids::size(ids::Null) + vint::size(empty.size() + padding) +
                empty.size() + padding, ee.finish_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Null) + vint::size(empty.size() + padding) +
                empty.size() + padding, output.tellp());
    }

    TEST_F(StringElementTest, Write)
    {
        std::stringstream output;
        std::string expected;
        StringElement ee(ids::Null, s1);
        unsigned int padding(0);

        // Test for exception and no change to output when finishing without
        // starting
        EXPECT_THROW(ee.finish_write(output), NotWriting);
        EXPECT_EQ(0, output.tellp());

        output.str(std::string());
        std::string().swap(expected);
        fill_buffer(expected, ids::Null, s1, true, true, true);
        EXPECT_EQ(ids::size(ids::Null) + vint::size(s1.size()) + s1.size(),
                write(ee, output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
                expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Null) + vint::size(s1.size()) + s1.size(),
                output.tellp());

        padding = 5;
        ee.padding(padding);
        ee.value(s3);
        output.str(std::string());
        std::string().swap(expected);
        fill_buffer(expected, ids::Null, s3, true, true, true, padding);
        EXPECT_EQ(tide::ids::size(ids::Null) + vint::size(s3.size() + padding)
                + s3.size() + padding, write(ee, output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
                expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Null) + vint::size(s3.size() + padding) +
                s3.size() + padding, output.tellp());

        // Zero-length string
        padding = 0;
        ee.padding(padding);
        std::string empty;
        ee.value(empty);
        output.str(std::string());
        std::string().swap(expected);
        fill_buffer(expected, ids::Null, empty, true, true, true);
        EXPECT_EQ(tide::ids::size(ids::Null) + vint::size(empty.size()) +
                empty.size(), write(ee, output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
                expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Null) + vint::size(empty.size() + padding) +
                empty.size() + padding, output.tellp());

        // Zero-length string with padding
        padding = 5;
        ee.value(empty);
        ee.padding(padding);
        output.str(std::string());
        std::string().swap(expected);
        fill_buffer(expected, ids::Null, empty, true, true, true, padding);
        EXPECT_EQ(ids::size(ids::Null) + vint::size(empty.size() + padding) +
                empty.size() + padding, write(ee, output));
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Null) + vint::size(empty.size() + padding) +
                empty.size() + padding, output.tellp());
    }
}; // namespace test_string_el


