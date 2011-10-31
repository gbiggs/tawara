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
#include <istream>
#include <ostream>
#include <stdint.h>
#include <utility>

/// \addtogroup utilities Utilities
/// @{

namespace tide
{
    /** \brief Functions for managing variable-length integers.
     *
     * This namespace contains the functions used to manage variable-length
     * integers. These are a part of the EBML specification that stores
     * unsigned integers for the Element IDs and Element data sizes in a
     * UTF-8-like encoding scheme, using leading zeros to indicate the value's
     * stored size. This allows integers of varying length to be stored without
     * needing a separate byte count value.
     *
     * Note that this is distinct from the coding used on standard integers
     * (signed and unsigned) stores in element bodies, which merely truncates
     * leading 0x00 and 0xFF values.
     */
    namespace vint
    {
        /** \brief Get the size of an integer after encoding.
         *
         * The size required by an encoded integer depends on the value of that
         * integer, and will range from 1 to 8 bytes.
         *
         * \param[in] integer The integer to get the size of.
         * \return The size, in bytes, that the integer will require when
         * coded.
         * \exception VarIntTooBig if the integer is above the maximum value
         * for variable-length integers (0xFFFFFFFFFFFFFF).
         */
        size_t coded_size(uint64_t integer);

        /** \brief Encode an unsigned integer into a buffer.
         *
         * Encodes an unsigned variable-length integer according to the EBML
         * specification. Leading zero bits are used to indicate the length of
         * the encoded integer in bytes.
         *
         * \param[in] integer The integer to encode.
         * \param[in] buffer A pointer to the buffer into which to put the
         * bytes.
         * \param[in] n The length of the buffer available for use.
         * \return The number of bytes actually used.
         * \exception VarIntTooBig if the integer is above the maximum value
         * for variable-length integers (0xFFFFFFFFFFFFFF).
         * \exception BufferTooSmall if the integer is above the maximum size
         * that can fit in the available buffer space.
         */
        size_t encode(uint64_t integer, uint8_t* buffer, size_t n);

        /** \brief Decode an unsigned variable-length integer from a buffer.
         *
         * Decodes the variable-length integer stored in the buffer.
         *
         * \param[in] buffer The buffer holding the raw data.
         * \param[in] n The length of the buffer available for reading.
         * \return A pair containing the decoded unsigned integer in the first
         * and the number of bytes used from the buffer in the second.
         * \exception InvalidVarInt if the first byte in the buffer is
         * zero, an invalid starting byte for a variable-length integer.
         * \exception BufferTooSmall if the expected encoded length of the
         * variable-length integer is larger than the available buffer length.
         */
        std::pair<uint64_t, size_t> decode(uint8_t const* buffer, size_t n);

        /** \brief Encode an unsigned integer and write it to an output stream.
         *
         * This function performs the same task as tide::vint::encode(), but it
         * writes the result to a std::ostream instead of a simple buffer.
         *
         * \param[in] integer The integer to encode.
         * \param[in] output The std::ostream object to write the encoded
         * integer to.
         * \return The number of bytes written.
         * \exception VarIntTooBig if the integer is above the maximum value
         * for variable-length integers (0xFFFFFFFFFFFFFF).
         * \exception WriteError if there is an error writing to the stream.
         */
        size_t write(uint64_t integer, std::ostream& output);

        /** \brief Decode an unsigned integer from an input stream.
         *
         * This function performs the same task as tide::vint::decode(), but it
         * reads the bytes from the input stream rather than a simple buffer.
         *
         * \param[in] input The std::istream object to read bytes from.
         * \return A pair containing the value read in the first and the number
         * of bytes read from the stream in the second.
         * \exception InvalidVarInt if the variable-length integer in the byte
         * stream is invalid.
         * \exception ReadError if there is an error reading the input stream.
         */
        std::pair<uint64_t, size_t> read(std::istream& input);
    }; // namespace vint
}; // namespace tide

/// @}
// group utilities

#endif // TIDE_VINT_H_

