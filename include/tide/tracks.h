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

#include <map>
#include <tide/master_element.h>
#include <tide/uint_element.h>
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
    class TIDE_EXPORT Tracks : public MasterElement
    {
        public:
            /** \brief Construct a new Tracks element.
             *
             * Upon construction, the list of tracks will be empty. At least
             * one TrackEntry must be added before writing the element.
             */
            Tracks();

            /// \brief Destructor.
            virtual ~Tracks();

            /// \brief Get the size of the body of this element.
            virtual std::streamsize size() const;

            /// \brief Element body writing.
            virtual std::streamsize write_body(std::ostream& output);

            /// \brief Element body loading.
            virtual std::streamsize read_body(std::istream& input);

            /** \brief Replace the stored TrackEntries with those from another
             * Tracks element.
             */
            Tracks& operator=(Tracks const& other);

            /** \brief Get the TrackEntry with the given track number.
             *
             * \return A reference to the specified TrackEntry.
             * \throw std::out_of_range if the track number is invalid.
             */
            mapped_type& at(key_type const& pos);
            /** \brief Get the TrackEntry with the given track number.
             *
             * \return A reference to the specified TrackEntry.
             * \throw std::out_of_range if the track number is invalid.
             */
            mapped_type const& at(key_type const& pos) const;

            /** \brief Gets a reference to the TrackEntry with the given track
             * number.
             *
             * If no TrackEntry exists with the given number, a new one will be
             * created.
             *
             * \return A reference to a TrackEntry with the given track number.
             */
            mapped_type& operator[](const key_type& key);
            /** \brief Gets a reference to the TrackEntry with the given track
             * number.
             *
             * If no TrackEntry exists with the given number, a new one will be
             * created.
             *
             * \return A reference to a TrackEntry with the given track number.
             */
            mapped_type const& operator[](const key_type& key) const;

            /// \brief Get an iterator to the first TrackEntry.
            iterator begin();
            /// \brief Get an iterator to the first TrackEntry.
            const_iterator begin() const;
            /// \brief Get an iterator to the position past the last TrackEntry.
            iterator end();
            /// \brief Get an iterator to the position past the last TrackEntry.
            const_iterator end() const;
            /// \brief Get a reverse iterator to the last TrackEntry.
            reverse_iterator rbegin();
            /// \brief Get a reverse iterator to the last TrackEntry.
            const_reverse_iterator rbegin() const;
            /** \brief Get a reverse iterator to the position before the first
             * TrackEntry.
             */
            reverse_iterator rend();
            /** \brief Get a reverse iterator to the position before the first
             * TrackEntry.
             */
            const_reverse_iterator rend() const;

            /// \brief Check if there are no TrackElements.
            bool empty() const;
            /// \brief Get the number of TrackElements.
            size_type size() const;
            /// \brief Get the maximum number of TrackElements.
            size_type max_size() const;

            /// \brief Remove all TrackElements.
            void clear();
            /** \brief Insert a new TrackElement.
             *
             * If a TrackElement already exists with the same track number, the
             * new one will not replace it, and the return code will indicate
             * that no insertion took place.
             *
             * \param[in] value The TrackEntry to insert.
             *
             * \return A pair of the iterator at the position where the
             * TrackEntry was added (or blocked) and a boolean indicating if
             * the insertion took place.
             */
            std::pair<iterator, bool> insert(mapped_type const& value);
            /** \brief Insert a new TrackElement.
             *
             * If a TrackElement already exists with the same track number, the
             * new one will not replace it, and the return code will indicate
             * that no insertion took place.
             *
             * \param[in] value A pair of the track number to insert at and the
             * TrackEntry to insert there.
             *
             * \return A pair of the iterator at the position where the
             * TrackEntry was added (or blocked) and a boolean indicating if
             * the insertion took place.
             */
            std::pair<iterator, bool> insert(value_type const& value);
            /** \brief Insert a range of TrackElements.
             *
             * \param[in] first The start of the range.
             * \param[in] last The end of the range.
             */
            void insert(const_iterator first, const_iterator last);
            /** \brief Erase the TrackEntry at the specified iterator.
             *
             * \param[in] position The position to erase at.
             */
            void erase(iterator position);
            /** \brief Erase a range of TrackEntries.
             *
             * \param[in] first The start of the range.
             * \param[in] last The end of the range.
             */
            void erase(iterator first, iterator last);
            /** \brief Erase the TrackEntry with the given track number.
             *
             * \param[in] number The track number to erase.
             * \return The number of TrackEntries erased.
             */
            size_type erase(key_type const& number);
            /** \brief Swaps the contents of this Tracks element with another.
             *
             * \param[in] other The other Tracks element to swap with.
             */
            void swap(Tracks& other);

            /** \brief Search for the TrackEntry with the given track number.
             *
             * \param[in] number The track number to search for.
             * \return An iterator to the matching TrackEntry, or end() if
             * there is no TrackEntry with that number.
             */
            iterator find(key_type const& number);
            /** \brief Search for the TrackEntry with the given track number.
             *
             * \param[in] number The track number to search for.
             * \return An iterator to the matching TrackEntry, or end() if
             * there is no TrackEntry with that number.
             */
            const_iterator find(key_type const& number) const;

            /// \brief The key type (Key) of this container.
            typedef storage_type_::key_type key_type;
            /// \brief The mapped type (T) of this container.
            typedef storage_type_::mapped_type mapped_type;
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

        protected:
            /// \brief The type of the internal storage.
            typedef std::map<uint64_t, TrackEntry::Ptr> storage_type_;
            /** \brief The track entry store.
             *
             * This must always be sorted by TrackNumber.
             */
            storage_type_ entries_;
    }; // class Tracks

    friend
}; // namespace tide

/// @}
// group elements

#endif // TIDE_TRACKS_H_

