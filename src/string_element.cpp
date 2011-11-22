/* Tide
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
// Accessors
///////////////////////////////////////////////////////////////////////////////

std::streamsize StringElement::size() const
{
    return value_.size() + padding_;
}


///////////////////////////////////////////////////////////////////////////////
// I/O
///////////////////////////////////////////////////////////////////////////////

std::streamsize StringElement::write_body(std::ostream& output)
{
    output.write(value_.c_str(), value_.size());
    std::streamsize result = value_.size();
    if (!output)
    {
        throw WriteError() << err_pos(output.tellp());
    }
    for (uint64_t ii(0); ii < padding_; ++ii)
    {
        output.put(0x00);
        ++result;
    }
    return result;
}


std::streamsize StringElement::read_body(std::istream& input)
{
    std::pair<uint64_t, std::streamsize> result;

    // Read the body size
    result = tide::vint::read(input);
    // Read the string itself
    std::vector<char> tmp(result.first);
    input.read(&tmp[0], result.first);
    if (!input)
    {
        throw ReadError() << err_pos(input.tellg()) <<
            err_reqsize(result.first);
    }
    std::string(tmp.begin(), tmp.end()).swap(value_);
    return result.second + result.first;
}

