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

#include "test_utilities.h"
#include <boost/foreach.hpp>
#include <jonen/ids.h>
#include <jonen/integer_elements.h>
#include <jonen/track_join_blocks.h>
#include <vector>

using namespace jonen;
using namespace test_utils;

namespace test_track_join_blocks
{
    ///////////////////////////////////////////////////////////////////////////
    // Helper functions
    ///////////////////////////////////////////////////////////////////////////

    void populate_children(std::vector<ElPtr>& children)
    {
        children.push_back(ElPtr(new UIntElement(ids::TrackJoinUID, 1)));
        children.push_back(ElPtr(new UIntElement(ids::TrackJoinUID, 2)));
        children.push_back(ElPtr(new UIntElement(ids::TrackJoinUID, 3)));
    }

    void populate_element(TrackJoinBlocks& ee, std::vector<ElPtr> const& children)
    {
        using boost::dynamic_pointer_cast;

        BOOST_FOREACH(ElPtr const& v, children)
        {
            ee.push_back(dynamic_pointer_cast<UIntElement>(v)->value());
        }
    }

    void check_children_match(TrackJoinBlocks const& ee,
            std::vector<ElPtr> const& children)
    {
        using boost::dynamic_pointer_cast;

        EXPECT_TRUE(ee.size() == children.size());
        TrackJoinBlocks::const_iterator uid_itr(ee.begin());
        for(int ii(0); ii < children.size(); ++uid_itr, ++ii)
        {
            boost::shared_ptr<UIntElement> uid_ptr(
                    dynamic_pointer_cast<UIntElement>(children[ii]));
            EXPECT_EQ(*uid_ptr, *uid_itr);
        }
    }


    ///////////////////////////////////////////////////////////////////////////
    // TrackJoinBlocks interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST(TrackJoinBlocks, Construction)
    {
        TrackJoinBlocks ee;
        EXPECT_EQ(ids::TrackJoinBlocks, ee.id());
        EXPECT_EQ(ee.begin(), ee.end());
        EXPECT_TRUE(ee.empty());
        EXPECT_EQ(0, ee.size());
    }


    TEST(TrackJoinBlocks, CopyConstruction)
    {
        TrackJoinBlocks ee1;
        std::vector<ElPtr> children;
        populate_children(children);
        populate_element(ee1, children);

        TrackJoinBlocks ee2(ee1);
        EXPECT_TRUE(ee1 == ee2);
    }


    TEST(TrackJoinBlocks, Assignment)
    {
        TrackJoinBlocks ee1;
        std::vector<ElPtr> children;
        populate_children(children);
        populate_element(ee1, children);

        TrackJoinBlocks ee2;
        EXPECT_FALSE(ee1 == ee2);
        ee2 = ee1;
        EXPECT_TRUE(ee1 == ee2);
    }


    TEST(TrackJoinBlocks, Swap)
    {
        TrackJoinBlocks ee1;
        std::vector<ElPtr> children;
        populate_children(children);
        populate_element(ee1, children);

        TrackJoinBlocks ee2;
        swap(ee1, ee2);

        EXPECT_TRUE(ee1.empty());
        check_children_match(ee2, children);
    }


    ///////////////////////////////////////////////////////////////////////////
    // List interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST(TrackJoinBlocks, GetAllocator)
    {
        // TODO: How can this be tested?
    }


    TEST(TrackJoinBlocks, Equal)
    {
        TrackJoinBlocks ee1;
        ee1.push_back(1);
        TrackJoinBlocks ee2;
        ee2.push_back(1);
        TrackJoinBlocks ee3;
        ee3.push_back(2);

        EXPECT_TRUE(ee1 == ee2);
        EXPECT_FALSE(ee1 == ee3);
    }


    TEST(TrackJoinBlocks, LessThan)
    {
        TrackJoinBlocks ee1;
        ee1.push_back(1);
        TrackJoinBlocks ee2;
        ee2.push_back(2);

        EXPECT_TRUE(ee1 < ee2);
        EXPECT_FALSE(ee2 < ee1);
    }


    TEST(TrackJoinBlocks, LessThanEqual)
    {
        TrackJoinBlocks ee1;
        ee1.push_back(1);
        TrackJoinBlocks ee2;
        ee2.push_back(1);
        TrackJoinBlocks ee3;
        ee3.push_back(2);

        EXPECT_TRUE(ee1 <= ee2);
        EXPECT_TRUE(ee1 <= ee3);
        EXPECT_FALSE(ee3 <= ee2);
    }


    TEST(TrackJoinBlocks, NotEqual)
    {
        TrackJoinBlocks ee1;
        ee1.push_back(1);
        TrackJoinBlocks ee2;
        ee2.push_back(1);
        TrackJoinBlocks ee3;
        ee3.push_back(2);

        EXPECT_FALSE(ee1 != ee2);
        EXPECT_TRUE(ee1 != ee3);
    }


    TEST(TrackJoinBlocks, GreaterThanEqual)
    {
        TrackJoinBlocks ee1;
        ee1.push_back(2);
        TrackJoinBlocks ee2;
        ee2.push_back(2);
        TrackJoinBlocks ee3;
        ee3.push_back(1);

        EXPECT_TRUE(ee1 >= ee2);
        EXPECT_TRUE(ee1 >= ee3);
        EXPECT_FALSE(ee3 >= ee2);
    }


    TEST(TrackJoinBlocks, GreaterThan)
    {
        TrackJoinBlocks ee1;
        ee1.push_back(2);
        TrackJoinBlocks ee2;
        ee2.push_back(1);

        EXPECT_TRUE(ee1 > ee2);
        EXPECT_FALSE(ee2 > ee1);
    }


    TEST(TrackJoinBlocks, Assign)
    {
        TrackJoinBlocks ee;

        ee.assign(static_cast<TrackJoinBlocks::size_type>(3), 1);
        EXPECT_EQ(3, ee.size());
        EXPECT_EQ(1, ee[0]);
        EXPECT_EQ(1, ee[1]);
        EXPECT_EQ(1, ee[2]);

        std::vector<unsigned long long int> uids;
        uids.push_back(1);
        uids.push_back(2);
        uids.push_back(3);

        ee.clear();
        ee.assign(uids.begin(), uids.end());
        EXPECT_EQ(uids.size(), ee.size());
        EXPECT_EQ(uids[0], ee[0]);
        EXPECT_EQ(uids[1], ee[1]);
        EXPECT_EQ(uids[2], ee[2]);
    }


    TEST(TrackJoinBlocks, At)
    {
        TrackJoinBlocks ee;
        ee.push_back(1);
        ee.push_back(2);
        ee.push_back(3);

        EXPECT_EQ(1, ee.at(0));
        EXPECT_EQ(2, ee.at(1));
        EXPECT_EQ(3, ee.at(2));
        EXPECT_THROW(ee.at(3), std::out_of_range);
    }


    TEST(TrackJoinBlocks, IndexOperator)
    {
        TrackJoinBlocks ee;
        ee.push_back(1);
        ee.push_back(2);
        ee.push_back(3);

        EXPECT_EQ(1, ee[0]);
        EXPECT_EQ(2, ee[1]);
        EXPECT_EQ(3, ee[2]);
    }


    TEST(TrackJoinBlocks, Front)
    {
        TrackJoinBlocks ee;
        ee.push_back(1);
        ee.push_back(2);
        ee.push_back(3);

        EXPECT_EQ(1, ee.front());
    }


    TEST(TrackJoinBlocks, Back)
    {
        TrackJoinBlocks ee;
        ee.push_back(1);
        ee.push_back(2);
        ee.push_back(3);

        EXPECT_EQ(3, ee.back());
    }


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
    TEST(TrackJoinBlocks, Data)
    {
        TrackJoinBlocks ee;
        ee.push_back(1);
        ee.push_back(2);
        ee.push_back(3);
        EXPECT_EQ(&ee[0], ee.data());

        TrackJoinBlocks const ee_const(ee);
        EXPECT_EQ(&ee_const[0], ee_const.data());
    }
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


    TEST(TrackJoinBlocks, Begin)
    {
        TrackJoinBlocks ee;
        ee.push_back(1);
        ee.push_back(2);
        ee.push_back(3);

        EXPECT_EQ(1, *ee.begin());
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        EXPECT_EQ(1, *ee.cbegin());
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
        TrackJoinBlocks const ee_const(ee);
        EXPECT_EQ(1, *ee_const.begin());
    }


    TEST(TrackJoinBlocks, End)
    {
        TrackJoinBlocks ee;
        ee.push_back(1);
        ee.push_back(2);
        ee.push_back(3);

        EXPECT_EQ(3, *(--ee.end()));
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        EXPECT_EQ(3, *(--ee.cend()));
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
        TrackJoinBlocks const ee_const(ee);
        EXPECT_EQ(3, *(--ee_const.end()));
    }


    TEST(TrackJoinBlocks, RBegin)
    {
        TrackJoinBlocks ee;
        ee.push_back(1);
        ee.push_back(2);
        ee.push_back(3);

        EXPECT_EQ(3, *ee.rbegin());
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        EXPECT_EQ(3, *ee.crbegin());
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
        TrackJoinBlocks const ee_const(ee);
        EXPECT_EQ(3, *ee_const.rbegin());
    }


    TEST(TrackJoinBlocks, REnd)
    {
        TrackJoinBlocks ee;
        ee.push_back(1);
        ee.push_back(2);
        ee.push_back(3);

        EXPECT_EQ(1, *(--ee.rend()));
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        EXPECT_EQ(1, *(--ee.crend()));
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
        TrackJoinBlocks const ee_const(ee);
        EXPECT_EQ(1, *(--ee_const.rend()));
    }


    TEST(TrackJoinBlocks, Empty)
    {
        TrackJoinBlocks ee;
        EXPECT_TRUE(ee.empty());
        ee.push_back(1);
        ee.push_back(2);
        ee.push_back(3);
        EXPECT_FALSE(ee.empty());
    }


    TEST(TrackJoinBlocks, Size)
    {
        TrackJoinBlocks ee;
        EXPECT_EQ(0, ee.size());
        ee.push_back(1);
        ee.push_back(2);
        ee.push_back(3);
        EXPECT_EQ(3, ee.size());
    }


    TEST(TrackJoinBlocks, MaxSize)
    {
        // This value differs based on platform and standard library
        // implementation, so the best that can be done is test that the
        // underlying list's value is being passed correctly.
        TrackJoinBlocks ee;
        std::vector<unsigned long long int> vector;
        EXPECT_EQ(vector.max_size(), ee.max_size());
    }


    TEST(TrackJoinBlocks, Clear)
    {
        TrackJoinBlocks ee;
        ee.push_back(1);
        ee.push_back(2);
        ee.push_back(3);
        EXPECT_FALSE(ee.empty());
        ee.clear();
        EXPECT_TRUE(ee.empty());
    }


    TEST(TrackJoinBlocks, Insert)
    {
        TrackJoinBlocks ee;
        // Non-move version in C++11
        unsigned long long int uid(1);
        ee.insert(ee.begin(), uid);
        EXPECT_EQ(1, ee.size());
        EXPECT_EQ(1, *ee.begin());
        uid = 0;
        EXPECT_THROW(ee.insert(ee.begin(), uid), ValueOutOfRange);
        // Move version in C++11
        ee.insert(ee.begin(), 2);
        EXPECT_EQ(2, ee.size());
        EXPECT_EQ(2, *ee.begin());
        EXPECT_THROW(ee.insert(ee.begin(), 0), ValueOutOfRange);

        ee.clear();
        ee.insert(ee.begin(), static_cast<TrackJoinBlocks::size_type>(3), 1);
        EXPECT_EQ(3, ee.size());
        EXPECT_EQ(1, *ee.begin());
        EXPECT_THROW(ee.insert(ee.begin(), static_cast<TrackJoinBlocks::size_type>(3),
                    0), ValueOutOfRange);

        std::vector<unsigned long long int> uids;
        uids.push_back(1);
        uids.push_back(2);
        uids.push_back(3);
        ee.clear();
        ee.insert(ee.begin(), uids.begin(), uids.end());
        EXPECT_EQ(1, ee[0]);
        EXPECT_EQ(2, ee[1]);
        EXPECT_EQ(3, ee[2]);
        uids.assign(3, 0);
        EXPECT_THROW(ee.insert(ee.begin(), uids.begin(), uids.end()),
                ValueOutOfRange);

#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        ee.clear();
        ee.insert(ee.begin(), {1, 2, 3});
        EXPECT_EQ(3, ee.size());
        EXPECT_EQ(1, ee[0]);
        EXPECT_EQ(2, ee[1]);
        EXPECT_EQ(3, ee[2]);
        EXPECT_THROW(ee.insert(ee.begin(), {0, 0}), ValueOutOfRange);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
    }


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
    TEST(TrackJoinBlocks, Emplace)
    {
        TrackJoinBlocks ee;
        ee.emplace(ee.begin(), 1);
        EXPECT_EQ(1, ee.size());
        EXPECT_EQ(1, *ee.begin());
        EXPECT_THROW(ee.emplace(ee.begin(), 0), ValueOutOfRange);
    }
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


    TEST(TrackJoinBlocks, erase)
    {
        TrackJoinBlocks ee;
        ee.push_back(1);
        ee.push_back(2);
        ee.push_back(3);

        ee.erase(++ee.begin());
        EXPECT_EQ(2, ee.size());
        EXPECT_EQ(3, ee[1]);

        ee.clear();
        ee.push_back(1);
        ee.push_back(2);
        ee.push_back(3);
        ee.erase(ee.begin(), ++(++ee.begin()));
        EXPECT_EQ(1, ee.size());
        EXPECT_EQ(3, *ee.begin());
    }


    TEST(TrackJoinBlocks, PushBack)
    {
        TrackJoinBlocks ee;
        ee.push_back(1);
        ee.push_back(2);
        ee.push_back(3);

        unsigned long long int uid(4);
        ee.push_back(uid);
        EXPECT_EQ(4, ee.size());
        EXPECT_EQ(4, ee[3]);
        uid = 0;
        EXPECT_THROW(ee.push_back(uid), ValueOutOfRange);

        // Move version under C++11
        ee.push_back(5);
        EXPECT_EQ(5, ee.size());
        EXPECT_EQ(5, ee[4]);
        EXPECT_THROW(ee.push_back(0), ValueOutOfRange);
    }


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
    TEST(Attachements, EmplaceBack)
    {
        TrackJoinBlocks ee;
        ee.push_back(1);
        ee.push_back(2);
        ee.push_back(3);

        ee.emplace_back(4);
        EXPECT_EQ(4, ee.size());
        EXPECT_EQ(4, ee[3]);
        EXPECT_THROW(ee.emplace_back(0), ValueOutOfRange);
    }
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


    TEST(TrackJoinBlocks, PopBack)
    {
        TrackJoinBlocks ee;
        ee.push_back(1);
        ee.push_back(2);
        ee.push_back(3);

        EXPECT_EQ(3, ee.size());
        ee.pop_back();
        EXPECT_EQ(2, ee.size());
        EXPECT_EQ(2, *(--ee.end()));
    }


    TEST(TrackJoinBlocks, Resize)
    {
        TrackJoinBlocks ee;
        ee.push_back(1);
        ee.push_back(2);
        ee.push_back(3);

        EXPECT_EQ(3, ee.size());
        ee.resize(4, 4);
        EXPECT_EQ(4, ee.size());
        EXPECT_EQ(4, *(--ee.end()));
        ee.resize(3, 4);
        EXPECT_EQ(3, ee.size());
        EXPECT_EQ(3, *(--ee.end()));
        EXPECT_THROW(ee.resize(4, 0), ValueOutOfRange);

#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        ee.resize(2);
        EXPECT_EQ(2, ee.size());
        EXPECT_EQ(2, *(--ee.end()));
        ee.resize(3, 3);
        EXPECT_EQ(3, ee.size());
        EXPECT_EQ(3, *(--ee.end()));
        EXPECT_THROW(ee.resize(4, 0), ValueOutOfRange);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
    }


    ///////////////////////////////////////////////////////////////////////////
    // MasterElement interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST(TrackJoinBlocks, EnableCRC)
    {
        TrackJoinBlocks ee;
        EXPECT_TRUE(ee.crc_enabled());
        ee.disable_crc();
        EXPECT_FALSE(ee.crc_enabled());
        ee.enable_crc();
        EXPECT_TRUE(ee.crc_enabled());
    }


    ///////////////////////////////////////////////////////////////////////////
    // Element interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST(TrackJoinBlocks, ID)
    {
        TrackJoinBlocks ee;
        EXPECT_EQ(ids::TrackJoinBlocks, ee.id());
    }


    TEST(TrackJoinBlocks, Offset)
    {
        TrackJoinBlocks ee;
        EXPECT_EQ(0, ee.offset());

        std::stringstream ss;
        std::string input_val;

        // Set when read test
        IntElement ie(ids::Null, 42);
        write(ie, ss);
        std::vector<ElPtr> children;
        populate_children(children);
        fill_buffer(input_val, ids::TrackJoinBlocks, children, true, true, true);
        ss << input_val;
        ss.seekg(ie.stored_size() + ids::size(ids::TrackJoinBlocks));
        ee.read(ss);
        EXPECT_EQ(ie.stored_size(), ee.offset());

        // Set when written test
        write(ee, ss);
        EXPECT_EQ(ie.stored_size() + ee.stored_size(), ee.offset());
    }


    TEST(TrackJoinBlocks, StoredSize)
    {
        TrackJoinBlocks ee;
        ee.disable_crc();

        // Basic size (empty data)
        EXPECT_EQ(ids::size(ids::TrackJoinBlocks) +
                vint::size(0), ee.stored_size());

        // Size with data
        std::vector<ElPtr> children;
        populate_children(children);
        std::streamsize body_size(children_size(children));
        populate_element(ee, children);
        EXPECT_EQ(ids::size(ids::TrackJoinBlocks) +
                vint::size(body_size) + body_size, ee.stored_size());

        // Size with CRC enabled
        ee.enable_crc();
        body_size += 6;
        EXPECT_EQ(ids::size(ids::TrackJoinBlocks) +
                vint::size(body_size) + body_size, ee.stored_size());
        ee.disable_crc();
    }


    TEST(TrackJoinBlocks, Read)
    {
        std::vector<ElPtr> children;
        populate_children(children);
        std::streamsize body_size(children_size(children));
        std::string buffer;
        fill_buffer(buffer, ids::TrackJoinBlocks, children, false, true, true);
        std::stringstream input(buffer);

        TrackJoinBlocks ee;
        EXPECT_EQ(vint::size(body_size) + body_size, ee.read(input));
        check_children_match(ee, children);

        // Body size value wrong (too small)
        input.str(std::string());
        vint::write(2, input);
        write(*children[0], input);
        write(*children[1], input);
        EXPECT_THROW(ee.read(input), ReadError);
        // Post-condition test: error should leave element as before
        check_children_match(ee, children);

        // Invalid child
        input.str(std::string());
        input.clear();
        UIntElement ue(ids::EBML, 0xFFFF);
        vint::write(ue.stored_size(), input);
        write(ue, input);
        EXPECT_THROW(ee.read(input), InvalidChildID);
        // Post-condition test: error should leave element as before
        check_children_match(ee, children);

        // Child with a bad value
        children.push_back(ElPtr(new UIntElement(ids::TrackJoinUID, 0)));
        std::string().swap(buffer);
        fill_buffer(buffer, ids::TrackJoinBlocks, children, false, true, true);
        input.str(buffer);
        EXPECT_THROW(ee.read(input), ValueOutOfRange);
        // Post-condition test: error should leave element as before
        children.pop_back();
        check_children_match(ee, children);
    }


    TEST(TrackJoinBlocks, ReadWithCRC)
    {
        std::vector<ElPtr> children;
        populate_children(children);
        std::streamsize body_size(children_size(children) + 6); // +6 for CRC
        std::string buffer;
        fill_buffer(buffer, ids::TrackJoinBlocks, children, false, true, true, true);
        std::stringstream input(buffer);

        TrackJoinBlocks ee;
        ee.disable_crc();
        EXPECT_EQ(vint::size(body_size) + body_size, read(ee, input));
        check_children_match(ee, children);
        EXPECT_TRUE(ee.crc_enabled());

        // Bad CRC
        std::string().swap(buffer);
        fill_buffer(buffer, ids::TrackJoinBlocks, children, false, true, true, true);
        // Corrupt the CRC
        buffer[3] = 0x01;
        buffer[4] = 0x02;
        // Attempt to read
        input.str(buffer);
        EXPECT_THROW(read(ee, input), BadCRC);
        // Post-condition test: error should leave element as before
        check_children_match(ee, children);
        EXPECT_TRUE(ee.crc_enabled());

        // Test automatic enabling/disabling of CRC
        ee.disable_crc();
        std::string().swap(buffer);
        fill_buffer(buffer, ids::TrackJoinBlocks, children, false, true, true, true);
        input.str(buffer);
        read(ee, input);
        EXPECT_TRUE(ee.crc_enabled());
        std::string().swap(buffer);
        fill_buffer(buffer, ids::TrackJoinBlocks, children, false, true, true, false);
        input.str(buffer);
        read(ee, input);
        EXPECT_FALSE(ee.crc_enabled());
    }


    TEST(TrackJoinBlocks, StartWrite)
    {
        std::vector<ElPtr> children;
        populate_children(children);
        TrackJoinBlocks ee;
        populate_element(ee, children);
        ee.disable_crc();

        std::string expected;
        fill_buffer(expected, ids::TrackJoinBlocks, children, true, true, true);

        std::stringstream output;
        std::streamsize body_size(children_size(children));
        EXPECT_EQ(ids::size(ids::TrackJoinBlocks) + vint::size(body_size) +
                body_size, ee.start_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
                expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::TrackJoinBlocks) + vint::size(body_size) +
                body_size, output.tellp());

        // Test to avoid writing bad data
        *ee.begin() = 0;
        EXPECT_THROW(ee.start_write(output), ValueOutOfRange);
    }


    TEST(TrackJoinBlocks, FinishWrite)
    {
        std::vector<ElPtr> children;
        populate_children(children);
        TrackJoinBlocks ee;
        populate_element(ee, children);
        ee.disable_crc();

        // Test for exception and no change to output when finishing without
        // starting
        std::stringstream output;
        EXPECT_THROW(ee.finish_write(output), NotWriting);
        EXPECT_EQ(0, output.tellp());

        std::string expected;
        fill_buffer(expected, ids::TrackJoinBlocks, children, true, true, true);

        std::streamsize body_size(children_size(children));
        ee.start_write(output);
        EXPECT_EQ(ids::size(ids::TrackJoinBlocks) + vint::size(body_size) +
                body_size, ee.finish_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
                expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::TrackJoinBlocks) + vint::size(body_size) +
                body_size, output.tellp());

        // Test for double-finish raising an exception
        EXPECT_THROW(ee.finish_write(output), NotWriting);
    }


    TEST(TrackJoinBlocks, Write)
    {
        std::vector<ElPtr> children;
        populate_children(children);
        TrackJoinBlocks ee;
        populate_element(ee, children);
        ee.disable_crc();

        std::string expected;
        fill_buffer(expected, ids::TrackJoinBlocks, children, true, true, true);

        std::stringstream output;
        std::streamsize body_size(children_size(children));
        EXPECT_EQ(ids::size(ids::TrackJoinBlocks) + vint::size(body_size) +
                body_size, write(ee, output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
                expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::TrackJoinBlocks) + vint::size(body_size) +
                body_size, output.tellp());

        // Test for double-finish raising an exception
        EXPECT_THROW(ee.finish_write(output), NotWriting);
    }


    TEST(TrackJoinBlocks, WriteWithCRC)
    {
        std::vector<ElPtr> children;
        populate_children(children);
        TrackJoinBlocks ee;
        populate_element(ee, children);
        ee.disable_crc();

        std::string expected;
        fill_buffer(expected, ids::TrackJoinBlocks, children, true, true, true,
                true);

        ee.enable_crc();
        std::stringstream output;
        std::streamsize body_size(children_size(children) + 6); // +6 for CRC
        EXPECT_EQ(ids::size(ids::TrackJoinBlocks) + vint::size(body_size) +
                body_size, write(ee, output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
                expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::TrackJoinBlocks) + vint::size(body_size) +
                body_size, output.tellp());
    }


    TEST(TrackJoinBlocks, RoundTrip)
    {
        std::vector<ElPtr> children;
        populate_children(children);
        TrackJoinBlocks ee1;
        populate_element(ee1, children);

        std::stringstream io;
        std::streamsize written_bytes = write(ee1, io);
        // Skip the ID
        io.seekg(ids::size(ids::TrackJoinBlocks), std::ios::beg);
        TrackJoinBlocks ee2;
        std::streamsize read_bytes = read(ee2, io);

        check_children_match(ee2, children);
        EXPECT_TRUE(ee2.crc_enabled());
    }
}; // namespace test_track_join_blocks

