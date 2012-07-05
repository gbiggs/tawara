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

#include <celduin/ebml_element.h>

#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <gtest/gtest.h>
#include <celduin/el_ids.h>
#include <celduin/exceptions.h>
#include <celduin/celduin_config.h>
#include <celduin/uint_element.h>
#include <celduin/vint.h>

#include "test_consts.h"
#include "test_utils.h"


TEST(EBMLElement, Create)
{
    celduin::EBMLElement e("Blag");
    EXPECT_EQ(celduin::ids::EBML, e.id());
    EXPECT_EQ(1, e.version());
    EXPECT_EQ(1, e.read_version());
    EXPECT_EQ(4, e.max_id_length());
    EXPECT_EQ(8, e.max_size_length());
    EXPECT_EQ("Blag", e.doc_type());
    EXPECT_EQ(0, e.doc_version());
    EXPECT_EQ(0, e.doc_read_version());
}


TEST(EBMLElement, MaxIDLength)
{
    celduin::EBMLElement e("");
    EXPECT_EQ(4, e.max_id_length());
    e.max_id_length(8);
    EXPECT_EQ(8, e.max_id_length());
}


TEST(EBMLElement, MaxSizeLength)
{
    celduin::EBMLElement e("");
    EXPECT_EQ(8, e.max_size_length());
    e.max_size_length(4);
    EXPECT_EQ(4, e.max_size_length());
}


TEST(EBMLElement, DocType)
{
    celduin::EBMLElement e("");
    EXPECT_EQ("", e.doc_type());
    e.doc_type("Celduin");
    EXPECT_EQ("Celduin", e.doc_type());
}


TEST(EBMLElement, DocVersion)
{
    celduin::EBMLElement e("");
    EXPECT_EQ(0, e.doc_version());
    e.doc_version(2);
    EXPECT_EQ(2, e.doc_version());
}


TEST(EBMLElement, DocReadVersion)
{
    celduin::EBMLElement e("");
    EXPECT_EQ(0, e.doc_read_version());
    e.doc_read_version(2);
    EXPECT_EQ(2, e.doc_read_version());
}


TEST(EBMLElement, Write)
{
    std::ostringstream output;
    std::stringstream expected;
    typedef boost::shared_ptr<celduin::Element> ElPtr;
    std::vector<ElPtr> children;

    // Writing with everything defaults should give a full-length body for the
    // EBML header because values get written even if they are default
    children.push_back(ElPtr(new celduin::UIntElement(celduin::ids::EBMLVersion,
                    1)));
    children.push_back(ElPtr(new celduin::UIntElement(celduin::ids::EBMLReadVersion,
                    1)));
    children.push_back(ElPtr(new celduin::UIntElement(celduin::ids::EBMLMaxIDLength,
                    4)));
    children.push_back(ElPtr(new
                celduin::UIntElement(celduin::ids::EBMLMaxSizeLength, 8)));
    children.push_back(ElPtr(new celduin::StringElement(celduin::ids::DocType,
                    "celduin")));
    children.push_back(ElPtr(new celduin::UIntElement(celduin::ids::DocTypeVersion,
                    0)));
    children.push_back(ElPtr(new
                celduin::UIntElement(celduin::ids::DocTypeReadVersion, 0)));
    celduin::EBMLElement e;
    std::streamsize expected_size(0);
    BOOST_FOREACH(ElPtr el, children)
    {
        expected_size += el->size();
    }
    celduin::ids::write(celduin::ids::EBML, expected);
    celduin::vint::write(expected_size, expected);
    BOOST_FOREACH(ElPtr el, children)
    {
        el->write(expected);
    }

    EXPECT_EQ(celduin::ids::size(celduin::ids::EBML) +
            celduin::vint::size(expected_size) + expected_size,
            e.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());

    // Writing without defaults (note that EBMLVersion and EBMLReadVersion are
    // set by the library and so will *always* be default in this test).
    output.str(std::string());
    expected.str(std::string());
    children.clear();
    children.push_back(ElPtr(new celduin::UIntElement(celduin::ids::EBMLVersion,
                    1)));
    children.push_back(ElPtr(new celduin::UIntElement(celduin::ids::EBMLReadVersion,
                    1)));
    children.push_back(ElPtr(new celduin::UIntElement(celduin::ids::EBMLMaxIDLength,
                    5)));
    e.max_id_length(5);
    children.push_back(ElPtr(new
                celduin::UIntElement(celduin::ids::EBMLMaxSizeLength, 7)));
    e.max_size_length(7);
    children.push_back(ElPtr(new celduin::StringElement(celduin::ids::DocType,
                    "blag")));
    e.doc_type("blag");
    children.push_back(ElPtr(new celduin::UIntElement(celduin::ids::DocTypeVersion,
                    2)));
    e.doc_version(2);
    children.push_back(ElPtr(new
                celduin::UIntElement(celduin::ids::DocTypeReadVersion, 2)));
    e.doc_read_version(2);

    expected_size = 0;
    BOOST_FOREACH(ElPtr el, children)
    {
        expected_size += el->size();
    }
    celduin::ids::write(celduin::ids::EBML, expected);
    celduin::vint::write(expected_size, expected);
    BOOST_FOREACH(ElPtr el, children)
    {
        el->write(expected);
    }
    EXPECT_EQ(celduin::ids::size(celduin::ids::EBML) +
            celduin::vint::size(expected_size) + expected_size,
            e.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());
}


TEST(EBMLElement, Read)
{
    std::stringstream input;
    typedef boost::shared_ptr<celduin::Element> ElPtr;
    std::vector<ElPtr> children;

    children.push_back(ElPtr(new celduin::UIntElement(celduin::ids::EBMLVersion,
                    2)));
    children.push_back(ElPtr(new
                celduin::UIntElement(celduin::ids::DocTypeReadVersion, 2)));
    children.push_back(ElPtr(new celduin::UIntElement(celduin::ids::EBMLMaxIDLength,
                    5)));
    children.push_back(ElPtr(new celduin::UIntElement(celduin::ids::EBMLReadVersion,
                    2)));
    children.push_back(ElPtr(new
                celduin::UIntElement(celduin::ids::EBMLMaxSizeLength, 7)));
    children.push_back(ElPtr(new celduin::UIntElement(celduin::ids::DocTypeVersion,
                    2)));
    children.push_back(ElPtr(new celduin::StringElement(celduin::ids::DocType,
                    "blag")));

    celduin::EBMLElement e("");

    std::streamsize body_size(0);
    BOOST_FOREACH(ElPtr el, children)
    {
        body_size += el->size();
    }
    celduin::vint::write(body_size, input);
    BOOST_FOREACH(ElPtr el, children)
    {
        el->write(input);
    }

    EXPECT_EQ(celduin::vint::size(body_size) + body_size,
            e.read(input));
    EXPECT_EQ(2, e.version());
    EXPECT_EQ(2, e.read_version());
    EXPECT_EQ(5, e.max_id_length());
    EXPECT_EQ(7, e.max_size_length());
    EXPECT_EQ("blag", e.doc_type());
    EXPECT_EQ(2, e.doc_version());
    EXPECT_EQ(2, e.doc_read_version());

    // Zero-length body (all values should become defaults)
    input.str(std::string());
    celduin::vint::write(0, input);
    EXPECT_EQ(1, e.read(input));
    EXPECT_EQ(1, e.version());
    EXPECT_EQ(1, e.read_version());
    EXPECT_EQ(4, e.max_id_length());
    EXPECT_EQ(8, e.max_size_length());
    EXPECT_EQ(celduin::CelduinDocType, e.doc_type());
    EXPECT_EQ(0, e.doc_version());
    EXPECT_EQ(0, e.doc_read_version());

    // Body size value wrong (too small)
    input.str(std::string());
    celduin::vint::write(2, input);
    children[0]->write(input);
    children[3]->write(input);
    EXPECT_THROW(e.read(input), celduin::BadBodySize);
    // Invalid child
    input.str(std::string());
    celduin::UIntElement ue(celduin::ids::EBML, 0xFFFF);
    celduin::vint::write(ue.size(), input);
    ue.write(input);
    EXPECT_THROW(e.read(input), celduin::InvalidChildID);
}


TEST(EBMLElement, Size)
{
    typedef boost::shared_ptr<celduin::Element> ElPtr;
    std::vector<ElPtr> children;

    // Size with everything defaults
    celduin::EBMLElement e1;
    EXPECT_EQ(celduin::ids::size(celduin::ids::EBML) +
            celduin::vint::size(29) + 29, e1.size());

    // Size with non-defaults. Note that EBMLVersion and EBMLReadVersion can
    // never be anything other than the default in this test.
    children.push_back(ElPtr(new celduin::UIntElement(celduin::ids::EBMLVersion,
                    1)));
    children.push_back(ElPtr(new celduin::UIntElement(celduin::ids::EBMLReadVersion,
                    1)));
    children.push_back(ElPtr(new celduin::UIntElement(celduin::ids::EBMLMaxIDLength,
                    5)));
    children.push_back(ElPtr(new
                celduin::UIntElement(celduin::ids::EBMLMaxSizeLength, 7)));
    children.push_back(ElPtr(new celduin::StringElement(celduin::ids::DocType,
                    "blag")));
    children.push_back(ElPtr(new celduin::UIntElement(celduin::ids::DocTypeVersion,
                    2)));
    children.push_back(ElPtr(new
                celduin::UIntElement(celduin::ids::DocTypeReadVersion, 2)));

    std::streamsize body_size(0);
    BOOST_FOREACH(ElPtr el, children)
    {
        body_size += el->size();
    }

    celduin::EBMLElement e2("blag");
    e2.max_id_length(5);
    e2.max_size_length(7);
    e2.doc_version(2);
    e2.doc_read_version(2);
    EXPECT_EQ(celduin::ids::size(celduin::ids::EBML) +
            celduin::vint::size(body_size) + body_size, e2.size());
}

