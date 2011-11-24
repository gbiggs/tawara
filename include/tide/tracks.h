/* Tide
 *
 * Header for the TrackEntry element and related elements.
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

#if !defined(TIDE_TRACKS_H_)
#define TIDE_TRACKS_H_

#include <tide/master_element.h>
#include <tide/win_dll.h>

/// \addtogroup elements Elements
/// @{

namespace tide
{
    /** \brief The Tracks element, listing all tracks in the segment.
     *
     * The Tracks element contains the list of tracks that have data in the
     * segment. Each track is represented by a TrackEntry element, and each has
     * a number and a UID. The numbers and UIDs must be unique within the
     * segment, and UIDs should be as unique as possible.
     */
}; // namespace tide

/// @}
// group elements

#endif // TIDE_TRACKS_H_

