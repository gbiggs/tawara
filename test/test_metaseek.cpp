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
        EXPECT_TRUE(ee.crc_enabled());
    }


    TEST(Metaseek, CopyConstruction)
    {
        Metaseek ee1;
        ee1.disable_crc();

        Metaseek ee2(ee1);
        EXPECT_TRUE(ee1 == ee2);
        EXPECT_FALSE(ee2.crc_enabled());
    }


    TEST(Metaseek, Assignment)
    {
        Metaseek ee1;
        ee1.disable_crc();

        Metaseek ee2;
        ee2 = ee1;
        EXPECT_TRUE(ee1 == ee2);
        EXPECT_FALSE(ee2.crc_enabled());
    }


    TEST(Metaseek, Swap)
    {
        Metaseek ee1;
        ee1.enable_crc();

        Metaseek ee2;
        ee2.disable_crc();
        swap(ee1, ee2);

        EXPECT_FALSE(ee1.crc_enabled());

        EXPECT_TRUE(ee2.crc_enabled());
    }


    TEST(Metaseek, Equal)
    {
        Metaseek ee1;
        ee1.enable_crc();

        Metaseek ee2;
        ee2.enable_crc();

        EXPECT_TRUE(ee1 == ee2);

        //Metaseek ee3;
        //EXPECT_FALSE(ee1 == ee3);
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

        // Default size
        std::streamsize body_size(0);
        EXPECT_EQ(ids::size(ids::Metaseek) +
                vint::size(body_size) + body_size, ee.stored_size());

        // Size with CRC enabled
        ee.enable_crc();
        body_size = +6;
        EXPECT_EQ(ids::size(ids::Metaseek) +
                vint::size(body_size) + body_size, ee.stored_size());
        ee.disable_crc();

        // Size with data.

        body_size = 0;
        BOOST_FOREACH(ElPtr el, children)
        {
            body_size += el->stored_size();
        }

        Metaseek ee2;
        ee2.disable_crc();
        EXPECT_EQ(ids::size(ids::Metaseek) +
                vint::size(body_size) + body_size, ee2.stored_size());
    }

    TEST(Metaseek, Read)
    {
        std::stringstream input;
        std::vector<ElPtr> children;

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

        // Body size value wrong (too small)
        input.str(std::string());
        vint::write(2, input);
        //write(*children[0], input);
        //write(*children[3], input);
        EXPECT_THROW(ee.read(input), ReadError);
        // Post-condition test: error should leave element as before

        // Invalid child
        input.str(std::string());
        input.clear();
        UIntElement ue(ids::Metaseek, 0xFFFF);
        vint::write(ue.stored_size(), input);
        write(ue, input);
        EXPECT_THROW(ee.read(input), InvalidChildID);
        // Post-condition test: error should leave element as before
    }

    TEST(Metaseek, ReadWithCRC)
    {
        std::vector<ElPtr> children;

        std::string buffer;
        std::streamsize read_size(0);
        read_size = fill_buffer(buffer, children, false, true, true, true);

        Metaseek ee;
        ee.disable_crc();
        std::stringstream input(buffer);
        EXPECT_EQ(read_size, read(ee, input));

        // Bad CRC
        std::string().swap(buffer);
        read_size += fill_buffer(buffer, children, false, true, true, true);
        // Corrupt the CRC
        buffer[3] = 0x01;
        buffer[4] = 0x02;
        // Attempt to read
        input.str(buffer);
        EXPECT_THROW(read(ee, input), BadCRC);

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
        ee.disable_crc();

        std::vector<ElPtr> children;
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
        ee.disable_crc();

        // Test for exception and no change to output when finishing without
        // starting
        EXPECT_THROW(ee.finish_write(output), NotWriting);
        EXPECT_EQ(0, output.tellp());

        std::vector<ElPtr> children;
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
        ee.disable_crc();

        std::vector<ElPtr> children;
        fill_buffer(expected, children, true, true, true);

        std::streamsize body_size(children_size(children));
        EXPECT_EQ(ids::size(ids::Metaseek) + vint::size(body_size) + body_size,
                write(ee, output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Metaseek) + vint::size(body_size) + body_size,
                output.tellp());
    }

    TEST(Metaseek, WriteWithCRC)
    {
        std::stringstream output;
        std::string expected;
        Metaseek ee;

        std::vector<ElPtr> children;
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

        std::streamsize written_bytes = write(ee1, io);
        // Skip the ID
        io.seekg(4, std::ios::beg);
        Metaseek ee2;
        std::streamsize read_bytes = read(ee2, io);

        EXPECT_EQ(written_bytes - 4, read_bytes);
        EXPECT_TRUE(ee2.crc_enabled());
    }
}; // namespace test_metaseek
