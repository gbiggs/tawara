/* Tide
 *
 * Source file for the EBMLElement object.
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

#include <tide/ebml_element.h>

#include <tide/exceptions.h>
#include <tide/tide_config.h>
#include <tide/vint.h>

using namespace tide;


///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

EBMLElement::EBMLElement(std::string const& doc_type)
    : MasterElement(ids::EBML),
    ver_(ids::EBMLVersion, 1, 1), read_ver_(ids::EBMLReadVersion, 1, 1),
    max_id_length_(ids::EBMLMaxIDLength, 4, 4),
    max_size_length_(ids::EBMLMaxSizeLength, 8, 8),
    doc_type_(ids::DocType, doc_type, TideDocType),
    doc_type_ver_(ids::DocTypeVersion, TideVersionMajor, TideVersionMajor),
    doc_type_read_ver_(ids::DocTypeReadVersion, TideVersionMajor,
            TideVersionMajor)
{
}


///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

std::streamsize EBMLElement::size() const
{
    return ver_.total_size() +
        read_ver_.total_size() +
        max_id_length_.total_size() +
        max_size_length_.total_size() +
        doc_type_.total_size() +
        doc_type_ver_.total_size() +
        doc_type_read_ver_.total_size();
}


///////////////////////////////////////////////////////////////////////////////
// I/O
///////////////////////////////////////////////////////////////////////////////

std::streamsize EBMLElement::write_body(std::ostream& output)
{
    std::streamsize written(0);
    // The EBML header element always writes every value, regardless of if it
    // is the default or not. If it did not, other implementations may use
    // different defaults and things would go very wrong, very quickly.
    written += ver_.write(output);
    written += read_ver_.write(output);
    written += max_id_length_.write(output);
    written += max_size_length_.write(output);
    written += doc_type_.write(output);
    written += doc_type_ver_.write(output);
    written += doc_type_read_ver_.write(output);
    return written;
}


std::streamsize EBMLElement::read_body(std::istream& input,
        std::streamsize size)
{
    // Start by resetting everything to the defaults
    set_defaults_();
    std::streamsize read_bytes(0);
    // Read IDs until the body is exhausted
    while (read_bytes < size)
    {
        ids::ReadResult id_res = ids::read(input);
        ids::ID id(id_res.first);
        read_bytes += id_res.second;
        switch(id)
        {
            case ids::EBMLVersion:
                read_bytes += ver_.read(input);
                break;
            case ids::EBMLReadVersion:
                read_bytes += read_ver_.read(input);
                break;
            case ids::EBMLMaxIDLength:
                read_bytes += max_id_length_.read(input);
                break;
            case ids::EBMLMaxSizeLength:
                read_bytes += max_size_length_.read(input);
                break;
            case ids::DocType:
                read_bytes += doc_type_.read(input);
                break;
            case ids::DocTypeVersion:
                read_bytes += doc_type_ver_.read(input);
                break;
            case ids::DocTypeReadVersion:
                read_bytes += doc_type_read_ver_.read(input);
                break;
            default:
                throw InvalidChildID() << err_id(id) << err_par_id(id_) <<
                    err_pos(input.tellg() - id_res.second);
        }
    }
    if (read_bytes != size)
    {
        // Read more than was specified by the body size value
        throw BadBodySize() << err_id(id_) << err_el_size(size) <<
            err_pos(offset_);
    }

    return read_bytes;
}


///////////////////////////////////////////////////////////////////////////////
// Internal methods
///////////////////////////////////////////////////////////////////////////////

void EBMLElement::set_defaults_()
{
    ver_.value(1);
    read_ver_.value(1);
    max_id_length_.value(4);
    max_size_length_.value(8);
    doc_type_.value(TideDocType);
    doc_type_ver_.value(TideVersionMajor);
    doc_type_read_ver_.value(TideVersionMajor);
}

