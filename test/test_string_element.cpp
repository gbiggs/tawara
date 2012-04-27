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
        EXPECT_EQ("abcdefgh", ss.str());
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

    TEST_F(StringElementTest, Assign)
    {
        // Copies-of-a-value assign
        StringElement ee(ids::Null, s1);
        std::string value(10, 'a');
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, value,
                ee.assign(10, 'a').value()); // Tests the return value
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, value,
                ee.value()); // Tests the variable itself

        // Whole-string assign
        value = "12345";
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, value,
                ee.assign(value).value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, value,
                ee.value());

        // Substring assign
        value = "abcde";
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, value.substr(1, 2),
                ee.assign(value, 1, 2).value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, value.substr(1, 2),
                ee.value());

#if defined(TIDE_CPLUSPLUS11_SUPPORT)
        // Move assign
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "xyz",
                ee.assign("xyz").value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "xyz",
                ee.value());
#endif // defined(TIDE_CPLUSPLUS11_SUPPORT)

        // C substring assign
        char const* const c_str = "c_string";
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "c_s",
                ee.assign(c_str, 3).value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "c_s",
                ee.value());

        // C string assign
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "c_string",
                ee.assign(c_str).value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "c_string",
                ee.value());

        // Iterator range assign
        std::string(10, 'b').swap(value);
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq,
                std::string(5, 'b'),
                ee.assign(value.begin() + 5, value.end()).value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq,
                std::string(5, 'b'), ee.value());

#if defined(TIDE_CPLUSPLUS11_SUPPORT)
        // Initialiser list assign
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "12345",
                ee.assign({'1', '2', '3', '4', '5'}).value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "12345",
                ee.value());
#endif // defined(TIDE_CPLUSPLUS11_SUPPORT)
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
#if defined(TIDE_CPLUSPLUS11_SUPPORT)
        StringElement ee(ids::Null, s1);
        EXPECT_EQ('a', ee.front());
        StringElement const ee_const(ids::Null, s1);
        EXPECT_EQ('a', ee_const.front());
#endif // defined(TIDE_CPLUSPLUS11_SUPPORT)
    }

    TEST_F(StringElementTest, Back)
    {
#if defined(TIDE_CPLUSPLUS11_SUPPORT)
        StringElement ee(ids::Null, s1);
        EXPECT_EQ('h', ee.back());
        StringElement const ee_const(ids::Null, s1);
        EXPECT_EQ('h', ee_const.back());
#endif // defined(TIDE_CPLUSPLUS11_SUPPORT)
    }

    TEST_F(StringElementTest, Data)
    {
        StringElement ee(ids::Null, s1);
        EXPECT_EQ('a', *ee.data());
        StringElement const ee_const(ids::Null, s1);
        EXPECT_EQ('a', *ee_const.data());
    }

    TEST_F(StringElementTest, CStr)
    {
        StringElement ee(ids::Null, s1);
        EXPECT_EQ('a', *ee.c_str());
        StringElement const ee_const(ids::Null, s1);
        EXPECT_EQ('a', *ee_const.c_str());
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

    TEST_F(StringElementTest, Length)
    {
        StringElement ee(ids::Null, std::string());
        EXPECT_EQ(0, ee.length());
        ee.value(s1);
        EXPECT_EQ(8, ee.length());
    }

    TEST_F(StringElementTest, MaxSize)
    {
        // This value differs based on platform and standard library
        // implementation, so the best that can be done is test that the
        // underlying string type's value is being passed correctly.
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
        EXPECT_EQ(0, ee.size());
    }

    TEST_F(StringElementTest, Insert)
    {
        // Insert single character
        StringElement ee(ids::Null, s1);
#if !defined(TIDE_CPLUSPLUS11_SUPPORT)
        char z('z');
        StringElement::iterator itr(ee.insert(ee.begin() + 1, z));
        EXPECT_EQ('z', ee[1]);
        EXPECT_TRUE((ee.begin() + 1) == itr);
#endif // !defined(TIDE_CPLUSPLUS11_SUPPORT)
#if defined(TIDE_CPLUSPLUS11_SUPPORT)
        ee::iterator itr(ee.insert(ee.cbegin(), 'y'));
        EXPECT_EQ('y', ee[0]);
        EXPECT_TRUE((ee.begin() + 1) == itr);
#endif // defined(TIDE_CPLUSPLUS11_SUPPORT)

        // Insert character at iterator
        ee.insert(ee.begin(), 3, 'x');
        EXPECT_EQ('x', ee[0]);
        EXPECT_EQ('x', ee[1]);
        EXPECT_EQ('x', ee[2]);

        // Insert character at index
        ee.value("123");
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "xxx123",
                ee.insert(0, 3, 'x').value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "xxx123", ee.value());

        // Insert C string at index
        ee.value("123");
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "1abc23",
                ee.insert(1, "abc").value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "1abc23", ee.value());

        // Insert C substring at index
        ee.value("123");
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "1abc23",
                ee.insert(1, "abcdef", 3).value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "1abc23", ee.value());

        // Insert std::string at index
        ee.value("123");
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "1abc23",
                ee.insert(1, std::string("abc")).value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "1abc23", ee.value());

        // Insert std::string substring at index
        ee.value("123");
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "1cde23",
                ee.insert(1, std::string("abcdef", 2, 3)).value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "1cde23", ee.value());

        ee.value("123");
        ee.insert(ee.begin(), s3.begin(), s3.begin() + 2);
        EXPECT_EQ('n', ee[0]);
        EXPECT_EQ('o', ee[1]);
        EXPECT_EQ('1', ee[2]); // Only two elements should have been inserted

#if defined(TIDE_CPLUSPLUS11_SUPPORT)
        ee.insert(ee.begin(), {'1', '2', '3'});
        EXPECT_EQ('1', ee[0]);
        EXPECT_EQ('2', ee[1]);
        EXPECT_EQ('3', ee[2]);
#endif // defined(TIDE_CPLUSPLUS11_SUPPORT)
    }

    TEST_F(StringElementTest, Erase)
    {
        StringElement ee(ids::Null, s1);
        ee.erase(1, 2);
        EXPECT_EQ('a', ee[0]);
        EXPECT_EQ('d', ee[1]);
        EXPECT_EQ(6, ee.size());

        ee.erase(ee.begin());
        EXPECT_EQ('d', ee[0]);
        EXPECT_EQ(5, ee.size());

        ee.erase(ee.begin(), ee.begin() + 2);
        EXPECT_EQ('f', ee[0]);
        EXPECT_EQ(3, ee.size());
    }

    TEST_F(StringElementTest, PushBack)
    {
        StringElement ee(ids::Null, s1);
        ee.push_back('i');
        EXPECT_EQ('i', ee[8]);
        EXPECT_EQ(9, ee.size());
    }

    TEST_F(StringElementTest, PopBack)
    {
#if defined(TIDE_CPLUSPLUS11_SUPPORT)
        StringElement ee(ids::Null, s1);
        ee.pop_back();
        EXPECT_EQ(7, ee.size());
        EXPECT_EQ('g', ee.back());
#endif // defined(TIDE_CPLUSPLUS11_SUPPORT)
    }

    TEST_F(StringElementTest, Append)
    {
        StringElement ee(ids::Null, "123");

        // Append copies of a character
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "123aaa",
                ee.append(3, 'a').value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "123aaa",
                ee.value());

        // Append a string
        ee.value("123");
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "123aaa",
                ee.append(std::string("aaa")).value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "123aaa",
                ee.value());

        // Append a substring
        ee.value("123");
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "123aab",
                ee.append(std::string("aaabbb"), 1, 3).value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "123aab",
                ee.value());

        // Append a C substring
        ee.value("123");
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "123aaa",
                ee.append("aaabbb", 3).value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "123aaa",
                ee.value());

        // Append a C string
        ee.value("123");
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "123aaa",
                ee.append("aaa").value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "123aaa",
                ee.value());

        // Append a range
        ee.value("123");
        std::string value("aaabbb");
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "123aaa",
                ee.append(value.begin(), value.begin() + 3).value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "123aaa",
                ee.value());

#if defined(TIDE_CPLUSPLUS11_SUPPORT)
        // Append an initialiser list
        ee.value("123");
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "123aaa",
                ee.append({'a', 'a', 'a'}).value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "123aaa",
                ee.value());
#endif // defined(TIDE_CPLUSPLUS11_SUPPORT)
    }

    TEST_F(StringElementTest, AppendOperator)
    {
        StringElement ee(ids::Null, "123");

        // Append a string
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "123aaa",
                (ee += std::string("aaa")).value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "123aaa",
                ee.value());

        // Append a character
        ee.value("123");
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "123a",
                (ee += 'a').value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "123a",
                ee.value());

        // Append a C string
        ee.value("123");
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "123abc",
                (ee += "abc").value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "123abc",
                ee.value());

#if defined(TIDE_CPLUSPLUS11_SUPPORT)
        // Append an initialiser list
        ee.value("123");
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "123abc",
                (ee += {'a', 'b', 'c'}).value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "123abc",
                ee.value());
#endif // defined(TIDE_CPLUSPLUS11_SUPPORT)
    }

    TEST_F(StringElementTest, AdditionOperator)
    {
        StringElement ee(ids::Null, "123");
        StringElement expected(ids::Null, "123abc");

        // Add a string element
        StringElement ee2(ids::Null, "abc");
        EXPECT_EQ(expected, ee + ee2);

        // Add a string
        std::string letters("abc");
        EXPECT_EQ(expected, ee + letters);
        expected.value("abc123");
        EXPECT_EQ(expected, letters + ee);
        expected.value("123abc");

        // Add a C string
        char const* const c_str = "abc";
        EXPECT_EQ(expected, ee + c_str);
        expected.value("abc123");
        EXPECT_EQ(expected, c_str + ee);
        expected.value("123abc");

        // Add a character
        char c = 'a';
        expected.value("123a");
        EXPECT_EQ(expected, ee + c);
        expected.value("a123");
        EXPECT_EQ(expected, c + ee);
        expected.value("123abc");

#if defined(TIDE_CPLUSPLUS11_SUPPORT)
        // Move versions (maybe?)

        // Add a string element
        EXPECT_EQ(expected, ee + StringElement(ids::Null, "abc"));

        // Add a string
        EXPECT_EQ(expected, ee + std::string("abc"));
        expected.value("abc123");
        EXPECT_EQ(expected, std::string("abc") + ee);
        expected.value("123abc");

        // Add a C string
        EXPECT_EQ(expected, ee + "abc");
        expected.value("abc123");
        EXPECT_EQ(expected, "abc" + ee);
        expected.value("123abc");

        // Add a character
        expected.value("123a");
        EXPECT_EQ(expected, ee + 'a');
        expected.value("a123");
        EXPECT_EQ(expected, 'a' + ee);
#endif // defined(TIDE_CPLUSPLUS11_SUPPORT)

        // The original element should still be unchanged
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "123", ee.value());
    }

    TEST_F(StringElementTest, Compare)
    {
        StringElement ee(ids::Null, s2);

        // Compare with std::string
        EXPECT_TRUE(ee.compare(s1) > 0);
        EXPECT_TRUE(ee.compare(s2) == 0);
        EXPECT_TRUE(ee.compare(s3) < 0);

        // Compare substring with std::string
        std::string substr("mlk");
        EXPECT_TRUE(ee.compare(0, 3, s1) > 0);
        EXPECT_TRUE(ee.compare(0, 3, substr) == 0);
        EXPECT_TRUE(ee.compare(0, 3, s3) < 0);

        // Compare substring with std::string substring
        EXPECT_TRUE(ee.compare(0, 3, s1, 0, 3) > 0);
        EXPECT_TRUE(ee.compare(0, 3, s2, 0, 3) == 0);
        EXPECT_TRUE(ee.compare(0, 3, s3, 0, 3) < 0);

        // Compare with C string
        EXPECT_TRUE(ee.compare(s1.c_str()) > 0);
        EXPECT_TRUE(ee.compare(s2.c_str()) == 0);
        EXPECT_TRUE(ee.compare(s3.c_str()) < 0);

        // Compare substring with C string
        EXPECT_TRUE(ee.compare(0, 3, s1.c_str()) > 0);
        EXPECT_TRUE(ee.compare(0, 3, substr.c_str()) == 0);
        EXPECT_TRUE(ee.compare(0, 3, s3.c_str()) < 0);

        // Compare substring with std::string substring
        EXPECT_TRUE(ee.compare(0, 3, s1.c_str(), 3) > 0);
        EXPECT_TRUE(ee.compare(0, 3, s2.c_str(), 3) == 0);
        EXPECT_TRUE(ee.compare(0, 3, s3.c_str(), 3) < 0);
    }

    TEST_F(StringElementTest, Replace)
    {
        StringElement ee(ids::Null, "123");
        std::string value("aaa");

        // Replace with std::string
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "1aaa3",
                ee.replace(1, 1, value).value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "1aaa3",
                ee.value());
#if defined(TIDE_CPLUSPLUS11_SUPPORT)
        ee.value("123");
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "1aaa3",
                ee.replace(ee.begin() + 1, ee.begin() + 2, value).value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "1aaa3",
                ee.value());
#endif // defined(TIDE_CPLUSPLUS11_SUPPORT)

        // Replace with std::string substring
        ee.value("123");
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "1a3",
                ee.replace(1, 1, value, 1, 1).value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "1a3",
                ee.value());
#if defined(TIDE_CPLUSPLUS11_SUPPORT)
        ee.value("123");
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "1a3",
                ee.replace(ee.begin() + 1, ee.begin() + 2, value.begin(),
                    value.begin() + 1).value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "1a3",
                ee.value());
#endif // defined(TIDE_CPLUSPLUS11_SUPPORT)

        // Replace with C substring
        ee.value("123");
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "1a3",
                ee.replace(1, 1, "aaa", 1).value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "1a3",
                ee.value());
#if defined(TIDE_CPLUSPLUS11_SUPPORT)
        ee.value("123");
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "1a3",
                ee.replace(ee.begin() + 1, ee.begin() + 2, "aaa", 1).value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "1a3",
                ee.value());
#endif // defined(TIDE_CPLUSPLUS11_SUPPORT)

        // Replace with C string
        ee.value("123");
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "1aaa3",
                ee.replace(1, 1, "aaa").value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "1aaa3",
                ee.value());
#if defined(TIDE_CPLUSPLUS11_SUPPORT)
        ee.value("123");
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "1aaa3",
                ee.replace(ee.begin() + 1, ee.begin() + 2, "aaa").value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "1aaa3",
                ee.value());
#endif // defined(TIDE_CPLUSPLUS11_SUPPORT)

        // Replace with character
        ee.value("123");
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "1aaa3",
                ee.replace(1, 1, 3, 'a').value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "1aaa3",
                ee.value());
#if defined(TIDE_CPLUSPLUS11_SUPPORT)
        ee.value("123");
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "1aaa3",
                ee.replace(ee.begin() + 1, ee.begin() + 2, 3, 'a').value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "1aaa3",
                ee.value());
#endif // defined(TIDE_CPLUSPLUS11_SUPPORT)

#if defined(TIDE_CPLUSPLUS11_SUPPORT)
        // Replace with initialiser list
        ee.value("123");
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "1abc3",
                ee.replace(ee.begin() + 1, ee.begin() + 2,
                    {'a', 'b', 'c'}).value());
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "1abc3",
                ee.value());
#endif // defined(TIDE_CPLUSPLUS11_SUPPORT)
    }

    TEST_F(StringElementTest, Substring)
    {
        StringElement ee(ids::Null, s1);
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "bcd",
                ee.substr(1, 3));
    }

    TEST_F(StringElementTest, Copy)
    {
        StringElement ee(ids::Null, s1);
        char dest[4];
        dest[3] = '\0';
        EXPECT_EQ(3, ee.copy(dest, 3, 1));
        EXPECT_PRED_FORMAT2(test_utils::std_strings_eq, "bcd", dest);
    }

    TEST_F(StringElementTest, Resize)
    {
        StringElement ee(ids::Null, s1);
        ee.resize(3);
        EXPECT_EQ('c', *(ee.end() - 1));
        EXPECT_EQ(3, ee.size());

        ee.resize(5, 'z');
        EXPECT_EQ('z', ee[3]);
        EXPECT_EQ('z', ee[4]);
        EXPECT_EQ(5, ee.size());
    }

    TEST_F(StringElementTest, Find)
    {
        StringElement ee(ids::Null, "abcde");

        // Find std::string
        EXPECT_EQ(2, ee.find(std::string("cde")));
        EXPECT_EQ(std::string::npos, ee.find(std::string("xyz")));
        EXPECT_EQ(std::string::npos, ee.find(std::string("cde"), 3));
        // Find C string substring
        EXPECT_EQ(2, ee.find("cdexyz", 0, 3));
        EXPECT_EQ(std::string::npos, ee.find("mnocde", 0, 3));
        EXPECT_EQ(std::string::npos, ee.find("cde", 3, 3));
        // Find C string
        EXPECT_EQ(2, ee.find("cde"));
        EXPECT_EQ(std::string::npos, ee.find("xyz"));
        EXPECT_EQ(std::string::npos, ee.find("cde", 3));
        // Find character
        EXPECT_EQ(2, ee.find('c'));
        EXPECT_EQ(std::string::npos, ee.find('x'));
        EXPECT_EQ(std::string::npos, ee.find('c', 3));
    }

    TEST_F(StringElementTest, RFind)
    {
        StringElement ee(ids::Null, "abcdeabcde");

        // Find std::string
        EXPECT_EQ(7, ee.rfind(std::string("cde")));
        EXPECT_EQ(std::string::npos, ee.rfind(std::string("xyz")));
        EXPECT_EQ(std::string::npos, ee.rfind(std::string("cde"), 1));
        // Find C string substring
        EXPECT_EQ(7, ee.rfind("cdexyz", std::string::npos, 3));
        EXPECT_EQ(std::string::npos, ee.rfind("xyz", std::string::npos, 3));
        EXPECT_EQ(std::string::npos, ee.rfind("cdexyz", 1, 3));
        // Find C string
        EXPECT_EQ(7, ee.rfind("cde"));
        EXPECT_EQ(std::string::npos, ee.rfind("xyz"));
        EXPECT_EQ(std::string::npos, ee.rfind("cde", 1));
        // Find character
        EXPECT_EQ(7, ee.rfind('c'));
        EXPECT_EQ(std::string::npos, ee.rfind('x'));
        EXPECT_EQ(std::string::npos, ee.rfind('c', 1));
    }

    TEST_F(StringElementTest, FindFirstOf)
    {
        StringElement ee(ids::Null, "abcde");

        // Find std::string
        EXPECT_EQ(2, ee.find_first_of(std::string("xcz")));
        EXPECT_EQ(std::string::npos, ee.find_first_of(std::string("xyz")));
        EXPECT_EQ(std::string::npos, ee.find_first_of(std::string("xcz"), 3));
        // Find C string substring
        EXPECT_EQ(2, ee.find_first_of("xczmno", 0, 3));
        EXPECT_EQ(std::string::npos, ee.find_first_of("xyz", 0, 3));
        EXPECT_EQ(std::string::npos, ee.find_first_of("xcz", 3, 3));
        // Find C string
        EXPECT_EQ(2, ee.find_first_of("xcz"));
        EXPECT_EQ(std::string::npos, ee.find_first_of("xyz"));
        EXPECT_EQ(std::string::npos, ee.find_first_of("xcz", 3));
        // Find character
        EXPECT_EQ(2, ee.find_first_of('c'));
        EXPECT_EQ(std::string::npos, ee.find_first_of('y'));
        EXPECT_EQ(std::string::npos, ee.find_first_of('c', 3));
    }

    TEST_F(StringElementTest, FindFirstNotOf)
    {
        StringElement ee(ids::Null, "abcde");

        // Find std::string
        EXPECT_EQ(2, ee.find_first_not_of(std::string("abde")));
        EXPECT_EQ(std::string::npos,
                ee.find_first_not_of(std::string("abcde")));
        EXPECT_EQ(std::string::npos, ee.find_first_not_of(std::string("abde"),
                    3));
        // Find C string substring
        EXPECT_EQ(2, ee.find_first_not_of("abde", 0, 4));
        EXPECT_EQ(std::string::npos, ee.find_first_not_of("abcde", 0, 5));
        EXPECT_EQ(std::string::npos, ee.find_first_not_of("abde", 3, 4));
        // Find C string
        EXPECT_EQ(2, ee.find_first_not_of("abde"));
        EXPECT_EQ(std::string::npos, ee.find_first_not_of("abcde"));
        EXPECT_EQ(std::string::npos, ee.find_first_not_of("abde", 3));
        // Find character
        EXPECT_EQ(0, ee.find_first_not_of('b'));
        ee.value("aaaa");
        EXPECT_EQ(std::string::npos, ee.find_first_not_of('a'));
    }

    TEST_F(StringElementTest, FindLastOf)
    {
        StringElement ee(ids::Null, "abcdeabcde");

        // Find std::string
        EXPECT_EQ(7, ee.find_last_of(std::string("xcz")));
        EXPECT_EQ(std::string::npos, ee.find_last_of(std::string("xyz")));
        EXPECT_EQ(std::string::npos, ee.find_last_of(std::string("xcz"), 1));
        // Find C string substring
        EXPECT_EQ(7, ee.find_last_of("xczmno", std::string::npos, 3));
        EXPECT_EQ(std::string::npos, ee.find_last_of("xyz", std::string::npos,
                    3));
        EXPECT_EQ(std::string::npos, ee.find_last_of("xcz", 1, 3));
        // Find C string
        EXPECT_EQ(7, ee.find_last_of("xcz"));
        EXPECT_EQ(std::string::npos, ee.find_last_of("xyz"));
        EXPECT_EQ(std::string::npos, ee.find_last_of("xcz", 1));
        // Find character
        EXPECT_EQ(7, ee.find_last_of('c'));
        EXPECT_EQ(std::string::npos, ee.find_last_of('y'));
        EXPECT_EQ(std::string::npos, ee.find_last_of('c', 1));
    }

    TEST_F(StringElementTest, FindLastNotOf)
    {
        StringElement ee(ids::Null, "abcdeabcde");

        // Find std::string
        EXPECT_EQ(7, ee.find_last_not_of(std::string("abde")));
        EXPECT_EQ(std::string::npos,ee.find_last_not_of(std::string("abcde")));
        EXPECT_EQ(std::string::npos, ee.find_last_not_of(std::string("abde"),
                    1));
        // Find C string substring
        EXPECT_EQ(7, ee.find_last_not_of("abde", std::string::npos, 4));
        EXPECT_EQ(std::string::npos, ee.find_last_not_of("abcde",
                    std::string::npos, 5));
        EXPECT_EQ(std::string::npos, ee.find_last_not_of("abde", 1, 4));
        // Find C string
        EXPECT_EQ(7, ee.find_last_not_of("abde"));
        EXPECT_EQ(std::string::npos, ee.find_last_not_of("abcde"));
        EXPECT_EQ(std::string::npos, ee.find_last_not_of("abde", 1));
        // Find character
        EXPECT_EQ(9, ee.find_last_not_of('b'));
        ee.value("aaaa");
        EXPECT_EQ(std::string::npos, ee.find_last_not_of('a'));
    }


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


