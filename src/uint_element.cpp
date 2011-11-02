/* TIDE
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
 * This file is part of TIDE.
 *
 * TIDE is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * TIDE is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with TIDE. If not, see <http://www.gnu.org/licenses/>.
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
// I/O
///////////////////////////////////////////////////////////////////////////////

std::streamsize UIntElement::write_id(std::ostream& output)
{
    return tide::vint::write(id_, output);
}


std::streamsize UIntElement::write_body(std::ostream& output)
{
    size_t result(0);

    result += tide::vint::write(size(), output);
    result += tide::ebml_int::write_u(value_, output);
    return result;
}


std::streamsize UIntElement::read_body(std::istream& input)
{
    std::pair<uint64_t, size_t> result;

    result = tide::vint::read(input);
    value_ = tide::ebml_int::read_u(input, result.first);
    return result.second + result.first;
}


size_t UIntElement::size() const
{
    return tide::ebml_int::coded_size_u(value_);
}


size_t UIntElement::total_size() const
{
    size_t data_size(size());
    // The size value will always be 1 byte, as the data cannot use more than 8
    // bytes.
    return tide::vint::coded_size(id_) + 1 + data_size;
}

