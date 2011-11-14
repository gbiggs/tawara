/* Tide
 *
 * Source for the SegmentInfo element.
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

#include <tide/segment_info.h>

#include <algorithm>
#include <functional>
#include <tide/vint.h>

using namespace tide;


///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

SegmentInfo::SegmentInfo()
    : MasterElement(ids::Info),
    uid_(ids::SegmentUID, std::vector<char>()), have_uid_(false),
    seg_fn_(ids::SegmentFileName, ""), have_seg_fn_(false),
    prev_uid_(ids::PrevUID, std::vector<char>()), have_prev_uid_(false),
    prev_fn_(ids::PrevFileName, ""), have_prev_fn_(false),
    next_uid_(ids::NextUID, std::vector<char>()), have_next_uid_(false),
    next_fn_(ids::NextFileName, ""), have_next_fn_(false),
    seg_fam_(ids::SegmentFamily, std::vector<char>()), have_seg_fam_(false),
    tc_scale_(ids::TimecodeScale, 1000000, 1000000),
    duration_(ids::Duration, 1), have_duration_(false),
    date_(ids::DateUTC, 0), have_date_(false),
    title_(ids::Title, ""), have_title_(false),
    muxer_(ids::MuxingApp, ""), have_muxer_(false),
    writer_(ids::WritingApp, ""), have_writer_(false)
{
}


///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

void SegmentInfo::uid(std::vector<char> const& uid)
{
    if (uid.empty())
    {
        uid_ = uid;
        have_uid_ = false;
    }
    else
    {
        std::vector<char>::const_iterator non_zero(std::find_if(uid.begin(),
                    uid.end(), std::bind2nd(std::not_equal_to<int>(), 0)));
        if (non_zero == uid.end())
        {
            throw ValueOutOfRange() << err_id(ids::SegmentUID) <<
                err_id(ids::Info);
        }
        if (uid.size() != 8)
        {
            throw ValueSizeOutOfRange() << err_id(ids::SegmentUID) <<
                err_id(ids::Info);
        }

        uid_ = uid;
        have_uid_ = true;
    }
}


void SegmentInfo::filename(std::string const& filename)
{
    seg_fn_ = filename;
    if (filename.empty())
    {
        have_seg_fn_ = false;
    }
    else
    {
        have_seg_fn_ = true;
    }
}


void SegmentInfo::prev_uid(std::vector<char> const& uid)
{
    if (uid.empty())
    {
        prev_uid_ = uid;
        have_prev_uid_ = false;
    }
    else
    {
        std::vector<char>::const_iterator non_zero(std::find_if(uid.begin(),
                    uid.end(), std::bind2nd(std::not_equal_to<int>(), 0)));
        if (non_zero == uid.end())
        {
            throw ValueOutOfRange() << err_id(ids::PrevUID) <<
                err_id(ids::Info);
        }
        if (uid.size() != 8)
        {
            throw ValueSizeOutOfRange() << err_id(ids::PrevUID) <<
                err_id(ids::Info);
        }

        prev_uid_ = uid;
        have_prev_uid_ = true;
    }
}


void SegmentInfo::prev_filename(std::string const& filename)
{
    prev_fn_ = filename;
    if (filename.empty())
    {
        have_prev_fn_ = false;
    }
    else
    {
        have_prev_fn_ = true;
    }
}


void SegmentInfo::next_uid(std::vector<char> const& uid)
{
    if (uid.empty())
    {
        next_uid_ = uid;
        have_next_uid_ = false;
    }
    else
    {
        std::vector<char>::const_iterator non_zero(std::find_if(uid.begin(),
                    uid.end(), std::bind2nd(std::not_equal_to<int>(), 0)));
        if (non_zero == uid.end())
        {
            throw ValueOutOfRange() << err_id(ids::NextUID) <<
                err_id(ids::Info);
        }
        if (uid.size() != 8)
        {
            throw ValueSizeOutOfRange() << err_id(ids::NextUID) <<
                err_id(ids::Info);
        }

        next_uid_ = uid;
        have_next_uid_ = true;
    }
}


void SegmentInfo::next_filename(std::string const& filename)
{
    next_fn_ = filename;
    if (filename.empty())
    {
        have_next_fn_ = false;
    }
    else
    {
        have_next_fn_ = true;
    }
}


void SegmentInfo::segment_family(std::vector<char> const& segment_family)
{
    if (segment_family.empty())
    {
        seg_fam_ = segment_family;
        have_seg_fam_ = false;
    }
    else
    {
        std::vector<char>::const_iterator
            non_zero(std::find_if(segment_family.begin(), segment_family.end(),
                        std::bind2nd(std::not_equal_to<int>(), 0)));
        if (non_zero == segment_family.end())
        {
            throw ValueOutOfRange() << err_id(ids::SegmentFamily) <<
                err_id(ids::Info);
        }
        if (segment_family.size() != 8)
        {
            throw ValueSizeOutOfRange() << err_id(ids::SegmentFamily) <<
                err_id(ids::Info);
        }

        seg_fam_ = segment_family;
        have_seg_fam_ = true;
    }
}


void SegmentInfo::timecode_scale(uint64_t scale)
{
    if (scale == 0)
    {
        tc_scale_ = tc_scale_.get_default();
    }
    else
    {
        tc_scale_ = scale;
    }
}


void SegmentInfo::duration(double duration)
{
    if (duration <= 0)
    {
        throw ValueOutOfRange() << err_id(ids::Duration) <<
            err_id(ids::Info);
    }
    duration_ = duration;
    have_duration_ = true;
}


void SegmentInfo::date(int64_t date)
{
    date_ = date;
    have_date_ = true;
}


void SegmentInfo::title(std::string const& title)
{
    title_ = title;
    if (title.empty())
    {
        have_title_ = false;
    }
    else
    {
        have_title_ = true;
    }
}


void SegmentInfo::muxing_app(std::string const& muxing_app)
{
    muxer_ = muxing_app;
    if (muxing_app.empty())
    {
        have_muxer_ = false;
    }
    else
    {
        have_muxer_ = true;
    }
}


void SegmentInfo::writing_app(std::string const& writing_app)
{
    writer_ = writing_app;
    if (writing_app.empty())
    {
        have_writer_ = false;
    }
    else
    {
        have_writer_ = true;
    }
}


std::streamsize SegmentInfo::size() const
{
    std::streamsize result(tc_scale_.total_size());

    if (have_uid_)
    {
        result += uid_.total_size();
    }
    if (have_seg_fn_)
    {
        result += seg_fn_.total_size();
    }
    if (have_prev_uid_)
    {
        result += prev_uid_.total_size();
    }
    if (have_prev_fn_)
    {
        result += prev_fn_.total_size();
    }
    if (have_next_uid_)
    {
        result += next_uid_.total_size();
    }
    if (have_next_fn_)
    {
        result += next_fn_.total_size();
    }
    if (have_seg_fam_)
    {
        result += seg_fam_.total_size();
    }
    if (have_duration_)
    {
        result += duration_.total_size();
    }
    if (have_date_)
    {
        result += date_.total_size();
    }
    if (have_title_)
    {
        result += title_.total_size();
    }
    if (have_muxer_)
    {
        result += muxer_.total_size();
    }
    if (have_writer_)
    {
        result += writer_.total_size();
    }

    return result;
}


///////////////////////////////////////////////////////////////////////////////
// I/O
///////////////////////////////////////////////////////////////////////////////

std::streamsize SegmentInfo::write_body(std::ostream& output)
{
    std::streamsize written(0);

    // The spec may say that the TimecodeScale comes later, but in EBML it
    // doesn't actually matter.
    written += tc_scale_.write(output);
    if (have_uid_)
    {
        written += uid_.write(output);
    }
    if (have_seg_fn_)
    {
        written += seg_fn_.write(output);
    }
    if (have_prev_uid_)
    {
        written += prev_uid_.write(output);
    }
    if (have_prev_fn_)
    {
        written += prev_fn_.write(output);
    }
    if (have_next_uid_)
    {
        written += next_uid_.write(output);
    }
    if (have_next_fn_)
    {
        written += next_fn_.write(output);
    }
    if (have_seg_fam_)
    {
        written += seg_fam_.write(output);
    }
    if (have_duration_)
    {
        written += duration_.write(output);
    }
    if (have_date_)
    {
        written += date_.write(output);
    }
    if (have_title_)
    {
        written += title_.write(output);
    }
    if (have_muxer_)
    {
        written += muxer_.write(output);
    }
    if (have_writer_)
    {
        written += writer_.write(output);
    }
    return written;
}


std::streamsize SegmentInfo::read_body(std::istream& input)
{
    // Reset to defaults
    reset();

    std::streampos el_start(input.tellg());

    // Get the element's body size
    vint::ReadResult result = tide::vint::read(input);
    std::streamsize body_size(result.first);
    std::streamsize size_size(result.second);
    std::streamsize read_bytes(result.second);
    // Read elements until the body is exhausted
    while (read_bytes < size_size + body_size)
    {
        // Read the ID
        ids::ReadResult id_res = ids::read(input);
        ids::ID id(id_res.first);
        read_bytes += id_res.second;
        switch(id)
        {
            case ids::SegmentUID:
                read_bytes += uid_.read_body(input);
                have_uid_ = true;
                break;
            case ids::SegmentFileName:
                read_bytes += seg_fn_.read_body(input);
                have_seg_fn_ = true;
                break;
            case ids::PrevUID:
                read_bytes += prev_uid_.read_body(input);
                have_prev_uid_ = true;
                break;
            case ids::PrevFileName:
                read_bytes += prev_fn_.read_body(input);
                have_prev_fn_ = true;
                break;
            case ids::NextUID:
                read_bytes += next_uid_.read_body(input);
                have_next_uid_ = true;
                break;
            case ids::NextFileName:
                read_bytes += next_fn_.read_body(input);
                have_next_fn_ = true;
                break;
            case ids::SegmentFamily:
                read_bytes += seg_fam_.read_body(input);
                have_seg_fam_ = true;
                break;
            case ids::TimecodeScale:
                read_bytes += tc_scale_.read_body(input);
                break;
            case ids::Duration:
                read_bytes += duration_.read_body(input);
                have_duration_ = true;
                break;
            case ids::DateUTC:
                read_bytes += date_.read_body(input);
                have_date_ = true;
                break;
            case ids::Title:
                read_bytes += title_.read_body(input);
                have_title_ = true;
                break;
            case ids::MuxingApp:
                read_bytes += muxer_.read_body(input);
                have_muxer_ = true;
                break;
            case ids::WritingApp:
                read_bytes += writer_.read_body(input);
                have_writer_ = true;
                break;
            default:
                throw InvalidChildID() << err_id(id) << err_par_id(id_) <<
                    err_pos(input.tellg());
        }
    }
    if (read_bytes != size_size + body_size)
    {
        // Read more than was specified by the body size value
        throw BadBodySize() << err_id(id_) << err_el_size(body_size) <<
            err_pos(el_start);
    }

    return read_bytes;
}


///////////////////////////////////////////////////////////////////////////////
// Private functions
///////////////////////////////////////////////////////////////////////////////

void SegmentInfo::reset()
{
    uid_ = std::vector<char>();
    have_uid_ = false;
    seg_fn_ = "";
    have_seg_fn_ = false;
    prev_uid_ = std::vector<char>();
    have_prev_uid_ = false;
    prev_fn_ = "";
    have_prev_fn_ = false;
    next_uid_ = std::vector<char>();
    have_next_uid_ = false;
    next_fn_ = "";
    have_next_fn_ = false;
    seg_fam_ = std::vector<char>();
    have_seg_fam_ = false;
    tc_scale_ = 1000000;
    duration_ = 1;
    have_duration_ = false;
    date_ = 0;
    have_date_ = false;
    title_ = "";
    have_title_ = false;
    muxer_ = "";
    have_muxer_ = false;
    writer_ = "";
    have_writer_ = false;
}

