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
#include <celduin/celduin_config.h>
#include <celduin/ebml_header.h>
#include <celduin/element.h>
#include <celduin/exceptions.h>
#include <celduin/ids.h>
#include <celduin/integer_elements.h>
#include <celduin/string_element.h>
#include <celduin/vint.h>

#include "test_utilities.h"
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>

using namespace celduin;

namespace test_ebml_header
{
    ///////////////////////////////////////////////////////////////////////////
    // EBMLHeader interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST(EBMLHeader, Construction)
    {
        EXPECT_EQ(ids::EBML, EBMLHeader().id());
        EXPECT_EQ("celduin", EBMLHeader().doc_type());
        EXPECT_EQ("doctype", EBMLHeader("doctype").doc_type());

        EBMLHeader ee;
        EXPECT_EQ(CelduinEBMLVersion, ee.version());
        EXPECT_EQ(CelduinEBMLVersion, ee.read_version());
        EXPECT_EQ(4, ee.max_id_length());
        EXPECT_EQ(8, ee.max_size_length());
        EXPECT_EQ("celduin", ee.doc_type());
        EXPECT_EQ(CelduinVersionMajor, ee.doc_version());
        EXPECT_EQ(CelduinVersionMajor, ee.doc_read_version());
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
        EXPECT_EQ(CelduinEBMLVersion, ee2.version());
        EXPECT_EQ(CelduinEBMLVersion, ee2.read_version());
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
        EXPECT_EQ(CelduinEBMLVersion, ee2.version());
        EXPECT_EQ(CelduinEBMLVersion, ee2.read_version());
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
        swap(ee1, ee2);

        EXPECT_EQ(CelduinEBMLVersion, ee1.version());
        EXPECT_EQ(CelduinEBMLVersion, ee1.read_version());
        EXPECT_EQ(4, ee1.max_id_length());
        EXPECT_EQ(8, ee1.max_size_length());
        EXPECT_EQ("celduin", ee1.doc_type());
        EXPECT_EQ(CelduinVersionMajor, ee1.doc_version());
        EXPECT_EQ(CelduinVersionMajor, ee1.doc_read_version());
        EXPECT_FALSE(ee1.crc_enabled());

        EXPECT_EQ(CelduinEBMLVersion, ee2.version());
        EXPECT_EQ(CelduinEBMLVersion, ee2.read_version());
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


    ///////////////////////////////////////////////////////////////////////////
    // MasterElement interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST(EBMLHeader, EnableCRC)
    {
        EBMLHeader ee;
        EXPECT_FALSE(ee.crc_enabled());
        ee.enable_crc();
        EXPECT_TRUE(ee.crc_enabled());
        ee.disable_crc();
        EXPECT_FALSE(ee.crc_enabled());
    }


    ///////////////////////////////////////////////////////////////////////////
    // Element interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST(EBMLHeader, ID)
    {
        EBMLHeader ee;
        EXPECT_EQ(ids::EBML, ee.id());
    }

    TEST(EBMLHeader, Offset)
    {
    }

    TEST(EBMLHeader, StoredSize)
    {
        EBMLHeader ee("doctype");

        typedef boost::shared_ptr<Element> ElPtr;
        std::vector<ElPtr> children;

        // Size with everything defaults
        std::streamsize body_size(4+4+4+4+10+3+3);
        EXPECT_EQ(ids::size(ids::EBML) +
                vint::size(body_size) + body_size, ee.stored_size());

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
        typedef boost::shared_ptr<Element> ElPtr;
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

        // Body size value wrong (too small)
        input.str(std::string());
        vint::write(2, input);
        write(*children[0], input);
        write(*children[3], input);
        EXPECT_THROW(ee.read(input), BadBodySize);
        // Invalid child
        input.str(std::string());
        UIntElement ue(ids::EBML, 0xFFFF);
        vint::write(ue.stored_size(), input);
        write(ue, input);
        EXPECT_THROW(ee.read(input), InvalidChildID);
    }

    TEST(EBMLHeader, StartWrite)
    {
    }

    TEST(EBMLHeader, FinishWrite)
    {
    }

    TEST(EBMLHeader, Write)
    {
    }
}; // namespace test_ebml_header

