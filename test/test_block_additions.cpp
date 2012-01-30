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
#include <tide/block_additions.h>
#include <tide/el_ids.h>
#include <tide/ebml_int.h>
#include <tide/exceptions.h>
#include <tide/uint_element.h>
#include <tide/vint.h>

#include "test_utils.h"


namespace test_block_add
{
    tide::BlockAdditions::value_type make_add()
    {
        static unsigned int id(1);
        static size_t size(7);
        return tide::BlockAdditions::value_type(new
                tide::BlockAdditions::Addition(id++,
                    *test_utils::make_blob(size)));
        size += 4;
        if (size > 50)
        {
            // Don't let this get too big
            size = 7;
        }
    }
}; // namespace test_block_add


TEST(BlockAdditionals, Create)
{
    tide::BlockAdditions b;
    EXPECT_TRUE(b.empty());
}


TEST(BlockAdditionals, Assignment)
{
    tide::BlockAdditions b1;
    tide::BlockAdditions b2;
    tide::BlockAdditions::value_type f1(test_block_add::make_add());
    tide::BlockAdditions::value_type f2(test_block_add::make_add());
    b1.push_back(f1);
    b1.push_back(f2);

    EXPECT_TRUE(b2.empty());
    b2 = b1;
    EXPECT_FALSE(b2.empty());
    EXPECT_EQ(b2[1]->first, f2->first);
    EXPECT_EQ(b2[1]->second.size(), f2->second.size());
}


TEST(BlockAdditionals, At)
{
    tide::BlockAdditions b;
    tide::BlockAdditions::value_type f1(test_block_add::make_add());
    tide::BlockAdditions::value_type f2(test_block_add::make_add());
    b.push_back(f1);
    b.push_back(f2);
    EXPECT_EQ(b[1], b.at(1));
    EXPECT_EQ(b.at(1)->second.size(), f2->second.size());
    EXPECT_THROW(b.at(2), std::out_of_range);
}


TEST(BlockAdditionals, SubscriptOperator)
{
    tide::BlockAdditions b;
    tide::BlockAdditions::value_type f1(test_block_add::make_add());
    tide::BlockAdditions::value_type f2(test_block_add::make_add());
    b.push_back(f1);
    b.push_back(f2);
    EXPECT_EQ(b[1], b.at(1));
    EXPECT_EQ(b.at(1)->second.size(), f2->second.size());
    EXPECT_NO_THROW(b[2]);

    tide::BlockAdditions::value_type f3(test_block_add::make_add());
    b[1] = f3;
    EXPECT_EQ(b[1], f3);
    EXPECT_EQ(b[1]->first, f3->first);
    EXPECT_EQ(b[1]->second.size(), f3->second.size());
}


TEST(BlockAdditionals, BeginEnd)
{
    tide::BlockAdditions b;
    tide::BlockAdditions::value_type f1(test_block_add::make_add());

    EXPECT_TRUE(b.begin() == b.end());
    EXPECT_TRUE(b.rbegin() == b.rend());
    b.push_back(f1);
    EXPECT_FALSE(b.begin() == b.end());
    EXPECT_FALSE(b.rbegin() == b.rend());
}


TEST(BlockAdditionals, Counts)
{
    tide::BlockAdditions b;
    tide::BlockAdditions::value_type f1(test_block_add::make_add());
    tide::BlockAdditions::value_type f2(test_block_add::make_add());

    EXPECT_TRUE(b.empty());
    b.push_back(f1);
    b.push_back(f2);
    EXPECT_FALSE(b.empty());
    EXPECT_EQ(2, b.count());

    EXPECT_NE(0, b.max_count());
}


TEST(BlockAdditionals, Clear)
{
    tide::BlockAdditions b;
    tide::BlockAdditions::value_type f1(test_block_add::make_add());
    b.push_back(f1);
    EXPECT_FALSE(b.empty());
    b.clear();
    EXPECT_TRUE(b.empty());
}


TEST(BlockAdditionals, PushBack)
{
    tide::BlockAdditions b;
    tide::BlockAdditions::value_type f1(test_block_add::make_add());
    tide::BlockAdditions::value_type f2(test_block_add::make_add());

    EXPECT_TRUE(b.empty());
    EXPECT_NO_THROW(b.push_back(f1));
    EXPECT_FALSE(b.empty());
    EXPECT_EQ(1, b.count());
    EXPECT_NO_THROW(b.push_back(f2));
    EXPECT_EQ(2, b.count());

    tide::BlockAdditions::value_type zero(new
            tide::BlockAdditions::Addition(0,
                *test_utils::make_blob(5)));
    EXPECT_THROW(b.push_back(zero), tide::ValueOutOfRange);
}


TEST(BlockAdditionals, Erase)
{
    tide::BlockAdditions b;
    tide::BlockAdditions::value_type f1(test_block_add::make_add());
    b.push_back(f1);

    EXPECT_FALSE(b.empty());
    b.erase(b.begin());
    EXPECT_TRUE(b.empty());

    b.push_back(f1);
    EXPECT_FALSE(b.empty());
    b.erase(b.begin(), b.end());
    EXPECT_TRUE(b.empty());
}


TEST(BlockAdditionals, Resize)
{
    tide::BlockAdditions b;
    tide::BlockAdditions::value_type f1(test_block_add::make_add());
    tide::BlockAdditions::value_type f2(test_block_add::make_add());
    tide::BlockAdditions::value_type f3(test_block_add::make_add());

    EXPECT_NO_THROW(b.resize(2));
    EXPECT_EQ(2, b.count());
    EXPECT_TRUE(!b[1]);
}


TEST(BlockAdditionals, Swap)
{
    tide::BlockAdditions b1;
    tide::BlockAdditions b2;
    tide::BlockAdditions::value_type f1(test_block_add::make_add());
    tide::BlockAdditions::value_type f2(test_block_add::make_add());
    tide::BlockAdditions::value_type f3(test_block_add::make_add());
    b1.push_back(f1);
    b2.push_back(f2);
    b2.push_back(f3);

    b2.swap(b1);
    EXPECT_EQ(2, b1.count());
    EXPECT_EQ(1, b2.count());
    EXPECT_TRUE(f2 == b1[0]);
    EXPECT_TRUE(f1 == b2[0]);
}


TEST(BlockAdditionals, Equality)
{
    tide::BlockAdditions b1;
    tide::BlockAdditions b2;
    EXPECT_TRUE(b1 == b2);
    EXPECT_FALSE(b1 != b2);

    tide::BlockAdditions::value_type f1(test_block_add::make_add());
    b1.push_back(f1);
    EXPECT_FALSE(b1 == b2);
    EXPECT_TRUE(b1 != b2);
}


TEST(BlockAdditionals, Size)
{
    tide::BlockAdditions b;
    tide::BlockAdditions::value_type f1(test_block_add::make_add());
    tide::BlockAdditions::value_type f2(test_block_add::make_add());
    // Make sure we only have one value with an AddID of 1
    f1->first = 1;
    f2->first = 2;
    b.push_back(f1);
    b.push_back(f2);

    std::streamsize body_size(0);

    // First value has a default BlockAddID
    std::streamsize more_size_1 = tide::ids::size(tide::ids::BlockAdditional) +
        tide::vint::size(f1->second.size()) + f1->second.size();
    std::streamsize more_size_2 = tide::ids::size(tide::ids::BlockAddID) +
        tide::vint::size(tide::ebml_int::size_u(f1->first)) +
        tide::ebml_int::size_u(f1->first) +
        tide::ids::size(tide::ids::BlockAdditional) +
        tide::vint::size(f2->second.size()) + f2->second.size();
    body_size += tide::ids::size(tide::ids::BlockMore) * 2 +
        tide::vint::size(more_size_1) + more_size_1 +
        tide::vint::size(more_size_2) + more_size_2;
    std::streamsize total_size = tide::ids::size(tide::ids::BlockAdditions) +
        tide::vint::size(body_size) + body_size;

    EXPECT_EQ(total_size, b.size());
}


TEST(BlockAdditionals, Write)
{
    std::ostringstream output;
    std::stringstream expected;

    tide::BlockAdditions b;
    tide::BlockAdditions::value_type f1(test_block_add::make_add());
    tide::BlockAdditions::value_type f2(test_block_add::make_add());

    std::streamsize more_size_1 = tide::ids::size(tide::ids::BlockAdditional) +
        tide::vint::size(f1->second.size()) + f1->second.size();
    std::streamsize more_size_2 = tide::ids::size(tide::ids::BlockAddID) +
        tide::vint::size(tide::ebml_int::size_u(f1->first)) +
        tide::ebml_int::size_u(f1->first) +
        tide::ids::size(tide::ids::BlockAdditional) +
        tide::vint::size(f2->second.size()) + f2->second.size();
    std::streamsize body_size = tide::ids::size(tide::ids::BlockMore) * 2 +
        tide::vint::size(more_size_1) + more_size_1 +
        tide::vint::size(more_size_2) + more_size_2;
    std::streamsize total_size = tide::ids::size(tide::ids::BlockAdditions) +
        tide::vint::size(body_size) + body_size;

    // Make sure we only have one value with an AddID of 1
    f1->first = 1;
    f2->first = 2;
    b.push_back(f1);
    b.push_back(f2);
    tide::ids::write(tide::ids::BlockAdditions, expected);
    tide::vint::write(body_size, expected);
    // First child
    tide::ids::write(tide::ids::BlockMore, expected);
    tide::vint::write(more_size_1, expected);
    tide::ids::write(tide::ids::BlockAdditional, expected);
    tide::vint::write(f1->second.size(), expected);
    expected.write(&f1->second[0], f1->second.size());
    // Second child
    tide::ids::write(tide::ids::BlockMore, expected);
    tide::vint::write(more_size_2, expected);
    tide::ids::write(tide::ids::BlockAddID, expected);
    tide::vint::write(tide::ebml_int::size_u(f2->first), expected);
    tide::ebml_int::write_u(f2->first, expected);
    tide::ids::write(tide::ids::BlockAdditional, expected);
    tide::vint::write(f2->second.size(), expected);
    expected.write(&f2->second[0], f2->second.size());
    EXPECT_EQ(total_size, b.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());

    // No children
    b.clear();
    EXPECT_THROW(b.write(output), tide::EmptyBlockAdditionsElement);

    // Zero value in children
    tide::BlockAdditions::value_type zero(new
            tide::BlockAdditions::Addition(0,
                *test_utils::make_blob(5)));
    b.push_back(f1);
    b[0]->first = 0;
    EXPECT_THROW(b.write(output), tide::ValueOutOfRange);
}


TEST(BlockAdditionals, Read)
{
    std::stringstream input;

    tide::BlockAdditions b;
    tide::BlockAdditions::value_type f1(test_block_add::make_add());
    tide::BlockAdditions::value_type f2(test_block_add::make_add());
    // Make sure we only have one value with an AddID of 1
    f1->first = 1;
    f2->first = 2;

    std::streamsize more_size_1 = tide::ids::size(tide::ids::BlockAdditional) +
        tide::vint::size(f1->second.size()) + f1->second.size();
    std::streamsize more_size_2 = tide::ids::size(tide::ids::BlockAddID) +
        tide::vint::size(tide::ebml_int::size_u(f1->first)) +
        tide::ebml_int::size_u(f1->first) +
        tide::ids::size(tide::ids::BlockAdditional) +
        tide::vint::size(f2->second.size()) + f2->second.size();
    std::streamsize body_size = tide::ids::size(tide::ids::BlockMore) * 2 +
        tide::vint::size(more_size_1) + more_size_1 +
        tide::vint::size(more_size_2) + more_size_2;
    std::streamsize total_size = tide::vint::size(body_size) + body_size;

    tide::vint::write(body_size, input);
    // First child
    tide::ids::write(tide::ids::BlockMore, input);
    tide::vint::write(more_size_1, input);
    tide::ids::write(tide::ids::BlockAdditional, input);
    tide::vint::write(f1->second.size(), input);
    input.write(&f1->second[0], f1->second.size());
    // Second child
    tide::ids::write(tide::ids::BlockMore, input);
    tide::vint::write(more_size_2, input);
    tide::ids::write(tide::ids::BlockAddID, input);
    tide::vint::write(tide::ebml_int::size_u(f2->first), input);
    tide::ebml_int::write_u(f2->first, input);
    tide::ids::write(tide::ids::BlockAdditional, input);
    tide::vint::write(f2->second.size(), input);
    input.write(&f2->second[0], f2->second.size());
    EXPECT_EQ(total_size, b.read(input));
    EXPECT_EQ(2, b.count());
    EXPECT_EQ(f1->first, b[0]->first);
    EXPECT_TRUE(f1->second == b[0]->second);
    EXPECT_EQ(f2->first, b[1]->first);
    EXPECT_TRUE(f2->second == b[1]->second);

    // No children
    input.str(std::string());
    tide::vint::write(0, input);
    EXPECT_THROW(b.read(input), tide::EmptyBlockAdditionsElement);

    // Missing child
    tide::vint::write(tide::ids::size(tide::ids::BlockMore) +
            tide::vint::size(0), input);
    // First child
    tide::ids::write(tide::ids::BlockMore, input);
    tide::vint::write(0, input);
    EXPECT_THROW(b.read(input), tide::MissingChild);

    // Read error
    input.str(std::string());
    tide::vint::write(body_size, input);
    tide::ids::write(tide::ids::BlockMore, input);
    tide::vint::write(more_size_1, input);
    EXPECT_THROW(b.read(input), tide::ReadError);
    input.clear();

    // Bad body size
    input.str(std::string());
    tide::vint::write(tide::vint::size(more_size_1) + more_size_1 + 5, input);
    // First child
    tide::ids::write(tide::ids::BlockMore, input);
    tide::vint::write(more_size_1, input);
    tide::ids::write(tide::ids::BlockAdditional, input);
    tide::vint::write(f1->second.size(), input);
    input.write(&f1->second[0], f1->second.size());
    // Second child
    tide::ids::write(tide::ids::BlockMore, input);
    tide::vint::write(more_size_2, input);
    tide::ids::write(tide::ids::BlockAddID, input);
    tide::vint::write(tide::ebml_int::size_u(f2->first), input);
    tide::ebml_int::write_u(f2->first, input);
    tide::ids::write(tide::ids::BlockAdditional, input);
    tide::vint::write(f2->second.size(), input);
    input.write(&f2->second[0], f2->second.size());
    EXPECT_THROW(b.read(input), tide::BadBodySize);

    // Invalid child
    input.str(std::string());
    tide::UIntElement ue(tide::ids::EBML, 0xFFFF);
    tide::vint::write(ue.size(), input);
    ue.write(input);
    EXPECT_THROW(b.read(input), tide::InvalidChildID);
    // Invalid child ID deeper down
    input.str(std::string());
    tide::vint::write(body_size, input);
    tide::ids::write(tide::ids::BlockMore, input);
    tide::vint::write(ue.size(), input);
    ue.write(input);
    EXPECT_THROW(b.read(input), tide::InvalidChildID);
    // Value out of range
    input.str(std::string());
    tide::vint::write(body_size, input);
    tide::ids::write(tide::ids::BlockMore, input);
    tide::vint::write(more_size_2, input);
    tide::ids::write(tide::ids::BlockAddID, input);
    tide::vint::write(tide::ebml_int::size_u(0), input);
    tide::ebml_int::write_u(0, input);
    tide::ids::write(tide::ids::BlockAdditional, input);
    tide::vint::write(f2->second.size(), input);
    input.write(&f2->second[0], f2->second.size());
    EXPECT_THROW(b.read(input), tide::ValueOutOfRange);
}

