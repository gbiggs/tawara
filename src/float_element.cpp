/* TIDE
 *
 * Source file for the float element object.
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

#include <tide/float_element.h>

#include <tide/exceptions.h>
#include <tide/vint.h>
#include <vector>

using namespace tide;


///////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////

FloatElement& FloatElement::operator=(double const& rhs)
{
    value_ = rhs;
    return *this;
}


///////////////////////////////////////////////////////////////////////////////
// I/O
///////////////////////////////////////////////////////////////////////////////

std::streamsize FloatElement::write_id(std::basic_ostream<uint8_t>& output)
{
    return tide::vint::write(id_, output);
}

std::streamsize FloatElement::write_body(std::basic_ostream<uint8_t>& output)
{
    float tmp(0);
    size_t result(0);
    result += tide::vint::write(size(), output);
    switch(prec_)
    {
        case EBML_FLOAT_PREC_SINGLE:
            tmp = value_;
            output.write(reinterpret_cast<uint8_t*>(&tmp), 4);
            if (!output)
            {
                throw WriteError() << err_pos(output.tellp());
            }
            result += 4;
            break;
        case EBML_FLOAT_PREC_DOUBLE:
            output.write(reinterpret_cast<uint8_t*>(&value_), 8);
            if (!output)
            {
                throw WriteError() << err_pos(output.tellp());
            }
            result += 8;
            break;
    };
    return result;
}


std::streamsize FloatElement::read_body(std::basic_istream<uint8_t>& input)
{
    std::pair<uint64_t, size_t> result;

    result = tide::vint::read(input);
    if (result.first == 4)
    {
        float tmp(0);
        input.read(reinterpret_cast<uint8_t*>(&tmp), 4);
        if (!input)
        {
            throw ReadError() << err_pos(input.tellg());
        }
        value_ = tmp;
        prec_ = EBML_FLOAT_PREC_SINGLE;
        return result.second + 4;
    }
    else if (result.first == 8)
    {
        double tmp(0);
        input.read(reinterpret_cast<uint8_t*>(&tmp), 8);
        if (!input)
        {
            throw ReadError() << err_pos(input.tellg());
        }
        value_ = tmp;
        prec_ = EBML_FLOAT_PREC_DOUBLE;
        return result.second + 8;
    }
    else
    {
        std::vector<size_t> valid_sizes;
        valid_sizes.push_back(4); valid_sizes.push_back(8);
        throw BadElementLength() << err_pos(input.tellg()) << err_id(id_) <<
            err_valid_sizes(valid_sizes) << err_el_size(result.first);
    }
}


size_t FloatElement::size() const
{
    switch(prec_)
    {
        case EBML_FLOAT_PREC_SINGLE:
            return 4;
        case EBML_FLOAT_PREC_DOUBLE:
            return 8;
    };
}


size_t FloatElement::total_size() const
{
    // Float elements are always 4 or 8 bytes, so the data value will always be
    // 1 byte.
    return tide::vint::coded_size(id_) + 1 + size();
}

