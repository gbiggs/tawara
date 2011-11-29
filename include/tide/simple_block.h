/* Tide
 *
 * Header for the SimpleBlock element.
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

#if !defined(TIDE_SIMPLE_BLOCK_H_)
#define TIDE_SIMPLE_BLOCK_H_

#include <tide/element.h>
#include <tide/win_dll.h>

/// \addtogroup elements Elements
/// @{

namespace tide
{
    /** \brief The SimpleBlock element, a simplified version of Blocks.
     *
     * The SimpleBlock element is used to store a block of data without any
     * extra information, such as reference frames or private codec data. The
     * block is stored as a binary element, with the internal format of the
     * binary data specified by the Simple Block format, which this element
     * implementation interprets.
     */
    class TIDE_EXPORT SimpleBlock : virtual Element, virtual BlockBase
    {
        public:
    }; // class SimpleBlock
}; // namespace tide

/// @}
// group elements

#endif // TIDE_SIMPLE_BLOCK_H_

