/* Tide
 *
 * Source file for the BlockGroup element.
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

#include <tide/block_group.h>

#include <boost/foreach.hpp>
#include <tide/el_ids.h>
#include <tide/ebml_int.h>
#include <tide/int_element.h>
#include <tide/vint.h>

using namespace tide;


///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

BlockGroup::BlockGroup(uint64_t track_number, int16_t timecode,
        LacingType lacing, uint64_t duration, uint64_t ref_priority)
    : BlockElement(tide::ids::BlockGroup, track_number, timecode, lacing),
    duration_(ids::BlockDuration, duration),
    ref_priority_(ids::ReferencePriority, ref_priority, 0),
    codec_state_(ids::CodecState, std::vector<char>()),
    block_(track_number, timecode, lacing)
{
}


///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

void BlockGroup::swap(BlockGroup& other)
{
    std::swap(additions_, other.additions_);
    std::swap(duration_, other.duration_);
    std::swap(ref_priority_, other.ref_priority_);
    ref_blocks_.swap(other.ref_blocks_);
    std::swap(codec_state_, other.codec_state_);
    block_.swap(other.block_);
}


///////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////

bool tide::operator==(BlockGroup const& lhs, BlockGroup const& rhs)
{
    return lhs.additions_ == rhs.additions_ &&
        lhs.duration_ == rhs.duration_ &&
        lhs.ref_priority_ == rhs.ref_priority_ &&
        lhs.ref_blocks_ == rhs.ref_blocks_ &&
        lhs.codec_state_ == rhs.codec_state_ &&
        lhs.block_ == rhs.block_;
}


///////////////////////////////////////////////////////////////////////////////
// Element interface
///////////////////////////////////////////////////////////////////////////////

std::streamsize BlockGroup::body_size() const
{
    std::streamsize result(0);

    if (!additions_.empty())
    {
        result += additions_.size();
    }
    if (duration_ != 0)
    {
        result += duration_.size();
    }
    if (!ref_priority_.is_default())
    {
        result += ref_priority_.size();
    }
    if (!ref_blocks_.empty())
    {
        BOOST_FOREACH(int16_t ref, ref_blocks_)
        {
            result += ids::size(ids::ReferenceBlock);
            result += vint::size(ebml_int::size_s(ref));
            result += ebml_int::size_s(ref);
        }
    }
    if (!codec_state_.value().empty())
    {
        result += codec_state_.size();
    }
    return result + ids::size(ids::Block) + vint::size(block_.size()) +
        block_.size();
}


std::streamsize BlockGroup::write_body(std::ostream& output)
{
    std::streamsize written(0);

    // Write the block element
    written += ids::write(ids::Block, output);
    written += vint::write(block_.size(), output);
    written += block_.write(output, 0);

    // Write the other elements
    if (!additions_.empty())
    {
        written += additions_.write(output);
    }
    if (duration_ != 0)
    {
        written += duration_.write(output);
    }
    if (!ref_priority_.is_default())
    {
        written += ref_priority_.write(output);
    }
    if (!ref_blocks_.empty())
    {
        BOOST_FOREACH(int16_t ref, ref_blocks_)
        {
            written += ids::write(ids::ReferenceBlock, output);
            written += vint::write(ebml_int::size_s(ref), output);
            written += ebml_int::write_s(ref, output);
        }
    }
    if (!codec_state_.value().empty())
    {
        written += codec_state_.write(output);
    }
    return written;
}


std::streamsize BlockGroup::read_body(std::istream& input,
        std::streamsize size)
{
    // Reset to defaults
    reset();

    std::streamsize read_bytes(0);
    // Read elements until the body is exhausted
    bool have_block(false);
    while (read_bytes < size)
    {
        // Read the ID
        ids::ReadResult id_res = ids::read(input);
        ids::ID id(id_res.first);
        read_bytes += id_res.second;
        vint::ReadResult size_res;
        BlockImpl::ReadResult block_res;
        IntElement tmp(ids::Null, 0);
        switch(id)
        {
            case ids::Block:
                size_res = vint::read(input);
                read_bytes += size_res.second;

                block_res = block_.read(input, size_res.first);
                read_bytes += block_res.first;
                have_block = true;
                break;
            case ids::BlockAdditions:
                read_bytes += additions_.read(input);
                break;
            case ids::BlockDuration:
                read_bytes += duration_.read(input);
                break;
            case ids::ReferencePriority:
                read_bytes += ref_priority_.read(input);
                break;
            case ids::ReferenceBlock:
                read_bytes += tmp.read(input);
                ref_blocks_.push_back(tmp);
                break;
            case ids::CodecState:
                read_bytes += codec_state_.read(input);
                break;
            default:
                throw InvalidChildID() << err_id(id) << err_par_id(id_) <<
                    // The cast here makes Apple's LLVM compiler happy
                    err_pos(static_cast<std::streamsize>(input.tellg()) -
                            id_res.second);
        };
    }
    if (read_bytes != size)
    {
        // Read more than was specified by the body size value
        throw BadBodySize() << err_id(id_) << err_el_size(size) <<
            err_pos(offset_);
    }
    if (!have_block)
    {
        throw MissingChild() << err_id(ids::Block) << err_par_id(id_) <<
            err_pos(offset_);
    }

    return read_bytes;
}


///////////////////////////////////////////////////////////////////////////////
// Private functions
///////////////////////////////////////////////////////////////////////////////

void BlockGroup::reset()
{
    additions_.clear();
    duration_.value(duration_.get_default());
    ref_priority_.value(ref_priority_.get_default());
    ref_blocks_.clear();
    codec_state_.value(std::vector<char>());
    block_.clear();
}

