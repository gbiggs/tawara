/* Tide
 *
 * Source file for the unsigned integer element object.
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

#include <tide/uint_element.h>

#include <tide/ebml_int.h>
#include <tide/vint.h>

using namespace tide;


///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

UIntElement::UIntElement(uint32_t id, uint64_t value)
    : PrimitiveElement<uint64_t>(id, value)
{
}


UIntElement::UIntElement(uint32_t id, uint64_t value, uint64_t default_value)
    : PrimitiveElement<uint64_t>(id, value, default_value)
{
}


///////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////

UIntElement& UIntElement::operator=(uint64_t const& rhs)
{
    value_ = rhs;
    return *this;
}


///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Element interface
///////////////////////////////////////////////////////////////////////////////

std::streamsize UIntElement::body_size() const
{
    return tide::ebml_int::coded_size_u(value_);
}


std::streamsize UIntElement::write_body(std::ostream& output)
{
    return tide::ebml_int::write_u(value_, output);
}


std::streamsize UIntElement::read_body(std::istream& input,
        std::streamsize size)
{
    value_ = tide::ebml_int::read_u(input, size);
    return size;
}

