/* Tide
 *
 * Header for the Segment element.
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

#if !defined(TIDE_SEGMENT_H_)
#define TIDE_SEGMENT_H_

#include <tide/win_dll.h>

/// \addtogroup elements Elements
/// @{

namespace tide
{
    /** \brief 
     */
    class TIDE_EXPORT Segment : public MasterElement
    {
        public:
            /// \brief Create a new segment element.
            Segment();

            /// \brief Destructor.
            virtual ~Segment() {}

            /// \brief Element body writing.
            virtual std::streamsize write_body(std::ostream& output);

        protected:
            /// \brief Get the size of the body of this element.
            virtual std::streamsize size() const;

            /// \brief Element body loading.
            virtual std::streamsize read_body(std::istream& input,
                    std::streamsize size);
    }; // class Segment
}; // namespace tide

/// @}

#endif // TIDE_SEGMENT_H_


