/* TIDE
 *
 * Variable-length integer header file.
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

#if !defined(TIDE_VINT_H_)
#define TIDE_VINT_H_

#include <cstddef>
#include <iostream>
#include <stdint.h>

namespace tide
{
    uint8_t* encode_vint(int64_t integer, uint8_t* buffer, size_t size);

    int64_t decode_vint(uint8_t const* buffer);

    void write_vint(int64_t integer, std::ostream& file);

    int64_t read_vint(std::istream& file);
}; // namespace tide

#endif // TIDE_VINT_H_

