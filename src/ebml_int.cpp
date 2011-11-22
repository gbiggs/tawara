/* Tide
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

#include <tide/ebml_int.h>
#include <tide/exceptions.h>


std::streamsize tide::ebml_int::coded_size_u(uint64_t integer)
{
    if (integer == 0)
    {
        return 0;
    }
    else if (integer <= 0xFF)
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
    else if (integer <= 0xFFFFFFFFl)
    {
        return 4;
    }
    else if (integer <= 0xFFFFFFFFFFl)
    {
        return 5;
    }
    else if (integer <= 0xFFFFFFFFFFFFl)
    {
        return 6;
    }
    else if (integer <= 0xFFFFFFFFFFFFFFl)
    {
        return 7;
    }
    else
    {
        return 8;
    }
}


std::streamsize tide::ebml_int::coded_size_s(int64_t integer)
{
    if (integer == 0)
    {
        return 0;
    }
    else if (integer >= -0x80 && integer <= 0x7F)
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
    else if (integer >= -0x80000000l && integer <= 0x7FFFFFFFl)
    {
        return 4;
    }
    else if (integer >= -0x8000000000l && integer <= 0x7FFFFFFFFFl)
    {
        return 5;
    }
    else if (integer >= -0x800000000000l && integer <= 0x7FFFFFFFFFFFl)
    {
        return 6;
    }
    else if (integer >= -0x80000000000000l && integer <= 0x7FFFFFFFFFFFFFl)
    {
        return 7;
    }
    else
    {
        return 8;
    }
}


std::vector<char> tide::ebml_int::encode_u(uint64_t integer)
{
    std::vector<char> buffer;
    if (integer == 0)
    {
        // Zero values are encoded as nothing
        return buffer;
    }
    std::streamsize size(coded_size_u(integer));
    buffer.assign(size, 0);
    for (std::streamsize ii(0); ii < size; ++ii)
    {
        buffer[size - ii - 1] = integer & 0xFF;
        integer >>= 8;
    }

    return buffer;
}


std::vector<char> tide::ebml_int::encode_s(int64_t integer)
{
    std::vector<char> buffer;
    if (integer == 0)
    {
        // Zero values are encoded as nothing
        return buffer;
    }
    std::streamsize size(coded_size_s(integer));
    buffer.assign(size, 0);
    for (std::streamsize ii(0); ii < size; ++ii)
    {
        buffer[size - ii - 1] = integer & 0xFF;
        integer >>= 8;
    }

    return buffer;
}


std::streamsize tide::ebml_int::write_u(uint64_t integer, std::ostream& output)
{
    std::vector<char> buffer(encode_u(integer));

    if (buffer.size() != 0)
    {
        output.write(&buffer[0], buffer.size());
        if (!output)
        {
            throw tide::WriteError() << tide::err_pos(output.tellp());
        }
    }
    return buffer.size();
}


std::streamsize tide::ebml_int::write_s(int64_t integer, std::ostream& output)
{
    std::vector<char> buffer(encode_s(integer));
    if (buffer.size() != 0)
    {
        output.write(&buffer[0], buffer.size());
        if (!output)
        {
            throw tide::WriteError() << tide::err_pos(output.tellp());
        }
    }
    return buffer.size();
}


uint64_t tide::ebml_int::decode_u(std::vector<char> const& buffer)
{
    assert(buffer.size() <= 8);

    if (buffer.empty())
    {
        // Zero-length value means a zero-value integer
        return 0;
    }

    uint64_t result(0);
    for (unsigned int ii(0); ii < buffer.size(); ++ii)
    {
        result <<= 8;
        (reinterpret_cast<char*>(&result))[0] |= buffer[ii];
    }
    return result;
}


int64_t tide::ebml_int::decode_s(std::vector<char> const& buffer)
{
    assert(buffer.size() <= 8);

    if (buffer.empty())
    {
        // Zero-length value means a zero-value integer
        return 0;
    }

    int64_t result(0);
    if (buffer[0] & 0x80)
    {
        // Negative value
        result = -1;
    }
    for (unsigned int ii(0); ii < buffer.size(); ++ii)
    {
        result <<= 8;
        (reinterpret_cast<char*>(&result))[0] |= buffer[ii];
    }
    return result;
}


uint64_t tide::ebml_int::read_u(std::istream& input, std::streamsize n)
{
    assert(n <= 8);

    std::vector<char> tmp(n, 0);
    input.read(&tmp[0], n);
    if (!input)
    {
        throw tide::ReadError() << tide::err_pos(input.tellg());
    }
    return tide::ebml_int::decode_u(tmp);
}


int64_t tide::ebml_int::read_s(std::istream& input, std::streamsize n)
{
    assert(n <= 8);

    std::vector<char> tmp(n, 0);
    input.read(&tmp[0], n);
    if (!input)
    {
        throw tide::ReadError() << tide::err_pos(input.tellg());
    }
    return tide::ebml_int::decode_s(tmp);
}

