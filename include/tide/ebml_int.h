/* TIDE
 *
 * Coded integer header file.
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
#include <istream>
#include <ostream>
#include <stdint.h>

namespace tide
{
    /** \brief Functions for managing integers coded for EBML.
     *
     * This namespace contains the functions used to mange the way integers,
     * both signed and unsigned, are stored in EBML files. Rather than writing
     * a constant number of bytes regardless of the value being stored, EBML
     * specifies that leading 0x00 bytes and, for negative signed integers,
     * leading 0xFF bytes, be trimmed. For example, a value of 2 will be stored
     * as 0x02, even if the variable that holds it is 32-bit (i.e. 0x00000002).
     * Similarly, a value of -30000, or 0xFFFFF530 in 32 bits, will be stored
     * as 0xF530.
     *
     * Note that this is distinct from the coding used on EBML Element IDs and
     * data sizes, which relies on leading zero bits to indicate the stored
     * size.
     */
    namespace ebml_int
    {
        /** \brief Get the size of an unsigned integer after encoding.
         *
         * The size required by an encoded integer depends on the value of that
         * integer, and will range from 1 to 8 bytes.
         *
         * \param[in] integer The integer to get the size of.
         * \return The size, in bytes, that the integer will require when
         * coded.
         */
        size_t coded_size_u(uint64_t integer);

        /** \brief Get the size of a signed integer after encoding.
         *
         * The size required by an encoded integer depends on the value of that
         * integer, and will range from 1 to 8 bytes.
         *
         * \param[in] integer The integer to get the size of.
         * \return The size, in bytes, that the integer will require when
         * coded.
         */
        size_t coded_size_s(int64_t integer);

        /** \brief Encode an unsigned integer into a buffer.
         *
         * Encodes an unsigned integer according to the EBML specification for
         * unsigned integers. Leading zero bytes are trimmed.
         *
         * \param[in] integer The integer to encode.
         * \param[in] buffer A pointer to the buffer into which to put the
         * bytes.
         * \param[in] n The length of the buffer available for use.
         * \return The number of bytes actually used.
         * \exception BufferTooSmall if the integer is above the maximum size
         * that can fit in the available buffer space.
         */
        size_t encode_u(uint64_t integer, uint8_t* buffer, size_t n);

        /** \brief Encode a signed integer into a buffer.
         *
         * Encodes an unsigned integer according to the EBML specification for
         * signed integers. Leading zero or 0xFF bytes are trimmed.
         *
         * \param[in] integer The integer to encode.
         * \param[in] buffer A pointer to the buffer into which to put the
         * bytes.
         * \param[in] n The length of the buffer available for use.
         * \return The number of bytes actually used.
         * \exception BufferTooSmall if the integer is above the maximum size
         * that can fit in the available buffer space.
         */
        size_t encode_s(int64_t integer, uint8_t* buffer, size_t n);

        /** \brief Decode an unsigned integer from a buffer.
         *
         * Decodes the unsigned integer stored in the buffer according to the
         * EBML specification for unsigned integers.
         *
         * \param[in] buffer The buffer holding the raw data.
         * \param[in] n The number of bytes from the buffer to read.
         * \return The decoded unsigned integer.
         */
        uint64_t decode_u(uint8_t const* buffer, size_t n);

        /** \brief Decode an unsigned integer from a buffer.
         *
         * Decodes the unsigned integer stored in the buffer according to the
         * EBML specification for unsigned integers.
         *
         * \param[in] buffer The buffer holding the raw data.
         * \param[in] n The number of bytes from the buffer to read.
         * \return The decoded unsigned integer.
         */
        uint64_t decode_s(uint8_t const* buffer, size_t n);
    }; // namespace ebml_int
}; // namespace tide

#endif // TIDE_VINT_H_

