/* Tide
 *
 * Header file for the implementation of the Tide interface.
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

#if !defined(TIDE_TIDE_IMPL_H_)
#define TIDE_TIDE_IMPL_H_

#include <tide/tide.h>
#include <tide/win_dll.h>

#include <fstream>
#include <ostream>
#include <string>

/// \addtogroup implementations Implementations
/// @{

namespace tide
{
    /** \brief Implementation of the Tide interface.
     *
     * See tide::Tide for method documentation.
     */
    class TIDE_EXPORT TideImpl : public Tide
    {
        public:
            /// \brief Create a new Tide implementation object.
            TideImpl(std::stream& stream);

            /// \brief Destructor for the Tide implementation object.
            virtual ~TideImpl() {};

        protected:
            /// The stream being operated on.
            std::stream file_;

            /// Prepares the stream for use by checking for EBML content, and
            /// adding a header if the stream is empty.
            bool prepare_stream();
    }; // class TideFile
}; // namespace tide

/// @}
// group implementations

#endif // TIDE_TIDE_IMPL_H_

