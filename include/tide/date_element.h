/* TIDE
 *
 * Header file for the date element object.
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

#if !defined(TIDE_DATE_ELEMENT_H_)
#define TIDE_DATE_ELEMENT_H_

#include <tide/prim_element.h>
#include <tide/win_dll.h>

#include <stdint.h>

/// \addtogroup implementations Implementations
/// @{

namespace tide
{
    /** Date primitive element.
     *
     * This element stores a date. A date is represented as a signed, 64-bit
     * integer giving the number of nanoseconds since 2001-01-01 00:00:00.
     */
    class TIDE_EXPORT DateElement : public PrimitiveElement<int64_t>
    {
        public:
            /** \brief Create a new date element with no default.
             *
             * \param[in] id The element's ID, as an unsigned integer up to 28
             * bits.
             * \param[in] value The element's value.
             */
            DateElement(uint32_t id, int64_t value);

            /** \brief Create a new date element with a default value.
             *
             * \param[in] id The element's ID, as an unsigned integer up to 28
             * bits.
             * \param[in] value The element's value.
             * \param[in] default_value The default value of the element.
             */
            DateElement(uint32_t id, int64_t value, int64_t default_value);

            /// \brief Value assignment operator.
            virtual DateElement& operator=(int64_t const& rhs);

            /** \brief Element ID writing.
             *
             * Writes the element's EBML ID to a byte stream providing a
             * std::basic_ostream<uint8_t> interface. Up to 4 bytes may be
             * written.
             *
             * \param[in] output The destination byte stream to write to.
             * \return The number of bytes written.
             * \exception WriteError if an error occurs writing data.
             */
            virtual std::streamsize write_id(
                    std::basic_ostream<uint8_t>& output);

            /** \brief Element body writing.
             *
             * Writes the element's size and body to a byte stream providing a
             * std::basic_ostream<uint8_t> interface.
             *
             * \return The number of bytes written.
             * \exception WriteError if an error occurs writing data.
             */
            virtual std::streamsize write_body(
                    std::basic_ostream<uint8_t>& output);

            /** \brief Element body loading.
             *
             * Reads the element from a byte stream providing a
             * std::basic_ostream<uint8_t> interface.
             *
             * This method assumes that the Element ID has already been read
             * (and thus used to construct the Element instance doing the
             * reading), which means that the file's read pointer should be
             * positioned at the first byte of the element's size.
             *
             * \return The number of bytes read.
             * \exception BadElementLength if the date element is an incorrect
             * length (i.e. not 8 bytes).
             * \exception ReadError if an error occurs reading data.
             */
            virtual std::streamsize read_body(
                    std::basic_istream<uint8_t>& input);

            /** \brief Get the size of the body of this element.
             *
             * Returns the size, in bytes, required to store this element's
             * body. This does not include the space required by the ID or the
             * data size value.
             *
             * See also total_size().
             *
             * \return The size of the element's body, in bytes.
             */
            virtual size_t size() const;

            /** \brief Get the total size of the element.
             *
             * Returns the size, in bytes, required to store this entire
             * element, including its ID, data size value and body.
             *
             * \return The size of the entire element, in bytes.
             */
            virtual size_t total_size() const;
    }; // class DateElement
}; // namespace tide

/// @}
// group implementations

#endif // TIDE_DATE_ELEMENT_H_

