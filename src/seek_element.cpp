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


///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

SeekElement::SeekElement()
    : MasterElement(ids::Seek)
{
}


///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

size_t SeekElement::size() const
{
    return indexed_id_.total_size() + offset_.total_size();
}


///////////////////////////////////////////////////////////////////////////////
// I/O
///////////////////////////////////////////////////////////////////////////////

std::streamsize SeekElement::write_body(std::ostream& output)
{
    std::streamsize written(0);
    result += indexed_id_.write(output);
    result += offset_.write(output);
    return result;
}


std::streamsize Metaseek::read_body(std::istream& input)
{
    std::streamsize el_start(input.tellg());

    // Get the element's body size
    vint::read_result result = tide::vint::read(input);
    std::streamsize body_size(result.first);
    std::streamsize read_bytes(result.second);
    // Read elements until the body is exhausted
    bool have_id(false), have_offset(false);
    while (read_bytes < body_size + result.second)
    {
        if (have_id && have_offset)
        {
            // If both children have been read, why is there still body left?
            throw BadBodySize() << err_id(id_) << err_el_size(body_size) <<
                err_pos(el_start);
        }
        // Read the ID
        result = tide::vint::read(input);
        ids::ID id(result.first);
        read_bytes += result.second;
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


