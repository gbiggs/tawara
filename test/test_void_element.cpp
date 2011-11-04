/* TIDE
 *
 * Void element tests.
 *
 * Copyright 2011 Geoffrey Biggs geoffrey.biggs@aist.go.jp
 *     RT-Synthesis Research Group
 *     Intelligent Systems Research Institute,
 *     National Institute of Advanced Industrial Science and Technology (AIST),
 *     Japan
 *     All rights reserved.
 *
 * This file is part of TIDE.
 *
 * TIDE is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * TIDE is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with TIDE. If not, see <http://www.gnu.org/licenses/>.
 */

#include <tide/void_element.h>

#include <gtest/gtest.h>
#include <tide/binary_element.h>
#include <tide/exceptions.h>
#include <tide/string_element.h>
#include <tide/vint.h>

#include "test_consts.h"
#include "test_utils.h"


namespace test_vel
{

size_t fill_buffer(std::string& b, size_t void_size, size_t total_size,
        bool fill, bool write_id, bool write_body)
{
    char temp[8];
    size_t n(0), written(0);
    if (write_id)
    {
        n = tide::vint::encode(tide::VOID_ELEMENT_ID,
                reinterpret_cast<uint8_t*>(temp), 8);
        b.append(temp, 0, n);
        written += n;
    }
    if (write_body)
    {
        n = tide::vint::encode(void_size, reinterpret_cast<uint8_t*>(temp), 8);
        b.append(temp, 0, n);
        written += n;
        int remaining(total_size - written);
        if (fill)
        {
            for (size_t ii(0); ii < void_size; ++ii)
            {
                b.push_back(0);
            }
            written += void_size;
            remaining -= void_size;
        }
        if (remaining > 0)
        {
            for (int ii(0); ii < remaining; ++ii)
            {
                b.push_back(0xC0);
            }
            written += remaining;
        }
    }
    return written;
}

}; // namespace test_vel


TEST(VoidElement, Construction)
{
    tide::VoidElement ve1(8);
    EXPECT_EQ(tide::VOID_ELEMENT_ID, ve1.id());
    EXPECT_EQ(8, ve1.size());
    EXPECT_FALSE(ve1.fill());

    tide::VoidElement ve2(8, true);
    EXPECT_EQ(tide::VOID_ELEMENT_ID, ve2.id());
    EXPECT_EQ(8, ve2.size());
    EXPECT_TRUE(ve2.fill());

    tide::VoidElement ve3(8, false);
    EXPECT_EQ(tide::VOID_ELEMENT_ID, ve3.id());
    EXPECT_EQ(8, ve3.size());
    EXPECT_FALSE(ve3.fill());
}


TEST(VoidElement, CopyConstruction)
{
    tide::VoidElement ve1(8);
    EXPECT_EQ(tide::VOID_ELEMENT_ID, tide::VoidElement(ve1).id());
    EXPECT_EQ(8, tide::VoidElement(ve1).size());
    EXPECT_FALSE(tide::VoidElement(ve1).fill());

    tide::VoidElement ve2(8, true);
    EXPECT_EQ(tide::VOID_ELEMENT_ID, tide::VoidElement(ve2).id());
    EXPECT_EQ(8, tide::VoidElement(ve2).size());
    EXPECT_TRUE(tide::VoidElement(ve2).fill());

    tide::VoidElement ve3(8, false);
    EXPECT_EQ(tide::VOID_ELEMENT_ID, tide::VoidElement(ve3).id());
    EXPECT_EQ(8, tide::VoidElement(ve3).size());
    EXPECT_FALSE(tide::VoidElement(ve3).fill());
}


TEST(VoidElement, CopyElement)
{
    tide::StringElement se(1, "12345");

    size_t se_size(se.total_size());
    tide::VoidElement ve1(se);
    EXPECT_EQ(se_size, ve1.total_size());
    EXPECT_EQ(se_size - 2, ve1.size());
    EXPECT_FALSE(ve1.fill());

    tide::VoidElement ve2(se, true);
    EXPECT_EQ(se_size, ve2.total_size());
    EXPECT_EQ(se_size - 2, ve2.size());
    EXPECT_TRUE(ve2.fill());

    // Test elements with a size right on the border of two encoded sizes for
    // the body size
    for (int ii(0); ii < 10; ++ii)
    {
        tide::BinaryElement be(1, std::vector<char>(0x3FFB + ii, 0xC0));
        tide::VoidElement ve3(be);
        EXPECT_EQ(be.total_size(), ve3.total_size());
    }
}


TEST(VoidElement, Assignment)
{
    tide::VoidElement e1(8), e2(16);
    e2 = e1;
    EXPECT_EQ(e1.id(), e2.id());
    EXPECT_EQ(e1.size(), e2.size());
    EXPECT_EQ(e1.fill(), e2.fill());

    tide::VoidElement e3(8, true), e4(16, false);
    e3 = e4;
    EXPECT_EQ(e3.id(), e4.id());
    EXPECT_EQ(e3.size(), e4.size());
    EXPECT_EQ(e3.fill(), e4.fill());

    tide::VoidElement e5(8, false), e6(16, true);
    e5 = e6;
    EXPECT_EQ(e5.id(), e6.id());
    EXPECT_EQ(e5.size(), e6.size());
    EXPECT_EQ(e5.fill(), e6.fill());
}


TEST(VoidElement, Size)
{
    tide::VoidElement e1(8);
    EXPECT_EQ(8, e1.size());
    EXPECT_EQ(10, e1.total_size());
    e1.size(16000);
    EXPECT_EQ(16000, e1.size());
    EXPECT_EQ(16003, e1.total_size());
    e1.size(4);
    EXPECT_EQ(4, e1.size());
    EXPECT_EQ(6, e1.total_size());
}


TEST(VoidElement, Fill)
{
    tide::VoidElement e1(8);
    EXPECT_FALSE(e1.fill());
    e1.fill(true);
    EXPECT_TRUE(e1.fill());
    e1.fill(false);
    EXPECT_FALSE(e1.fill());

    tide::VoidElement e2(8, true);
    EXPECT_TRUE(e2.fill());
    e2.fill(false);
    EXPECT_FALSE(e2.fill());

    tide::VoidElement e3(8, false);
    EXPECT_FALSE(e3.fill());
    e3.fill(true);
    EXPECT_TRUE(e3.fill());
}


TEST(VoidElement, Write)
{
    std::stringstream output;
    std::string expected;
    size_t size(5), f_size(20);
    bool fill(false);
    std::string c0;
    for (size_t ii(0); ii < f_size; ++ii)
    {
        // Fill the output with some data to overwrite
        c0.push_back(0xC0);
    }

    tide::VoidElement v(size);
    test_vel::fill_buffer(expected, size, f_size, fill, false, true);
    output.str(c0);
    EXPECT_EQ(tide::vint::coded_size(size) + size, v.write_body(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
    // With or without filling, the stream write pointer should be after the
    // body of the void element
    EXPECT_EQ(tide::vint::coded_size(size) + size, output.tellp());

    output.seekp(0, std::ios::beg);
    output.str(std::string());
    std::string().swap(expected);
    test_vel::fill_buffer(expected, size, 0, fill, true, false);
    EXPECT_EQ(tide::vint::coded_size(tide::VOID_ELEMENT_ID),
            v.write_id(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.seekp(0, std::ios::beg);
    output.str(c0);
    std::string().swap(expected);
    test_vel::fill_buffer(expected, size, f_size, fill, true, true);
    EXPECT_EQ(tide::vint::coded_size(tide::VOID_ELEMENT_ID) +
            tide::vint::coded_size(size) + size, v.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
    EXPECT_EQ(v.total_size(), output.tellp());

    fill = true;
    v.fill(fill);
    std::string().swap(expected);
    test_vel::fill_buffer(expected, size, f_size, fill, false, true);
    output.seekp(0, std::ios::beg);
    output.str(c0);
    EXPECT_EQ(tide::vint::coded_size(size) + size, v.write_body(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
    // With or without filling, the stream write pointer should be after the
    // body of the void element
    EXPECT_EQ(tide::vint::coded_size(size) + size, output.tellp());
    EXPECT_EQ(tide::vint::coded_size(size) + size, output.tellp());

    output.seekp(0, std::ios::beg);
    output.str(std::string());
    std::string().swap(expected);
    test_vel::fill_buffer(expected, size, 0, fill, true, false);
    EXPECT_EQ(tide::vint::coded_size(tide::VOID_ELEMENT_ID),
            v.write_id(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);

    output.seekp(0, std::ios::beg);
    output.str(c0);
    std::string().swap(expected);
    test_vel::fill_buffer(expected, size, f_size, fill, true, true);
    EXPECT_EQ(tide::vint::coded_size(tide::VOID_ELEMENT_ID) +
            tide::vint::coded_size(size) + size, v.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
    EXPECT_EQ(v.total_size(), output.tellp());
}


TEST(VoidElement, Read)
{
    std::stringstream input;
    std::string input_val;
    size_t size(5), f_size(20);
    bool fill(false);

    tide::VoidElement v(0);
    test_vel::fill_buffer(input_val, size, f_size, fill, false, true);
    input.str(input_val);
    EXPECT_EQ(tide::vint::coded_size(size) + size, v.read_body(input));
    EXPECT_EQ(tide::VOID_ELEMENT_ID, v.id());
    EXPECT_EQ(size, v.size());
    EXPECT_EQ(tide::vint::coded_size(tide::VOID_ELEMENT_ID) +
            tide::vint::coded_size(size) + size, v.total_size());
    EXPECT_EQ(tide::vint::coded_size(v.size()) + v.size(), input.tellg());

    fill = true;
    std::string().swap(input_val);
    test_vel::fill_buffer(input_val, size, f_size, fill, false, true);
    input.str(input_val);
    EXPECT_EQ(tide::vint::coded_size(size) + size, v.read_body(input));
    EXPECT_EQ(tide::VOID_ELEMENT_ID, v.id());
    EXPECT_EQ(size, v.size());
    EXPECT_EQ(tide::vint::coded_size(tide::VOID_ELEMENT_ID) +
            tide::vint::coded_size(size) + size, v.total_size());
    EXPECT_EQ(tide::vint::coded_size(v.size()) + v.size(), input.tellg());

    // Test for ReadError exception
    std::string().swap(input_val);
    test_vel::fill_buffer(input_val, size, f_size, fill, false, true);
    input.str(input_val.substr(0, 4));
    EXPECT_THROW(v.read_body(input), tide::ReadError);
}

