/* TIDE
 *
 * Source file for the void element object.
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

#include <tide/void_element.h>

#include <tide/exceptions.h>
#include <tide/vint.h>

using namespace tide;


///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

VoidElement::VoidElement(size_t size, bool fill)
    : Element(VOID_ELEMENT_ID), size_(size), fill_(fill), extra_size_(0)
{
}


VoidElement::VoidElement(Element const& element, bool fill)
    : Element(VOID_ELEMENT_ID), fill_(fill), extra_size_(0)
{
    // Set this element's size from the total size of the element to replace.
    // We need to calculate an appropriate body size that, when combined with
    // the data size and the ID size, will give the same size as
    // element.total_size(). Start by estimating the bytes required for the
    // body size.
    size_ = element.total_size() - 1;
    size_ -= tide::vint::coded_size(size_);
    // Check if enough space is used
    if (total_size() != element.total_size())
    {
        // Need to use more space (typically 1 more byte), but if we increase
        // the body size, it might push the data size value over the line to
        // requiring another byte, meaning that the void element's total size
        // would go from 1 byte under to 1 byte over. Instead, we require that
        // the body size is stored with an extra byte.
        extra_size_ = 1;
    }
    assert(total_size() == element.total_size());
}


///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

size_t VoidElement::total_size() const
{
    // ID is always 1 byte
    return 1 + tide::vint::coded_size(size_) + size_ + extra_size_;
}


///////////////////////////////////////////////////////////////////////////////
// I/O
///////////////////////////////////////////////////////////////////////////////

std::streamsize VoidElement::write_id(std::ostream& output)
{
    return tide::vint::write(id_, output);
}


std::streamsize VoidElement::write_body(std::ostream& output)
{
    size_t result(0);

    // Write the body size value padded with extra bytes if necessary
    result += tide::vint::write(size_, output,
            tide::vint::coded_size(size_) + extra_size_);
    if (fill_)
    {
        std::vector<char> zeros(size_, 0);
        output.write(&zeros[0], zeros.size());
        if (!output)
        {
            throw WriteError() << err_pos(output.tellp());
        }
    }
    else
    {
        // Skip ahead in the file to the end of the body
        output.seekp(size_, std::ios::cur);
    }
    return result + size_;
}


std::streamsize VoidElement::read_body(std::istream& input)
{
    std::pair<uint64_t, size_t> result;

    // Read the body size
    result = tide::vint::read(input);
    size_ = result.first;
    // Record the extra body size byte count for future writing
    extra_size_ = result.second - tide::vint::coded_size(size_);
    // Skip the body
    input.seekg(size_, std::ios::cur);
    if (!input)
    {
        throw ReadError() << err_pos(input.tellg());
    }
    return result.second + result.first;
}

