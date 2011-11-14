/* Tide
 *
 * Test the SegmentInfo element.
 *
 * Copyright 2011 Geoffrey Biggs geoffrey.biggs@aist.go.jp
 *     RT-Synthesis Research Group
 *     Intelligent Systems Research Institute,
 *     National Institute of Advanced Industrial Science and Technology (AIST),
 *     Japan
 *     All rights reserved.
 *
 * This file is part of Tide.
 *
 * Tide is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * Tide is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Tide. If not, see <http://www.gnu.org/licenses/>.
 */

#include <algorithm>
#include <boost/foreach.hpp>
#include <gtest/gtest.h>
#include <numeric>
#include <tide/binary_element.h>
#include <tide/date_element.h>
#include <tide/el_ids.h>
#include <tide/exceptions.h>
#include <tide/float_element.h>
#include <tide/segment_info.h>
#include <tide/uint_element.h>
#include <tide/vint.h>

#include "test_utils.h"


TEST(SegmentInfo, Create)
{
    tide::SegmentInfo e;
    EXPECT_EQ(tide::ids::Info, e.id());
    EXPECT_TRUE(e.uid().empty());
    EXPECT_TRUE(e.filename().empty());
    EXPECT_TRUE(e.prev_uid().empty());
    EXPECT_TRUE(e.prev_filename().empty());
    EXPECT_TRUE(e.next_uid().empty());
    EXPECT_TRUE(e.next_filename().empty());
    EXPECT_TRUE(e.segment_family().empty());
    EXPECT_EQ(1000000, e.timecode_scale());
    EXPECT_TRUE(e.duration() > 0);
    EXPECT_EQ(0, e.date());
    EXPECT_TRUE(e.title().empty());
    EXPECT_TRUE(e.muxing_app().empty());
    EXPECT_TRUE(e.writing_app().empty());
}


TEST(SegmentInfo, UID)
{
    tide::SegmentInfo e;
    std::vector<char> uid;
    EXPECT_TRUE(e.uid().empty());
    uid.push_back(0x01);
    uid.push_back(0x02);
    uid.push_back(0x03);
    uid.push_back(0x04);
    EXPECT_THROW(e.uid(uid), tide::ValueSizeOutOfRange);
    uid.push_back(0x05);
    uid.push_back(0x06);
    uid.push_back(0x07);
    uid.push_back(0x08);
    EXPECT_NO_THROW(e.uid(uid));
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, uid, e.uid());
    uid.clear();
    EXPECT_NO_THROW(e.uid(uid));
    EXPECT_TRUE(e.uid().empty());
    uid.assign(8, 0x00);
    EXPECT_THROW(e.uid(uid), tide::ValueOutOfRange);
}


TEST(SegmentInfo, SegmentFileName)
{
    tide::SegmentInfo e;
    EXPECT_TRUE(e.filename().empty());
    std::string fn("a_file.tide");
    EXPECT_NO_THROW(e.filename(fn));
    EXPECT_EQ(fn, e.filename());
    fn.clear();
    EXPECT_NO_THROW(e.filename(fn));
    EXPECT_TRUE(e.filename().empty());
}


TEST(SegmentInfo, PrevUID)
{
    tide::SegmentInfo e;
    std::vector<char> uid;
    EXPECT_TRUE(e.prev_uid().empty());
    uid.push_back(0x01);
    uid.push_back(0x02);
    uid.push_back(0x03);
    uid.push_back(0x04);
    EXPECT_THROW(e.prev_uid(uid), tide::ValueSizeOutOfRange);
    uid.push_back(0x05);
    uid.push_back(0x06);
    uid.push_back(0x07);
    uid.push_back(0x08);
    EXPECT_NO_THROW(e.prev_uid(uid));
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, uid, e.prev_uid());
    uid.clear();
    EXPECT_NO_THROW(e.prev_uid(uid));
    EXPECT_TRUE(e.prev_uid().empty());
    uid.assign(8, 0x00);
    EXPECT_THROW(e.prev_uid(uid), tide::ValueOutOfRange);
}


TEST(SegmentInfo, PrevFileName)
{
    tide::SegmentInfo e;
    EXPECT_TRUE(e.prev_filename().empty());
    std::string fn("a_file.tide");
    EXPECT_NO_THROW(e.prev_filename(fn));
    EXPECT_EQ(fn, e.prev_filename());
    fn.clear();
    EXPECT_NO_THROW(e.prev_filename(fn));
    EXPECT_TRUE(e.prev_filename().empty());
}


TEST(SegmentInfo, NextUID)
{
    tide::SegmentInfo e;
    std::vector<char> uid;
    EXPECT_TRUE(e.next_uid().empty());
    uid.push_back(0x01);
    uid.push_back(0x02);
    uid.push_back(0x03);
    uid.push_back(0x04);
    EXPECT_THROW(e.next_uid(uid), tide::ValueSizeOutOfRange);
    uid.push_back(0x05);
    uid.push_back(0x06);
    uid.push_back(0x07);
    uid.push_back(0x08);
    EXPECT_NO_THROW(e.next_uid(uid));
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, uid, e.next_uid());
    uid.clear();
    EXPECT_NO_THROW(e.next_uid(uid));
    EXPECT_TRUE(e.next_uid().empty());
    uid.assign(8, 0x00);
    EXPECT_THROW(e.next_uid(uid), tide::ValueOutOfRange);
}


TEST(SegmentInfo, NextFileName)
{
    tide::SegmentInfo e;
    EXPECT_TRUE(e.next_filename().empty());
    std::string fn("a_file.tide");
    EXPECT_NO_THROW(e.next_filename(fn));
    EXPECT_EQ(fn, e.next_filename());
    fn.clear();
    EXPECT_NO_THROW(e.next_filename(fn));
    EXPECT_TRUE(e.next_filename().empty());
}


TEST(SegmentInfo, SegmentFamily)
{
    tide::SegmentInfo e;
    std::vector<char> uid;
    EXPECT_TRUE(e.segment_family().empty());
    uid.push_back(0x01);
    uid.push_back(0x02);
    uid.push_back(0x03);
    uid.push_back(0x04);
    EXPECT_THROW(e.segment_family(uid), tide::ValueSizeOutOfRange);
    uid.push_back(0x05);
    uid.push_back(0x06);
    uid.push_back(0x07);
    uid.push_back(0x08);
    EXPECT_NO_THROW(e.segment_family(uid));
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, uid, e.segment_family());
    uid.clear();
    EXPECT_NO_THROW(e.segment_family(uid));
    EXPECT_TRUE(e.segment_family().empty());
    uid.assign(8, 0x00);
    EXPECT_THROW(e.segment_family(uid), tide::ValueOutOfRange);
}


TEST(SegmentInfo, TimecodeScale)
{
    tide::SegmentInfo e;
    EXPECT_EQ(1000000, e.timecode_scale());
    EXPECT_NO_THROW(e.timecode_scale(314159));
    EXPECT_EQ(314159, e.timecode_scale());
    EXPECT_NO_THROW(e.timecode_scale(0));
    EXPECT_EQ(1000000, e.timecode_scale());
}


TEST(SegmentInfo, Duration)
{
    tide::SegmentInfo e;
    EXPECT_TRUE(e.duration() > 0);
    EXPECT_NO_THROW(e.duration(314159));
    EXPECT_EQ(314159, e.duration());
    EXPECT_THROW(e.duration(-314159), tide::ValueOutOfRange);
    EXPECT_THROW(e.duration(0), tide::ValueOutOfRange);
    // Test 64-bit values to confirm range
    EXPECT_NO_THROW(e.duration(0xFFFFFFFFFFFFFFFF));
    EXPECT_EQ(0xFFFFFFFFFFFFFFFF, e.duration());
}


TEST(SegmentInfo, Date)
{
    tide::SegmentInfo e;
    EXPECT_EQ(e.date(), 0);
    EXPECT_NO_THROW(e.date(-314159));
    EXPECT_EQ(-314159, e.date());
    EXPECT_NO_THROW(e.date(0));
    EXPECT_EQ(0, e.date());
    EXPECT_NO_THROW(e.date(314159));
    EXPECT_EQ(314159, e.date());
    // Test 64-bit values to confirm range
    EXPECT_NO_THROW(e.date(0x7FFFFFFFFFFFFFFF));
    EXPECT_EQ(0x7FFFFFFFFFFFFFFF, e.date());
    EXPECT_NO_THROW(e.date(-0x8000000000000000));
    EXPECT_EQ(-0x8000000000000000, e.date());
}


TEST(SegmentInfo, Title)
{
    tide::SegmentInfo e;
    EXPECT_TRUE(e.title().empty());
    std::string fn("a_file.tide");
    EXPECT_NO_THROW(e.title(fn));
    EXPECT_EQ(fn, e.title());
    fn.clear();
    EXPECT_NO_THROW(e.title(fn));
    EXPECT_TRUE(e.title().empty());
}


TEST(SegmentInfo, MuxingApp)
{
    tide::SegmentInfo e;
    EXPECT_TRUE(e.muxing_app().empty());
    std::string fn("a_file.tide");
    EXPECT_NO_THROW(e.muxing_app(fn));
    EXPECT_EQ(fn, e.muxing_app());
    fn.clear();
    EXPECT_NO_THROW(e.muxing_app(fn));
    EXPECT_TRUE(e.muxing_app().empty());
}


TEST(SegmentInfo, WritingApp)
{
    tide::SegmentInfo e;
    EXPECT_TRUE(e.writing_app().empty());
    std::string fn("a_file.tide");
    EXPECT_NO_THROW(e.writing_app(fn));
    EXPECT_EQ(fn, e.writing_app());
    fn.clear();
    EXPECT_NO_THROW(e.writing_app(fn));
    EXPECT_TRUE(e.writing_app().empty());
}


namespace test_seg_info
{

void DoWriteTest(std::vector<test_utils::ElPtr> const& els,
        tide::SegmentInfo& e, std::string msg)
{
    std::ostringstream output;
    std::stringstream expected;

    std::streamsize expected_size(std::accumulate(els.begin(), els.end(), 0,
                test_utils::TotalSizeOp()));
    BOOST_FOREACH(test_utils::ElPtr el, els)
    {
        el->write(expected);
    }
    EXPECT_EQ(expected_size, e.write_body(output)) << msg;
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str()) << msg;
    output.str(std::string());
    expected.str(std::string());
    tide::ids::write(tide::ids::Info, expected);
    tide::vint::write(expected_size, expected);
    BOOST_FOREACH(test_utils::ElPtr el, els)
    {
        el->write(expected);
    }
    EXPECT_EQ(tide::ids::coded_size(tide::ids::Info) +
            tide::vint::coded_size(expected_size) + expected_size,
            e.write(output)) << msg;
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str()) << msg;
}

}; // namespace test_seg_info


TEST(SegmentInfo, Size)
{
    std::vector<test_utils::ElPtr> all_children, used_children;

    all_children.push_back(test_utils::ElPtr(new
                tide::BinaryElement(tide::ids::SegmentUID, std::vector<char>(8,
                        0xC0))));
    all_children.push_back(test_utils::ElPtr(new
                tide::StringElement(tide::ids::SegmentFileName,
                    "Segment filename")));
    all_children.push_back(test_utils::ElPtr(new
                tide::BinaryElement(tide::ids::PrevUID,
                    std::vector<char>(8, 0xC0))));
    all_children.push_back(test_utils::ElPtr(new
                tide::StringElement(tide::ids::PrevFileName,
                    "Previous filename")));
    all_children.push_back(test_utils::ElPtr(new
                tide::BinaryElement(tide::ids::NextUID,
                    std::vector<char>(8, 0xC0))));
    all_children.push_back(test_utils::ElPtr(new
                tide::StringElement(tide::ids::NextFileName,
                    "Next filename")));
    all_children.push_back(test_utils::ElPtr(new
                tide::BinaryElement(tide::ids::SegmentFamily,
                    std::vector<char>(8, 0xC0))));
    all_children.push_back(test_utils::ElPtr(new
                tide::FloatElement(tide::ids::Duration, 12345)));
    all_children.push_back(test_utils::ElPtr(new
                tide::DateElement(tide::ids::DateUTC, 0xFFFFFFFFFF)));
    all_children.push_back(test_utils::ElPtr(new
                tide::StringElement(tide::ids::Title, "Title")));
    all_children.push_back(test_utils::ElPtr(new
                tide::StringElement(tide::ids::MuxingApp, "Muxer")));
    all_children.push_back(test_utils::ElPtr(new
                tide::StringElement(tide::ids::WritingApp, "Writer")));
    used_children.push_back(test_utils::ElPtr(new
                tide::UIntElement(tide::ids::TimecodeScale, 1000000)));

    tide::SegmentInfo e;
    EXPECT_EQ(used_children[0]->total_size(), e.size());
    EXPECT_EQ(tide::ids::coded_size(tide::ids::Info) +
            tide::vint::coded_size(used_children[0]->total_size()) +
            used_children[0]->total_size(), e.total_size());

    std::streamsize body_size(0);
    e.uid(std::vector<char>(8, 0xC0));
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(body_size, e.size());
    EXPECT_EQ(tide::ids::coded_size(tide::ids::Info) +
            tide::vint::coded_size(body_size) + body_size, e.total_size());

    body_size = 0;
    e.filename("Segment filename");
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(body_size, e.size());
    EXPECT_EQ(tide::ids::coded_size(tide::ids::Info) +
            tide::vint::coded_size(body_size) + body_size, e.total_size());

    body_size = 0;
    e.prev_uid(std::vector<char>(8, 0xC0));
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(body_size, e.size());
    EXPECT_EQ(tide::ids::coded_size(tide::ids::Info) +
            tide::vint::coded_size(body_size) + body_size, e.total_size());

    body_size = 0;
    e.prev_filename("Previous filename");
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(body_size, e.size());
    EXPECT_EQ(tide::ids::coded_size(tide::ids::Info) +
            tide::vint::coded_size(body_size) + body_size, e.total_size());

    body_size = 0;
    e.next_uid(std::vector<char>(8, 0xC0));
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(body_size, e.size());
    EXPECT_EQ(tide::ids::coded_size(tide::ids::Info) +
            tide::vint::coded_size(body_size) + body_size, e.total_size());

    body_size = 0;
    e.next_filename("Next filename");
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(body_size, e.size());
    EXPECT_EQ(tide::ids::coded_size(tide::ids::Info) +
            tide::vint::coded_size(body_size) + body_size, e.total_size());

    body_size = 0;
    e.segment_family(std::vector<char>(8, 0xC0));
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(body_size, e.size());
    EXPECT_EQ(tide::ids::coded_size(tide::ids::Info) +
            tide::vint::coded_size(body_size) + body_size, e.total_size());

    body_size = 0;
    e.duration(12345);
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(body_size, e.size());
    EXPECT_EQ(tide::ids::coded_size(tide::ids::Info) +
            tide::vint::coded_size(body_size) + body_size, e.total_size());

    body_size = 0;
    e.date(0xFFFFFFFFFF);
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(body_size, e.size());
    EXPECT_EQ(tide::ids::coded_size(tide::ids::Info) +
            tide::vint::coded_size(body_size) + body_size, e.total_size());

    body_size = 0;
    e.title("Title");
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(body_size, e.size());
    EXPECT_EQ(tide::ids::coded_size(tide::ids::Info) +
            tide::vint::coded_size(body_size) + body_size, e.total_size());

    body_size = 0;
    e.muxing_app("Muxer");
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(body_size, e.size());
    EXPECT_EQ(tide::ids::coded_size(tide::ids::Info) +
            tide::vint::coded_size(body_size) + body_size, e.total_size());

    body_size = 0;
    e.writing_app("Writer");
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(body_size, e.size());
    EXPECT_EQ(tide::ids::coded_size(tide::ids::Info) +
            tide::vint::coded_size(body_size) + body_size, e.total_size());
}


TEST(SegmentInfo, Write)
{
    std::vector<test_utils::ElPtr> all_children, used_children;
    all_children.push_back(test_utils::ElPtr(new
                tide::BinaryElement(tide::ids::SegmentUID, std::vector<char>(8,
                        0xC0))));
    all_children.push_back(test_utils::ElPtr(new
                tide::StringElement(tide::ids::SegmentFileName,
                    "Segment filename")));
    all_children.push_back(test_utils::ElPtr(new
                tide::BinaryElement(tide::ids::PrevUID,
                    std::vector<char>(8, 0xC0))));
    all_children.push_back(test_utils::ElPtr(new
                tide::StringElement(tide::ids::PrevFileName,
                    "Previous filename")));
    all_children.push_back(test_utils::ElPtr(new
                tide::BinaryElement(tide::ids::NextUID,
                    std::vector<char>(8, 0xC0))));
    all_children.push_back(test_utils::ElPtr(new
                tide::StringElement(tide::ids::NextFileName,
                    "Next filename")));
    all_children.push_back(test_utils::ElPtr(new
                tide::BinaryElement(tide::ids::SegmentFamily,
                    std::vector<char>(8, 0xC0))));
    all_children.push_back(test_utils::ElPtr(new
                tide::FloatElement(tide::ids::Duration, 12345)));
    all_children.push_back(test_utils::ElPtr(new
                tide::DateElement(tide::ids::DateUTC, 0xFFFFFFFFFF)));
    all_children.push_back(test_utils::ElPtr(new
                tide::StringElement(tide::ids::Title, "Title")));
    all_children.push_back(test_utils::ElPtr(new
                tide::StringElement(tide::ids::MuxingApp, "Muxer")));
    all_children.push_back(test_utils::ElPtr(new
                tide::StringElement(tide::ids::WritingApp, "Writer")));
    used_children.push_back(test_utils::ElPtr(new
                tide::UIntElement(tide::ids::TimecodeScale, 1000000)));

    tide::SegmentInfo e;
    test_seg_info::DoWriteTest(used_children, e, "tc_scale");

    e.uid(std::vector<char>(8, 0xC0));
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    test_seg_info::DoWriteTest(used_children, e, "uid");

    e.filename("Segment filename");
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    test_seg_info::DoWriteTest(used_children, e, "filename");

    e.prev_uid(std::vector<char>(8, 0xC0));
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    test_seg_info::DoWriteTest(used_children, e, "prev_uid");

    e.prev_filename("Previous filename");
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    test_seg_info::DoWriteTest(used_children, e, "prev_filename");

    e.next_uid(std::vector<char>(8, 0xC0));
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    test_seg_info::DoWriteTest(used_children, e, "next_uid");

    e.next_filename("Next filename");
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    test_seg_info::DoWriteTest(used_children, e, "next_filename");

    e.segment_family(std::vector<char>(8, 0xC0));
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    test_seg_info::DoWriteTest(used_children, e, "segment_family");

    e.duration(12345);
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    test_seg_info::DoWriteTest(used_children, e, "duration");

    e.date(0xFFFFFFFFFF);
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    test_seg_info::DoWriteTest(used_children, e, "date");

    e.title("Title");
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    test_seg_info::DoWriteTest(used_children, e, "title");

    e.muxing_app("Muxer");
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    test_seg_info::DoWriteTest(used_children, e, "muxing_app");

    e.writing_app("Writer");
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    test_seg_info::DoWriteTest(used_children, e, "writing_app");
}


TEST(SegmentInfo, Read)
{
    std::stringstream input;

    std::vector<test_utils::ElPtr> all_children, used_children;
    all_children.push_back(test_utils::ElPtr(new
                tide::BinaryElement(tide::ids::SegmentUID, std::vector<char>(8,
                        0xC0))));
    all_children.push_back(test_utils::ElPtr(new
                tide::StringElement(tide::ids::SegmentFileName,
                    "Segment filename")));
    all_children.push_back(test_utils::ElPtr(new
                tide::BinaryElement(tide::ids::PrevUID,
                    std::vector<char>(8, 0xC0))));
    all_children.push_back(test_utils::ElPtr(new
                tide::StringElement(tide::ids::PrevFileName,
                    "Previous filename")));
    all_children.push_back(test_utils::ElPtr(new
                tide::BinaryElement(tide::ids::NextUID,
                    std::vector<char>(8, 0xC0))));
    all_children.push_back(test_utils::ElPtr(new
                tide::StringElement(tide::ids::NextFileName,
                    "Next filename")));
    all_children.push_back(test_utils::ElPtr(new
                tide::BinaryElement(tide::ids::SegmentFamily,
                    std::vector<char>(8, 0xC0))));
    all_children.push_back(test_utils::ElPtr(new
                tide::FloatElement(tide::ids::Duration, 12345)));
    all_children.push_back(test_utils::ElPtr(new
                tide::DateElement(tide::ids::DateUTC, 0xFFFFFFFFFF)));
    all_children.push_back(test_utils::ElPtr(new
                tide::StringElement(tide::ids::Title, "Title")));
    all_children.push_back(test_utils::ElPtr(new
                tide::StringElement(tide::ids::MuxingApp, "Muxer")));
    all_children.push_back(test_utils::ElPtr(new
                tide::StringElement(tide::ids::WritingApp, "Writer")));
    used_children.push_back(test_utils::ElPtr(new
                tide::UIntElement(tide::ids::TimecodeScale, 200000)));

    tide::SegmentInfo e;
    tide::vint::write(0, input);
    EXPECT_EQ(tide::vint::coded_size(0), e.read_body(input));
    EXPECT_EQ(1000000, e.timecode_scale());

    input.str(std::string());
    std::streamsize body_size(used_children[0]->total_size());
    tide::vint::write(body_size, input);
    used_children[0]->write(input);
    EXPECT_EQ(tide::vint::coded_size(body_size) + body_size,
            e.read_body(input));
    EXPECT_TRUE(e.uid().empty());
    EXPECT_TRUE(e.filename().empty());
    EXPECT_TRUE(e.prev_uid().empty());
    EXPECT_TRUE(e.prev_filename().empty());
    EXPECT_TRUE(e.next_uid().empty());
    EXPECT_TRUE(e.next_filename().empty());
    EXPECT_TRUE(e.segment_family().empty());
    EXPECT_TRUE(e.duration() > 0);
    EXPECT_EQ(0, e.date());
    EXPECT_TRUE(e.title().empty());
    EXPECT_TRUE(e.muxing_app().empty());
    EXPECT_TRUE(e.writing_app().empty());
    EXPECT_EQ(200000, e.timecode_scale());

    BOOST_FOREACH(test_utils::ElPtr el, all_children)
    {
        used_children.push_back(el);
        body_size = std::accumulate(used_children.begin(), used_children.end(),
                0, test_utils::TotalSizeOp());
        tide::vint::write(body_size, input);
        BOOST_FOREACH(test_utils::ElPtr uel, used_children)
        {
            uel->write(input);
        }
        EXPECT_EQ(tide::vint::coded_size(body_size) + body_size,
            e.read_body(input));
    }

    // Body size value wrong (too small)
    input.str(std::string());
    tide::vint::write(2, input);
    used_children[0]->write(input);
    used_children[3]->write(input);
    EXPECT_THROW(e.read_body(input), tide::BadBodySize);
}


