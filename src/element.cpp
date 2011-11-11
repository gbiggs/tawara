/* Tide
 *
 * Source file for the base Element object.
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

#include <tide/element.h>

#include <tide/exceptions.h>
#include <tide/vint.h>

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
            id_ == 0xFFFFFFFF)
    {
        throw InvalidElementID() << err_id(id_);
    }
}


///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

size_t Element::total_size() const
{
    size_t data_size(size());
    return tide::ids::coded_size(id_) + tide::vint::coded_size(data_size) +
        data_size;
}


///////////////////////////////////////////////////////////////////////////////
// I/O
///////////////////////////////////////////////////////////////////////////////

std::streamsize Element::write(std::ostream& output)
{
    return write_id(output) + write_size(output) + write_body(output);
}


std::streamsize Element::write_id(std::ostream& output)
{
    return tide::ids::write(id_, output);
}


std::streamsize Element::write_size(std::ostream& output)
{
    return tide::vint::write(size(), output);
}

