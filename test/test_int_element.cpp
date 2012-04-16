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
#include <tide/exceptions.h>
#include <tide/integer_elements.h>

#include "test_consts.h"

using namespace tide;

namespace test_int_els
{
    TEST(IntElement, Construction)
    {
        EXPECT_EQ(ids::Null, IntElement(ids::Null, 0).id());
        EXPECT_EQ(42, IntElement(ids::Null, 42).value());
        EXPECT_EQ(42, IntElement(ids::Null, 42, 0).value());
        EXPECT_EQ(0, IntElement(ids::Null, 42, 0).get_default());
        EXPECT_EQ(false, IntElement(ids::Null, 42).has_default());
        EXPECT_EQ(true, IntElement(ids::Null, 42, 0).has_default());

        EXPECT_THROW(IntElement(0x00, 1), InvalidElementID);
        EXPECT_THROW(IntElement(0xFF, 1), InvalidElementID);
        EXPECT_THROW(IntElement(0xFFFF, 1), InvalidElementID);
        EXPECT_THROW(IntElement(0xFFFFFF, 1), InvalidElementID);
        EXPECT_THROW(IntElement(0xFFFFFFFF, 1), InvalidElementID);

        EXPECT_THROW(IntElement(0x00, 1, 2), InvalidElementID);
        EXPECT_THROW(IntElement(0xFF, 1, 2), InvalidElementID);
        EXPECT_THROW(IntElement(0xFFFF, 1, 2), InvalidElementID);
        EXPECT_THROW(IntElement(0xFFFFFF, 1, 2), InvalidElementID);
        EXPECT_THROW(IntElement(0xFFFFFFFF, 1, 2), InvalidElementID);
    }

    TEST(IntElement, Addition)
    {
        IntElement ee1(ids::Null, 1);
        IntElement ee2(ids::Null, 2);
        EXPECT_EQ(3, (ee1 + ee2).value());
        EXPECT_EQ(3, (ee1 + 2).value());
        EXPECT_EQ(3, 1 + ee2);
        ee1 += ee2;
        EXPECT_EQ(3, ee1.value());
        ee2 += 1;
        EXPECT_EQ(3, ee2.value());
    }
}; // namespace test_int_els

