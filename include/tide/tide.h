/* Tide
 *
 * Header file for the Tide object interface.
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

#if !defined(TIDE_TIDE_H_)
#define TIDE_TIDE_H_

#include <tide/win_dll.h>

#include <boost/utility.hpp>
#include <iostream>
#include <stdint.h>

/// \addtogroup interfaces Interfaces
/// @{

namespace tide
{
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
             * This constructor makes a new Tide object around an input/output
             * stream.
             *
             * The stream will be checked for its current contents. If it is
             * empty (i.e. the size of its contents is zero), then it will be
             * initialised as a new EBML file. Otherwise, it will be treated as
             * an existing EBML file and read. In this case, if an EBML header
             * cannot be found or the DocType is incorrect, NotTide will be
             * raised.
             *
             * EBML supports placing text before the EBML header. Anything up
             * to the first 0x1A byte is ignored. This means you can place data
             * into the stream before passing it to Tide::Tide(). Tide will
             * treat the current position in the stream when this constructor
             * is called as the beginning of the stream.
             *
             * \param[in] stream A reference to the std::iostream object to
             * read from and write to.
             * \exception NotEBML if the stream is not empty and does not
             * contain an EBML header.
             * \exception NotTide if the stream is not empty and does not
             * contain valid EBML with the "tide" DocType.
             */
            Tide(std::iostream& stream) {}

            /** \brief Destroy the Tide object.
             *
             * All resources used by the object will be cleaned up when its
             * destructor is called. Any other objects, such as Element
             * objects, referencing this Tide object will become invalid and
             * should not be used. Behaviour if they are used is undefined.
             *
             * The stream passed to the constructor is \e not closed.
             */
            virtual ~Tide() {};
    }; // class Tide
}; // namespace tide

/// @}
// group interfaces

#endif // TIDE_TIDE_H_

