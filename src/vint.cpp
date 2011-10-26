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


uint8_t* tide::encode_vint(uint64_t integer, uint8_t* buffer, size_t n)
{
    assert(n > 0);

    unsigned int shifts(0);
    uint8_t mask(0);

    if (integer < 0x80)
    {
        shifts = 0;
        mask = 0x80;
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
        throw VarIntTooBig() << err_varint(integer) << err_bufsize(n);
    }

    // Buffer size must be at least the number of shifts + 1
    if (n < shifts + 1)
    {
        throw BufferTooSmall() << err_varint(integer) << err_bufsize(n);
    }

    for(int ii(shifts); ii > 0; --ii)
    {
        buffer[ii] = (integer >> (shifts - ii) * 8) & 0xFF;
    }
    buffer[0] = ((integer >> shifts * 8) & 0xFF) | mask;

    return buffer;
}


uint64_t tide::decode_vint(uint8_t const* buffer, size_t n)
{
    assert(n > 0);

    uint64_t result(0);
    int to_copy(0);
    if (buffer[0] >= 0x80) // 1 byte
    {
        result = buffer[0] & 0x7F;
        to_copy = 0;
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
    else if (buffer[0] >= 0x04) // 5 bytes
    {
        result = buffer[0] & 0x03;
        to_copy = 5;
    }
    else if (buffer[0] >= 0x02) // 5 bytes
    {
        result = buffer[0] & 0x01;
        to_copy = 6;
    }
    else if (buffer[0] == 0x01) // 5 bytes
    {
        // The first byte is not part of the result in this case
        to_copy = 7;
    }
    else
    {
        // All bits zero is invalid
        throw InvalidVarInt();
    }

    if (n <= to_copy)
    {
        throw BufferTooSmall() << err_bufsize(n);
    }

    // Copy the remaining bytes
    for (int ii(1); ii < to_copy + 1; ++ii)
    {
        result <<= 8;
        result += buffer[ii];
    }
    return result;
}


std::ostream& write_vint(uint64_t integer, std::ostream& output)
{
    unsigned int shifts(0);
    uint8_t mask(0);

    if (integer < 0x80)
    {
        shifts = 0;
        mask = 0x80;
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
        throw VarIntTooBig() << err_varint(integer) << err_bufsize(n);
    }

    // Write the first byte with its length indicator
    output.put(((integer >> shifts * 8) & 0xFF) | mask);
    // Write the remaining bytes
    for (int ii(1); ii <= shifts; ++ii)
    {
        output.put((integer >> (shifts - ii) * 8) & 0xFF);
    }

    return output;
}


uint64_t read_vint(std::istream& file)
{
}

