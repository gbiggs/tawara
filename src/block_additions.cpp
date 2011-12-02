/* Tide
 *
 * Source file for the BlockAdditions element.
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

#include <tide/block_additions.h>

#include <boost/foreach.hpp>
#include <tide/binary_element.h>
#include <tide/el_ids.h>
#include <tide/ebml_int.h>
#include <tide/exceptions.h>
#include <tide/uint_element.h>
#include <tide/vint.h>

using namespace tide;


///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

BlockAdditions::BlockAdditions()
    : MasterElement(ids::BlockAdditions)
{
}


///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////

bool tide::operator==(BlockAdditions const& lhs, BlockAdditions const& rhs)
{
    return lhs.additions_ == rhs.additions_;
}


///////////////////////////////////////////////////////////////////////////////
// Element interface
///////////////////////////////////////////////////////////////////////////////

std::streamsize BlockAdditions::body_size() const
{
    std::streamsize result(0);
    BOOST_FOREACH(value_type add, additions_)
    {
        std::streamsize this_size(0);
        // BlockAddID defaults to 1
        if (add->first != 1)
        {
            this_size += ids::size(ids::BlockAddID) +
                vint::size(ebml_int::size_u(add->first)) +
                ebml_int::size_u(add->first);
        }
        this_size += ids::size(ids::BlockAdditional) +
            vint::size(add->second.size()) + add->second.size();
        result += ids::size(ids::BlockMore) + vint::size(this_size) + this_size;
    }
    return result;
}


std::streamsize BlockAdditions::write_body(std::ostream& output)
{
    std::streamsize written(0);

    if (additions_.empty())
    {
        throw EmptyBlockAdditionsElement();
    }

    BOOST_FOREACH(value_type add, additions_)
    {
        UIntElement add_id(ids::BlockAddID, add->first, 1);
        BinaryElement additional(ids::BlockAdditional, add->second);

        written += ids::write(ids::BlockMore, output);
        std::streamsize this_size(0);
        // BlockAddID defaults to 1
        if (!add_id.is_default())
        {
            this_size += add_id.size();
        }
        this_size += additional.size();
        written += vint::write(this_size, output);
        if (!add_id.is_default())
        {
            written += add_id.write(output);
        }
        written += additional.write(output);
    }

    return written;
}


std::streamsize BlockAdditions::read_body(std::istream& input,
        std::streamsize size)
{
    // Clear the additions
    additions_.clear();
    std::streamsize read_bytes(0);
    // Read elements until the body is exhausted
    while (read_bytes < size)
    {
        // Read the ID
        ids::ReadResult id_res = ids::read(input);
        ids::ID id(id_res.first);
        read_bytes += id_res.second;
        if (id != ids::BlockMore)
        {
            // Only BlockMore elements may be in the BlockAdditions element
            throw InvalidChildID() << err_id(id) << err_par_id(id_) <<
                err_pos(input.tellg());
        }
        // Read the size
        vint::ReadResult size_res = vint::read(input);
        read_bytes += size_res.second;
        read_bytes += read_addition(input, size_res.first);
    }
    if (read_bytes != size)
    {
        // Read more than was specified by the body size value
        throw BadBodySize() << err_id(id_) << err_el_size(size) <<
            err_pos(offset_);
    }
    if (additions_.empty())
    {
        // No BlockMores is bad.
        throw EmptyBlockAdditionsElement() << err_pos(offset_);
    }

    return read_bytes;
}


std::streamsize BlockAdditions::read_addition(std::istream& input,
        std::streamsize size)
{
    std::streampos el_start(input.tellg());
    UIntElement addid(ids::BlockAddID, 1, 1);
    BinaryElement additional(ids::BlockAdditional, std::vector<char>());
    bool have_addid(false), have_additional(false);
    std::streamsize read_bytes(0);
    // Read elements until the body is exhausted
    while (read_bytes < size)
    {
        if (have_addid && have_additional)
        {
            // If both children have been read, why is there still body left?
            throw BadBodySize() << err_id(id_) << err_el_size(size) <<
                err_pos(el_start);
        }
        // Read the ID
        ids::ReadResult id_res = ids::read(input);
        ids::ID id(id_res.first);
        read_bytes += id_res.second;
        switch (id)
        {
            case ids::BlockAddID:
                read_bytes += addid.read(input);
                have_addid = true;
                break;
            case ids::BlockAdditional:
                read_bytes += additional.read(input);
                have_additional = true;
                break;
            default:
                throw InvalidChildID() << err_id(ids::BlockMore) <<
                    err_par_id(id_) <<
                    // The cast here makes Apple's LLVM compiler happy
                    err_pos(static_cast<std::streamsize>(input.tellg()) -
                            id_res.second);
        }
    }
    if (read_bytes != size)
    {
        // Read more than was specified by the body size value
        throw BadBodySize() << err_id(id_) << err_el_size(size) <<
            err_pos(el_start);
    }
    if (!have_additional)
    {
        throw MissingChild() << err_id(ids::BlockAdditional) <<
            err_par_id(ids::BlockMore) << err_pos(el_start);
    }
    value_type new_addition(new Addition(addid.value(), additional.value()));
    additions_.push_back(new_addition);

    return read_bytes;
}

