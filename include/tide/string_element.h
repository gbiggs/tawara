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
            StringElement(uint32_t id, std::string value)
                : PrimitiveElement<std::string>(id, value)
            {
            }

            /** \brief Create a new string element with a default value.
             *
             * \param[in] id The element's ID, as an unsigned integer up to 28
             * bits.
             * \param[in] value The element's value.
             * \param[in] default_value The default value of the element.
             */
            StringElement(uint32_t id, std::string value, std::string default_value)
                : PrimitiveElement<std::string>(id, value, default_value)
            {
            }

            /** \brief Element storage.
             *
             * Writes the element to a byte store providing a std::ostream
             * interface.
             *
             * \param[in] output The destination byte stream to write to.
             * \return The number of bytes written.
             */
            virtual std::streamsize write(std::ostream& output);

            /** \brief Element loading.
             *
             * Reads the element from a byte store providing a std::ostream
             * interface.
             *
             * This method assumes that the Element ID has already been read
             * (and thus used to construct the Element instance doing the
             * reading), which means that the file's read pointer should be
             * positioned at the first byte of the element's size.
             *
             * \param[in] input The source byte stream to read from.
             * \return The number of bytes read.
             */
            virtual std::streamsize read(std::istream& input);
    }; // class StringElement
}; // namespace tide

/// @}
// group implementations

#endif // TIDE_STRING_ELEMENT_H_

