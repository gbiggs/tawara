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

#include <limits>
#include <tide/exceptions.h>
#include <tide/vint.h>

using namespace tide;

///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

Element::Element(uint32_t id)
    : id_(id), offset_(std::numeric_limits<std::streampos>::max())
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

std::streamsize Element::size() const
{
    std::streamsize body(body_size());
    return tide::ids::coded_size(id_) + tide::vint::coded_size(body) +
        body;
}


///////////////////////////////////////////////////////////////////////////////
// I/O
///////////////////////////////////////////////////////////////////////////////

std::streamsize Element::write(std::ostream& output)
{
    // Fill in the offset of this element in the byte stream.
    offset_ = output.tellp();

    return write_id(output) + write_size(output) + write_body(output);
}


std::streamsize Element::write_id(std::ostream& output)
{
    return tide::ids::write(id_, output);
}


std::streamsize Element::write_size(std::ostream& output)
{
    return tide::vint::write(body_size(), output);
}


std::streamsize Element::read(std::istream& input)
{
    // Fill in the offset of this element in the byte stream.
    // The input stream will be at the start of the size value, so:
    //
    //  offset = current position - size of ID
    //
    // The cast here makes Apple's LLVM compiler happy
    offset_ = static_cast<std::streamsize>(input.tellg()) -
        ids::coded_size(id_);
    // Get the element's body size
    vint::ReadResult result = tide::vint::read(input);
    std::streamsize body_size(result.first);
    std::streamsize read_bytes(result.second);
    // The rest of the read is implemented by child classes
    return read_bytes + read_body(input, body_size);
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

