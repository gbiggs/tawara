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

#include "test_utilities.h"
#include <boost/date_time/date_defs.hpp>
#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/foreach.hpp>
#include <jonen/binary_element.h>
#include <jonen/date_element.h>
#include <jonen/float_element.h>
#include <jonen/integer_elements.h>
#include <jonen/segment_info.h>
#include <jonen/string_element.h>
#include <vector>

using namespace jonen;
using namespace test_utils;

namespace test_segment_info
{
    ///////////////////////////////////////////////////////////////////////////
    // Helper functions
    ///////////////////////////////////////////////////////////////////////////

    void populate_children(std::vector<ElPtr>& children)
    {
        children.push_back(ElPtr(new BinaryElement(ids::SegmentUID,
                        std::vector<char>(16, 2))));
        children.push_back(ElPtr(new StringElement(ids::SegmentFileName,
                        "seg_filename")));
        children.push_back(ElPtr(new BinaryElement(ids::PrevUID,
                        std::vector<char>(16, 1))));
        children.push_back(ElPtr(new StringElement(ids::PrevFileName,
                        "prev_filename")));
        children.push_back(ElPtr(new BinaryElement(ids::NextUID,
                        std::vector<char>(16, 3))));
        children.push_back(ElPtr(new StringElement(ids::NextFileName,
                        "next_filename")));
        children.push_back(ElPtr(new BinaryElement(ids::SegmentFamily,
                        std::vector<char>(16, 4))));
        children.push_back(ElPtr(new BinaryElement(ids::SegmentFamily,
                        std::vector<char>(16, 5))));
        children.push_back(ElPtr(new UIntElement(ids::TimecodeScale,
                        2000000)));
        children.push_back(ElPtr(new FloatElement(ids::Duration, 4.2)));
        children.push_back(ElPtr(new DateElement(ids::DateUTC,
                        boost::posix_time::ptime(boost::gregorian::date(
                                2012, boost::date_time::Aug, 13),
                            boost::posix_time::hours(1)))));
        children.push_back(ElPtr(new StringElement(ids::Title, "title")));
        children.push_back(ElPtr(new StringElement(ids::MuxingApp, "muxer")));
        children.push_back(ElPtr(new StringElement(ids::WritingApp,
                        "writer")));
    }

    void populate_element(SegmentInfo& ee, std::vector<ElPtr> const& children)
    {
        using boost::dynamic_pointer_cast;

        ee.uid(*dynamic_pointer_cast<BinaryElement>(children[0]));
        ee.filename(*dynamic_pointer_cast<StringElement>(children[1]));
        ee.previous_uid(*dynamic_pointer_cast<BinaryElement>(children[2]));
        ee.previous_fn(*dynamic_pointer_cast<StringElement>(children[3]));
        ee.next_uid(*dynamic_pointer_cast<BinaryElement>(children[4]));
        ee.next_fn(*dynamic_pointer_cast<StringElement>(children[5]));
        ee.add_family(*dynamic_pointer_cast<BinaryElement>(children[6]));
        ee.add_family(*dynamic_pointer_cast<BinaryElement>(children[7]));
        ee.timecode_scale(*dynamic_pointer_cast<UIntElement>(children[8]));
        ee.duration(*dynamic_pointer_cast<FloatElement>(children[9]));
        ee.date(*dynamic_pointer_cast<DateElement>(children[10]));
        ee.title(*dynamic_pointer_cast<StringElement>(children[11]));
        ee.muxing_app(*dynamic_pointer_cast<StringElement>(children[12]));
        ee.writing_app(*dynamic_pointer_cast<StringElement>(children[13]));
    }

    void check_children_match(SegmentInfo const& ee,
            std::vector<ElPtr> const& children)
    {
        using boost::dynamic_pointer_cast;

        EXPECT_PRED_FORMAT2(std_vectors_eq,
                dynamic_pointer_cast<BinaryElement>(children[0])->value(),
                ee.uid().value());
        EXPECT_PRED_FORMAT2(std_strings_eq,
                dynamic_pointer_cast<StringElement>(children[1])->value(),
                ee.filename().value());
        EXPECT_PRED_FORMAT2(std_vectors_eq,
                dynamic_pointer_cast<BinaryElement>(children[2])->value(),
                ee.previous_uid().value());
        EXPECT_PRED_FORMAT2(std_strings_eq,
                dynamic_pointer_cast<StringElement>(children[3])->value(),
                ee.previous_fn().value());
        EXPECT_PRED_FORMAT2(std_vectors_eq,
                dynamic_pointer_cast<BinaryElement>(children[4])->value(),
                ee.next_uid().value());
        EXPECT_PRED_FORMAT2(std_strings_eq,
                dynamic_pointer_cast<StringElement>(children[5])->value(),
                ee.next_fn().value());
        EXPECT_PRED_FORMAT2(std_vectors_eq,
                dynamic_pointer_cast<BinaryElement>(children[6])->value(),
                ee.families()[0]);
        EXPECT_PRED_FORMAT2(std_vectors_eq,
                dynamic_pointer_cast<BinaryElement>(children[7])->value(),
                ee.families()[1]);
        EXPECT_EQ(*dynamic_pointer_cast<UIntElement>(children[8]),
                ee.timecode_scale());
        EXPECT_EQ(*dynamic_pointer_cast<FloatElement>(children[9]),
                ee.duration());
        EXPECT_EQ(*dynamic_pointer_cast<DateElement>(children[10]), ee.date());
        EXPECT_PRED_FORMAT2(std_strings_eq,
                dynamic_pointer_cast<StringElement>(children[11])->value(),
                ee.title().value());
        EXPECT_PRED_FORMAT2(std_strings_eq,
                dynamic_pointer_cast<StringElement>(children[12])->value(),
                ee.muxing_app().value());
        EXPECT_PRED_FORMAT2(std_strings_eq,
                dynamic_pointer_cast<StringElement>(children[13])->value(),
                ee.writing_app().value());
    }


    ///////////////////////////////////////////////////////////////////////////
    // SegmentInto interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST(SegmentInfo, Construction)
    {
        SegmentInfo ee;
        EXPECT_EQ(ids::Info, ee.id());
        EXPECT_TRUE(ee.uid().empty());
        EXPECT_TRUE(ee.filename().empty());
        EXPECT_TRUE(ee.previous_uid().empty());
        EXPECT_TRUE(ee.previous_fn().empty());
        EXPECT_TRUE(ee.next_uid().empty());
        EXPECT_TRUE(ee.next_fn().empty());
        EXPECT_TRUE(ee.families().empty());
        EXPECT_EQ(1000000, ee.timecode_scale());
        EXPECT_EQ(0.0, ee.duration());
        EXPECT_EQ(boost::posix_time::not_a_date_time, ee.date());
        EXPECT_TRUE(ee.title().empty());
        EXPECT_TRUE(ee.muxing_app().empty());
        EXPECT_TRUE(ee.writing_app().empty());
    }


    TEST(SegmentInfo, CopyConstruction)
    {
        SegmentInfo ee1;
        std::vector<ElPtr> children;
        populate_children(children);
        populate_element(ee1, children);

        SegmentInfo ee2(ee1);
        EXPECT_TRUE(ee1 == ee2);
    }


    TEST(SegmentInfo, Assignment)
    {
        SegmentInfo ee1;
        std::vector<ElPtr> children;
        populate_children(children);
        populate_element(ee1, children);

        SegmentInfo ee2;
        ee2 = ee1;
        EXPECT_TRUE(ee1 == ee2);
    }


    TEST(SegmentInfo, Swap)
    {
        SegmentInfo ee1;
        std::vector<ElPtr> children;
        populate_children(children);
        populate_element(ee1, children);

        SegmentInfo ee2;
        swap(ee1, ee2);

        check_children_match(ee2, children);
        EXPECT_TRUE(ee1.uid().empty());
        EXPECT_TRUE(ee1.filename().empty());
        EXPECT_TRUE(ee1.previous_uid().empty());
        EXPECT_TRUE(ee1.previous_fn().empty());
        EXPECT_TRUE(ee1.next_uid().empty());
        EXPECT_TRUE(ee1.next_fn().empty());
        EXPECT_TRUE(ee1.families().empty());
        EXPECT_EQ(1000000, ee1.timecode_scale());
        EXPECT_EQ(0.0, ee1.duration());
        EXPECT_EQ(boost::posix_time::not_a_date_time, ee1.date());
        EXPECT_TRUE(ee1.title().empty());
        EXPECT_TRUE(ee1.muxing_app().empty());
        EXPECT_TRUE(ee1.writing_app().empty());
    }


    TEST(SegmentInfo, Equal)
    {
        SegmentInfo ee1, ee2;
        std::vector<ElPtr> children;
        populate_children(children);
        populate_element(ee1, children);
        populate_element(ee2, children);

        EXPECT_TRUE(ee1 == ee2);
    }


    TEST(SegmentInfo, UID)
    {
        std::vector<char> uid1(16, 1);
        BinaryElement uid2(ids::Null, std::vector<char>(16, 2));

        SegmentInfo ee;
        EXPECT_TRUE(ee.uid().empty());

        ee.uid(uid1);
        EXPECT_PRED_FORMAT2(std_vectors_eq, uid1, ee.uid().value());
        ee.uid(uid2);
        EXPECT_PRED_FORMAT2(std_vectors_eq, uid2.value(), ee.uid().value());
    }


    TEST(SegmentInfo, Filename)
    {
        std::string fn1("11111");
        StringElement fn2(ids::Null, std::string("22222"));

        SegmentInfo ee;
        EXPECT_TRUE(ee.filename().empty());

        ee.filename(fn1);
        EXPECT_PRED_FORMAT2(std_strings_eq, fn1, ee.filename().value());
        ee.filename(fn2);
        EXPECT_PRED_FORMAT2(std_strings_eq, fn2.value(),
                ee.filename().value());
    }


    TEST(SegmentInfo, PreviousUID)
    {
        std::vector<char> uid1(16, 1);
        BinaryElement uid2(ids::Null, std::vector<char>(16, 2));

        SegmentInfo ee;
        EXPECT_TRUE(ee.previous_uid().empty());

        ee.previous_uid(uid1);
        EXPECT_PRED_FORMAT2(std_vectors_eq, uid1, ee.previous_uid().value());
        ee.previous_uid(uid2);
        EXPECT_PRED_FORMAT2(std_vectors_eq, uid2.value(),
                ee.previous_uid().value());
    }


    TEST(SegmentInfo, PreviousFilename)
    {
        std::string fn1("11111");
        StringElement fn2(ids::Null, std::string("22222"));

        SegmentInfo ee;
        EXPECT_TRUE(ee.previous_fn().empty());

        ee.previous_fn(fn1);
        EXPECT_PRED_FORMAT2(std_strings_eq, fn1, ee.previous_fn().value());
        ee.previous_fn(fn2);
        EXPECT_PRED_FORMAT2(std_strings_eq, fn2.value(),
                ee.previous_fn().value());
    }


    TEST(SegmentInfo, NextUID)
    {
        std::vector<char> uid1(16, 1);
        BinaryElement uid2(ids::Null, std::vector<char>(16, 2));

        SegmentInfo ee;
        EXPECT_TRUE(ee.next_uid().empty());

        ee.next_uid(uid1);
        EXPECT_PRED_FORMAT2(std_vectors_eq, uid1, ee.next_uid().value());
        ee.next_uid(uid2);
        EXPECT_PRED_FORMAT2(std_vectors_eq, uid2.value(),
                ee.next_uid().value());
    }


    TEST(SegmentInfo, NextFilename)
    {
        std::string fn1("11111");
        StringElement fn2(ids::Null, std::string("22222"));

        SegmentInfo ee;
        EXPECT_TRUE(ee.next_fn().empty());

        ee.next_fn(fn1);
        EXPECT_PRED_FORMAT2(std_strings_eq, fn1, ee.next_fn().value());
        ee.next_fn(fn2);
        EXPECT_PRED_FORMAT2(std_strings_eq, fn2.value(), ee.next_fn().value());
    }


    TEST(SegmentInfo, Families)
    {
        std::vector<char> uid1(16, 1);
        BinaryElement uid2(ids::Null, std::vector<char>(16, 2));

        SegmentInfo ee;
        EXPECT_TRUE(ee.families().empty());

        ee.add_family(uid1);
        EXPECT_EQ(1, ee.families().size());
        EXPECT_PRED_FORMAT2(std_vectors_eq, uid1, ee.families()[0]);
        ee.add_family(uid2);
        EXPECT_EQ(2, ee.families().size());
        EXPECT_PRED_FORMAT2(std_vectors_eq, uid2.value(), ee.families()[1]);

        ee.remove_family(uid1);
        EXPECT_EQ(1, ee.families().size());
        EXPECT_PRED_FORMAT2(std_vectors_eq, uid2.value(), ee.families()[0]);
        ee.remove_family(uid2);
        EXPECT_TRUE(ee.families().empty());
    }


    TEST(SegmentInfo, TimecodeScale)
    {
        SegmentInfo ee;
        EXPECT_EQ(1000000, ee.timecode_scale());

        ee.timecode_scale(2000000);
        EXPECT_EQ(2000000, ee.timecode_scale());

        UIntElement tcs(ids::Null, 3000000);
        ee.timecode_scale(tcs);
        EXPECT_EQ(3000000, ee.timecode_scale());
    }


    TEST(SegmentInfo, Duration)
    {
        SegmentInfo ee;
        EXPECT_EQ(0.0, ee.duration());

        ee.duration(4.2);
        EXPECT_EQ(4.2, ee.duration());

        FloatElement dur(ids::Null, 2.4);
        ee.duration(dur);
        EXPECT_EQ(2.4, ee.duration());
    }


    TEST(SegmentInfo, Date)
    {
        boost::posix_time::ptime date1(boost::gregorian::date(2012, 8, 13),
                    boost::posix_time::hours(17));
        DateElement date2(ids::Null,
                boost::posix_time::ptime(boost::gregorian::date(2012, 8, 13),
                    boost::posix_time::hours(1)));

        SegmentInfo ee;
        EXPECT_EQ(boost::posix_time::not_a_date_time, ee.date());

        ee.date(date1);
        EXPECT_EQ(date1, ee.date());

        ee.date(date2);
        EXPECT_EQ(date2, ee.date());
    }


    TEST(SegmentInfo, Title)
    {
        std::string s1("11111");
        StringElement s2(ids::Null, std::string("22222"));

        SegmentInfo ee;
        EXPECT_TRUE(ee.title().empty());

        ee.title(s1);
        EXPECT_PRED_FORMAT2(std_strings_eq, s1, ee.title().value());
        ee.title(s2);
        EXPECT_PRED_FORMAT2(std_strings_eq, s2.value(), ee.title().value());
    }


    TEST(SegmentInfo, MuxingApp)
    {
        std::string s1("11111");
        StringElement s2(ids::Null, std::string("22222"));

        SegmentInfo ee;
        EXPECT_TRUE(ee.muxing_app().empty());

        ee.muxing_app(s1);
        EXPECT_PRED_FORMAT2(std_strings_eq, s1, ee.muxing_app().value());
        ee.muxing_app(s2);
        EXPECT_PRED_FORMAT2(std_strings_eq, s2.value(),
                ee.muxing_app().value());
    }


    TEST(SegmentInfo, WritingApp)
    {
        std::string s1("11111");
        StringElement s2(ids::Null, std::string("22222"));

        SegmentInfo ee;
        EXPECT_TRUE(ee.writing_app().empty());

        ee.writing_app(s1);
        EXPECT_PRED_FORMAT2(std_strings_eq, s1, ee.writing_app().value());
        ee.writing_app(s2);
        EXPECT_PRED_FORMAT2(std_strings_eq, s2.value(),
                ee.writing_app().value());
    }


    ///////////////////////////////////////////////////////////////////////////
    // MasterElement interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST(SegmentInfo, EnableCRC)
    {
        SegmentInfo ee;
        EXPECT_TRUE(ee.crc_enabled());
        ee.disable_crc();
        EXPECT_FALSE(ee.crc_enabled());
        ee.enable_crc();
        EXPECT_TRUE(ee.crc_enabled());
    }


    ///////////////////////////////////////////////////////////////////////////
    // Element interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST(SegmentInfo, ID)
    {
        SegmentInfo ee;
        EXPECT_EQ(ids::Info, ee.id());
    }


    TEST(SegmentInfo, Offset)
    {
        SegmentInfo ee;
        EXPECT_EQ(0, ee.offset());

        std::stringstream ss;
        std::string input_val;

        // Set when read test
        IntElement ie(ids::Null, 42);
        write(ie, ss);
        std::vector<ElPtr> children;
        populate_children(children);
        fill_buffer(input_val, ids::Info, children, true, true, true);
        ss << input_val;
        ss.seekg(ie.stored_size() + ids::size(ids::Info));
        ee.read(ss);
        EXPECT_EQ(ie.stored_size(), ee.offset());

        // Set when written test
        write(ee, ss);
        EXPECT_EQ(ie.stored_size() + ee.stored_size(), ee.offset());
    }


    TEST(SegmentInfo, StoredSize)
    {
        SegmentInfo ee;
        ee.disable_crc();

        std::vector<ElPtr> children;
        populate_children(children);

        // Size with everything defaults
        std::streamsize body_size(children[8]->stored_size());
        EXPECT_EQ(ids::size(ids::Info) +
                vint::size(body_size) + body_size, ee.stored_size());

        // Size with CRC enabled
        ee.enable_crc();
        body_size += 6;
        EXPECT_EQ(ids::size(ids::Info) +
                vint::size(body_size) + body_size, ee.stored_size());
        ee.disable_crc();

        // Size with all values set
        body_size = 0;
        BOOST_FOREACH(ElPtr el, children)
        {
            body_size += el->stored_size();
        }
        populate_element(ee, children);
        EXPECT_EQ(ids::size(ids::Info) +
                vint::size(body_size) + body_size, ee.stored_size());
    }


    TEST(SegmentInfo, Read)
    {
        using boost::dynamic_pointer_cast;

        std::vector<ElPtr> children;
        populate_children(children);

        // Reading just the required value
        std::stringstream input;
        vint::write(children[8]->stored_size(), input);
        write(*children[8], input);

        SegmentInfo ee;
        EXPECT_EQ(vint::size(children[8]->stored_size()) +
                children[8]->stored_size(), ee.read(input));
        EXPECT_TRUE(ee.uid().empty());
        EXPECT_TRUE(ee.filename().empty());
        EXPECT_TRUE(ee.previous_uid().empty());
        EXPECT_TRUE(ee.previous_fn().empty());
        EXPECT_TRUE(ee.next_uid().empty());
        EXPECT_TRUE(ee.next_fn().empty());
        EXPECT_TRUE(ee.families().empty());
        EXPECT_EQ(*dynamic_pointer_cast<UIntElement>(children[8]),
                ee.timecode_scale());
        EXPECT_EQ(0.0, ee.duration());
        EXPECT_EQ(boost::posix_time::not_a_date_time, ee.date());
        EXPECT_TRUE(ee.title().empty());
        EXPECT_TRUE(ee.muxing_app().empty());
        EXPECT_TRUE(ee.writing_app().empty());

        // Reading all values
        std::string buffer;
        std::streamsize body_size = fill_buffer(buffer, ids::Info, children,
                false, true, true, true);
        input.str(buffer);
        EXPECT_EQ(vint::size(body_size) + body_size, ee.read(input));
        check_children_match(ee, children);

        // Body size value wrong (too small)
        input.str(std::string());
        vint::write(2, input);
        write(*children[0], input);
        write(*children[8], input);
        EXPECT_THROW(ee.read(input), ReadError);
        // Post-condition test: error should leave element as before
        check_children_match(ee, children);

        // Invalid child
        input.str(std::string());
        UIntElement ue(ids::EBML, 0xFFFF);
        vint::write(ue.stored_size(), input);
        write(ue, input);
        EXPECT_THROW(ee.read(input), InvalidChildID);
        // Post-condition test: error should leave element as before
        check_children_match(ee, children);

        // No time code scale should cause the default to be used
        input.str(std::string());
        body_size = write_except(input, children, 8); // 8 = timecode scale
        input.str(std::string());
        vint::write(body_size, input);
        write_except(input, children, 8); // 8 = timecode scale
        EXPECT_NO_THROW(read(ee, input));
        EXPECT_EQ(1000000, ee.timecode_scale());

        // Zero-length body should not be a problem (default timecode scale is
        // used)
        input.str(std::string());
        populate_element(ee, children);
        vint::write(0, input);
        EXPECT_NO_THROW(read(ee, input));
        EXPECT_TRUE(ee.uid().empty());
        EXPECT_TRUE(ee.filename().empty());
        EXPECT_TRUE(ee.previous_uid().empty());
        EXPECT_TRUE(ee.previous_fn().empty());
        EXPECT_TRUE(ee.next_uid().empty());
        EXPECT_TRUE(ee.next_fn().empty());
        EXPECT_TRUE(ee.families().empty());
        EXPECT_EQ(1000000, ee.timecode_scale());
        EXPECT_EQ(0.0, ee.duration());
        EXPECT_EQ(boost::posix_time::not_a_date_time, ee.date());
        EXPECT_TRUE(ee.title().empty());
        EXPECT_TRUE(ee.muxing_app().empty());
        EXPECT_TRUE(ee.writing_app().empty());

        // Reading just the required value should cause the other values to be
        // reset to defaults
        input.str(std::string());
        populate_element(ee, children);
        vint::write(children[8]->stored_size(), input);
        write(*children[8], input);
        EXPECT_EQ(vint::size(children[8]->stored_size()) +
                children[8]->stored_size(), ee.read(input));
        EXPECT_TRUE(ee.uid().empty());
        EXPECT_TRUE(ee.filename().empty());
        EXPECT_TRUE(ee.previous_uid().empty());
        EXPECT_TRUE(ee.previous_fn().empty());
        EXPECT_TRUE(ee.next_uid().empty());
        EXPECT_TRUE(ee.next_fn().empty());
        EXPECT_TRUE(ee.families().empty());
        EXPECT_EQ(*dynamic_pointer_cast<UIntElement>(children[8]),
                ee.timecode_scale());
        EXPECT_EQ(0.0, ee.duration());
        EXPECT_EQ(boost::posix_time::not_a_date_time, ee.date());
        EXPECT_TRUE(ee.title().empty());
        EXPECT_TRUE(ee.muxing_app().empty());
        EXPECT_TRUE(ee.writing_app().empty());
    }


    TEST(SegmentInfo, ReadWithCRC)
    {
        std::vector<ElPtr> children;
        populate_children(children);

        std::string buffer;
        std::streamsize read_size(0);
        read_size = fill_buffer(buffer, ids::Info, children, false, true, true,
                true);

        SegmentInfo ee;
        ee.disable_crc();
        std::stringstream input(buffer);
        EXPECT_EQ(read_size, read(ee, input));
        check_children_match(ee, children);

        // Bad CRC
        std::string().swap(buffer);
        fill_buffer(buffer, ids::Info, children, false, true, true, true);
        // Corrupt the CRC
        buffer[3] = 0x00;
        buffer[4] = 0x00;
        // Attempt to read
        input.str(buffer);
        EXPECT_THROW(read(ee, input), BadCRC);

        // Test automatic enabling/disabling of CRC
        ee.disable_crc();
        std::string().swap(buffer);
        fill_buffer(buffer, ids::Info, children, false, true, true, true);
        input.str(buffer);
        read(ee, input);
        EXPECT_TRUE(ee.crc_enabled());
        std::string().swap(buffer);
        fill_buffer(buffer, ids::Info, children, false, true, true, false);
        input.str(buffer);
        read(ee, input);
        EXPECT_FALSE(ee.crc_enabled());
    }


    TEST(SegmentInfo, StartWrite)
    {
        using boost::dynamic_pointer_cast;

        std::stringstream output, expected_ss;
        std::string expected;
        SegmentInfo ee;
        ee.disable_crc();

        std::vector<ElPtr> children;
        populate_children(children);
        ee.timecode_scale(*dynamic_pointer_cast<UIntElement>(children[8]));

        // Writing with defaults should give a body with only a timecode scale.
        std::streamsize body_size(children[8]->stored_size());
        ids::write(ids::Info, expected_ss);
        vint::write(body_size, expected_ss);
        write(*children[8], expected_ss);
        EXPECT_EQ(ids::size(ids::Info) + vint::size(body_size) + body_size,
                ee.start_write(output));
        EXPECT_PRED_FORMAT2(std_buffers_eq, output.str(), expected_ss.str());
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Info) + vint::size(body_size) + body_size,
                output.tellp());

        // Write with everything set
        populate_element(ee, children);
        fill_buffer(expected, ids::Info, children, true, true, true);

        body_size = children_size(children);
        EXPECT_EQ(ids::size(ids::Info) + vint::size(body_size) + body_size,
                ee.start_write(output));
        EXPECT_PRED_FORMAT2(std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Info) + vint::size(body_size) + body_size,
                output.tellp());
    }


    TEST(SegmentInfo, FinishWrite)
    {
        using boost::dynamic_pointer_cast;

        std::stringstream output, expected_ss;
        std::string expected;
        SegmentInfo ee;
        ee.disable_crc();

        // Test for exception and no change to output when finishing without
        // starting
        EXPECT_THROW(ee.finish_write(output), NotWriting);
        EXPECT_EQ(0, output.tellp());

        std::vector<ElPtr> children;
        populate_children(children);
        ee.timecode_scale(*dynamic_pointer_cast<UIntElement>(children[8]));

        // Writing with defaults should give a body with only a timecode scale.
        std::streamsize body_size(children[8]->stored_size());
        ids::write(ids::Info, expected_ss);
        vint::write(body_size, expected_ss);
        write(*children[8], expected_ss);
        ee.start_write(output);
        EXPECT_EQ(ids::size(ids::Info) + vint::size(body_size) + body_size,
                ee.finish_write(output));
        EXPECT_PRED_FORMAT2(std_buffers_eq, output.str(), expected_ss.str());
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Info) + vint::size(body_size) + body_size,
                output.tellp());

        // Write with everything set
        populate_element(ee, children);
        fill_buffer(expected, ids::Info, children, true, true, true);

        body_size = children_size(children);
        ee.start_write(output);
        EXPECT_EQ(ids::size(ids::Info) + vint::size(body_size) + body_size,
                ee.finish_write(output));
        EXPECT_PRED_FORMAT2(std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Info) + vint::size(body_size) + body_size,
                output.tellp());

        // Test for double-finish raising an exception
        EXPECT_THROW(ee.finish_write(output), NotWriting);
    }


    TEST(SegmentInfo, Write)
    {
        using boost::dynamic_pointer_cast;

        std::stringstream output, expected_ss;
        std::string expected;
        SegmentInfo ee;
        ee.disable_crc();

        std::vector<ElPtr> children;
        populate_children(children);
        ee.timecode_scale(*dynamic_pointer_cast<UIntElement>(children[8]));

        // Writing with defaults should give a body with only a timecode scale.
        std::streamsize body_size(children[8]->stored_size());
        ids::write(ids::Info, expected_ss);
        vint::write(body_size, expected_ss);
        write(*children[8], expected_ss);
        ee.start_write(output);
        EXPECT_EQ(ids::size(ids::Info) + vint::size(body_size) + body_size,
                write(ee, output));
        EXPECT_PRED_FORMAT2(std_buffers_eq, output.str(), expected_ss.str());
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Info) + vint::size(body_size) + body_size,
                output.tellp());

        // Write with everything set
        populate_element(ee, children);
        fill_buffer(expected, ids::Info, children, true, true, true);

        body_size = children_size(children);
        ee.start_write(output);
        EXPECT_EQ(ids::size(ids::Info) + vint::size(body_size) + body_size,
                write(ee, output));
        EXPECT_PRED_FORMAT2(std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Info) + vint::size(body_size) + body_size,
                output.tellp());
    }


    TEST(SegmentInfo, WriteWithCRC)
    {
        using boost::dynamic_pointer_cast;

        std::stringstream output, expected_ss;
        std::string expected;
        SegmentInfo ee;

        std::vector<ElPtr> children;
        populate_children(children);
        ee.timecode_scale(*dynamic_pointer_cast<UIntElement>(children[8]));

        ee.enable_crc();
        populate_element(ee, children);
        std::streamsize body_size(0);
        body_size = fill_buffer(expected, ids::Info, children, true, true,
                true, true);
        ee.start_write(output);
        EXPECT_EQ(ids::size(ids::Info) + vint::size(body_size) + body_size,
                write(ee, output));
        EXPECT_PRED_FORMAT2(std_buffers_eq, output.str(), expected_ss.str());
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Info) + vint::size(body_size) + body_size,
                output.tellp());
    }


    TEST(SegmentInfo, RoundTrip)
    {
        std::vector<ElPtr> children;
        populate_children(children);

        SegmentInfo ee1;
        populate_element(ee1, children);

        std::stringstream io;
        std::streamsize written_bytes = write(ee1, io);
        // Skip the ID
        io.seekg(ids::size(ids::Info), std::ios::beg);
        SegmentInfo ee2;
        std::streamsize read_bytes = read(ee2, io);

        check_children_match(ee2, children);
        EXPECT_EQ(written_bytes - ids::size(ids::Info), read_bytes);
    }
}; // namespace test_segment_info

