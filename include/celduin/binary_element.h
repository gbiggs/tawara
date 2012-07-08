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

#if !defined(BINARY_ELEMENT_H_)
#define BINARY_ELEMENT_H_

#include <celduin/binary_element_impl.h>
#include <celduin/element_base.h>
#include <celduin/primitive_element.h>
#include <celduin/celduin_config.h>
#include <celduin/win_dll.h>

#include <boost/operators.hpp>
#include <iostream>
#include <vector>

/// \addtogroup interfaces Interfaces
/// @{

namespace celduin
{
    /** \brief Interface to a binary primitive element.
     *
     * The binary element is the primitive element used to store arbitrary
     * data.
     *
     * This class implements the Element interface.
     *
     * This class implements the PrimitiveElement interface.
     *
     * This class implements the std::vector<char> interface.
     *
     * Copy-construction and assignment are provided by the compiler.
     */
    class CELDUIN_EXPORT BinaryElement
        : public ElementBase<BinaryElement>,
        public PrimitiveElement<std::vector<char> >,
        boost::totally_ordered<BinaryElement>,
        boost::totally_ordered<BinaryElement, std::vector<char> >,
        boost::equality_comparable<BinaryElement>,
        boost::equality_comparable<BinaryElement, std::vector<char> >
    {
        friend class ElementBase<BinaryElement>;

        public:
            // Export the underlying type's typedefs
            typedef impl::BinaryElementImpl::value_type value_type;
            typedef impl::BinaryElementImpl::allocator_type allocator_type;
            typedef impl::BinaryElementImpl::size_type size_type;
            typedef impl::BinaryElementImpl::difference_type difference_type;
            typedef impl::BinaryElementImpl::reference reference;
            typedef impl::BinaryElementImpl::const_reference const_reference;
            typedef impl::BinaryElementImpl::pointer pointer;
            typedef impl::BinaryElementImpl::const_pointer const_pointer;
            typedef impl::BinaryElementImpl::iterator iterator;
            typedef impl::BinaryElementImpl::const_iterator const_iterator;
            typedef impl::BinaryElementImpl::reverse_iterator reverse_iterator;
            typedef impl::BinaryElementImpl::const_reverse_iterator
                const_reverse_iterator;

            /** \brief Constructor.
             *
             * \param[in] id The ID of the element.
             * \param[in] value The value of the element.
             */
            BinaryElement(ids::ID id, std::vector<char> const& value);

            /** \brief Constructor.
             *
             * \param[in] id The ID of the element.
             * \param[in] value The value of the element.
             * \param[in] default_val The default value of the element.
             */
            BinaryElement(celduin::ids::ID id, std::vector<char> const& value,
                    std::vector<char> const& default_val);

            /** \brief Swap this element with another.
             *
             * \param[in] other The other element to swap with.
             */
            void swap(BinaryElement& other);

            /** \brief Swap this element's value with another instance of the
             * value type.
             *
             * Only the value is swapped. The ID and default value are left
             * unchanged.
             *
             * \param[in] other The other value to swap with.
             */
            void swap(std::vector<char>& other);

            /** \brief Assignment-to-value operator.
             *
             * This operator assigns just the value of the element. The ID and
             * default value are left unchanged.
             */
            BinaryElement& operator=(std::vector<char> const& rhs)
            {
                value(rhs);
                return *this;
            }

            /// \brief Less-than comparison operator.
            friend bool operator<(BinaryElement const& lhs,
                    BinaryElement const& rhs)
            {
                return lhs.value() < rhs.value();
            }

            /// \brief Less-than comparison operator.
            friend bool operator<(BinaryElement const& lhs,
                    std::vector<char> const& rhs)
            {
                return lhs.value() < rhs;
            }

            /// \brief Greater-than comparison operator.
            friend bool operator>(BinaryElement const& lhs,
                    std::vector<char> const& rhs)
            {
                return lhs.value() > rhs;
            }

            /// \brief Streaming output operator.
            friend std::ostream& operator<<(std::ostream& o,
                    BinaryElement const& rhs);

            ///////////////////////////////////////////////////////////////////
            // Vector interface member functions

            /// \brief Replace the contents of the container.
            void assign(size_type count, char value)
            {
                return impl_.assign(count, value);
            }
            /// \brief Replace the contents of the container.
            template<class InputIterator>
            void assign(InputIterator first, InputIterator last)
            {
                return impl_.assign(first, last);
            }

            /// \brief Get the allocator associated with the container.
            allocator_type get_allocator() const
            {
                return impl_.get_allocator();
            }

            /** \brief Get a reference to the element at the specified
             * location.
             */
            reference at(size_type pos)
            {
                return impl_.at(pos);
            }
            /** \brief Get a reference to the element at the specified
             * location.
             *
             * \exception std::out_of_range if the location is invalid.
             */
            const_reference at(size_type pos) const
            {
                return impl_.at(pos);
            }

            /** \brief Get a reference to the element at the specified
             * location.
             *
             * No bounds checking is performed.
             */
            reference operator[](size_type pos)
            {
                return impl_[pos];
            }
            /** \brief Get a reference to the element at the specified
             * location.
             *
             * No bounds checking is performed.
             */
            const_reference operator[](size_type pos) const
            {
                return impl_[pos];
            }

            /// \brief Get a reference to the first element.
            reference front()
            {
                return impl_.front();
            }
            /// \brief Get a reference to the first element.
            const_reference front() const
            {
                return impl_.front();
            }

            /// \brief Get a reference to the last element.
            reference back()
            {
                return impl_.back();
            }
            /// \brief Get a reference to the last element.
            const_reference back() const
            {
                return impl_.back();
            }

            /// \brief Get a pointer to the underlying array.
            char* data()
            {
                return impl_.data();
            }
            /// \brief Get a pointer to the underlying array.
            char const* data() const
            {
                return impl_.data();
            }

            /// \brief Get an iterator to the first element of the container.
            iterator begin()
            {
                return impl_.begin();
            }
            /// \brief Get an iterator to the first element of the container.
            const_iterator begin() const
            {
                return impl_.begin();
            }
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Get an iterator to the first element of the container.
             *
             * Requires C++11 support.
             */
            const_iterator cbegin() const
            {
                return impl_.cbegin();
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

            /** \brief Get an iterator to the element following the last
             * element of the container.
             */
            iterator end()
            {
                return impl_.end();
            }
            /** \brief Get an iterator to the element following the last
             * element of the container.
             */
            const_iterator end() const
            {
                return impl_.end();
            }
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Get an iterator to the element following the last
             * element of the container.
             *
             * Requires C++11 support.
             */
            const_iterator cend() const
            {
                return impl_.cend();
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

            /** \brief Get a reverse iterator to the first element of the
             * reversed container.
             */
            reverse_iterator rbegin()
            {
                return impl_.rbegin();
            }
            /** \brief Get a reverse iterator to the first element of the
             * reversed container.
             */
            const_reverse_iterator rbegin() const
            {
                return impl_.rbegin();
            }
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Get a reverse iterator to the first element of the
             * reversed container.
             *
             * Requires C++11 support.
             */
            const_reverse_iterator crbegin() const
            {
                return impl_.crbegin();
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

            /** \brief Get a reverse iterator to the element following the
             * last element of the reversed container.
             */
            reverse_iterator rend()
            {
                return impl_.rend();
            }
            /** \brief Get a reverse iterator to the element following the
             * last element of the reversed container.
             */
            const_reverse_iterator rend() const
            {
                return impl_.rend();
            }
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Get a reverse iterator to the element following the
             * last element of the reversed container.
             *
             * Requires C++11 support.
             */
            const_reverse_iterator crend() const
            {
                return impl_.crend();
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

            /// \brief Check if the container is empty.
            bool empty() const
            {
                return impl_.empty();
            }

            /// \brief Get the size of the container.
            size_type size() const
            {
                return impl_.size();
            }

            /** \brief Get the maximum number of elements the container can
             * hold.
             */
            size_type max_size() const
            {
                return impl_.max_size();
            }

            /** \brief Set the capacity of the container to at least size,
             * allocating new storage if necessary.
             */
            void reserve(size_type size)
            {
                impl_.reserve(size);
            }

            /** \brief Get the number of elements that the container has
             * currently allocated space for.
             */
            size_type capacity() const
            {
                return impl_.capacity();
            }

#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Request the removal of unused capacity.
             *
             * This is non-binding: the request is not guaranteed to be
             * fulfilled.
             *
             * Requires C++11 support.
             */
            void shrink_to_fit()
            {
                impl_.shrink_to_fit();
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

            /** \brief Remove all elements from the container.
             *
             * The allocated memory will not be released.
             */
            void clear()
            {
                impl_.clear();
            }

#if !defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /// \brief Insert value before the element pointed to by pos.
            iterator insert(iterator pos, char const& value)
            {
                return impl_.insert(pos, value);
            }
#endif // !defined(CELDUIN_CPLUSPLUS11_SUPPORT)
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Insert value before the element pointed to by pos.
             *
             * Requires C++11 support.
             */
            iterator insert(const_iterator pos, char const& value)
            {
                return impl_.insert(pos, value);
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Insert value before the element pointed to by pos.
             *
             * Requires C++11 support.
             */
            iterator insert(const_iterator pos, char&& value)
            {
                return impl_.insert(pos, value);
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)
#if !defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Insert count copies of the value before the element
             * pointed to by pos.
             */
            void insert(iterator pos, size_type count, char const& value)
            {
                impl_.insert(pos, count, value);
            }
#endif // !defined(CELDUIN_CPLUSPLUS11_SUPPORT)
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Insert count copies of the value before the element
             * pointed to by pos.
             *
             * Requires C++11 support.
             */
            iterator insert(const_iterator pos, size_type count, char const& value)
            {
                return impl_.insert(pos, count, value);
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)
#if !defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Insert elements from the range [first, last) before the
             * element pointed to by pos.
             */
            template<class InputIterator>
            void insert(iterator pos, InputIterator first, InputIterator last)
            {
                impl_.insert(pos, first, last);
            }
#endif // !defined(CELDUIN_CPLUSPLUS11_SUPPORT)
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Insert elements from the range [first, last) before the
             * element pointed to by pos.
             *
             * Requires C++11 support.
             */
            template<class InputIterator>
            iterator insert(const_iterator pos, InputIterator first,
                    InputIterator last)
            {
                return impl_.insert(pos, first, last);
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Insert elements from the initialiser list ilist.
             *
             * Requires C++11 support.
             */
            iterator insert(const_iterator pos,
                    std::initializer_list<char> ilist)
            {
                return impl_.insert(pos, ilist);
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Insert a new element at the specified position in the
             * container.
             *
             * The element is constructed in-place and called with the same
             * arguments as those supplied to this function. Iterators and
             * references to the elements between pos and the end of the
             * container are invalidated. 
             *
             * Requires C++11 support.
             */
            template<class... Args>
            iterator emplace(const_iterator pos, Args&&... args)
            {
                return impl_.emplace(pos, args);
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

#if !defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /// \brief Removes the element at pos.
            iterator erase(iterator pos)
            {
                return impl_.erase(pos);
            }
#endif // !defined(CELDUIN_CPLUSPLUS11_SUPPORT)
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Removes the element at pos.
             *
             * Requires C++11 support.
             */
            iterator erase(const_iterator pos)
            {
                return impl_.erase(pos);
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)
#if !defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /// \brief Removes the elements in the range [first, last).
            iterator erase(iterator first, iterator last)
            {
                return impl_.erase(first, last);
            }
#endif // !defined(CELDUIN_CPLUSPLUS11_SUPPORT)
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Removes the elements in the range [first, last).
             *
             * Requires C++11 support.
             */
            iterator erase(const_iterator first, const_iterator last)
            {
                return impl_.erase(first, last);
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

            /** \brief Appends the given element to the end of the container.
             *
             * If size() is not larger than capacity(), no iterators or
             * references are invalidated. Otherwise all iterators and
             * references are invalidated.
             */
            void push_back(char value)
            {
                return impl_.push_back(value);
            }
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Appends the given element to the end of the container.
             *
             * If size() is not larger than capacity(), no iterators or
             * references are invalidated. Otherwise all iterators and
             * references are invalidated.
             *
             * Requires C++11 support.
             */
            void push_back(char&& value)
            {
                impl_.push_back(value);
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Appends a new element to the end of the container.
             *
             * The element is constructed in place, and called with the same
             * arguments as supplied to this function.
             *
             * If new size() is not larger than capacity(), no iterators or
             * references are invalidated. Otherwise all iterators and
             * references are invalidated.
             *
             * Requires C++11 support.
             */
            template<class... Args>
            void emplace_back(Args&&... args)
            {
                impl_.emplace_back(args);
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

            /** \brief Removes the last element of the container.
             *
             * No iterators or references are invalidated.
             */
            void pop_back()
            {
                impl_.pop_back();
            }

            /** \brief Resizes the container to contain count elements.
             *
             * If the current size is less than count, additional elements are
             * appended and initialized with value.
             *
             * If the current size is greater than count, the container is
             * reduced to its first count elements.
             */
            void resize(size_type count, char value = '\0')
            {
                impl_.resize(count, value);
            }
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Resizes the container to contain count elements.
             *
             * If the current size is less than count, additional elements are
             * appended and initialized with value.
             *
             * If the current size is greater than count, the container is
             * reduced to its first count elements.
             *
             * Requires C++11 support.
             */
            void resize(size_type count)
            {
                impl_.resize(count);
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Resizes the container to contain count elements.
             *
             * If the current size is less than count, additional elements are
             * appended and initialized with value.
             *
             * If the current size is greater than count, the container is
             * reduced to its first count elements.
             *
             * Requires C++11 support.
             */
            void resize(size_type count, value_type const& value)
            {
                impl_.resize(count, value);
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

        private:
            ///////////////////////////////////////////////////////////////////
            // PrimitiveBase interface implementation
            impl::BinaryElementImpl impl_;

            // TODO: Fix the PrimitiveElement virtual functions to use
            // references automatically where appropriate.
            virtual std::vector<char> value_impl() const
            {
                return impl_.value();
            }

            // TODO: Fix the PrimitiveElement virtual functions to use
            // references automatically where appropriate.
            virtual void value_impl(std::vector<char> value)
            {
                impl_.value(value);
            }

            virtual bool has_default_impl() const
            {
                return impl_.has_default();
            }

            // TODO: Fix the PrimitiveElement virtual functions to use
            // references automatically where appropriate.
            virtual std::vector<char> get_default_impl() const
            {
                return impl_.get_default();
            }

            // TODO: Fix the PrimitiveElement virtual functions to use
            // references automatically where appropriate.
            virtual void set_default_impl(std::vector<char> default_value)
            {
                impl_.set_default(default_value);
            }

            // TODO: Fix the PrimitiveElement virtual functions to use
            // references automatically where appropriate.
            virtual std::vector<char> remove_default_impl()
            {
                return impl_.remove_default();
            }

            virtual bool is_default_impl() const
            {
                return impl_.is_default();
            }

            ///////////////////////////////////////////////////////////////////
            // ElementBase CRTP required members
            ids::ID id_;
            mutable std::streampos offset_;
            mutable bool writing_;

            std::streamsize body_stored_size() const;

            std::streamsize read_body(std::istream& i, std::streamsize size);

            std::streamsize start_body(std::iostream& io) const;

            std::streamsize finish_body(std::iostream& io) const;
    }; // class BinaryElement


    /// \brief Swap two binary elements.
    void swap(BinaryElement& a, BinaryElement& b);


    /** \brief Swap this element's value with another instance of the
     * value type.
     *
     * Only the value is swapped. The ID and default value are left
     * unchanged.
     */
    void swap(BinaryElement& a, std::vector<char>& b);


    /// \brief Stream output operator.
    std::ostream& operator<<(std::ostream& o, BinaryElement const& rhs)
    {
        return o << rhs.impl_;
    }
}; // namespace celduin

/// @}
// group interfaces

#endif // !defined(BINARY_ELEMENT_H_

