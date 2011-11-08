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
#include <tide/exceptions.h>

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
        // Header found; check DocType
        // Header not found: throw
        // Return to the current read position
        stream_.seekg(cur);
    }
}

