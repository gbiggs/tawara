/* Tide
 *
 * Test the TrackOperation and related elements.
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
#include <tide/track_operation.h>
#include <tide/uint_element.h>
#include <tide/vint.h>

#include "test_utils.h"


///////////////////////////////////////////////////////////////////////////////
// TrackJoinBlocks tests
///////////////////////////////////////////////////////////////////////////////

TEST(TrackJoinBlocks, Create)
{
    tide::TrackJoinBlocks e;
    EXPECT_EQ(tide::ids::TrackJoinBlocks, e.id());
    EXPECT_EQ(0, e.count());
}


TEST(TrackJoinBlocks, Type)
{
    tide::TrackJoinBlocks e;
    EXPECT_EQ(e.type(), "joinblocks");
}


TEST(TrackJoinBlocks, Append)
{
    tide::TrackJoinBlocks e;
    e.append(0xFFFF);
    ASSERT_EQ(1, e.count());
    e.append(0xC0C0);
    ASSERT_EQ(2, e.count());
    EXPECT_EQ(0xFFFF, e[0]);
    EXPECT_EQ(0xC0C0, e[1]);

    EXPECT_THROW(e.append(0), tide::ValueOutOfRange);
}


TEST(TrackJoinBlocks, Remove)
{
    tide::TrackJoinBlocks e;
    e.append(0xFFFF);
    e.append(0xC0C0);
    ASSERT_EQ(2, e.count());
    e.remove(0);
    ASSERT_EQ(1, e.count());
    EXPECT_EQ(0xC0C0, e[0]);
    e.append(0x0101);
    e.remove(1);
    EXPECT_EQ(0xC0C0, e[0]);
}


TEST(TrackJoinBlocks, IndexOperator)
{
    tide::TrackJoinBlocks e;
    e.append(0xFFFF);
    e.append(0xC0C0);
    e.append(0x0101);
    EXPECT_EQ(0xFFFF, e[0]);
    EXPECT_EQ(0x0101, e[2]);
}


TEST(TrackJoinBlocks, Size)
{
    tide::TrackJoinBlocks e;

    EXPECT_EQ(0, e.size());
    EXPECT_EQ(tide::ids::coded_size(tide::ids::TrackJoinBlocks) +
            tide::vint::coded_size(0),
            e.total_size());

    std::vector<tide::UIntElement> children;
    children.push_back(tide::UIntElement(tide::ids::TrackJoinUID, 0xFFFF));
    children.push_back(tide::UIntElement(tide::ids::TrackJoinUID, 0xC0C0));
    children.push_back(tide::UIntElement(tide::ids::TrackJoinUID, 0x0101));

    std::streamsize body_size(0);
    BOOST_FOREACH(tide::UIntElement child, children)
    {
        body_size += child.total_size();
        e.append(child.value());
    }

    EXPECT_EQ(body_size, e.size());
    EXPECT_EQ(tide::ids::coded_size(tide::ids::TrackJoinBlocks) +
            tide::vint::coded_size(body_size) + body_size,
            e.total_size());
}


TEST(TrackJoinBlocks, Write)
{
    std::ostringstream output;
    std::stringstream expected;

    tide::TrackJoinBlocks e;

    std::vector<tide::UIntElement> children;
    children.push_back(tide::UIntElement(tide::ids::TrackJoinUID, 0xFFFF));
    children.push_back(tide::UIntElement(tide::ids::TrackJoinUID, 0xC0C0));
    children.push_back(tide::UIntElement(tide::ids::TrackJoinUID, 0x0101));

    std::streamsize body_size(0);
    BOOST_FOREACH(tide::UIntElement child, children)
    {
        body_size += child.total_size();
        child.write(expected);
        e.append(child.value());
    }

    EXPECT_EQ(body_size, e.write_body(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected.str());

    output.str(std::string());
    expected.str(std::string());
    tide::ids::write(tide::ids::TrackJoinBlocks, expected);
    tide::vint::write(body_size, expected);
    BOOST_FOREACH(tide::UIntElement e, children)
    {
        e.write(expected);
    }

    EXPECT_EQ(tide::ids::coded_size(tide::ids::TrackJoinBlocks) +
            tide::vint::coded_size(body_size) + body_size,
            e.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected.str());
}


TEST(TrackJoinBlocks, Read)
{
    std::stringstream input;

    std::vector<tide::UIntElement> children;
    children.push_back(tide::UIntElement(tide::ids::TrackJoinUID, 0xFFFF));
    children.push_back(tide::UIntElement(tide::ids::TrackJoinUID, 0xC0C0));
    children.push_back(tide::UIntElement(tide::ids::TrackJoinUID, 0x0101));

    std::streamsize body_size(0);
    BOOST_FOREACH(tide::UIntElement child, children)
    {
        body_size += child.total_size();
    }
    tide::vint::write(body_size, input);
    BOOST_FOREACH(tide::UIntElement child, children)
    {
        child.write(input);
    }

    tide::TrackJoinBlocks e;
    EXPECT_EQ(tide::vint::coded_size(body_size) + body_size,
            e.read_body(input));
    EXPECT_EQ(3, e.count());
    EXPECT_EQ(0xFFFF, e[0]);
    EXPECT_EQ(0xC0C0, e[1]);
    EXPECT_EQ(0x0101, e[2]);

    // No children at all
    input.str(std::string());
    tide::vint::write(0, input);
    EXPECT_THROW(e.read_body(input), tide::MissingChild);
    // TrackJoinBlocks should be clearing its stored UIDs before reading, so
    // the previous test's index should not affect this result
    EXPECT_EQ(0, e.count());
    // Body size value wrong (too small)
    input.str(std::string());
    tide::vint::write(2, input);
    children[0].write(input);
    EXPECT_THROW(e.read_body(input), tide::BadBodySize);
    // Invalid child
    input.str(std::string());
    tide::UIntElement ue(tide::ids::EBML, 0xFFFF);
    tide::vint::write(ue.total_size(), input);
    ue.write(input);
    EXPECT_THROW(e.read_body(input), tide::InvalidChildID);
}


///////////////////////////////////////////////////////////////////////////////
// TrackOperation tests
///////////////////////////////////////////////////////////////////////////////

TEST(TrackOperation, Create)
{
    tide::TrackOperation e;
    EXPECT_EQ(tide::ids::TrackOperation, e.id());
    EXPECT_EQ(0, e.count());
}


TEST(TrackOperation, Append)
{
    tide::TrackOperation e;
    tide::TrackOperation::OpPtr op1(new tide::TrackJoinBlocks);
    boost::static_pointer_cast<tide::TrackJoinBlocks>(op1)->append(0xFFFF);
    e.append(op1);
    ASSERT_EQ(1, e.count());
    tide::TrackOperation::OpPtr op2(new tide::TrackJoinBlocks);
    boost::static_pointer_cast<tide::TrackJoinBlocks>(op2)->append(0xC0C0);
    e.append(op2);
    ASSERT_EQ(2, e.count());
    boost::shared_ptr<tide::TrackJoinBlocks> v1, v2;
    v1 = boost::static_pointer_cast<tide::TrackJoinBlocks>(e[0]);
    v2 = boost::static_pointer_cast<tide::TrackJoinBlocks>(e[1]);
    EXPECT_EQ(0xFFFF, (*v1)[0]);
    EXPECT_EQ(0xC0C0, (*v2)[0]);

    tide::TrackOperation::OpPtr op3(new tide::TrackJoinBlocks);
    EXPECT_THROW(e.append(op3), tide::ValueOutOfRange);
}


TEST(TrackOperation, Remove)
{
    tide::TrackOperation e;
    tide::TrackOperation::OpPtr op1(new tide::TrackJoinBlocks);
    boost::static_pointer_cast<tide::TrackJoinBlocks>(op1)->append(0xFFFF);
    e.append(op1);
    tide::TrackOperation::OpPtr op2(new tide::TrackJoinBlocks);
    boost::static_pointer_cast<tide::TrackJoinBlocks>(op2)->append(0xC0C0);
    e.append(op2);
    ASSERT_EQ(2, e.count());
    e.remove(0);
    ASSERT_EQ(1, e.count());
    EXPECT_EQ(0xC0C0,
            (*boost::static_pointer_cast<tide::TrackJoinBlocks>(e[0]))[0]);
    tide::TrackOperation::OpPtr op3(new tide::TrackJoinBlocks);
    boost::static_pointer_cast<tide::TrackJoinBlocks>(op3)->append(0x0101);
    e.append(op3);
    e.remove(1);
    EXPECT_EQ(0xC0C0,
            (*boost::static_pointer_cast<tide::TrackJoinBlocks>(e[0]))[0]);
}


TEST(TrackOperation, IndexOperator)
{
    tide::TrackOperation e;
    tide::TrackOperation::OpPtr op1(new tide::TrackJoinBlocks);
    boost::static_pointer_cast<tide::TrackJoinBlocks>(op1)->append(0xFFFF);
    e.append(op1);
    tide::TrackOperation::OpPtr op2(new tide::TrackJoinBlocks);
    boost::static_pointer_cast<tide::TrackJoinBlocks>(op2)->append(0xC0C0);
    e.append(op2);
    tide::TrackOperation::OpPtr op3(new tide::TrackJoinBlocks);
    boost::static_pointer_cast<tide::TrackJoinBlocks>(op3)->append(0x0101);
    e.append(op3);
    EXPECT_EQ(0xFFFF,
            (*boost::static_pointer_cast<tide::TrackJoinBlocks>(e[0]))[0]);
    EXPECT_EQ(0x0101,
            (*boost::static_pointer_cast<tide::TrackJoinBlocks>(e[2]))[0]);

    tide::TrackOperation::OpPtr op4(new tide::TrackJoinBlocks);
    boost::static_pointer_cast<tide::TrackJoinBlocks>(op4)->append(0x1010);
    e[1] = op4;
    EXPECT_EQ(0x1010,
            (*boost::static_pointer_cast<tide::TrackJoinBlocks>(e[1]))[0]);
}


TEST(TrackOperation, Size)
{
    tide::TrackOperation e;

    EXPECT_EQ(0, e.size());
    EXPECT_EQ(tide::ids::coded_size(tide::ids::TrackOperation) +
            tide::vint::coded_size(0),
            e.total_size());

    std::vector<tide::TrackJoinBlocks> children;
    tide::TrackJoinBlocks op1; op1.append(0xFFFF);
    children.push_back(op1);
    tide::TrackJoinBlocks op2; op2.append(0xC0C0);
    children.push_back(op2);
    tide::TrackJoinBlocks op3; op3.append(0x0101);
    children.push_back(op3);

    std::streamsize body_size(0);
    BOOST_FOREACH(tide::TrackJoinBlocks child, children)
    {
        body_size += child.total_size();
        e.append(tide::TrackOperation::OpPtr(new tide::TrackJoinBlocks(child)));
    }

    EXPECT_EQ(body_size, e.size());
    EXPECT_EQ(tide::ids::coded_size(tide::ids::TrackOperation) +
            tide::vint::coded_size(body_size) + body_size,
            e.total_size());
}


TEST(TrackOperation, Write)
{
    std::ostringstream output;
    std::stringstream expected;

    tide::TrackOperation e;

    std::vector<tide::TrackJoinBlocks> children;
    tide::TrackJoinBlocks op1; op1.append(0xFFFF);
    children.push_back(op1);
    tide::TrackJoinBlocks op2; op2.append(0xC0C0);
    children.push_back(op2);
    tide::TrackJoinBlocks op3; op3.append(0x0101);
    children.push_back(op3);

    std::streamsize body_size(0);
    BOOST_FOREACH(tide::TrackJoinBlocks child, children)
    {
        body_size += child.total_size();
        child.write(expected);
        e.append(tide::TrackOperation::OpPtr(new tide::TrackJoinBlocks(child)));
    }

    EXPECT_EQ(body_size, e.write_body(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected.str());

    output.str(std::string());
    expected.str(std::string());
    tide::ids::write(tide::ids::TrackOperation, expected);
    tide::vint::write(body_size, expected);
    BOOST_FOREACH(tide::TrackJoinBlocks child, children)
    {
        child.write(expected);
    }

    EXPECT_EQ(tide::ids::coded_size(tide::ids::TrackOperation) +
            tide::vint::coded_size(body_size) + body_size,
            e.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected.str());
}


TEST(TrackOperation, Read)
{
    std::stringstream input;

    std::vector<tide::TrackJoinBlocks> children;
    tide::TrackJoinBlocks op1; op1.append(0xFFFF);
    children.push_back(op1);
    tide::TrackJoinBlocks op2; op2.append(0xC0C0);
    children.push_back(op2);
    tide::TrackJoinBlocks op3; op3.append(0x0101);
    children.push_back(op3);

    std::streamsize body_size(0);
    BOOST_FOREACH(tide::TrackJoinBlocks child, children)
    {
        body_size += child.total_size();
    }

    tide::vint::write(body_size, input);
    BOOST_FOREACH(tide::TrackJoinBlocks child, children)
    {
        child.write(input);
    }

    tide::TrackOperation e;
    EXPECT_EQ(tide::vint::coded_size(body_size) + body_size,
            e.read_body(input));
    EXPECT_EQ(3, e.count());
    EXPECT_EQ(0xFFFF,
            (*boost::static_pointer_cast<tide::TrackJoinBlocks>(e[0]))[0]);
    EXPECT_EQ(0xC0C0,
            (*boost::static_pointer_cast<tide::TrackJoinBlocks>(e[1]))[0]);
    EXPECT_EQ(0x0101,
            (*boost::static_pointer_cast<tide::TrackJoinBlocks>(e[2]))[0]);

    // No children at all
    input.str(std::string());
    tide::vint::write(0, input);
    EXPECT_EQ(tide::vint::coded_size(0), e.read_body(input));
    EXPECT_EQ(0, e.count());
    // Body size value wrong (too small)
    input.str(std::string());
    tide::vint::write(2, input);
    children[0].write(input);
    EXPECT_THROW(e.read_body(input), tide::BadBodySize);
    // Invalid child
    input.str(std::string());
    tide::UIntElement ue(tide::ids::EBML, 0xFFFF);
    tide::vint::write(ue.total_size(), input);
    ue.write(input);
    EXPECT_THROW(e.read_body(input), tide::InvalidChildID);
}

