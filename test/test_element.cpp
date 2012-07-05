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
#include <celduin/el_ids.h>
#include <celduin/element.h>
#include <celduin/exceptions.h>
#include <celduin/uint_element.h>
#include <celduin/vint.h>

#include "test_consts.h"
#include "test_utils.h"


// Fake Element implementation
class FakeElement : public celduin::Element
{
    public:
        FakeElement(uint32_t id)
            : Element(id)
        {
        }

        std::streamsize body_size() const
        {
            return 0;
        }

        std::streamsize write_id(std::ostream& output)
        {
            return 0;
        }

        std::streamsize write_body(std::ostream& output)
        {
            return 0;
        }

        std::streamsize read_body(std::istream& input, std::streamsize size)
        {
            return size;
        }
}; // class FakeElement


TEST(Element, Construction)
{
    EXPECT_EQ(1234, FakeElement(1234).id());
    EXPECT_THROW(FakeElement(0x00), celduin::InvalidElementID);
    EXPECT_THROW(FakeElement(0xFF), celduin::InvalidElementID);
    EXPECT_THROW(FakeElement(0xFFFF), celduin::InvalidElementID);
    EXPECT_THROW(FakeElement(0xFFFFFF), celduin::InvalidElementID);
    EXPECT_THROW(FakeElement(0xFFFFFFFF), celduin::InvalidElementID);
}


TEST(Element, CopyConstruction)
{
    EXPECT_EQ(1234, FakeElement(FakeElement(1234)).id());
    // The exception actually comes from the inner constructor, but just to be
    // sure it makes it out...
    EXPECT_THROW(FakeElement(FakeElement(0x00)), celduin::InvalidElementID);
}


TEST(Element, Assignment)
{
    FakeElement e1(1), e2(2);
    e2 = e1;
    EXPECT_EQ(e1.id(), e2.id());
}


TEST(Element, Size)
{
    FakeElement e(celduin::ids::EBML);
    EXPECT_EQ(celduin::ids::size(celduin::ids::EBML) +
            celduin::vint::size(0), e.size());
}


TEST(Element, Write)
{
    std::ostringstream output;
    std::stringstream expected;
    unsigned int dummy(0xFFFF);

    // Place some dummy data at the start to test the element recording its
    // write position.
    celduin::vint::write(dummy, output);
    celduin::vint::write(dummy, expected);

    FakeElement e(celduin::ids::EBML);
    celduin::ids::write(celduin::ids::EBML, expected);
    celduin::vint::write(0, expected);
    EXPECT_EQ(celduin::ids::size(celduin::ids::EBML) +
            celduin::vint::size(0), e.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());
    EXPECT_EQ(celduin::vint::size(dummy), e.offset());
}


TEST(Element, Read)
{
    std::stringstream input;
    unsigned int dummy(0xFFFF);
    FakeElement e(celduin::ids::EBML);

    celduin::vint::write(dummy, input);
    celduin::ids::write(celduin::ids::Info, input);
    celduin::vint::write(0, input);
    input.seekg(celduin::vint::size(dummy) +
            celduin::ids::size(celduin::ids::Info), std::ios::beg);

    EXPECT_EQ(celduin::vint::size(0), e.read(input));
    EXPECT_EQ(celduin::vint::size(dummy),
            e.offset());
}


TEST(ElementUtils, SkipRead)
{
    std::stringstream input;

    celduin::UIntElement ue1(celduin::ids::Null, 0xFFFFFFFF);
    celduin::UIntElement ue2(celduin::ids::Null, 0xFFFFFFFF);
    celduin::UIntElement ue3(celduin::ids::Null, 0xFFFFFFFF);
    ue1.write(input);
    ue2.write(input);
    ue3.write(input);
    input.seekp(0);
    input.seekg(0);

    celduin::skip_read(input, true);
    EXPECT_EQ(ue1.size(), input.tellg());
    EXPECT_EQ(0, input.tellp());
    celduin::ids::read(input);
    celduin::skip_read(input, false);
    EXPECT_EQ(ue1.size() + ue2.size(), input.tellg());
    EXPECT_EQ(0, input.tellp());
}


TEST(ElementUtils, SkipWrite)
{
    std::stringstream input;

    celduin::UIntElement ue1(celduin::ids::Null, 0xFFFFFFFF);
    celduin::UIntElement ue2(celduin::ids::Null, 0xFFFFFFFF);
    celduin::UIntElement ue3(celduin::ids::Null, 0xFFFFFFFF);
    ue1.write(input);
    ue2.write(input);
    ue3.write(input);
    input.seekp(0);
    input.seekg(0);

    celduin::skip_write(input, true);
    EXPECT_EQ(ue1.size(), input.tellp());
    EXPECT_EQ(0, input.tellg());
    input.seekp(celduin::ids::size(celduin::ids::Null), std::ios::cur);
    celduin::skip_write(input, false);
    EXPECT_EQ(ue1.size() + ue2.size(), input.tellp());
    EXPECT_EQ(0, input.tellg());
}

