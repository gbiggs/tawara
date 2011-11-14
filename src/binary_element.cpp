/* Tide
 *
 * Source file for the binary element object.
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

#include <tide/binary_element.h>

#include <tide/exceptions.h>
#include <tide/vint.h>

using namespace tide;


///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

BinaryElement::BinaryElement(uint32_t id, std::vector<char> value) :
    PrimitiveElement<std::vector<char> >(id, value)
{
}


BinaryElement::BinaryElement(uint32_t id, std::vector<char> value,
        std::vector<char> default_value) :
    PrimitiveElement<std::vector<char> >(id, value, default_value)
{
}


///////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////

BinaryElement& BinaryElement::operator=(std::vector<char> const& rhs)
{
    value_ = rhs;
    return *this;
}


///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

std::streamsize BinaryElement::size() const
{
    return value_.size();
}


///////////////////////////////////////////////////////////////////////////////
// I/O
///////////////////////////////////////////////////////////////////////////////

std::streamsize BinaryElement::write_body(std::ostream& output)
{
    output.write(&value_[0], value_.size());
    if (!output)
    {
        throw WriteError() << err_pos(output.tellp());
    }
    return value_.size();
}


std::streamsize BinaryElement::read_body(std::istream& input)
{
    std::pair<uint64_t, std::streamsize> result;

    // Read the body size
    result = tide::vint::read(input);
    // Read the binary data itself
    std::vector<char> tmp(result.first);
    input.read(&tmp[0], result.first);
    if (!input)
    {
        throw ReadError() << err_pos(input.tellg()) <<
            err_reqsize(result.first);
    }
    tmp.swap(value_);
    return result.second + result.first;
}

