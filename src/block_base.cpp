/* Tide
 *
 * Source for the base block type.
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

#include <tide/block_base.h>

#include <algorithm>
#include <boost/foreach.hpp>
#include <tide/el_ids.h>
#include <tide/exceptions.h>

using namespace tide;


///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

BlockBase::BlockBase(uint64_t track_number, int16_t timecode,
        LacingType lacing)
    : track_num_(track_number), timecode_(timecode), invisible_(false),
    lacing_(lacing)
{
}


BlockBase::~BlockBase()
{
}


///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

BlockBase& BlockBase::operator=(BlockBase const& other)
{
    track_num_ = other.track_num_;
    timecode_ = other.timecode_;
    invisible_ = other.invisible_;
    lacing_ = other.lacing_;
    frames_ = other.frames_;
    return *this;
}


BlockBase::size_type BlockBase::max_count() const
{
    if (lacing_ == LACING_NONE)
    {
        return 1;
    }
    return frames_.max_size();
}


void BlockBase::push_back(BlockBase::value_type const& value)
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
    frames_.push_back(value);
}


void BlockBase::resize(BlockBase::size_type count)
{
    if (count > 1 && lacing_ == LACING_NONE)
    {
        throw MaxLaceSizeExceeded() << err_max_lace(1) << err_req_lace(count);
    }
    frames_.resize(count);
}


void BlockBase::swap(BlockBase& other)
{
    std::swap(track_num_, other.track_num_);
    std::swap(timecode_, other.timecode_);
    std::swap(invisible_, other.invisible_);
    std::swap(lacing_, other.lacing_);
    frames_.swap(other.frames_);
}


///////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////

bool tide::operator==(BlockBase const& lhs, BlockBase const& rhs)
{
    return lhs.track_num_ == rhs.track_num_ &&
        lhs.timecode_ == rhs.timecode_ &&
        lhs.invisible_ == rhs.invisible_ &&
        lhs.lacing_ == rhs.lacing_ &&
        lhs.frames_ == rhs.frames_;
}


///////////////////////////////////////////////////////////////////////////////
// Private functions
///////////////////////////////////////////////////////////////////////////////

void BlockBase::validate() const
{
    assert((lacing_ == LACING_NONE && frames_.size() == 1) ||
            lacing_ != LACING_NONE);

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
    }
}

