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

#if !defined(BINARY_ELEMENT_IMPL_H_)
#define BINARY_ELEMENT_IMPL_H_

#include <tide/primitive_element_base.h>
#include <tide/tide_config.h>

#include <ios>
#include <vector>

/// \addtogroup implmentations Internal implementations
/// @{

namespace tide
{
    namespace impl
    {
        /** \brief Internal implementation of the binary primitive element.
         *
         * This class implements the PrimitiveElementBase CRTP requirements.
         */
        class BinaryElementImpl
            : public PrimitiveElementBase<BinaryElementImpl,
                std::vector<char> >
        {
            friend class PrimitiveElementBase<BinaryElementImpl,
                   std::vector<char> >;

            public:
                // Export the underlying type's typedefs
                typedef std::vector<char>::value_type value_type;
                typedef std::vector<char>::allocator_type allocator_type;
                typedef std::vector<char>::size_type size_type;
                typedef std::vector<char>::difference_type difference_type;
                typedef std::vector<char>::reference reference;
                typedef std::vector<char>::const_reference const_reference;
                typedef std::vector<char>::pointer pointer;
                typedef std::vector<char>::const_pointer const_pointer;
                typedef std::vector<char>::iterator iterator;
                typedef std::vector<char>::const_iterator const_iterator;
                typedef std::vector<char>::reverse_iterator reverse_iterator;
                typedef std::vector<char>::const_reverse_iterator
                    const_reverse_iterator;

                /** \brief Constructor.
                 *
                 * \param[in] value The value of the element.
                 */
                BinaryElementImpl(std::vector<char> const& value);

                /** \brief Constructor.
                 *
                 * \param[in] value The value of the element.
                 * \param[in] default_val The default value of the element.
                 */
                BinaryElementImpl(std::vector<char> const& value,
                        std::vector<char> const& default_val);

                /** \brief Swap this element with another.
                 *
                 * \param[in] other The other element to swap with.
                 */
                void swap(BinaryElementImpl& other);

                /** \brief Swap this element's value with another instance of
                 * the value type.
                 *
                 * Only the value is swapped. The ID and default value are left
                 * unchanged.
                 *
                 * \param[in] other The other value to swap with.
                 */
                void swap(std::vector<char>& other);

                /// \brief Less-than comparison operator.
                bool operator<(BinaryElementImpl const& rhs)
                {
                    return value_ < rhs.value_;
                }

                /// \brief Less-than comparison operator.
                bool operator<(std::vector<char> const& rhs)
                {
                    return value_ < rhs;
                }

                /// \brief Greater-than comparison operator.
                bool operator>(std::vector<char> const& rhs)
                {
                    return value_ > rhs;
                }

                /// \brief Get the size of the stored value.
                std::streamsize body_stored_size() const;

                /// \brief Read the stored value from a byte stream.
                std::streamsize read_body(std::istream& i, std::streamsize size);

                /// \brief Begin writing the stored value to a byte stream.
                std::streamsize start_body(std::ostream& o) const;

                /// \brief Finish writing the stored value to a byte stream.
                std::streamsize finish_body(std::ostream& o) const;

                /** \brief Stream output operator.
                 *
                 * This function is a friend so it can access the value_ member
                 * without copying.
                 * TODO: remove friendship when PrimitiveElement supports
                 * references in its virtual interface.
                 */
                friend std::ostream& operator<<(std::ostream& o,
                        BinaryElementImpl const& rhs);

                ///////////////////////////////////////////////////////////////////
                // Vector interface member functions

                /// \brief Replace the contents of the container.
                void assign(size_type count, char value)
                {
                    return value_.assign(count, value);
                }
                /// \brief Replace the contents of the container.
                template<class InputIterator>
                void assign(InputIterator first, InputIterator last)
                {
                    return value_.assign(first, last);
                }

                /// \brief Get the allocator associated with the container.
                allocator_type get_allocator() const
                {
                    return value_.get_allocator();
                }

                /** \brief Get a reference to the element at the specified
                 * location.
                 */
                reference at(size_type pos)
                {
                    return value_.at(pos);
                }
                /** \brief Get a reference to the element at the specified
                 * location.
                 *
                 * \exception std::out_of_range if the location is invalid.
                 */
                const_reference at(size_type pos) const
                {
                    return value_.at(pos);
                }

                /** \brief Get a reference to the element at the specified
                 * location.
                 *
                 * No bounds checking is performed.
                 */
                reference operator[](size_type pos)
                {
                    return value_[pos];
                }
                /** \brief Get a reference to the element at the specified
                 * location.
                 *
                 * No bounds checking is performed.
                 */
                const_reference operator[](size_type pos) const
                {
                    return value_[pos];
                }

                /// \brief Get a reference to the first element.
                reference front()
                {
                    return value_.front();
                }
                /// \brief Get a reference to the first element.
                const_reference front() const
                {
                    return value_.front();
                }

                /// \brief Get a reference to the last element.
                reference back()
                {
                    return value_.back();
                }
                /// \brief Get a reference to the last element.
                const_reference back() const
                {
                    return value_.back();
                }

                /// \brief Get a pointer to the underlying array.
                char* data()
                {
                    return value_.data();
                }
                /// \brief Get a pointer to the underlying array.
                char const* data() const
                {
                    return value_.data();
                }

                /// \brief Get an iterator to the first element of the container.
                iterator begin()
                {
                    return value_.begin();
                }
                /// \brief Get an iterator to the first element of the container.
                const_iterator begin() const
                {
                    return value_.begin();
                }
#if __cplusplus > 199711L
                /** \brief Get an iterator to the first element of the container.
                 *
                 * Requires C++11 support.
                 */
                const_iterator cbegin() const
                {
                    return value_.cbegin();
                }
#endif // __cplusplus > 199711L

                /** \brief Get an iterator to the element following the last
                 * element of the container.
                 */
                iterator end()
                {
                    return value_.end();
                }
                /** \brief Get an iterator to the element following the last
                 * element of the container.
                 */
                const_iterator end() const
                {
                    return value_.end();
                }
#if __cplusplus > 199711L
                /** \brief Get an iterator to the element following the last
                 * element of the container.
                 *
                 * Requires C++11 support.
                 */
                const_iterator cend() const
                {
                    return value_.cend();
                }
#endif // __cplusplus > 199711L

                /** \brief Get a reverse iterator to the first element of the
                 * reversed container.
                 */
                reverse_iterator rbegin()
                {
                    return value_.rbegin();
                }
                /** \brief Get a reverse iterator to the first element of the
                 * reversed container.
                 */
                const_reverse_iterator rbegin() const
                {
                    return value_.rbegin();
                }
#if __cplusplus > 199711L
                /** \brief Get a reverse iterator to the first element of the
                 * reversed container.
                 *
                 * Requires C++11 support.
                 */
                const_reverse_iterator crbegin() const
                {
                    return value_.crbegin();
                }
#endif // __cplusplus > 199711L

                /** \brief Get a reverse iterator to the element following the
                 * last element of the reversed container.
                 */
                reverse_iterator rend()
                {
                    return value_.rend();
                }
                /** \brief Get a reverse iterator to the element following the
                 * last element of the reversed container.
                 */
                const_reverse_iterator rend() const
                {
                    return value_.rend();
                }
#if __cplusplus > 199711L
                /** \brief Get a reverse iterator to the element following the
                 * last element of the reversed container.
                 *
                 * Requires C++11 support.
                 */
                const_reverse_iterator crend() const
                {
                    return value_.crend();
                }
#endif // __cplusplus > 199711L

                /// \brief Check if the container is empty.
                bool empty() const
                {
                    return value_.empty();
                }

                /// \brief Get the size of the container.
                size_type size() const
                {
                    return value_.size();
                }

                /** \brief Get the maximum number of elements the container can
                 * hold.
                 */
                size_type max_size() const
                {
                    return value_.max_size();
                }

                /** \brief Set the capacity of the container to at least size,
                 * allocating new storage if necessary.
                 */
                void reserve(size_type size)
                {
                    value_.reserve(size);
                }

                /** \brief Get the number of elements that the container has
                 * currently allocated space for.
                 */
                size_type capacity() const
                {
                    return value_.capacity();
                }

#if __cplusplus > 199711L
                /** \brief Request the removal of unused capacity.
                 *
                 * This is non-binding: the request is not guaranteed to be
                 * fulfilled.
                 *
                 * Requires C++11 support.
                 */
                void shrink_to_fit()
                {
                    value_.shrink_to_fit();
                }
#endif // __cplusplus > 199711L

                /** \brief Remove all elements from the container.
                 *
                 * The allocated memory will not be released.
                 */
                void clear()
                {
                    value_.clear();
                }

                /// \brief Insert value before the element pointed to by pos.
                iterator insert(iterator pos, char const& value)
                {
                    return value_.insert(pos, value);
                }
#if __cplusplus > 199711L
                /** \brief Insert value before the element pointed to by pos.
                 *
                 * Requires C++11 support.
                 */
                iterator insert(const_iterator pos, char const& value)
                {
                    return value_.insert(pos, value);
                }
#endif // __cplusplus > 199711L
#if __cplusplus > 199711L
                /** \brief Insert value before the element pointed to by pos.
                 *
                 * Requires C++11 support.
                 */
                iterator insert(const_iterator pos, char&& value)
                {
                    return value_.insert(pos, value);
                }
#endif // __cplusplus > 199711L
                /** \brief Insert count copies of the value before the element
                 * pointed to by pos.
                 */
                void insert(iterator pos, size_type count, char const& value)
                {
                    value_.insert(pos, count, value);
                }
#if __cplusplus > 199711L
                /** \brief Insert count copies of the value before the element
                 * pointed to by pos.
                 *
                 * Requires C++11 support.
                 */
                iterator insert(const_iterator pos, size_type count, char const& value)
                {
                    return value_.insert(pos, count, value);
                }
#endif // __cplusplus > 199711L
                /** \brief Insert elements from the range [first, last) before the
                 * element pointed to by pos.
                 */
                template<class InputIterator>
                void insert(iterator pos, InputIterator first, InputIterator last)
                {
                    value_.insert(pos, first, last);
                }
#if __cplusplus > 199711L
                /** \brief Insert elements from the range [first, last) before the
                 * element pointed to by pos.
                 *
                 * Requires C++11 support.
                 */
                template<class InputIterator>
                iterator insert(const_iterator pos, InputIterator first,
                        InputIterator last)
                {
                    return value_.insert(pos, first, last);
                }
#endif // __cplusplus > 199711L
#if __cplusplus > 199711L
                /** \brief Insert elements from the initialiser list ilist.
                 *
                 * Requires C++11 support.
                 */
                iterator insert(const_iterator pos,
                        std::initializer_list<char> ilist)
                {
                    return value_.insert(pos, ilist);
                }
#endif // __cplusplus > 199711L

#if __cplusplus > 199711L
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
                    return value_.emplace(pos, args);
                }
#endif // __cplusplus > 199711L

                /// \brief Removes the element at pos.
                iterator erase(iterator pos)
                {
                    return value_.erase(pos);
                }
#if __cplusplus > 199711L
                /** \brief Removes the element at pos.
                 *
                 * Requires C++11 support.
                 */
                iterator erase(const_iterator pos)
                {
                    return value_.erase(pos);
                }
#endif // __cplusplus > 199711L
                /// \brief Removes the elements in the range [first, last).
                iterator erase(iterator first, iterator last)
                {
                    return value_.erase(first, last);
                }
#if __cplusplus > 199711L
                /** \brief Removes the elements in the range [first, last).
                 *
                 * Requires C++11 support.
                 */
                iterator erase(const_iterator first, const_iterator last)
                {
                    return value_.erase(first, last);
                }
#endif // __cplusplus > 199711L

                /** \brief Appends the given element to the end of the container.
                 *
                 * If size() is not larger than capacity(), no iterators or
                 * references are invalidated. Otherwise all iterators and
                 * references are invalidated.
                 */
                void push_back(char value)
                {
                    return value_.push_back(value);
                }
#if __cplusplus > 199711L
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
                    value_.push_back(value);
                }
#endif // __cplusplus > 199711L

#if __cplusplus > 199711L
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
                    value_.emplace_back(args);
                }
#endif // __cplusplus > 199711L

                /** \brief Removes the last element of the container.
                 *
                 * No iterators or references are invalidated.
                 */
                void pop_back()
                {
                    value_.pop_back();
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
                    value_.resize(count, value);
                }
#if __cplusplus > 199711L
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
                    value_.resize(count);
                }
#endif // __cplusplus > 199711L
#if __cplusplus > 199711L
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
                    value_.resize(count, value);
                }
#endif // __cplusplus > 199711L

            private:
                ///////////////////////////////////////////////////////////////////
                // PrimitiveElement CRTP required members
                std::vector<char> value_;
                std::vector<char> default_;
                bool has_default_;
                // Must be mutable to be updated in start_body()
                mutable std::streampos body_end_;
        }; // class BinaryElementImpl


        /// \brief Swap date element implementation objects.
        void swap(BinaryElementImpl& a, BinaryElementImpl& b);


        /** \brief Swap this element's value with another instance of the
         * value type.
         *
         * Only the value is swapped. The ID and default value are left
         * unchanged.
         */
        void swap(BinaryElementImpl& a, std::vector<char>& b);


        /// \brief Stream output operator.
        std::ostream& operator<<(std::ostream& o, BinaryElementImpl const& rhs);
    }; // namespace impl
}; // namespace tide

/// @}
// group implementations

#endif // !defined(BINARY_ELEMENT_IMPL_H_

