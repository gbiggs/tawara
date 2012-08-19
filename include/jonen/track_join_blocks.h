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

#if !defined(TRACK_JOIN_BLOCKS_H_)
#define TRACK_JOIN_BLOCKS_H_

#include <jonen/element_base.h>
#include <jonen/jonen_config.h>
#include <jonen/master_element.h>
#include <jonen/master_element_impl.h>
#include <jonen/track_join_blocks.h>
#include <jonen/win_dll.h>

#include <boost/operators.hpp>
#include <vector>

/// \addtogroup interfaces Interfaces
/// @{

namespace jonen
{
    /** \brief The TrackJoinBlocks element specifies a track operation.
     *
     * This track operation merges the blocks from multiple tracks into a
     * single virtual track. The blocks are interleaved in time-order. This
     * element contains the UIDs of the source tracks for the operation.
     *
     * The UIDs must all be greater than zero. Attempting to write a
     * TrackJoinBlocks element containing zero-value UIDs is an error.
     *
     * This class implements the Element interface.
     *
     * This class implements the MasterElement interface.
     *
     * This class implements the std::vector interface.
     */
    class JONEN_EXPORT TrackJoinBlocks :
        public ElementBase<TrackJoinBlocks>,
        public MasterElement,
        boost::equality_comparable<TrackJoinBlocks>,
        boost::less_than_comparable<TrackJoinBlocks>
    {
        friend class ElementBase<TrackJoinBlocks>;

        public:
            /// \brief The value type (T) of this container.
            typedef unsigned long long int value_type;

        private:
            /// \brief The type of the internal storage.
            typedef std::vector<value_type> storage_type_;

        public:
            typedef storage_type_::allocator_type allocator_type;
            typedef storage_type_::size_type size_type;
            typedef storage_type_::difference_type difference_type;
            typedef storage_type_::reference reference;
            typedef storage_type_::const_reference const_reference;
            typedef storage_type_::pointer pointer;
            typedef storage_type_::const_pointer const_pointer;
            typedef storage_type_::iterator iterator;
            typedef storage_type_::const_iterator const_iterator;
            typedef storage_type_::reverse_iterator reverse_iterator;
            typedef storage_type_::const_reverse_iterator
                const_reverse_iterator;

            /** \brief Constructor.
             *
             * Upon creation, the list of tracks will be empty.
             */
            TrackJoinBlocks();

            /// \brief Copy constructor.
            TrackJoinBlocks(TrackJoinBlocks const& rhs);

            /// \brief Assignment operator.
            TrackJoinBlocks& operator=(TrackJoinBlocks const& rhs);

            /// \brief Destructor.
            ~TrackJoinBlocks();

            /** \brief Swap this element with another.
             *
             * \param[in] other The other element to swap with.
             */
            void swap(TrackJoinBlocks& other);

            /// \brief Get the allocator.
            allocator_type get_allocator() const;

            /// \brief Replace the contents of the container.
            void assign(size_type count, value_type const& value);
            /// \brief Replace the contents of the container.
            template<typename InputIt>
            void assign(InputIt first, InputIt last)
            {
                for (InputIt temp(first); temp != last; ++temp)
                {
                    if (*temp == 0)
                    {
                        throw ValueOutOfRange() << err_id(ids::TrackJoinUID) <<
                            err_par_id(id_);
                    }
                }
                vector_.assign(first, last);
            }

            /// \brief Get a reference to the specified element.
            reference at(size_type pos);
            /// \brief Get a reference to the specified element.
            const_reference at(size_type pos) const;

            /** \brief Get a reference to the specified element.
             *
             * No bounds checking is performed.
             */
            reference operator[](size_type pos);
            /** \brief Get a reference to the specified element.
             *
             * No bounds checking is performed.
             */
            const_reference operator[](size_type pos) const;

            /// \brief Get a reference to the first element.
            reference front();
            /// \brief Get a reference to the first element.
            const_reference front() const;

            /// \brief Get a reference to the last element.
            reference back();
            /// \brief Get a reference to the last element.
            const_reference back() const;

#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            /// \brief Get a pointer to the underlying data.
            value_type* data();
            /// \brief Get a pointer to the underlying data.
            value_type const* data() const;
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)

            /// \brief Get an iterator to the first attached file.
            iterator begin();
            /// \brief Get an iterator to the first attached file.
            const_iterator begin() const;
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            /// \brief Get an iterator to the first attached file.
            const_iterator cbegin() const;
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Get an iterator to the position past the last attached
             * file.
             */
            iterator end();
            /** \brief Get an iterator to the position past the last attached
             * file.
             */
            const_iterator end() const;
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Get an iterator to the position past the last attached
             * file.
             */
            const_iterator cend() const;
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
            /// \brief Get a reverse iterator to the last attached file.
            reverse_iterator rbegin();
            /// \brief Get a reverse iterator to the last attached file.
            const_reverse_iterator rbegin() const;
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            /// \brief Get a reverse iterator to the last attached file.
            const_reverse_iterator crbegin() const;
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Get a reverse iterator to the position before the first
             * attached file.
             */
            reverse_iterator rend();
            /** \brief Get a reverse iterator to the position before the first
             * attached file.
             */
            const_reverse_iterator rend() const;
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Get a reverse iterator to the position before the first
             * attached file.
             */
            const_reverse_iterator crend() const;
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)

            /// \brief Check if there are no attached files.
            bool empty() const;
            /// \brief Get the number of attached files.
            size_type size() const;
            /// \brief Get the maximum number of attached files.
            size_type max_size() const;

            /** \brief Set the capacity of the container.
             *
             * New storage is allocated if necessary.
             */
            void reserve(size_type size);
            /// \brief Get the current capacity of the container.
            size_type capacity() const;
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Request the removal of unused capacity.
             *
             * This request is non-binding.
             */
            void shrink_to_fit();
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)

            /// \brief Remove all attached files.
            void clear();

            /** \brief Insert a new attached file.
             *
             * The new attached file will be inserted before the specified
             * position.
             *
             * \param[in] pos The attached file to insert before.
             * \param[in] value The attached file to insert.
             */
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            iterator insert(const_iterator pos, value_type const& value);
#else // defined(JONEN_CPLUSPLUS11_SUPPORT)
            iterator insert(iterator pos, value_type const& value);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Move an attached file into the list.
             *
             * The new attached file will be inserted before the specified
             * position.
             *
             * \param[in] pos The attached file to insert before.
             * \param[in] value The attached file to insert.
             */
            iterator insert(const_iterator pos, value_type&& value);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Insert multiple copies of an attached file.
             *
             * The new attached files will be inserted before the specified
             * position.
             *
             * \param[in] pos The attached file to insert before.
             * \param[in] count The number of copies to insert.
             * \param[in] value The attached file to insert.
             */
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            void insert(const_iterator pos, size_type count,
                    value_type const& value);
#else // defined(JONEN_CPLUSPLUS11_SUPPORT)
            void insert(iterator pos, size_type count,
                    value_type const& value);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Insert a range of attached files.
             *
             * The new attached files will be inserted before the specified
             * position.
             *
             * \param[in] pos The attached file to insert before.
             * \param[in] first The first attached file to insert.
             * \param[in] last The attached file \e after the last to insert.
             */
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            template<typename InputIt>
            iterator insert(const_iterator pos, InputIt first, InputIt last)
            {
                for (InputIt temp(first); temp != last; ++temp)
                {
                    if (*temp == 0)
                    {
                        throw ValueOutOfRange() << err_id(ids::TrackJoinUID) <<
                            err_par_id(id_);
                    }
                }
                return vector_.insert(pos, first, last);
            }
#else // defined(JONEN_CPLUSPLUS11_SUPPORT)
            template<typename InputIt>
            void insert(iterator pos, InputIt first, InputIt last)
            {
                for (InputIt temp(first); temp != last; ++temp)
                {
                    if (*temp == 0)
                    {
                        throw ValueOutOfRange() << err_id(ids::TrackJoinUID) <<
                            err_par_id(id_);
                    }
                }
                vector_.insert(pos, first, last);
            }
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Insert attached files from an initialiser list.
             *
             * The new attached files will be inserted before the specified
             * position.
             *
             * \param[in] pos The attached file to insert before.
             * \param[in] ilist The initialiser list.
             */
            iterator insert(const_iterator pos,
                    std::initializer_list<value_type> ilist);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)

#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Insert a new attached file.
             *
             * The new attached file is constructed in-place. No iterators or
             * references are invalidated.
             *
             * \param[in] pos The position to emplace before.
             * \param[in] name The file name.
             * \param[in] mime_type The file's MIME type.
             * \param[in] data The file's data.
             * \param[in] uid A unique identifier for the file. Must not be
             * zero.
             * \param[in] desc A human-readable description of the file.
             */
            iterator emplace(const_iterator pos, value_type value);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)

#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Erase the attached file at the specified position.
             *
             * \param[in] position The position to erase at.
             * \return An iterator following the removed element.
             */
            iterator erase(const_iterator position);
#else // defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Erase the attached file at the specified position.
             *
             * \param[in] position The position to erase at.
             */
            void erase(iterator position);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Erase a range of attached files.
             *
             * \param[in] first The start of the range.
             * \param[in] last The end of the range.
             * \return An iterator following the last removed element.
             */
            iterator erase(const_iterator first, const_iterator last);
#else // defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Erase a range of attached files.
             *
             * \param[in] first The start of the range.
             * \param[in] last The end of the range.
             */
            void erase(iterator first, iterator last);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)

            /// \brief Append an attached file to the end of the list.
            void push_back(value_type const& value);
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            /// \brief Append an attached file to the end of the list.
            void push_back(value_type&& value);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)

#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Append an attached file to the end of the list.
             *
             * The new attached file is constructed in-place. No iterators or
             * references are invalidated.
             *
             * \param[in] name The file name.
             * \param[in] mime_type The file's MIME type.
             * \param[in] data The file's data.
             * \param[in] uid A unique identifier for the file. Must not be
             * zero.
             * \param[in] desc A human-readable description of the file.
             */
            void emplace_back(value_type value);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)

            /// \brief Remove the last attached file from the list.
            void pop_back();

            /** \brief Resizes the list to contain count elements.
             *
             * If the current size is less than count, additional attached
             * files are appended and initialized with value.
             *
             * If the current size is greater than count, the list is reduced
             * to its first count elements.
             */
            void resize(size_type count, value_type value);
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Resizes the list to contain count elements.
             *
             * If the current size is less than count, additional attached
             * files are appended and initialized with value.
             *
             * If the current size is greater than count, the list is reduced
             * to its first count elements.
             */
            void resize(size_type count);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Resizes the list to contain count elements.
             *
             * If the current size is less than count, additional attached
             * files are appended and initialized with value.
             *
             * If the current size is greater than count, the list is reduced
             * to its first count elements.
             */
            void resize(size_type count, value_type const& value);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)

            /// \brief Equality operator.
            friend bool operator==(TrackJoinBlocks const& lhs,
                    TrackJoinBlocks const& rhs);

            /// \brief Less-than operator.
            friend bool operator<(TrackJoinBlocks const& lhs,
                    TrackJoinBlocks const& rhs);

        private:
            storage_type_ vector_;
            impl::MasterElementImpl master_impl_;
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
    }; // class TrackJoinBlocks


    /// \brief Equality operator.
    bool operator==(TrackJoinBlocks const& lhs, TrackJoinBlocks const& rhs);


    /// \brief Less-than operator.
    bool operator<(TrackJoinBlocks const& lhs, TrackJoinBlocks const& rhs);


    /// \brief Swap elements
    void swap(TrackJoinBlocks& a, TrackJoinBlocks& b);


    /// \brief Stream output operator.
    std::ostream& operator<<(std::ostream& o, TrackJoinBlocks const& rhs);
}; // namespace jonen

/// @}
/// group interfaces

#endif // !defined(TRACK_JOIN_BLOCKS_H_)

