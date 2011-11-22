/* Tide
 *
 * Source for the TrackEntry element.
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

#include <tide/track_entry.h>

#include <boost/foreach.hpp>
#include <tide/el_ids.h>
#include <tide/element.h>
#include <tide/vint.h>

using namespace tide;

///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

TrackEntry::TrackEntry(uint64_t number, uint64_t uid, std::string const& codec)
    : MasterElement(ids::TrackEntry),
    number_(ids::TrackNumber, number), uid_(ids::TrackUID, uid),
    type_(ids::TrackType, 0x70), enabled_(ids::FlagEnabled, 1, 1),
    forced_(ids::FlagForced, 0, 0), lacing_(ids::FlagLacing, 1, 1),
    min_cache_(ids::MinCache, 0, 0), max_cache_(ids::MaxCache, 0, 0),
    default_dur_(ids::DefaultDuration, 0),
    timecode_scale_(ids::TrackTimecodeScale, 1.0, 1.0),
    max_block_add_id_(ids::MaxBlockAdditionID, 0, 0),
    name_(ids::Name, ""), codec_id_(ids::CodecID, codec),
    codec_private_(ids::CodecPrivate, std::vector<char>()),
    codec_name_(ids::CodecName, ""), attachment_link_(ids::AttachmentLink, 0),
    decode_all_(ids::CodecDecodeAll, 0, 0)
{
    if (number == 0)
    {
        throw ValueOutOfRange() << err_id(number_.id()) << err_par_id(id_);
    }
    if (uid == 0)
    {
        throw ValueOutOfRange() << err_id(uid_.id()) << err_par_id(id_);
    }
    if (codec.empty())
    {
        throw ValueOutOfRange() << err_id(codec_id_.id()) <<
            err_par_id(id_);
    }
}


///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

void TrackEntry::number(uint64_t number)
{
    if (number == 0)
    {
        throw ValueOutOfRange() << err_id(number_.id()) << err_par_id(id_);
    }
    number_ = number;
}


void TrackEntry::uid(uint64_t uid)
{
    if (uid == 0)
    {
        throw ValueOutOfRange() << err_id(uid_.id()) << err_par_id(id_);
    }
    uid_ = uid;
}


void TrackEntry::type(uint8_t type)
{
    // Valid range: 0 - 254
    if (type == 255)
    {
        throw ValueOutOfRange() << err_id(type_.id()) << err_par_id(id_);
    }
    type_ = type;
}


void TrackEntry::timecode_scale(double timecode_scale)
{
    if (timecode_scale <= 0.0)
    {
        throw ValueOutOfRange() << err_id(timecode_scale_.id()) <<
            err_par_id(id_);
    }
    timecode_scale_ = timecode_scale;
}


void TrackEntry::codec_id(std::string id)
{
    if (id.empty())
    {
        throw ValueOutOfRange() << err_id(codec_id_.id()) <<
            err_par_id(id_);
    }
    codec_id_ = id;
}


std::vector<uint64_t> TrackEntry::overlays() const
{
    std::vector<uint64_t> result;
    BOOST_FOREACH(UIntElement overlay, overlays_)
    {
        result.push_back(overlay);
    }
    return result;
}


void TrackEntry::overlays(std::vector<uint64_t> const& uids)
{
    overlays_.clear();
    BOOST_FOREACH(uint64_t uid, uids)
    {
        overlays_.push_back(UIntElement(ids::TrackOverlay, uid));
    }
}


std::streamsize TrackEntry::size() const
{
    std::streamsize size(0);
    size += number_.total_size();
    size += uid_.total_size();
    size += type_.total_size();
    if (!enabled_.is_default())
    {
        size += enabled_.total_size();
    }
    if (!forced_.is_default())
    {
        size += forced_.total_size();
    }
    if (!lacing_.is_default())
    {
        size += lacing_.total_size();
    }
    if (!min_cache_.is_default())
    {
        size += min_cache_.total_size();
    }
    if (!max_cache_.is_default())
    {
        size += max_cache_.total_size();
    }
    if (default_dur_ != 0)
    {
        size += default_dur_.total_size();
    }
    if (!timecode_scale_.is_default())
    {
        size += timecode_scale_.total_size();
    }
    if (!max_block_add_id_.is_default())
    {
        size += max_block_add_id_.total_size();
    }
    if (!name_.value().empty())
    {
        size += name_.total_size();
    }
    if (!codec_private_.value().empty())
    {
        size += codec_private_.total_size();
    }
    if (!codec_name_.value().empty())
    {
        size += codec_name_.total_size();
    }
    if (attachment_link_ != 0)
    {
        size += attachment_link_.total_size();
    }
    if (!decode_all_.is_default())
    {
        size += decode_all_.total_size();
    }
    BOOST_FOREACH(UIntElement overlay, overlays_)
    {
        size += overlay.total_size();
    }
    if (operation_)
    {
        size += tide::ids::coded_size(ids::TrackOperation) +
            tide::vint::coded_size(operation_->total_size()) +
            operation_->total_size();
    }
    return size;
}


///////////////////////////////////////////////////////////////////////////////
// I/O
///////////////////////////////////////////////////////////////////////////////

std::streamsize TrackEntry::write_body(std::ostream& output)
{
    assert(number_ != 0);
    assert(uid_ != 0);
    assert(type_ >= 0 && type_ < 255);
    assert(enabled_ == 0 || enabled_ == 1);
    assert(forced_ == 0 || forced_ == 1);
    assert(lacing_ == 0 || lacing_ == 1);
    assert(timecode_scale_ > 0.0);
    assert(decode_all_ == 0 || decode_all_ == 1);

    std::streamsize written(0);
    written += number_.write(output);
    written += uid_.write(output);
    written += type_.write(output);
    if (!enabled_.is_default())
    {
        written += enabled_.write(output);
    }
    if (!forced_.is_default())
    {
        written += forced_.write(output);
    }
    if (!lacing_.is_default())
    {
        written += lacing_.write(output);
    }
    if (!min_cache_.is_default())
    {
        written += min_cache_.write(output);
    }
    if (!max_cache_.is_default())
    {
        written += max_cache_.write(output);
    }
    if (default_dur_ != 0)
    {
        written += default_dur_.write(output);
    }
    if (!timecode_scale_.is_default())
    {
        written += timecode_scale_.write(output);
    }
    if (!max_block_add_id_.is_default())
    {
        written += max_block_add_id_.write(output);
    }
    if (!name_.value().empty())
    {
        written += name_.write(output);
    }
    if (!codec_private_.value().empty())
    {
        written += codec_private_.write(output);
    }
    if (!codec_name_.value().empty())
    {
        written += codec_name_.write(output);
    }
    if (attachment_link_ != 0)
    {
        written += attachment_link_.write(output);
    }
    if (!decode_all_.is_default())
    {
        written += decode_all_.write(output);
    }
    BOOST_FOREACH(UIntElement overlay, overlays_)
    {
        written += overlay.write(output);
    }
    if (operation_)
    {
        written += tide::ids::write(ids::TrackOperation, output);
        written += tide::vint::write(operation_->total_size(), output);
        written += operation_->write(output);
    }
    return written;
}


std::streamsize TrackEntry::read_body(std::istream& input)
{
    std::streampos el_start(input.tellg());

    // Reset to defaults
    reset();

    // Get the element's body size
    vint::ReadResult result = tide::vint::read(input);
    std::streamsize body_size(result.first);
    std::streamsize size_size(result.second);
    std::streamsize read_bytes(result.second);
    // Read elements until the body is exhausted
    bool have_type(false);
    while (read_bytes < size_size + body_size)
    {
        UIntElement uid(ids::TrackOverlay, 0);
        // Read the ID
        ids::ReadResult id_res = ids::read(input);
        ids::ID id(id_res.first);
        read_bytes += id_res.second;
        switch(id)
        {
            case ids::TrackNumber:
                read_bytes += number_.read_body(input);
                if (number_ == 0)
                {
                    throw ValueOutOfRange() << err_id(number_.id()) <<
                        err_par_id(id_) << err_pos(input.tellg());
                }
                break;
            case ids::TrackUID:
                read_bytes += uid_.read_body(input);
                if (uid_ == 0)
                {
                    throw ValueOutOfRange() << err_id(uid_.id()) <<
                        err_par_id(id_) << err_pos(input.tellg());
                }
                break;
            case ids::TrackType:
                read_bytes += type_.read_body(input);
                if (type_ > 254)
                {
                    throw ValueOutOfRange() << err_id(type_.id()) <<
                        err_par_id(id_) << err_pos(input.tellg());
                }
                have_type = true;
                break;
            case ids::FlagEnabled:
                read_bytes += enabled_.read_body(input);
                if (enabled_ != 0 && enabled_ != 1)
                {
                    throw ValueOutOfRange() << err_id(enabled_.id()) <<
                        err_par_id(id_) << err_pos(input.tellg());
                }
                break;
            case ids::FlagDefault:
                // Skip this element
                skip_read(input, false);
                break;
            case ids::FlagForced:
                read_bytes += forced_.read_body(input);
                if (forced_ != 0 && forced_ != 1)
                {
                    throw ValueOutOfRange() << err_id(forced_.id()) <<
                        err_par_id(id_) << err_pos(input.tellg());
                }
                break;
            case ids::FlagLacing:
                read_bytes += lacing_.read_body(input);
                if (lacing_ != 0 && lacing_ != 1)
                {
                    throw ValueOutOfRange() << err_id(lacing_.id()) <<
                        err_par_id(id_) << err_pos(input.tellg());
                }
                break;
            case ids::MinCache:
                read_bytes += min_cache_.read_body(input);
                break;
            case ids::MaxCache:
                read_bytes += max_cache_.read_body(input);
                break;
            case ids::DefaultDuration:
                read_bytes += default_dur_.read_body(input);
                if (default_dur_ == 0)
                {
                    throw ValueOutOfRange() << err_id(default_dur_.id()) <<
                        err_par_id(id_) << err_pos(input.tellg());
                }
                break;
            case ids::TrackTimecodeScale:
                read_bytes += timecode_scale_.read_body(input);
                if (timecode_scale_ <= 0.0)
                {
                    throw ValueOutOfRange() << err_id(timecode_scale_.id()) <<
                        err_par_id(id_) << err_pos(input.tellg());
                }
                break;
            case ids::MaxBlockAdditionID:
                read_bytes += max_block_add_id_.read_body(input);
                break;
            case ids::Name:
                read_bytes += name_.read_body(input);
                break;
            case ids::CodecID:
                read_bytes += codec_id_.read_body(input);
                if (codec_id_.value().empty())
                {
                    throw ValueOutOfRange() << err_id(codec_id_.id()) <<
                        err_par_id(id_) << err_pos(input.tellg());
                }
                break;
            case ids::CodecPrivate:
                read_bytes += codec_private_.read_body(input);
                break;
            case ids::CodecName:
                read_bytes += codec_name_.read_body(input);
                break;
            case ids::AttachmentLink:
                read_bytes += attachment_link_.read_body(input);
                if (attachment_link_ == 0)
                {
                    throw ValueOutOfRange() << err_id(attachment_link_.id()) <<
                        err_par_id(id_) << err_pos(input.tellg());
                }
                break;
            case ids::CodecDecodeAll:
                read_bytes += decode_all_.read_body(input);
                if (decode_all_ != 0 && decode_all_ != 1)
                {
                    throw ValueOutOfRange() << err_id(decode_all_.id()) <<
                        err_par_id(id_) << err_pos(input.tellg());
                }
                break;
            case ids::TrackOverlay:
                read_bytes += uid.read_body(input);
                overlays_.push_back(uid);
                break;
            case ids::TrackOperation:
                read_bytes += read_operation(input);
                break;
            default:
                throw InvalidChildID() << err_id(id) << err_par_id(id_) <<
                    err_pos(input.tellg());
        };
    }
    if (read_bytes != size_size + body_size)
    {
        // Read more than was specified by the body size value
        throw BadBodySize() << err_id(id_) << err_el_size(body_size) <<
            err_pos(el_start);
    }
    if (number_ == 0)
    {
        throw MissingChild() << err_id(ids::TrackNumber) << err_par_id(id_) <<
            err_pos(el_start);
    }
    if (uid_ == 0)
    {
        throw MissingChild() << err_id(ids::TrackUID) << err_par_id(id_) <<
            err_pos(el_start);
    }
    if (!have_type)
    {
        throw MissingChild() << err_id(ids::TrackType) << err_par_id(id_) <<
            err_pos(el_start);
    }
    if (codec_id_.value().empty())
    {
        throw MissingChild() << err_id(ids::CodecID) << err_par_id(id_) <<
            err_pos(el_start);
    }

    return read_bytes;
}


///////////////////////////////////////////////////////////////////////////////
// Private functions
///////////////////////////////////////////////////////////////////////////////

void TrackEntry::reset()
{
    number_ = 0;
    uid_ = 0;
    type_ = 0x70;
    enabled_ = enabled_.get_default();
    forced_ = forced_.get_default();
    lacing_ = lacing_.get_default();
    min_cache_ = min_cache_.get_default();
    max_cache_ = max_cache_.get_default();
    default_dur_ = 0;
    timecode_scale_ = timecode_scale_.get_default();
    max_block_add_id_ = max_block_add_id_.get_default();
    name_ = name_.get_default();
    codec_id_ = "";
    codec_private_ = std::vector<char>();
    codec_name_ = "";
    attachment_link_ = 0;
    decode_all_ = decode_all_.get_default();
    overlays_.clear();
    operation_.reset();
}


std::streamsize TrackEntry::read_operation(std::istream& input)
{
    std::streamsize read_bytes(0);
    vint::ReadResult op_size_res = vint::read(input);
    read_bytes += op_size_res.second;
    if (read_bytes == 0)
    {
        return 0;
        // An empty operation is legal.
    }
    ids::ReadResult op_id_res = ids::read(input);
    read_bytes += op_id_res.second;
    // Currently, only the TrackJoinBlocks operation is supported
    if (op_id_res.first != ids::TrackJoinBlocks)
    {
        throw InvalidChildID() << err_id(op_id_res.first) <<
            err_par_id(id_) << err_pos(input.tellg());
    }
    OpPtr op(new TrackJoinBlocks());
    read_bytes += op->read_body(input);
    operation_ = op;
    return read_bytes;
}

