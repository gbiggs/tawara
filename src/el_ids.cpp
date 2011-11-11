/* Tide
 *
 * EBML class ID management.
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

#include <tide/el_ids.h>
#include <tide/exceptions.h>


std::streamsize tide::ids::coded_size(uint64_t id)
{
    if (id >= 0x80 && id <= 0xFE)
    {
        return 1;
    }
    else if (id >= 0x4000 && id <= 0x7FFE)
    {
        return 2;
    }
    else if (id >= 0x200000 && id <= 0x3FFFFE)
    {
        return 3;
    }
    else if (id >= 0x10000000 && id <= 0x1FFFFFFE)
    {
        return 4;
    }
    else if (id >= 0x0800000000 && id <= 0x0FFFFFFFFE)
    {
        return 5;
    }
    else if (id >= 0x040000000000 && id <= 0x07FFFFFFFFFE)
    {
        return 6;
    }
    else if (id >= 0x02000000000000 && id <= 0x03FFFFFFFFFFFE)
    {
        return 7;
    }
    else if (id >= 0x0100000000000000 && id <= 0x01FFFFFFFFFFFFFE)
    {
        return 8;
    }
    else
    {
        throw tide::InvalidEBMLID() << tide::err_varint(id);
    }
}


std::streamsize tide::ids::write(uint64_t id, std::ostream& output)
{
    std::streamsize c_size(coded_size(id));
    // Write the remaining bytes
    for (unsigned int ii(0); ii < c_size; ++ii)
    {
        output.put((id >> (c_size - ii - 1) * 8) & 0xFF);
    }
    if (!output)
    {
        throw tide::WriteError() << tide::err_pos(output.tellp());
    }

    return c_size;
}


tide::ids::ReadResult tide::ids::read(std::istream& input)
{
    uint64_t result(0);
    std::streamsize to_copy(0);
    uint8_t buffer[8];

    // Read the first byte
    input.read(reinterpret_cast<char*>(buffer), 1);
    if (!input)
    {
        throw tide::ReadError() << tide::err_pos(input.tellg());
    }
    result = buffer[0];
    // Check the size
    if (buffer[0] >= 0x80) // 1 byte
    {
        to_copy = 0;
    }
    else if (buffer[0] >= 0x40) // 2 bytes
    {
        to_copy = 1;
    }
    else if (buffer[0] >= 0x20) // 3 bytes
    {
        to_copy = 2;
    }
    else if (buffer[0] >= 0x10) // 4 bytes
    {
        to_copy = 3;
    }
    else if (buffer[0] >= 0x08) // 5 bytes
    {
        to_copy = 4;
    }
    else if (buffer[0] >= 0x04) // 6 bytes
    {
        to_copy = 5;
    }
    else if (buffer[0] >= 0x02) // 7 bytes
    {
        to_copy = 6;
    }
    else if (buffer[0] == 0x01) // 8 bytes
    {
        // The first byte is not part of the result in this case
        to_copy = 7;
    }
    else
    {
        // All bits zero is invalid
        throw tide::InvalidVarInt();
    }

    // Copy the remaining bytes
    input.read(reinterpret_cast<char*>(&buffer[1]), to_copy);
    if (input.fail())
    {
        throw tide::ReadError() << tide::err_pos(input.tellg());
    }

    for (std::streamsize ii(1); ii < to_copy + 1; ++ii)
    {
        result <<= 8;
        result += buffer[ii];
    }

    // Calling coded_size provides a check on the value of the ID, throwing
    // InvalidEBMLID if it is not in one of the allowable ranges.
    try
    {
        coded_size(result);
    }
    catch(boost::exception& e)
    {
        e << tide::err_pos(input.tellg());
        throw;
    }
    return std::make_pair(result, to_copy + 1);
}

