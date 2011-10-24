/* TIDE
 *
 * Header file for a null-output stream using Boost.Iostreams.
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

#if !defined(TIDE_NULL_STREAM_H_)
#define TIDE_NULL_STREAM_H_

#include <boost/iostreams/stream.hpp>

/// \addtogroup utilities Utilities
/// @{

namespace tide
{
    /** \brief A null implementation of the std::ostream interface.
     *
     * This std::ostream implementation ignores every write, effectively
     * discarding the data into the ether. Use it for the verbose output of a
     * Tide object to disable verbose information output.
     */
    typedef boost::iostreams::stream<boost::iostreams::basic_null_device<char,
            boost::iostreams::output> >
                NullOStream;
}; // namespace tide

/// @}
// group utilities

#endif // TIDE_NULL_STREAM_H_

