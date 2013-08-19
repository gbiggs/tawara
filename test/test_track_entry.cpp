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
#include <tawara/binary_element.h>
#include <tawara/el_ids.h>
#include <tawara/exceptions.h>
#include <tawara/float_element.h>
#include <tawara/string_element.h>
#include <tawara/track_entry.h>
#include <tawara/uint_element.h>
#include <tawara/vint.h>

#include "test_utils.h"


TEST(TrackEntry, Create)
{
    tawara::TrackEntry e(1, 2, "MDCC");
    EXPECT_EQ(tawara::ids::TrackEntry, e.id());
    EXPECT_EQ(1, e.number());
    EXPECT_EQ(2, e.uid());
    EXPECT_EQ(0x70, e.type());
    EXPECT_TRUE(e.enabled());
    EXPECT_FALSE(e.forced());
    EXPECT_TRUE(e.lacing());
    EXPECT_EQ(0, e.min_cache());
    EXPECT_EQ(0, e.max_cache());
    EXPECT_EQ(0, e.default_duration());
    EXPECT_EQ(1.0, e.timecode_scale());
    EXPECT_EQ(0, e.max_block_add_id());
    EXPECT_EQ("", e.name());
    EXPECT_EQ("MDCC", e.codec_id());
    EXPECT_EQ(0, e.codec_private().size());
    EXPECT_EQ("", e.codec_name());
    EXPECT_EQ(0, e.attachment_link());
    EXPECT_FALSE(e.decode_all());
    EXPECT_EQ(0, e.overlays().size());
    EXPECT_FALSE(e.is_virtual());

    EXPECT_THROW(tawara::TrackEntry(0, 2, "MDCC"), tawara::ValueOutOfRange);
    EXPECT_THROW(tawara::TrackEntry(1, 0, "MDCC"), tawara::ValueOutOfRange);
    EXPECT_THROW(tawara::TrackEntry(1, 2, ""), tawara::ValueOutOfRange);
}


TEST(TrackEntry, Number)
{
    tawara::TrackEntry e(1, 2, "MDCC");
    EXPECT_EQ(1, e.number());
    e.number(3);
    EXPECT_EQ(3, e.number());
    EXPECT_THROW(e.number(0), tawara::ValueOutOfRange);
}


TEST(TrackEntry, UID)
{
    tawara::TrackEntry e(1, 2, "MDCC");
    EXPECT_EQ(2, e.uid());
    e.uid(3);
    EXPECT_EQ(3, e.uid());
    EXPECT_THROW(e.uid(0), tawara::ValueOutOfRange);
}


TEST(TrackEntry, Type)
{
    tawara::TrackEntry e(1, 2, "MDCC");
    EXPECT_EQ(0x70, e.type());
    e.type(0x11);
    EXPECT_EQ(0x11, e.type());
    EXPECT_THROW(e.type(0xFF), tawara::ValueOutOfRange);
}


TEST(TrackEntry, Enabled)
{
    tawara::TrackEntry e(1, 2, "MDCC");
    EXPECT_TRUE(e.enabled());
    e.enabled(false);
    EXPECT_FALSE(e.enabled());
}


TEST(TrackEntry, Forced)
{
    tawara::TrackEntry e(1, 2, "MDCC");
    EXPECT_FALSE(e.forced());
    e.forced(true);
    EXPECT_TRUE(e.forced());
}


TEST(TrackEntry, Lacing)
{
    tawara::TrackEntry e(1, 2, "MDCC");
    EXPECT_TRUE(e.lacing());
    e.lacing(false);
    EXPECT_FALSE(e.lacing());
}


TEST(TrackEntry, MinCache)
{
    tawara::TrackEntry e(1, 2, "MDCC");
    EXPECT_EQ(0, e.min_cache());
    e.min_cache(5);
    EXPECT_EQ(5, e.min_cache());
}


TEST(TrackEntry, MaxCache)
{
    tawara::TrackEntry e(1, 2, "MDCC");
    EXPECT_EQ(0, e.max_cache());
    e.max_cache(5);
    EXPECT_EQ(5, e.max_cache());
}


TEST(TrackEntry, DefaultDuration)
{
    tawara::TrackEntry e(1, 2, "MDCC");
    EXPECT_EQ(0, e.default_duration());
    e.default_duration(5);
    EXPECT_EQ(5, e.default_duration());
}


TEST(TrackEntry, TimecodeScale)
{
    tawara::TrackEntry e(1, 2, "MDCC");
    EXPECT_DOUBLE_EQ(1.0, e.timecode_scale());
    e.timecode_scale(0.75);
    EXPECT_DOUBLE_EQ(0.75, e.timecode_scale());
    EXPECT_THROW(e.timecode_scale(0.0), tawara::ValueOutOfRange);
    EXPECT_THROW(e.timecode_scale(-1.0), tawara::ValueOutOfRange);
}


TEST(TrackEntry, MaxBlockAddID)
{
    tawara::TrackEntry e(1, 2, "MDCC");
    EXPECT_EQ(0, e.max_block_add_id());
    e.max_block_add_id(0xFFFF);
    EXPECT_EQ(0xFFFF, e.max_block_add_id());
}


TEST(TrackEntry, Name)
{
    tawara::TrackEntry e(1, 2, "MDCC");
    EXPECT_EQ("", e.name());
    e.name("Blag");
    EXPECT_EQ("Blag", e.name());
}


TEST(TrackEntry, CodecID)
{
    tawara::TrackEntry e(1, 2, "MDCC");
    EXPECT_EQ("MDCC", e.codec_id());
    e.codec_id("SCDC");
    EXPECT_EQ("SCDC", e.codec_id());
    EXPECT_THROW(e.codec_id(""), tawara::ValueOutOfRange);
}


TEST(TrackEntry, CodecPrivate)
{
    tawara::TrackEntry e(1, 2, "MDCC");
    EXPECT_EQ(0, e.codec_private().size());
    std::vector<char> priv_data;
    priv_data.push_back(0x00);
    priv_data.push_back(0x01);
    priv_data.push_back(0x02);
    priv_data.push_back(0x03);
    e.codec_private(priv_data);
    EXPECT_EQ(4, e.codec_private().size());
    EXPECT_EQ(0x02, e.codec_private()[2]);
}


TEST(TrackEntry, CodecName)
{
    tawara::TrackEntry e(1, 2, "MDCC");
    EXPECT_EQ("", e.codec_name());
    e.codec_name("Blag");
    EXPECT_EQ("Blag", e.codec_name());
}


TEST(TrackEntry, AttachmentLink)
{
    tawara::TrackEntry e(1, 2, "MDCC");
    EXPECT_EQ(0, e.attachment_link());
    e.attachment_link(42);
    EXPECT_EQ(42, e.attachment_link());
}


TEST(TrackEntry, DecodeAll)
{
    tawara::TrackEntry e(1, 2, "MDCC");
    EXPECT_FALSE(e.decode_all());
    e.decode_all(true);
    EXPECT_TRUE(e.decode_all());
}


TEST(TrackEntry, Overlays)
{
    tawara::TrackEntry e(1, 2, "MDCC");
    EXPECT_EQ(0, e.overlays().size());
    std::vector<uint64_t> overlays;
    overlays.push_back(0);
    overlays.push_back(1);
    overlays.push_back(2);
    overlays.push_back(3);
    e.overlays(overlays);
    EXPECT_EQ(4, e.overlays().size());
    EXPECT_EQ(2, e.overlays()[2]);
}


TEST(TrackEntry, Operation)
{
    tawara::TrackEntry e(1, 2, "MDCC");
    EXPECT_FALSE(e.is_virtual());
    EXPECT_FALSE(e.operation());
    boost::shared_ptr<tawara::TrackJoinBlocks> op(new tawara::TrackJoinBlocks);
    op->append(42);
    e.operation(op);
    EXPECT_TRUE(e.is_virtual());
    EXPECT_EQ(op, e.operation());
}


TEST(TrackEntry, Equality)
{
    tawara::TrackEntry e1(1, 2, "MDCC");
    tawara::TrackEntry e2(1, 2, "MDCC");
    std::vector<uint64_t> overlays;
    overlays.push_back(0);
    overlays.push_back(1);
    overlays.push_back(2);
    overlays.push_back(3);
    e1.overlays(overlays);
    e2.overlays(overlays);
    boost::shared_ptr<tawara::TrackJoinBlocks> op(new tawara::TrackJoinBlocks);
    op->append(42);
    e1.operation(op);
    e2.operation(op);
    EXPECT_TRUE(e1 == e2);
    EXPECT_FALSE(e1 != e2);

    e1.name("Another track");
    EXPECT_FALSE(e1 == e2);
    EXPECT_TRUE(e1 != e2);
}


namespace test_track_entry
{

void DoSizeTest(std::vector<test_utils::ElPtr> const& els,
        tawara::TrackEntry& e, std::string msg)
{
    std::streamsize body_size(0);
    BOOST_FOREACH(test_utils::ElPtr el, els)
    {
        body_size += el->size();
    }
    EXPECT_EQ(tawara::ids::size(tawara::ids::TrackEntry) +
            tawara::vint::size(body_size) + body_size,
            e.size()) << msg;
}


void DoWriteTest(std::vector<test_utils::ElPtr> const& els,
        tawara::TrackEntry& e, std::string msg)
{
    std::ostringstream output;
    std::stringstream expected;

    std::streamsize expected_size(std::accumulate(els.begin(), els.end(), 0,
                test_utils::TotalSizeOp()));
    tawara::ids::write(tawara::ids::TrackEntry, expected);
    tawara::vint::write(expected_size, expected);
    BOOST_FOREACH(test_utils::ElPtr el, els)
    {
        el->write(expected);
    }
    EXPECT_EQ(tawara::ids::size(tawara::ids::TrackEntry) +
            tawara::vint::size(expected_size) + expected_size,
            e.write(output)) << msg;
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str()) << msg;
}


std::vector<test_utils::ElPtr> default_children()
{
    std::vector<test_utils::ElPtr> result;

    result.push_back(test_utils::ElPtr(new
            tawara::UIntElement(tawara::ids::TrackNumber, 1)));
    result.push_back(test_utils::ElPtr(new
            tawara::UIntElement(tawara::ids::TrackUID, 2)));
    result.push_back(test_utils::ElPtr(new
            tawara::UIntElement(tawara::ids::TrackType, 0x70)));
    result.push_back(test_utils::ElPtr(new
            tawara::StringElement(tawara::ids::CodecID, "MDCC")));

    return result;
}


std::vector<test_utils::ElPtr> required_children()
{
    std::vector<test_utils::ElPtr> result;

    result.push_back(test_utils::ElPtr(new
            tawara::UIntElement(tawara::ids::TrackNumber, 4)));
    result.push_back(test_utils::ElPtr(new
            tawara::UIntElement(tawara::ids::TrackUID, 42)));
    result.push_back(test_utils::ElPtr(new
            tawara::UIntElement(tawara::ids::TrackType, 0x11)));
    result.push_back(test_utils::ElPtr(new
            tawara::StringElement(tawara::ids::CodecID, "SCDC")));

    return result;
}


std::vector<test_utils::ElPtr> possible_children()
{
    std::vector<test_utils::ElPtr> result;

    result.push_back(test_utils::ElPtr(new
            tawara::UIntElement(tawara::ids::FlagEnabled, 0)));
    result.push_back(test_utils::ElPtr(new
            tawara::UIntElement(tawara::ids::FlagForced, 1)));
    result.push_back(test_utils::ElPtr(new
            tawara::UIntElement(tawara::ids::FlagLacing, 0)));
    result.push_back(test_utils::ElPtr(new
            tawara::UIntElement(tawara::ids::MinCache, 24)));
    result.push_back(test_utils::ElPtr(new
            tawara::UIntElement(tawara::ids::MaxCache, 42)));
    result.push_back(test_utils::ElPtr(new
            tawara::UIntElement(tawara::ids::DefaultDuration, 37)));
    result.push_back(test_utils::ElPtr(new
            tawara::FloatElement(tawara::ids::TrackTimecodeScale, 0.5)));
    result.push_back(test_utils::ElPtr(new
            tawara::UIntElement(tawara::ids::MaxBlockAdditionID, 21)));
    result.push_back(test_utils::ElPtr(new
            tawara::StringElement(tawara::ids::Name, "Blag")));
    std::vector<char> priv_data;
    priv_data.push_back(0x00);
    priv_data.push_back(0x01);
    priv_data.push_back(0x02);
    priv_data.push_back(0x03);
    result.push_back(test_utils::ElPtr(new
            tawara::BinaryElement(tawara::ids::CodecPrivate, priv_data)));
    result.push_back(test_utils::ElPtr(new
            tawara::StringElement(tawara::ids::CodecName,
                "Super-Cool Data Codec")));
    result.push_back(test_utils::ElPtr(new
            tawara::UIntElement(tawara::ids::AttachmentLink, 84)));
    result.push_back(test_utils::ElPtr(new
            tawara::UIntElement(tawara::ids::CodecDecodeAll, 1)));
    result.push_back(test_utils::ElPtr(new
            tawara::UIntElement(tawara::ids::TrackOverlay, 168)));
    result.push_back(test_utils::ElPtr(new
            tawara::UIntElement(tawara::ids::TrackOverlay, 336)));

    return result;
}


}; // namespace test_track_entry


TEST(TrackEntry, Size)
{
    tawara::TrackEntry e(1, 2, "MDCC");
    std::streamsize body_size(0);
    BOOST_FOREACH(test_utils::ElPtr el, test_track_entry::required_children())
    {
        body_size += el->size();
    }
    EXPECT_EQ(tawara::ids::size(tawara::ids::TrackEntry) +
            tawara::vint::size(body_size) + body_size,
            e.size());

    std::vector<test_utils::ElPtr>
        used_children(test_track_entry::required_children());
    std::vector<test_utils::ElPtr>
        pos_children(test_track_entry::possible_children());
    test_track_entry::DoSizeTest(used_children, e, "empty");
    e.number(4);
    e.uid(42);
    e.type(0x11);
    e.codec_id("SCDC");
    test_track_entry::DoSizeTest(used_children, e, "required only");

    e.enabled(false);
    used_children.push_back(pos_children[0]);
    pos_children.erase(pos_children.begin());
    test_track_entry::DoSizeTest(used_children, e, "enabled");

    e.forced(true);
    used_children.push_back(pos_children[0]);
    pos_children.erase(pos_children.begin());
    test_track_entry::DoSizeTest(used_children, e, "forced");

    e.lacing(false);
    used_children.push_back(pos_children[0]);
    pos_children.erase(pos_children.begin());
    test_track_entry::DoSizeTest(used_children, e, "lacing");

    e.min_cache(24);
    used_children.push_back(pos_children[0]);
    pos_children.erase(pos_children.begin());
    test_track_entry::DoSizeTest(used_children, e, "min_cache");

    e.max_cache(42);
    used_children.push_back(pos_children[0]);
    pos_children.erase(pos_children.begin());
    test_track_entry::DoSizeTest(used_children, e, "max_cache");

    e.default_duration(37);
    used_children.push_back(pos_children[0]);
    pos_children.erase(pos_children.begin());
    test_track_entry::DoSizeTest(used_children, e, "default_duration");

    e.timecode_scale(0.5);
    used_children.push_back(pos_children[0]);
    pos_children.erase(pos_children.begin());
    test_track_entry::DoSizeTest(used_children, e, "timecode_scale");

    e.max_block_add_id(21);
    used_children.push_back(pos_children[0]);
    pos_children.erase(pos_children.begin());
    test_track_entry::DoSizeTest(used_children, e, "max_block_add_id");

    e.name("Blag");
    used_children.push_back(pos_children[0]);
    pos_children.erase(pos_children.begin());
    test_track_entry::DoSizeTest(used_children, e, "name");

    std::vector<char> priv_data;
    priv_data.push_back(0x00);
    priv_data.push_back(0x01);
    priv_data.push_back(0x02);
    priv_data.push_back(0x03);
    e.codec_private(priv_data);
    used_children.push_back(pos_children[0]);
    pos_children.erase(pos_children.begin());
    test_track_entry::DoSizeTest(used_children, e, "codec_private");

    e.codec_name("Super-Cool Data Codec");
    used_children.push_back(pos_children[0]);
    pos_children.erase(pos_children.begin());
    test_track_entry::DoSizeTest(used_children, e, "codec_name");

    e.attachment_link(84);
    used_children.push_back(pos_children[0]);
    pos_children.erase(pos_children.begin());
    test_track_entry::DoSizeTest(used_children, e, "attachment_link");

    e.decode_all(true);
    used_children.push_back(pos_children[0]);
    pos_children.erase(pos_children.begin());
    test_track_entry::DoSizeTest(used_children, e, "decode_all");

    std::vector<uint64_t> overlays;
    overlays.push_back(168);
    overlays.push_back(336);
    e.overlays(overlays);
    used_children.push_back(pos_children[0]);
    pos_children.erase(pos_children.begin());
    used_children.push_back(pos_children[0]);
    pos_children.erase(pos_children.begin());
    test_track_entry::DoSizeTest(used_children, e, "overlays");

    boost::shared_ptr<tawara::TrackJoinBlocks> op(new tawara::TrackJoinBlocks);
    op->append(42);
    e.operation(op);
    body_size = std::accumulate(used_children.begin(),
                used_children.end(), 0, test_utils::TotalSizeOp());
    body_size += tawara::ids::size(tawara::ids::TrackOperation) +
        tawara::vint::size(op->size()) + op->size();
    EXPECT_EQ(tawara::ids::size(tawara::ids::TrackEntry) +
            tawara::vint::size(body_size) + body_size,
            e.size());
}


TEST(TrackEntry, Write)
{
    tawara::TrackEntry e(1, 2, "MDCC");
    test_track_entry::DoWriteTest(test_track_entry::default_children(), e,
            "empty");

    std::vector<test_utils::ElPtr>
        used_children(test_track_entry::required_children());
    e.number(4);
    e.uid(42);
    e.type(0x11);
    e.codec_id("SCDC");
    test_track_entry::DoWriteTest(used_children, e, "required only");

    std::vector<test_utils::ElPtr>
        pos_children(test_track_entry::possible_children());
    e.enabled(false);
    used_children.push_back(pos_children[0]);
    pos_children.erase(pos_children.begin());
    test_track_entry::DoWriteTest(used_children, e, "enabled");

    e.forced(true);
    used_children.push_back(pos_children[0]);
    pos_children.erase(pos_children.begin());
    test_track_entry::DoWriteTest(used_children, e, "forced");

    e.lacing(false);
    used_children.push_back(pos_children[0]);
    pos_children.erase(pos_children.begin());
    test_track_entry::DoWriteTest(used_children, e, "lacing");

    e.min_cache(24);
    used_children.push_back(pos_children[0]);
    pos_children.erase(pos_children.begin());
    test_track_entry::DoWriteTest(used_children, e, "min_cache");

    e.max_cache(42);
    used_children.push_back(pos_children[0]);
    pos_children.erase(pos_children.begin());
    test_track_entry::DoWriteTest(used_children, e, "max_cache");

    e.default_duration(37);
    used_children.push_back(pos_children[0]);
    pos_children.erase(pos_children.begin());
    test_track_entry::DoWriteTest(used_children, e, "default_duration");

    e.timecode_scale(0.5);
    used_children.push_back(pos_children[0]);
    pos_children.erase(pos_children.begin());
    test_track_entry::DoWriteTest(used_children, e, "timecode_scale");

    e.max_block_add_id(21);
    used_children.push_back(pos_children[0]);
    pos_children.erase(pos_children.begin());
    test_track_entry::DoWriteTest(used_children, e, "max_block_add_id");

    e.name("Blag");
    used_children.push_back(pos_children[0]);
    pos_children.erase(pos_children.begin());
    test_track_entry::DoWriteTest(used_children, e, "name");

    std::vector<char> priv_data;
    priv_data.push_back(0x00);
    priv_data.push_back(0x01);
    priv_data.push_back(0x02);
    priv_data.push_back(0x03);
    e.codec_private(priv_data);
    used_children.push_back(pos_children[0]);
    pos_children.erase(pos_children.begin());
    test_track_entry::DoWriteTest(used_children, e, "codec_private");

    e.codec_name("Super-Cool Data Codec");
    used_children.push_back(pos_children[0]);
    pos_children.erase(pos_children.begin());
    test_track_entry::DoWriteTest(used_children, e, "codec_name");

    e.attachment_link(84);
    used_children.push_back(pos_children[0]);
    pos_children.erase(pos_children.begin());
    test_track_entry::DoWriteTest(used_children, e, "attachment_link");

    e.decode_all(true);
    used_children.push_back(pos_children[0]);
    pos_children.erase(pos_children.begin());
    test_track_entry::DoWriteTest(used_children, e, "decode_all");

    std::vector<uint64_t> overlays;
    overlays.push_back(168);
    overlays.push_back(336);
    e.overlays(overlays);
    used_children.push_back(pos_children[0]);
    pos_children.erase(pos_children.begin());
    used_children.push_back(pos_children[0]);
    pos_children.erase(pos_children.begin());
    test_track_entry::DoWriteTest(used_children, e, "overlays");

    std::ostringstream output;
    std::stringstream expected;
    boost::shared_ptr<tawara::TrackJoinBlocks> op(new tawara::TrackJoinBlocks);
    op->append(42);
    e.operation(op);
    std::streamsize expected_size(std::accumulate(used_children.begin(),
                used_children.end(), 0, test_utils::TotalSizeOp()));
    expected_size += tawara::ids::size(tawara::ids::TrackOperation) +
        tawara::vint::size(op->size()) + op->size();
    tawara::ids::write(tawara::ids::TrackEntry, expected);
    tawara::vint::write(expected_size, expected);
    BOOST_FOREACH(test_utils::ElPtr el, used_children)
    {
        el->write(expected);
    }
    tawara::ids::write(tawara::ids::TrackOperation, expected);
    tawara::vint::write(op->size(), expected);
    op->write(expected);
    EXPECT_EQ(tawara::ids::size(tawara::ids::TrackEntry) +
            tawara::vint::size(expected_size) + expected_size,
            e.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
            expected.str());
}


TEST(TrackEntry, Read)
{
    std::stringstream input;
    std::vector<test_utils::ElPtr>
        used_children(test_track_entry::required_children());

    tawara::TrackEntry e(1, 2, "MDCC");

    std::streamsize body_size(std::accumulate(used_children.begin(),
                used_children.end(), 0, test_utils::TotalSizeOp()));
    tawara::vint::write(body_size, input);
    used_children[0]->write(input);
    used_children[1]->write(input);
    used_children[2]->write(input);
    used_children[3]->write(input);
    EXPECT_EQ(tawara::vint::size(body_size) + body_size,
            e.read(input));
    EXPECT_EQ(4, e.number());
    EXPECT_EQ(42, e.uid());
    EXPECT_EQ(0x11, e.type());
    EXPECT_TRUE(e.enabled());
    EXPECT_FALSE(e.forced());
    EXPECT_TRUE(e.lacing());
    EXPECT_EQ(0, e.min_cache());
    EXPECT_EQ(0, e.max_cache());
    EXPECT_EQ(0, e.default_duration());
    EXPECT_EQ(1.0, e.timecode_scale());
    EXPECT_EQ(0, e.max_block_add_id());
    EXPECT_EQ("", e.name());
    EXPECT_EQ("SCDC", e.codec_id());
    EXPECT_EQ(0, e.codec_private().size());
    EXPECT_EQ("", e.codec_name());
    EXPECT_EQ(0, e.attachment_link());
    EXPECT_FALSE(e.decode_all());
    EXPECT_EQ(0, e.overlays().size());
    EXPECT_FALSE(e.is_virtual());

    BOOST_FOREACH(test_utils::ElPtr el, test_track_entry::possible_children())
    {
        input.str(std::string());
        used_children.push_back(el);
        body_size = std::accumulate(used_children.begin(), used_children.end(),
                0, test_utils::TotalSizeOp());
        tawara::vint::write(body_size, input);
        BOOST_FOREACH(test_utils::ElPtr uel, used_children)
        {
            uel->write(input);
        }
        EXPECT_EQ(tawara::vint::size(body_size) + body_size,
            e.read(input));
    }

    // Body size value wrong (too small)
    input.str(std::string());
    tawara::vint::write(2, input);
    used_children[0]->write(input);
    used_children[1]->write(input);
    used_children[2]->write(input);
    used_children[3]->write(input);
    EXPECT_THROW(e.read(input), tawara::BadBodySize);
    // Invalid child
    input.str(std::string());
    tawara::UIntElement ue(tawara::ids::EBML, 0xFFFF);
    tawara::vint::write(ue.size(), input);
    ue.write(input);
    EXPECT_THROW(e.read(input), tawara::InvalidChildID);
    // Missing children
    input.str(std::string());
    tawara::vint::write(used_children[1]->size() +
            used_children[2]->size()+
            used_children[3]->size(), input);
    used_children[1]->write(input);
    used_children[2]->write(input);
    used_children[3]->write(input);
    EXPECT_THROW(e.read(input), tawara::MissingChild);
    input.str(std::string());
    tawara::vint::write(used_children[0]->size() +
            used_children[2]->size()+
            used_children[3]->size(), input);
    used_children[0]->write(input);
    used_children[2]->write(input);
    used_children[3]->write(input);
    EXPECT_THROW(e.read(input), tawara::MissingChild);
    input.str(std::string());
    tawara::vint::write(used_children[0]->size() +
            used_children[1]->size()+
            used_children[3]->size(), input);
    used_children[0]->write(input);
    used_children[1]->write(input);
    used_children[3]->write(input);
    EXPECT_THROW(e.read(input), tawara::MissingChild);
    input.str(std::string());
    tawara::vint::write(used_children[0]->size() +
            used_children[1]->size()+
            used_children[2]->size(), input);
    used_children[0]->write(input);
    used_children[1]->write(input);
    used_children[2]->write(input);
    EXPECT_THROW(e.read(input), tawara::MissingChild);
}


TEST(TrackEntry, ReadOutOfRangeValues)
{
    std::stringstream input;
    tawara::TrackEntry e(1, 2, "MDCC");

    tawara::UIntElement bad(tawara::ids::TrackNumber, 0);
    tawara::vint::write(bad.size(), input);
    bad.write(input);
    EXPECT_THROW(e.read(input), tawara::ValueOutOfRange);

    input.str(std::string());
    bad.id(tawara::ids::TrackUID);
    bad = 0;
    tawara::vint::write(bad.size(), input);
    bad.write(input);
    EXPECT_THROW(e.read(input), tawara::ValueOutOfRange);

    input.str(std::string());
    bad.id(tawara::ids::TrackType);
    bad = 255;
    tawara::vint::write(bad.size(), input);
    bad.write(input);
    EXPECT_THROW(e.read(input), tawara::ValueOutOfRange);

    input.str(std::string());
    bad.id(tawara::ids::FlagEnabled);
    bad = 2;
    tawara::vint::write(bad.size(), input);
    bad.write(input);
    EXPECT_THROW(e.read(input), tawara::ValueOutOfRange);

    input.str(std::string());
    bad.id(tawara::ids::FlagForced);
    bad = 2;
    tawara::vint::write(bad.size(), input);
    bad.write(input);
    EXPECT_THROW(e.read(input), tawara::ValueOutOfRange);

    input.str(std::string());
    bad.id(tawara::ids::FlagLacing);
    bad = 2;
    tawara::vint::write(bad.size(), input);
    bad.write(input);
    EXPECT_THROW(e.read(input), tawara::ValueOutOfRange);

    input.str(std::string());
    bad.id(tawara::ids::DefaultDuration);
    bad = 0;
    tawara::vint::write(bad.size(), input);
    bad.write(input);
    EXPECT_THROW(e.read(input), tawara::ValueOutOfRange);

    input.str(std::string());
    tawara::FloatElement bad_f(tawara::ids::TrackTimecodeScale, 0.0);
    tawara::vint::write(bad_f.size(), input);
    bad_f.write(input);
    EXPECT_THROW(e.read(input), tawara::ValueOutOfRange);
    bad_f = -1.0;
    tawara::vint::write(bad_f.size(), input);
    bad_f.write(input);
    EXPECT_THROW(e.read(input), tawara::ValueOutOfRange);

    input.str(std::string());
    tawara::StringElement bad_s(tawara::ids::CodecID, "");
    tawara::vint::write(bad_s.size(), input);
    bad_s.write(input);
    EXPECT_THROW(e.read(input), tawara::ValueOutOfRange);

    input.str(std::string());
    bad.id(tawara::ids::AttachmentLink);
    bad = 0;
    tawara::vint::write(bad.size(), input);
    bad.write(input);
    EXPECT_THROW(e.read(input), tawara::ValueOutOfRange);

    input.str(std::string());
    bad.id(tawara::ids::CodecDecodeAll);
    bad = 2;
    tawara::vint::write(bad.size(), input);
    bad.write(input);
    EXPECT_THROW(e.read(input), tawara::ValueOutOfRange);
}

