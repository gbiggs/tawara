/* TIDE
 *
 * Header file for the TIDE object interface.
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

#if !defined(TIDE_TIDE_H_)
#define TIDE_TIDE_H_

#include <tide/null_stream.h>
#include <tide/win_dll.h>

#include <boost/utility.hpp>
#include <string>

/// \addtogroup interface Interface
/// @{

namespace tide
{
    /// \brief Access modes.
    enum MODE
    {
        /// Read-only access.
        MODE_READ,
        /// Write-only access, truncating.
        MODE_WRITE,
        /// Write-only access, appending.
        MODE_APPEND
    };


    /** \brief The Tide interface, the interface provided by all Tide objects.
     *
     * This class defines the Tide interface. All implementations must implement
     * these methods.  A Tide implementation may use whatever it desires to
     * provide the functionality. Two commonly-used implementations are a file
     * store and a memory store.
     */
    class TIDE_EXPORT Tide : private boost::noncopyable
    {
        public:
            /** \brief Create a new Tide object.
             *
             * The verbose output will be sent to std::clog.
             *
             * \param[in] name The name of the object. Usage is
             * implementation-dependent. If \ref MODE_WRITE is passed for mode
             * and the name is in use, the previous entity using that name will
             * be destroyed. The definition of "in use" and "destroyed" varies
             * by implementation.
             * \param[in] mode The open mode, one of the possible values
             * defined by \ref MODE.
             */
            Tide(std::string name, MODE mode) {};

            /** \brief Create a new Tide object.
             *
             * \param[in] name The name of the object. Usage is
             * implementation-dependent. If \ref MODE_WRITE is passed for mode
             * and the name is in use, the previous entity using that name will
             * be destroyed. The definition of "in use" and "destroyed" varies
             * by implementation.
             * \param[in] mode The open mode, one of the possible values
             * defined by \ref MODE.
             * \param[in] verb_out Output ostream for the verbose output. Pass
             * in a null stream (e.g. a stream constructed using
             * boost::iostreams::basic_null_device, which is the default) to
             * disable verbose output.
             */
            Tide(std::string name, MODE mode, std::ostream& verb_out) {};

            /** \brief Destroy the Tide object.
             *
             * All resources used by the object will be cleaned up when its
             * destructor is called. Any other objects, such as Channel or Tag
             * objects, referencing this Tide object will become invalid and
             * should not be used. Behaviour if they are used is undefined.
             */
            virtual ~Tide() {};
    }; // class Tide
}; // namespace tide

/// @}
// group interface

#endif // TIDE_TIDE_H_

