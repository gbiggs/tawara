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
#include <jonen/jonen_config.h>
#include <jonen/ebml_header.h>
#include <jonen/element.h>
#include <jonen/exceptions.h>
#include <jonen/ids.h>
#include <jonen/integer_elements.h>
#include <jonen/string_element.h>
#include <jonen/vint.h>

#include "test_utilities.h"
#include <boost/crc.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>

using namespace jonen;

namespace test_ebml_header
{
    ///////////////////////////////////////////////////////////////////////////
    // EBMLHeader interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST(EBMLHeader, Construction)
    {
        EXPECT_EQ(ids::EBML, EBMLHeader().id());
        EXPECT_EQ("jonen", EBMLHeader().doc_type());
        EXPECT_EQ("doctype", EBMLHeader("doctype").doc_type());

        EBMLHeader ee;
        EXPECT_EQ(JonenEBMLVersion, ee.version());
        EXPECT_EQ(JonenEBMLVersion, ee.read_version());
        EXPECT_EQ(4, ee.max_id_length());
        EXPECT_EQ(8, ee.max_size_length());
        EXPECT_EQ("jonen", ee.doc_type());
        EXPECT_EQ(JonenVersionMajor, ee.doc_version());
        EXPECT_EQ(JonenVersionMajor, ee.doc_read_version());
        EXPECT_FALSE(ee.crc_enabled());
    }


    TEST(EBMLHeader, CopyConstruction)
    {
        EBMLHeader ee1("doctype");
        ee1.max_id_length(6);
        ee1.max_size_length(10);
        ee1.doc_version(2);
        ee1.doc_read_version(3);
        ee1.enable_crc();

        EBMLHeader ee2(ee1);
        EXPECT_TRUE(ee1 == ee2);
        EXPECT_EQ(JonenEBMLVersion, ee2.version());
        EXPECT_EQ(JonenEBMLVersion, ee2.read_version());
        EXPECT_EQ(6, ee2.max_id_length());
        EXPECT_EQ(10, ee2.max_size_length());
        EXPECT_EQ("doctype", ee2.doc_type());
        EXPECT_EQ(2, ee2.doc_version());
        EXPECT_EQ(3, ee2.doc_read_version());
        EXPECT_TRUE(ee2.crc_enabled());
    }


    TEST(EBMLHeader, Assignment)
    {
        EBMLHeader ee1("doctype");
        ee1.max_id_length(6);
        ee1.max_size_length(10);
        ee1.doc_version(2);
        ee1.doc_read_version(3);
        ee1.enable_crc();

        EBMLHeader ee2;
        ee2 = ee1;
        EXPECT_TRUE(ee1 == ee2);
        EXPECT_EQ(JonenEBMLVersion, ee2.version());
        EXPECT_EQ(JonenEBMLVersion, ee2.read_version());
        EXPECT_EQ(6, ee2.max_id_length());
        EXPECT_EQ(10, ee2.max_size_length());
        EXPECT_EQ("doctype", ee2.doc_type());
        EXPECT_EQ(2, ee2.doc_version());
        EXPECT_EQ(3, ee2.doc_read_version());
        EXPECT_TRUE(ee2.crc_enabled());
    }


    TEST(EBMLHeader, Swap)
    {
        EBMLHeader ee1("doctype");
        ee1.max_id_length(6);
        ee1.max_size_length(10);
        ee1.doc_version(2);
        ee1.doc_read_version(3);
        ee1.enable_crc();

        EBMLHeader ee2;
        ee2.disable_crc();
        swap(ee1, ee2);

        EXPECT_EQ(JonenEBMLVersion, ee1.version());
        EXPECT_EQ(JonenEBMLVersion, ee1.read_version());
        EXPECT_EQ(4, ee1.max_id_length());
        EXPECT_EQ(8, ee1.max_size_length());
        EXPECT_EQ("jonen", ee1.doc_type());
        EXPECT_EQ(JonenVersionMajor, ee1.doc_version());
        EXPECT_EQ(JonenVersionMajor, ee1.doc_read_version());
        EXPECT_FALSE(ee1.crc_enabled());

        EXPECT_EQ(JonenEBMLVersion, ee2.version());
        EXPECT_EQ(JonenEBMLVersion, ee2.read_version());
        EXPECT_EQ(6, ee2.max_id_length());
        EXPECT_EQ(10, ee2.max_size_length());
        EXPECT_EQ("doctype", ee2.doc_type());
        EXPECT_EQ(2, ee2.doc_version());
        EXPECT_EQ(3, ee2.doc_read_version());
        EXPECT_TRUE(ee2.crc_enabled());
    }


    TEST(EBMLHeader, Equal)
    {
        EBMLHeader ee1("doctype");
        ee1.max_id_length(6);
        ee1.max_size_length(10);
        ee1.doc_version(2);
        ee1.doc_read_version(3);
        ee1.enable_crc();

        EBMLHeader ee2("doctype");
        ee2.max_id_length(6);
        ee2.max_size_length(10);
        ee2.doc_version(2);
        ee2.doc_read_version(3);
        ee2.enable_crc();

        EXPECT_TRUE(ee1 == ee2);

        EBMLHeader ee3("doctype");
        EXPECT_FALSE(ee1 == ee3);

    }


    TEST(EBMLHeader, Version)
    {
        EBMLHeader ee;
        EXPECT_EQ(JonenEBMLVersion, ee.version());
    }


    TEST(EBMLHeader, ReadVersion)
    {
        EBMLHeader ee;
        EXPECT_EQ(JonenEBMLVersion, ee.read_version());
    }


    TEST(EBMLHeader, MaxIDLength)
    {
        EBMLHeader ee;
        EXPECT_EQ(4, ee.max_id_length());
        ee.max_id_length(8);
        EXPECT_EQ(8, ee.max_id_length());
    }


    TEST(EBMLHeader, MaxSizeLength)
    {
        EBMLHeader ee;
        EXPECT_EQ(8, ee.max_size_length());
        ee.max_size_length(4);
        EXPECT_EQ(4, ee.max_size_length());
    }


    TEST(EBMLHeader, DocType)
    {
        EBMLHeader ee;
        EXPECT_EQ("jonen", ee.doc_type());
        ee.doc_type("tide");
        EXPECT_EQ("tide", ee.doc_type());
    }


    TEST(EBMLHeader, DocVersion)
    {
        EBMLHeader ee;
        EXPECT_EQ(0, ee.doc_version());
        ee.doc_version(2);
        EXPECT_EQ(2, ee.doc_version());
    }


    TEST(EBMLHeader, DocReadVersion)
    {
        EBMLHeader ee;
        EXPECT_EQ(0, ee.doc_read_version());
        ee.doc_read_version(2);
        EXPECT_EQ(2, ee.doc_read_version());
    }


    ///////////////////////////////////////////////////////////////////////////
    // MasterElement interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST(EBMLHeader, EnableCRC)
    {
        EBMLHeader ee;
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
            std::vector<char> tmp(ebml_int::encode_u(ids::EBML));
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

    std::streamsize write_except(std::stringstream& ss,
            std::vector<ElPtr> const& children, int except)
    {
        std::streamsize body_size(0);
        for (int ii(0); ii < children.size(); ++ii)
        {
            if (ii == except)
            {
                continue;
            }
            body_size += write(*children[ii], ss);
        }
        return body_size;
    }

    TEST(EBMLHeader, ID)
    {
        EBMLHeader ee;
        EXPECT_EQ(ids::EBML, ee.id());
    }

    TEST(EBMLHeader, Offset)
    {
        EBMLHeader ee;
        EXPECT_EQ(0, ee.offset());

        std::stringstream ss;
        std::string input_val;

        // Set when read test
        IntElement ie(ids::Null, 42);
        write(ie, ss);
        std::vector<ElPtr> children;
        children.push_back(ElPtr(new UIntElement(ids::EBMLVersion,
                        JonenEBMLVersion)));
        children.push_back(ElPtr(new UIntElement(ids::EBMLReadVersion,
                        JonenEBMLVersion)));
        children.push_back(ElPtr(new UIntElement(ids::EBMLMaxIDLength,
                        5)));
        children.push_back(ElPtr(new UIntElement(ids::EBMLMaxSizeLength,
                        7)));
        children.push_back(ElPtr(new StringElement(ids::DocType, "blag")));
        children.push_back(ElPtr(new UIntElement(ids::DocTypeVersion, 2)));
        children.push_back(ElPtr(new UIntElement(ids::DocTypeReadVersion,
                        2)));
        fill_buffer(input_val, children, true, true, true);
        ss << input_val;
        ss.seekg(ie.stored_size() + ids::size(ids::EBML));
        ee.read(ss);
        EXPECT_EQ(ie.stored_size(), ee.offset());

        // Set when written test
        write(ee, ss);
        EXPECT_EQ(ie.stored_size() + ee.stored_size(), ee.offset());
    }

    TEST(EBMLHeader, StoredSize)
    {
        EBMLHeader ee("doctype");
        ee.disable_crc();

        std::vector<ElPtr> children;

        // Size with everything defaults
        std::streamsize body_size(4+4+4+4+10+3+3);
        EXPECT_EQ(ids::size(ids::EBML) +
                vint::size(body_size) + body_size, ee.stored_size());

        // Size with CRC enabled
        ee.enable_crc();
        body_size = 4+4+4+4+10+3+3 +6;
        EXPECT_EQ(ids::size(ids::EBML) +
                vint::size(body_size) + body_size, ee.stored_size());
        ee.disable_crc();

        // Size with non-defaults. Note that EBMLVersion and EBMLReadVersion can
        // never be anything other than the default in this test.
        children.push_back(ElPtr(new UIntElement(ids::EBMLVersion, 1)));
        children.push_back(ElPtr(new UIntElement(ids::EBMLReadVersion, 1)));
        children.push_back(ElPtr(new UIntElement(ids::EBMLMaxIDLength, 5)));
        children.push_back(ElPtr(new UIntElement(ids::EBMLMaxSizeLength, 7)));
        children.push_back(ElPtr(new StringElement(ids::DocType, "blag")));
        children.push_back(ElPtr(new UIntElement(ids::DocTypeVersion, 2)));
        children.push_back(ElPtr(new UIntElement(ids::DocTypeReadVersion, 3)));

        body_size = 0;
        BOOST_FOREACH(ElPtr el, children)
        {
            body_size += el->stored_size();
        }

        EBMLHeader ee2("blag");
        ee2.disable_crc();
        ee2.max_id_length(5);
        ee2.max_size_length(7);
        ee2.doc_version(2);
        ee2.doc_read_version(3);
        EXPECT_EQ(ids::size(ids::EBML) +
                vint::size(body_size) + body_size, ee2.stored_size());
    }

    TEST(EBMLHeader, Read)
    {
        std::stringstream input;
        std::vector<ElPtr> children;

        children.push_back(ElPtr(new UIntElement(ids::EBMLVersion, 2)));
        children.push_back(ElPtr(new UIntElement(ids::EBMLReadVersion, 2)));
        children.push_back(ElPtr(new UIntElement(ids::EBMLMaxIDLength, 5)));
        children.push_back(ElPtr(new UIntElement(ids::EBMLMaxSizeLength, 7)));
        children.push_back(ElPtr(new StringElement(ids::DocType, "blag")));
        children.push_back(ElPtr(new UIntElement(ids::DocTypeVersion, 2)));
        children.push_back(ElPtr(new UIntElement(ids::DocTypeReadVersion, 2)));

        EBMLHeader ee("");

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
        EXPECT_EQ(2, ee.version());
        EXPECT_EQ(2, ee.read_version());
        EXPECT_EQ(5, ee.max_id_length());
        EXPECT_EQ(7, ee.max_size_length());
        EXPECT_EQ("blag", ee.doc_type());
        EXPECT_EQ(2, ee.doc_version());
        EXPECT_EQ(2, ee.doc_read_version());

        // Zero-length body should give an exception for missing children
        input.str(std::string());
        vint::write(0, input);
        EXPECT_THROW(read(ee, input), MissingChild);
        // Post-condition test: error should leave element as before
        EXPECT_EQ(2, ee.version());
        EXPECT_EQ(2, ee.read_version());
        EXPECT_EQ(5, ee.max_id_length());
        EXPECT_EQ(7, ee.max_size_length());
        EXPECT_EQ("blag", ee.doc_type());
        EXPECT_EQ(2, ee.doc_version());
        EXPECT_EQ(2, ee.doc_read_version());

        // Body size value wrong (too small)
        input.str(std::string());
        vint::write(2, input);
        write(*children[0], input);
        write(*children[3], input);
        EXPECT_THROW(ee.read(input), ReadError);
        // Post-condition test: error should leave element as before
        EXPECT_EQ(2, ee.version());
        EXPECT_EQ(2, ee.read_version());
        EXPECT_EQ(5, ee.max_id_length());
        EXPECT_EQ(7, ee.max_size_length());
        EXPECT_EQ("blag", ee.doc_type());
        EXPECT_EQ(2, ee.doc_version());
        EXPECT_EQ(2, ee.doc_read_version());

        // Invalid child
        input.str(std::string());
        UIntElement ue(ids::EBML, 0xFFFF);
        vint::write(ue.stored_size(), input);
        write(ue, input);
        EXPECT_THROW(ee.read(input), InvalidChildID);
        // Post-condition test: error should leave element as before
        EXPECT_EQ(2, ee.version());
        EXPECT_EQ(2, ee.read_version());
        EXPECT_EQ(5, ee.max_id_length());
        EXPECT_EQ(7, ee.max_size_length());
        EXPECT_EQ("blag", ee.doc_type());
        EXPECT_EQ(2, ee.doc_version());
        EXPECT_EQ(2, ee.doc_read_version());

        // Missing child
        for (int ii(0); ii < children.size(); ++ii)
        {
            input.str(std::string());
            body_size = write_except(input, children, ii);
            input.str(std::string());
            vint::write(body_size, input);
            write_except(input, children, ii);
            EXPECT_THROW(ee.read(input), MissingChild);
            // Post-condition test: error should leave element as before
            EXPECT_EQ(2, ee.version());
            EXPECT_EQ(2, ee.read_version());
            EXPECT_EQ(5, ee.max_id_length());
            EXPECT_EQ(7, ee.max_size_length());
            EXPECT_EQ("blag", ee.doc_type());
            EXPECT_EQ(2, ee.doc_version());
            EXPECT_EQ(2, ee.doc_read_version());
        }
    }

    TEST(EBMLHeader, ReadWithCRC)
    {
        std::vector<ElPtr> children;
        children.push_back(ElPtr(new UIntElement(ids::EBMLVersion, 2)));
        children.push_back(ElPtr(new UIntElement(ids::EBMLReadVersion, 2)));
        children.push_back(ElPtr(new UIntElement(ids::EBMLMaxIDLength, 5)));
        children.push_back(ElPtr(new UIntElement(ids::EBMLMaxSizeLength, 7)));
        children.push_back(ElPtr(new StringElement(ids::DocType, "blag")));
        children.push_back(ElPtr(new UIntElement(ids::DocTypeVersion, 2)));
        children.push_back(ElPtr(new UIntElement(ids::DocTypeReadVersion, 2)));

        std::string buffer;
        std::streamsize read_size(0);
        read_size = fill_buffer(buffer, children, false, true, true, true);

        EBMLHeader ee("");
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

    TEST(EBMLHeader, StartWrite)
    {
        std::stringstream output;
        std::string expected;
        EBMLHeader ee;
        ee.disable_crc();

        // Writing with defaults should give a full-length body for the EBML
        // header because values get written even if they are the default.
        std::vector<ElPtr> children;
        children.push_back(ElPtr(new UIntElement(ids::EBMLVersion,
                        JonenEBMLVersion)));
        children.push_back(ElPtr(new UIntElement(ids::EBMLReadVersion,
                        JonenEBMLVersion)));
        children.push_back(ElPtr(new UIntElement(ids::EBMLMaxIDLength,
                        4)));
        children.push_back(ElPtr(new UIntElement(ids::EBMLMaxSizeLength,
                        8)));
        children.push_back(ElPtr(new StringElement(ids::DocType, "jonen")));
        children.push_back(ElPtr(new UIntElement(ids::DocTypeVersion,
                        JonenVersionMajor)));
        children.push_back(ElPtr(new UIntElement(ids::DocTypeReadVersion,
                        JonenVersionMajor)));
        fill_buffer(expected, children, true, true, true);

        std::streamsize body_size(children_size(children));
        EXPECT_EQ(ids::size(ids::EBML) + vint::size(body_size) + body_size,
                ee.start_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::EBML) + vint::size(body_size) + body_size,
                output.tellp());

        // Writing without defaults (note that EBMLVersion and EBMLReadVersion
        // are set by the library and so will *always* be default in this
        // test).
        output.str(std::string());
        std::string().swap(expected);
        children.clear();
        children.push_back(ElPtr(new UIntElement(ids::EBMLVersion,
                        JonenEBMLVersion)));
        children.push_back(ElPtr(new UIntElement(ids::EBMLReadVersion,
                        JonenEBMLVersion)));
        children.push_back(ElPtr(new UIntElement(ids::EBMLMaxIDLength,
                        5)));
        ee.max_id_length(5);
        children.push_back(ElPtr(new UIntElement(ids::EBMLMaxSizeLength,
                        7)));
        ee.max_size_length(7);
        children.push_back(ElPtr(new StringElement(ids::DocType, "blag")));
        ee.doc_type("blag");
        children.push_back(ElPtr(new UIntElement(ids::DocTypeVersion, 2)));
        ee.doc_version(2);
        children.push_back(ElPtr(new UIntElement(ids::DocTypeReadVersion,
                        2)));
        ee.doc_read_version(2);
        fill_buffer(expected, children, true, true, true);

        body_size = children_size(children);
        EXPECT_EQ(ids::size(ids::EBML) + vint::size(body_size) + body_size,
                ee.start_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::EBML) + vint::size(body_size) + body_size,
                output.tellp());
    }

    TEST(EBMLHeader, FinishWrite)
    {
        std::stringstream output;
        std::string expected;
        EBMLHeader ee;
        ee.disable_crc();

        // Test for exception and no change to output when finishing without
        // starting
        EXPECT_THROW(ee.finish_write(output), NotWriting);
        EXPECT_EQ(0, output.tellp());

        // Writing with defaults should give a full-length body for the EBML
        // header because values get written even if they are the default.
        std::vector<ElPtr> children;
        children.push_back(ElPtr(new UIntElement(ids::EBMLVersion,
                        JonenEBMLVersion)));
        children.push_back(ElPtr(new UIntElement(ids::EBMLReadVersion,
                        JonenEBMLVersion)));
        children.push_back(ElPtr(new UIntElement(ids::EBMLMaxIDLength,
                        4)));
        children.push_back(ElPtr(new UIntElement(ids::EBMLMaxSizeLength,
                        8)));
        children.push_back(ElPtr(new StringElement(ids::DocType, "jonen")));
        children.push_back(ElPtr(new UIntElement(ids::DocTypeVersion,
                        JonenVersionMajor)));
        children.push_back(ElPtr(new UIntElement(ids::DocTypeReadVersion,
                        JonenVersionMajor)));
        fill_buffer(expected, children, true, true, true);

        std::streamsize body_size(children_size(children));
        ee.start_write(output);
        EXPECT_EQ(ids::size(ids::EBML) + vint::size(body_size) + body_size,
                ee.finish_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::EBML) + vint::size(body_size) + body_size,
                output.tellp());

        // Writing without defaults (note that EBMLVersion and EBMLReadVersion
        // are set by the library and so will *always* be default in this
        // test).
        output.str(std::string());
        std::string().swap(expected);
        children.clear();
        children.push_back(ElPtr(new UIntElement(ids::EBMLVersion,
                        JonenEBMLVersion)));
        children.push_back(ElPtr(new UIntElement(ids::EBMLReadVersion,
                        JonenEBMLVersion)));
        children.push_back(ElPtr(new UIntElement(ids::EBMLMaxIDLength,
                        5)));
        ee.max_id_length(5);
        children.push_back(ElPtr(new UIntElement(ids::EBMLMaxSizeLength,
                        7)));
        ee.max_size_length(7);
        children.push_back(ElPtr(new StringElement(ids::DocType, "blag")));
        ee.doc_type("blag");
        children.push_back(ElPtr(new UIntElement(ids::DocTypeVersion, 2)));
        ee.doc_version(2);
        children.push_back(ElPtr(new UIntElement(ids::DocTypeReadVersion,
                        2)));
        ee.doc_read_version(2);
        fill_buffer(expected, children, true, true, true);

        body_size = children_size(children);
        ee.start_write(output);
        EXPECT_EQ(ids::size(ids::EBML) + vint::size(body_size) + body_size,
                ee.finish_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::EBML) + vint::size(body_size) + body_size,
                output.tellp());

        // Test for double-finish raising an exception
        EXPECT_THROW(ee.finish_write(output), NotWriting);
    }

    TEST(EBMLHeader, Write)
    {
        std::stringstream output;
        std::string expected;
        EBMLHeader ee;
        ee.disable_crc();

        // Writing with defaults should give a full-length body for the EBML
        // header because values get written even if they are the default.
        std::vector<ElPtr> children;
        children.push_back(ElPtr(new UIntElement(ids::EBMLVersion,
                        JonenEBMLVersion)));
        children.push_back(ElPtr(new UIntElement(ids::EBMLReadVersion,
                        JonenEBMLVersion)));
        children.push_back(ElPtr(new UIntElement(ids::EBMLMaxIDLength,
                        4)));
        children.push_back(ElPtr(new UIntElement(ids::EBMLMaxSizeLength,
                        8)));
        children.push_back(ElPtr(new StringElement(ids::DocType, "jonen")));
        children.push_back(ElPtr(new UIntElement(ids::DocTypeVersion,
                        JonenVersionMajor)));
        children.push_back(ElPtr(new UIntElement(ids::DocTypeReadVersion,
                        JonenVersionMajor)));
        fill_buffer(expected, children, true, true, true);

        std::streamsize body_size(children_size(children));
        EXPECT_EQ(ids::size(ids::EBML) + vint::size(body_size) + body_size,
                write(ee, output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::EBML) + vint::size(body_size) + body_size,
                output.tellp());

        // Writing without defaults (note that EBMLVersion and EBMLReadVersion
        // are set by the library and so will *always* be default in this
        // test).
        output.str(std::string());
        std::string().swap(expected);
        children.clear();
        children.push_back(ElPtr(new UIntElement(ids::EBMLVersion,
                        JonenEBMLVersion)));
        children.push_back(ElPtr(new UIntElement(ids::EBMLReadVersion,
                        JonenEBMLVersion)));
        children.push_back(ElPtr(new UIntElement(ids::EBMLMaxIDLength,
                        5)));
        ee.max_id_length(5);
        children.push_back(ElPtr(new UIntElement(ids::EBMLMaxSizeLength,
                        7)));
        ee.max_size_length(7);
        children.push_back(ElPtr(new StringElement(ids::DocType, "blag")));
        ee.doc_type("blag");
        children.push_back(ElPtr(new UIntElement(ids::DocTypeVersion, 2)));
        ee.doc_version(2);
        children.push_back(ElPtr(new UIntElement(ids::DocTypeReadVersion,
                        2)));
        ee.doc_read_version(2);
        fill_buffer(expected, children, true, true, true);

        body_size = children_size(children);
        EXPECT_EQ(ids::size(ids::EBML) + vint::size(body_size) + body_size,
                write(ee, output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::EBML) + vint::size(body_size) + body_size,
                output.tellp());
    }

    TEST(EBMLHeader, WriteWithCRC)
    {
        std::stringstream output;
        std::string expected;
        EBMLHeader ee;

        // Writing with defaults should give a full-length body for the EBML
        // header because values get written even if they are the default.
        std::vector<ElPtr> children;
        children.push_back(ElPtr(new UIntElement(ids::EBMLVersion,
                        JonenEBMLVersion)));
        children.push_back(ElPtr(new UIntElement(ids::EBMLReadVersion,
                        JonenEBMLVersion)));
        children.push_back(ElPtr(new UIntElement(ids::EBMLMaxIDLength,
                        4)));
        children.push_back(ElPtr(new UIntElement(ids::EBMLMaxSizeLength,
                        8)));
        children.push_back(ElPtr(new StringElement(ids::DocType, "jonen")));
        children.push_back(ElPtr(new UIntElement(ids::DocTypeVersion,
                        JonenVersionMajor)));
        children.push_back(ElPtr(new UIntElement(ids::DocTypeReadVersion,
                        JonenVersionMajor)));
        fill_buffer(expected, children, true, true, true, true);

        ee.enable_crc();
        std::streamsize body_size(children_size(children) + 6);
        EXPECT_EQ(ids::size(ids::EBML) + vint::size(body_size) + body_size,
                write(ee, output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::EBML) + vint::size(body_size) + body_size,
                output.tellp());
    }

    TEST(EBMLHeader, RoundTrip)
    {
        std::stringstream io;
        EBMLHeader ee1;
        ee1.max_id_length(5);
        ee1.max_size_length(7);
        ee1.doc_type("stuff");
        ee1.doc_version(3);
        ee1.doc_read_version(4);

        std::streamsize written_bytes = write(ee1, io);
        // Skip the ID
        io.seekg(4, std::ios::beg);
        EBMLHeader ee2;
        std::streamsize read_bytes = read(ee2, io);

        EXPECT_EQ(written_bytes - 4, read_bytes);
        EXPECT_EQ(CelduinEBMLVersion, ee2.version());
        EXPECT_EQ(CelduinEBMLVersion, ee2.read_version());
        EXPECT_EQ(5, ee2.max_id_length());
        EXPECT_EQ(7, ee2.max_size_length());
        EXPECT_EQ("stuff", ee2.doc_type());
        EXPECT_EQ(3, ee2.doc_version());
        EXPECT_EQ(4, ee2.doc_read_version());
        EXPECT_TRUE(ee2.crc_enabled());
    }
}; // namespace test_ebml_header

