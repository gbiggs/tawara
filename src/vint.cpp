/* TIDE
 *
 * Variable-length integer source file.
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

#include <tide/vint.h>
#include <tide/exceptions.h>


size_t tide::vint::coded_size(uint64_t integer)
{
    if (integer < 0x80)
    {
        return 1;
    }
    else if (integer < 0x4000)
    {
        return 2;
    }
    else if (integer < 0x200000)
    {
        return 3;
    }
    else if (integer < 0x10000000)
    {
        return 4;
    }
    else if (integer < 0x800000000)
    {
        return 5;
    }
    else if (integer < 0x40000000000)
    {
        return 6;
    }
    else if (integer < 0x2000000000000)
    {
        return 7;
    }
    else if (integer < 0x100000000000000)
    {
        return 8;
    }
    else
    {
        throw tide::VarIntTooBig() << tide::err_varint(integer);
    }
}


size_t tide::vint::encode(uint64_t integer, uint8_t* buffer, size_t n)
{
    assert(n > 0);

    // Short-circuit optimisations for common values
    unsigned int shifts(0);
    uint8_t mask(0);

    if (integer < 0x80)
    {
        // Short-circuit the byte-copying machinery.
        buffer[0] = integer | 0x80;
        return 1;
    }
    else if (integer < 0x4000)
    {
        shifts = 1;
        mask = 0x40;
    }
    else if (integer < 0x200000)
    {
        shifts = 2;
        mask = 0x20;
    }
    else if (integer < 0x10000000)
    {
        shifts = 3;
        mask = 0x10;
    }
    else if (integer < 0x800000000)
    {
        shifts = 4;
        mask = 0x08;
    }
    else if (integer < 0x40000000000)
    {
        shifts = 5;
        mask = 0x04;
    }
    else if (integer < 0x2000000000000)
    {
        shifts = 6;
        mask = 0x02;
    }
    else if (integer < 0x100000000000000)
    {
        shifts = 7;
        mask = 0x01;
    }
    else
    {
        throw tide::VarIntTooBig() << tide::err_varint(integer) <<
            tide::err_bufsize(n);
    }

    // Buffer size must be at least the number of shifts + 1
    if (n < shifts + 1)
    {
        throw tide::BufferTooSmall() << tide::err_varint(integer) <<
            tide::err_bufsize(n) << tide::err_reqsize(shifts + 1);
    }

    for(int ii(shifts); ii > 0; --ii)
    {
        buffer[ii] = (integer >> (shifts - ii) * 8) & 0xFF;
    }
    buffer[0] = ((integer >> shifts * 8) & 0xFF) | mask;

    return shifts + 1;
}


std::pair<uint64_t, size_t> tide::vint::decode(uint8_t const* buffer, size_t n)
{
    assert(n > 0);

    uint64_t result(0);
    size_t to_copy(0);
    if (buffer[0] >= 0x80) // 1 byte
    {
        // There will be no extra bytes to copy.
        return std::make_pair(buffer[0] & 0x7F, 1);
    }
    else if (buffer[0] >= 0x40) // 2 bytes
    {
        result = buffer[0] & 0x3F;
        to_copy = 1;
    }
    else if (buffer[0] >= 0x20) // 3 bytes
    {
        result = buffer[0] & 0x1F;
        to_copy = 2;
    }
    else if (buffer[0] >= 0x10) // 4 bytes
    {
        result = buffer[0] & 0x0F;
        to_copy = 3;
    }
    else if (buffer[0] >= 0x08) // 5 bytes
    {
        result = buffer[0] & 0x07;
        to_copy = 4;
    }
    else if (buffer[0] >= 0x04) // 6 bytes
    {
        result = buffer[0] & 0x03;
        to_copy = 5;
    }
    else if (buffer[0] >= 0x02) // 7 bytes
    {
        result = buffer[0] & 0x01;
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

    if (n <= to_copy)
    {
        throw tide::BufferTooSmall() << tide::err_bufsize(n) <<
            tide::err_reqsize(to_copy);
    }

    // Copy the remaining bytes
    for (size_t ii(1); ii < to_copy + 1; ++ii)
    {
        result <<= 8;
        result += buffer[ii];
    }
    return std::make_pair(result, to_copy + 1);
}


size_t tide::vint::write(uint64_t integer, std::ostream& output)
{
    unsigned int shifts(0);
    uint8_t mask(0);

    if (integer < 0x80)
    {
        // Short-circuit the byte-copying machinery.
        output.put(integer | 0x80);
        return 1;
    }
    else if (integer < 0x4000)
    {
        shifts = 1;
        mask = 0x40;
    }
    else if (integer < 0x200000)
    {
        shifts = 2;
        mask = 0x20;
    }
    else if (integer < 0x10000000)
    {
        shifts = 3;
        mask = 0x10;
    }
    else if (integer < 0x800000000)
    {
        shifts = 4;
        mask = 0x08;
    }
    else if (integer < 0x40000000000)
    {
        shifts = 5;
        mask = 0x04;
    }
    else if (integer < 0x2000000000000)
    {
        shifts = 6;
        mask = 0x02;
    }
    else if (integer < 0x100000000000000)
    {
        shifts = 7;
        mask = 0x01;
    }
    else
    {
        throw tide::VarIntTooBig() << tide::err_varint(integer);
    }

    // Write the first byte with its length indicator
    output.put(((integer >> shifts * 8) & 0xFF) | mask);
    // Write the remaining bytes
    for (unsigned int ii(1); ii <= shifts; ++ii)
    {
        output.put((integer >> (shifts - ii) * 8) & 0xFF);
    }
    if (!output)
    {
        throw tide::WriteError() << tide::err_pos(output.tellp());
    }

    return shifts + 1;
}


std::pair<uint64_t, size_t> tide::vint::read(std::istream& input)
{
    uint64_t result(0);
    std::streamsize to_copy(0);
    char buffer[8];

    // Read the first byte
    buffer[0] = input.get();
    // Check the size
    if (buffer[0] >= 0x80) // 1 byte
    {
        return std::make_pair(buffer[0] & 0x7F, 1);
    }
    else if (buffer[0] >= 0x40) // 2 bytes
    {
        result = buffer[0] & 0x3F;
        to_copy = 1;
    }
    else if (buffer[0] >= 0x20) // 3 bytes
    {
        result = buffer[0] & 0x1F;
        to_copy = 2;
    }
    else if (buffer[0] >= 0x10) // 4 bytes
    {
        result = buffer[0] & 0x0F;
        to_copy = 3;
    }
    else if (buffer[0] >= 0x08) // 5 bytes
    {
        result = buffer[0] & 0x07;
        to_copy = 4;
    }
    else if (buffer[0] >= 0x04) // 6 bytes
    {
        result = buffer[0] & 0x03;
        to_copy = 5;
    }
    else if (buffer[0] >= 0x02) // 7 bytes
    {
        result = buffer[0] & 0x01;
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
    input.read(&buffer[1], to_copy);
    if (!input)
    {
        throw tide::ReadError() << tide::err_pos(input.tellg());
    }

    for (std::streamsize ii(1); ii < to_copy + 1; ++ii)
    {
        result <<= 8;
        result += buffer[ii];
    }
    return std::make_pair(result, to_copy + 1);
}

