/* Tide
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

#if !defined(TIDE_VINT_H_)
#define TIDE_VINT_H_

#include <cstddef>
#include <istream>
#include <ostream>
#include <stdint.h>
#include <utility>
#include <vector>

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
        std::streamsize coded_size(uint64_t integer);

        /** \brief Encode an unsigned integer into a buffer.
         *
         * Encodes an unsigned variable-length integer according to the EBML
         * specification. Leading zero bits are used to indicate the length of
         * the encoded integer in bytes.
         *
         * The vector provided as a buffer must already have enough space to
         * store the encoded data reserved. This can be done by either
         * reserving the maximum possible size (8 bytes) or by using
         * coded_size() to find the required size.
         *
         * \param[in] integer The integer to encode.
         * \param[in] req_size If not zero, then use this length when encoding
         * the integer instead of the optimal size. Must be equal to or larger
         * than the optimal size.
         * \return A vector containing the encoded data.
         * \exception VarIntTooBig if the integer is above the maximum value
         * for variable-length integers (0xFFFFFFFFFFFFFF).
         * \exception SpecSizeTooSmall if the integer is too big for the
         * requested size.
         */
        std::vector<char> encode(uint64_t integer, std::streamsize req_size=0);

        /** \brief The result of a decode operation is a pair of the integer
         * decoded and an iterator pointing to the first element after the used
         * data.
         */
        typedef std::pair<uint64_t, std::vector<char>::const_iterator> DecodeResult;

        /** \brief Decode an unsigned variable-length integer from a buffer.
         *
         * Decodes the variable-length integer stored in the buffer.
         *
         * \param[in] buffer The buffer holding the raw data.
         * \return The DecodeResult, containing the decoded integer.
         * and the number of bytes used from the buffer in the second.
         * \exception InvalidVarInt if the first byte in the buffer is
         * zero, an invalid starting byte for a variable-length integer.
         * \exception BufferTooSmall if the expected encoded length of the
         * variable-length integer is larger than the available buffer length.
         */
        DecodeResult decode(std::vector<char> const& buffer);

        /** \brief Encode an unsigned integer and write it to an output stream.
         *
         * This function performs the same task as tide::vint::encode(), but it
         * writes the result to a std::ostream instead of a
         * simple buffer.
         *
         * \param[in] integer The integer to encode.
         * \param[in] output The std::ostream object to write
         * the encoded integer to.
         * \param[in] req_size If not zero, then use this length when encoding
         * the integer instead of the optimal size. Must be equal to or larger
         * than the optimal size.
         * \return The number of bytes written.
         * \exception VarIntTooBig if the integer is above the maximum value
         * for variable-length integers (0xFFFFFFFFFFFFFF).
         * \exception WriteError if there is an error writing to the stream.
         * \exception SpecSizeTooSmall if the integer is too big for the
         * requested size.
         */
        std::streamsize write(uint64_t integer, std::ostream& output,
                std::streamsize req_size=0);

        /** \brief The result of a read operation is a pair of the integer read
         * and the number of bytes read.
         */
        typedef std::pair<uint64_t, std::streamsize> ReadResult;

        /** \brief Decode an unsigned integer from an input stream.
         *
         * This function performs the same task as tide::vint::decode(), but it
         * reads the bytes from the input stream rather than a simple buffer.
         *
         * \param[in] input The std::istream object to read
         * bytes from.
         * \return A pair containing the value read in the first and the number
         * of bytes read from the stream in the second.
         * \exception InvalidVarInt if the variable-length integer in the byte
         * stream is invalid.
         * \exception ReadError if there is an error reading the input stream.
         */
        ReadResult read(std::istream& input);
    }; // namespace vint
}; // namespace tide

/// @}
// group utilities

#endif // TIDE_VINT_H_

