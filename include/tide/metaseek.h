/* Tide
 *
 * Header file for the Metaseek element.
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

#if !defined(TIDE_METASEEK_H_)
#define TIDE_METASEEK_H_

#include <tide/el_ids.h>
#include <tide/master_element.h>
#include <tide/seek_element.h>
#include <tide/win_dll.h>
#include <vector>

/// \addtogroup interfaces Interfaces
/// @{

namespace tide
{
    /** \brief The Metaseek element, used as an index for a Tide file.
     *
     * Generally, any Tide segment should contain one, and only one, instance
     * of a Metaseek element. This element should provide the byte offsets in
     * the data stream of the other Level 1 elements (SegmentInfo, Tracks,
     * etc.) in the segment. This function also means that the Metaseek element
     * should be placed first in its segment, so that it can be found quickly
     * to speed up file loading.
     *
     * When writing Tide files, it is advisable to reserve some space at the
     * start of the segment for the Metaseek element to be written in once the
     * file is complete.
     */
    class TIDE_EXPORT Metaseek : public MasterElement
    {
        public:
            /** Create a new Metaseek element.
             *
             * Upon creation, the element's index will be empty.
             */
            Metaseek();

            /// \brief Destructor.
            virtual ~Metaseek() {}

            /// \brief Get the size of the body of this element.
            virtual std::streamsize size() const;

            /** \brief An index item, as a mapping from an EBML element ID to
             * a stream offset.
             */
            typedef std::pair<ids::ID, std::streampos> IndexItem;

            /** \brief Append a new index item.
             *
             * \param[in] item The index item to append.
             */
            void append(IndexItem item);
            /** \brief Remove an index item.
             *
             * \param[in] pos The position in the index of the item to remove.
             * \return The removed IndexItem.
             */
            IndexItem remove(unsigned int pos);

            /** \brief Indexing operator.
             *
             * Gets the index item at the specified position.
             */
            IndexItem operator[](unsigned int pos);

            /// \brief Get the number of items in the index.
            unsigned int index_size() const { return index_.size(); }

            /// TODO: Proper iterator model for the index

            /// \brief Element body writing.
            virtual std::streamsize write_body(std::ostream& output);

            /// \brief Element body loading.
            virtual std::streamsize read_body(std::istream& input);

        protected:
            std::vector<SeekElement> index_;
    }; // class Metaseek
}; // namespace Tide

/// @}

#endif // TIDE_METASEEK_H_

