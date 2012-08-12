/*
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2011, 2012, Geoffrey Biggs, geoffrey.biggs@aist.go.jp
 *     RT-Synthesis Research Group
 *     Intelligent Systems Research Institute,
 *     National Institute of Advanced Industrial Science and Technology (AIST),
 *     Japan
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Geoffrey Biggs nor AIST, nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#if !defined(METASEEK_H_)
#define METASEEK_H_

#include <jonen/element_base.h>
#include <jonen/jonen_config.h>
#include <jonen/master_element.h>
#include <jonen/win_dll.h>

#include <boost/operators.hpp>
#include <boost/scoped_ptr.hpp>

/// \addtogroup interfaces Interfaces
/// @{

namespace jonen
{
    /** \brief The Metaseek element, used as an index for a Jonen file.
     *
     * Generally, any Jonen segment should contain one, and only one, instance
     * of a Metaseek element. This element should provide the byte offsets in
     * the data stream of the other Level 1 elements (SegmentInfo, Tracks,
     * etc.) in the segment. This function also means that the Metaseek element
     * should be placed first in its segment, so that it can be found quickly
     * to speed up file loading.
     *
     * When writing Jonen files, it is advisable to reserve some space at the
     * start of the segment for the Metaseek element to be written in once the
     * file is complete.
     *
     * This class implements the Element interface.
     *
     * This class implements the MasterElement interface.
     *
     * This class implements the std::multimap interface.
     */
    class JONEN_EXPORT Metaseek :
        public ElementBase<Metaseek>,
        public MasterElement,
        boost::equality_comparable<Metaseek>,
        boost::less_than_comparable<Metaseek>
    {
        friend class ElementBase<Metaseek>;

        public:
            /// \brief The key type (Key) of this container.
            typedef ids::ID key_type;
            /// \brief The mapped type (T) of this container.
            typedef std::streamoff mapped_type;

        private:
            /// \brief The type of the internal storage.
            typedef std::multimap<key_type, mapped_type> storage_type_;

        public:
            // Export typedefs
            /// \brief The value type of this container.
            typedef storage_type_::value_type value_type;
            /// \brief The size type of this container.
            typedef storage_type_::size_type size_type;
            /// \brief The type used for differences.
            typedef storage_type_::difference_type difference_type;
            /// \brief Comparator.
            typedef storage_type_::key_compare key_compare;
            /// \brief The allocator type.
            typedef storage_type_::allocator_type allocator_type;
            /// \brief The reference type.
            typedef storage_type_::reference reference;
            /// \brief The constant reference type.
            typedef storage_type_::const_reference const_reference;
            /// \brief The pointer type.
            typedef storage_type_::pointer pointer;
            /// \brief The constant pointer type.
            typedef storage_type_::const_pointer const_pointer;
            /// \brief The random access iterator type.
            typedef storage_type_::iterator iterator;
            /// \brief The constant random access iterator type.
            typedef storage_type_::const_iterator const_iterator;
            /// \brief The reversed random access iterator type.
            typedef storage_type_::reverse_iterator reverse_iterator;
            /// \brief The constant reversed random access iterator type.
            typedef storage_type_::const_reverse_iterator
                const_reverse_iterator;
            /// \brief The class that compares two entries.
            typedef storage_type_::value_compare value_compare;

            /** \brief Constructor.
             *
             * Upon creation, the index will be empty.
             */
            Metaseek();

            /// \brief Copy constructor.
            Metaseek(Metaseek const& rhs);

            /// \brief Assignment operator.
            Metaseek& operator=(Metaseek const& rhs);

            /// \brief Destructor.
            ~Metaseek();

            /// \brief Get the allocator.
            allocator_type get_allocator() const;

            /// \brief Get an iterator to the first index entry.
            iterator begin();
            /// \brief Get an iterator to the first index entry.
            const_iterator begin() const;
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            /// \brief Get an iterator to the first index entry.
            const_iterator cbegin() const;
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Get an iterator to the position past the last index
             * entry.
             */
            iterator end();
            /** \brief Get an iterator to the position past the last index
             * entry.
             */
            const_iterator end() const;
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Get an iterator to the position past the last index
             * entry.
             */
            const_iterator cend() const;
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
            /// \brief Get a reverse iterator to the last index entry.
            reverse_iterator rbegin();
            /// \brief Get a reverse iterator to the last index entry.
            const_reverse_iterator rbegin() const;
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            /// \brief Get a reverse iterator to the last index entry.
            const_reverse_iterator crbegin() const;
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Get a reverse iterator to the position before the first
             * index entry.
             */
            reverse_iterator rend();
            /** \brief Get a reverse iterator to the position before the first
             * index entry.
             */
            const_reverse_iterator rend() const;
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Get a reverse iterator to the position before the first
             * index entry.
             */
            const_reverse_iterator crend() const;
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)

            /// \brief Check if there are no index entries.
            bool empty() const;
            /// \brief Get the number of index entries.
            size_type size() const;
            /// \brief Get the maximum number of index entries.
            size_type max_size() const;

            /// \brief Remove all index entries.
            void clear();
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
            iterator insert(value_type const& value);
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
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
            iterator insert(value_type&& value);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Insert a new index entry.
             *
             * The new entry is inserted as close as possible and before the
             * given hint. No index entries are overwritten.
             *
             * \param[in] hint The insert position hint.
             * \param[in] value The offset to insert and its ID.
             *
             * \return The iterator at the position where the offset was added.
             */
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            iterator insert(const_iterator hint, value_type const& value);
#else // defined(JONEN_CPLUSPLUS11_SUPPORT)
            iterator insert(iterator hint, value_type const& value);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Insert a new index entry.
             *
             * The new entry is inserted as close as possible and before the
             * given hint. No index entries are overwritten.
             *
             * \param[in] hint The insert position hint.
             * \param[in] value The offset to insert and its ID.
             *
             * \return The iterator at the position where the offset was added.
             */
            iterator insert(const_interator hint, value_type&& value);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Insert a range of offsets.
             *
             * \param[in] first The start of the range.
             * \param[in] last The end of the range.
             */
            void insert(const_iterator first, const_iterator last);
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Insert a new index entry.
             *
             * The new index entry is constructed in-place. No iterators or
             * references are invalidated.
             *
             * \param[in] id The ID for the index entry.
             * \param[in] offset The byte-offset of the element with the ID.
             *
             * \return The iterator at the position where the offset was added.
             */
            iterator emplace(key_type id, mapped_type offset);
            /** \brief Insert a new index entry.
             *
             * The new index entry is constructed in-place using hint as a
             * suggestion for where to put it. No iterators or references are
             * invalidated.
             *
             * \param[in] hint The insert position hint.
             * \param[in] id The ID for the index entry.
             * \param[in] offset The byte-offset of the element with the ID.
             *
             * \return The iterator at the position where the offset was added.
             */
            iterator emplace_hint(const_iterator hint, key_type id,
                    mapped_type offset);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Erase the index entry at the specified iterator.
             *
             * \param[in] position The position to erase at.
             * \return An iterator following the removed element.
             */
            iterator erase(const_iterator position);
#else // defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Erase the index entry at the specified iterator.
             *
             * \param[in] position The position to erase at.
             */
            void erase(iterator position);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Erase a range of index entries.
             *
             * \param[in] first The start of the range.
             * \param[in] last The end of the range.
             * \return An iterator following the last removed element.
             */
            iterator erase(const_iterator first, const_iterator last);
#else // defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Erase a range of index entries.
             *
             * \param[in] first The start of the range.
             * \param[in] last The end of the range.
             */
            void erase(iterator first, iterator last);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Erase all index entries with the given ID.
             *
             * \param[in] id The ID to erase.
             * \return The number of entries erased.
             */
            size_type erase(key_type id);
            /** \brief Swap this element with another.
             *
             * \param[in] other The other element to swap with.
             */
            void swap(Metaseek& other);

            /** \brief Get the number of elements with the given key.
             *
             * \param[in] id The ID to search for.
             */
            size_type count(key_type id) const;
            /** \brief Search for the index entry with the given ID.
             *
             * If multiple offsets exist for the ID, the first one inserted
             * will be returned.
             *
             * \param[in] id The ID to search for.
             * \return An iterator to the matching offset, or end() if
             * there is no index entry with that ID.
             */
            iterator find(key_type id);
            /** \brief Search for the index entry with the given ID.
             *
             * \param[in] id The ID to search for.
             * \return An iterator to the matching offset, or end() if
             * there is no index entry with that ID.
             */
            const_iterator find(key_type id) const;
            /** \brief Get a range containing all elements with the given ID.
             *
             * The range is defined by two iterators. One points to the first
             * element not less than key, and the other points to the first
             * element greater than key.
             *
             * This method is equivalent to calling both lower_bound() and
             * upper_bound().
             *
             * \param[in] id The ID to search for.
             */
            std::pair<iterator, iterator> equal_range(key_type id);
            /** \brief Get a range containing all elements with the given ID.
             *
             * The range is defined by two iterators. One points to the first
             * element not less than key, and the other points to the first
             * element greater than key.
             *
             * This method is equivalent to calling both lower_bound() and
             * upper_bound().
             *
             * \param[in] id The ID to search for.
             */
            std::pair<const_iterator, const_iterator> equal_range(
                    key_type id) const;
            /** \brief Find the first element not less than the given ID.
             *
             * \param[in] id The ID to search for.
             */
            iterator lower_bound(key_type id);
            /** \brief Find the first element not less than the given ID.
             *
             * \param[in] id The ID to search for.
             */
            const_iterator lower_bound(key_type id) const;
            /** \brief Find the first element greater than the given ID.
             *
             * \param[in] id The ID to search for.
             */
            iterator upper_bound(key_type id);
            /** \brief Find the first element greater than the given ID.
             *
             * \param[in] id The ID to search for.
             */
            const_iterator upper_bound(key_type id) const;

            /// \brief Get the function used to compare IDs.
            key_compare key_comp() const;
            /// \brief Get the function object used to compare ID-offset pairs.
            value_compare value_comp() const;

            /// \brief Equality operator.
            bool operator==(Metaseek const& rhs);

            /// \brief Less-than operator.
            bool operator<(Metaseek const& rhs);

        private:
            // Pimpl member
            class Impl;
            boost::scoped_ptr<Impl> pimpl_;
            // Must be mutable to be updated in start_body()
            mutable std::streampos body_end_;

            ///////////////////////////////////////////////////////////////////
            // ElementBase CRTP required members
            ids::ID id_;
            mutable std::streampos offset_;
            mutable bool writing_;

            std::streamsize body_stored_size() const;

            std::streamsize read_body(std::istream& i, std::streamsize size);

            std::streamsize start_body(std::iostream& io) const;

            std::streamsize finish_body(std::iostream& io) const;

            ///////////////////////////////////////////////////////////////////
            // MasterElement interface implementation
            void enable_crc_impl();

            void disable_crc_impl();

            bool crc_enabled_impl() const;
    }; // class Metaseek


    /** \brief Create an index entry.
     *
     * \param[in] id The element ID of the index entry.
     * \param[in] offset The byte-offset in the stream of the element.
     */
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
    Metaseek::value_type make_ms_entry(Metaseek::key_type&& key,
            Metaseek::mapped_type&& offset);
#else // defined(JONEN_CPLUSPLUS11_SUPPORT)
    Metaseek::value_type make_ms_entry(Metaseek::key_type key,
            Metaseek::mapped_type offset);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


    /// \brief Swap EBML header elements
    void swap(Metaseek& a, Metaseek& b);


    /// \brief Stream output operator.
    std::ostream& operator<<(std::ostream& o, Metaseek const& rhs);
}; // namespace jonen

/// @}
/// group interfaces

#endif // !defined(METASEEK_H_)

