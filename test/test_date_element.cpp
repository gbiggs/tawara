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
#include <tide/date_element.h>
#include <tide/ebml_integer.h>
#include <tide/exceptions.h>
#include <tide/tide_config.h>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "test_utilities.h"

using namespace tide;
namespace bpt = boost::posix_time;
namespace bgr = boost::gregorian;

namespace test_date_el
{
    bpt::ptime mindt(bpt::min_date_time);
    bpt::ptime maxdt(bpt::max_date_time);
    bpt::ptime ninf(bpt::neg_infin);
    bpt::ptime pinf(bpt::pos_infin);

    // Three dates each an hour apart
    bpt::ptime early(bgr::date(2010, bgr::Jan, 1),
            bpt::time_duration(0, 0, 0));
    bpt::ptime mid(bgr::date(2010, bgr::Jan, 1), bpt::time_duration(1, 0, 0));
    bpt::ptime late(bgr::date(2010, bgr::Jan, 1), bpt::time_duration(2, 0, 0));

    ///////////////////////////////////////////////////////////////////////////
    // DateElement interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST(DateElement, Construction)
    {
        EXPECT_EQ(ids::Null, DateElement(ids::Null, bpt::pos_infin).id());
        EXPECT_EQ(maxdt,
                DateElement(ids::Null, bpt::max_date_time).value());
        EXPECT_EQ(maxdt,
                DateElement(ids::Null, bpt::max_date_time,
                    bpt::min_date_time).value());
        EXPECT_EQ(mindt,
                DateElement(ids::Null, bpt::max_date_time,
                    bpt::min_date_time).get_default());
        EXPECT_FALSE(DateElement(ids::Null,
                    bpt::max_date_time).has_default());
        EXPECT_TRUE(DateElement(ids::Null, bpt::max_date_time,
                    bpt::min_date_time).has_default());

        EXPECT_THROW(DateElement(0x00, bpt::pos_infin), InvalidElementID);
        EXPECT_THROW(DateElement(0xFF, bpt::pos_infin), InvalidElementID);
        EXPECT_THROW(DateElement(0xFFFF, bpt::pos_infin), InvalidElementID);
        EXPECT_THROW(DateElement(0xFFFFFF, bpt::pos_infin), InvalidElementID);
        EXPECT_THROW(DateElement(0xFFFFFFFF, bpt::pos_infin),
                InvalidElementID);

        EXPECT_THROW(DateElement(0x00, bpt::pos_infin, bpt::neg_infin),
                InvalidElementID);
        EXPECT_THROW(DateElement(0xFF, bpt::pos_infin, bpt::neg_infin),
                InvalidElementID);
        EXPECT_THROW(DateElement(0xFFFF, bpt::pos_infin, bpt::neg_infin),
                InvalidElementID);
        EXPECT_THROW(DateElement(0xFFFFFF, bpt::pos_infin, bpt::neg_infin),
                InvalidElementID);
        EXPECT_THROW(DateElement(0xFFFFFFFF, bpt::pos_infin, bpt::neg_infin),
                InvalidElementID);
    }

    TEST(DateElement, Swap)
    {
        DateElement ee1(0x21, bpt::min_date_time, bpt::neg_infin);
        DateElement ee2(0x42, bpt::max_date_time, bpt::pos_infin);
        swap(ee1, ee2);
        EXPECT_EQ(0x42, ee1.id());
        EXPECT_EQ(maxdt, ee1.value());
        EXPECT_EQ(pinf, ee1.get_default());
        EXPECT_EQ(0x21, ee2.id());
        EXPECT_EQ(mindt, ee2.value());
        EXPECT_EQ(ninf, ee2.get_default());

        ee1.swap(ee2);
        EXPECT_EQ(0x21, ee1.id());
        EXPECT_EQ(mindt, ee1.value());
        EXPECT_EQ(ninf, ee1.get_default());
        EXPECT_EQ(0x42, ee2.id());
        EXPECT_EQ(maxdt, ee2.value());
        EXPECT_EQ(pinf, ee2.get_default());

        bpt::ptime maxdt_copy(maxdt);
        swap(ee1, maxdt_copy);
        EXPECT_EQ(0x21, ee1.id());
        EXPECT_EQ(maxdt, ee1.value());
        EXPECT_EQ(mindt, maxdt_copy);
        EXPECT_EQ(ninf, ee1.get_default());
    }

    TEST(DateElement, LessThan)
    {
        DateElement ee1(ids::Null, early);
        DateElement ee2(ids::Null, late);
        EXPECT_TRUE(ee1 < ee2);
        EXPECT_TRUE(ee1 < late);
        EXPECT_TRUE(early < ee2);
        EXPECT_FALSE(ee2 < ee1);
        EXPECT_FALSE(ee2 < early);
        EXPECT_FALSE(late < ee1);
    }

    TEST(DateElement, LessThanEqual)
    {
        DateElement ee1(ids::Null, early);
        DateElement ee2(ids::Null, early);
        DateElement ee3(ids::Null, mid);
        EXPECT_TRUE(ee1 <= ee2);
        EXPECT_TRUE(ee1 <= ee3);
        EXPECT_TRUE(ee1 <= early);
        EXPECT_TRUE(ee1 <= mid);
        EXPECT_TRUE(early <= ee3);
        EXPECT_TRUE(mid <= ee3);
        EXPECT_FALSE(ee3 <= ee2);
        EXPECT_FALSE(ee3 <= early);
        EXPECT_FALSE(late <= ee3);
    }

    TEST(DateElement, NotEqual)
    {
        DateElement ee1(ids::Null, early);
        DateElement ee2(ids::Null, early);
        DateElement ee3(ids::Null, mid);
        EXPECT_FALSE(ee1 != ee2);
        EXPECT_FALSE(ee1 != early);
        EXPECT_FALSE(early != ee1);
        EXPECT_TRUE(ee1 != ee3);
        EXPECT_TRUE(ee1 != mid);
        EXPECT_TRUE(mid != ee1);
    }

    TEST(DateElement, GreaterThanEqual)
    {
        DateElement ee1(ids::Null, late);
        DateElement ee2(ids::Null, late);
        DateElement ee3(ids::Null, mid);
        EXPECT_TRUE(ee1 >= ee2);
        EXPECT_TRUE(ee1 >= ee3);
        EXPECT_TRUE(ee1 >= mid);
        EXPECT_TRUE(ee1 >= late);
        EXPECT_TRUE(mid >= ee3);
        EXPECT_TRUE(late >= ee3);
        EXPECT_FALSE(ee3 >= ee2);
        EXPECT_FALSE(ee3 >= late);
        EXPECT_FALSE(early >= ee3);
    }

    TEST(DateElement, GreaterThan)
    {
        DateElement ee1(ids::Null, mid);
        DateElement ee2(ids::Null, early);
        EXPECT_TRUE(ee1 > ee2);
        EXPECT_TRUE(ee1 > early);
        EXPECT_TRUE(mid > ee2);
        EXPECT_FALSE(ee2 > ee1);
        EXPECT_FALSE(ee2 > mid);
        EXPECT_FALSE(early > ee1);
    }

    TEST(DateElement, Subtraction)
    {
        bpt::time_duration two_hours(2, 0, 0);

        DateElement ee1(ids::Null, late);
        DateElement ee2(ids::Null, early);
        EXPECT_EQ(two_hours, ee1 - ee2);
        EXPECT_EQ(two_hours, ee1 - early);
        EXPECT_EQ(two_hours, late - ee2);
    }


    TEST(DateElement, AdditionDays)
    {
        bpt::ptime plusone(early + bgr::days(1));

        DateElement ee1(ids::Null, early);
        EXPECT_EQ(plusone, ee1 + bgr::days(1));
    }

    TEST(DateElement, SubtractionDays)
    {
        bpt::ptime minusone(early - bgr::days(1));

        DateElement ee1(ids::Null, early);
        EXPECT_EQ(minusone, ee1 - bgr::days(1));
    }


    TEST(DateElement, AdditionDuration)
    {
        DateElement ee1(ids::Null, early);
        EXPECT_EQ(mid, ee1 + bpt::time_duration(1, 0, 0));
    }

    TEST(DateElement, SubtractionDuration)
    {
        DateElement ee1(ids::Null, mid);
        EXPECT_EQ(early, ee1 - bpt::time_duration(1, 0, 0));
    }


    ///////////////////////////////////////////////////////////////////////////
    // PrimitiveElement interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST(DateElement, Value)
    {
        EXPECT_EQ(maxdt, DateElement(ids::Null,
                    bpt::max_date_time).value());
        EXPECT_EQ(maxdt, DateElement(ids::Null,
                    bpt::max_date_time, bpt::min_date_time).value());

        DateElement ee1(ids::Null, bpt::max_date_time);
        EXPECT_EQ(maxdt, ee1.value());
        ee1.value(bpt::pos_infin);
        EXPECT_EQ(pinf, ee1.value());

        DateElement ee2(ids::Null, bpt::max_date_time, bpt::min_date_time);
        ee2.value(bpt::pos_infin);
        EXPECT_EQ(pinf, ee2.value());
    }

    TEST(DateElement, Default)
    {
        EXPECT_FALSE(DateElement(ids::Null, bpt::max_date_time).has_default());
        EXPECT_TRUE(DateElement(ids::Null, bpt::max_date_time,
                    bpt::min_date_time).has_default());

        DateElement ee1(ids::Null, bpt::max_date_time, bpt::min_date_time);
        EXPECT_EQ(mindt, ee1.get_default());
        EXPECT_TRUE(ee1.has_default());
        ee1.remove_default();
        EXPECT_FALSE(ee1.has_default());
        ee1.set_default(bpt::pos_infin);
        EXPECT_TRUE(ee1.has_default());
        EXPECT_EQ(pinf, ee1.get_default());

        DateElement ee2(ids::Null, bpt::max_date_time);
        EXPECT_FALSE(ee2.has_default());
        ee2.set_default(bpt::min_date_time);
        EXPECT_TRUE(ee2.has_default());
        EXPECT_EQ(mindt, ee2.get_default());
        ee2.remove_default();
        EXPECT_FALSE(ee2.has_default());

        DateElement ee3(ids::Null, bpt::max_date_time);
        EXPECT_FALSE(ee3.is_default());
        ee3.set_default(bpt::max_date_time);
        EXPECT_TRUE(ee3.is_default());
        ee3.set_default(bpt::min_date_time);
        EXPECT_FALSE(ee3.is_default());
        ee3.value(mindt);
        EXPECT_TRUE(ee3.is_default());
    }

    TEST(DateElement, Equal)
    {
        DateElement ee1(ids::Null, bpt::max_date_time);
        DateElement ee2(ids::Null, bpt::max_date_time);
        DateElement ee3(ids::Null, bpt::min_date_time);
        EXPECT_TRUE(ee1 == ee2);
        EXPECT_TRUE(ee1 == bpt::max_date_time);
        EXPECT_TRUE(maxdt == ee1);
        EXPECT_FALSE(ee1 == ee3);
        EXPECT_FALSE(ee1 == bpt::min_date_time);
        EXPECT_FALSE(mindt == ee1);
    }


    ///////////////////////////////////////////////////////////////////////////
    // Element interface tests
    ///////////////////////////////////////////////////////////////////////////

    std::streamsize fill_buffer(std::string& b, ids::ID id, bpt::ptime date,
            bool write_id, bool write_size, bool write_body)
    {
        std::streamsize total(0);
        if (write_id)
        {
            // Cheating on the IDs a bit - there is no protection here against
            // invalid IDs
            std::vector<char> tmp(tide::ebml_int::encode_u(id));
            b.append(&tmp[0], 0, tmp.size());
            total += tmp.size();
        }
        if (write_size)
        {
            std::vector<char> tmp(tide::vint::encode(8));
            b.append(&tmp[0], 0, tmp.size());
            total += tmp.size();
        }
        if (write_body)
        {
            bpt::ptime basis(bgr::date(2001, 1, 1));
            bpt::time_duration d(date - basis);
#if defined(TIDE_USE_NANOSECONDS)
            long long int temp = d.total_nanoseconds();
#else
            long long int temp = d.total_microseconds() * 1000;
#endif // defined(TIDE_USE_NANOSECONDS)
            for (std::streamsize ii(0); ii < 8; ++ii)
            {
                b.push_back(reinterpret_cast<uint8_t*>(&temp)[ii]);
            }
            total += 8;
        }
        return total;
    }

    TEST(DateElement, ID)
    {
        DateElement ee(42, bpt::max_date_time);
        EXPECT_EQ(42, ee.id());
    }

    TEST(DateElement, Offset)
    {
        DateElement ee1(ids::Null, bpt::max_date_time);
        EXPECT_EQ(0, ee1.offset());

        std::stringstream ss;
        std::string input_val;

        // Set when read test
        DateElement ee2(ids::Null, early), ee3(ids::Null, bpt::max_date_time);
        write(ee2, ss);
        ids::write(ids::Null, ss);
        fill_buffer(input_val, ids::Null, late, false, true, true);
        ss << input_val;
        ss.seekg(ee2.stored_size() + ids::size(ids::Null));
        ee3.read(ss);
        EXPECT_EQ(ee2.stored_size(), ee3.offset());

        // Set when written test
        write(ee3, ss);
        EXPECT_EQ(ee2.stored_size() + ee3.stored_size(), ee3.offset());
    }

    TEST(DateElement, StoredSize)
    {
        DateElement ee(ids::Null, bpt::max_date_time);
        EXPECT_EQ(ids::size(ids::Null) + vint::size(8) + 8, ee.stored_size());
        ee.value(bpt::min_date_time);
        EXPECT_EQ(ids::size(ids::Null) + vint::size(8) + 8, ee.stored_size());
    }

    TEST(DateElement, Read)
    {
        std::istringstream input;
        std::string input_val;

        DateElement ee(ids::Null, bpt::not_a_date_time);
        fill_buffer(input_val, ids::Null, early, false, true, true);
        input.str(input_val);
        EXPECT_EQ(vint::size(8) + 8, ee.read(input));
        EXPECT_EQ(ids::Null, ee.id());
        EXPECT_EQ(early, ee.value());

        ee.value(bpt::max_date_time);
        ee.set_default(bpt::max_date_time);
        EXPECT_TRUE(ee.has_default());
        EXPECT_TRUE(ee.is_default());
        std::string().swap(input_val);
        fill_buffer(input_val, ids::Null, mid, false, true, true);
        input.str(input_val);
        EXPECT_EQ(vint::size(8) + 8, ee.read(input));
        EXPECT_EQ(mid, ee.value());
        EXPECT_EQ(maxdt, ee.get_default());
        EXPECT_FALSE(ee.is_default());

        // Test for ReadError exception
        std::string().swap(input_val);
        fill_buffer(input_val, ids::Null, early, false, true, true);
        input.str(input_val.substr(0, 4));
        EXPECT_THROW(ee.read(input), ReadError);
    }

    TEST(DateElement, StartWrite)
    {
        std::stringstream output;
        std::string expected;
        DateElement ee(ids::Null, early);

        fill_buffer(expected, ids::Null, early, true, true, true);
        EXPECT_EQ(ids::size(ids::Null) + vint::size(8) + 8,
                ee.start_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Null) + vint::size(8) + 8, output.tellp());
    }

    TEST(DateElement, FinishWrite)
    {
        std::stringstream output;
        std::string expected;
        DateElement ee(ids::Null, early);

        // Test for exception and no change to output when finishing without
        // starting
        EXPECT_THROW(ee.finish_write(output), NotWriting);
        EXPECT_EQ(0, output.tellp());

        fill_buffer(expected, ids::Null, early, true, true, true);
        ee.start_write(output);
        EXPECT_EQ(ids::size(ids::Null) + vint::size(8) + 8,
                ee.finish_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Null) + vint::size(8) + 8, output.tellp());

        // Test for double-finish raising an exception
        EXPECT_THROW(ee.finish_write(output), NotWriting);
    }

    TEST(DateElement, Write)
    {
        std::stringstream output;
        std::string expected;
        DateElement ee(ids::Null, early);

        fill_buffer(expected, ids::Null, early, true, true, true);
        EXPECT_EQ(ids::size(ids::Null) + vint::size(8) + 8,
                write(ee, output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Null) + vint::size(8) + 8, output.tellp());
    }
}; // namespace test_date_el

