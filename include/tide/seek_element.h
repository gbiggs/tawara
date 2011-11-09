/* Tide
 *
 * Header file for the Seek element, a child of SeekHead.
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

#if !defined(TIDE_SEEK_ELEMENT_H_)
#define TIDE_SEEK_ELEMENT_H_

#include <el_ids.h>
#include <tide/master_element.h>
#include <tide/win_dll.h>

/// \addtogroup interfaces Interfaces
/// @{

namespace tide
{
    /** \brief The Seek element, which contains a single index point in the
     * SeekHead element.
     *
     * The SeekHead (Metaseek) element is used as an index into the file's data
     * stream, giving the byte offsets of other level 1 elements. A Seek
     * element is a single index entry in the Metaseek. It stores the element
     * ID and the byte offset of the element.
     */
    class TIDE_EXPORT SeekElement : public MasterElement
    {
        public:
            /** Create a new Seek element.
             *
             * \param[in] id The element ID to index.
             * \param[in] offset The position in the bytestream from the start
             * of the segment to the element's ID. 0 for the first element in
             * the segment.
             */
            SeekElement(ids::ID id, std::streamoffset offset);

            /// \brief Destructor.
            virtual ~SeekElement() {}

            /// \brief Get the ID that is indexed by this Seek element.
            ids::ID id() const { return indexed_id_.value(); }
            /// \brief Set the ID that is indexed.
            void id(ids::ID id) { indexed_id_.value(id); }

            /// \brief Get the offset of the indexed ID.
            std::streamsize offset() const { return offset_.value(); }
            /// \brief Set the offset of the indexed ID.
            void offset(std::streamsize offset) { offset_.value(offset); }

            /// \brief Get the size of the body of this element.
            virtual size_t size() const;

            /// \brief Element body writing.
            virtual std::streamsize write_body(std::ostream& output);

            /// \brief Element body loading.
            virtual std::streamsize read_body(std::istream& input);

        protected:
            BinaryElement indexed_id_;
            UIntElement offset_;
    }; // class SeekElement
}; // namespace tide

/// @}

#endif // TIDE_SEEK_ELEMENT_H_

