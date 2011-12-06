/* Tide
 *
 * Source file for the SimpleBlock element.
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

#include <tide/simple_block.h>

#include <tide/el_ids.h>

using namespace tide;


///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

SimpleBlock::SimpleBlock(uint64_t track_number, int16_t timecode,
        LacingType lacing)
    : BlockElement(tide::ids::SimpleBlock, track_number, timecode, lacing),
    keyframe_(false), discardable_(false),
    block_(track_number, timecode, lacing)
{
}


///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

void SimpleBlock::swap(SimpleBlock& other)
{
    std::swap(keyframe_, other.keyframe_);
    std::swap(discardable_, other.discardable_);
    block_.swap(other.block_);
}


///////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////

bool tide::operator==(SimpleBlock const& lhs, SimpleBlock const& rhs)
{
    return lhs.keyframe_ == rhs.keyframe_ &&
        lhs.discardable_ == rhs.discardable_ &&
        lhs.block_ == rhs.block_;
}


///////////////////////////////////////////////////////////////////////////////
// Element interface
///////////////////////////////////////////////////////////////////////////////

std::streamsize SimpleBlock::body_size() const
{
    return block_.size();
}


std::streamsize SimpleBlock::write_body(std::ostream& output)
{
    uint8_t extra_flags(0);

    if (keyframe_)
    {
        extra_flags |= 0x01;
    }
    if (discardable_)
    {
        extra_flags |= 0x80;
    }
    return block_.write(output, extra_flags);
}


std::streamsize SimpleBlock::read_body(std::istream& input,
        std::streamsize size)
{
    BlockImpl::ReadResult res(block_.read(input, size));
    if (res.second & 0x01)
    {
        keyframe_ = true;
    }
    else
    {
        keyframe_ = false;
    }
    if (res.second & 0x80)
    {
        discardable_ = true;
    }
    else
    {
        discardable_ = false;
    }
    return res.first;
}

