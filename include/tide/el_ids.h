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

namespace tide
{
    namespace ids
    {
        const uint32_t EBML(0x0A45DFA3);
            const uint32_t EBMLVersion(0x0286);
            const uint32_t EBMLReadVersion(0x02F7);
            const uint32_t EBMLMaxIDLength(0x02F2);
            const uint32_t EBMLMaxSizeLength(0x02F3);
            const uint32_t DocType(0x0282);
            const uint32_t DocTypeVersion(0x0287);
            const uint32_t DocTypeReadVersion(0x0285);
    }; // namespace ids
}; // namespace tide

#endif // TIDE_EL_IDS_H_

