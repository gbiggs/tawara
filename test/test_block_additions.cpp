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
#include <celduin/block_additions.h>
#include <celduin/el_ids.h>
#include <celduin/ebml_int.h>
#include <celduin/exceptions.h>
#include <celduin/uint_element.h>
#include <celduin/vint.h>

#include "test_utils.h"


namespace test_block_add
{
    celduin::BlockAdditions::value_type make_add()
    {
        static unsigned int id(1);
        static size_t size(7);
        return celduin::BlockAdditions::value_type(new
                celduin::BlockAdditions::Addition(id++,
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
    celduin::BlockAdditions b;
    EXPECT_TRUE(b.empty());
}


TEST(BlockAdditionals, Assignment)
{
    celduin::BlockAdditions b1;
    celduin::BlockAdditions b2;
    celduin::BlockAdditions::value_type f1(test_block_add::make_add());
    celduin::BlockAdditions::value_type f2(test_block_add::make_add());
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
    celduin::BlockAdditions b;
    celduin::BlockAdditions::value_type f1(test_block_add::make_add());
    celduin::BlockAdditions::value_type f2(test_block_add::make_add());
    b.push_back(f1);
    b.push_back(f2);
    EXPECT_EQ(b[1], b.at(1));
    EXPECT_EQ(b.at(1)->second.size(), f2->second.size());
    EXPECT_THROW(b.at(2), std::out_of_range);
}


TEST(BlockAdditionals, SubscriptOperator)
{
    celduin::BlockAdditions b;
    celduin::BlockAdditions::value_type f1(test_block_add::make_add());
    celduin::BlockAdditions::value_type f2(test_block_add::make_add());
    b.push_back(f1);
    b.push_back(f2);
    EXPECT_EQ(b[1], b.at(1));
    EXPECT_EQ(b.at(1)->second.size(), f2->second.size());
    EXPECT_NO_THROW(b[2]);

    celduin::BlockAdditions::value_type f3(test_block_add::make_add());
    b[1] = f3;
    EXPECT_EQ(b[1], f3);
    EXPECT_EQ(b[1]->first, f3->first);
    EXPECT_EQ(b[1]->second.size(), f3->second.size());
}


TEST(BlockAdditionals, BeginEnd)
{
    celduin::BlockAdditions b;
    celduin::BlockAdditions::value_type f1(test_block_add::make_add());

    EXPECT_TRUE(b.begin() == b.end());
    EXPECT_TRUE(b.rbegin() == b.rend());
    b.push_back(f1);
    EXPECT_FALSE(b.begin() == b.end());
    EXPECT_FALSE(b.rbegin() == b.rend());
}


TEST(BlockAdditionals, Counts)
{
    celduin::BlockAdditions b;
    celduin::BlockAdditions::value_type f1(test_block_add::make_add());
    celduin::BlockAdditions::value_type f2(test_block_add::make_add());

    EXPECT_TRUE(b.empty());
    b.push_back(f1);
    b.push_back(f2);
    EXPECT_FALSE(b.empty());
    EXPECT_EQ(2, b.count());

    EXPECT_NE(0, b.max_count());
}


TEST(BlockAdditionals, Clear)
{
    celduin::BlockAdditions b;
    celduin::BlockAdditions::value_type f1(test_block_add::make_add());
    b.push_back(f1);
    EXPECT_FALSE(b.empty());
    b.clear();
    EXPECT_TRUE(b.empty());
}


TEST(BlockAdditionals, PushBack)
{
    celduin::BlockAdditions b;
    celduin::BlockAdditions::value_type f1(test_block_add::make_add());
    celduin::BlockAdditions::value_type f2(test_block_add::make_add());

    EXPECT_TRUE(b.empty());
    EXPECT_NO_THROW(b.push_back(f1));
    EXPECT_FALSE(b.empty());
    EXPECT_EQ(1, b.count());
    EXPECT_NO_THROW(b.push_back(f2));
    EXPECT_EQ(2, b.count());

    celduin::BlockAdditions::value_type zero(new
            celduin::BlockAdditions::Addition(0,
                *test_utils::make_blob(5)));
    EXPECT_THROW(b.push_back(zero), celduin::ValueOutOfRange);
}


TEST(BlockAdditionals, Erase)
{
    celduin::BlockAdditions b;
    celduin::BlockAdditions::value_type f1(test_block_add::make_add());
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
    celduin::BlockAdditions b;
    celduin::BlockAdditions::value_type f1(test_block_add::make_add());
    celduin::BlockAdditions::value_type f2(test_block_add::make_add());
    celduin::BlockAdditions::value_type f3(test_block_add::make_add());

    EXPECT_NO_THROW(b.resize(2));
    EXPECT_EQ(2, b.count());
    EXPECT_TRUE(!b[1]);
}


TEST(BlockAdditionals, Swap)
{
    celduin::BlockAdditions b1;
    celduin::BlockAdditions b2;
    celduin::BlockAdditions::value_type f1(test_block_add::make_add());
    celduin::BlockAdditions::value_type f2(test_block_add::make_add());
    celduin::BlockAdditions::value_type f3(test_block_add::make_add());
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
    celduin::BlockAdditions b1;
    celduin::BlockAdditions b2;
    EXPECT_TRUE(b1 == b2);
    EXPECT_FALSE(b1 != b2);

    celduin::BlockAdditions::value_type f1(test_block_add::make_add());
    b1.push_back(f1);
    EXPECT_FALSE(b1 == b2);
    EXPECT_TRUE(b1 != b2);
}


TEST(BlockAdditionals, Size)
{
    celduin::BlockAdditions b;
    celduin::BlockAdditions::value_type f1(test_block_add::make_add());
    celduin::BlockAdditions::value_type f2(test_block_add::make_add());
    // Make sure we only have one value with an AddID of 1
    f1->first = 1;
    f2->first = 2;
    b.push_back(f1);
    b.push_back(f2);

    std::streamsize body_size(0);

    // First value has a default BlockAddID
    std::streamsize more_size_1 = celduin::ids::size(celduin::ids::BlockAdditional) +
        celduin::vint::size(f1->second.size()) + f1->second.size();
    std::streamsize more_size_2 = celduin::ids::size(celduin::ids::BlockAddID) +
        celduin::vint::size(celduin::ebml_int::size_u(f1->first)) +
        celduin::ebml_int::size_u(f1->first) +
        celduin::ids::size(celduin::ids::BlockAdditional) +
        celduin::vint::size(f2->second.size()) + f2->second.size();
    body_size += celduin::ids::size(celduin::ids::BlockMore) * 2 +
        celduin::vint::size(more_size_1) + more_size_1 +
        celduin::vint::size(more_size_2) + more_size_2;
    std::streamsize total_size = celduin::ids::size(celduin::ids::BlockAdditions) +
        celduin::vint::size(body_size) + body_size;

    EXPECT_EQ(total_size, b.size());
}


TEST(BlockAdditionals, Write)
{
    std::ostringstream output;
    std::stringstream expected;

    celduin::BlockAdditions b;
    celduin::BlockAdditions::value_type f1(test_block_add::make_add());
    celduin::BlockAdditions::value_type f2(test_block_add::make_add());

    std::streamsize more_size_1 = celduin::ids::size(celduin::ids::BlockAdditional) +
        celduin::vint::size(f1->second.size()) + f1->second.size();
    std::streamsize more_size_2 = celduin::ids::size(celduin::ids::BlockAddID) +
        celduin::vint::size(celduin::ebml_int::size_u(f1->first)) +
        celduin::ebml_int::size_u(f1->first) +
        celduin::ids::size(celduin::ids::BlockAdditional) +
        celduin::vint::size(f2->second.size()) + f2->second.size();
    std::streamsize body_size = celduin::ids::size(celduin::ids::BlockMore) * 2 +
        celduin::vint::size(more_size_1) + more_size_1 +
        celduin::vint::size(more_size_2) + more_size_2;
    std::streamsize total_size = celduin::ids::size(celduin::ids::BlockAdditions) +
        celduin::vint::size(body_size) + body_size;

    // Make sure we only have one value with an AddID of 1
    f1->first = 1;
    f2->first = 2;
    b.push_back(f1);
    b.push_back(f2);
    celduin::ids::write(celduin::ids::BlockAdditions, expected);
    celduin::vint::write(body_size, expected);
    // First child
    celduin::ids::write(celduin::ids::BlockMore, expected);
    celduin::vint::write(more_size_1, expected);
    celduin::ids::write(celduin::ids::BlockAdditional, expected);
    celduin::vint::write(f1->second.size(), expected);
    expected.write(&f1->second[0], f1->second.size());
    // Second child
    celduin::ids::write(celduin::ids::BlockMore, expected);
    celduin::vint::write(more_size_2, expected);
    celduin::ids::write(celduin::ids::BlockAddID, expected);
    celduin::vint::write(celduin::ebml_int::size_u(f2->first), expected);
    celduin::ebml_int::write_u(f2->first, expected);
    celduin::ids::write(celduin::ids::BlockAdditional, expected);
    celduin::vint::write(f2->second.size(), expected);
    expected.write(&f2->second[0], f2->second.size());
    EXPECT_EQ(total_size, b.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());

    // No children
    b.clear();
    EXPECT_THROW(b.write(output), celduin::EmptyBlockAdditionsElement);

    // Zero value in children
    celduin::BlockAdditions::value_type zero(new
            celduin::BlockAdditions::Addition(0,
                *test_utils::make_blob(5)));
    b.push_back(f1);
    b[0]->first = 0;
    EXPECT_THROW(b.write(output), celduin::ValueOutOfRange);
}


TEST(BlockAdditionals, Read)
{
    std::stringstream input;

    celduin::BlockAdditions b;
    celduin::BlockAdditions::value_type f1(test_block_add::make_add());
    celduin::BlockAdditions::value_type f2(test_block_add::make_add());
    // Make sure we only have one value with an AddID of 1
    f1->first = 1;
    f2->first = 2;

    std::streamsize more_size_1 = celduin::ids::size(celduin::ids::BlockAdditional) +
        celduin::vint::size(f1->second.size()) + f1->second.size();
    std::streamsize more_size_2 = celduin::ids::size(celduin::ids::BlockAddID) +
        celduin::vint::size(celduin::ebml_int::size_u(f1->first)) +
        celduin::ebml_int::size_u(f1->first) +
        celduin::ids::size(celduin::ids::BlockAdditional) +
        celduin::vint::size(f2->second.size()) + f2->second.size();
    std::streamsize body_size = celduin::ids::size(celduin::ids::BlockMore) * 2 +
        celduin::vint::size(more_size_1) + more_size_1 +
        celduin::vint::size(more_size_2) + more_size_2;
    std::streamsize total_size = celduin::vint::size(body_size) + body_size;

    celduin::vint::write(body_size, input);
    // First child
    celduin::ids::write(celduin::ids::BlockMore, input);
    celduin::vint::write(more_size_1, input);
    celduin::ids::write(celduin::ids::BlockAdditional, input);
    celduin::vint::write(f1->second.size(), input);
    input.write(&f1->second[0], f1->second.size());
    // Second child
    celduin::ids::write(celduin::ids::BlockMore, input);
    celduin::vint::write(more_size_2, input);
    celduin::ids::write(celduin::ids::BlockAddID, input);
    celduin::vint::write(celduin::ebml_int::size_u(f2->first), input);
    celduin::ebml_int::write_u(f2->first, input);
    celduin::ids::write(celduin::ids::BlockAdditional, input);
    celduin::vint::write(f2->second.size(), input);
    input.write(&f2->second[0], f2->second.size());
    EXPECT_EQ(total_size, b.read(input));
    EXPECT_EQ(2, b.count());
    EXPECT_EQ(f1->first, b[0]->first);
    EXPECT_TRUE(f1->second == b[0]->second);
    EXPECT_EQ(f2->first, b[1]->first);
    EXPECT_TRUE(f2->second == b[1]->second);

    // No children
    input.str(std::string());
    celduin::vint::write(0, input);
    EXPECT_THROW(b.read(input), celduin::EmptyBlockAdditionsElement);

    // Missing child
    celduin::vint::write(celduin::ids::size(celduin::ids::BlockMore) +
            celduin::vint::size(0), input);
    // First child
    celduin::ids::write(celduin::ids::BlockMore, input);
    celduin::vint::write(0, input);
    EXPECT_THROW(b.read(input), celduin::MissingChild);

    // Read error
    input.str(std::string());
    celduin::vint::write(body_size, input);
    celduin::ids::write(celduin::ids::BlockMore, input);
    celduin::vint::write(more_size_1, input);
    EXPECT_THROW(b.read(input), celduin::ReadError);
    input.clear();

    // Bad body size
    input.str(std::string());
    celduin::vint::write(celduin::vint::size(more_size_1) + more_size_1 + 5, input);
    // First child
    celduin::ids::write(celduin::ids::BlockMore, input);
    celduin::vint::write(more_size_1, input);
    celduin::ids::write(celduin::ids::BlockAdditional, input);
    celduin::vint::write(f1->second.size(), input);
    input.write(&f1->second[0], f1->second.size());
    // Second child
    celduin::ids::write(celduin::ids::BlockMore, input);
    celduin::vint::write(more_size_2, input);
    celduin::ids::write(celduin::ids::BlockAddID, input);
    celduin::vint::write(celduin::ebml_int::size_u(f2->first), input);
    celduin::ebml_int::write_u(f2->first, input);
    celduin::ids::write(celduin::ids::BlockAdditional, input);
    celduin::vint::write(f2->second.size(), input);
    input.write(&f2->second[0], f2->second.size());
    EXPECT_THROW(b.read(input), celduin::BadBodySize);

    // Invalid child
    input.str(std::string());
    celduin::UIntElement ue(celduin::ids::EBML, 0xFFFF);
    celduin::vint::write(ue.size(), input);
    ue.write(input);
    EXPECT_THROW(b.read(input), celduin::InvalidChildID);
    // Invalid child ID deeper down
    input.str(std::string());
    celduin::vint::write(body_size, input);
    celduin::ids::write(celduin::ids::BlockMore, input);
    celduin::vint::write(ue.size(), input);
    ue.write(input);
    EXPECT_THROW(b.read(input), celduin::InvalidChildID);
    // Value out of range
    input.str(std::string());
    celduin::vint::write(body_size, input);
    celduin::ids::write(celduin::ids::BlockMore, input);
    celduin::vint::write(more_size_2, input);
    celduin::ids::write(celduin::ids::BlockAddID, input);
    celduin::vint::write(celduin::ebml_int::size_u(0), input);
    celduin::ebml_int::write_u(0, input);
    celduin::ids::write(celduin::ids::BlockAdditional, input);
    celduin::vint::write(f2->second.size(), input);
    input.write(&f2->second[0], f2->second.size());
    EXPECT_THROW(b.read(input), celduin::ValueOutOfRange);
}

