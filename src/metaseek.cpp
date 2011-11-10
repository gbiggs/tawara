/* Tide
 *
 * Source file for the Metaseek element.
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
#include <tide/seek_element.h>
#include <tide/vint.h>

using namespace tide;


///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

Metaseek::Metaseek()
    : MasterElement(ids::SeekHead)
{
}


///////////////////////////////////////////////////////////////////////////////
// Index management
///////////////////////////////////////////////////////////////////////////////

void Metaseek::append(Metaseek::IndexItem item)
{
    index_.push_back(SeekElement(item.first, item.second));
}


Metaseek::IndexItem Metaseek::remove(unsigned int pos)
{
    SeekElement se = index_[pos];
    index_.erase(index_.begin() + pos);
    return Metaseek::IndexItem(se.id(), se.offset());
}


Metaseek::IndexItem Metaseek::operator[](unsigned int pos)
{
    SeekElement se = index_[pos];
    return Metaseek::IndexItem(se.id(), se.offset());
}


///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

size_t Metaseek::size() const
{
    size_t result(0);
    BOOST_FOREACH(SeekElement ii, index_)
    {
        result += ii.total_size();
    }
    return result;
}


///////////////////////////////////////////////////////////////////////////////
// I/O
///////////////////////////////////////////////////////////////////////////////

std::streamsize Metaseek::write_body(std::ostream& output)
{
    std::streamsize written(0);
    BOOST_FOREACH(SeekElement ii, index_)
    {
        written += ii.write(output);
    }
    return written;
}


std::streamsize Metaseek::read_body(std::istream& input)
{
    // Clear the index
    index_.clear();
    // Get the element's body size
    vint::ReadResult result = tide::vint::read(input);
    std::streamsize body_size(result.first);
    std::streamsize read_bytes(result.second);
    // Read elements until the body is exhausted
    while (body_size > 0)
    {
        // Read the ID
        result = tide::vint::read(input);
        ids::ID id(result.first);
        read_bytes += result.second;
        body_size -= result.second;
        if (id != ids::Seek)
        {
            // Only Seek elements may be in the SeekHead
            throw InvalidChildID() << err_id(id) << err_par_id(id_) <<
                err_pos(input.tellg());
        }
        // Read the body
        SeekElement se(0, 0);
        std::streamsize el_read_bytes = se.read_body(input);
        read_bytes += el_read_bytes;
        body_size -= el_read_bytes;
        index_.push_back(se);
    }

    return read_bytes;
}

