/* Tide
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
// Accessors
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Element interface
///////////////////////////////////////////////////////////////////////////////

std::streamsize DateElement::body_size() const
{
    // Dates are always stored as the full 8 bytes
    return 8;
}


std::streamsize DateElement::write_body(std::ostream& output)
{
    output.write(reinterpret_cast<char*>(&value_), 8);
    if (!output)
    {
        throw WriteError() << err_pos(output.tellp());
    }
    return 8;
}


std::streamsize DateElement::read_body(std::istream& input,
        std::streamsize size)
{
    if (size != 8)
    {
        throw BadElementLength() << err_pos(offset_) << err_id(id_) <<
            err_valid_sizes(std::vector<std::streamsize>(1, 8)) <<
            err_el_size(size);
    }
    input.read(reinterpret_cast<char*>(&value_), 8);
    if (!input)
    {
        throw ReadError() << err_pos(input.tellg());
    }
    return 8;
}

