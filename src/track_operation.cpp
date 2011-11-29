/* Tide
 *
 * Source for the track operation elements.
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

#include <tide/track_operation.h>

#include <boost/foreach.hpp>
#include <tide/vint.h>

using namespace tide;


///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

TrackJoinBlocks::TrackJoinBlocks()
    : TrackOperationBase(ids::TrackJoinBlocks)
{
}


///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

void TrackJoinBlocks::append(uint64_t uid)
{
    if (uid == 0)
    {
        // Zero-value UIDs are illegal
        throw ValueOutOfRange() << err_id(ids::TrackJoinUID) <<
            err_par_id(id_);
    }
    uids_.push_back(UIntElement(ids::TrackJoinUID, uid));
}


uint64_t TrackJoinBlocks::remove(unsigned int pos)
{
    UIntElement uid = uids_[pos];
    uids_.erase(uids_.begin() + pos);
    return uid.value();
}


uint64_t TrackJoinBlocks::operator[](unsigned int pos) const
{
    return uids_[pos].value();
}


///////////////////////////////////////////////////////////////////////////////
// Element interface
///////////////////////////////////////////////////////////////////////////////

std::streamsize TrackJoinBlocks::body_size() const
{
    std::streamsize size(0);
    BOOST_FOREACH(UIntElement el, uids_)
    {
        size += el.size();
    }
    return size;
}


std::streamsize TrackJoinBlocks::write_body(std::ostream& output)
{
    assert(!uids_.empty());

    std::streamsize written(0);
    BOOST_FOREACH(UIntElement uid, uids_)
    {
        if (uid.value() == 0)
        {
            // Zero-value UIDs are illegal
            throw ValueOutOfRange() << err_id(ids::TrackJoinUID) <<
                err_par_id(id_);
        }
        written += uid.write(output);
    }
    return written;
}


std::streamsize TrackJoinBlocks::read_body(std::istream& input,
        std::streamsize size)
{
    // Clear the UIDs list
    uids_.clear();
    std::streamsize read_bytes(0);
    // Read elements until the body is exhausted
    while (read_bytes < size)
    {
        // Read the ID
        ids::ReadResult id_res = tide::ids::read(input);
        ids::ID id(id_res.first);
        read_bytes += id_res.second;
        if (id != ids::TrackJoinUID)
        {
            // Only TrackJoinUID elements may be in the TrackJoinBlocks element
            throw InvalidChildID() << err_id(id) << err_par_id(id_) <<
                // The cast here makes Apple's LLVM compiler happy
                err_pos(static_cast<std::streamsize>(input.tellg()) -
                        id_res.second);
        }
        // Read the body
        UIntElement uid(ids::Null, 0);
        read_bytes += uid.read(input);
        if (uid.value() == 0)
        {
            // Zero-value UIDs are illegal
            throw ValueOutOfRange() << err_id(id) << err_par_id(id_) <<
                err_pos(input.tellg());
        }
        uids_.push_back(uid);
    }
    if (read_bytes != size)
    {
        // Read more than was specified by the body size value
        throw BadBodySize() << err_id(id_) << err_el_size(size) <<
            err_pos(offset_);
    }
    if (uids_.empty())
    {
        // Must have read at least one TrackJoinUID
        throw MissingChild() << err_id(ids::TrackJoinUID) << err_par_id(id_) <<
            err_pos(offset_);
    }

    return read_bytes;
}

