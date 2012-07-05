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

#include <algorithm>
#include <boost/foreach.hpp>
#include <gtest/gtest.h>
#include <numeric>
#include <celduin/binary_element.h>
#include <celduin/date_element.h>
#include <celduin/el_ids.h>
#include <celduin/exceptions.h>
#include <celduin/float_element.h>
#include <celduin/segment_info.h>
#include <celduin/uint_element.h>
#include <celduin/vint.h>

#include "test_utils.h"


TEST(SegmentInfo, Create)
{
    celduin::SegmentInfo e;
    EXPECT_EQ(celduin::ids::Info, e.id());
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
    celduin::SegmentInfo e;
    std::vector<char> uid;
    EXPECT_TRUE(e.uid().empty());
    uid.push_back(0x01);
    uid.push_back(0x02);
    uid.push_back(0x03);
    uid.push_back(0x04);
    EXPECT_THROW(e.uid(uid), celduin::ValueSizeOutOfRange);
    uid.push_back(0x05);
    uid.push_back(0x06);
    uid.push_back(0x07);
    uid.push_back(0x08);
    uid.push_back(0x09);
    uid.push_back(0x0A);
    uid.push_back(0x0B);
    uid.push_back(0x0C);
    uid.push_back(0x0D);
    uid.push_back(0x0D);
    uid.push_back(0x0E);
    uid.push_back(0x0F);
    EXPECT_NO_THROW(e.uid(uid));
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, uid, e.uid());
    uid.clear();
    EXPECT_NO_THROW(e.uid(uid));
    EXPECT_TRUE(e.uid().empty());
    uid.assign(16, 0x00);
    EXPECT_THROW(e.uid(uid), celduin::ValueOutOfRange);
}


TEST(SegmentInfo, SegmentFileName)
{
    celduin::SegmentInfo e;
    EXPECT_TRUE(e.filename().empty());
    std::string fn("a_file.celduin");
    EXPECT_NO_THROW(e.filename(fn));
    EXPECT_EQ(fn, e.filename());
    fn.clear();
    EXPECT_NO_THROW(e.filename(fn));
    EXPECT_TRUE(e.filename().empty());
}


TEST(SegmentInfo, PrevUID)
{
    celduin::SegmentInfo e;
    std::vector<char> uid;
    EXPECT_TRUE(e.prev_uid().empty());
    uid.push_back(0x01);
    uid.push_back(0x02);
    uid.push_back(0x03);
    uid.push_back(0x04);
    EXPECT_THROW(e.prev_uid(uid), celduin::ValueSizeOutOfRange);
    uid.push_back(0x05);
    uid.push_back(0x06);
    uid.push_back(0x07);
    uid.push_back(0x08);
    uid.push_back(0x09);
    uid.push_back(0x0A);
    uid.push_back(0x0B);
    uid.push_back(0x0C);
    uid.push_back(0x0D);
    uid.push_back(0x0D);
    uid.push_back(0x0E);
    uid.push_back(0x0F);
    EXPECT_NO_THROW(e.prev_uid(uid));
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, uid, e.prev_uid());
    uid.clear();
    EXPECT_NO_THROW(e.prev_uid(uid));
    EXPECT_TRUE(e.prev_uid().empty());
    uid.assign(8, 0x00);
    EXPECT_THROW(e.prev_uid(uid), celduin::ValueOutOfRange);
}


TEST(SegmentInfo, PrevFileName)
{
    celduin::SegmentInfo e;
    EXPECT_TRUE(e.prev_filename().empty());
    std::string fn("a_file.celduin");
    EXPECT_NO_THROW(e.prev_filename(fn));
    EXPECT_EQ(fn, e.prev_filename());
    fn.clear();
    EXPECT_NO_THROW(e.prev_filename(fn));
    EXPECT_TRUE(e.prev_filename().empty());
}


TEST(SegmentInfo, NextUID)
{
    celduin::SegmentInfo e;
    std::vector<char> uid;
    EXPECT_TRUE(e.next_uid().empty());
    uid.push_back(0x01);
    uid.push_back(0x02);
    uid.push_back(0x03);
    uid.push_back(0x04);
    EXPECT_THROW(e.next_uid(uid), celduin::ValueSizeOutOfRange);
    uid.push_back(0x05);
    uid.push_back(0x06);
    uid.push_back(0x07);
    uid.push_back(0x08);
    uid.push_back(0x09);
    uid.push_back(0x0A);
    uid.push_back(0x0B);
    uid.push_back(0x0C);
    uid.push_back(0x0D);
    uid.push_back(0x0D);
    uid.push_back(0x0E);
    uid.push_back(0x0F);
    EXPECT_NO_THROW(e.next_uid(uid));
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, uid, e.next_uid());
    uid.clear();
    EXPECT_NO_THROW(e.next_uid(uid));
    EXPECT_TRUE(e.next_uid().empty());
    uid.assign(16, 0x00);
    EXPECT_THROW(e.next_uid(uid), celduin::ValueOutOfRange);
}


TEST(SegmentInfo, NextFileName)
{
    celduin::SegmentInfo e;
    EXPECT_TRUE(e.next_filename().empty());
    std::string fn("a_file.celduin");
    EXPECT_NO_THROW(e.next_filename(fn));
    EXPECT_EQ(fn, e.next_filename());
    fn.clear();
    EXPECT_NO_THROW(e.next_filename(fn));
    EXPECT_TRUE(e.next_filename().empty());
}


TEST(SegmentInfo, SegmentFamily)
{
    celduin::SegmentInfo e;
    std::vector<char> uid;
    EXPECT_TRUE(e.segment_family().empty());
    uid.push_back(0x01);
    uid.push_back(0x02);
    uid.push_back(0x03);
    uid.push_back(0x04);
    EXPECT_THROW(e.segment_family(uid), celduin::ValueSizeOutOfRange);
    uid.push_back(0x05);
    uid.push_back(0x06);
    uid.push_back(0x07);
    uid.push_back(0x08);
    uid.push_back(0x09);
    uid.push_back(0x0A);
    uid.push_back(0x0B);
    uid.push_back(0x0C);
    uid.push_back(0x0D);
    uid.push_back(0x0D);
    uid.push_back(0x0E);
    uid.push_back(0x0F);
    EXPECT_NO_THROW(e.segment_family(uid));
    EXPECT_PRED_FORMAT2(test_utils::std_vectors_eq, uid, e.segment_family());
    uid.clear();
    EXPECT_NO_THROW(e.segment_family(uid));
    EXPECT_TRUE(e.segment_family().empty());
    uid.assign(16, 0x00);
    EXPECT_THROW(e.segment_family(uid), celduin::ValueOutOfRange);
}


TEST(SegmentInfo, TimecodeScale)
{
    celduin::SegmentInfo e;
    EXPECT_EQ(1000000, e.timecode_scale());
    EXPECT_NO_THROW(e.timecode_scale(314159));
    EXPECT_EQ(314159, e.timecode_scale());
    EXPECT_NO_THROW(e.timecode_scale(0));
    EXPECT_EQ(1000000, e.timecode_scale());
}


TEST(SegmentInfo, Duration)
{
    celduin::SegmentInfo e;
    EXPECT_TRUE(e.duration() > 0);
    EXPECT_NO_THROW(e.duration(314159));
    EXPECT_EQ(314159, e.duration());
    EXPECT_THROW(e.duration(-314159), celduin::ValueOutOfRange);
    EXPECT_THROW(e.duration(0), celduin::ValueOutOfRange);
    // Test 64-bit values to confirm range
    EXPECT_NO_THROW(e.duration(0xFFFFFFFFFFFFFFFF));
    EXPECT_EQ(0xFFFFFFFFFFFFFFFF, e.duration());
}


TEST(SegmentInfo, Date)
{
    celduin::SegmentInfo e;
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
    celduin::SegmentInfo e;
    EXPECT_TRUE(e.title().empty());
    std::string fn("a_file.celduin");
    EXPECT_NO_THROW(e.title(fn));
    EXPECT_EQ(fn, e.title());
    fn.clear();
    EXPECT_NO_THROW(e.title(fn));
    EXPECT_TRUE(e.title().empty());
}


TEST(SegmentInfo, MuxingApp)
{
    celduin::SegmentInfo e;
    EXPECT_TRUE(e.muxing_app().empty());
    std::string fn("a_file.celduin");
    EXPECT_NO_THROW(e.muxing_app(fn));
    EXPECT_EQ(fn, e.muxing_app());
    fn.clear();
    EXPECT_NO_THROW(e.muxing_app(fn));
    EXPECT_TRUE(e.muxing_app().empty());
}


TEST(SegmentInfo, WritingApp)
{
    celduin::SegmentInfo e;
    EXPECT_TRUE(e.writing_app().empty());
    std::string fn("a_file.celduin");
    EXPECT_NO_THROW(e.writing_app(fn));
    EXPECT_EQ(fn, e.writing_app());
    fn.clear();
    EXPECT_NO_THROW(e.writing_app(fn));
    EXPECT_TRUE(e.writing_app().empty());
}


namespace test_seg_info
{

void DoWriteTest(std::vector<test_utils::ElPtr> const& els,
        celduin::SegmentInfo& e, std::string msg)
{
    std::ostringstream output;
    std::stringstream expected;

    std::streamsize expected_size(std::accumulate(els.begin(), els.end(), 0,
                test_utils::TotalSizeOp()));
    celduin::ids::write(celduin::ids::Info, expected);
    celduin::vint::write(expected_size, expected);
    BOOST_FOREACH(test_utils::ElPtr el, els)
    {
        el->write(expected);
    }
    EXPECT_EQ(celduin::ids::size(celduin::ids::Info) +
            celduin::vint::size(expected_size) + expected_size,
            e.write(output)) << msg;
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str()) << msg;
}

}; // namespace test_seg_info


TEST(SegmentInfo, Size)
{
    std::vector<test_utils::ElPtr> all_children, used_children;

    all_children.push_back(test_utils::ElPtr(new
                celduin::BinaryElement(celduin::ids::SegmentUID, std::vector<char>(16,
                        0xC0))));
    all_children.push_back(test_utils::ElPtr(new
                celduin::StringElement(celduin::ids::SegmentFileName,
                    "Segment filename")));
    all_children.push_back(test_utils::ElPtr(new
                celduin::BinaryElement(celduin::ids::PrevUID,
                    std::vector<char>(16, 0xC0))));
    all_children.push_back(test_utils::ElPtr(new
                celduin::StringElement(celduin::ids::PrevFileName,
                    "Previous filename")));
    all_children.push_back(test_utils::ElPtr(new
                celduin::BinaryElement(celduin::ids::NextUID,
                    std::vector<char>(16, 0xC0))));
    all_children.push_back(test_utils::ElPtr(new
                celduin::StringElement(celduin::ids::NextFileName,
                    "Next filename")));
    all_children.push_back(test_utils::ElPtr(new
                celduin::BinaryElement(celduin::ids::SegmentFamily,
                    std::vector<char>(16, 0xC0))));
    all_children.push_back(test_utils::ElPtr(new
                celduin::FloatElement(celduin::ids::Duration, 12345)));
    all_children.push_back(test_utils::ElPtr(new
                celduin::DateElement(celduin::ids::DateUTC, 0xFFFFFFFFFF)));
    all_children.push_back(test_utils::ElPtr(new
                celduin::StringElement(celduin::ids::Title, "Title")));
    all_children.push_back(test_utils::ElPtr(new
                celduin::StringElement(celduin::ids::MuxingApp, "Muxer")));
    all_children.push_back(test_utils::ElPtr(new
                celduin::StringElement(celduin::ids::WritingApp, "Writer")));
    used_children.push_back(test_utils::ElPtr(new
                celduin::UIntElement(celduin::ids::TimecodeScale, 1000000)));

    celduin::SegmentInfo e;
    EXPECT_EQ(celduin::ids::size(celduin::ids::Info) +
            celduin::vint::size(used_children[0]->size()) +
            used_children[0]->size(), e.size());

    std::streamsize body_size(0);
    e.uid(std::vector<char>(16, 0xC0));
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(celduin::ids::size(celduin::ids::Info) +
            celduin::vint::size(body_size) + body_size, e.size());

    body_size = 0;
    e.filename("Segment filename");
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(celduin::ids::size(celduin::ids::Info) +
            celduin::vint::size(body_size) + body_size, e.size());

    body_size = 0;
    e.prev_uid(std::vector<char>(16, 0xC0));
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(celduin::ids::size(celduin::ids::Info) +
            celduin::vint::size(body_size) + body_size, e.size());

    body_size = 0;
    e.prev_filename("Previous filename");
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(celduin::ids::size(celduin::ids::Info) +
            celduin::vint::size(body_size) + body_size, e.size());

    body_size = 0;
    e.next_uid(std::vector<char>(16, 0xC0));
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(celduin::ids::size(celduin::ids::Info) +
            celduin::vint::size(body_size) + body_size, e.size());

    body_size = 0;
    e.next_filename("Next filename");
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(celduin::ids::size(celduin::ids::Info) +
            celduin::vint::size(body_size) + body_size, e.size());

    body_size = 0;
    e.segment_family(std::vector<char>(16, 0xC0));
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(celduin::ids::size(celduin::ids::Info) +
            celduin::vint::size(body_size) + body_size, e.size());

    body_size = 0;
    e.duration(12345);
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(celduin::ids::size(celduin::ids::Info) +
            celduin::vint::size(body_size) + body_size, e.size());

    body_size = 0;
    e.date(0xFFFFFFFFFF);
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(celduin::ids::size(celduin::ids::Info) +
            celduin::vint::size(body_size) + body_size, e.size());

    body_size = 0;
    e.title("Title");
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(celduin::ids::size(celduin::ids::Info) +
            celduin::vint::size(body_size) + body_size, e.size());

    body_size = 0;
    e.muxing_app("Muxer");
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(celduin::ids::size(celduin::ids::Info) +
            celduin::vint::size(body_size) + body_size, e.size());

    body_size = 0;
    e.writing_app("Writer");
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    body_size = std::accumulate(used_children.begin(), used_children.end(),
            0, test_utils::TotalSizeOp());
    EXPECT_EQ(celduin::ids::size(celduin::ids::Info) +
            celduin::vint::size(body_size) + body_size, e.size());
}


TEST(SegmentInfo, Write)
{
    std::vector<test_utils::ElPtr> all_children, used_children;
    all_children.push_back(test_utils::ElPtr(new
                celduin::BinaryElement(celduin::ids::SegmentUID, std::vector<char>(16,
                        0xC0))));
    all_children.push_back(test_utils::ElPtr(new
                celduin::StringElement(celduin::ids::SegmentFileName,
                    "Segment filename")));
    all_children.push_back(test_utils::ElPtr(new
                celduin::BinaryElement(celduin::ids::PrevUID,
                    std::vector<char>(16, 0xC0))));
    all_children.push_back(test_utils::ElPtr(new
                celduin::StringElement(celduin::ids::PrevFileName,
                    "Previous filename")));
    all_children.push_back(test_utils::ElPtr(new
                celduin::BinaryElement(celduin::ids::NextUID,
                    std::vector<char>(16, 0xC0))));
    all_children.push_back(test_utils::ElPtr(new
                celduin::StringElement(celduin::ids::NextFileName,
                    "Next filename")));
    all_children.push_back(test_utils::ElPtr(new
                celduin::BinaryElement(celduin::ids::SegmentFamily,
                    std::vector<char>(16, 0xC0))));
    all_children.push_back(test_utils::ElPtr(new
                celduin::FloatElement(celduin::ids::Duration, 12345)));
    all_children.push_back(test_utils::ElPtr(new
                celduin::DateElement(celduin::ids::DateUTC, 0xFFFFFFFFFF)));
    all_children.push_back(test_utils::ElPtr(new
                celduin::StringElement(celduin::ids::Title, "Title")));
    all_children.push_back(test_utils::ElPtr(new
                celduin::StringElement(celduin::ids::MuxingApp, "Muxer")));
    all_children.push_back(test_utils::ElPtr(new
                celduin::StringElement(celduin::ids::WritingApp, "Writer")));
    used_children.push_back(test_utils::ElPtr(new
                celduin::UIntElement(celduin::ids::TimecodeScale, 1000000)));

    celduin::SegmentInfo e;
    test_seg_info::DoWriteTest(used_children, e, "tc_scale");

    e.uid(std::vector<char>(16, 0xC0));
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    test_seg_info::DoWriteTest(used_children, e, "uid");

    e.filename("Segment filename");
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    test_seg_info::DoWriteTest(used_children, e, "filename");

    e.prev_uid(std::vector<char>(16, 0xC0));
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    test_seg_info::DoWriteTest(used_children, e, "prev_uid");

    e.prev_filename("Previous filename");
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    test_seg_info::DoWriteTest(used_children, e, "prev_filename");

    e.next_uid(std::vector<char>(16, 0xC0));
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    test_seg_info::DoWriteTest(used_children, e, "next_uid");

    e.next_filename("Next filename");
    used_children.push_back(all_children[0]);
    all_children.erase(all_children.begin());
    test_seg_info::DoWriteTest(used_children, e, "next_filename");

    e.segment_family(std::vector<char>(16, 0xC0));
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
                celduin::BinaryElement(celduin::ids::SegmentUID, std::vector<char>(16,
                        0xC0))));
    all_children.push_back(test_utils::ElPtr(new
                celduin::StringElement(celduin::ids::SegmentFileName,
                    "Segment filename")));
    all_children.push_back(test_utils::ElPtr(new
                celduin::BinaryElement(celduin::ids::PrevUID,
                    std::vector<char>(16, 0xC0))));
    all_children.push_back(test_utils::ElPtr(new
                celduin::StringElement(celduin::ids::PrevFileName,
                    "Previous filename")));
    all_children.push_back(test_utils::ElPtr(new
                celduin::BinaryElement(celduin::ids::NextUID,
                    std::vector<char>(16, 0xC0))));
    all_children.push_back(test_utils::ElPtr(new
                celduin::StringElement(celduin::ids::NextFileName,
                    "Next filename")));
    all_children.push_back(test_utils::ElPtr(new
                celduin::BinaryElement(celduin::ids::SegmentFamily,
                    std::vector<char>(16, 0xC0))));
    all_children.push_back(test_utils::ElPtr(new
                celduin::FloatElement(celduin::ids::Duration, 12345)));
    all_children.push_back(test_utils::ElPtr(new
                celduin::DateElement(celduin::ids::DateUTC, 0xFFFFFFFFFF)));
    all_children.push_back(test_utils::ElPtr(new
                celduin::StringElement(celduin::ids::Title, "Title")));
    all_children.push_back(test_utils::ElPtr(new
                celduin::StringElement(celduin::ids::MuxingApp, "Muxer")));
    all_children.push_back(test_utils::ElPtr(new
                celduin::StringElement(celduin::ids::WritingApp, "Writer")));
    used_children.push_back(test_utils::ElPtr(new
                celduin::UIntElement(celduin::ids::TimecodeScale, 200000)));

    celduin::SegmentInfo e;
    celduin::vint::write(0, input);
    EXPECT_EQ(celduin::vint::size(0), e.read(input));
    EXPECT_EQ(1000000, e.timecode_scale());

    input.str(std::string());
    std::streamsize body_size(used_children[0]->size());
    celduin::vint::write(body_size, input);
    used_children[0]->write(input);
    EXPECT_EQ(celduin::vint::size(body_size) + body_size,
            e.read(input));
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
        celduin::vint::write(body_size, input);
        BOOST_FOREACH(test_utils::ElPtr uel, used_children)
        {
            uel->write(input);
        }
        EXPECT_EQ(celduin::vint::size(body_size) + body_size,
            e.read(input));
    }

    // Body size value wrong (too small)
    input.str(std::string());
    celduin::vint::write(2, input);
    used_children[0]->write(input);
    used_children[3]->write(input);
    EXPECT_THROW(e.read(input), celduin::BadBodySize);
    // Invalid child
    input.str(std::string());
    celduin::UIntElement ue(celduin::ids::EBML, 0xFFFF);
    celduin::vint::write(ue.size(), input);
    ue.write(input);
    EXPECT_THROW(e.read(input), celduin::InvalidChildID);
}

