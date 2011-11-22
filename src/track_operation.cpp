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


std::streamsize TrackJoinBlocks::size() const
{
    std::streamsize size(0);
    BOOST_FOREACH(UIntElement el, uids_)
    {
        size += el.total_size();
    }
    return size;
}


///////////////////////////////////////////////////////////////////////////////
// I/O
///////////////////////////////////////////////////////////////////////////////

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


std::streamsize TrackJoinBlocks::read_body(std::istream& input)
{
    std::streampos el_start(input.tellg());

    // Clear the UIDs list
    uids_.clear();
    // Get the element's body size
    vint::ReadResult result = tide::vint::read(input);
    std::streamsize body_size(result.first);
    std::streamsize size_size(result.second);
    std::streamsize read_bytes(result.second);
    // Read elements until the body is exhausted
    while (read_bytes < size_size + body_size)
    {
        // Read the ID
        ids::ReadResult id_res = tide::ids::read(input);
        ids::ID id(id_res.first);
        read_bytes += id_res.second;
        if (id != ids::TrackJoinUID)
        {
            // Only TrackJoinUID elements may be in the TrackJoinBlocks element
            throw InvalidChildID() << err_id(id) << err_par_id(id_) <<
                err_pos(input.tellg());
        }
        // Read the body
        UIntElement uid(ids::Null, 0);
        read_bytes += uid.read_body(input);
        if (uid.value() == 0)
        {
            // Zero-value UIDs are illegal
            throw ValueOutOfRange() << err_id(id) << err_par_id(id_) <<
                err_pos(input.tellg());
        }
        uids_.push_back(uid);
    }
    if (read_bytes != size_size + body_size)
    {
        // Read more than was specified by the body size value
        throw BadBodySize() << err_id(id_) << err_el_size(body_size) <<
            err_pos(el_start);
    }
    if (uids_.empty())
    {
        // Must have read at least one TrackJoinUID
        throw MissingChild() << err_id(ids::TrackJoinUID) << err_par_id(id_) <<
            err_pos(el_start);
    }

    return read_bytes;
}

