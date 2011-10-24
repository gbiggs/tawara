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

#include <tide/tide_file.h>

#include <tide/exceptions.h>
#include <tide/null_stream.h>

using namespace tide;


//////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
//////////////////////////////////////////////////////////////////////////////

TideFile::TideFile(std::string name, MODE mode)
    : Tide(name, mode),
    name_(name), mode_(mode), verb_(std::clog)
{
    if (name == "")
    {
        throw NoName();
    }
}


TideFile::TideFile(std::string name, MODE mode, std::ostream& verb_out)
    : Tide(name, mode),
    name_(name), mode_(mode), verb_(verb_out)
{
    if (name == "")
    {
        throw NoName();
    }
}


TideFile::~TideFile()
{
}

