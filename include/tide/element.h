/* TIDE
 *
 * Header file for the base element object.
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

#if !defined(TIDE_ELEMENT_H_)
#define TIDE_ELEMENT_H_

#include <tide/win_dll.h>

#include <ios>
#include <iostream>
#include <stdint.h>


/// \addtogroup interfaces Interfaces
/// @{

namespace tide
{
    /** \brief The Element interface, a basic interface to an element object.
     *
     * Tide objects store their data in elements. Like in XML, the elements
     * form a tree of information. Each element contains a single value and
     * zero or more sub-elements. Each element has a unique ID within the
     * format.
     *
     * This interface provides the most basic element facilities. It provides
     * the element's ID and an abstract interface to read and write elements to
     * a byte stream.
     */
    class TIDE_EXPORT Element
    {
        public:
            /** \brief Create a new Element.
             *
             * \param[in] id The element's ID, as an unsigned integer up to 28
             * bits.
             */
            Element(uint32_t id);

            /// \brief Destructor.
            virtual ~Element() {};

            /** Get the element's ID.
             */
            virtual uint32_t id() const { return id_; }
            /** \brief Set the element's ID.
             *
             * The element's ID is an unsigned integer with a maximum size of
             * 28 bits. Some IDs are invalid:
             * - 0
             * - 0xFF
             * - 0xFFFF
             * - 0xFFFFFF
             * - 0xFFFFFFFF
             * - 0x100000000 or greater
             *
             * IDs are divided into four classes:
             * - Class A (0 to 2^7 - 1)
             * - Class B (2^7 to 2^14 - 1)
             * - Class C (2^14 to 2^21 - 1)
             * - Class D (2^21 to 2^28 - 1)
             *
             * In some storage types, such as EBML file storage, the higher
             * classes use more storage space and so are more likely to be
             * unique within the file. This allows them to be used for
             * synchronisation in the event of file corruption. To take
             * advantage of this, you should use higher IDs for elements that
             * occur less frequently, such as the top-level elements.
             *
             * \param[in] id The element's new ID, as an unsigned integer up to
             * 28 bits.
             */
            virtual void id(uint32_t id);

            /** \brief Element writing.
             *
             * Writes the entire element, including its ID, body size and body
             * data, to a byte stream providing a std::ostream interface.
             *
             * \param[in] output The destination byte stream to write to.
             * \return The number of bytes written.
             */
            virtual std::streamsize write(std::ostream& output);

            /** \brief Element ID writing.
             *
             * Writes the element's EBML ID to a byte stream providing a
             * std::ostream interface. Up to 4 bytes may be written.
             *
             * \param[in] output The destination byte stream to write to.
             * \return The number of bytes written.
             */
            virtual std::streamsize write_id(std::ostream& output) = 0;

            /** \brief Element body writing.
             *
             * Writes the element's size and body to a byte stream providing a
             * std::ostream interface.
             *
             * \return The number of bytes written.
             */
            virtual std::streamsize write_body(std::ostream& output) = 0;

            /** \brief Element body loading.
             *
             * Reads the element from a byte stream providing a std::istream
             * interface.
             *
             * This method assumes that the Element ID has already been read
             * (and thus used to construct the Element instance doing the
             * reading), which means that the file's read pointer should be
             * positioned at the first byte of the element's size.
             *
             * \return The number of bytes read.
             */
            virtual std::streamsize read_body(std::istream& input) = 0;
        protected:
            uint32_t id_;
    }; // class Element
}; // namespace tide

/// @}
/// group interfaces

#endif // TIDE_ELEMENT_H_

