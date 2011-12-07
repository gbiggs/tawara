/* Tide
 *
 * Header file for the SeekHead element.
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

#include <boost/operators.hpp>
#include <map>
#include <tide/el_ids.h>
#include <tide/master_element.h>
#include <tide/seek_element.h>
#include <tide/win_dll.h>

/// \addtogroup elements Elements
/// @{

namespace tide
{
    /** \brief The SeekHead element, used as an index for a Tide file.
     *
     * Generally, any Tide segment should contain one, and only one, instance
     * of a SeekHead element. This element should provide the byte offsets in
     * the data stream of the other Level 1 elements (SegmentInfo, Tracks,
     * etc.) in the segment. This function also means that the SeekHead element
     * should be placed first in its segment, so that it can be found quickly
     * to speed up file loading.
     *
     * When writing Tide files, it is advisable to reserve some space at the
     * start of the segment for the SeekHead element to be written in once the
     * file is complete.
     */
    class TIDE_EXPORT SeekHead : public MasterElement,
            public boost::equality_comparable<SeekHead>
    {
        public:
            /// \brief The key type (Key) of this container.
            typedef ids::ID key_type;
            /// \brief The mapped type (T) of this container.
            typedef std::streamoff mapped_type;

        protected:
            /// \brief The type of the internal storage.
            typedef std::multimap<key_type, mapped_type> storage_type_;

        public:
            /** Create a new SeekHead element.
             *
             * Upon creation, the element's index will be empty.
             */
            SeekHead();

            /// \brief The value type of this container.
            typedef storage_type_::value_type value_type;
            /// \brief The size type of this container.
            typedef storage_type_::size_type size_type;
            /// \brief The reference type.
            typedef storage_type_::reference reference;
            /// \brief The constant reference type.
            typedef storage_type_::const_reference const_reference;
            /// \brief The random access iterator type.
            typedef storage_type_::iterator iterator;
            /// \brief The constant random access iterator type.
            typedef storage_type_::const_iterator const_iterator;
            /// \brief The reversed random access iterator type.
            typedef storage_type_::reverse_iterator reverse_iterator;
            /// \brief The constant reversed random access iterator type.
            typedef storage_type_::const_reverse_iterator
                const_reverse_iterator;

            /** \brief Replace the stored offsets with those from another
             * SeekHead element.
             */
            SeekHead& operator=(SeekHead const& other)
                { index_ = other.index_; return *this; }

            /// \brief Get an iterator to the first index entry.
            iterator begin() { return index_.begin(); }
            /// \brief Get an iterator to the first index entry.
            const_iterator begin() const { return index_.begin(); }
            /** \brief Get an iterator to the position past the last index
             * entry.
             */
            iterator end() { return index_.end(); }
            /** \brief Get an iterator to the position past the last index
             * entry.
             */
            const_iterator end() const { return index_.end(); }
            /// \brief Get a reverse iterator to the last index entry.
            reverse_iterator rbegin() { return index_.rbegin(); }
            /// \brief Get a reverse iterator to the last index entry.
            const_reverse_iterator rbegin() const { return index_.rbegin(); }
            /** \brief Get a reverse iterator to the position before the first
             * index entry.
             */
            reverse_iterator rend() { return index_.rend(); }
            /** \brief Get a reverse iterator to the position before the first
             * index entry.
             */
            const_reverse_iterator rend() const { return index_.rend(); }

            /// \brief Check if there are no index entries.
            bool empty() const { return index_.empty(); }
            /// \brief Get the number of index entries.
            size_type count() const { return index_.size(); }
            /// \brief Get the maximum number of index entries.
            size_type max_count() const { return index_.max_size(); }

            /// \brief Remove all index entries.
            void clear() { index_.clear(); }
            /** \brief Insert a new index entry.
             *
             * If an index entry already exists with the same ID, the new
             * offset is entered into the index after it. No index entries are
             * overwritten.
             *
             * \param[in] value The offset to insert and its ID.
             *
             * \return The iterator at the position where the offset was added.
             */
            iterator insert(value_type const& value)
                { return index_.insert(value); }
            /** \brief Insert a range of offsets.
             *
             * \param[in] first The start of the range.
             * \param[in] last The end of the range.
             */
            void insert(const_iterator first, const_iterator last)
                { index_.insert(first, last); }
            /** \brief Erase the index entry at the specified iterator.
             *
             * \param[in] position The position to erase at.
             */
            void erase(iterator position) { index_.erase(position); }
            /** \brief Erase a range of index entries.
             *
             * \param[in] first The start of the range.
             * \param[in] last The end of the range.
             */
            void erase(iterator first, iterator last)
                { index_.erase(first, last); }
            /** \brief Erase all index entries with the given ID.
             *
             * \param[in] id The ID to erase.
             * \return The ID of entries erased.
             */
            size_type erase(key_type const& id)
                { return index_.erase(id); }
            /** \brief Swaps the contents of this SeekHead with another.
             *
             * \param[in] other The other SeekHead to swap with.
             */
            void swap(SeekHead& other)
                { index_.swap(other.index_); }

            /** \brief Search for the index entry with the given ID.
             *
             * If multiple offsets exist for the ID, the first one inserted
             * will be returned.
             *
             * \param[in] id The ID to search for.
             * \return An iterator to the matching offset, or end() if
             * there is no index entry with that ID.
             */
            iterator find(key_type const& id) { return index_.find(id); }
            /** \brief Search for the index entry with the given ID.
             *
             * \param[in] id The ID to search for.
             * \return An iterator to the matching offset, or end() if
             * there is no index entry with that ID.
             */
            const_iterator find(key_type const& id) const
                { return index_.find(id); }

            /// \brief Equality operator.
            friend bool operator==(SeekHead const& lhs, SeekHead const& rhs);

        protected:
            storage_type_ index_;

            /// \brief Get the size of the body of this element.
            virtual std::streamsize body_size() const;

            /// \brief Element body writing.
            virtual std::streamsize write_body(std::ostream& output);

            /// \brief Element body loading.
            virtual std::streamsize read_body(std::istream& input,
                    std::streamsize size);
    }; // class SeekHead

    /// \brief Equality operator.
    bool operator==(SeekHead const& lhs, SeekHead const& rhs);
}; // namespace Tide

/// @}
// group elements

#endif // TIDE_METASEEK_H_

