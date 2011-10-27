/* TIDE
 *
 * Source file for the file implementation of the TIDE interface.
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

#include <tide/element.h>

#include <tide/exceptions.h>

using namespace tide;

///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

Element::Element(uint32_t id)
    : id_(id)
{
    if (id_ == 0 ||
            id_ == 0xFF ||
            id_ == 0xFFFF ||
            id_ == 0xFFFFFF ||
            id_ == 0xFFFFFFFF ||
            id_ > 0x100000000)
    {
        throw InvalidElementID() << err_id(id_);
    }
}


Element::Element(Element const& rhs)
    : id_(rhs.id_)
{
    if (id_ == 0 ||
            id_ == 0xFF ||
            id_ == 0xFFFF ||
            id_ == 0xFFFFFF ||
            id_ == 0xFFFFFFFF ||
            id_ > 0x100000000)
    {
        throw InvalidElementID() << err_id(id_);
    }
}


///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

void Element::set_id(uint32_t id)
{
    if (id == 0 ||
            id == 0xFF ||
            id == 0xFFFF ||
            id == 0xFFFFFF ||
            id == 0xFFFFFFFF ||
            id > 0x100000000)
    {
        throw InvalidElementID() << err_id(id);
    }
    id_ = id;
}


///////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////

Element& Element::operator=(Element const& rhs)
{
    if (rhs.id_ == 0 ||
            rhs.id_ == 0xFF ||
            rhs.id_ == 0xFFFF ||
            rhs.id_ == 0xFFFFFF ||
            rhs.id_ == 0xFFFFFFFF ||
            rhs.id_ > 0x100000000)
    {
        throw InvalidElementID() << err_id(rhs.id_);
    }
    id_ = rhs.id_;
    return *this;
}