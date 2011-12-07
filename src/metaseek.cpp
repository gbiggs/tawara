/* Tide
 *
 * Source file for the SeekHead element.
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

#include <tide/metaseek.h>

#include <boost/foreach.hpp>
#include <numeric>
#include <tide/seek_element.h>
#include <tide/vint.h>

using namespace tide;


///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

SeekHead::SeekHead()
    : MasterElement(ids::SeekHead)
{
}


///////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////

bool tide::operator==(SeekHead const& lhs, SeekHead const& rhs)
{
    return lhs.index_ == rhs.index_;
}


///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Element interface
///////////////////////////////////////////////////////////////////////////////

std::streamsize add_size(std::streamsize x, tide::SeekHead::value_type v)
{
    SeekElement se(v.first, v.second);
    return x + se.size();
}

std::streamsize SeekHead::body_size() const
{
    return std::accumulate(index_.begin(), index_.end(), 0,
            std::ptr_fun(add_size));
}


std::streamsize SeekHead::write_body(std::ostream& output)
{
    std::streamsize written(0);
    BOOST_FOREACH(value_type entry, index_)
    {
        SeekElement se(entry.first, entry.second);
        written += se.write(output);
    }
    return written;
}


std::streamsize SeekHead::read_body(std::istream& input,
        std::streamsize size)
{
    // Clear the index
    index_.clear();
    std::streamsize read_bytes(0);
    // Read elements until the body is exhausted
    while (read_bytes < size)
    {
        // Read the ID
        ids::ReadResult id_res = tide::ids::read(input);
        ids::ID id(id_res.first);
        read_bytes += id_res.second;
        if (id != ids::Seek)
        {
            // Only Seek elements may be in the SeekHead
            throw InvalidChildID() << err_id(id) << err_par_id(id_) <<
                // The cast here makes Apple's LLVM compiler happy
                err_pos(static_cast<std::streamsize>(input.tellg()) -
                        id_res.second);
        }
        // Read the body
        SeekElement se(ids::Null, 0);
        read_bytes += se.read(input);
        index_.insert(std::make_pair(se.indexed_id(), se.offset()));
    }
    if (read_bytes != size)
    {
        // Read more than was specified by the body size value
        throw BadBodySize() << err_id(id_) << err_el_size(size) <<
            err_pos(offset_);
    }

    return read_bytes;
}

