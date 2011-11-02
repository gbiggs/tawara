/* TIDE
 *
 * Source file for the signed integer element object.
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

#include <tide/int_element.h>
#include <tide/ebml_int.h>
#include <tide/vint.h>

using namespace tide;


///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

IntElement::IntElement(uint32_t id, int64_t value)
    : PrimitiveElement<int64_t>(id, value)
{
}


IntElement::IntElement(uint32_t id, int64_t value, int64_t default_value)
    : PrimitiveElement<int64_t>(id, value, default_value)
{
}


///////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////

IntElement& IntElement::operator=(int64_t const& rhs)
{
    value_ = rhs;
    return *this;
}


///////////////////////////////////////////////////////////////////////////////
// I/O
///////////////////////////////////////////////////////////////////////////////

std::streamsize IntElement::write_id(std::ostream& output)
{
    return tide::vint::write(id_, output);
}


std::streamsize IntElement::write_body(std::ostream& output)
{
    size_t result(0);

    result += tide::vint::write(size(), output);
    result += tide::ebml_int::write_s(value_, output);
    return result;
}


std::streamsize IntElement::read_body(std::istream& input)
{
    std::pair<uint64_t, size_t> result;

    result = tide::vint::read(input);
    value_ = tide::ebml_int::read_s(input, result.first);
    return result.second + result.first;
}


size_t IntElement::size() const
{
    return tide::ebml_int::coded_size_s(value_);
}


size_t IntElement::total_size() const
{
    size_t data_size(size());
    // The size value will always be 1 byte, as the data cannot use more than 8
    // bytes.
    return tide::vint::coded_size(id_) + 1 + data_size;
}

