/*
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2011, 2012, Geoffrey Biggs, geoffrey.biggs@aist.go.jp
 *     RT-Synthesis Research Group
 *     Intelligent Systems Research Institute,
 *     National Institute of Advanced Industrial Science and Technology (AIST),
 *     Japan
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Geoffrey Biggs nor AIST, nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#if !defined(ID_UTILS_H_)
#define ID_UTILS_H_

#include <tide/ids.h>

#include <ios>
#include <stdint.h>
#include <vector>

/// \addtogroup ebml EBML
/// @{

namespace tide
{
    /** \brief This namespace contains constants and functions for managing
     * EBML class IDs.
     *
     * The EBML class IDs used for the elements in a Tide document and
     * functions for reading/writing them to byte streams are contained in this
     * namespace.
     */
    namespace ids
    {
        /** \brief Get the number of bytes required by an ID.
         *
         * The size required by an ID depends on its value, and will range from
         * 1 to 8 bytes.
         *
         * \param[in] id The ID to get the size of.
         * \return The size, in bytes, that the ID will require for optimum
         * storage.
         * \exception InvalidEBMLID if the ID is invalid.
         */
        std::streamsize size(ID id);

        /** \brief Encode an unsigned integer into a buffer.
         *
         * Encodes an unsigned variable-length integer according to the EBML
         * specification. Leading zero bits are used to indicate the length of
         * the encoded integer in bytes.
         *
         * \param[in] integer The integer to encode.
         * \return A vector containing the encoded data.
         * \exception InvalidEBMLID if the ID is invalid.
         */
        std::vector<char> encode(ID integer);

        /** \brief The result of a decode operation is a pair of the ID
         * decoded and an iterator pointing to the first element after the used
         * data.
         */
        typedef std::pair<uint64_t, std::vector<char>::const_iterator>
            DecodeResult;

        /** \brief Decode an ID from a buffer.
         *
         * Decodes the ID stored in the buffer.
         *
         * \param[in] buffer The buffer holding the raw data.
         * \return The DecodeResult, containing the decoded ID
         * and an iterator pointing to the first element after the used data.
         * \exception InvalidVarInt if the first byte in the buffer is
         * zero, an invalid starting byte for a variable-length integer.
         * \exception BufferTooSmall if the expected encoded length of the
         * variable-length integer is larger than the available buffer length.
         * \exception InvalidEBMLID if the ID is invalid.
         */
        DecodeResult decode(std::vector<char> const& buffer);

        /** \brief Write an ID to a byte stream.
         *
         * This function writes an ID to a byte stream, using the value of
         * the ID to calculate the number of bytes required for storage.
         *
         * \param[in] id The ID to write.
         * \param[in] io The byte stream to write to.
         * \return The number of bytes written.
         * \exception InvalidEBMLID if the ID is invalid.
         * \exception WriteError if there is an error writing to the stream.
         */
        std::streamsize write(ID id, std::iostream& io);

        /** \brief The result of a read operation is a pair of the ID read
         * and the number of bytes read.
         */
        typedef std::pair<ID, std::streamsize> ReadResult;

        /** \brief Read an ID from a byte stream.
         *
         * This function reads an ID from a byte stream, using the value of
         * the first byte to determine the length of the ID.
         *
         * \param[in] io The byte stream to read bytes from.
         * \return A pair containing the ID read in the first and the number
         * of bytes read from the stream in the second.
         * \exception InvalidEBMLID if the ID is invalid.
         * \exception InvalidVarInt if the ID in the byte stream is unreadable.
         * \exception ReadError if there is an error reading the byte stream.
         */
        ReadResult read(std::iostream& io);
    }; // namespace ids
}; // namespace tide

/// @} // group ebml

#endif // ID_UTILS_H_

