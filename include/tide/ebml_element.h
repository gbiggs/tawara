/* TIDE
 *
 * Header file for the EBML element object.
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
     * This interface is very generic. It provides access to the element's ID
     * and required storage size, as well as child elements. Specialisations
     * are required to provide data access.
     */
    class TIDE_EXPORT EBMLElement
    {
        public:
            /** \brief Create a new Element.
             *
             * \param[in] id The element's ID, as an unsigned integer up to 28
             * bits.
             */
            EBMLElement(uint32_t id);

            /// \brief Copy constructor.
            EBMLElement(Element const& rhs);

            /// \brief Destructor.
            virtual ~EBMLElement();

    }; // class Element
}; // namespace tide

/// @}
/// group interfaces

#endif // TIDE_ELEMENT_H_

