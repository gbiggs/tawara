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
#include <celduin/ebml_integer.h>
#include <celduin/exceptions.h>

#include "test_utilities.h"

using namespace celduin;

namespace test_ebml_integer
{
    TEST(EBMLInteger, CodedSizeUnsigned)
    {
        // 0 bytes
        EXPECT_EQ(0, ebml_int::size_u(0x00));
        // 1 byte
        EXPECT_EQ(1, ebml_int::size_u(0x01));
        EXPECT_EQ(1, ebml_int::size_u(0x7F));
        EXPECT_EQ(1, ebml_int::size_u(0xFF));
        // 2 bytes
        EXPECT_EQ(2, ebml_int::size_u(0x0100));
        EXPECT_EQ(2, ebml_int::size_u(0x0101));
        EXPECT_EQ(2, ebml_int::size_u(0x7FFF));
        EXPECT_EQ(2, ebml_int::size_u(0xFFFF));
        // 3 bytes
        EXPECT_EQ(3, ebml_int::size_u(0x010000));
        EXPECT_EQ(3, ebml_int::size_u(0x010001));
        EXPECT_EQ(3, ebml_int::size_u(0x7FFFFF));
        EXPECT_EQ(3, ebml_int::size_u(0xFFFFFF));
        // 4 bytes
        EXPECT_EQ(4, ebml_int::size_u(0x01000000));
        EXPECT_EQ(4, ebml_int::size_u(0x01000001));
        EXPECT_EQ(4, ebml_int::size_u(0x7FFFFFFF));
        EXPECT_EQ(4, ebml_int::size_u(0xFFFFFFFF));
        // 5 bytes
        EXPECT_EQ(5, ebml_int::size_u(0x0100000000));
        EXPECT_EQ(5, ebml_int::size_u(0x0100000001));
        EXPECT_EQ(5, ebml_int::size_u(0x7FFFFFFFFF));
        EXPECT_EQ(5, ebml_int::size_u(0xFFFFFFFFFF));
        // 6 bytes
        EXPECT_EQ(6, ebml_int::size_u(0x010000000000));
        EXPECT_EQ(6, ebml_int::size_u(0x010000000001));
        EXPECT_EQ(6, ebml_int::size_u(0x7FFFFFFFFFFF));
        EXPECT_EQ(6, ebml_int::size_u(0xFFFFFFFFFFFF));
        // 7 bytes
        EXPECT_EQ(7, ebml_int::size_u(0x01000000000000));
        EXPECT_EQ(7, ebml_int::size_u(0x01000000000001));
        EXPECT_EQ(7, ebml_int::size_u(0x7FFFFFFFFFFFFF));
        EXPECT_EQ(7, ebml_int::size_u(0xFFFFFFFFFFFFFF));
        // 8 bytes
        EXPECT_EQ(8, ebml_int::size_u(0x0100000000000000));
        EXPECT_EQ(8, ebml_int::size_u(0x0100000000000001));
        EXPECT_EQ(8, ebml_int::size_u(0x7FFFFFFFFFFFFFFF));
        EXPECT_EQ(8, ebml_int::size_u(0xFFFFFFFFFFFFFFFF));
    }


    TEST(EBMLInteger, CodedSizeSigned)
    {
        // 0 bytes
        EXPECT_EQ(0, ebml_int::size_s(0));
        // 1 byte
        EXPECT_EQ(1, ebml_int::size_s(-1));
        EXPECT_EQ(1, ebml_int::size_s(1));
        EXPECT_EQ(1, ebml_int::size_s(-0x80));
        EXPECT_EQ(1, ebml_int::size_s(0x7F));
        // 2 bytes
        EXPECT_EQ(2, ebml_int::size_s(-0x81));
        EXPECT_EQ(2, ebml_int::size_s(0x80));
        EXPECT_EQ(2, ebml_int::size_s(-0x8000));
        EXPECT_EQ(2, ebml_int::size_s(0x7FFF));
        // 3 bytes
        EXPECT_EQ(3, ebml_int::size_s(-0x8001));
        EXPECT_EQ(3, ebml_int::size_s(0x8000));
        EXPECT_EQ(3, ebml_int::size_s(-0x800000));
        EXPECT_EQ(3, ebml_int::size_s(0x7FFFFF));
        // 4 bytes
        EXPECT_EQ(4, ebml_int::size_s(-0x800001));
        EXPECT_EQ(4, ebml_int::size_s(0x800000));
        EXPECT_EQ(4, ebml_int::size_s(-0x80000000l));
        EXPECT_EQ(4, ebml_int::size_s(0x7FFFFFFFl));
        // 5 bytes
        EXPECT_EQ(5, ebml_int::size_s(-0x80000001l));
        EXPECT_EQ(5, ebml_int::size_s(0x80000000l));
        EXPECT_EQ(5, ebml_int::size_s(-0x8000000000l));
        EXPECT_EQ(5, ebml_int::size_s(0x7FFFFFFFFFl));
        // 6 bytes
        EXPECT_EQ(6, ebml_int::size_s(-0x8000000001l));
        EXPECT_EQ(6, ebml_int::size_s(0x8000000000l));
        EXPECT_EQ(6, ebml_int::size_s(-0x800000000000l));
        EXPECT_EQ(6, ebml_int::size_s(0x7FFFFFFFFFFFl));
        // 7 bytes
        EXPECT_EQ(7, ebml_int::size_s(-0x800000000001l));
        EXPECT_EQ(7, ebml_int::size_s(0x800000000000l));
        EXPECT_EQ(7, ebml_int::size_s(-0x80000000000000l));
        EXPECT_EQ(7, ebml_int::size_s(0x7FFFFFFFFFFFFFl));
        // 8 bytes
        EXPECT_EQ(8, ebml_int::size_s(-0x80000000000001l));
        EXPECT_EQ(8, ebml_int::size_s(0x80000000000000l));
        EXPECT_EQ(8, ebml_int::size_s(-0x8000000000000000l));
        EXPECT_EQ(8, ebml_int::size_s(0x7FFFFFFFFFFFFFFFl));
    }


    TEST(EBMLInteger, EncodeUnsigned)
    {
        std::vector<char> expected;
        // 0 bytes
        // This is checking that the buffer is untouched for zero
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0x00));
        // 1 byte
        expected.assign(1, 0x01);
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0x01));
        expected[0] = 0x7F;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0x7F));
        expected[0] = 0xFF;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0xFF));
        // 2 bytes
        expected.assign(2, 0);
        expected[0] = 0x01;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0x0100));
        expected[0] = 0x01; expected[1] = 0x01;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0x0101));
        expected[0] = 0x7F; expected[1] = 0xFF;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0x7FFF));
        expected[0] = 0xFF; expected[1] = 0xFF;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0xFFFF));
        // 3 bytes
        expected.assign(3, 0);
        expected[0] = 0x01;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0x010000));
        expected[0] = 0x01; expected[2] = 0x01;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0x010001));
        expected[0] = 0x7F; expected[1] = 0xFF; expected[2] = 0xFF;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0x7FFFFF));
        expected[0] = 0xFF;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0xFFFFFF));
        // 4 bytes
        expected.assign(4, 0);
        expected[0] = 0x01;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0x01000000));
        expected[0] = 0x01; expected[3] = 0x01;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0x01000001));
        expected.assign(4, 0xFF);
        expected[0] = 0x7F;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0x7FFFFFFF));
        expected[0] = 0xFF;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0xFFFFFFFF));
        // 5 bytes
        expected.assign(5, 0);
        expected[0] = 0x01;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0x0100000000l));
        expected[0] = 0x01; expected[4] = 0x01;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0x0100000001l));
        expected.assign(5, 0xFF);
        expected[0] = 0x7F;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0x7FFFFFFFFFl));
        expected[0] = 0xFF;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0xFFFFFFFFFFl));
        // 6 bytes
        expected.assign(6, 0);
        expected[0] = 0x01;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0x010000000000l));
        expected[0] = 0x01; expected[5] = 0x01;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0x010000000001l));
        expected.assign(6, 0xFF);
        expected[0] = 0x7F;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0x7FFFFFFFFFFFl));
        expected[0] = 0xFF;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0xFFFFFFFFFFFFl));
        // 7 bytes
        expected.assign(7, 0);
        expected[0] = 0x01;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0x01000000000000l));
        expected[0] = 0x01; expected[6] = 0x01;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0x01000000000001l));
        expected.assign(7, 0xFF);
        expected[0] = 0x7F;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0x7FFFFFFFFFFFFFl));
        expected[0] = 0xFF;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0xFFFFFFFFFFFFFFl));
        // 8 bytes
        expected.assign(8, 0);
        expected[0] = 0x01;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0x0100000000000000l));
        expected[0] = 0x01; expected[7] = 0x01;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0x0100000000000001l));
        expected.assign(8, 0xFF);
        expected[0] = 0x7F;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0x7FFFFFFFFFFFFFFFl));
        expected[0] = 0xFF;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0xFFFFFFFFFFFFFFFFl));
    }


    TEST(EBMLInteger, EncodeSigned)
    {
        std::vector<char> expected;
        // 0 bytes
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_u(0));
        // 1 byte
        expected.assign(1, 0xFF);
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(-1));
        expected[0] = 0x01;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(1));
        expected[0] = 0x80;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(-0x80));
        expected[0] = 0x7F;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(0x7F));
        // 2 bytes
        expected.assign(2, 0);
        expected[0] = 0xFF; expected[1] = 0x7F;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(-0x81));
        expected[0] = 0x00; expected[1] = 0x80;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(0x80));
        expected[0] = 0x80; expected[1] = 0x00;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(-0x8000));
        expected[0] = 0x7F; expected[1] = 0xFF;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(0x7FFF));
        // 3 bytes
        expected.assign(3, 0xFF);
        expected[1] = 0x7F;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(-0x8001));
        expected.assign(3, 0);
        expected[1] = 0x80;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(0x8000));
        expected.assign(3, 0);
        expected[0] = 0x80;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(-0x800000));
        expected.assign(3, 0xFF);
        expected[0] = 0x7F;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(0x7FFFFF));
        // 4 bytes
        expected.assign(4, 0xFF);
        expected[1] = 0x7F;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(-0x800001));
        expected.assign(4, 0);
        expected[1] = 0x80;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(0x800000));
        expected.assign(4, 0);
        expected[0] = 0x80;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(-0x80000000l));
        expected.assign(4, 0xFF);
        expected[0] = 0x7F;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(0x7FFFFFFFl));
        // 5 bytes
        expected.assign(5, 0xFF);
        expected[1] = 0x7F;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(-0x80000001l));
        expected.assign(5, 0);
        expected[1] = 0x80;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(0x80000000l));
        expected.assign(5, 0);
        expected[0] = 0x80;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(-0x8000000000l));
        expected.assign(5, 0xFF);
        expected[0] = 0x7F;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(0x7FFFFFFFFFl));
        // 6 bytes
        expected.assign(6, 0xFF);
        expected[1] = 0x7F;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(-0x8000000001));
        expected.assign(6, 0);
        expected[1] = 0x80;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(0x8000000000));
        expected.assign(6, 0);
        expected[0] = 0x80;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(-0x800000000000l));
        expected.assign(6, 0xFF);
        expected[0] = 0x7F;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(0x7FFFFFFFFFFFl));
        // 7 bytes
        expected.assign(7, 0xFF);
        expected[1] = 0x7F;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(-0x800000000001));
        expected.assign(7, 0);
        expected[1] = 0x80;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(0x800000000000));
        expected.assign(7, 0);
        expected[0] = 0x80;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(-0x80000000000000l));
        expected.assign(7, 0xFF);
        expected[0] = 0x7F;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(0x7FFFFFFFFFFFFFl));
        // 8 bytes
        expected.assign(8, 0xFF);
        expected[1] = 0x7F;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(-0x80000000000001));
        expected.assign(8, 0);
        expected[1] = 0x80;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(0x80000000000000));
        expected.assign(8, 0);
        expected[0] = 0x80;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(-0x8000000000000000l));
        expected.assign(8, 0xFF);
        expected[0] = 0x7F;
        EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, expected,
                ebml_int::encode_s(0x7FFFFFFFFFFFFFFFl));
    }


    TEST(EBMLInteger, DecodeUnsigned)
    {
        std::vector<char> buffer;
        // 0 bytes
        EXPECT_EQ(0, ebml_int::decode_u(buffer));
        // 1 byte
        buffer.assign(1, 0x01);
        EXPECT_EQ(1, ebml_int::decode_u(buffer));
        buffer[0] = 0x7F;
        EXPECT_EQ(0x7F, ebml_int::decode_u(buffer));
        buffer[0] = 0xFF;
        EXPECT_EQ(0xFF, ebml_int::decode_u(buffer));
        // 2 bytes
        buffer.assign(2, 0);
        buffer[0] = 0x01;
        EXPECT_EQ(0x0100, ebml_int::decode_u(buffer));
        buffer[0] = 0x01; buffer[1] = 0x01;
        EXPECT_EQ(0x0101, ebml_int::decode_u(buffer));
        buffer[0] = 0x7F; buffer[1] = 0xFF;
        EXPECT_EQ(0x7FFF, ebml_int::decode_u(buffer));
        buffer[0] = 0xFF; buffer[1] = 0xFF;
        EXPECT_EQ(0xFFFF, ebml_int::decode_u(buffer));
        // 3 bytes
        buffer.assign(3, 0);
        buffer[0] = 0x01;
        EXPECT_EQ(0x010000, ebml_int::decode_u(buffer));
        buffer[0] = 0x01; buffer[2] = 0x01;
        EXPECT_EQ(0x010001, ebml_int::decode_u(buffer));
        buffer.assign(3, 0xFF);
        buffer[0] = 0x7F;
        EXPECT_EQ(0x7FFFFF, ebml_int::decode_u(buffer));
        buffer[0] = 0xFF;
        EXPECT_EQ(0xFFFFFF, ebml_int::decode_u(buffer));
        // 4 bytes
        buffer.assign(4, 0);
        buffer[0] = 0x01;
        EXPECT_EQ(0x01000000, ebml_int::decode_u(buffer));
        buffer[0] = 0x01; buffer[3] = 0x01;
        EXPECT_EQ(0x01000001, ebml_int::decode_u(buffer));
        buffer.assign(4, 0xFF);
        buffer[0] = 0x7F;
        EXPECT_EQ(0x7FFFFFFF, ebml_int::decode_u(buffer));
        buffer[0] = 0xFF;
        EXPECT_EQ(0xFFFFFFFF, ebml_int::decode_u(buffer));
        // 5 bytes
        buffer.assign(5, 0);
        buffer[0] = 0x01;
        EXPECT_EQ(0x0100000000, ebml_int::decode_u(buffer));
        buffer[0] = 0x01; buffer[4] = 0x01;
        EXPECT_EQ(0x0100000001, ebml_int::decode_u(buffer));
        buffer.assign(5, 0xFF);
        buffer[0] = 0x7F;
        EXPECT_EQ(0x7FFFFFFFFF, ebml_int::decode_u(buffer));
        buffer[0] = 0xFF;
        EXPECT_EQ(0xFFFFFFFFFF, ebml_int::decode_u(buffer));
        // 6 bytes
        buffer.assign(6, 0);
        buffer[0] = 0x01;
        EXPECT_EQ(0x010000000000, ebml_int::decode_u(buffer));
        buffer[0] = 0x01; buffer[5] = 0x01;
        EXPECT_EQ(0x010000000001, ebml_int::decode_u(buffer));
        buffer.assign(6, 0xFF);
        buffer[0] = 0x7F;
        EXPECT_EQ(0x7FFFFFFFFFFF, ebml_int::decode_u(buffer));
        buffer[0] = 0xFF;
        EXPECT_EQ(0xFFFFFFFFFFFF, ebml_int::decode_u(buffer));
        // 7 bytes
        buffer.assign(7, 0);
        buffer[0] = 0x01;
        EXPECT_EQ(0x01000000000000, ebml_int::decode_u(buffer));
        buffer[0] = 0x01; buffer[6] = 0x01;
        EXPECT_EQ(0x01000000000001, ebml_int::decode_u(buffer));
        buffer.assign(7, 0xFF);
        buffer[0] = 0x7F;
        EXPECT_EQ(0x7FFFFFFFFFFFFF, ebml_int::decode_u(buffer));
        buffer[0] = 0xFF;
        EXPECT_EQ(0xFFFFFFFFFFFFFF, ebml_int::decode_u(buffer));
        // 8 bytes
        buffer.assign(8, 0);
        buffer[0] = 0x01;
        EXPECT_EQ(0x0100000000000000, ebml_int::decode_u(buffer));
        buffer[0] = 0x01; buffer[7] = 0x01;
        EXPECT_EQ(0x0100000000000001, ebml_int::decode_u(buffer));
        buffer.assign(8, 0xFF);
        buffer[0] = 0x7F;
        EXPECT_EQ(0x7FFFFFFFFFFFFFFF, ebml_int::decode_u(buffer));
        buffer[0] = 0xFF;
        EXPECT_EQ(0xFFFFFFFFFFFFFFFF, ebml_int::decode_u(buffer));
    }


    TEST(EBMLInteger, DecodeSigned)
    {
        std::vector<char> buffer;
        // 0 bytes
        EXPECT_EQ(0, ebml_int::decode_s(buffer));
        // 1 byte
        buffer.assign(1, 0x80);
        EXPECT_EQ(-0x80, ebml_int::decode_s(buffer));
        buffer[0] = 0xFF;
        EXPECT_EQ(-1, ebml_int::decode_s(buffer));
        buffer[0] = 0x7F;
        EXPECT_EQ(0x7F, ebml_int::decode_s(buffer));
        // 2 bytes
        buffer.assign(2, 0xFF);
        buffer[1] = 0x7F;
        EXPECT_EQ(-0x81, ebml_int::decode_s(buffer));
        buffer.assign(2, 0);
        buffer[1] = 0x80;
        EXPECT_EQ(0x80, ebml_int::decode_s(buffer));
        buffer.assign(2, 0);
        buffer[0] = 0x80;
        EXPECT_EQ(-0x8000, ebml_int::decode_s(buffer));
        buffer.assign(2, 0xFF);
        buffer[0] = 0x7F;
        EXPECT_EQ(0x7FFF, ebml_int::decode_s(buffer));
        // 3 bytes
        buffer.assign(3, 0xFF);
        buffer[1] = 0x7F;
        EXPECT_EQ(-0x8001, ebml_int::decode_s(buffer));
        buffer.assign(3, 0);
        buffer[1] = 0x80;
        EXPECT_EQ(0x8000, ebml_int::decode_s(buffer));
        buffer.assign(3, 0);
        buffer[0] = 0x80;
        EXPECT_EQ(-0x800000, ebml_int::decode_s(buffer));
        buffer.assign(3, 0xFF);
        buffer[0] = 0x7F;
        EXPECT_EQ(0x7FFFFF, ebml_int::decode_s(buffer));
        // 4 bytes
        buffer.assign(4, 0xFF);
        buffer[1] = 0x7F;
        EXPECT_EQ(-0x800001, ebml_int::decode_s(buffer));
        buffer.assign(4, 0);
        buffer[1] = 0x80;
        EXPECT_EQ(0x800000, ebml_int::decode_s(buffer));
        buffer.assign(4, 0);
        buffer[0] = 0x80;
        EXPECT_EQ(-0x80000000l, ebml_int::decode_s(buffer));
        buffer.assign(4, 0xFF);
        buffer[0] = 0x7F;
        EXPECT_EQ(0x7FFFFFFFl, ebml_int::decode_s(buffer));
        // 5 bytes
        buffer.assign(5, 0xFF);
        buffer[1] = 0x7F;
        EXPECT_EQ(-0x80000001l, ebml_int::decode_s(buffer));
        buffer.assign(5, 0);
        buffer[1] = 0x80;
        EXPECT_EQ(0x80000000l, ebml_int::decode_s(buffer));
        buffer.assign(5, 0);
        buffer[0] = 0x80;
        EXPECT_EQ(-0x8000000000l, ebml_int::decode_s(buffer));
        buffer.assign(5, 0xFF);
        buffer[0] = 0x7F;
        EXPECT_EQ(0x7FFFFFFFFFl, ebml_int::decode_s(buffer));
        // 6 bytes
        buffer.assign(6, 0xFF);
        buffer[1] = 0x7F;
        EXPECT_EQ(-0x8000000001l, ebml_int::decode_s(buffer));
        buffer.assign(6, 0);
        buffer[1] = 0x80;
        EXPECT_EQ(0x8000000000l, ebml_int::decode_s(buffer));
        buffer.assign(6, 0);
        buffer[0] = 0x80;
        EXPECT_EQ(-0x800000000000l, ebml_int::decode_s(buffer));
        buffer.assign(6, 0xFF);
        buffer[0] = 0x7F;
        EXPECT_EQ(0x7FFFFFFFFFFFl, ebml_int::decode_s(buffer));
        // 7 bytes
        buffer.assign(7, 0xFF);
        buffer[1] = 0x7F;
        EXPECT_EQ(-0x800000000001l, ebml_int::decode_s(buffer));
        buffer.assign(7, 0);
        buffer[1] = 0x80;
        EXPECT_EQ(0x800000000000l, ebml_int::decode_s(buffer));
        buffer.assign(7, 0);
        buffer[0] = 0x80;
        EXPECT_EQ(-0x80000000000000l, ebml_int::decode_s(buffer));
        buffer.assign(7, 0xFF);
        buffer[0] = 0x7F;
        EXPECT_EQ(0x7FFFFFFFFFFFFFl, ebml_int::decode_s(buffer));
        // 8 bytes
        buffer.assign(8, 0xFF);
        buffer[1] = 0x7F;
        EXPECT_EQ(-0x80000000000001l, ebml_int::decode_s(buffer));
        buffer.assign(8, 0);
        buffer[1] = 0x80;
        EXPECT_EQ(0x80000000000000l, ebml_int::decode_s(buffer));
        buffer.assign(8, 0);
        buffer[0] = 0x80;
        EXPECT_EQ(-0x8000000000000000l, ebml_int::decode_s(buffer));
        buffer.assign(8, 0xFF);
        buffer[0] = 0x7F;
        EXPECT_EQ(0x7FFFFFFFFFFFFFFFl, ebml_int::decode_s(buffer));
    }


    TEST(EBMLInteger, EncodeDecodeUnsigned)
    {
        // 0 bytes
        EXPECT_EQ(0, ebml_int::decode_u(ebml_int::encode_u(0)));
        // 1 byte
        EXPECT_EQ(1, ebml_int::decode_u(ebml_int::encode_u(1)));
        EXPECT_EQ(0x7F, ebml_int::decode_u(ebml_int::encode_u(0x7F)));
        EXPECT_EQ(0xFF, ebml_int::decode_u(ebml_int::encode_u(0xFF)));
        // 2 bytes
        EXPECT_EQ(0x100, ebml_int::decode_u(ebml_int::encode_u(0x100)));
        EXPECT_EQ(0x101, ebml_int::decode_u(ebml_int::encode_u(0x101)));
        EXPECT_EQ(0x7FFF,
                ebml_int::decode_u(ebml_int::encode_u(0x7FFF)));
        EXPECT_EQ(0xFFFF,
                ebml_int::decode_u(ebml_int::encode_u(0xFFFF)));
        // 3 bytes
        EXPECT_EQ(0x10000,
                ebml_int::decode_u(ebml_int::encode_u(0x10000)));
        EXPECT_EQ(0x10001,
                ebml_int::decode_u(ebml_int::encode_u(0x10001)));
        EXPECT_EQ(0x7FFFFF,
                ebml_int::decode_u(ebml_int::encode_u(0x7FFFFF)));
        EXPECT_EQ(0xFFFFFF,
                ebml_int::decode_u(ebml_int::encode_u(0xFFFFFF)));
        // 4 bytes
        EXPECT_EQ(0x1000000l,
                ebml_int::decode_u(ebml_int::encode_u(0x1000000l)));
        EXPECT_EQ(0x1000001l,
                ebml_int::decode_u(ebml_int::encode_u(0x1000001l)));
        EXPECT_EQ(0x7FFFFFFFl,
                ebml_int::decode_u(ebml_int::encode_u(0x7FFFFFFFl)));
        EXPECT_EQ(0xFFFFFFFFl,
                ebml_int::decode_u(ebml_int::encode_u(0xFFFFFFFFl)));
        // 5 bytes
        EXPECT_EQ(0x100000000l,
                ebml_int::decode_u(ebml_int::encode_u(0x100000000l)));
        EXPECT_EQ(0x100000001l,
                ebml_int::decode_u(ebml_int::encode_u(0x100000001l)));
        EXPECT_EQ(0x7FFFFFFFFFl,
                ebml_int::decode_u(ebml_int::encode_u(0x7FFFFFFFFFl)));
        EXPECT_EQ(0xFFFFFFFFFFl,
                ebml_int::decode_u(ebml_int::encode_u(0xFFFFFFFFFFl)));
        // 6 bytes
        EXPECT_EQ(0x10000000000l,
                ebml_int::decode_u(ebml_int::encode_u(0x10000000000l)));
        EXPECT_EQ(0x10000000001l,
                ebml_int::decode_u(ebml_int::encode_u(0x10000000001l)));
        EXPECT_EQ(0x7FFFFFFFFFFFl,
                ebml_int::decode_u(ebml_int::encode_u(0x7FFFFFFFFFFFl)));
        EXPECT_EQ(0xFFFFFFFFFFFFl,
                ebml_int::decode_u(ebml_int::encode_u(0xFFFFFFFFFFFFl)));
        // 7 bytes
        EXPECT_EQ(0x1000000000000l,
                ebml_int::decode_u(ebml_int::encode_u(0x1000000000000l)));
        EXPECT_EQ(0x1000000000001l,
                ebml_int::decode_u(ebml_int::encode_u(0x1000000000001l)));
        EXPECT_EQ(0x7FFFFFFFFFFFFFl,
                ebml_int::decode_u(ebml_int::encode_u(0x7FFFFFFFFFFFFFl)));
        EXPECT_EQ(0xFFFFFFFFFFFFFFl,
                ebml_int::decode_u(ebml_int::encode_u(0xFFFFFFFFFFFFFFl)));
        // 8 bytes
        EXPECT_EQ(0x100000000000000l,
                ebml_int::decode_u(ebml_int::encode_u(0x100000000000000l)));
        EXPECT_EQ(0x100000000000001l,
                ebml_int::decode_u(ebml_int::encode_u(0x100000000000001l)));
        EXPECT_EQ(0x7FFFFFFFFFFFFFFFl,
                ebml_int::decode_u(ebml_int::encode_u(0x7FFFFFFFFFFFFFFFl)));
        EXPECT_EQ(0xFFFFFFFFFFFFFFFFl,
                ebml_int::decode_u(ebml_int::encode_u(0xFFFFFFFFFFFFFFFFl)));
    }


    TEST(EBMLInteger, EncodeDecodeSigned)
    {
        // 0 bytes
        EXPECT_EQ(0, ebml_int::decode_s(ebml_int::encode_s(0)));
        // 1 byte
        EXPECT_EQ(-0x80, ebml_int::decode_s(ebml_int::encode_s(-0x80)));
        EXPECT_EQ(-1, ebml_int::decode_s(ebml_int::encode_s(-1)));
        EXPECT_EQ(0x7F, ebml_int::decode_s(ebml_int::encode_s(0x7F)));
        // 2 bytes
        EXPECT_EQ(-0x81, ebml_int::decode_s(ebml_int::encode_s(-0x81)));
        EXPECT_EQ(0x80, ebml_int::decode_s(ebml_int::encode_s(0x80)));
        EXPECT_EQ(-0x8000,
                ebml_int::decode_s(ebml_int::encode_s(-0x8000)));
        EXPECT_EQ(0x7FFF,
                ebml_int::decode_s(ebml_int::encode_s(0x7FFF)));
        // 3 bytes
        EXPECT_EQ(-0x8001,
                ebml_int::decode_s(ebml_int::encode_s(-0x8001)));
        EXPECT_EQ(0x8000,
                ebml_int::decode_s(ebml_int::encode_s(0x8000)));
        EXPECT_EQ(-0x800000,
                ebml_int::decode_s(ebml_int::encode_s(-0x800000)));
        EXPECT_EQ(0x7FFFFF,
                ebml_int::decode_s(ebml_int::encode_s(0x7FFFFF)));
        // 4 bytes
        EXPECT_EQ(-0x800001l,
                ebml_int::decode_s(ebml_int::encode_s(-0x800001l)));
        EXPECT_EQ(0x800000l,
                ebml_int::decode_s(ebml_int::encode_s(0x800000l)));
        EXPECT_EQ(-0x80000000l,
                ebml_int::decode_s(ebml_int::encode_s(-0x80000000l)));
        EXPECT_EQ(0x7FFFFFFFl,
                ebml_int::decode_s(ebml_int::encode_s(0x7FFFFFFFl)));
        // 5 bytes
        EXPECT_EQ(-0x80000001l,
                ebml_int::decode_s(ebml_int::encode_s(-0x80000001l)));
        EXPECT_EQ(0x80000000l,
                ebml_int::decode_s(ebml_int::encode_s(0x80000000l)));
        EXPECT_EQ(-0x8000000000l,
                ebml_int::decode_s(ebml_int::encode_s(-0x8000000000l)));
        EXPECT_EQ(0x7FFFFFFFFFl,
                ebml_int::decode_s(ebml_int::encode_s(0x7FFFFFFFFFl)));
        // 6 bytes
        EXPECT_EQ(-0x8000000001l,
                ebml_int::decode_s(ebml_int::encode_s(-0x8000000001l)));
        EXPECT_EQ(0x8000000000l,
                ebml_int::decode_s(ebml_int::encode_s(0x8000000000l)));
        EXPECT_EQ(-0x800000000000l,
                ebml_int::decode_s(ebml_int::encode_s(-0x800000000000l)));
        EXPECT_EQ(0x7FFFFFFFFFFFl,
                ebml_int::decode_s(ebml_int::encode_s(0x7FFFFFFFFFFFl)));
        // 7 bytes
        EXPECT_EQ(-0x800000000001l,
                ebml_int::decode_s(ebml_int::encode_s(-0x800000000001l)));
        EXPECT_EQ(0x800000000000l,
                ebml_int::decode_s(ebml_int::encode_s(0x800000000000l)));
        EXPECT_EQ(-0x80000000000000l,
                ebml_int::decode_s(ebml_int::encode_s(-0x80000000000000l)));
        EXPECT_EQ(0x7FFFFFFFFFFFFFl,
                ebml_int::decode_s(ebml_int::encode_s(0x7FFFFFFFFFFFFFl)));
        // 8 bytes
        EXPECT_EQ(-0x80000000000001l,
                ebml_int::decode_s(ebml_int::encode_s(-0x80000000000001l)));
        EXPECT_EQ(0x80000000000000l,
                ebml_int::decode_s(ebml_int::encode_s(0x80000000000000l)));
        EXPECT_EQ(-0x8000000000000000l,
                ebml_int::decode_s(ebml_int::encode_s(-0x8000000000000000l)));
        EXPECT_EQ(0x7FFFFFFFFFFFFFFFl,
                ebml_int::decode_s(ebml_int::encode_s(0x7FFFFFFFFFFFFFFFl)));
    }


    void fill_buffer(std::stringstream& buf, uint8_t c, std::streamsize n)
    {
        for (std::streamsize ii(0); ii < n; ++ii)
        {
            buf.put(c);
        }
    }


    TEST(EBMLIntegerStream, WriteUnsigned)
    {
        std::ostringstream buffer;
        std::string expected;
        expected.reserve(8);
        // 0 bytes
        expected.push_back(0xFF);
        buffer.put(0xFF);
        // This is checking that the buffer is untouched for zero
        EXPECT_EQ(0, ebml_int::write_u(0x00, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        // 1 byte
        buffer.str(std::string());
        expected[0] = 0x01;
        EXPECT_EQ(1, ebml_int::write_u(0x01, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected[0] = 0x7F;
        EXPECT_EQ(1, ebml_int::write_u(0x7F, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected[0] = 0xFF;
        EXPECT_EQ(1, ebml_int::write_u(0xFF, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        // 2 bytes
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 2, 0x00);
        expected[0] = 0x01;
        EXPECT_EQ(2, ebml_int::write_u(0x0100, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected[0] = 0x01; expected[1] = 0x01;
        EXPECT_EQ(2, ebml_int::write_u(0x0101, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 2, 0xFF);
        expected[0] = 0x7F;
        EXPECT_EQ(2, ebml_int::write_u(0x7FFF, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected[0] = 0xFF; expected[1] = 0xFF;
        EXPECT_EQ(2, ebml_int::write_u(0xFFFF, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        // 3 bytes
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 3, 0x00);
        expected[0] = 0x01;
        EXPECT_EQ(3, ebml_int::write_u(0x010000, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected[0] = 0x01; expected[2] = 0x01;
        EXPECT_EQ(3, ebml_int::write_u(0x010001, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 3, 0xFF);
        expected[0] = 0x7F;
        EXPECT_EQ(3, ebml_int::write_u(0x7FFFFF, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected[0] = 0xFF;
        EXPECT_EQ(3, ebml_int::write_u(0xFFFFFF, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        // 4 bytes
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 4, 0x00);
        expected[0] = 0x01;
        EXPECT_EQ(4, ebml_int::write_u(0x01000000, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected[0] = 0x01; expected[3] = 0x01;
        EXPECT_EQ(4, ebml_int::write_u(0x01000001, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 4, 0xFF);
        expected[0] = 0x7F;
        EXPECT_EQ(4, ebml_int::write_u(0x7FFFFFFF, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected[0] = 0xFF;
        EXPECT_EQ(4, ebml_int::write_u(0xFFFFFFFF, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        // 5 bytes
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 5, 0x00);
        expected[0] = 0x01;
        EXPECT_EQ(5, ebml_int::write_u(0x0100000000l, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected[0] = 0x01; expected[4] = 0x01;
        EXPECT_EQ(5, ebml_int::write_u(0x0100000001l, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 5, 0xFF);
        expected[0] = 0x7F;
        EXPECT_EQ(5, ebml_int::write_u(0x7FFFFFFFFFl, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected[0] = 0xFF;
        EXPECT_EQ(5, ebml_int::write_u(0xFFFFFFFFFFl, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        // 6 bytes
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 6, 0x00);
        expected[0] = 0x01;
        EXPECT_EQ(6, ebml_int::write_u(0x010000000000l, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected[0] = 0x01; expected[5] = 0x01;
        EXPECT_EQ(6, ebml_int::write_u(0x010000000001l, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 6, 0xFF);
        expected[0] = 0x7F;
        EXPECT_EQ(6, ebml_int::write_u(0x7FFFFFFFFFFFl, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected[0] = 0xFF;
        EXPECT_EQ(6, ebml_int::write_u(0xFFFFFFFFFFFFl, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        // 7 bytes
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 7, 0x00);
        expected[0] = 0x01;
        EXPECT_EQ(7, ebml_int::write_u(0x01000000000000l, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected[0] = 0x01; expected[6] = 0x01;
        EXPECT_EQ(7, ebml_int::write_u(0x01000000000001l, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 7, 0xFF);
        expected[0] = 0x7F;
        EXPECT_EQ(7, ebml_int::write_u(0x7FFFFFFFFFFFFFl, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected[0] = 0xFF;
        EXPECT_EQ(7, ebml_int::write_u(0xFFFFFFFFFFFFFFl, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        // 8 bytes
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 8, 0x00);
        expected[0] = 0x01;
        EXPECT_EQ(8, ebml_int::write_u(0x0100000000000000l, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected[0] = 0x01; expected[7] = 0x01;
        EXPECT_EQ(8, ebml_int::write_u(0x0100000000000001l, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 8, 0xFF);
        expected[0] = 0x7F;
        EXPECT_EQ(8, ebml_int::write_u(0x7FFFFFFFFFFFFFFFl, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected[0] = 0xFF;
        EXPECT_EQ(8, ebml_int::write_u(0xFFFFFFFFFFFFFFFFl, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    }


    TEST(EBMLIntegerStream, WriteSigned)
    {
        std::ostringstream buffer;
        std::string expected;
        expected.reserve(8);
        // 0 bytes
        expected.push_back(0xFF);
        buffer.put(0xFF);
        // This is checking that the buffer is untouched for zero
        EXPECT_EQ(0, ebml_int::write_s(0, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        // 1 byte
        buffer.str(std::string());
        expected[0] = 0xFF;
        EXPECT_EQ(1, ebml_int::write_s(-1, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected[0] = 0x01;
        EXPECT_EQ(1, ebml_int::write_s(1, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected[0] = 0x80;
        EXPECT_EQ(1, ebml_int::write_s(-0x80, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected[0] = 0x7F;
        EXPECT_EQ(1, ebml_int::write_s(0x7F, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        // 2 bytes
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 2, 0xFF);
        expected[1] = 0x7F;
        EXPECT_EQ(2, ebml_int::write_s(-0x81, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 2, 0x00);
        expected[1] = 0x80;
        EXPECT_EQ(2, ebml_int::write_s(0x80, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected[0] = 0x80; expected[1] = 0x00;
        EXPECT_EQ(2, ebml_int::write_s(-0x8000, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected[0] = 0x7F; expected[1] = 0xFF;
        EXPECT_EQ(2, ebml_int::write_s(0x7FFF, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        // 3 bytes
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 3, 0xFF);
        expected[1] = 0x7F;
        EXPECT_EQ(3, ebml_int::write_s(-0x8001, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 3, 0x00);
        expected[1] = 0x80;
        EXPECT_EQ(3, ebml_int::write_s(0x8000, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 3, 0x00);
        expected[0] = 0x80;
        EXPECT_EQ(3, ebml_int::write_s(-0x800000, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 3, 0xFF);
        expected[0] = 0x7F;
        EXPECT_EQ(3, ebml_int::write_s(0x7FFFFF, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        // 4 bytes
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 4, 0xFF);
        expected[1] = 0x7F;
        EXPECT_EQ(4, ebml_int::write_s(-0x800001, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 4, 0x00);
        expected[1] = 0x80;
        EXPECT_EQ(4, ebml_int::write_s(0x800000, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 4, 0x00);
        expected[0] = 0x80;
        EXPECT_EQ(4, ebml_int::write_s(-0x80000000l, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 4, 0xFF);
        expected[0] = 0x7F;
        EXPECT_EQ(4, ebml_int::write_s(0x7FFFFFFFl, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        // 5 bytes
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 5, 0xFF);
        expected[1] = 0x7F;
        EXPECT_EQ(5, ebml_int::write_s(-0x80000001l, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 5, 0x00);
        expected[1] = 0x80;
        EXPECT_EQ(5, ebml_int::write_s(0x80000000l, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 5, 0x00);
        expected[0] = 0x80;
        EXPECT_EQ(5, ebml_int::write_s(-0x8000000000l, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 5, 0xFF);
        expected[0] = 0x7F;
        EXPECT_EQ(5, ebml_int::write_s(0x7FFFFFFFFFl, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        // 6 bytes
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 6, 0xFF);
        expected[1] = 0x7F;
        EXPECT_EQ(6, ebml_int::write_s(-0x8000000001l, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 6, 0x00);
        expected[1] = 0x80;
        EXPECT_EQ(6, ebml_int::write_s(0x8000000000l, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 6, 0x00);
        expected[0] = 0x80;
        EXPECT_EQ(6, ebml_int::write_s(-0x800000000000l, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 6, 0xFF);
        expected[0] = 0x7F;
        EXPECT_EQ(6, ebml_int::write_s(0x7FFFFFFFFFFFl, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        // 7 bytes
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 7, 0xFF);
        expected[1] = 0x7F;
        EXPECT_EQ(7, ebml_int::write_s(-0x800000000001l, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 7, 0x00);
        expected[1] = 0x80;
        EXPECT_EQ(7, ebml_int::write_s(0x800000000000l, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 7, 0x00);
        expected[0] = 0x80;
        EXPECT_EQ(7, ebml_int::write_s(-0x80000000000000l, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 7, 0xFF);
        expected[0] = 0x7F;
        EXPECT_EQ(7, ebml_int::write_s(0x7FFFFFFFFFFFFFl, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        // 8 bytes
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 8, 0xFF);
        expected[1] = 0x7F;
        EXPECT_EQ(8, ebml_int::write_s(-0x80000000000001l, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 8, 0x00);
        expected[1] = 0x80;
        EXPECT_EQ(8, ebml_int::write_s(0x80000000000000l, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 8, 0x00);
        expected[0] = 0x80;
        EXPECT_EQ(8, ebml_int::write_s(-0x8000000000000000l, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
        buffer.str(std::string());
        expected.replace(expected.begin(), expected.end(), 8, 0xFF);
        expected[0] = 0x7F;
        EXPECT_EQ(8, ebml_int::write_s(0x7FFFFFFFFFFFFFFFl, buffer));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected, buffer.str());
    }


    TEST(EBMLIntegerStream, ReadUnsigned)
    {
        std::stringstream buffer;
        // 0 bytes
        EXPECT_EQ(0, ebml_int::read_u(buffer, 0));
        // 1 byte
        buffer.put(0x01);
        EXPECT_EQ(1, ebml_int::read_u(buffer, 1));
        buffer.put(0x7F);
        EXPECT_EQ(0x7F, ebml_int::read_u(buffer, 1));
        buffer.put(0xFF);
        EXPECT_EQ(0xFF, ebml_int::read_u(buffer, 1));
        // 2 bytes
        buffer.put(0x01); buffer.put(0x00);
        EXPECT_EQ(0x0100, ebml_int::read_u(buffer, 2));
        buffer.put(0x01); buffer.put(0x01);
        EXPECT_EQ(0x0101, ebml_int::read_u(buffer, 2));
        buffer.put(0x7F); buffer.put(0xFF);
        EXPECT_EQ(0x7FFF, ebml_int::read_u(buffer, 2));
        buffer.put(0xFF); buffer.put(0xFF);
        EXPECT_EQ(0xFFFF, ebml_int::read_u(buffer, 2));
        // 3 bytes
        buffer.put(0x01); fill_buffer(buffer, 0x00, 2);
        EXPECT_EQ(0x010000, ebml_int::read_u(buffer, 3));
        buffer.put(0x01); buffer.put(0x00); buffer.put(0x01);
        EXPECT_EQ(0x010001, ebml_int::read_u(buffer, 3));
        buffer.put(0x7F); buffer.put(0xFF); buffer.put(0xFF);
        EXPECT_EQ(0x7FFFFF, ebml_int::read_u(buffer, 3));
        buffer.put(0xFF); buffer.put(0xFF); buffer.put(0xFF);;
        EXPECT_EQ(0xFFFFFF, ebml_int::read_u(buffer, 3));
        // 4 bytes
        buffer.put(0x01); fill_buffer(buffer, 0x00, 3);
        EXPECT_EQ(0x01000000, ebml_int::read_u(buffer, 4));
        buffer.put(0x01); fill_buffer(buffer, 0x00, 2); buffer.put(0x01);
        EXPECT_EQ(0x01000001, ebml_int::read_u(buffer, 4));
        buffer.put(0x7F); fill_buffer(buffer, 0xFF, 3);
        EXPECT_EQ(0x7FFFFFFF, ebml_int::read_u(buffer, 4));
        fill_buffer(buffer, 0xFF, 4);
        EXPECT_EQ(0xFFFFFFFF, ebml_int::read_u(buffer, 4));
        // 5 bytes
        buffer.put(0x01); fill_buffer(buffer, 0x00, 4);
        EXPECT_EQ(0x0100000000, ebml_int::read_u(buffer, 5));
        buffer.put(0x01); fill_buffer(buffer, 0x00, 3); buffer.put(0x01);
        EXPECT_EQ(0x0100000001, ebml_int::read_u(buffer, 5));
        buffer.put(0x7F); fill_buffer(buffer, 0xFF, 4);
        EXPECT_EQ(0x7FFFFFFFFF, ebml_int::read_u(buffer, 5));
        fill_buffer(buffer, 0xFF, 5);
        EXPECT_EQ(0xFFFFFFFFFF, ebml_int::read_u(buffer, 5));
        // 6 bytes
        buffer.put(0x01); fill_buffer(buffer, 0x00, 5);
        EXPECT_EQ(0x010000000000, ebml_int::read_u(buffer, 6));
        buffer.put(0x01); fill_buffer(buffer, 0x00, 4); buffer.put(0x01);
        EXPECT_EQ(0x010000000001, ebml_int::read_u(buffer, 6));
        buffer.put(0x7F); fill_buffer(buffer, 0xFF, 5);
        EXPECT_EQ(0x7FFFFFFFFFFF, ebml_int::read_u(buffer, 6));
        fill_buffer(buffer, 0xFF, 6);
        EXPECT_EQ(0xFFFFFFFFFFFF, ebml_int::read_u(buffer, 6));
        // 7 bytes
        buffer.put(0x01); fill_buffer(buffer, 0x00, 6);
        EXPECT_EQ(0x01000000000000, ebml_int::read_u(buffer, 7));
        buffer.put(0x01); fill_buffer(buffer, 0x00, 5); buffer.put(0x01);
        EXPECT_EQ(0x01000000000001, ebml_int::read_u(buffer, 7));
        buffer.put(0x7F); fill_buffer(buffer, 0xFF, 6);
        EXPECT_EQ(0x7FFFFFFFFFFFFF, ebml_int::read_u(buffer, 7));
        fill_buffer(buffer, 0xFF, 7);
        EXPECT_EQ(0xFFFFFFFFFFFFFF, ebml_int::read_u(buffer, 7));
        // 8 bytes
        buffer.put(0x01); fill_buffer(buffer, 0x00, 7);
        EXPECT_EQ(0x0100000000000000, ebml_int::read_u(buffer, 8));
        buffer.put(0x01); fill_buffer(buffer, 0x00, 6); buffer.put(0x01);
        EXPECT_EQ(0x0100000000000001, ebml_int::read_u(buffer, 8));
        buffer.put(0x7F); fill_buffer(buffer, 0xFF, 7);
        EXPECT_EQ(0x7FFFFFFFFFFFFFFF, ebml_int::read_u(buffer, 8));
        fill_buffer(buffer, 0xFF, 8);
        EXPECT_EQ(0xFFFFFFFFFFFFFFFF, ebml_int::read_u(buffer, 8));
    }


    TEST(EBMLIntegerStream, ReadSigned)
    {
        std::stringstream buffer;
        // 0 bytes
        EXPECT_EQ(0, ebml_int::read_s(buffer, 0));
        // 1 byte
        buffer.put(0x80);
        EXPECT_EQ(-0x80, ebml_int::read_s(buffer, 1));
        buffer.put(0xFF);
        EXPECT_EQ(-1, ebml_int::read_s(buffer, 1));
        buffer.put(0x7F);
        EXPECT_EQ(0x7F, ebml_int::read_s(buffer, 1));
        // 2 bytes
        buffer.put(0xFF); buffer.put(0x7F);
        EXPECT_EQ(-0x81, ebml_int::read_s(buffer, 2));
        buffer.put(0x00); buffer.put(0x80);
        EXPECT_EQ(0x80, ebml_int::read_s(buffer, 2));
        buffer.put(0x80); buffer.put(0x00);
        EXPECT_EQ(-0x8000, ebml_int::read_s(buffer, 2));
        buffer.put(0x7F); buffer.put(0xFF);
        EXPECT_EQ(0x7FFF, ebml_int::read_s(buffer, 2));
        // 3 bytes
        buffer.put(0xFF); buffer.put(0x7F); buffer.put(0xFF);
        EXPECT_EQ(-0x8001, ebml_int::read_s(buffer, 3));
        buffer.put(0x00); buffer.put(0x80); buffer.put(0x00);
        EXPECT_EQ(0x8000, ebml_int::read_s(buffer, 3));
        buffer.put(0x80); fill_buffer(buffer, 0x00, 2);
        EXPECT_EQ(-0x800000, ebml_int::read_s(buffer, 3));
        buffer.put(0x7F); fill_buffer(buffer, 0xFF, 2);
        EXPECT_EQ(0x7FFFFF, ebml_int::read_s(buffer, 3));
        // 4 bytes
        buffer.put(0xFF); buffer.put(0x7F); fill_buffer(buffer, 0xFF, 2);
        EXPECT_EQ(-0x800001, ebml_int::read_s(buffer, 4));
        buffer.put(0x00); buffer.put(0x80); fill_buffer(buffer, 0x00, 2);
        EXPECT_EQ(0x800000, ebml_int::read_s(buffer, 4));
        buffer.put(0x80); fill_buffer(buffer, 0x00, 3);
        EXPECT_EQ(-0x80000000l, ebml_int::read_s(buffer, 4));
        buffer.put(0x7F); fill_buffer(buffer, 0xFF, 3);
        EXPECT_EQ(0x7FFFFFFFl, ebml_int::read_s(buffer, 4));
        // 5 bytes
        buffer.put(0xFF); buffer.put(0x7F); fill_buffer(buffer, 0xFF, 3);
        EXPECT_EQ(-0x80000001l, ebml_int::read_s(buffer, 5));
        buffer.put(0x00); buffer.put(0x80); fill_buffer(buffer, 0x00, 3);
        EXPECT_EQ(0x80000000l, ebml_int::read_s(buffer, 5));
        buffer.put(0x80); fill_buffer(buffer, 0x00, 4);
        EXPECT_EQ(-0x8000000000l, ebml_int::read_s(buffer, 5));
        buffer.put(0x7F); fill_buffer(buffer, 0xFF, 4);
        EXPECT_EQ(0x7FFFFFFFFFl, ebml_int::read_s(buffer, 5));
        // 6 bytes
        buffer.put(0xFF); buffer.put(0x7F); fill_buffer(buffer, 0xFF, 4);
        EXPECT_EQ(-0x8000000001l, ebml_int::read_s(buffer, 6));
        buffer.put(0x00); buffer.put(0x80); fill_buffer(buffer, 0x00, 4);
        EXPECT_EQ(0x8000000000l, ebml_int::read_s(buffer, 6));
        buffer.put(0x80); fill_buffer(buffer, 0x00, 5);
        EXPECT_EQ(-0x800000000000l, ebml_int::read_s(buffer, 6));
        buffer.put(0x7F); fill_buffer(buffer, 0xFF, 5);
        EXPECT_EQ(0x7FFFFFFFFFFFl, ebml_int::read_s(buffer, 6));
        // 7 bytes
        buffer.put(0xFF); buffer.put(0x7F); fill_buffer(buffer, 0xFF, 5);
        EXPECT_EQ(-0x800000000001l, ebml_int::read_s(buffer, 7));
        buffer.put(0x00); buffer.put(0x80); fill_buffer(buffer, 0x00, 5);
        EXPECT_EQ(0x800000000000l, ebml_int::read_s(buffer, 7));
        buffer.put(0x80); fill_buffer(buffer, 0x00, 6);
        EXPECT_EQ(-0x80000000000000l, ebml_int::read_s(buffer, 7));
        buffer.put(0x7F); fill_buffer(buffer, 0xFF, 6);
        EXPECT_EQ(0x7FFFFFFFFFFFFFl, ebml_int::read_s(buffer, 7));
        // 8 bytes
        buffer.put(0xFF); buffer.put(0x7F); fill_buffer(buffer, 0xFF, 6);
        EXPECT_EQ(-0x80000000000001l, ebml_int::read_s(buffer, 8));
        buffer.put(0x00); buffer.put(0x80); fill_buffer(buffer, 0x00, 6);
        EXPECT_EQ(0x80000000000000l, ebml_int::read_s(buffer, 8));
        buffer.put(0x80); fill_buffer(buffer, 0x00, 7);
        EXPECT_EQ(-0x8000000000000000l, ebml_int::read_s(buffer, 8));
        buffer.put(0x7F); fill_buffer(buffer, 0xFF, 7);
        EXPECT_EQ(0x7FFFFFFFFFFFFFFFl, ebml_int::read_s(buffer, 8));
    }


    TEST(EBMLIntegerStream, WriteReadUnsigned)
    {
        std::stringstream buffer;
        // 0 bytes
        ebml_int::write_u(0, buffer);
        EXPECT_EQ(0, ebml_int::read_u(buffer, 0));
        // 1 byte
        ebml_int::write_u(1, buffer);
        EXPECT_EQ(1, ebml_int::read_u(buffer, 1));
        ebml_int::write_u(0x7F, buffer);
        EXPECT_EQ(0x7F, ebml_int::read_u(buffer, 1));
        ebml_int::write_u(0xFF, buffer);
        EXPECT_EQ(0xFF, ebml_int::read_u(buffer, 1));
        // 2 bytes
        ebml_int::write_u(0x100, buffer);
        EXPECT_EQ(0x100, ebml_int::read_u(buffer, 2));
        ebml_int::write_u(0x101, buffer);
        EXPECT_EQ(0x101, ebml_int::read_u(buffer, 2));
        ebml_int::write_u(0x7FFF, buffer);
        EXPECT_EQ(0x7FFF, ebml_int::read_u(buffer, 2));
        ebml_int::write_u(0xFFFF, buffer);
        EXPECT_EQ(0xFFFF, ebml_int::read_u(buffer, 2));
        // 3 bytes
        ebml_int::write_u(0x10000, buffer);
        EXPECT_EQ(0x10000, ebml_int::read_u(buffer, 3));
        ebml_int::write_u(0x10001, buffer);
        EXPECT_EQ(0x10001, ebml_int::read_u(buffer, 3));
        ebml_int::write_u(0x7FFFFF, buffer);
        EXPECT_EQ(0x7FFFFF, ebml_int::read_u(buffer, 3));
        ebml_int::write_u(0xFFFFFF, buffer);
        EXPECT_EQ(0xFFFFFF, ebml_int::read_u(buffer, 3));
        // 4 bytes
        ebml_int::write_u(0x1000000l, buffer);
        EXPECT_EQ(0x1000000l, ebml_int::read_u(buffer, 4));
        ebml_int::write_u(0x1000001l, buffer);
        EXPECT_EQ(0x1000001l, ebml_int::read_u(buffer, 4));
        ebml_int::write_u(0x7FFFFFFFl, buffer);
        EXPECT_EQ(0x7FFFFFFFl, ebml_int::read_u(buffer, 4));
        ebml_int::write_u(0xFFFFFFFFl, buffer);
        EXPECT_EQ(0xFFFFFFFFl, ebml_int::read_u(buffer, 4));
        // 5 bytes
        ebml_int::write_u(0x100000000l, buffer);
        EXPECT_EQ(0x100000000l, ebml_int::read_u(buffer, 5));
        ebml_int::write_u(0x100000001l, buffer);
        EXPECT_EQ(0x100000001l, ebml_int::read_u(buffer, 5));
        ebml_int::write_u(0x7FFFFFFFFFl, buffer);
        EXPECT_EQ(0x7FFFFFFFFFl, ebml_int::read_u(buffer, 5));
        ebml_int::write_u(0xFFFFFFFFFFl, buffer);
        EXPECT_EQ(0xFFFFFFFFFFl, ebml_int::read_u(buffer, 5));
        // 6 bytes
        ebml_int::write_u(0x10000000000l, buffer);
        EXPECT_EQ(0x10000000000l, ebml_int::read_u(buffer, 6));
        ebml_int::write_u(0x10000000001l, buffer);
        EXPECT_EQ(0x10000000001l, ebml_int::read_u(buffer, 6));
        ebml_int::write_u(0x7FFFFFFFFFFFl, buffer);
        EXPECT_EQ(0x7FFFFFFFFFFFl, ebml_int::read_u(buffer, 6));
        ebml_int::write_u(0xFFFFFFFFFFFFl, buffer);
        EXPECT_EQ(0xFFFFFFFFFFFFl, ebml_int::read_u(buffer, 6));
        // 7 bytes
        ebml_int::write_u(0x1000000000000l, buffer);
        EXPECT_EQ(0x1000000000000l, ebml_int::read_u(buffer, 7));
        ebml_int::write_u(0x1000000000001l, buffer);
        EXPECT_EQ(0x1000000000001l, ebml_int::read_u(buffer, 7));
        ebml_int::write_u(0x7FFFFFFFFFFFFFl, buffer);
        EXPECT_EQ(0x7FFFFFFFFFFFFFl, ebml_int::read_u(buffer, 7));
        ebml_int::write_u(0xFFFFFFFFFFFFFFl, buffer);
        EXPECT_EQ(0xFFFFFFFFFFFFFFl, ebml_int::read_u(buffer, 7));
        // 8 bytes
        ebml_int::write_u(0x100000000000000l, buffer);
        EXPECT_EQ(0x100000000000000l, ebml_int::read_u(buffer, 8));
        ebml_int::write_u(0x100000000000001l, buffer);
        EXPECT_EQ(0x100000000000001l, ebml_int::read_u(buffer, 8));
        ebml_int::write_u(0x7FFFFFFFFFFFFFFFl, buffer);
        EXPECT_EQ(0x7FFFFFFFFFFFFFFFl, ebml_int::read_u(buffer, 8));
        ebml_int::write_u(0xFFFFFFFFFFFFFFFFl, buffer);
        EXPECT_EQ(0xFFFFFFFFFFFFFFFFl, ebml_int::read_u(buffer, 8));
    }


    TEST(EBMLIntegerStream, WriteReadSigned)
    {
        std::stringstream buffer;
        // 0 bytes
        ebml_int::write_s(0, buffer);
        EXPECT_EQ(0, ebml_int::read_s(buffer, 0));
        // 1 byte
        ebml_int::write_s(-0x80, buffer);
        EXPECT_EQ(-0x80, ebml_int::read_s(buffer, 1));
        ebml_int::write_s(-1, buffer);
        EXPECT_EQ(-1, ebml_int::read_s(buffer, 1));
        ebml_int::write_s(0x7F, buffer);
        EXPECT_EQ(0x7F, ebml_int::read_s(buffer, 1));
        // 2 bytes
        ebml_int::write_s(-0x81, buffer);
        EXPECT_EQ(-0x81, ebml_int::read_s(buffer, 2));
        ebml_int::write_s(0x80, buffer);
        EXPECT_EQ(0x80, ebml_int::read_s(buffer, 2));
        ebml_int::write_s(-0x8000, buffer);
        EXPECT_EQ(-0x8000, ebml_int::read_s(buffer, 2));
        ebml_int::write_s(0x7FFF, buffer);
        EXPECT_EQ(0x7FFF, ebml_int::read_s(buffer, 2));
        // 3 bytes
        ebml_int::write_s(-0x8001, buffer);
        EXPECT_EQ(-0x8001, ebml_int::read_s(buffer, 3));
        ebml_int::write_s(0x8000, buffer);
        EXPECT_EQ(0x8000, ebml_int::read_s(buffer, 3));
        ebml_int::write_s(-0x800000, buffer);
        EXPECT_EQ(-0x800000, ebml_int::read_s(buffer, 3));
        ebml_int::write_s(0x7FFFFF, buffer);
        EXPECT_EQ(0x7FFFFF, ebml_int::read_s(buffer, 3));
        // 4 bytes
        ebml_int::write_s(-0x800001l, buffer);
        EXPECT_EQ(-0x800001l, ebml_int::read_s(buffer, 4));
        ebml_int::write_s(0x800000l, buffer);
        EXPECT_EQ(0x800000l, ebml_int::read_s(buffer, 4));
        ebml_int::write_s(-0x80000000l, buffer);
        EXPECT_EQ(-0x80000000l, ebml_int::read_s(buffer, 4));
        ebml_int::write_s(0x7FFFFFFFl, buffer);
        EXPECT_EQ(0x7FFFFFFFl, ebml_int::read_s(buffer, 4));
        // 5 bytes
        ebml_int::write_s(-0x80000001l, buffer);
        EXPECT_EQ(-0x80000001l, ebml_int::read_s(buffer, 5));
        ebml_int::write_s(0x80000000l, buffer);
        EXPECT_EQ(0x80000000l, ebml_int::read_s(buffer, 5));
        ebml_int::write_s(-0x8000000000l, buffer);
        EXPECT_EQ(-0x8000000000l, ebml_int::read_s(buffer, 5));
        ebml_int::write_s(0x7FFFFFFFFFl, buffer);
        EXPECT_EQ(0x7FFFFFFFFFl, ebml_int::read_s(buffer, 5));
        // 6 bytes
        ebml_int::write_s(-0x8000000001l, buffer);
        EXPECT_EQ(-0x8000000001l, ebml_int::read_s(buffer, 6));
        ebml_int::write_s(0x8000000000l, buffer);
        EXPECT_EQ(0x8000000000l, ebml_int::read_s(buffer, 6));
        ebml_int::write_s(-0x800000000000l, buffer);
        EXPECT_EQ(-0x800000000000l, ebml_int::read_s(buffer, 6));
        ebml_int::write_s(0x7FFFFFFFFFFFl, buffer);
        EXPECT_EQ(0x7FFFFFFFFFFFl, ebml_int::read_s(buffer, 6));
        // 7 bytes
        ebml_int::write_s(-0x800000000001l, buffer);
        EXPECT_EQ(-0x800000000001l, ebml_int::read_s(buffer, 7));
        ebml_int::write_s(0x800000000000l, buffer);
        EXPECT_EQ(0x800000000000l, ebml_int::read_s(buffer, 7));
        ebml_int::write_s(-0x80000000000000l, buffer);
        EXPECT_EQ(-0x80000000000000l, ebml_int::read_s(buffer, 7));
        ebml_int::write_s(0x7FFFFFFFFFFFFFl, buffer);
        EXPECT_EQ(0x7FFFFFFFFFFFFFl, ebml_int::read_s(buffer, 7));
        // 8 bytes
        ebml_int::write_s(-0x80000000000001l, buffer);
        EXPECT_EQ(-0x80000000000001l, ebml_int::read_s(buffer, 8));
        ebml_int::write_s(0x80000000000000l, buffer);
        EXPECT_EQ(0x80000000000000l, ebml_int::read_s(buffer, 8));
        ebml_int::write_s(-0x8000000000000000l, buffer);
        EXPECT_EQ(-0x8000000000000000l, ebml_int::read_s(buffer, 8));
        ebml_int::write_s(0x7FFFFFFFFFFFFFFFl, buffer);
        EXPECT_EQ(0x7FFFFFFFFFFFFFFFl, ebml_int::read_s(buffer, 8));
    }


    TEST(EBMLIntegerStream, ReadError)
    {
        std::stringstream buffer;
        EXPECT_THROW(ebml_int::read_s(buffer, 8), ReadError);
        buffer.put(0x7F); buffer.put(0x29);
        EXPECT_THROW(ebml_int::read_s(buffer, 8), ReadError);
    }
}; // namespace test_ebml_integer

