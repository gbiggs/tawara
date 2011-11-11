/* Tide
 *
 * This file contains all the EBML element IDs used by Tide.
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

#if !defined(TIDE_EL_IDS_H_)
#define TIDE_EL_IDS_H_

#include <stdint.h>
#include <ios>

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
        typedef uint32_t ID;

        const ID Void(0x6C);
        const ID CRC32(0x3F);

        const ID EBML(0x0A45DFA3);
            const ID EBMLVersion(0x0286);
            const ID EBMLReadVersion(0x02F7);
            const ID EBMLMaxIDLength(0x02F2);
            const ID EBMLMaxSizeLength(0x02F3);
            const ID DocType(0x0282);
            const ID DocTypeVersion(0x0287);
            const ID DocTypeReadVersion(0x0285);

        const ID Segment(0x08538067);

            const ID SeekHead(0x014D9B74);
                const ID Seek(0x0DBB);
                const ID SeekID(0x13AB);
                const ID SeekPosition(0x13AC);

            const ID Info(0x0549A966);
                const ID SegmentUID(0x33A4);
                const ID SegmentFilename(0x3384);
                const ID PrevUID(0x1CB923);
                const ID PrevFilename(0x1C83AB);

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
        std::streamsize coded_size(uint64_t id);

        /** \brief Write an ID to an output stream.
         *
         * This function writes an ID to an output stream, using the value of
         * the ID to calculate the number of bytes required for storage.
         *
         * \param[in] ID The ID to write.
         * \param[in] output The std::ostream object to write to.
         * \return The number of bytes written.
         * \exception VarIntTooBig if the ID is above the maximum value
         * for variable-length integers (0xFFFFFFFFFFFFFF).
         * \exception WriteError if there is an error writing to the stream.
         */
        std::streamsize write(uint64_t integer, std::ostream& output);

        /** \brief The result of a read operation is a pair of the ID read
         * and the number of bytes read.
         */
        typedef std::pair<uint64_t, std::streamsize> ReadResult;

        /** \brief Read an ID from an input stream.
         *
         * This function reads an ID from an input stream, using the value of
         * the first byte to determine the length of the ID.
         *
         * \param[in] input The std::istream object to read bytes from.
         * \return A pair containing the ID read in the first and the number
         * of bytes read from the stream in the second.
         * \exception InvalidEBMLID if the ID is invalid.
         * \exception InvalidVarInt if the ID in the byte stream is unreadable.
         * \exception ReadError if there is an error reading the input stream.
         */
        ReadResult read(std::istream& input);
    }; // namespace ids
}; // namespace tide

/// @} // group ebml

#endif // TIDE_EL_IDS_H_

