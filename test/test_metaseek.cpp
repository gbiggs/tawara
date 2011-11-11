/* Tide
 *
 * Test the SeekHead (metaseek) element.
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

#include <boost/foreach.hpp>
#include <gtest/gtest.h>
#include <tide/el_ids.h>
#include <tide/exceptions.h>
#include <tide/metaseek.h>
#include <tide/vint.h>

#include "test_utils.h"


TEST(Metaseek, Create)
{
    tide::Metaseek e;
    EXPECT_EQ(tide::ids::SeekHead, e.id());
    EXPECT_EQ(0, e.index_size());
}


TEST(Metaseek, Append)
{
    tide::Metaseek e;
    e.append(tide::Metaseek::IndexItem(tide::ids::EBML, 15));
    ASSERT_EQ(1, e.index_size());
    e.append(tide::Metaseek::IndexItem(tide::ids::SeekHead, 4829));
    ASSERT_EQ(2, e.index_size());
    tide::Metaseek::IndexItem i0 = e[0];
    EXPECT_EQ(i0.first, tide::ids::EBML);
    EXPECT_EQ(i0.second, 15);
    tide::Metaseek::IndexItem i1 = e[1];
    EXPECT_EQ(i1.first, tide::ids::SeekHead);
    EXPECT_EQ(i1.second, 4829);
}


TEST(Metaseek, Remove)
{
    tide::Metaseek e;
    e.append(tide::Metaseek::IndexItem(tide::ids::SeekHead, 4829));
    e.append(tide::Metaseek::IndexItem(tide::ids::EBML, 15));
    ASSERT_EQ(2, e.index_size());
    e.remove(0);
    ASSERT_EQ(1, e.index_size());
    tide::Metaseek::IndexItem i0 = e[0];
    EXPECT_EQ(i0.first, tide::ids::EBML);
    EXPECT_EQ(i0.second, 15);
    e.append(tide::Metaseek::IndexItem(tide::ids::DocType, 42));
    e.remove(1);
    tide::Metaseek::IndexItem i1 = e[0];
    EXPECT_EQ(i1.first, tide::ids::EBML);
    EXPECT_EQ(i1.second, 15);
}


TEST(Metaseek, IndexOperator)
{
    tide::Metaseek e;
    e.append(tide::Metaseek::IndexItem(tide::ids::SeekHead, 4829));
    e.append(tide::Metaseek::IndexItem(tide::ids::EBML, 15));
    e.append(tide::Metaseek::IndexItem(tide::ids::DocType, 42));
    tide::Metaseek::IndexItem i0 = e[0];
    EXPECT_EQ(i0.first, tide::ids::SeekHead);
    EXPECT_EQ(i0.second, 4829);
    i0 = e[2];
    EXPECT_EQ(i0.first, tide::ids::DocType);
    EXPECT_EQ(i0.second, 42);
}


TEST(Metaseek, Size)
{
    tide::Metaseek ms;

    EXPECT_EQ(0, ms.size());
    EXPECT_EQ(tide::ids::coded_size(tide::ids::SeekHead) +
            tide::vint::coded_size(0),
            ms.total_size());

    std::vector<tide::SeekElement> children;
    children.push_back(tide::SeekElement(tide::ids::SeekHead, 0x7F));
    children.push_back(tide::SeekElement(tide::ids::DocType, 0x76FB));
    children.push_back(tide::SeekElement(tide::ids::SeekID, 0x1FFFFFFF));

    std::streamsize body_size(0);
    BOOST_FOREACH(tide::SeekElement e, children)
    {
        body_size += e.total_size();
        ms.append(tide::Metaseek::IndexItem(e.indexed_id(), e.offset()));
    }

    EXPECT_EQ(body_size, ms.size());
    EXPECT_EQ(tide::ids::coded_size(tide::ids::SeekHead) +
            tide::vint::coded_size(body_size) + body_size,
            ms.total_size());
}


TEST(Metaseek, Write)
{
    std::ostringstream output;
    std::stringstream expected;

    tide::Metaseek ms;

    std::vector<tide::SeekElement> children;
    children.push_back(tide::SeekElement(tide::ids::SeekHead, 0x7F));
    children.push_back(tide::SeekElement(tide::ids::DocType, 0x76FB));
    children.push_back(tide::SeekElement(tide::ids::SeekID, 0x1FFFFFFF));

    std::streamsize body_size(0);
    BOOST_FOREACH(tide::SeekElement e, children)
    {
        body_size += e.total_size();
        e.write(expected);
        ms.append(tide::Metaseek::IndexItem(e.indexed_id(), e.offset()));
    }

    EXPECT_EQ(body_size, ms.write_body(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected.str());

    output.str(std::string());
    expected.str(std::string());
    tide::ids::write(tide::ids::SeekHead, expected);
    tide::vint::write(body_size, expected);
    BOOST_FOREACH(tide::SeekElement e, children)
    {
        e.write(expected);
    }

    EXPECT_EQ(tide::ids::coded_size(tide::ids::SeekHead) +
            tide::vint::coded_size(body_size) + body_size,
            ms.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected.str());
}


TEST(Metaseek, Read)
{
    std::stringstream input;

    std::vector<tide::SeekElement> children;
    children.push_back(tide::SeekElement(tide::ids::SeekHead, 0x7F));
    children.push_back(tide::SeekElement(tide::ids::DocType, 0x76FB));
    children.push_back(tide::SeekElement(tide::ids::SeekID, 0x10203040));

    std::streamsize body_size(0);
    BOOST_FOREACH(tide::SeekElement e, children)
    {
        body_size += e.total_size();
    }
    tide::vint::write(body_size, input);
    BOOST_FOREACH(tide::SeekElement e, children)
    {
        e.write(input);
    }

    tide::Metaseek ms;
    EXPECT_EQ(tide::vint::coded_size(body_size) + body_size,
            ms.read_body(input));
    EXPECT_EQ(3, ms.index_size());
    tide::Metaseek::IndexItem ii(ms[0]);
    EXPECT_EQ(tide::ids::SeekHead, ii.first);
    EXPECT_EQ(0x7F, ii.second);
    ii = ms[1];
    EXPECT_EQ(tide::ids::DocType, ii.first);
    EXPECT_EQ(0x76FB, ii.second);
    ii = ms[2];
    EXPECT_EQ(tide::ids::SeekID, ii.first);
    EXPECT_EQ(0x10203040, ii.second);

    // No children at all
    input.str(std::string());
    tide::vint::write(0, input);
    EXPECT_EQ(tide::vint::coded_size(0), ms.read_body(input));
    // Metaseek should be clearing its stored index before reading, so the
    // previous test's index should not affect this result
    EXPECT_EQ(0, ms.index_size());
    // Body size value wrong (too small)
    input.str(std::string());
    tide::vint::write(2, input);
    children[0].write(input);
    EXPECT_THROW(ms.read_body(input), tide::BadBodySize);
}

