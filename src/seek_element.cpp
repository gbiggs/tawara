/* Tide
 *
 * Source file for the Seek element.
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

#include <tide/seek_element.h>

#include <tide/vint.h>

using namespace tide;


///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

SeekElement::SeekElement(ids::ID id, std::streampos offset)
    : MasterElement(ids::Seek),
    indexed_id_(ids::SeekID, tide::ids::encode(id)),
    offset_(ids::SeekPosition, offset)
{
    assert(offset >= 0);
}


///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

ids::ID SeekElement::indexed_id() const
{
    std::vector<char> bin(indexed_id_.value());
    ids::DecodeResult r(ids::decode(bin));
    return r.first;
}


void SeekElement::indexed_id(ids::ID id)
{
    indexed_id_.value(ids::encode(id));
}


std::streamsize SeekElement::size() const
{
    return indexed_id_.total_size() + offset_.total_size();
}


///////////////////////////////////////////////////////////////////////////////
// I/O
///////////////////////////////////////////////////////////////////////////////

std::streamsize SeekElement::write_body(std::ostream& output)
{
    std::streamsize written(0);
    written += indexed_id_.write(output);
    written += offset_.write(output);
    return written;
}


std::streamsize SeekElement::read_body(std::istream& input)
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


