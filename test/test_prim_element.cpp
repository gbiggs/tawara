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

#include <jonen/prim_element.h>

#include <gtest/gtest.h>
#include <jonen/exceptions.h>

#include "test_consts.h"


// Fake PrimitiveElement implementation
class FakePrimElement : public jonen::PrimitiveElement<int>
{
    public:
        FakePrimElement(uint32_t id)
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

        std::streamsize read_body(std::istream& input)
        {
            return 0;
        }
}; // class FakePrimElement


TEST(PrimElement, Construction)
{
    EXPECT_EQ(1234, FakePrimElement(1234).id());
    EXPECT_THROW(FakePrimElement(0x00), jonen::InvalidElementID);
    EXPECT_THROW(FakePrimElement(0xFF), jonen::InvalidElementID);
    EXPECT_THROW(FakePrimElement(0xFFFF), jonen::InvalidElementID);
    EXPECT_THROW(FakePrimElement(0xFFFFFF), jonen::InvalidElementID);
    EXPECT_THROW(FakePrimElement(0xFFFFFFFF), jonen::InvalidElementID);
}


TEST(PrimElement, CopyConstruction)
{
    EXPECT_EQ(1234, FakePrimElement(FakePrimElement(1234, 16)).id());
    EXPECT_EQ(16, FakePrimElement(FakePrimElement(1234, 16)).value());
    EXPECT_EQ(55, FakePrimElement(FakePrimElement(1234, 16, 55)).get_default());
    // The exception actually comes from the inner constructor, but just to be
    // sure it makes it out...
    EXPECT_THROW(FakePrimElement(FakePrimElement(0x00)), jonen::InvalidElementID);
}


TEST(PrimElement, SetID)
{
    FakePrimElement e(1234);
    e.id(9999999);
    EXPECT_EQ(9999999, e.id());
    EXPECT_THROW(FakePrimElement(1).id(0x00), jonen::InvalidElementID);
    EXPECT_THROW(FakePrimElement(1).id(0xFF), jonen::InvalidElementID);
    EXPECT_THROW(FakePrimElement(1).id(0xFFFF), jonen::InvalidElementID);
    EXPECT_THROW(FakePrimElement(1).id(0xFFFFFF), jonen::InvalidElementID);
    EXPECT_THROW(FakePrimElement(1).id(0xFFFFFFFF), jonen::InvalidElementID);
}


TEST(PrimElement, Assignment)
{
    FakePrimElement e1(1, 1), e2(2, 2);
    e2 = e1;
    EXPECT_EQ(e1.id(), e2.id());
    EXPECT_EQ(e1.value(), e2.value());
}

