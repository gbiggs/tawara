/* TIDE
 *
 * Source file for the string element object.
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

#include <tide/string_element.h>

#include <tide/exceptions.h>
#include <tide/vint.h>

using namespace tide;


///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

StringElement::StringElement(uint32_t id, std::string value)
    : PrimitiveElement<std::string>(id, value),
    padding_(0)
{
}


StringElement::StringElement(uint32_t id, std::string value,
        std::string default_value)
    : PrimitiveElement<std::string>(id, value, default_value),
    padding_(0)
{
}


///////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////

StringElement& StringElement::operator=(std::string const& rhs)
{
    value_ = rhs;
    return *this;
}


///////////////////////////////////////////////////////////////////////////////
// I/O
///////////////////////////////////////////////////////////////////////////////

std::streamsize StringElement::write_id(std::basic_ostream<uint8_t>& output)
{
    return tide::vint::write(id_, output);
}


std::streamsize StringElement::write_body(std::basic_ostream<uint8_t>& output)
{
    size_t result(0);

    result += tide::vint::write(size(), output);
    output.write(reinterpret_cast<uint8_t const*>(value_.c_str()),
            value_.size());
    result += value_.size();
    if (!output)
    {
        throw WriteError() << err_pos(output.tellp());
    }
    for (size_t ii(0); ii < padding_; ++ii)
    {
        output.put(0x00);
        ++result;
    }
    return result;
}


std::streamsize StringElement::read_body(std::basic_istream<uint8_t>& input)
{
    std::pair<uint64_t, size_t> result;

    result = tide::vint::read(input);
    return result.second + result.first;
}


size_t StringElement::size() const
{
    return value_.size() + padding_;
}


size_t StringElement::total_size() const
{
    size_t data_size(size());
    return tide::vint::coded_size(id_) + tide::vint::coded_size(data_size) +
        data_size;
}

