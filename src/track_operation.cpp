/* Tide
 *
 * Source for the TrackOperation element and related elements.
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


using namespace std;


///////////////////////////////////////////////////////////////////////////////
// TrackOperationBase Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

TrackOperationBase::TrackOperationBase(ids::ID id)
    : MasterElement(id)
{
}


///////////////////////////////////////////////////////////////////////////////
// TrackOperationBase Accessors
///////////////////////////////////////////////////////////////////////////////

void TrackOperationBase::append(uint64_t uid)
{
    if (uid == 0)
    {
        // Zero-value UIDs are illegal
        throw ValueOutOfRange() << err_id(id_);
    }
    uids_.push_back(UIntElement(ids::TrackJoinUID, uid));
}


uint64_t TrackOperationBase::remove(unsigned int pos)
{
    UIntElement uid = uids_[pos];
    uids_.erase(index.begin() + pos);
    return uids_.value();
}


uint64_t const& TrackOperatorBase::operator[](unsigned int pos) const
{
    return uids_[pos].value();
}


uint64_t& TrackOperatorBase::operator[](unsigned int pos)
{
    return uids_[pos].value();
}


std::streamsize TrackOperationBase::size() const
{
    std::streamsize size(0);
    BOOST_FOREACH(UIntElement el, uids_)
    {
        size += el.total_size(output);
    }
    return size;
}


///////////////////////////////////////////////////////////////////////////////
// TrackOperationBase I/O
///////////////////////////////////////////////////////////////////////////////

std::streamsize TrackOperationBase::write_body(std::ostream& output)
{
    std::streamsize written(0);
    BOOST_FOREACH(UIntElement el, uids_)
    {
        written += el.write(output);
    }
    return written;
}


std::streamsize TrackOperationBase::read_body(std::istream& input)
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
        throw MissingChild() << err_id(ids::TrackJoinID) << err_par_id(id_) <<
            err_pos(el_start);
    }

    return read_bytes;
}


///////////////////////////////////////////////////////////////////////////////
// TrackOperation Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

TrackOperation::TrackOperation()
    : MasterElement(ids::TrackOperation)
{
}


///////////////////////////////////////////////////////////////////////////////
// TrackOperation Accessors
///////////////////////////////////////////////////////////////////////////////

std::vector<TrackOperationBase> TrackOperation::operations() const
{
    return operations_;
}


std::streamsize TrackOperation::size() const
{
    std::streamsize size(0);
    BOOST_FOREACH(TrackOperationBase el, operations_)
    {
        size += el.total_size(output);
    }
    return size;
}


///////////////////////////////////////////////////////////////////////////////
// TrackOperation I/O
///////////////////////////////////////////////////////////////////////////////

std::streamsize TrackOperation::write_body(std::ostream& output)
{
    std::streamsize written(0);
    BOOST_FOREACH(TrackOperationBase el, operations_)
    {
        written += el.write(output);
    }
    return written;
}


std::streamsize TrackOperation::read_body(std::istream& input)
{
    std::streampos el_start(input.tellg());

    // Get the element's body size
    vint::ReadResult result = tide::vint::read(input);
    std::streamsize body_size(result.first);
    std::streamsize size_size(result.second);
    std::streamsize read_bytes(result.second);
    // Read elements until the body is exhausted
    bool have_id(false), have_offset(false);
    while (read_bytes < size_size + body_size)
    {
        if (have_id && have_offset)
        {
            // If both children have been read, why is there still body left?
            throw BadBodySize() << err_id(id_) << err_el_size(body_size) <<
                err_pos(el_start);
        }
        // Read the ID
        ids::ReadResult id_res = ids::read(input);
        ids::ID id(id_res.first);
        read_bytes += id_res.second;
        switch(id)
        {
            case ids::SeekID:
                read_bytes += indexed_id_.read_body(input);
                have_id = true;
                break;
            case ids::SeekPosition:
                read_bytes += offset_.read_body(input);
                have_offset = true;
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

    if (!have_id)
    {
        throw MissingChild() << err_id(ids::SeekID) << err_par_id(ids::Seek) <<
            err_pos(el_start);
    }
    if (!have_offset)
    {
        throw MissingChild() << err_id(ids::SeekPosition) <<
            err_par_id(ids::Seek) << err_pos(el_start);
    }

    return read_bytes;
}

