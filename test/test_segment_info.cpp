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


namespace test_seg_info
{
    uint64_t 
}; // namespace test_seg_info


TEST(SegmentInfo, Size)
{
    typedef boost::shared_ptr<tide::Element> ElPtr;
    std::vector<ElPtr> all_children, used_children;

    all_children.push_back(ElPtr(new BinaryElement(0x80, std::vector<char>(8, 0xC0))));
    all_children.push_back(ElPtr(new StringElement(0x80, "Segment filename")));
    all_children.push_back(ElPtr(new BinaryElement(0x80, std::vector<char>(8, 0xC0))));
    all_children.push_back(ElPtr(new StringElement(0x80, "Previous filename")));
    all_children.push_back(ElPtr(new BinaryElement(0x80, std::vector<char>(8, 0xC0))));
    all_children.push_back(ElPtr(new StringElement(0x80, "Next filename")));
    all_children.push_back(ElPtr(new BinaryElement(0x80, std::vector<char>(8, 0xC0))));
    all_children.push_back(ElPtr(new FloatElement(0x80, 12345)));
    all_children.push_back(ElPtr(new DateElement(0x80, 0xFFFFFFFFFF)));
    all_children.push_back(ElPtr(new StringElement(0x80, "Title")));
    all_children.push_back(ElPtr(new StringElement(0x80, "Muxer")));
    all_children.push_back(ElPtr(new StringElement(0x80, "Writer")));
    used_children.push_back(ElPtr(new UIntElement(0x80, 1000000)));

    tide::SegmentInfo e();
    EXPECT_EQ(used_children[0]->size(), e.size());
    EXPECT_EQ(tide::ids::coded_size(ids::SegmentInfo) +
            tide::vint::coded_size(used_children[0]->total_size()) +
            used_children[0]->total_size(), e.total_size());

    std::streamsize(0);
    e.uid(all_children[0]->value());
    BOOST_FOREACH(ElPtr uel, used_children)
    {
        body_size += uel->total_size();
    }


    BOOST_FOREACH(ElPtr el, all_children)
    {
        used_children.push_back(el);
        std::streamsize body_size(0);
    }
}


TEST(SegmentInfo, Write)
{
    std::ostringstream output;
    std::stringstream expected;

    tide::BinaryElement be(tide::ids::SegmentInfoID,
            // SegmentInfoHead is a nice, long ID to test with
            tide::ids::encode(tide::ids::SegmentInfoHead));
    tide::UIntElement ue(tide::ids::SegmentInfoPosition, 12345);
    be.write(expected);
    ue.write(expected);
    std::streamsize expected_size(be.total_size() + ue.total_size());

    tide::SegmentInfo se(tide::ids::SegmentInfoHead, 12345);

    EXPECT_EQ(expected_size, se.write_body(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected.str());

    output.str(std::string());
    expected.str(std::string());
    tide::ids::write(tide::ids::SegmentInfo, expected);
    tide::vint::write(expected_size, expected);
    be.write(expected);
    ue.write(expected);

    EXPECT_EQ(tide::ids::coded_size(tide::ids::SegmentInfo) +
            tide::vint::coded_size(expected_size) + expected_size,
            se.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(), expected.str());
}


TEST(SegmentInfo, Read)
{
    std::stringstream input;

    tide::BinaryElement be(tide::ids::SegmentInfoID,
            // SegmentInfoHead is a nice, long ID to test with
            tide::ids::encode(tide::ids::SegmentInfoHead));
    tide::UIntElement ue(tide::ids::SegmentInfoPosition, 12345);
    std::streamsize body_size(be.total_size() + ue.total_size());

    tide::vint::write(body_size, input);
    be.write(input);
    ue.write(input);

    tide::SegmentInfo e(0x80, 0);
    EXPECT_EQ(tide::vint::coded_size(body_size) + body_size,
        e.read_body(input));
    EXPECT_EQ(tide::ids::SegmentInfoHead, e.indexed_id());
    EXPECT_EQ(12345, e.offset());

    input.str(std::string());
    be.value(tide::ids::encode(tide::ids::EBML));
    ue.value(54321);
    tide::vint::write(body_size, input);
    // Note the reversed order this time
    ue.write(input);
    be.write(input);

    EXPECT_EQ(tide::vint::coded_size(body_size) + body_size,
        e.read_body(input));
    EXPECT_EQ(tide::ids::EBML, e.indexed_id());
    EXPECT_EQ(54321, e.offset());

    // No SegmentInfoID child
    input.str(std::string());
    tide::vint::write(ue.total_size(), input);
    ue.write(input);
    EXPECT_THROW(e.read_body(input), tide::MissingChild);
    // No SegmentInfoPosition child
    input.str(std::string());
    tide::vint::write(be.total_size(), input);
    be.write(input);
    EXPECT_THROW(e.read_body(input), tide::MissingChild);
    // No children at all
    input.str(std::string());
    tide::vint::write(0, input);
    EXPECT_THROW(e.read_body(input), tide::MissingChild);
    // Body size value wrong (too big)
    input.str(std::string());
    tide::vint::write(ue.total_size() + be.total_size() + 5, input);
    ue.write(input);
    be.write(input);
    EXPECT_THROW(e.read_body(input), tide::BadBodySize);
    // Body size value wrong (too small)
    input.str(std::string());
    tide::vint::write(2, input);
    ue.write(input);
    be.write(input);
    EXPECT_THROW(e.read_body(input), tide::BadBodySize);
}


