/* TIDE
 *
 * Header file for the unsigned integer element object.
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

#if !defined(TIDE_UINT_ELEMENT_H_)
#define TIDE_UINT_ELEMENT_H_

#include <tide/win_dll.h>

/// \addtogroup interfaces Interfaces
/// @{

namespace tide
{
    /** \brief The unsigned integer element interface.
     *
     * This element stores an unsigned integer. The maximum size of the value
     * is 2^64.
     */
    class TIDE_EXPORT UIntElement
    {
        public:
            /** \brief Create a new Element.
             *
             * \param[in] id The element's ID, as an unsigned integer up to 28
             * bits.
             */
            UIntElement(uint32_t id, uint64_t default);

            /// \brief Copy constructor.
            UIntElement(UIntElement const& rhs);

            /// \brief Destructor.
            virtual ~UIntElement();

            /// \brief Assignment operator.
            virtual UIntElement& operator=(UIntElement const& rhs);

        private:
            uint64_t value_;
            uint64_t default_;
    }; // class Element
}; // namespace tide

/// @}
/// group interfaces

#endif // TIDE_UINT_ELEMENT_H_

