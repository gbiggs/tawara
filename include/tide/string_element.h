/* TIDE
 *
 * Header file for the string element object.
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

#if !defined(TIDE_STRING_ELEMENT_H_)
#define TIDE_STRING_ELEMENT_H_

#include <tide/prim_element.h>
#include <tide/win_dll.h>

#include <string>

/// \addtogroup implementations Implementations
/// @{

namespace tide
{
    /** String primitive element.
     *
     * This element stores a UTF-8 string. Upon writing to a store, the string
     * may or may not be padded with null bytes.
     */
    class TIDE_EXPORT StringElement : public PrimitiveElement<std::string>
    {
        public:
            /** \brief Create a new string element with no default.
             *
             * \param[in] id The element's ID, as an unsigned integer up to 28
             * bits.
             * \param[in] value The element's value.
             */
            StringElement(uint32_t id, std::string value);

            /** \brief Create a new string element with a default value.
             *
             * \param[in] id The element's ID, as an unsigned integer up to 28
             * bits.
             * \param[in] value The element's value.
             * \param[in] default_value The default value of the element.
             */
            StringElement(uint32_t id, std::string value,
                    std::string default_value);

            /// \brief Value assignment operator.
            virtual StringElement& operator=(std::string const& rhs);

            /// \brief Get the amount of padding used.
            virtual uint64_t padding() const { return padding_; }
            /** \brief Set the amount of padding to use.
             *
             * Strings can be zero-padded at the end. This is particularly
             * useful when overwriting an existing string with one that is
             * shorter, so that the file does not need to be rewritten or a
             * void element used.
             */
            virtual void padding(uint64_t padding) { padding_ = padding; }

            /** \brief Element ID writing.
             *
             * Writes the element's EBML ID to a byte stream providing a
             * std::ostream interface. Up to 4 bytes may be written.
             *
             * \param[in] output The destination byte stream to write to.
             * \return The number of bytes written.
             * \exception WriteError if an error occurs writing data.
             */
            virtual std::streamsize write_id(std::ostream& output);

            /** \brief Element body writing.
             *
             * Writes the element's size and body to a byte stream providing a
             * std::ostream interface.
             *
             * \return The number of bytes written.
             * \exception WriteError if an error occurs writing data.
             */
            virtual std::streamsize write_body(std::ostream& output);

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
             * \exception ReadError if an error occurs reading data.
             */
            virtual std::streamsize read_body(std::istream& input);

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

        protected:
            uint64_t padding_;
    }; // class StringElement
}; // namespace tide

/// @}
// group implementations

#endif // TIDE_STRING_ELEMENT_H_

