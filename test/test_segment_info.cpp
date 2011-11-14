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

#include <gtest/gtest.h>
#include <tide/el_ids.h>
#include <tide/exceptions.h>
#include <tide/segment_info.h>
#include <tide/vint.h>

#include "test_utils.h"


TEST(SegmentInfo, Create)
{
    tide::SegmentInfo e();
    EXPECT_EQ(tide::ids::SegmentInfo, e.id());
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
    tide::SegmentInfo e();
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
    uid.empty();
    EXPECT_NO_THROW(e.uid(uid));
    EXPECT_TRUE(e.uid().empty());
    uid.assign(8, 0x00);
    EXPECT_THROW(e.uid(uid), tide::ValueOutOfRange);
}


TEST(SegmentInfo, SegmentFileName)
{
    tide::SegmentInfo e();
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
    tide::SegmentInfo e();
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
    uid.empty();
    EXPECT_NO_THROW(e.prev_uid(uid));
    EXPECT_TRUE(e.prev_uid().empty());
    uid.assign(8, 0x00);
    EXPECT_THROW(e.prev_uid(uid), tide::ValueOutOfRange);
}


TEST(SegmentInfo, PrevFileName)
{
    tide::SegmentInfo e();
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
    tide::SegmentInfo e();
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
    uid.empty();
    EXPECT_NO_THROW(e.next_uid(uid));
    EXPECT_TRUE(e.next_uid().empty());
    uid.assign(8, 0x00);
    EXPECT_THROW(e.next_uid(uid), tide::ValueOutOfRange);
}


TEST(SegmentInfo, NextFileName)
{
    tide::SegmentInfo e();
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
    tide::SegmentInfo e();
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
    uid.empty();
    EXPECT_NO_THROW(e.segment_family(uid));
    EXPECT_TRUE(e.segment_family().empty());
    uid.assign(8, 0x00);
    EXPECT_THROW(e.segment_family(uid), tide::ValueOutOfRange);
}


TEST(SegmentInfo, TimecodeScale)
{
    tide::SegmentInfo e();
    EXPECT_EQ(1000000, e.timecode_scale());
    EXPECT_NO_THROW(e.timecode_scale(314159));
    EXPECT_EQ(314159, e.timecode_scale());
    EXPECT_NO_THROW(e.timecode_scale(0));
    EXPECT_EQ(1000000, e.timecode_scale());
}


TEST(SegmentInfo, Duration)
{
    tide::SegmentInfo e();
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
    tide::SegmentInfo e();
    EXPECT_EQ(e.date(), 0);
    EXPECT_NO_THROW(e.date(-314159));
    EXPECT_EQ(-314159, e.date());
    EXPECT_NO_THROW(e.date(0));
    EXPECT_EQ(0, e.date());
    EXPECT_NO_THROW(e.date(314159));
    EXPECT_EQ(314159, e.date());
    // Test 64-bit values to confirm range
    EXPECT_NO_THROW(e.date(0x7FFFFFFFFFFFFFFF));
    EXPECT_EQ(0x7FFFFFFFFFFFFFFFFF, e.date());
    EXPECT_NO_THROW(e.date(-0x8000000000000000));
    EXPECT_EQ(-0x8000000000000000, e.date());
}


TEST(SegmentInfo, Title)
{
    tide::SegmentInfo e();
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
    tide::SegmentInfo e();
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
    tide::SegmentInfo e();
    EXPECT_TRUE(e.writing_app().empty());
    std::string fn("a_file.tide");
    EXPECT_NO_THROW(e.writing_app(fn));
    EXPECT_EQ(fn, e.writing_app());
    fn.clear();
    EXPECT_NO_THROW(e.writing_app(fn));
    EXPECT_TRUE(e.writing_app().empty());
}


TEST(SegmentInfo, Size)
{
    std::vector<test_utils::ElPtr> all_children, used_children;

    all_children.push_back(test_utils::ElPtr(new BinaryElement(0x80,
                    std::vector<char>(8, 0xC0))));
    all_children.push_back(test_utils::ElPtr(new StringElement(0x80, "Segment
                    filename")));
    all_children.push_back(test_utils::ElPtr(new BinaryElement(0x80,
                    std::vector<char>(8, 0xC0))));
    all_children.push_back(test_utils::ElPtr(new StringElement(0x80, "Previous
                    filename")));
    all_children.push_back(test_utils::ElPtr(new BinaryElement(0x80,
                    std::vector<char>(8, 0xC0))));
    all_children.push_back(test_utils::ElPtr(new StringElement(0x80, "Next
                    filename")));
    all_children.push_back(test_utils::ElPtr(new BinaryElement(0x80,
                    std::vector<char>(8, 0xC0))));
    all_children.push_back(test_utils::ElPtr(new FloatElement(0x80, 12345)));
    all_children.push_back(test_utils::ElPtr(new DateElement(0x80, 0xFFFFFFFFFF)));
    all_children.push_back(test_utils::ElPtr(new StringElement(0x80, "Title")));
    all_children.push_back(test_utils::ElPtr(new StringElement(0x80, "Muxer")));
    all_children.push_back(test_utils::ElPtr(new StringElement(0x80, "Writer")));
    used_children.push_back(test_utils::ElPtr(new UIntElement(0x80, 1000000)));

    tide::SegmentInfo e();
    EXPECT_EQ(used_children[0]->size(), e.size());
    EXPECT_EQ(tide::ids::coded_size(ids::SegmentInfo) +
            tide::vint::coded_size(used_children[0]->total_size()) +
            used_children[0]->total_size(), e.total_size());

    std::streamsize body_size(0);
    e.uid(all_children[0]->value());
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(body_size, e.size());
    EXPECT_EQ(tide::ids::coded_size(ids::SegmentInfo) +
            tide::vint::coded_size(body_size) + body_size, e.total_size());

    std::streamsize body_size(0);
    e.filename(all_children[0]->value());
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(body_size, e.size());
    EXPECT_EQ(tide::ids::coded_size(ids::SegmentInfo) +
            tide::vint::coded_size(body_size) + body_size, e.total_size());

    std::streamsize body_size(0);
    e.prev_uid(all_children[0]->value());
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(body_size, e.size());
    EXPECT_EQ(tide::ids::coded_size(ids::SegmentInfo) +
            tide::vint::coded_size(body_size) + body_size, e.total_size());

    std::streamsize body_size(0);
    e.prev_filename(all_children[0]->value());
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(body_size, e.size());
    EXPECT_EQ(tide::ids::coded_size(ids::SegmentInfo) +
            tide::vint::coded_size(body_size) + body_size, e.total_size());

    std::streamsize body_size(0);
    e.next_uid(all_children[0]->value());
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(body_size, e.size());
    EXPECT_EQ(tide::ids::coded_size(ids::SegmentInfo) +
            tide::vint::coded_size(body_size) + body_size, e.total_size());

    std::streamsize body_size(0);
    e.next_filename(all_children[0]->value());
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(body_size, e.size());
    EXPECT_EQ(tide::ids::coded_size(ids::SegmentInfo) +
            tide::vint::coded_size(body_size) + body_size, e.total_size());

    std::streamsize body_size(0);
    e.segment_family(all_children[0]->value());
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(body_size, e.size());
    EXPECT_EQ(tide::ids::coded_size(ids::SegmentInfo) +
            tide::vint::coded_size(body_size) + body_size, e.total_size());

    std::streamsize body_size(0);
    e.duration(all_children[0]->value());
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(body_size, e.size());
    EXPECT_EQ(tide::ids::coded_size(ids::SegmentInfo) +
            tide::vint::coded_size(body_size) + body_size, e.total_size());

    std::streamsize body_size(0);
    e.date(all_children[0]->value());
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(body_size, e.size());
    EXPECT_EQ(tide::ids::coded_size(ids::SegmentInfo) +
            tide::vint::coded_size(body_size) + body_size, e.total_size());

    std::streamsize body_size(0);
    e.title(all_children[0]->value());
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(body_size, e.size());
    EXPECT_EQ(tide::ids::coded_size(ids::SegmentInfo) +
            tide::vint::coded_size(body_size) + body_size, e.total_size());

    std::streamsize body_size(0);
    e.muxer(all_children[0]->value());
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(body_size, e.size());
    EXPECT_EQ(tide::ids::coded_size(ids::SegmentInfo) +
            tide::vint::coded_size(body_size) + body_size, e.total_size());

    std::streamsize body_size(0);
    e.writer(all_children[0]->value());
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(body_size, e.size());
    EXPECT_EQ(tide::ids::coded_size(ids::SegmentInfo) +
            tide::vint::coded_size(body_size) + body_size, e.total_size());
}


TEST(SegmentInfo, Write)
{
    std::ostringstream output;
    std::stringstream expected;

    std::vector<test_utils::ElPtr> all_children, used_children;
    all_children.push_back(test_utils::ElPtr(new BinaryElement(0x80,
                    std::vector<char>(8, 0xC0))));
    all_children.push_back(test_utils::ElPtr(new StringElement(0x80, "Segment
                    filename")));
    all_children.push_back(test_utils::ElPtr(new BinaryElement(0x80,
                    std::vector<char>(8, 0xC0))));
    all_children.push_back(test_utils::ElPtr(new StringElement(0x80, "Previous
                    filename")));
    all_children.push_back(test_utils::ElPtr(new BinaryElement(0x80,
                    std::vector<char>(8, 0xC0))));
    all_children.push_back(test_utils::ElPtr(new StringElement(0x80, "Next
                    filename")));
    all_children.push_back(test_utils::ElPtr(new BinaryElement(0x80,
                    std::vector<char>(8, 0xC0))));
    all_children.push_back(test_utils::ElPtr(new FloatElement(0x80, 12345)));
    all_children.push_back(test_utils::ElPtr(new DateElement(0x80, 0xFFFFFFFFFF)));
    all_children.push_back(test_utils::ElPtr(new StringElement(0x80, "Title")));
    all_children.push_back(test_utils::ElPtr(new StringElement(0x80, "Muxer")));
    all_children.push_back(test_utils::ElPtr(new StringElement(0x80, "Writer")));
    used_children.push_back(test_utils::ElPtr(new UIntElement(0x80, 1000000)));

    tide::SegmentInfo e();
    used_children[0]->write(expected);
    std::streamsize expected_size(used_children[0]->total_size);
    EXPECT_EQ(expected_size, e.write_body(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());
    output.str(std::string());
    expected.str(std::string());
    tide::ids::write(tide::ids::SegmentInfo, expected);
    tide::vint::write(expected_size, expected);
    used_children[0]->write(expected);
    EXPECT_EQ(tide::ids::coded_size(tide::ids::SegmentInfo) +
            tide::vint::coded_size(expected_size) + expected_size,
            e.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected.str());
}


TEST(SegmentInfo, Read)
{
    std::stringstream input;

    std::vector<test_utils::ElPtr> all_children, used_children;
    all_children.push_back(test_utils::ElPtr(new BinaryElement(0x80,
                    std::vector<char>(8, 0xC0))));
    all_children.push_back(test_utils::ElPtr(new StringElement(0x80, "Segment
                    filename")));
    all_children.push_back(test_utils::ElPtr(new BinaryElement(0x80,
                    std::vector<char>(8, 0xC0))));
    all_children.push_back(test_utils::ElPtr(new StringElement(0x80, "Previous
                    filename")));
    all_children.push_back(test_utils::ElPtr(new BinaryElement(0x80,
                    std::vector<char>(8, 0xC0))));
    all_children.push_back(test_utils::ElPtr(new StringElement(0x80, "Next
                    filename")));
    all_children.push_back(test_utils::ElPtr(new BinaryElement(0x80,
                    std::vector<char>(8, 0xC0))));
    all_children.push_back(test_utils::ElPtr(new FloatElement(0x80, 12345)));
    all_children.push_back(test_utils::ElPtr(new DateElement(0x80, 0xFFFFFFFFFF)));
    all_children.push_back(test_utils::ElPtr(new StringElement(0x80, "Title")));
    all_children.push_back(test_utils::ElPtr(new StringElement(0x80, "Muxer")));
    all_children.push_back(test_utils::ElPtr(new StringElement(0x80, "Writer")));
    used_children.push_back(test_utils::ElPtr(new UIntElement(0x80, 200000)));

    tide::SegmentInfo e();
    tide::vint::write(0, input);
    EXPECT_EQ(tide::vint::coded_size(0), e.read_body(input));
    EXPECT_EQ(1000000, e.timecode_scale());

    input.str(std::string());
    std::streamsize body_size(used_children[0]->total_size);
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

    BOOST_FOREACH(ElPtr el, all_children)
    {
        used_children.push_back(el);
        body_size = std::accumulate(used_children.begin(), used_children.end(),
                0, test_utils::TotalSizeOp());
        tide::vint::write(body_size, input);
        BOOST_FOREACH(ElPtr uel, used_children)
        {
            uel->write(input);
        }
        EXPECT_EQ(tide::vint::coded_size(body_size) + body_size),
            e.read_body(input));
    }

    // Body size value wrong (too big)
    input.str(std::string());
    tide::vint::write(used_children[0]->total_size() +
            used_children[3]->total_size() + 5, input);
    used_children[0]->write(input);
    used_children[3]->write(input);
    EXPECT_THROW(e.read_body(input), tide::BadBodySize);
    // Body size value wrong (too small)
    input.str(std::string());
    tide::vint::write(2, input);
    EXPECT_THROW(e.read_body(input), tide::BadBodySize);
}


