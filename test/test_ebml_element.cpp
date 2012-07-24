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

#include <jonen/ebml_element.h>

#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <gtest/gtest.h>
#include <jonen/el_ids.h>
#include <jonen/exceptions.h>
#include <jonen/jonen_config.h>
#include <jonen/uint_element.h>
#include <jonen/vint.h>

#include "test_consts.h"
#include "test_utils.h"


TEST(EBMLElement, Create)
{
    jonen::EBMLElement e("Blag");
    EXPECT_EQ(jonen::ids::EBML, e.id());
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
    jonen::EBMLElement e("");
    EXPECT_EQ(4, e.max_id_length());
    e.max_id_length(8);
    EXPECT_EQ(8, e.max_id_length());
}


TEST(EBMLElement, MaxSizeLength)
{
    jonen::EBMLElement e("");
    EXPECT_EQ(8, e.max_size_length());
    e.max_size_length(4);
    EXPECT_EQ(4, e.max_size_length());
}


TEST(EBMLElement, DocType)
{
    jonen::EBMLElement e("");
    EXPECT_EQ("", e.doc_type());
    e.doc_type("Jonen");
    EXPECT_EQ("Jonen", e.doc_type());
}


TEST(EBMLElement, DocVersion)
{
    jonen::EBMLElement e("");
    EXPECT_EQ(0, e.doc_version());
    e.doc_version(2);
    EXPECT_EQ(2, e.doc_version());
}


TEST(EBMLElement, DocReadVersion)
{
    jonen::EBMLElement e("");
    EXPECT_EQ(0, e.doc_read_version());
    e.doc_read_version(2);
    EXPECT_EQ(2, e.doc_read_version());
}


TEST(EBMLElement, Write)
{
    std::ostringstream output;
    std::stringstream expected;
    typedef boost::shared_ptr<jonen::Element> ElPtr;
    std::vector<ElPtr> children;

    // Writing with everything defaults should give a full-length body for the
    // EBML header because values get written even if they are default
    children.push_back(ElPtr(new jonen::UIntElement(jonen::ids::EBMLVersion,
                    1)));
    children.push_back(ElPtr(new jonen::UIntElement(jonen::ids::EBMLReadVersion,
                    1)));
    children.push_back(ElPtr(new jonen::UIntElement(jonen::ids::EBMLMaxIDLength,
                    4)));
    children.push_back(ElPtr(new
                jonen::UIntElement(jonen::ids::EBMLMaxSizeLength, 8)));
    children.push_back(ElPtr(new jonen::StringElement(jonen::ids::DocType,
                    "jonen")));
    children.push_back(ElPtr(new jonen::UIntElement(jonen::ids::DocTypeVersion,
                    0)));
    children.push_back(ElPtr(new
                jonen::UIntElement(jonen::ids::DocTypeReadVersion, 0)));
    jonen::EBMLElement e;
    std::streamsize expected_size(0);
    BOOST_FOREACH(ElPtr el, children)
    {
        expected_size += el->size();
    }
    jonen::ids::write(jonen::ids::EBML, expected);
    jonen::vint::write(expected_size, expected);
    BOOST_FOREACH(ElPtr el, children)
    {
        el->write(expected);
    }

    EXPECT_EQ(jonen::ids::size(jonen::ids::EBML) +
            jonen::vint::size(expected_size) + expected_size,
            e.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());

    // Writing without defaults (note that EBMLVersion and EBMLReadVersion are
    // set by the library and so will *always* be default in this test).
    output.str(std::string());
    expected.str(std::string());
    children.clear();
    children.push_back(ElPtr(new jonen::UIntElement(jonen::ids::EBMLVersion,
                    1)));
    children.push_back(ElPtr(new jonen::UIntElement(jonen::ids::EBMLReadVersion,
                    1)));
    children.push_back(ElPtr(new jonen::UIntElement(jonen::ids::EBMLMaxIDLength,
                    5)));
    e.max_id_length(5);
    children.push_back(ElPtr(new
                jonen::UIntElement(jonen::ids::EBMLMaxSizeLength, 7)));
    e.max_size_length(7);
    children.push_back(ElPtr(new jonen::StringElement(jonen::ids::DocType,
                    "blag")));
    e.doc_type("blag");
    children.push_back(ElPtr(new jonen::UIntElement(jonen::ids::DocTypeVersion,
                    2)));
    e.doc_version(2);
    children.push_back(ElPtr(new
                jonen::UIntElement(jonen::ids::DocTypeReadVersion, 2)));
    e.doc_read_version(2);

    expected_size = 0;
    BOOST_FOREACH(ElPtr el, children)
    {
        expected_size += el->size();
    }
    jonen::ids::write(jonen::ids::EBML, expected);
    jonen::vint::write(expected_size, expected);
    BOOST_FOREACH(ElPtr el, children)
    {
        el->write(expected);
    }
    EXPECT_EQ(jonen::ids::size(jonen::ids::EBML) +
            jonen::vint::size(expected_size) + expected_size,
            e.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());
}


TEST(EBMLElement, Read)
{
    std::stringstream input;
    typedef boost::shared_ptr<jonen::Element> ElPtr;
    std::vector<ElPtr> children;

    children.push_back(ElPtr(new jonen::UIntElement(jonen::ids::EBMLVersion,
                    2)));
    children.push_back(ElPtr(new
                jonen::UIntElement(jonen::ids::DocTypeReadVersion, 2)));
    children.push_back(ElPtr(new jonen::UIntElement(jonen::ids::EBMLMaxIDLength,
                    5)));
    children.push_back(ElPtr(new jonen::UIntElement(jonen::ids::EBMLReadVersion,
                    2)));
    children.push_back(ElPtr(new
                jonen::UIntElement(jonen::ids::EBMLMaxSizeLength, 7)));
    children.push_back(ElPtr(new jonen::UIntElement(jonen::ids::DocTypeVersion,
                    2)));
    children.push_back(ElPtr(new jonen::StringElement(jonen::ids::DocType,
                    "blag")));

    jonen::EBMLElement e("");

    std::streamsize body_size(0);
    BOOST_FOREACH(ElPtr el, children)
    {
        body_size += el->size();
    }
    jonen::vint::write(body_size, input);
    BOOST_FOREACH(ElPtr el, children)
    {
        el->write(input);
    }

    EXPECT_EQ(jonen::vint::size(body_size) + body_size,
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
    jonen::vint::write(0, input);
    EXPECT_EQ(1, e.read(input));
    EXPECT_EQ(1, e.version());
    EXPECT_EQ(1, e.read_version());
    EXPECT_EQ(4, e.max_id_length());
    EXPECT_EQ(8, e.max_size_length());
    EXPECT_EQ(jonen::JonenDocType, e.doc_type());
    EXPECT_EQ(0, e.doc_version());
    EXPECT_EQ(0, e.doc_read_version());

    // Body size value wrong (too small)
    input.str(std::string());
    jonen::vint::write(2, input);
    children[0]->write(input);
    children[3]->write(input);
    EXPECT_THROW(e.read(input), jonen::BadBodySize);
    // Invalid child
    input.str(std::string());
    jonen::UIntElement ue(jonen::ids::EBML, 0xFFFF);
    jonen::vint::write(ue.size(), input);
    ue.write(input);
    EXPECT_THROW(e.read(input), jonen::InvalidChildID);
}


TEST(EBMLElement, Size)
{
    typedef boost::shared_ptr<jonen::Element> ElPtr;
    std::vector<ElPtr> children;

    // Size with everything defaults
    jonen::EBMLElement e1;
    EXPECT_EQ(jonen::ids::size(jonen::ids::EBML) +
            jonen::vint::size(29) + 29, e1.size());

    // Size with non-defaults. Note that EBMLVersion and EBMLReadVersion can
    // never be anything other than the default in this test.
    children.push_back(ElPtr(new jonen::UIntElement(jonen::ids::EBMLVersion,
                    1)));
    children.push_back(ElPtr(new jonen::UIntElement(jonen::ids::EBMLReadVersion,
                    1)));
    children.push_back(ElPtr(new jonen::UIntElement(jonen::ids::EBMLMaxIDLength,
                    5)));
    children.push_back(ElPtr(new
                jonen::UIntElement(jonen::ids::EBMLMaxSizeLength, 7)));
    children.push_back(ElPtr(new jonen::StringElement(jonen::ids::DocType,
                    "blag")));
    children.push_back(ElPtr(new jonen::UIntElement(jonen::ids::DocTypeVersion,
                    2)));
    children.push_back(ElPtr(new
                jonen::UIntElement(jonen::ids::DocTypeReadVersion, 2)));

    std::streamsize body_size(0);
    BOOST_FOREACH(ElPtr el, children)
    {
        body_size += el->size();
    }

    jonen::EBMLElement e2("blag");
    e2.max_id_length(5);
    e2.max_size_length(7);
    e2.doc_version(2);
    e2.doc_read_version(2);
    EXPECT_EQ(jonen::ids::size(jonen::ids::EBML) +
            jonen::vint::size(body_size) + body_size, e2.size());
}

