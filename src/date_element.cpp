/* TIDE
 *
 * Source file for the date element object.
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

#include <tide/date_element.h>

#include <tide/ebml_int.h>
#include <tide/exceptions.h>
#include <tide/vint.h>

using namespace tide;


///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

DateElement::DateElement(uint32_t id, int64_t value)
    : PrimitiveElement<int64_t>(id, value)
{
}


DateElement::DateElement(uint32_t id, int64_t value, int64_t default_value)
    : PrimitiveElement<int64_t>(id, value, default_value)
{
}


///////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////

DateElement& DateElement::operator=(int64_t const& rhs)
{
    value_ = rhs;
    return *this;
}


///////////////////////////////////////////////////////////////////////////////
// I/O
///////////////////////////////////////////////////////////////////////////////

std::streamsize DateElement::write_id(std::basic_ostream<uint8_t>& output)
{
    return tide::vint::write(id_, output);
}


std::streamsize DateElement::write_body(std::basic_ostream<uint8_t>& output)
{
    size_t result = tide::vint::write(size(), output);
    output.write(reinterpret_cast<uint8_t*>(&value_), 8);
    if (!output)
    {
        throw WriteError() << err_pos(output.tellp());
    }
    return result + 8;
}


std::streamsize DateElement::read_body(std::basic_istream<uint8_t>& input)
{
    std::pair<uint64_t, size_t> result;

    result = tide::vint::read(input);
    if (result.first != 8)
    {
        throw BadElementLength() << err_pos(input.tellg()) << err_id(id_) <<
            err_valid_sizes(std::vector<size_t>(1, 8)) <<
            err_el_size(result.first);
    }
    input.read(reinterpret_cast<uint8_t*>(&value_), 8);
    if (!input)
    {
        throw ReadError() << err_pos(input.tellg());
    }
    return result.second + 8;
}


size_t DateElement::size() const
{
    // Dates are always stored as the full 8 bytes
    return 8;
}


size_t DateElement::total_size() const
{
    size_t data_size(size());
    // The size value will always be 1 byte, as the data cannot use more than 8
    // bytes.
    return tide::vint::coded_size(id_) + 1 + data_size;
}

