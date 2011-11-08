/* Tide
 *
 * Header file for the binary element object.
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

#if !defined(TIDE_BINARY_ELEMENT_H_)
#define TIDE_BINARY_ELEMENT_H_

#include <tide/prim_element.h>
#include <tide/win_dll.h>

#include <stdint.h>
#include <vector>

/// \addtogroup implementations Implementations
/// @{

namespace tide
{
    /** Binary primitive element.
     *
     * This element stores a block of opaque binary data.
     */
    class TIDE_EXPORT BinaryElement : public PrimitiveElement<std::vector<char> >
    {
        public:
            /** \brief Create a new binary element with no default.
             *
             * \param[in] id The element's ID, as an unsigned integer up to 28
             * bits.
             * \param[in] value The element's value.
             */
            BinaryElement(uint32_t id, std::vector<char> value);

            /** \brief Create a new binary element with a default value.
             *
             * \param[in] id The element's ID, as an unsigned integer up to 28
             * bits.
             * \param[in] value The element's value.
             * \param[in] default_value The default value of the element.
             */
            BinaryElement(uint32_t id, std::vector<char> value,
                    std::vector<char> default_value);

            /// \brief Value assignment operator.
            virtual BinaryElement& operator=(std::vector<char> const& rhs);

            /// \brief Get the size of the body of this element.
            virtual size_t size() const;

            /// \brief Element body writing.
            virtual std::streamsize write_body(std::ostream& output);

            /// \brief Element body loading.
            virtual std::streamsize read_body(std::istream& input);
    }; // class BinaryElement
}; // namespace tide

/// @}
// group implementations

#endif // TIDE_BINARY_ELEMENT_H_

