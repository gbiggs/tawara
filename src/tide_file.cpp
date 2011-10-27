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

#include <boost/filesystem.hpp>
#include <tide/exceptions.h>

using namespace tide;


///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

TideFile::TideFile(std::string name, MODE mode)
    : Tide(name, mode),
    name_(name), mode_(mode), verb_(std::clog)
{
    if (name == "")
    {
        verb_ << "No name provided to TideFile()\n";
        throw NoName();
    }

    open();
}


TideFile::TideFile(std::string name, MODE mode, std::ostream& verb_out)
    : Tide(name, mode),
    name_(name), mode_(mode), verb_(verb_out)
{
    if (name == "")
    {
        verb_ << "No name provided to TideFile()\n";
        throw NoName();
    }

    open();
}


TideFile::~TideFile()
{
}


///////////////////////////////////////////////////////////////////////////////
// Private methods
///////////////////////////////////////////////////////////////////////////////

void TideFile::open()
{
    switch(mode_)
    {
        case MODE_READ:
            file_.open(name_.c_str(), std::ios::in);
            if (!file_)
            {
                verb_ << "Failed to open file " << name_ <<
                    ": File not found\n";
                throw NoObject() << err_name(name_);
            }
            if (!validate_ebml_header())
            {
                throw NotTide() << err_name(name_);
            }
            // Also check if the file still has stuff to read? ie is not eof
            break;
        case MODE_WRITE:
            // out|trunc
            break;
        case MODE_APPEND:
            // Check if the file size is zero
            bool new_file(false);
            if (!boost::filesystem::is_regular_file(name_))
            {
                verb_ << "File to append to, " << name_ <<
                    ", does not exist.\n";
                new_file = true;
            }
            else if (boost::filesystem::file_size(name_) == 0)
            {
                verb_ << "Existing file " << name_ << " is empty.\n";
                new_file = true;
            }
            file_.open(name_.c_str(), std::ios::out | std::ios::app);
            if (!file_)
            {
                verb_ << "Failed to open file " << name_ <<
                    '\n';
                throw NoObject() << err_name(name_);
            }
            if (!validate_ebml_header())
            {
                throw NotTide() << err_name(name_);
            }
            break;
    }
}


bool TideFile::validate_ebml_header()
{
    // Preserve the current read position
    // Seek to the start of the file
    // Read an element from the file
    // Validate its values
    return false;
}

