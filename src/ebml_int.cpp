/* TIDE
 *
 * EBML-format integer source file.
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

#include <tide/coded_int.h>
#include <tide/exceptions.h>


size_t tide::ebml_int::coded_size_u(uint64_t integer)
{
    if (integer <= 0xFF)
    {
        return 1;
    }
    else if (integer <= 0xFFFF)
    {
        return 2;
    }
    else if (integer <= 0xFFFFFF)
    {
        return 3;
    }
    else if (integer <= 0xFFFFFFFF)
    {
        return 4;
    }
    else if (integer <= 0xFFFFFFFFFF)
    {
        return 5;
    }
    else if (integer <= 0xFFFFFFFFFFFF)
    {
        return 6;
    }
    else if (integer <= 0xFFFFFFFFFFFFFF)
    {
        return 7;
    }
    else
    {
        return 8;
    }
}


size_t tide::ebml_int::coded_size_s(int64_t integer)
{
    if (integer >= -0x80 && integer <= 0x7F)
    {
        return 1;
    }
    else if (integer >= -0x8000 && integer <= 0x7FFF)
    {
        return 2;
    }
    else if (integer >= -0x800000 && integer <= 0x7FFFFF)
    {
        return 3;
    }
    else if (integer >= -0x80000000 && integer <= 0x7FFFFFFF)
    {
        return 4;
    }
    else if (integer >= -0x8000000000 && integer <= 0x7FFFFFFFFF)
    {
        return 5;
    }
    else if (integer >= -0x800000000000 && integer <= 0x7FFFFFFFFFFF)
    {
        return 6;
    }
    else if (integer >= -0x80000000000000 && integer <= 0x7FFFFFFFFFFFFF)
    {
        return 7;
    }
    else
    {
        return 8;
    }
}


size_t tide::ebml_int::encode_u(uint64_t integer, uint8_t* buffer,
        size_t n)
{
    size_t size(coded_size(integer));
    if (n < size)
    {
        throw tide::BufferTooSmall() << tide::err_bufsize(n) <<
            tide::err_reqsize(size);
    }
    for (size_t ii(0); ii < size; ++ii)
    {
        buffer[size - ii - 1] = integer & 0xFF;
        integer >>= 8;
    }

    return size;
}


size_t tide::ebml_int::encode_s(int64_t integer, uint8_t* buffer,
        size_t n)
{
    size_t size(coded_size(integer));
    if (n < size)
    {
        throw tide::BufferTooSmall() << tide::err_bufsize(n) <<
            tide::err_reqsize(size);
    }
    for (size_t ii(0); ii < size; ++ii)
    {
        buffer[size - ii - 1] = integer & 0xFF;
        integer >>= 8;
    }

    return size;
}


uint64_t tide::ebml_int::decode_u(uint8_t const* buffer, size_t n)
{
    uint64_t result(0);
    for (size_t ii(0); ii < n; ++ii)
    {
        result <<= 8;
        result |= buffer[ii];
    }
    return result;
}


int64_t tide::ebml_int::decode_s(uint8_t const* buffer, size_t n)
{
    int64_t result(0);
    for (size_t ii(0); ii < n; ++ii)
    {
        result <<= 8;
        result |= buffer[ii];
    }
    if (buffer[0] & 0x80)
    {
        // Negative value
        result *= -1;
    }
    return result;
}

