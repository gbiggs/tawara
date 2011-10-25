/* TIDE
 *
 * Exceptions header file.
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

#if !defined(TIDE_EXCEPTIONS_H_)
#define TIDE_EXCEPTIONS_H_

#include <tide/win_dll.h>

#include <boost/exception/all.hpp>
#include <exception>

/// \addtogroup exceptions Exceptions
/// @{

namespace tide
{
    /// \brief Base error type.
    struct TideError : virtual std::exception, virtual boost::exception {};

//////////////////////////////////////////////////////////////////////////////
// Error types
//////////////////////////////////////////////////////////////////////////////

    /** \brief No name error.
     *
     * Encountered when passing an empty string as the name for a new Tide
     * instance.
     */
    struct NoName : virtual TideError {};

    /** \brief Object does not exist error.
     *
     * Usually encountered when attempting to create a new Tide instance in
     * read mode based on a name that is not in use (in other words, the file
     * does not exist).
     */
    struct NoObject : virtual TideError {};

//////////////////////////////////////////////////////////////////////////////
// Error information tags
//////////////////////////////////////////////////////////////////////////////

    /// \brief Name of the Tide object.
    typedef boost::error_info<struct tag_name, std::string> error_name;
}; // namespace tide

/// @}
// group exceptions

#endif // TIDE_EXCEPTIONS_H_

