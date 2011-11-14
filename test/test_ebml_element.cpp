/* Tide
 *
 * EBML Header element tests.
 *
 * Copyright 2011 Geoffrey Biggs geoffrey.biggs@aist.go.jp
 *     RT-Synthesis Research Group
 *     Intelligent Systems Research Institute,
 *     National Institute of Advanced Industrial Science and Technology (AIST),
 *     Japan
 *     All rights reserved.
 *
 * This file is part of Tide.
 *
 * Tide is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * Tide is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Tide. If not, see <http://www.gnu.org/licenses/>.
 */

#include <tide/ebml_element.h>

#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <gtest/gtest.h>
#include <tide/el_ids.h>
#include <tide/tide_config.h>
#include <tide/vint.h>

#include "test_consts.h"
#include "test_utils.h"


TEST(EBMLElement, Create)
{
    tide::EBMLElement e("Blag");
    EXPECT_EQ(tide::ids::EBML, e.id());
    EXPECT_EQ(1, e.version());
    EXPECT_EQ(1, e.read_version());
    EXPECT_EQ(4, e.max_id_length());
    EXPECT_EQ(8, e.max_size_length());
    EXPECT_EQ("Blag", e.doc_type());
    EXPECT_EQ(1, e.doc_version());
    EXPECT_EQ(1, e.doc_read_version());
}


TEST(EBMLElement, MaxIDLength)
{
    tide::EBMLElement e("");
    EXPECT_EQ(4, e.max_id_length());
    e.max_id_length(8);
    EXPECT_EQ(8, e.max_id_length());
}


TEST(EBMLElement, MaxSizeLength)
{
    tide::EBMLElement e("");
    EXPECT_EQ(8, e.max_size_length());
    e.max_size_length(4);
    EXPECT_EQ(4, e.max_size_length());
}


TEST(EBMLElement, DocType)
{
    tide::EBMLElement e("");
    EXPECT_EQ("", e.doc_type());
    e.doc_type("Tide");
    EXPECT_EQ("Tide", e.doc_type());
}


TEST(EBMLElement, DocVersion)
{
    tide::EBMLElement e("");
    EXPECT_EQ(1, e.doc_version());
    e.doc_version(2);
    EXPECT_EQ(2, e.doc_version());
}


TEST(EBMLElement, DocReadVersion)
{
    tide::EBMLElement e("");
    EXPECT_EQ(1, e.doc_read_version());
    e.doc_read_version(2);
    EXPECT_EQ(2, e.doc_read_version());
}


TEST(EBMLElement, Write)
{
    std::ostringstream output;
    std::stringstream expected;
    typedef boost::shared_ptr<tide::Element> ElPtr;
    std::vector<ElPtr> children;

    // Writing with everything defaults should give a full-length body for the
    // EBML header because values get written even if they are default
    children.push_back(ElPtr(new tide::UIntElement(tide::ids::EBMLVersion,
                    1)));
    children.push_back(ElPtr(new tide::UIntElement(tide::ids::EBMLReadVersion,
                    1)));
    children.push_back(ElPtr(new tide::UIntElement(tide::ids::EBMLMaxIDLength,
                    4)));
    children.push_back(ElPtr(new
                tide::UIntElement(tide::ids::EBMLMaxSizeLength, 8)));
    children.push_back(ElPtr(new tide::StringElement(tide::ids::DocType,
                    "tide")));
    children.push_back(ElPtr(new tide::UIntElement(tide::ids::DocTypeVersion,
                    1)));
    children.push_back(ElPtr(new
                tide::UIntElement(tide::ids::DocTypeReadVersion, 1)));
    tide::EBMLElement e;
    std::streamsize expected_size(0);
    BOOST_FOREACH(ElPtr el, children)
    {
        el->write(expected);
        expected_size += el->total_size();
    }

    EXPECT_EQ(expected_size, e.write_body(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());

    // Writing without defaults (note that EBMLVersion and EBMLReadVersion are
    // set by the library and so will *always* be default in this test).
    output.str(std::string());
    expected.str(std::string());
    children.clear();
    children.push_back(ElPtr(new tide::UIntElement(tide::ids::EBMLVersion,
                    1)));
    children.push_back(ElPtr(new tide::UIntElement(tide::ids::EBMLReadVersion,
                    1)));
    children.push_back(ElPtr(new tide::UIntElement(tide::ids::EBMLMaxIDLength,
                    5)));
    e.max_id_length(5);
    children.push_back(ElPtr(new
                tide::UIntElement(tide::ids::EBMLMaxSizeLength, 7)));
    e.max_size_length(7);
    children.push_back(ElPtr(new tide::StringElement(tide::ids::DocType,
                    "blag")));
    e.doc_type("blag");
    children.push_back(ElPtr(new tide::UIntElement(tide::ids::DocTypeVersion,
                    2)));
    e.doc_version(2);
    children.push_back(ElPtr(new
                tide::UIntElement(tide::ids::DocTypeReadVersion, 2)));
    e.doc_read_version(2);

    expected_size = 0;
    BOOST_FOREACH(ElPtr el, children)
    {
        el->write(expected);
        expected_size += el->total_size();
    }

    EXPECT_EQ(expected_size, e.write_body(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());

    expected.str(std::string());
    output.str(std::string());
    expected_size = tide::ids::coded_size(tide::ids::EBML) +
        tide::vint::coded_size(e.size());
    tide::ids::write(tide::ids::EBML, expected);
    tide::vint::write(e.size(), expected);
    BOOST_FOREACH(ElPtr el, children)
    {
        el->write(expected);
        expected_size += el->total_size();
    }
    EXPECT_EQ(expected_size, e.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());
}


TEST(EBMLElement, Read)
{
    std::stringstream input;
    typedef boost::shared_ptr<tide::Element> ElPtr;
    std::vector<ElPtr> children;

    children.push_back(ElPtr(new tide::UIntElement(tide::ids::EBMLVersion,
                    2)));
    children.push_back(ElPtr(new
                tide::UIntElement(tide::ids::DocTypeReadVersion, 2)));
    children.push_back(ElPtr(new tide::UIntElement(tide::ids::EBMLMaxIDLength,
                    5)));
    children.push_back(ElPtr(new tide::UIntElement(tide::ids::EBMLReadVersion,
                    2)));
    children.push_back(ElPtr(new
                tide::UIntElement(tide::ids::EBMLMaxSizeLength, 7)));
    children.push_back(ElPtr(new tide::UIntElement(tide::ids::DocTypeVersion,
                    2)));
    children.push_back(ElPtr(new tide::StringElement(tide::ids::DocType,
                    "blag")));

    tide::EBMLElement e("");

    std::streamsize body_size(0);
    BOOST_FOREACH(ElPtr el, children)
    {
        body_size += el->total_size();
    }
    tide::vint::write(body_size, input);
    BOOST_FOREACH(ElPtr el, children)
    {
        el->write(input);
    }

    EXPECT_EQ(tide::vint::coded_size(body_size) + body_size,
            e.read_body(input));
    EXPECT_EQ(2, e.version());
    EXPECT_EQ(2, e.read_version());
    EXPECT_EQ(5, e.max_id_length());
    EXPECT_EQ(7, e.max_size_length());
    EXPECT_EQ("blag", e.doc_type());
    EXPECT_EQ(2, e.doc_version());
    EXPECT_EQ(2, e.doc_read_version());

    // Zero-length body (all values should become defaults)
    input.str(std::string());
    tide::vint::write(0, input);
    EXPECT_EQ(1, e.read_body(input));
    EXPECT_EQ(1, e.version());
    EXPECT_EQ(1, e.read_version());
    EXPECT_EQ(4, e.max_id_length());
    EXPECT_EQ(8, e.max_size_length());
    EXPECT_EQ(tide::TideDocType, e.doc_type());
    EXPECT_EQ(1, e.doc_version());
    EXPECT_EQ(1, e.doc_read_version());
}


TEST(EBMLElement, Size)
{
    typedef boost::shared_ptr<tide::Element> ElPtr;
    std::vector<ElPtr> children;

    // Size with everything defaults
    tide::EBMLElement e1;
    EXPECT_EQ(31, e1.size());

    // Size with non-defaults. Note that EBMLVersion and EBMLReadVersion can
    // never be anything other than the default in this test.
    children.push_back(ElPtr(new tide::UIntElement(tide::ids::EBMLVersion,
                    1)));
    children.push_back(ElPtr(new tide::UIntElement(tide::ids::EBMLReadVersion,
                    1)));
    children.push_back(ElPtr(new tide::UIntElement(tide::ids::EBMLMaxIDLength,
                    5)));
    children.push_back(ElPtr(new
                tide::UIntElement(tide::ids::EBMLMaxSizeLength, 7)));
    children.push_back(ElPtr(new tide::StringElement(tide::ids::DocType,
                    "blag")));
    children.push_back(ElPtr(new tide::UIntElement(tide::ids::DocTypeVersion,
                    2)));
    children.push_back(ElPtr(new
                tide::UIntElement(tide::ids::DocTypeReadVersion, 2)));

    std::streamsize body_size(0);
    BOOST_FOREACH(ElPtr el, children)
    {
        body_size += el->total_size();
    }

    tide::EBMLElement e2("blag");
    e2.max_id_length(5);
    e2.max_size_length(7);
    e2.doc_version(2);
    e2.doc_read_version(2);
    EXPECT_EQ(body_size, e2.size());
    EXPECT_EQ(4 + tide::vint::coded_size(body_size) + body_size,
            e2.total_size());
}

