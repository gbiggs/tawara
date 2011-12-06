/* Tide
 *
 * Header for the BlockElement base class.
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

#if !defined(TIDE_BLOCK_ELEMENT_H_)
#define TIDE_BLOCK_ELEMENT_H_

#include <boost/shared_ptr.hpp>
#include <tide/block.h>
#include <tide/element.h>

namespace tide
{
    class TIDE_EXPORT BlockElement : public Element, public Block
    {
        public:
            BlockElement(tide::ids::ID id, uint64_t track_number,
                    int16_t timecode, LacingType lacing=LACING_NONE)
                : Element(id), Block(track_number, timecode, lacing)
            {
            }

            typedef boost::shared_ptr<BlockElement> Ptr;
    }; // class BlockElement
}; // namespace tide

#endif // TIDE_BLOCK_ELEMENT_H_

