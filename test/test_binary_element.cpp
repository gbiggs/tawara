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
#include <jonen/binary_element.h>
#include <jonen/ebml_integer.h>
#include <jonen/exceptions.h>
#include <jonen/jonen_config.h>

#include <limits>

#include "test_utilities.h"

using namespace jonen;

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
    }; // class BinaryElementTest


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

    TEST_F(BinaryElementTest, CopyConstruction)
    {
        BinaryElement ee1(ids::Null, b1);

        BinaryElement ee2(ee1);
        EXPECT_EQ(ids::Null, ee2.id());
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b1, ee2.value());
        EXPECT_FALSE(ee2.has_default());

        BinaryElement ee3(ids::Null, b2, b3);
        BinaryElement ee4(ee3);
        EXPECT_EQ(ids::Null, ee4.id());
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b2, ee4.value());
        EXPECT_TRUE(ee4.has_default());
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b3, ee4.get_default());
    }

    TEST_F(BinaryElementTest, Assignment)
    {
        BinaryElement ee1(ids::Null, b1), ee2(0x81, std::vector<char>());
        ee2 = ee1;
        EXPECT_EQ(ee2, ee1);

        BinaryElement ee3(ids::Null, b2, b3), ee4(0x81, std::vector<char>());
        ee4 = ee3;
        EXPECT_EQ(ee4, ee3);

        ee4 = b4;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b4, ee4.value());
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

        std::vector<char> b2_copy(b2);
        swap(ee1, b2_copy);
        EXPECT_EQ(0x21, ee1.id());
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b2, ee1.value());
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b2_copy, b1);
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, b2, ee1.get_default());
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

    TEST_F(BinaryElementTest, StreamOut)
    {
        std::stringstream ss;
        BinaryElement ee1(ids::Null, b1);
        ss << ee1;
        EXPECT_EQ("0x61 0x62 0x63 0x64 0x65 0x66 0x67 0x68", ss.str());
    }


    ///////////////////////////////////////////////////////////////////////////
    // Vector interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST_F(BinaryElementTest, Assign)
    {
        // Copies-of-a-value assign
        BinaryElement ee(ids::Null, b1);
        std::vector<char> aaa(10, 'a');
        ee.assign(10, 'a');
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, aaa,
                ee.value());

        // Iterator range assign
        std::vector<char> bbb(10, 'b');
        ee.assign(bbb.begin() + 5, bbb.end());
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq,
                std::vector<char>(5, 'b'), ee.value());
    }


    TEST_F(BinaryElementTest, GetAllocator)
    {
        // TODO: How can this be tested?
    }


    TEST_F(BinaryElementTest, At)
    {
        BinaryElement ee(ids::Null, b1);
        EXPECT_EQ('a', ee.at(0));
        EXPECT_THROW(ee.at(20), std::out_of_range);

        BinaryElement const ee_const(ids::Null, b1);
        EXPECT_EQ('e', ee_const.at(4));
        EXPECT_THROW(ee_const.at(20), std::out_of_range);
    }


    TEST_F(BinaryElementTest, IndexOperator)
    {
        BinaryElement ee(ids::Null, b1);
        EXPECT_EQ('a', ee[0]);
        EXPECT_NO_THROW(ee[20]);

        BinaryElement const ee_const(ids::Null, b1);
        EXPECT_EQ('e', ee_const[4]);
        EXPECT_NO_THROW(ee_const[20]);
    }


    TEST_F(BinaryElementTest, Front)
    {
        BinaryElement ee(ids::Null, b1);
        EXPECT_EQ('a', ee.front());
        BinaryElement const ee_const(ids::Null, b1);
        EXPECT_EQ('a', ee_const.front());
    }


    TEST_F(BinaryElementTest, Back)
    {
        BinaryElement ee(ids::Null, b1);
        EXPECT_EQ('h', ee.back());
        BinaryElement const ee_const(ids::Null, b1);
        EXPECT_EQ('h', ee_const.back());
    }


    TEST_F(BinaryElementTest, Data)
    {
        BinaryElement ee(ids::Null, b1);
        EXPECT_EQ('a', *ee.data());
        BinaryElement const ee_const(ids::Null, b1);
        EXPECT_EQ('a', *ee_const.data());
    }


    TEST_F(BinaryElementTest, Begin)
    {
        BinaryElement ee(ids::Null, b1);
        EXPECT_EQ('a', *(ee.begin()));
        *(ee.begin()) = 'z';
        EXPECT_EQ('z', *(ee.begin()));
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        EXPECT_EQ('a', *(ee.cbegin()));
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
        BinaryElement const ee_const(ids::Null, b1);
        EXPECT_EQ('a', *(ee_const.begin()));
    }


    TEST_F(BinaryElementTest, End)
    {
        BinaryElement ee(ids::Null, b1);
        EXPECT_EQ('h', *(ee.end() - 1));
        *(ee.end() - 1) = 'z';
        EXPECT_EQ('z', *(--ee.end()));
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        EXPECT_EQ('h', *(ee.cend() - 1));
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
        BinaryElement const ee_const(ids::Null, b1);
        EXPECT_EQ('h', *(ee_const.end() - 1));
    }


    TEST_F(BinaryElementTest, RBegin)
    {
        BinaryElement ee(ids::Null, b1);
        EXPECT_EQ('h', *(ee.rbegin()));
        *(ee.rbegin()) = 'z';
        EXPECT_EQ('z', *(ee.rbegin()));
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        EXPECT_EQ('h', *(ee.crbegin()));
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
        BinaryElement const ee_const(ids::Null, b1);
        EXPECT_EQ('h', *(ee_const.rbegin()));
    }


    TEST_F(BinaryElementTest, REnd)
    {
        BinaryElement ee(ids::Null, b1);
        EXPECT_EQ('a', *(ee.rend() - 1));
        *(ee.rend() - 1) = 'z';
        EXPECT_EQ('z', *(ee.rend() - 1));
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        EXPECT_EQ('a', *(ee.crend() - 1));
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
        BinaryElement const ee_const(ids::Null, b1);
        EXPECT_EQ('a', *(ee_const.rend() - 1));
    }


    TEST_F(BinaryElementTest, Empty)
    {
        BinaryElement ee(ids::Null, std::vector<char>());
        EXPECT_TRUE(ee.empty());
        ee.value(b1);
        EXPECT_FALSE(ee.empty());
    }


    TEST_F(BinaryElementTest, Size)
    {
        BinaryElement ee(ids::Null, std::vector<char>());
        EXPECT_EQ(0, ee.size());
        ee.value(b1);
        EXPECT_EQ(8, ee.size());
    }


    TEST_F(BinaryElementTest, MaxSize)
    {
        // This value differs based on platform and standard library
        // implementation, so the best that can be done is test that the
        // underlying vector's value is being passed correctly.
        BinaryElement ee(ids::Null, std::vector<char>());
        EXPECT_EQ(std::vector<char>().max_size(), ee.max_size());
    }


    TEST_F(BinaryElementTest, Reserve)
    {
        BinaryElement ee(ids::Null, b1);
        EXPECT_EQ(8, ee.capacity());
        ee.reserve(32);
        EXPECT_EQ(32, ee.capacity());
    }


    TEST_F(BinaryElementTest, Capacity)
    {
        BinaryElement ee(ids::Null, b1);
        EXPECT_EQ(8, ee.capacity());
    }


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
    TEST_F(BinaryElementTest, ShrinkToFit)
    {
        BinaryElement ee(ids::Null, b1);
        ee.capacity(32);
        EXPECT_EQ(32, ee.capacity());
        ee.shrink_to_fit();
        EXPECT_EQ(8, ee.capacity());
    }
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


    TEST_F(BinaryElementTest, Clear)
    {
        BinaryElement ee(ids::Null, b1);
        ee.clear();
        EXPECT_EQ(8, ee.capacity());
        // Because the vector does not contain constructed values, the stored
        // contents will not change.
    }


    TEST_F(BinaryElementTest, Insert)
    {
        BinaryElement ee(ids::Null, b1);
        char z('z');
        ee.insert(ee.begin(), z);
        EXPECT_EQ('z', ee[0]);

#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        ee.insert(ee.cbegin(), 'y');
        EXPECT_EQ('y', ee[0]);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)

        ee.insert(ee.begin(), 3, 'x');
        EXPECT_EQ('x', ee[0]);
        EXPECT_EQ('x', ee[1]);
        EXPECT_EQ('x', ee[2]);

        ee.insert(ee.begin(), b3.begin(), b3.begin() + 2);
        EXPECT_EQ('n', ee[0]);
        EXPECT_EQ('o', ee[1]);
        EXPECT_EQ('x', ee[2]); // Only two elements should have been inserted

#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        ee.insert(ee.begin(), {'1', '2', '3'});
        EXPECT_EQ('1', ee[0]);
        EXPECT_EQ('2', ee[1]);
        EXPECT_EQ('3', ee[2]);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
    }


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
    TEST_F(BinaryElementTest, Emplace)
    {
        BinaryElement ee(ids::Null, b1);
        ee.emplace(ee.begin(), 'm');
        EXPECT_EQ('m', ee[0]);
    }
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


    TEST_F(BinaryElementTest, Erase)
    {
        BinaryElement ee(ids::Null, b1);
        ee.erase(ee.begin());
        EXPECT_EQ('b', ee[0]);
        EXPECT_EQ(7, ee.size());
        ee.erase(ee.begin(), ee.begin() + 2);
        EXPECT_EQ('d', ee[0]);
        EXPECT_EQ(5, ee.size());
    }


    TEST_F(BinaryElementTest, PushBack)
    {
        BinaryElement ee(ids::Null, b1);
        ee.push_back('i');
        EXPECT_EQ('i', ee[8]);
        EXPECT_EQ(9, ee.size());
    }


    TEST_F(BinaryElementTest, EmplaceBack)
    {
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        BinaryElement ee(ids::Null, b1);
        ee.emplace_back('i');
        EXPECT_EQ('i', ee[8]);
        EXPECT_EQ(9, ee.size());
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
    }


    TEST_F(BinaryElementTest, PopBack)
    {
        BinaryElement ee(ids::Null, b1);
        ee.pop_back();
        EXPECT_EQ(7, ee.size());
        EXPECT_EQ('g', ee.back());
    }


    TEST_F(BinaryElementTest, Resize)
    {
        BinaryElement ee(ids::Null, b1);
        ee.resize(10, 'z');
        EXPECT_EQ('z', ee[8]);
        EXPECT_EQ('z', ee[9]);
        EXPECT_EQ(10, ee.size());

        ee.resize(3);
        EXPECT_EQ('c', ee.back());
        EXPECT_EQ(3, ee.size());
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
        jonen::BinaryElement ee(ids::Null, b1);

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
        EXPECT_EQ(jonen::ids::size(ids::Null) + 1 + b3.size(),
                ee.start_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
                expected);

        // Zero-length binary
        std::vector<char> empty;
        ee.value(empty);
        output.str(std::string());
        std::string().swap(expected);
        fill_buffer(expected, ids::Null, empty, true, true, true);
        EXPECT_EQ(jonen::ids::size(ids::Null) + vint::size(empty.size()) +
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
        EXPECT_EQ(jonen::ids::size(ids::Null) + 1 + b3.size(),
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
        EXPECT_EQ(jonen::ids::size(ids::Null) + vint::size(empty.size()) +
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
        EXPECT_EQ(jonen::ids::size(ids::Null) + 1 + b3.size(),
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
        EXPECT_EQ(jonen::ids::size(ids::Null) + vint::size(empty.size()) +
                empty.size(), write(ee, output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
                expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Null) + vint::size(empty.size()) +
                empty.size(), output.tellp());
    }
}; // namespace test_binary_el

