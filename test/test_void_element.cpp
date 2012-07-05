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

#include <celduin/void_element.h>

#include <gtest/gtest.h>
#include <celduin/binary_element.h>
#include <celduin/el_ids.h>
#include <celduin/ebml_int.h>
#include <celduin/exceptions.h>
#include <celduin/string_element.h>
#include <celduin/vint.h>

#include "test_consts.h"
#include "test_utils.h"


namespace test_vel
{

std::streamsize fill_buffer(std::string& b, std::streamsize void_size, std::streamsize total_size,
        bool fill, bool write_id, bool write_body)
{
    std::streamsize written(0);
    if (write_id)
    {
        // Cheating on the IDs a bit - there is no protection here against
        // invalid IDs
        std::vector<char> tmp(celduin::ebml_int::encode_u(celduin::ids::Void));
        b.append(&tmp[0], 0, tmp.size());
        written += tmp.size();
    }
    else
    {
        // Ensure the written body size is correct for the void's total size
        void_size -= 1;
    }
    if (write_body)
    {
        std::vector<char> tmp(celduin::vint::encode(void_size - written - 1));
        b.append(&tmp[0], 0, tmp.size());
        written += tmp.size();
        int remaining(total_size - written);
        if (fill)
        {
            for (std::streamsize ii(0); ii < void_size; ++ii)
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
    celduin::VoidElement ve1(8);
    EXPECT_EQ(celduin::ids::Void, ve1.id());
    EXPECT_EQ(8, ve1.size());
    EXPECT_FALSE(ve1.fill());

    celduin::VoidElement ve2(8, true);
    EXPECT_EQ(celduin::ids::Void, ve2.id());
    EXPECT_EQ(8, ve2.size());
    EXPECT_TRUE(ve2.fill());

    celduin::VoidElement ve3(8, false);
    EXPECT_EQ(celduin::ids::Void, ve3.id());
    EXPECT_EQ(8, ve3.size());
    EXPECT_FALSE(ve3.fill());
}


TEST(VoidElement, CopyConstruction)
{
    celduin::VoidElement ve1(8);
    EXPECT_EQ(celduin::ids::Void, celduin::VoidElement(ve1).id());
    EXPECT_EQ(8, celduin::VoidElement(ve1).size());
    EXPECT_FALSE(celduin::VoidElement(ve1).fill());

    celduin::VoidElement ve2(8, true);
    EXPECT_EQ(celduin::ids::Void, celduin::VoidElement(ve2).id());
    EXPECT_EQ(8, celduin::VoidElement(ve2).size());
    EXPECT_TRUE(celduin::VoidElement(ve2).fill());

    celduin::VoidElement ve3(8, false);
    EXPECT_EQ(celduin::ids::Void, celduin::VoidElement(ve3).id());
    EXPECT_EQ(8, celduin::VoidElement(ve3).size());
    EXPECT_FALSE(celduin::VoidElement(ve3).fill());
}


TEST(VoidElement, CopyElement)
{
    celduin::StringElement se(0x80, "12345");

    std::streamsize se_size(se.size());
    celduin::VoidElement ve1(se);
    EXPECT_EQ(se_size, ve1.size());
    EXPECT_FALSE(ve1.fill());

    celduin::VoidElement ve2(se, true);
    EXPECT_EQ(se_size, ve2.size());
    EXPECT_TRUE(ve2.fill());

    // Test elements with a size right on the border of two encoded sizes for
    // the body size
    for (int ii(0); ii < 10; ++ii)
    {
        celduin::BinaryElement be(0x81, std::vector<char>(0x3FFB + ii, 0xC0));
        celduin::VoidElement ve3(be);
        EXPECT_EQ(be.size(), ve3.size());
    }
}


TEST(VoidElement, Assignment)
{
    celduin::VoidElement e1(8), e2(16);
    e2 = e1;
    EXPECT_EQ(e1.id(), e2.id());
    EXPECT_EQ(e1.size(), e2.size());
    EXPECT_EQ(e1.fill(), e2.fill());

    celduin::VoidElement e3(8, true), e4(16, false);
    e3 = e4;
    EXPECT_EQ(e3.id(), e4.id());
    EXPECT_EQ(e3.size(), e4.size());
    EXPECT_EQ(e3.fill(), e4.fill());

    celduin::VoidElement e5(8, false), e6(16, true);
    e5 = e6;
    EXPECT_EQ(e5.id(), e6.id());
    EXPECT_EQ(e5.size(), e6.size());
    EXPECT_EQ(e5.fill(), e6.fill());
}


TEST(VoidElement, Size)
{
    celduin::VoidElement e1(8);
    EXPECT_EQ(8, e1.size());
    e1.set_size(16000);
    EXPECT_EQ(16000, e1.size());
    e1.set_size(4);
    EXPECT_EQ(4, e1.size());
}


TEST(VoidElement, Fill)
{
    celduin::VoidElement e1(8);
    EXPECT_FALSE(e1.fill());
    e1.fill(true);
    EXPECT_TRUE(e1.fill());
    e1.fill(false);
    EXPECT_FALSE(e1.fill());

    celduin::VoidElement e2(8, true);
    EXPECT_TRUE(e2.fill());
    e2.fill(false);
    EXPECT_FALSE(e2.fill());

    celduin::VoidElement e3(8, false);
    EXPECT_FALSE(e3.fill());
    e3.fill(true);
    EXPECT_TRUE(e3.fill());
}


TEST(VoidElement, Write)
{
    std::stringstream output;
    std::string expected;
    std::streamsize size(5), f_size(20);
    bool fill(false);
    std::string c0;
    for (std::streamsize ii(0); ii < f_size; ++ii)
    {
        // Fill the output with some data to overwrite
        c0.push_back(0xC0);
    }

    celduin::VoidElement v(size);
    output.str(c0);
    std::string().swap(expected);
    test_vel::fill_buffer(expected, size, f_size, fill, true, true);
    EXPECT_EQ(size, v.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
    // With or without filling, the stream write pointer should be after the
    // body of the void element
    EXPECT_EQ(v.size(), output.tellp());

    fill = true;
    v.fill(fill);
    output.str(c0);
    output.seekp(0, std::ios::beg);
    std::string().swap(expected);
    test_vel::fill_buffer(expected, size, f_size, fill, true, true);
    EXPECT_EQ(size, v.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
    EXPECT_EQ(v.size(), output.tellp());

    // Offset test
    output.str("ab" + c0);
    output.seekp(2);
    expected = "ab";
    test_vel::fill_buffer(expected, size, f_size, fill, true, true);
    v.write(output);
    EXPECT_EQ(2, v.offset());
}


TEST(VoidElement, Read)
{
    std::stringstream input;
    std::string input_val;
    std::streamsize size(5), f_size(20);
    bool fill(false);

    celduin::VoidElement v(2);
    test_vel::fill_buffer(input_val, size, f_size, fill, false, true);
    input.str(input_val);
    EXPECT_EQ(size - celduin::ids::size(celduin::ids::Void), v.read(input));
    EXPECT_EQ(celduin::ids::Void, v.id());
    EXPECT_EQ(size, v.size());
    // Subtract the ID size because it wasn't written to input
    EXPECT_EQ(v.size() - celduin::ids::size(celduin::ids::Void),
            input.tellg());

    fill = true;
    std::string().swap(input_val);
    test_vel::fill_buffer(input_val, size, f_size, fill, false, true);
    input.str(input_val);
    EXPECT_EQ(size - celduin::ids::size(celduin::ids::Void), v.read(input));
    EXPECT_EQ(celduin::ids::Void, v.id());
    EXPECT_EQ(size, v.size());
    // Subtract the ID size because it wasn't written to input
    EXPECT_EQ(v.size() - celduin::ids::size(celduin::ids::Void),
            input.tellg());

    // Offset test
    input_val = "ab";
    input_val.push_back(celduin::ids::Void);
    test_vel::fill_buffer(input_val, size, f_size, fill, false, true);
    input.str(input_val);
    input.seekg(3);
    v.read(input);
    EXPECT_EQ(2, v.offset());

    // Test for ReadError exception
    std::string().swap(input_val);
    test_vel::fill_buffer(input_val, size, f_size, fill, false, true);
    input.str(input_val.substr(0, 3));
    EXPECT_THROW(v.read(input), celduin::ReadError);
}

