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
#include <jonen/ids.h>
#include <jonen/jonen_config.h>
#include <jonen/seek.h>

#include "test_utilities.h"
#include <boost/crc.hpp>
#include <boost/foreach.hpp>

using namespace jonen;
using namespace jonen::impl;

namespace test_seek
{
    class SeekTest : public ::testing::Test
    {
        public:
            SeekTest()
                : test_id(ids::EBML),
                test_id_bin(jonen::ids::encode(ids::EBML)),
                test_id_bin_el(ids::Null, jonen::ids::encode(ids::EBML))
            {
            }

        jonen::ids::ID test_id;
        std::vector<char> test_id_bin;
        BinaryElement test_id_bin_el;
    }; // class SeekTest


    ///////////////////////////////////////////////////////////////////////////
    // Seek interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST_F(SeekTest, Construction)
    {
        EXPECT_EQ(ids::Seek, Seek(ids::Null, 0).id());
        EXPECT_EQ(ids::Null, Seek(ids::Null, 0).element_id());
        EXPECT_EQ(0, Seek(ids::Null, 0).segment_offset());

        EXPECT_EQ(test_id, Seek(test_id, 1234).element_id());
        EXPECT_EQ(1234, Seek(test_id, 1234).segment_offset());

        EXPECT_EQ(test_id, Seek(test_id_bin_el, 1234).element_id());
        EXPECT_EQ(1234, Seek(test_id_bin_el, 1234).segment_offset());

        EXPECT_EQ(test_id, Seek(test_id,
                    UIntElement(ids::Null, 1234)).element_id());
        EXPECT_EQ(1234, Seek(test_id,
                    UIntElement(ids::Null, 1234)).segment_offset());
        // Test the offset element ID is reset
        EXPECT_EQ(ids::SeekPosition,
                Seek(test_id, 1234).segment_offset().id());

        EXPECT_EQ(test_id, Seek(test_id_bin_el,
                    UIntElement(ids::Null, 1234)).element_id());
        EXPECT_EQ(1234, Seek(test_id_bin_el,
                    UIntElement(ids::Null, 1234)).segment_offset());
        // Test the offset element ID is reset
        EXPECT_EQ(ids::SeekPosition,
                Seek(test_id, 1234).segment_offset().id());
    }


    TEST_F(SeekTest, CopyConstruction)
    {
        Seek ee1(test_id, 1234);
        Seek ee2(ee1);

        EXPECT_EQ(test_id, ee2.element_id());
        EXPECT_EQ(1234, ee2.segment_offset());
    }


    TEST_F(SeekTest, Assignment)
    {
        Seek ee1(test_id, 1234);
        Seek ee2(ids::Null, 0);

        ee2 = ee1;
        EXPECT_EQ(test_id, ee2.element_id());
        EXPECT_EQ(1234, ee2.segment_offset());
    }


    TEST_F(SeekTest, Swap)
    {
        Seek ee1(test_id, 1234);
        Seek ee2(ids::Null, 0);

        swap(ee1, ee2);
        EXPECT_EQ(ids::Null, ee1.element_id());
        EXPECT_EQ(0, ee1.segment_offset());
        EXPECT_EQ(test_id, ee2.element_id());
        EXPECT_EQ(1234, ee2.segment_offset());
    }


    TEST_F(SeekTest, Equal)
    {
        Seek ee1(test_id, 1234);
        Seek ee2(ids::Null, 0);

        EXPECT_FALSE(ee2 == ee1);
        ee2 = ee1;
        EXPECT_TRUE(ee2 == ee1);
    }


    TEST_F(SeekTest, ElementID)
    {
        Seek ee(test_id, 1234);

        EXPECT_EQ(test_id, ee.element_id());
        ee.element_id(ids::EBML);
        EXPECT_EQ(ids::EBML, ee.element_id());
        ee.element_id(test_id_bin_el);
        EXPECT_EQ(test_id, ee.element_id());
    }


    TEST_F(SeekTest, SegmentOffset)
    {
        Seek ee(test_id, 1234);

        EXPECT_EQ(1234, ee.segment_offset());
        ee.segment_offset(5678);
        EXPECT_EQ(5678, ee.segment_offset());
        ee.segment_offset(UIntElement(ids::Null, 4321));
        EXPECT_EQ(4321, ee.segment_offset());
        EXPECT_EQ(ids::SeekPosition, ee.segment_offset().id());
    }


    ///////////////////////////////////////////////////////////////////////////
    // MasterElement interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST_F(SeekTest, EnableCRC)
    {
        Seek ee(ids::Null, 0);
        EXPECT_FALSE(ee.crc_enabled());
        ee.enable_crc();
        EXPECT_TRUE(ee.crc_enabled());
        ee.disable_crc();
        EXPECT_FALSE(ee.crc_enabled());
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
            std::vector<char> tmp(ebml_int::encode_u(ids::Seek));
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

    TEST_F(SeekTest, ID)
    {
        Seek ee(ids::Null, 0);
        EXPECT_EQ(ids::Seek, ee.id());
    }

    TEST_F(SeekTest, Offset)
    {
        Seek ee(test_id, 1234);
        EXPECT_EQ(0, ee.offset());

        std::stringstream ss;
        std::string input_val;

        // Set when read test
        IntElement ie(ids::Null, 42);
        write(ie, ss);
        std::vector<ElPtr> children;
        children.push_back(ElPtr(new BinaryElement(ids::SeekID, test_id_bin)));
        children.push_back(ElPtr(new UIntElement(ids::SeekPosition, 1234)));
        fill_buffer(input_val, children, true, true, true);
        ss << input_val;
        ss.seekg(ie.stored_size() + ids::size(ids::Seek));
        ee.read(ss);
        EXPECT_EQ(ie.stored_size(), ee.offset());

        // Set when written test
        write(ee, ss);
        EXPECT_EQ(ie.stored_size() + ee.stored_size(), ee.offset());
    }

    TEST_F(SeekTest, StoredSize)
    {
        Seek ee(test_id, 1234);

        std::vector<ElPtr> children;
        children.push_back(ElPtr(new BinaryElement(ids::SeekID, test_id_bin)));
        children.push_back(ElPtr(new UIntElement(ids::SeekPosition, 1234)));

        std::streamsize body_size(children[0]->stored_size() +
                children[1]->stored_size());
        EXPECT_EQ(ids::size(ids::Seek) +
                vint::size(body_size) + body_size, ee.stored_size());

        // Size with CRC enabled
        ee.enable_crc();
        body_size += 6;
        EXPECT_EQ(ids::size(ids::Seek) +
                vint::size(body_size) + body_size, ee.stored_size());
        ee.disable_crc();
    }

    TEST_F(SeekTest, Read)
    {
        std::stringstream input;
        std::vector<ElPtr> children;
        children.push_back(ElPtr(new BinaryElement(ids::SeekID, test_id_bin)));
        children.push_back(ElPtr(new UIntElement(ids::SeekPosition, 1234)));

        Seek ee(ids::Null, 0);

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
        EXPECT_EQ(test_id, ee.element_id());
        EXPECT_EQ(1234, ee.segment_offset());

        // Zero-length body should give an exception for missing children
        input.str(std::string());
        vint::write(0, input);
        EXPECT_THROW(read(ee, input), MissingChild);
        // Post-condition test: error should leave element as before
        EXPECT_EQ(test_id, ee.element_id());
        EXPECT_EQ(1234, ee.segment_offset());

        // Body size value wrong (too small)
        input.str(std::string());
        vint::write(2, input);
        write(*children[0], input);
        write(*children[1], input);
        EXPECT_THROW(ee.read(input), ReadError);
        // Post-condition test: error should leave element as before
        EXPECT_EQ(test_id, ee.element_id());
        EXPECT_EQ(1234, ee.segment_offset());

        // Invalid child
        input.str(std::string());
        UIntElement ue(ids::EBML, 0xFFFF);
        vint::write(ue.stored_size(), input);
        write(ue, input);
        EXPECT_THROW(ee.read(input), InvalidChildID);
        // Post-condition test: error should leave element as before
        EXPECT_EQ(test_id, ee.element_id());
        EXPECT_EQ(1234, ee.segment_offset());

        // Missing child
        input.str(std::string());
        vint::write(children[0]->stored_size(), input);
        write(*children[0], input);
        EXPECT_THROW(ee.read(input), MissingChild);
    }

    TEST_F(SeekTest, ReadWithCRC)
    {
        std::vector<ElPtr> children;
        children.push_back(ElPtr(new BinaryElement(ids::SeekID, test_id_bin)));
        children.push_back(ElPtr(new UIntElement(ids::SeekPosition, 1234)));

        std::string buffer;
        std::streamsize read_size(0);
        read_size = fill_buffer(buffer, children, false, true, true, true);

        Seek ee(ids::Null, 0);
        ee.disable_crc();
        std::stringstream input(buffer);
        EXPECT_EQ(read_size, read(ee, input));

        // Bad CRC
        std::string().swap(buffer);
        read_size += fill_buffer(buffer, children, false, true, true, true);
        // Corrupt the CRC
        buffer[3] = 0x00;
        buffer[4] = 0x00;
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

    TEST_F(SeekTest, StartWrite)
    {
        std::stringstream output;
        std::string expected;
        Seek ee(test_id, 1234);

        std::vector<ElPtr> children;
        children.push_back(ElPtr(new BinaryElement(ids::SeekID, test_id_bin)));
        children.push_back(ElPtr(new UIntElement(ids::SeekPosition, 1234)));
        fill_buffer(expected, children, true, true, true);

        std::streamsize body_size(children_size(children));
        EXPECT_EQ(ids::size(ids::Seek) + vint::size(body_size) + body_size,
                ee.start_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Seek) + vint::size(body_size) + body_size,
                output.tellp());
    }

    TEST_F(SeekTest, FinishWrite)
    {
        std::stringstream output;
        std::string expected;
        Seek ee(test_id, 1234);

        // Test for exception and no change to output when finishing without
        // starting
        EXPECT_THROW(ee.finish_write(output), NotWriting);
        EXPECT_EQ(0, output.tellp());

        std::vector<ElPtr> children;
        children.push_back(ElPtr(new BinaryElement(ids::SeekID, test_id_bin)));
        children.push_back(ElPtr(new UIntElement(ids::SeekPosition, 1234)));
        fill_buffer(expected, children, true, true, true);

        std::streamsize body_size(children_size(children));
        ee.start_write(output);
        EXPECT_EQ(ids::size(ids::Seek) + vint::size(body_size) + body_size,
                ee.finish_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Seek) + vint::size(body_size) + body_size,
                output.tellp());

        // Test for double-finish raising an exception
        EXPECT_THROW(ee.finish_write(output), NotWriting);
    }

    TEST_F(SeekTest, Write)
    {
        std::stringstream output;
        std::string expected;
        Seek ee(test_id, 1234);

        std::vector<ElPtr> children;
        children.push_back(ElPtr(new BinaryElement(ids::SeekID, test_id_bin)));
        children.push_back(ElPtr(new UIntElement(ids::SeekPosition, 1234)));
        fill_buffer(expected, children, true, true, true);

        std::streamsize body_size(children_size(children));
        EXPECT_EQ(ids::size(ids::Seek) + vint::size(body_size) + body_size,
                write(ee, output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Seek) + vint::size(body_size) + body_size,
                output.tellp());
    }

    TEST_F(SeekTest, WriteWithCRC)
    {
        std::stringstream output;
        std::string expected;
        Seek ee(test_id, 1234);
        ee.enable_crc();

        std::vector<ElPtr> children;
        children.push_back(ElPtr(new BinaryElement(ids::SeekID, test_id_bin)));
        children.push_back(ElPtr(new UIntElement(ids::SeekPosition, 1234)));
        fill_buffer(expected, children, true, true, true, true);

        std::streamsize body_size(children_size(children) + 6);
        EXPECT_EQ(ids::size(ids::Seek) + vint::size(body_size) + body_size,
                write(ee, output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Seek) + vint::size(body_size) + body_size,
                output.tellp());
    }

    TEST_F(SeekTest, RoundTrip)
    {
        std::stringstream io;
        Seek ee1(test_id, 1234);

        std::streamsize written_bytes = write(ee1, io);
        // Skip the ID
        io.seekg(ids::size(ids::Seek), std::ios::beg);
        Seek ee2(ids::Null, 0);
        std::streamsize read_bytes = read(ee2, io);

        EXPECT_EQ(written_bytes - ids::size(ids::Seek), read_bytes);
        EXPECT_EQ(test_id, ee2.element_id());
        EXPECT_EQ(1234, ee2.segment_offset());
    }
}; // namespace test_ebml_header

