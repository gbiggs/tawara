/* Tide
 *
 * Header file for the base master element object.
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

#if !defined(TIDE_MASTER_ELEMENT_H_)
#define TIDE_MASTER_ELEMENT_H_

#include <tide/element.h>
#include <tide/win_dll.h>

/// \addtogroup interfaces Interfaces
/// @{

namespace tide
{
    /** \brief The MasterElement interface.
     *
     * EBML elements can be divided into primitive elements and master
     * elements. While primitive elements directly store a single piece of data
     * in a format corresponding to their type, master elements are used to
     * group other elements together. Instead of a single data value, they
     * store zero or more child elements, which can themselves be master or
     * primitive elements.
     *
     * Generally, each master element will have quite specific functionality.
     * In such cases, a new class should be created that inherits from and
     * implements this interface.
     */
    class TIDE_EXPORT MasterElement : public Element
    {
        public:
            /** \brief Create a new MasterElement.
             *
             * \param[in] id The element's ID, as an unsigned integer up to 28
             * bits.
             * \param[in] crc Use a CRC32 element to provide a check for file
             * corruption.
             */
            MasterElement(uint32_t id, bool crc=false);

            /// \brief Destructor
            virtual ~MasterElement() {};

        private:
            bool crc_;
    }; // class MasterElement
}; // namespace tide

/// @}
// group interfaces

#endif // TIDE_MASTER_ELEMENT_H_

