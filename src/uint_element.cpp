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

#include <tide/exceptions.h>

using namespace tide;

///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

UIntElement::UIntElement(uint32_t id, uint64_t value)
    : Element(id),
    value_(value), default_(0), has_default_(false)
{
}


UIntElement::UIntElement(uint32_t id, uint64_t value, uint64_t default_value)
    : Element(id),
    value_(value), default_(default_value), has_default_(true)
{
}


UIntElement::UIntElement(UIntElement const& rhs)
    : Element(rhs),
    value_(rhs.value_), default_(rhs.default_), has_default_(rhs.has_default_)
{
}


///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

void UIntElement::set_default(uint64_t default_value)
{
    default_ = default_value;
    has_default_ = true;
}


uint64_t UIntElement::remove_default()
{
    has_default_ = false;
    return default_;
}

///////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////

UIntElement& UIntElement::operator=(UIntElement const& rhs)
{
    Element::operator=(rhs);
    value_ = rhs.value_;
    default_ = rhs.default_;
    has_default_ = rhs.has_default_;
    return *this;
}

