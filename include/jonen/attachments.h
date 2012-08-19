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

#if !defined(ATTACHMENTS_H_)
#define ATTACHMENTS_H_

#include <jonen/attached_file.h>
#include <jonen/element_base.h>
#include <jonen/jonen_config.h>
#include <jonen/master_element.h>
#include <jonen/master_element_impl.h>
#include <jonen/win_dll.h>

#include <boost/operators.hpp>
#include <boost/scoped_ptr.hpp>
#include <list>

/// \addtogroup interfaces Interfaces
/// @{

namespace jonen
{
    /** \brief The Attachments element, used to store attached files.
     *
     * A Jonen segment may contain one or more attached files. They are stored
     * in this element as an array.
     *
     * This class implements the Element interface.
     *
     * This class implements the MasterElement interface.
     *
     * This class implements the std::list interface.
     */
    class JONEN_EXPORT Attachments :
        public ElementBase<Attachments>,
        public MasterElement,
        boost::equality_comparable<Attachments>,
        boost::less_than_comparable<Attachments>
    {
        friend class ElementBase<Attachments>;

        public:
            /// \brief The value type (T) of this container.
            typedef AttachedFile value_type;

        private:
            /// \brief The type of the internal storage.
            typedef std::list<value_type> storage_type_;

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
             * Upon creation, the list of attachments will be empty.
             */
            Attachments();

            /// \brief Copy constructor.
            Attachments(Attachments const& rhs);

            /// \brief Assignment operator.
            Attachments& operator=(Attachments const& rhs);

            /// \brief Destructor.
            ~Attachments();

            /** \brief Swap this element with another.
             *
             * \param[in] other The other element to swap with.
             */
            void swap(Attachments& other);

            /// \brief Get the allocator.
            allocator_type get_allocator() const;

            /// \brief Replace the contents of the container.
            void assign(size_type count, value_type const& value);
            /// \brief Replace the contents of the container.
            template<typename InputIt>
            void assign(InputIt first, InputIt last);

            /// \brief Get a reference to the first element.
            reference front();
            /// \brief Get a reference to the first element.
            const_reference front() const;

            /// \brief Get a reference to the last element.
            reference back();
            /// \brief Get a reference to the last element.
            const_reference back() const;

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
                return list_.insert(pos, first, last);
            }
#else // defined(JONEN_CPLUSPLUS11_SUPPORT)
            template<typename InputIt>
            void insert(iterator pos, InputIt first, InputIt last)
            {
                list_.insert(pos, first, last);
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
            iterator emplace(const_iterator pos, std::string const& name,
                    std::string const& mime_type,
                    std::vector<char> const& data, unsigned long long int uid,
                    std::string const& desc="");
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
            void emplace_back(std::string const& name,
                    std::string const& mime_type,
                    std::vector<char> const& data, unsigned long long int uid,
                    std::string const& desc="");
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)

            /// \brief Remove the last attached file from the list.
            void pop_back();

            /// \brief Push an attached file on to the front of the list.
            void push_front(value_type const& value);
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            /// \brief Push an attached file on to the front of the list.
            void push_front(value_type&& value);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)

#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Insert an attached file at the front of the list.
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
            void emplace_front(std::string const& name,
                    std::string const& mime_type,
                    std::vector<char> const& data, unsigned long long int uid,
                    std::string const& desc="");

#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
            /// \brief Remove the first attached file from the list.
            void pop_front();

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

            /** \brief Merge two sorted lists of attached files.
             *
             * The two lists should be sorted into ascending order.
             *
             * No attached files are copied; the other list will be empty after
             * this operation.
             */
            void merge(Attachments& other);
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Merge two sorted lists of attached files.
             *
             * The two lists should be sorted into ascending order.
             *
             * No attached files are copied; the other list will be empty after
             * this operation.
             */
            void merge(Attachments&& other);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Merge two sorted lists of attached files.
             *
             * The two lists should be sorted into ascending order. The
             * elements are compared using the provided comparison function.
             *
             * No attached files are copied; the other list will be empty after
             * this operation.
             */
            template<typename Compare>
            void merge(Attachments& other, Compare comp)
            {
                list_.merge(other.list_, comp);
            }
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Merge two sorted lists of attached files.
             *
             * The two lists should be sorted into ascending order. The
             * elements are compared using the provided comparison function.
             *
             * No attached files are copied; the other list will be empty after
             * this operation.
             */
            template<typename Compare>
            void merge(Attachments&& other, Compare comp)
            {
                list_.merge(other.list_, comp);
            }
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)

            /** \brief Move the attached files from the other list into this
             * one.
             *
             * \param[in] pos The position to insert the attached files before.
             * \param[in] other The source list.
             */
            void splice(iterator pos, Attachments& other);
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Move the attached files from the other list into this
             * one.
             *
             * \param[in] pos The position to insert the attached files before.
             * \param[in] other The source list.
             */
            void splice(const_iterator pos, Attachments&& other);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Move the specified attached file from the other list
             * into this one.
             *
             * \param[in] pos The position to insert the attached file before.
             * \param[in] other The source list.
             * \param[in] it The attached file to move.
             */
            void splice(iterator pos, Attachments& other, iterator it);
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Move the specified attached file from the other list
             * into this one.
             *
             * \param[in] pos The position to insert the attached file before.
             * \param[in] other The source list.
             * \param[in] it The attached file to move.
             */
            void splice(const_iterator pos, Attachments&& other,
                    const_iterator it);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Move the attached files in the specified range from the
             * other list into this one.
             *
             * \param[in] pos The position to insert the attached files before.
             * \param[in] other The source list.
             * \param[in] first The first attached file to move.
             * \param[in] last The attached file \e after the last to move.
             */
            void splice(iterator pos, Attachments& other,
                    iterator first, iterator last);
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
            /** \brief Move the attached files in the specified range from the
             * other list into this one.
             *
             * \param[in] pos The position to insert the attached files before.
             * \param[in] other The source list.
             * \param[in] first The first attached file to move.
             * \param[in] last The attached file \e after the last to move.
             */
            void splice(const_iterator pos, Attachments&& other,
                    const_iterator first, const_iterator last);
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)

            /// \brief Remove all attached files equal to the one provided.
            void remove(value_type const& value);
            /// \brief Remove all attached files matching the given predicate.
            template<typename UnaryPredicate>
            void remove_if(UnaryPredicate p)
            {
                list_.remove_if(p);
            }

            /// \brief Reverse the order of the attached files.
            void reverse();

            /// \brief Remove all consecutive duplicate attached files.
            void unique();
            /** \brief Remove all consecutive duplicate attached files, using
             * the given predicate to determine equality.
             */
            template<typename BinaryPredicate>
            void unique(BinaryPredicate p)
            {
                list_.unique(p);
            }

            /// \brief Sort the attached files in ascending order.
            void sort();
            /** \brief Sort the attached files using the given predicate.
             */
            template<typename Compare>
            void sort(Compare comp)
            {
                list_.sort(comp);
            }

            /// \brief Equality operator.
            friend bool operator==(Attachments const& lhs,
                    Attachments const& rhs);

            /// \brief Less-than operator.
            friend bool operator<(Attachments const& lhs,
                    Attachments const& rhs);

        private:
            storage_type_ list_;
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
    }; // class Attachments


    /// \brief Equality operator.
    bool operator==(Attachments const& lhs, Attachments const& rhs);


    /// \brief Less-than operator.
    bool operator<(Attachments const& lhs, Attachments const& rhs);


    /// \brief Swap elements
    void swap(Attachments& a, Attachments& b);


    /// \brief Stream output operator.
    std::ostream& operator<<(std::ostream& o, Attachments const& rhs);
}; // namespace jonen

/// @}
/// group interfaces

#endif // !defined(ATTACHMENTS_H_)

