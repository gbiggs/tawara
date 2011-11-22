/* Tide
 *
 * Source file for the base Element object.
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

#include <tide/element.h>

#include <tide/exceptions.h>
#include <tide/vint.h>

using namespace tide;

///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

Element::Element(uint32_t id)
    : id_(id)
{
    if (id_ == 0 ||
            id_ == 0xFF ||
            id_ == 0xFFFF ||
            id_ == 0xFFFFFF ||
            id_ == 0xFFFFFFFF)
    {
        throw InvalidElementID() << err_id(id_);
    }
}


///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

std::streamsize Element::total_size() const
{
    std::streamsize data_size(size());
    return tide::ids::coded_size(id_) + tide::vint::coded_size(data_size) +
        data_size;
}


///////////////////////////////////////////////////////////////////////////////
// I/O
///////////////////////////////////////////////////////////////////////////////

std::streamsize Element::write(std::ostream& output)
{
    return write_id(output) + write_size(output) + write_body(output);
}


std::streamsize Element::write_id(std::ostream& output)
{
    return tide::ids::write(id_, output);
}


std::streamsize Element::write_size(std::ostream& output)
{
    return tide::vint::write(size(), output);
}


///////////////////////////////////////////////////////////////////////////////
// Other functions in element.h
///////////////////////////////////////////////////////////////////////////////

std::streamsize tide::skip_read(std::istream& input, bool and_id)
{
    std::streamsize skipped_bytes(0);
    if (and_id)
    {
        skipped_bytes += ids::read(input).second;
    }
    vint::ReadResult size_res(vint::read(input));
    skipped_bytes += size_res.second;
    input.seekg(size_res.first, std::ios::cur);
    skipped_bytes += size_res.first;
    return skipped_bytes;
}


std::streamsize tide::skip_write(std::iostream& stream, bool and_id)
{
    std::streampos cur_read(stream.tellg());
    stream.seekg(stream.tellp());
    std::streamsize skipped_bytes = tide::skip_read(stream, and_id);
    stream.seekp(stream.tellg());
    stream.seekg(cur_read);
    return skipped_bytes;
}

