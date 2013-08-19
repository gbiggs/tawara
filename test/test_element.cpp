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
#include <tawara/el_ids.h>
#include <tawara/element.h>
#include <tawara/exceptions.h>
#include <tawara/uint_element.h>
#include <tawara/vint.h>

#include "test_consts.h"
#include "test_utils.h"


// Fake Element implementation
class FakeElement : public tawara::Element
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
    EXPECT_THROW(FakeElement(0x00), tawara::InvalidElementID);
    EXPECT_THROW(FakeElement(0xFF), tawara::InvalidElementID);
    EXPECT_THROW(FakeElement(0xFFFF), tawara::InvalidElementID);
    EXPECT_THROW(FakeElement(0xFFFFFF), tawara::InvalidElementID);
    EXPECT_THROW(FakeElement(0xFFFFFFFF), tawara::InvalidElementID);
}


TEST(Element, CopyConstruction)
{
    EXPECT_EQ(1234, FakeElement(FakeElement(1234)).id());
    // The exception actually comes from the inner constructor, but just to be
    // sure it makes it out...
    EXPECT_THROW(FakeElement(FakeElement(0x00)), tawara::InvalidElementID);
}


TEST(Element, Assignment)
{
    FakeElement e1(1), e2(2);
    e2 = e1;
    EXPECT_EQ(e1.id(), e2.id());
}


TEST(Element, Size)
{
    FakeElement e(tawara::ids::EBML);
    EXPECT_EQ(tawara::ids::size(tawara::ids::EBML) +
            tawara::vint::size(0), e.size());
}


TEST(Element, Write)
{
    std::ostringstream output;
    std::stringstream expected;
    unsigned int dummy(0xFFFF);

    // Place some dummy data at the start to test the element recording its
    // write position.
    tawara::vint::write(dummy, output);
    tawara::vint::write(dummy, expected);

    FakeElement e(tawara::ids::EBML);
    tawara::ids::write(tawara::ids::EBML, expected);
    tawara::vint::write(0, expected);
    EXPECT_EQ(tawara::ids::size(tawara::ids::EBML) +
            tawara::vint::size(0), e.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());
    EXPECT_EQ(tawara::vint::size(dummy), e.offset());
}


TEST(Element, Read)
{
    std::stringstream input;
    unsigned int dummy(0xFFFF);
    FakeElement e(tawara::ids::EBML);

    tawara::vint::write(dummy, input);
    tawara::ids::write(tawara::ids::Info, input);
    tawara::vint::write(0, input);
    input.seekg(tawara::vint::size(dummy) +
            tawara::ids::size(tawara::ids::Info), std::ios::beg);

    EXPECT_EQ(tawara::vint::size(0), e.read(input));
    EXPECT_EQ(tawara::vint::size(dummy),
            e.offset());
}


TEST(ElementUtils, SkipRead)
{
    std::stringstream input;

    tawara::UIntElement ue1(tawara::ids::Null, 0xFFFFFFFF);
    tawara::UIntElement ue2(tawara::ids::Null, 0xFFFFFFFF);
    tawara::UIntElement ue3(tawara::ids::Null, 0xFFFFFFFF);
    ue1.write(input);
    ue2.write(input);
    ue3.write(input);
    input.seekp(0);
    input.seekg(0);

    tawara::skip_read(input, true);
    EXPECT_EQ(ue1.size(), input.tellg());
    EXPECT_EQ(0, input.tellp());
    tawara::ids::read(input);
    tawara::skip_read(input, false);
    EXPECT_EQ(ue1.size() + ue2.size(), input.tellg());
    EXPECT_EQ(0, input.tellp());
}


TEST(ElementUtils, SkipWrite)
{
    std::stringstream input;

    tawara::UIntElement ue1(tawara::ids::Null, 0xFFFFFFFF);
    tawara::UIntElement ue2(tawara::ids::Null, 0xFFFFFFFF);
    tawara::UIntElement ue3(tawara::ids::Null, 0xFFFFFFFF);
    ue1.write(input);
    ue2.write(input);
    ue3.write(input);
    input.seekp(0);
    input.seekg(0);

    tawara::skip_write(input, true);
    EXPECT_EQ(ue1.size(), input.tellp());
    EXPECT_EQ(0, input.tellg());
    input.seekp(tawara::ids::size(tawara::ids::Null), std::ios::cur);
    tawara::skip_write(input, false);
    EXPECT_EQ(ue1.size() + ue2.size(), input.tellp());
    EXPECT_EQ(0, input.tellg());
}

