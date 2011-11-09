/* Tide
 *
 * Source file for the implementation of the Tide interface.
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

#include <tide/tide_impl.h>

#include <tide/ebml_element.h>
#include <tide/el_ids.h>
#include <tide/exceptions.h>
#include <tide/vint.h>

#include <iostream>

using namespace tide;


///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

TideImpl::TideImpl(std::iostream& stream)
    : Tide(stream),
    stream_(stream)
{
    prepare_stream();
}


///////////////////////////////////////////////////////////////////////////////
// Private methods
///////////////////////////////////////////////////////////////////////////////

void TideImpl::prepare_stream()
{
    // Preserve the current read position
    std::streamsize cur(stream_.tellg());
    // Check the file size
    stream_.seekg(0, std::ios::end);
    std::streamsize size(stream_.tellg());
    stream_.seekg(cur, std::ios::beg);
    // If the file is empty, write an EBML header
    if (size <= 0)
    {
        stream_.seekg(0, std::ios::beg);
        EBMLElement e;
        e.write(stream_);
    }
    // If the file is not empty, search for an EBML header
    else
    {
        bool found_header(false);
        while (!stream_.eof())
        {
            char c(stream_.get());
            if (c == 0x1A)
            {
                stream_.seekg(-1, std::ios::cur);
                // Read the Header ID
                vint::read_result hdr = vint::read(stream_);
                if (hdr.first != ids::EBML)
                {
                    // Not an EBML header; throw
                    throw NotEBML();
                }
                found_header = true;
                break;
            }
        }
        if (!found_header)
        {
            // Header not found: throw
            throw NotEBML();
        }
        // Read the header and check the DocType
        EBMLElement e;
        e.read_body(stream_);
        if (e.doc_type() != TideDocType)
        {
            throw NotTide();
        }
        if (e.read_version() > TideVersionMajor)
        {
            throw BadReadVersion();
        }
        if (e.doc_read_version() > TideVersionMajor)
        {
            throw BadDocReadVersion();
        }
    }
}

