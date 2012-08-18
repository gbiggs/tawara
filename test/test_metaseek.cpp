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
#include <jonen/integer_elements.h>
#include <jonen/metaseek.h>
#include <jonen/seek.h>

#include "test_utilities.h"
#include <boost/crc.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>

using namespace jonen;

namespace test_metaseek
{
    ///////////////////////////////////////////////////////////////////////////
    // Metaseek interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST(Metaseek, Construction)
    {
        EXPECT_EQ(ids::Metaseek, Metaseek().id());

        Metaseek ee;
        EXPECT_EQ(ee.begin(), ee.end());
        EXPECT_TRUE(ee.empty());
        EXPECT_EQ(0, ee.size());
    }


    TEST(Metaseek, CopyConstruction)
    {
        Metaseek ee1;
        ee1.insert(make_ms_entry(ids::Null, 1234));

        Metaseek ee2(ee1);
        EXPECT_FALSE(ee2.empty());
        EXPECT_TRUE(ee1.size() == ee2.size());
        EXPECT_TRUE(*ee1.find(ids::Null) == *ee2.find(ids::Null));
    }


    TEST(Metaseek, Assignment)
    {
        Metaseek ee1;
        ee1.insert(make_ms_entry(ids::Null, 1234));

        Metaseek ee2;
        EXPECT_TRUE(ee2.empty());
        ee2 = ee1;
        EXPECT_FALSE(ee2.empty());
        EXPECT_TRUE(ee1.size() == ee2.size());
        EXPECT_TRUE(*ee1.find(ids::Null) == *ee2.find(ids::Null));
    }


    TEST(Metaseek, Swap)
    {
        Metaseek ee1, ee2;
        ee1.insert(make_ms_entry(ids::Null, 1234));
        EXPECT_FALSE(ee1.empty());
        EXPECT_TRUE(ee2.empty());
        EXPECT_NE(ee1.end(), ee1.find(ids::Null));
        EXPECT_EQ(ee2.end(), ee2.find(ids::Null));

        swap(ee1, ee2);
        EXPECT_TRUE(ee1.empty());
        EXPECT_FALSE(ee2.empty());
        EXPECT_EQ(ee1.end(), ee1.find(ids::Null));
        EXPECT_NE(ee2.end(), ee2.find(ids::Null));
    }


    ///////////////////////////////////////////////////////////////////////////
    // Multimap interface tests
    ///////////////////////////////////////////////////////////////////////////


    TEST(Metaseek, GetAllocator)
    {
        // TODO: How can this be tested?
    }


    TEST(Metaseek, Equal)
    {
        Metaseek ee1;
        ee1.insert(make_ms_entry(ids::Null, 1234));

        Metaseek ee2;
        ee2.insert(make_ms_entry(ids::Null, 1234));

        EXPECT_TRUE(ee1 == ee2);

        Metaseek ee3;
        EXPECT_FALSE(ee1 == ee3);
    }


    TEST(Metaseek, LessThan)
    {
        Metaseek ee1;
        ee1.insert(make_ms_entry(ids::Null, 1));
        Metaseek ee2;
        ee2.insert(make_ms_entry(ids::Null, 2));

        EXPECT_TRUE(ee1 < ee2);
        EXPECT_FALSE(ee2 < ee1);
    }


    TEST(Metaseek, LessThanEqual)
    {
        Metaseek ee1;
        ee1.insert(make_ms_entry(ids::Null, 1));
        Metaseek ee2;
        ee2.insert(make_ms_entry(ids::Null, 1));
        Metaseek ee3;
        ee3.insert(make_ms_entry(ids::Null, 2));

        EXPECT_TRUE(ee1 <= ee2);
        EXPECT_TRUE(ee1 <= ee3);
        EXPECT_FALSE(ee3 <= ee2);
    }


    TEST(Metaseek, NotEqual)
    {
        Metaseek ee1;
        ee1.insert(make_ms_entry(ids::Null, 1));
        Metaseek ee2;
        ee2.insert(make_ms_entry(ids::Null, 1));
        Metaseek ee3;
        ee3.insert(make_ms_entry(ids::Null, 2));

        EXPECT_FALSE(ee1 != ee2);
        EXPECT_TRUE(ee1 != ee3);
    }


    TEST(Metaseek, GreaterThanEqual)
    {
        Metaseek ee1;
        ee1.insert(make_ms_entry(ids::Null, 2));
        Metaseek ee2;
        ee2.insert(make_ms_entry(ids::Null, 2));
        Metaseek ee3;
        ee3.insert(make_ms_entry(ids::Null, 1));

        EXPECT_TRUE(ee1 >= ee2);
        EXPECT_TRUE(ee1 >= ee3);
        EXPECT_FALSE(ee3 >= ee2);
    }


    TEST(Metaseek, GreaterThan)
    {
        Metaseek ee1;
        ee1.insert(make_ms_entry(ids::Null, 2));
        Metaseek ee2;
        ee2.insert(make_ms_entry(ids::Null, 1));

        EXPECT_TRUE(ee1 > ee2);
        EXPECT_FALSE(ee2 > ee1);
    }


    TEST(Metaseek, Begin)
    {
        Metaseek ee;
        ee.insert(make_ms_entry(ids::Null, 1234));
        ee.insert(make_ms_entry(ids::CRC32, 5678));
        EXPECT_EQ(ids::Null, ee.begin()->first);
        EXPECT_EQ(1234, ee.begin()->second);
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        EXPECT_EQ(ids::Null, ee.cbegin()->first);
        EXPECT_EQ(1234, ee.cbegin()->second);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
        ee.begin()->second = 4321;
        EXPECT_EQ(4321, ee.begin()->second);
        Metaseek const ee_const(ee);
        EXPECT_EQ(ids::Null, ee_const.begin()->first);
        EXPECT_EQ(4321, ee_const.begin()->second);
    }


    TEST(Metaseek, End)
    {
        Metaseek ee;
        ee.insert(make_ms_entry(ids::Null, 1234));
        ee.insert(make_ms_entry(ids::CRC32, 5678));
        EXPECT_EQ(ids::CRC32, (--ee.end())->first);
        EXPECT_EQ(5678, (--ee.end())->second);
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        EXPECT_EQ(ids::CRC32, (--ee.cend())->first);
        EXPECT_EQ(5678, (--ee.cend())->second);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
        (--ee.end())->second = 4321;
        EXPECT_EQ(ids::CRC32, (--ee.end())->first);
        EXPECT_EQ(4321, (--ee.end())->second);
        Metaseek const ee_const(ee);
        EXPECT_EQ(ids::CRC32, (--ee_const.end())->first);
        EXPECT_EQ(4321, (--ee_const.end())->second);
    }


    TEST(Metaseek, RBegin)
    {
        Metaseek ee;
        ee.insert(make_ms_entry(ids::Null, 1234));
        ee.insert(make_ms_entry(ids::CRC32, 5678));
        EXPECT_EQ(ids::CRC32, ee.rbegin()->first);
        EXPECT_EQ(5678, ee.rbegin()->second);
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        EXPECT_EQ(ids::CRC32, ee.crbegin()->first);
        EXPECT_EQ(5678, ee.crbegin()->second);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
        ee.rbegin()->second = 4321;
        EXPECT_EQ(ids::CRC32, ee.rbegin()->first);
        EXPECT_EQ(4321, ee.rbegin()->second);
        Metaseek const ee_const(ee);
        EXPECT_EQ(ids::CRC32, ee_const.rbegin()->first);
        EXPECT_EQ(4321, ee_const.rbegin()->second);
    }


    TEST(Metaseek, REnd)
    {
        Metaseek ee;
        ee.insert(make_ms_entry(ids::Null, 1234));
        ee.insert(make_ms_entry(ids::CRC32, 5678));
        EXPECT_EQ(ids::Null, (--ee.rend())->first);
        EXPECT_EQ(1234, (--ee.rend())->second);
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        EXPECT_EQ(ids::Null, (--ee.crend())->first);
        EXPECT_EQ(1234, (--ee.crend())->second);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
        (--ee.rend())->second = 4321;
        EXPECT_EQ(ids::Null, (--ee.rend())->first);
        EXPECT_EQ(4321, (--ee.rend())->second);
        Metaseek const ee_const(ee);
        EXPECT_EQ(ids::Null, (--ee_const.rend())->first);
        EXPECT_EQ(4321, (--ee_const.rend())->second);
    }


    TEST(Metaseek, Empty)
    {
        Metaseek ee;
        EXPECT_TRUE(ee.empty());
        ee.insert(make_ms_entry(ids::Null, 1234));
        EXPECT_FALSE(ee.empty());
    }


    TEST(Metaseek, Size)
    {
        Metaseek ee;
        EXPECT_EQ(0, ee.size());
        ee.insert(make_ms_entry(ids::Null, 1234));
        EXPECT_EQ(1, ee.size());
    }


    TEST(Metaseek, MaxSize)
    {
        // This value differs based on platform and standard library
        // implementation, so the best that can be done is test that the
        // underlying multimap's value is being passed correctly.
        Metaseek ee;
        std::multimap<Metaseek::key_type, Metaseek::mapped_type> map;
        EXPECT_EQ(map.max_size(), ee.max_size());
    }


    TEST(Metaseek, Clear)
    {
        Metaseek ee;
        ee.insert(make_ms_entry(ids::Null, 1234));
        EXPECT_FALSE(ee.empty());
        ee.clear();
        EXPECT_TRUE(ee.empty());
    }


    TEST(Metaseek, Insert)
    {
        Metaseek ee;
        ee.insert(make_ms_entry(ids::Null, 1234));
        EXPECT_EQ(1234, ee.find(ids::Null)->second);

#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        // Move version under C++11
        ee.insert(Metaseek::value_type(ids::CRC32, 5678));
#else // defined(JONEN_CPLUSPLUS11_SUPPORT)
        Metaseek::value_type entry(ids::CRC32, 5678);
        ee.insert(entry);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
        EXPECT_EQ(5678, ee.find(ids::CRC32)->second);

        // Insert with hint
        ee.insert(ee.begin(), make_ms_entry(ids::Null, 8765));
        EXPECT_EQ(ids::Null, ee.begin()->first);
        EXPECT_EQ(8765, ee.begin()->second);

#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        // Insert with const hint
        ee.insert(ee.cbegin(), make_ms_entry(ids::EBML, 4321));
        EXPECT_EQ(ids::EBML, ee.begin()->first);
        EXPECT_EQ(4321, ee.find(ids::EBML)->second);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)

#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        ee.clear();
        ee.insert(make_ms_entry(ids::Null, 1234));
        // Move-insert
        ee.insert(ee.begin(), entry);
        EXPECT_EQ(5678, ee.find(ids::CRC32)->second);
        EXPECT_EQ(ids::CRC32, ee.begin()->first);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)

        ee.clear();
        ee.insert(make_ms_entry(ids::Null, 1234));
        // Insert range
        Metaseek ee2;
        ee2.insert(make_ms_entry(ids::CRC32, 5678));
        ee2.insert(make_ms_entry(ids::EBML, 4321));
        ee2.insert(make_ms_entry(ids::Void, 8765));
        ee.insert(ee2.begin(), ee2.end());
        EXPECT_EQ(5678, ee.find(ids::CRC32)->second);
        EXPECT_EQ(4321, ee.find(ids::EBML)->second);
        EXPECT_EQ(8765, ee.find(ids::Void)->second);
    }


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
    TEST(Metaseek, Emplace)
    {
        Metaseek ee;
        ee.emplace(ids::Null, 1234);
        EXPECT_EQ(1234, ee[ids::Null]);

        ee.emplace_hint(ee.begin(), ids::CRC32, 5678);
        EXPECT_EQ(ids::CRC32, ee.begin()->first);
        EXPECT_EQ(5678, ee[ids::CRC32]);
    }
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


    TEST(Metaseek, Erase)
    {
        Metaseek ee;
        ee.insert(make_ms_entry(ids::Null, 1234));
        ee.insert(make_ms_entry(ids::CRC32, 5678));
        ee.insert(make_ms_entry(ids::EBML, 4321));
        ee.insert(make_ms_entry(ids::Void, 8765));

        Metaseek::value_type erased(*ee.begin());
        ee.erase(ee.begin());
        EXPECT_EQ(ee.end(), ee.find(erased.first));
        EXPECT_EQ(3, ee.size());

        // Erase range
        ee.erase(ee.begin(), ++(++ee.begin()));
        EXPECT_EQ(1, ee.size());

        // Erase by key
        ee.erase(ee.begin()->first);
        EXPECT_TRUE(ee.empty());
    }


    TEST(Metaseek, Count)
    {
        Metaseek ee;
        ee.insert(make_ms_entry(ids::Null, 1234));
        ee.insert(make_ms_entry(ids::CRC32, 5678));
        ee.insert(make_ms_entry(ids::CRC32, 9012));

        EXPECT_EQ(2, ee.count(ids::CRC32));
    }


    TEST(Metaseek, Find)
    {
        Metaseek ee;
        ee.insert(make_ms_entry(ids::Null, 1234));
        ee.insert(make_ms_entry(ids::CRC32, 5678));
        ee.insert(make_ms_entry(ids::CRC32, 9012));
        ee.insert(make_ms_entry(ids::EBML, 4321));
        ee.insert(make_ms_entry(ids::Void, 8765));

        // The first to be inserted should be returned
        EXPECT_EQ(5678, ee.find(ids::CRC32)->second);
    }


    TEST(Metaseek, EqualRange)
    {
        Metaseek ee;
        ee.insert(make_ms_entry(ids::Null, 1234));
        ee.insert(make_ms_entry(ids::CRC32, 5678));
        ee.insert(make_ms_entry(ids::CRC32, 9012));
        ee.insert(make_ms_entry(ids::Void, 8765));

        std::pair<Metaseek::iterator, Metaseek::iterator> result;
        result = ee.equal_range(ids::CRC32);
        EXPECT_EQ(ids::CRC32, result.first->first);
        EXPECT_EQ(ids::Void, result.second->first);

        result = ee.equal_range(ids::Void);
        EXPECT_EQ(ids::Void, result.first->first);
        EXPECT_EQ(ee.end(), result.second);

        Metaseek ee_const(ee);
        result = ee_const.equal_range(ids::CRC32);
        EXPECT_EQ(ids::CRC32, result.first->first);
        EXPECT_EQ(ids::Void, result.second->first);

        result = ee_const.equal_range(ids::Void);
        EXPECT_EQ(ids::Void, result.first->first);
        EXPECT_EQ(ee_const.end(), result.second);
    }


    TEST(Metaseek, LowerBound)
    {
        Metaseek ee;
        ee.insert(make_ms_entry(ids::Null, 1234));
        ee.insert(make_ms_entry(ids::CRC32, 5678));
        ee.insert(make_ms_entry(ids::CRC32, 9012));
        ee.insert(make_ms_entry(ids::Void, 8765));

        EXPECT_EQ(ids::CRC32, ee.lower_bound(ids::CRC32)->first);
        EXPECT_EQ(ee.end(), ee.lower_bound(ids::EBML));

        Metaseek ee_const(ee);
        EXPECT_EQ(ids::CRC32, ee_const.lower_bound(ids::CRC32)->first);
        EXPECT_EQ(ee_const.end(), ee_const.lower_bound(ids::EBML));
    }


    TEST(Metaseek, UpperBound)
    {
        Metaseek ee;
        ee.insert(make_ms_entry(ids::Null, 1234));
        ee.insert(make_ms_entry(ids::CRC32, 5678));
        ee.insert(make_ms_entry(ids::CRC32, 9012));
        ee.insert(make_ms_entry(ids::Void, 8765));

        EXPECT_EQ(ids::Void, ee.upper_bound(ids::CRC32)->first);
        EXPECT_EQ(ee.end(), ee.upper_bound(ids::Void));

        Metaseek ee_const(ee);
        EXPECT_EQ(ids::Void, ee_const.upper_bound(ids::CRC32)->first);
        EXPECT_EQ(ee_const.end(), ee_const.upper_bound(ids::Void));
    }


    ///////////////////////////////////////////////////////////////////////////
    // MasterElement interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST(Metaseek, EnableCRC)
    {
        Metaseek ee;
        EXPECT_TRUE(ee.crc_enabled());
        ee.disable_crc();
        EXPECT_FALSE(ee.crc_enabled());
        ee.enable_crc();
        EXPECT_TRUE(ee.crc_enabled());
    }


    ///////////////////////////////////////////////////////////////////////////
    // Element interface tests
    ///////////////////////////////////////////////////////////////////////////

    typedef boost::shared_ptr<Element> ElPtr;


    std::streamsize children_size(std::vector<ElPtr> const& children)
    {
        std::streamsize body_size(0);
        BOOST_FOREACH(ElPtr el, children)
        {
            body_size += el->stored_size();
        }
        return body_size;
    }

    std::streamsize fill_buffer(std::string& b,
            std::vector<ElPtr> const& children, bool write_id, bool write_size,
            bool write_body, bool use_crc=false)
    {
        char temp[8];
        std::streamsize total(0);
        if (write_id)
        {
            // Cheating on the IDs a bit - there is no protection here against
            // invalid IDs
            std::vector<char> tmp(ebml_int::encode_u(ids::Metaseek));
            b.append(&tmp[0], 0, tmp.size());
            total += tmp.size();
        }
        if (write_size)
        {
            int crc_size(0);
            if (use_crc)
            {
                crc_size = 6;
            }
            std::vector<char> tmp(vint::encode(children_size(children) +
                        crc_size));
            b.append(&tmp[0], 0, tmp.size());
            total += tmp.size();
        }
        if (write_body)
        {
            std::stringstream ss;
            BOOST_FOREACH(ElPtr el, children)
            {
                total += write(*el, ss);
            }
            if (use_crc)
            {
                boost::crc_32_type crc;
                crc.process_bytes(ss.str().c_str(), ss.str().length());
                BinaryElement crc_el(ids::CRC32, std::vector<char>());
                crc_el.push_back((crc.checksum() & 0x000000FF));
                crc_el.push_back((crc.checksum() & 0x0000FF00) >> 8);
                crc_el.push_back((crc.checksum() & 0x00FF0000) >> 16);
                crc_el.push_back((crc.checksum() & 0xFF000000) >> 24);
                std::stringstream crc_ss;
                total += write(crc_el, crc_ss);
                b += crc_ss.str();
            }
            b += ss.str();
        }
        return total;
    }

    TEST(Metaseek, ID)
    {
        Metaseek ee;
        EXPECT_EQ(ids::Metaseek, ee.id());
    }

    TEST(Metaseek, Offset)
    {
        Metaseek ee;
        EXPECT_EQ(0, ee.offset());

        std::stringstream ss;
        std::string input_val;

        // Set when read test
        IntElement ie(ids::Null, 42);
        write(ie, ss);
        std::vector<ElPtr> children;
        children.push_back(ElPtr(new impl::Seek(ids::Null, 1234)));
        children.push_back(ElPtr(new impl::Seek(ids::CRC32, 5678)));
        fill_buffer(input_val, children, true, true, true);
        ss << input_val;
        ss.seekg(ie.stored_size() + ids::size(ids::Metaseek));
        ee.read(ss);
        EXPECT_EQ(ie.stored_size(), ee.offset());

        // Set when written test
        write(ee, ss);
        EXPECT_EQ(ie.stored_size() + ee.stored_size(), ee.offset());
    }

    TEST(Metaseek, StoredSize)
    {
        Metaseek ee;
        ee.disable_crc();

        std::vector<ElPtr> children;
        children.push_back(ElPtr(new impl::Seek(ids::Null, 1234)));
        children.push_back(ElPtr(new impl::Seek(ids::CRC32, 5678)));

        // Default size
        std::streamsize body_size(0);
        EXPECT_EQ(ids::size(ids::Metaseek) +
                vint::size(body_size) + body_size, ee.stored_size());

        // Size with data
        ee.insert(make_ms_entry(ids::Null, 1234));
        ee.insert(make_ms_entry(ids::CRC32, 5678));
        body_size = children[0]->stored_size() + children[1]->stored_size();
        EXPECT_EQ(ids::size(ids::Metaseek) +
                vint::size(body_size) + body_size, ee.stored_size());

        // Size with CRC enabled
        ee.enable_crc();
        body_size += 6;
        EXPECT_EQ(ids::size(ids::Metaseek) +
                vint::size(body_size) + body_size, ee.stored_size());
        ee.disable_crc();
    }

    TEST(Metaseek, Read)
    {
        std::stringstream input;
        std::vector<ElPtr> children;
        children.push_back(ElPtr(new impl::Seek(ids::Null, 1234)));
        children.push_back(ElPtr(new impl::Seek(ids::CRC32, 5678)));

        Metaseek ee;

        std::streamsize body_size(0);
        BOOST_FOREACH(ElPtr el, children)
        {
            body_size += el->stored_size();
        }
        vint::write(body_size, input);
        BOOST_FOREACH(ElPtr el, children)
        {
            write(*el, input);
        }

        EXPECT_EQ(vint::size(body_size) + body_size,
                ee.read(input));
        EXPECT_EQ(2, ee.size());
        EXPECT_EQ(1234, ee.find(ids::Null)->second);
        EXPECT_EQ(5678, ee.find(ids::CRC32)->second);

        // Body size value wrong (too small)
        input.str(std::string());
        vint::write(2, input);
        write(*children[0], input);
        write(*children[1], input);
        EXPECT_THROW(ee.read(input), ReadError);
        // Post-condition test: error should leave element as before
        EXPECT_EQ(2, ee.size());
        EXPECT_EQ(1234, ee.find(ids::Null)->second);
        EXPECT_EQ(5678, ee.find(ids::CRC32)->second);

        // Invalid child
        input.str(std::string());
        input.clear();
        UIntElement ue(ids::EBML, 0xFFFF);
        vint::write(ue.stored_size(), input);
        write(ue, input);
        EXPECT_THROW(ee.read(input), InvalidChildID);
        // Post-condition test: error should leave element as before
        EXPECT_EQ(2, ee.size());
        EXPECT_EQ(1234, ee.find(ids::Null)->second);
        EXPECT_EQ(5678, ee.find(ids::CRC32)->second);
    }

    TEST(Metaseek, ReadWithCRC)
    {
        std::vector<ElPtr> children;
        children.push_back(ElPtr(new impl::Seek(ids::Null, 1234)));
        children.push_back(ElPtr(new impl::Seek(ids::CRC32, 5678)));

        std::string buffer;
        std::streamsize read_size(0);
        read_size = fill_buffer(buffer, children, false, true, true, true);

        Metaseek ee;
        ee.disable_crc();
        std::stringstream input(buffer);
        EXPECT_EQ(read_size, read(ee, input));
        EXPECT_EQ(2, ee.size());
        EXPECT_EQ(1234, ee.find(ids::Null)->second);
        EXPECT_EQ(5678, ee.find(ids::CRC32)->second);
        EXPECT_TRUE(ee.crc_enabled());

        // Bad CRC
        std::string().swap(buffer);
        read_size += fill_buffer(buffer, children, false, true, true, true);
        // Corrupt the CRC
        buffer[3] = 0x01;
        buffer[4] = 0x02;
        // Attempt to read
        input.str(buffer);
        EXPECT_THROW(read(ee, input), BadCRC);
        // Post-condition test: error should leave element as before
        EXPECT_EQ(2, ee.size());
        EXPECT_EQ(1234, ee.find(ids::Null)->second);
        EXPECT_EQ(5678, ee.find(ids::CRC32)->second);
        EXPECT_TRUE(ee.crc_enabled());

        // Test automatic enabling/disabling of CRC
        ee.disable_crc();
        std::string().swap(buffer);
        fill_buffer(buffer, children, false, true, true, true);
        input.str(buffer);
        read(ee, input);
        EXPECT_TRUE(ee.crc_enabled());
        std::string().swap(buffer);
        fill_buffer(buffer, children, false, true, true, false);
        input.str(buffer);
        read(ee, input);
        EXPECT_FALSE(ee.crc_enabled());
    }

    TEST(Metaseek, StartWrite)
    {
        std::stringstream output;
        std::string expected;
        Metaseek ee;
        ee.insert(make_ms_entry(ids::Null, 1234));
        ee.insert(make_ms_entry(ids::CRC32, 5678));
        ee.disable_crc();

        std::vector<ElPtr> children;
        children.push_back(ElPtr(new impl::Seek(ids::Null, 1234)));
        children.push_back(ElPtr(new impl::Seek(ids::CRC32, 5678)));
        fill_buffer(expected, children, true, true, true);

        std::streamsize body_size(children_size(children));
        EXPECT_EQ(ids::size(ids::Metaseek) + vint::size(body_size) + body_size,
                ee.start_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Metaseek) + vint::size(body_size) + body_size,
                output.tellp());
    }

    TEST(Metaseek, FinishWrite)
    {
        std::stringstream output;
        std::string expected;
        Metaseek ee;
        ee.insert(make_ms_entry(ids::Null, 1234));
        ee.insert(make_ms_entry(ids::CRC32, 5678));
        ee.disable_crc();

        // Test for exception and no change to output when finishing without
        // starting
        EXPECT_THROW(ee.finish_write(output), NotWriting);
        EXPECT_EQ(0, output.tellp());

        std::vector<ElPtr> children;
        children.push_back(ElPtr(new impl::Seek(ids::Null, 1234)));
        children.push_back(ElPtr(new impl::Seek(ids::CRC32, 5678)));
        fill_buffer(expected, children, true, true, true);

        std::streamsize body_size(children_size(children));
        ee.start_write(output);
        EXPECT_EQ(ids::size(ids::Metaseek) + vint::size(body_size) + body_size,
                ee.finish_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Metaseek) + vint::size(body_size) + body_size,
                output.tellp());

        // Test for double-finish raising an exception
        EXPECT_THROW(ee.finish_write(output), NotWriting);
    }

    TEST(Metaseek, Write)
    {
        std::stringstream output;
        std::string expected;
        Metaseek ee;
        ee.insert(make_ms_entry(ids::Null, 1234));
        ee.insert(make_ms_entry(ids::CRC32, 5678));
        ee.disable_crc();

        std::vector<ElPtr> children;
        children.push_back(ElPtr(new impl::Seek(ids::Null, 1234)));
        children.push_back(ElPtr(new impl::Seek(ids::CRC32, 5678)));
        fill_buffer(expected, children, true, true, true);

        std::streamsize body_size(children_size(children));
        EXPECT_EQ(ids::size(ids::Metaseek) + vint::size(body_size) + body_size,
                write(ee, output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Metaseek) + vint::size(body_size) + body_size,
                output.tellp());

        // Test for double-finish raising an exception
        EXPECT_THROW(ee.finish_write(output), NotWriting);
    }

    TEST(Metaseek, WriteWithCRC)
    {
        std::stringstream output;
        std::string expected;
        Metaseek ee;
        ee.insert(make_ms_entry(ids::Null, 1234));
        ee.insert(make_ms_entry(ids::CRC32, 5678));

        std::vector<ElPtr> children;
        children.push_back(ElPtr(new impl::Seek(ids::Null, 1234)));
        children.push_back(ElPtr(new impl::Seek(ids::CRC32, 5678)));
        fill_buffer(expected, children, true, true, true, true);

        ee.enable_crc();
        std::streamsize body_size(children_size(children) + 6);
        EXPECT_EQ(ids::size(ids::Metaseek) + vint::size(body_size) + body_size,
                write(ee, output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Metaseek) + vint::size(body_size) + body_size,
                output.tellp());
    }

    TEST(Metaseek, RoundTrip)
    {
        std::stringstream io;
        Metaseek ee1;
        ee1.insert(make_ms_entry(ids::Null, 1234));
        ee1.insert(make_ms_entry(ids::CRC32, 5678));

        std::streamsize written_bytes = write(ee1, io);
        // Skip the ID
        io.seekg(ids::size(ids::Metaseek), std::ios::beg);
        Metaseek ee2;
        std::streamsize read_bytes = read(ee2, io);

        EXPECT_EQ(written_bytes - ids::size(ids::Metaseek), read_bytes);
        EXPECT_EQ(2, ee2.size());
        EXPECT_EQ(1234, ee2.find(ids::Null)->second);
        EXPECT_EQ(5678, ee2.find(ids::CRC32)->second);
        EXPECT_TRUE(ee2.crc_enabled());
    }
}; // namespace test_metaseek
