/* Tide
 *
 * Source file for the common Block functionality object.
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

#include <tide/block_impl.h>

#include <algorithm>
#include <boost/foreach.hpp>
#include <numeric>
#include <tide/el_ids.h>
#include <tide/exceptions.h>
#include <tide/vint.h>

using namespace tide;


///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

BlockImpl::BlockImpl(uint64_t track_number, int16_t timecode,
        LacingType lacing)
    : Block(track_number, timecode, lacing),
    track_num_(track_number), timecode_(timecode), invisible_(false),
    lacing_(lacing)
{
}


///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

BlockImpl& BlockImpl::operator=(BlockImpl const& other)
{
    track_num_ = other.track_num_;
    timecode_ = other.timecode_;
    invisible_ = other.invisible_;
    lacing_ = other.lacing_;
    frames_ = other.frames_;
    return *this;
}


BlockImpl::size_type BlockImpl::max_count() const
{
    if (lacing_ == LACING_NONE)
    {
        return 1;
    }
    return frames_.max_size();
}


void BlockImpl::push_back(BlockImpl::value_type const& value)
{
    if (!value)
    {
        // Empty pointer
        throw EmptyFrame();
    }
    if (value->empty())
    {
        // Pointer has a vector, but it is empty
        throw EmptyFrame();
    }
    if (frames_.size() >= 1 && lacing_ == LACING_NONE)
    {
        throw MaxLaceSizeExceeded() << err_max_lace(1) <<
            err_req_lace(frames_.size() + 1);
    }
    if (frames_.size() > 0 && lacing_ == LACING_FIXED &&
            value->size() != frames_[0]->size())
    {
        throw BadLacedFrameSize() << err_frame_size(value->size());
    }
    frames_.push_back(value);
}


void BlockImpl::resize(BlockImpl::size_type count)
{
    if (count > 1 && lacing_ == LACING_NONE)
    {
        throw MaxLaceSizeExceeded() << err_max_lace(1) << err_req_lace(count);
    }
    frames_.resize(count);
}


void BlockImpl::swap(BlockImpl& other)
{
    std::swap(track_num_, other.track_num_);
    std::swap(timecode_, other.timecode_);
    std::swap(invisible_, other.invisible_);
    std::swap(lacing_, other.lacing_);
    frames_.swap(other.frames_);
}


std::streamsize add_size(std::streamsize x, Block::value_type frame)
{
    return x + frame->size();
}

std::streamsize BlockImpl::size() const
{
    // Timecode (2) + flags (1)
    std::streamsize hdr_size(3);

    hdr_size += tide::vint::size(track_num_);

    switch(lacing_)
    {
        case LACING_EBML:
            hdr_size += 1; // Number of frames
            if (!frames_.empty())
            {
                std::streamsize prev_size(frames_[0]->size());
                hdr_size += vint::size(prev_size);
                // Add the size of each of the remaining frames except the last
                BOOST_FOREACH(value_type frame,
                        std::make_pair(frames_.begin() + 1, frames_.end() - 1))
                {
                    std::streamsize size_diff(frame->size() - prev_size);
                    prev_size = frame->size();
                    hdr_size += vint::size(vint::s_to_u(size_diff));
                }
            }
            break;
        case LACING_FIXED:
            // Only the number of frames is stored
            hdr_size += 1;
            break;
        case LACING_NONE:
            // No lacing header
            break;
    }

    return hdr_size + std::accumulate(frames_.begin(), frames_.end(), 0,
            std::ptr_fun(add_size));
}


///////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////

bool tide::operator==(BlockImpl const& lhs, BlockImpl const& rhs)
{
    return lhs.track_num_ == rhs.track_num_ &&
        lhs.timecode_ == rhs.timecode_ &&
        lhs.invisible_ == rhs.invisible_ &&
        lhs.lacing_ == rhs.lacing_ &&
        lhs.frames_ == rhs.frames_;
}


///////////////////////////////////////////////////////////////////////////////
// I/O
///////////////////////////////////////////////////////////////////////////////

std::streamsize BlockImpl::write(std::ostream& output, uint8_t extra_flags)
{
    validate();

    std::streamsize written(0);

    // Write the track number
    written += vint::write(track_num_, output);
    // Write the time code (2 bytes)
    output.put(timecode_ >> 8);
    output.put(timecode_ & 0x00FF);
    if (!output)
    {
        throw tide::WriteError() << tide::err_pos(output.tellp());
    }
    written += 2;
    // Prepare and write the flags
    uint8_t flags(extra_flags);
    if (invisible_)
    {
        flags |= 0x10;
    }
    switch (lacing_)
    {
        case Block::LACING_EBML:
            flags |= 0x60;
            break;
        case Block::LACING_FIXED:
            flags |= 0x40;
            break;
        case LACING_NONE:
            // Nothing to do for no lacing
            break;
    }
    output.put(flags);
    if (!output)
    {
        throw tide::WriteError() << tide::err_pos(output.tellp());
    }
    written += 1;
    // Write the lacing header
    uint8_t num_frames(frames_.size());
    std::streamsize prev_size(0);
    switch (lacing_)
    {
        case Block::LACING_EBML:
            output.put(num_frames);
            if (!output)
            {
                throw tide::WriteError() << tide::err_pos(output.tellp());
            }
            written += 1;
            // Write the first frame size as an unsigned integer
            prev_size = frames_[0]->size();
            written += vint::write(prev_size, output);
            // Loop over the remaining frames
            BOOST_FOREACH(value_type frame,
                    std::make_pair(frames_.begin() + 1, frames_.end()))
            {
                std::streamsize size_diff(frame->size() - prev_size);
                prev_size = frame->size();
                // Write the frame size as an offset signed integer
                written += vint::write(vint::s_to_u(size_diff));
            }
            break;
        case Block::LACING_FIXED:
            output.put(num_frames);
            if (!output)
            {
                throw tide::WriteError() << tide::err_pos(output.tellp());
            }
            written += 1;
            break;
        case LACING_NONE:
            // Nothing to do for no lacing
            break;
    }
    // Write the frames
    BOOST_FOREACH(value_type frame, frames_)
    {
        output.write(&(*frame)[0], frame->size());
        if (!output)
        {
            throw tide::WriteError() << tide::err_pos(output.tellp());
        }
        written += frame->size();
    }
}


BlockImpl::ReadResult BlockImpl::read(std::istream& input)
{
}


///////////////////////////////////////////////////////////////////////////////
// Private functions
///////////////////////////////////////////////////////////////////////////////

void BlockImpl::validate() const
{
    assert((lacing_ == LACING_NONE && frames_.size() == 1) ||
            (lacing_ != LACING_NONE && frames_.size() > 0));

    std::streamsize first_size(frames_[0]->size());

    BOOST_FOREACH(value_type f, frames_)
    {
        if (!f)
        {
            // Empty pointer
            throw EmptyFrame();
        }
        if (f->empty())
        {
            // Pointer has a vector, but it is empty
            throw EmptyFrame();
        }
        if (f->size() != first_size && lacing_ == Block::LACING_FIXED)
        {
            // Fixed lacing requires that all frames are the same size
            throw BadLacedFrameSize() << err_frame_size(f->size());
        }
    }
}

