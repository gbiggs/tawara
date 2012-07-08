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

#if !defined(STRING_ELEMENT_H_)
#define STRING_ELEMENT_H_

#include <celduin/string_element_impl.h>
#include <celduin/element_base.h>
#include <celduin/primitive_element.h>
#include <celduin/win_dll.h>

#include <boost/operators.hpp>
#include <iosfwd>
#include <string>

/// \addtogroup interfaces Interfaces
/// @{

namespace celduin
{
    /** \brief Interface to a string primitive element.
     *
     * The string element is used to store text data.
     *
     * This class implements the Element interface.
     *
     * This class implements the PrimitiveElement interface.
     *
     * This class implements the std::string interface.
     *
     * Copy-construction and assignment are provided by the compiler.
     */
    class CELDUIN_EXPORT StringElement
        : public ElementBase<StringElement>,
        public PrimitiveElement<std::string>,
        boost::totally_ordered<StringElement>,
        boost::totally_ordered<StringElement, std::string>,
        boost::equality_comparable<StringElement>,
        boost::equality_comparable<StringElement, std::string>
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
        , boost::addable<StringElement, std::initializer_list<char>>
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)
    {
        friend class ElementBase<StringElement>;

        public:
            // Export the underlying type's typedefs
            typedef impl::StringElementImpl::traits_type traits_type;
            typedef impl::StringElementImpl::value_type value_type;
            typedef impl::StringElementImpl::allocator_type allocator_type;
            typedef impl::StringElementImpl::size_type size_type;
            typedef impl::StringElementImpl::difference_type difference_type;
            typedef impl::StringElementImpl::reference reference;
            typedef impl::StringElementImpl::const_reference const_reference;
            typedef impl::StringElementImpl::pointer pointer;
            typedef impl::StringElementImpl::const_pointer const_pointer;
            typedef impl::StringElementImpl::iterator iterator;
            typedef impl::StringElementImpl::const_iterator const_iterator;
            typedef impl::StringElementImpl::reverse_iterator reverse_iterator;
            typedef impl::StringElementImpl::const_reverse_iterator
                const_reverse_iterator;

            /** \brief Constructor.
             *
             * \param[in] id The ID of the element.
             * \param[in] value The value of the element.
             */
            StringElement(ids::ID id, std::string const& value);

            /** \brief Constructor.
             *
             * \param[in] id The ID of the element.
             * \param[in] value The value of the element.
             * \param[in] default_val The default value of the element.
             */
            StringElement(celduin::ids::ID id, std::string const& value,
                    std::string const& default_val);

            /** \brief Swap this element with another.
             *
             * \param[in] other The other element to swap with.
             */
            void swap(StringElement& other);

            /** \brief Swap this element's value with another instance of the
             * value type.
             *
             * Only the value is swapped. The ID and default value are left
             * unchanged.
             *
             * \param[in] other The other value to swap with.
             */
            void swap(std::string& other);

            /** \brief Assignment-to-value operator.
             *
             * This operator assigns just the value of the element. The ID and
             * default value are left unchanged.
             */
            StringElement& operator=(std::string const& rhs)
            {
                value(rhs);
                return *this;
            }

            /// \brief Less-than comparison operator.
            friend bool operator<(StringElement const& lhs,
                    StringElement const& rhs)
            {
                return lhs.value() < rhs.value();
            }

            /// \brief Less-than comparison operator.
            friend bool operator<(StringElement const& lhs,
                    std::string const& rhs)
            {
                return lhs.value() < rhs;
            }

            /// \brief Greater-than comparison operator.
            friend bool operator>(StringElement const& lhs,
                    std::string const& rhs)
            {
                return lhs.value() > rhs;
            }

            /// \brief Assignment addition operator.
            StringElement& operator+=(StringElement const& rhs)
            {
                impl_ += rhs.impl_;
                return *this;
            }
            /// \brief Assignment addition operator.
            StringElement& operator+=(std::string rhs)
            {
                impl_ += rhs;
                return *this;
            }
            /// \brief Assignment addition operator.
            StringElement& operator+=(char ch)
            {
                impl_ += ch;
                return *this;
            }
            /// \brief Assignment addition operator.
            StringElement& operator+=(char const* s)
            {
                impl_ += s;
                return *this;
            }
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /// \brief Assignment addition operator.
            StringElement& operator+=(std::initializer_list<char> ilist)
            {
                impl_ += ilist;
                return *this;
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

            /// \brief Streaming output operator.
            friend std::ostream& operator<<(std::ostream& o,
                    StringElement const& rhs);

            /// \brief Get the amount of padding used.
            unsigned int padding() const
            {
                return impl_.padding();
            }
            /** \brief Set the amount of padding to use.
             *
             * Strings can be zero-padded at the end. This is particularly
             * useful when overwriting an existing string with one that is
             * shorter, so that the file does not need to be rewritten or a
             * void element used.
             *
             * Management of this value is the responsibility of the user of
             * the StringElement. It will never be adjusted automatically.
             */
            void padding(unsigned int padding)
            {
                impl_.padding(padding);
            }

            ///////////////////////////////////////////////////////////////////
            // String interface member functions

            /// \brief Replace the contents of the string.
            StringElement& assign(size_type count, char value)
            {
                impl_.assign(count, value);
                return *this;
            }
            /// \brief Replace the contents of the string.
            StringElement& assign(std::string const& str)
            {
                impl_.assign(str);
                return *this;
            }
            /// \brief Replace the contents of the string.
            StringElement& assign(std::string const& str, size_type pos,
                    size_type count)
            {
                impl_.assign(str, pos, count);
                return *this;
            }
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Replace the contents of the string.
             *
             * Requires C++11 support.
             */
            StringElement& assign(std::string&& str)
            {
                impl_.assign(str);
                return *this;
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /// \brief Replace the contents of the string.
            StringElement& assign(char const* s, size_type count)
            {
                impl_.assign(s, count);
                return *this;
            }
            /// \brief Replace the contents of the string.
            StringElement& assign(char const* s)
            {
                impl_.assign(s);
                return *this;
            }
            /// \brief Replace the contents of the string.
            template<class InputIterator>
            StringElement& assign(InputIterator first, InputIterator last)
            {
                impl_.assign(first, last);
                return *this;
            }
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Replace the contents of the string.
             *
             * Requires C++11 support.
             */
            StringElement& assign(std::initializer_list<char> ilist)
            {
                impl_.assign(ilist);
                return *this;
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

            /// \brief Get the allocator associated with the string.
            allocator_type get_allocator() const
            {
                return impl_.get_allocator();
            }

            /** \brief Get a reference to the character at the specified
             * location.
             */
            reference at(size_type pos)
            {
                return impl_.at(pos);
            }
            /** \brief Get a reference to the character at the specified
             * location.
             *
             * \exception std::out_of_range if the location is invalid.
             */
            const_reference at(size_type pos) const
            {
                return impl_.at(pos);
            }

            /** \brief Get a reference to the character at the specified
             * location.
             *
             * No bounds checking is performed.
             */
            reference operator[](size_type pos)
            {
                return impl_[pos];
            }
            /** \brief Get a reference to the character at the specified
             * location.
             *
             * No bounds checking is performed.
             */
            const_reference operator[](size_type pos) const
            {
                return impl_[pos];
            }

#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Get a reference to the first character.
             *
             * Requires C++11 support.
             */
            reference front()
            {
                return impl_.front();
            }
            /** \brief Get a reference to the first character.
             *
             * Requires C++11 support.
             */
            const_reference front() const
            {
                return impl_.front();
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Get a reference to the last character.
             *
             * Requires C++11 support.
             */
            reference back()
            {
                return impl_.back();
            }
            /** \brief Get a reference to the last character.
             *
             * Requires C++11 support.
             */
            const_reference back() const
            {
                return impl_.back();
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

            /// \brief Get a pointer to the underlying array.
            char const* data() const
            {
                return impl_.data();
            }

            /** \brief Get a pointer to a null-terminated character array with
             * the equivalent data to that stored in the string.
             */
            char const* c_str() const
            {
                return impl_.c_str();
            }

            /// \brief Get an iterator to the first character of the string.
            iterator begin()
            {
                return impl_.begin();
            }
            /// \brief Get an iterator to the first character of the string.
            const_iterator begin() const
            {
                return impl_.begin();
            }
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Get an iterator to the first character of the string.
             *
             * Requires C++11 support.
             */
            const_iterator cbegin() const
            {
                return impl_.cbegin();
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

            /** \brief Get an iterator to the character following the last
             * character of the string.
             */
            iterator end()
            {
                return impl_.end();
            }
            /** \brief Get an iterator to the character following the last
             * character of the string.
             */
            const_iterator end() const
            {
                return impl_.end();
            }
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Get an iterator to the character following the last
             * character of the string.
             *
             * Requires C++11 support.
             */
            const_iterator cend() const
            {
                return impl_.cend();
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

            /** \brief Get a reverse iterator to the first character of the
             * reversed string.
             */
            reverse_iterator rbegin()
            {
                return impl_.rbegin();
            }
            /** \brief Get a reverse iterator to the first character of the
             * reversed string.
             */
            const_reverse_iterator rbegin() const
            {
                return impl_.rbegin();
            }
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Get a reverse iterator to the first character of the
             * reversed string.
             *
             * Requires C++11 support.
             */
            const_reverse_iterator crbegin() const
            {
                return impl_.crbegin();
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

            /** \brief Get a reverse iterator to the character following the
             * last character of the reversed string.
             */
            reverse_iterator rend()
            {
                return impl_.rend();
            }
            /** \brief Get a reverse iterator to the character following the
             * last character of the reversed string.
             */
            const_reverse_iterator rend() const
            {
                return impl_.rend();
            }
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Get a reverse iterator to the character following the
             * last character of the reversed string.
             *
             * Requires C++11 support.
             */
            const_reverse_iterator crend() const
            {
                return impl_.crend();
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

            /// \brief Check if the string is empty.
            bool empty() const
            {
                return impl_.empty();
            }

            /// \brief Get the number of characters in the string.
            size_type size() const
            {
                return impl_.size();
            }

            /// \brief Get the number of characters in the string.
            size_type length() const
            {
                return impl_.length();
            }

            /** \brief Get the maximum number of characters the string can
             * hold.
             */
            size_type max_size() const
            {
                return impl_.max_size();
            }

            /** \brief Set the capacity of the string to at least size,
             * allocating new storage if necessary.
             */
            void reserve(size_type size)
            {
                impl_.reserve(size);
            }

            /** \brief Get the number of characters that the string has
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

            /** \brief Remove all characters from the string.
             *
             * The allocated memory will not be released.
             */
            void clear()
            {
                impl_.clear();
            }

#if !defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /// \brief Insert value before the character pointed to by pos.
            iterator insert(iterator pos, char value)
            {
                return impl_.insert(pos, value);
            }
#endif // !defined(CELDUIN_CPLUSPLUS11_SUPPORT)
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Insert value before the character pointed to by pos.
             *
             * Requires C++11 support.
             */
            iterator insert(const_iterator pos, char value)
            {
                return impl_.insert(pos, value);
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Insert count copies of the value before the character
             * pointed to by pos.
             */
            void insert(iterator pos, size_type count, char value)
            {
                impl_.insert(pos, count, value);
            }
            /// Insert count copies of the value at the given index.
            StringElement& insert(size_type index, size_type count, char value)
            {
                impl_.insert(index, count, value);
                return *this;
            }
            /// Insert a null-terminated character string at the given index.
            StringElement& insert(size_type index, char const* s)
            {
                impl_.insert(index, s);
                return *this;
            }
            /** Insert the first count characters from a null-terminated
             * character string at the given index.
             */
            StringElement& insert(size_type index, char const* s,
                    size_type count)
            {
                impl_.insert(index, s, count);
                return *this;
            }
            /// Insert the given string at the given index.
            StringElement& insert(size_type index, std::string const& str)
            {
                impl_.insert(index, str);
                return *this;
            }
            /// Insert a substring at the given index.
            StringElement& insert(size_type index, std::string const& str,
                    size_type index_str, size_type count)
            {
                impl_.insert(index, str, index_str, count);
                return *this;
            }
#if !defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Insert characters from the range [first, last) before the
             * character pointed to by pos.
             */
            template<class InputIterator>
            void insert(iterator i, InputIterator first, InputIterator last)
            {
                impl_.insert(i, first, last);
            }
#endif // !defined(CELDUIN_CPLUSPLUS11_SUPPORT)
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Insert characters from the range [first, last) before the
             * character pointed to by pos.
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
            /** \brief Insert characters from the initialiser list ilist.
             *
             * Requires C++11 support.
             */
            iterator insert(const_iterator pos,
                    std::initializer_list<char> ilist)
            {
                return impl_.insert(pos, ilist);
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

            /// \brief Removes count characters starting at index.
            StringElement& erase(size_type index=0,
                    size_type count=std::string::npos)
            {
                impl_.erase(index, count);
                return *this;
            }
#if !defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /// \brief Removes the character at pos.
            iterator erase(iterator pos)
            {
                return impl_.erase(pos);
            }
#endif // !defined(CELDUIN_CPLUSPLUS11_SUPPORT)
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Removes the character at pos.
             *
             * Requires C++11 support.
             */
            iterator erase(const_iterator pos)
            {
                return impl_.erase(pos);
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)
#if !defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /// \brief Removes the characters in the range [first, last).
            iterator erase(iterator first, iterator last)
            {
                return impl_.erase(first, last);
            }
#endif // !defined(CELDUIN_CPLUSPLUS11_SUPPORT)
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Removes the characters in the range [first, last).
             *
             * Requires C++11 support.
             */
            iterator erase(const_iterator first, const_iterator last)
            {
                return impl_.erase(first, last);
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

            /** \brief Appends the given character to the end of the string.
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
            /** \brief Removes the last character of the string.
             *
             * No iterators or references are invalidated.
             */
            void pop_back()
            {
                impl_.pop_back();
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

            /// \brief Append count copies of the character.
            StringElement& append(size_type count, char const& ch)
            {
                impl_.append(count, ch);
                return *this;
            }
            /// \brief Append a string.
            StringElement& append(std::string const& str)
            {
                impl_.append(str);
                return *this;
            }
            /** \brief Append a substring.
             *
             * \exception std::out_of_range if pos >= str.size().
             */
            StringElement& append(std::string const& str, size_type pos,
                    size_type count)
            {
                impl_.append(str, pos, count);
                return *this;
            }
            /// \brief Append the first count characters of a character string.
            StringElement& append(char const* s, size_type count)
            {
                impl_.append(s, count);
                return *this;
            }
            /// \brief Append a null-terminated character string.
            StringElement& append(char const* s)
            {
                impl_.append(s);
                return *this;
            }
            /// \brief Append characters in the range [first, last).
            template<class InputIterator>
            StringElement& append(InputIterator first, InputIterator last)
            {
                impl_.append(first, last);
                return *this;
            }
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Appends the characters in the initialiser list.
             *
             * Requires C++11 support.
             */
            StringElement& append(std::initializer_lst<char> ilist)
            {
                impl_.append(ilist);
                return *this;
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

            /// \brief Compares this string to str.
            int compare(std::string const& str) const
            {
                return impl_.compare(str);
            }
            /// \brief Compares a substring of this string to str.
            int compare(size_type pos1, size_type count1,
                    std::string const& str) const
            {
                return impl_.compare(pos1, count1, str);
            }
            /** \brief Compares a substring of this string to a substring of
             * str.
             */
            int compare(size_type pos1, size_type count1,
                    std::string const& str, size_type pos2,
                    size_type count2) const
            {
                return impl_.compare(pos1, count1, str, pos2, count2);
            }
            /// \brief Compares this string to s.
            int compare(char const* s) const
            {
                return impl_.compare(s);
            }
            /// \brief Compares a substring of this string to s.
            int compare(size_type pos1, size_type count1,
                    char const* s) const
            {
                return impl_.compare(pos1, count1, s);
            }
            /** \brief Compares a substring of this string to a substring of
             * s.
             */
            int compare(size_type pos1, size_type count1,
                    char const* s, size_type count2) const
            {
                return impl_.compare(pos1, count1, s, count2);
            }

            /// \brief Replace the indicated substring with str.
            StringElement& replace(size_type pos, size_type count,
                    std::string const& str)
            {
                impl_.replace(pos, count, str);
                return *this;
            }
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /// \brief Replace the indicated substring with str.
            StringElement& replace(const_iterator first, const_iterator last,
                    std::string const& str)
            {
                impl_.replace(first, last, str);
                return *this;
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /// \brief Replace the indicated substring with a substring of str.
            StringElement& replace(size_type pos, size_type count,
                    std::string const& str, size_type pos2, size_type count2)
            {
                impl_.replace(pos, count, str, pos2, count2);
                return *this;
            }
            /// \brief Replace the indicated substring with a substring of str.
            template<class InputIterator>
            StringElement& replace(const_iterator first, const_iterator last,
                    InputIterator first2, InputIterator last2)
            {
                impl_.replace(first, last, first2, last2);
                return *this;
            }
            /** \brief Replace the indicated substring with the first count2
             * characters of cstr.
             */
            StringElement& replace(size_type pos, size_type count,
                    char const* cstr, size_type count2)
            {
                impl_.replace(pos, count, cstr, count2);
                return *this;
            }
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Replace the indicated substring with the first count2
             * characters of cstr.
             */
            StringElement& replace(const_iterator first, const_iterator last,
                    char const* cstr, size_type count2)
            {
                impl_.replace(first, last, cstr, count2);
                return *this;
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /// \brief Replace the indicated substring with cstr.
            StringElement& replace(size_type pos, size_type count,
                    char const* cstr)
            {
                impl_.replace(pos, count, cstr);
                return *this;
            }
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /// \brief Replace the indicated substring with cstr.
            StringElement& replace(const_iterator first, const_iterator last,
                    char const* cstr)
            {
                impl_.replace(first, last, cstr);
                return *this;
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Replace the indicated substring with count2 copies of
             * the given character.
             */
            StringElement& replace(size_type pos, size_type count,
                    size_type count2, char ch)
            {
                impl_.replace(pos, count, count2, ch);
                return *this;
            }
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Replace the indicated substring with count2 copies of
             * the given character.
             */
            StringElement& replace(const_iterator first, const_iterator last,
                    size_type count2, char ch)
            {
                impl_.replace(first, last, count2, ch);
                return *this;
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
            /** \brief Replace the indicated substring with the characters in
             * the initialiser list.
             */
            StringElement& replace(const_iterator first, const_iterator last,
                    std::initializer_list<char> ilist)
            {
                impl_.replace(first, last, ilist);
                return *this;
            }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

            /** \brief Get a substring of the string.
             *
             * \exception std::out_of_range if pos > size().
             */
            std::string substr(size_type pos=0,
                    size_type count=std::string::npos)
            {
                return impl_.substr(pos, count);
            }

            /** \brief Copy the indicated substring to the character string
             * pointed to by dest.
             *
             * \exception std::out_of_range if pos > size().
             */
            size_type copy(char* dest, size_type count, size_type pos=0)
            {
                return impl_.copy(dest, count, pos);
            }

            /** \brief Resizes the string to contain count characters.
             *
             * If the current size is less than count, additional characters are
             * appended and initialized with char().
             *
             * If the current size is greater than count, the string is
             * reduced to its first count characters.
             *
             * Requires C++11 support.
             */
            void resize(size_type count)
            {
                impl_.resize(count);
            }
            /** \brief Resizes the string to contain count characters.
             *
             * If the current size is less than count, additional characters are
             * appended and initialized with value.
             *
             * If the current size is greater than count, the string is
             * reduced to its first count characters.
             */
            void resize(size_type count, char value)
            {
                impl_.resize(count, value);
            }

            /** \brief Finds the first substring equal to the given character
             * sequence.
             *
             * \return The position of the first character of the found
             * substring or npos if no such substring is found.
             */
            size_type find(std::string const& str, size_type pos=0)
            {
                return impl_.find(str, pos);
            }
            /** \brief Finds the first substring equal to the first count
             * characters in the given character sequence.
             *
             * \return The position of the first character of the found
             * substring or npos if no such substring is found.
             */
            size_type find(char const* s, size_type pos, size_type count)
            {
                return impl_.find(s, pos, count);
            }
            /** \brief Finds the first substring equal to the given character
             * sequence.
             *
             * \return The position of the first character of the found
             * substring or npos if no such substring is found.
             */
            size_type find(char const* s, size_type pos=0)
            {
                return impl_.find(s, pos);
            }
            /** \brief Finds the first occurrence of the character ch.
             *
             * \return The position of the character or npos if not found.
             */
            size_type find(char ch, size_type pos=0)
            {
                return impl_.find(ch, pos);
            }

            /** \brief Finds the last substring equal to the given character
             * sequence.
             *
             * \return The position of the last character of the found
             * substring or npos if no such substring is found.
             */
            size_type rfind(std::string const& str,
                    size_type pos=std::string::npos)
            {
                return impl_.rfind(str, pos);
            }
            /** \brief Finds the last substring equal to the last count
             * characters in the given character sequence.
             *
             * \return The position of the last character of the found
             * substring or npos if no such substring is found.
             */
            size_type rfind(char const* s, size_type pos, size_type count)
            {
                return impl_.rfind(s, pos, count);
            }
            /** \brief Finds the last substring equal to the given character
             * sequence.
             *
             * \return The position of the last character of the found
             * substring or npos if no such substring is found.
             */
            size_type rfind(char const* s, size_type pos=std::string::npos)
            {
                return impl_.rfind(s, pos);
            }
            /** \brief Finds the last occurrence of the character ch.
             *
             * \return The position of the character or npos if not found.
             */
            size_type rfind(char ch, size_type pos=std::string::npos)
            {
                return impl_.rfind(ch, pos);
            }

            /** \brief Finds the first character equal to one of the characters
             * in the given character sequence.
             *
             * \return The position of the found character or npos if not
             * found.
             */
            size_type find_first_of(std::string const& str, size_type pos=0)
            {
                return impl_.find_first_of(str, pos);
            }
            /** \brief Finds the first character equal to one of the characters
             * in the given character sequence.
             *
             * \return The position of the found character or npos if not
             * found.
             */
            size_type find_first_of(char const* s, size_type pos,
                    size_type count)
            {
                return impl_.find_first_of(s, pos, count);
            }
            /** \brief Finds the first character equal to one of the characters
             * in the given character sequence.
             *
             * \return The position of the found character or npos if not
             * found.
             */
            size_type find_first_of(char const* s, size_type pos=0)
            {
                return impl_.find_first_of(s, pos);
            }
            /** \brief Finds the first character equal to ch.
             *
             * \return The position of the found character or npos if not
             * found.
             */
            size_type find_first_of(char ch, size_type pos=0)
            {
                return impl_.find_first_of(ch, pos);
            }

            /** \brief Finds the first character not equal to one of the
             * characters in the given character sequence.
             *
             * \return The position of the found character or npos if not
             * found.
             */
            size_type find_first_not_of(std::string const& str,
                    size_type pos=0)
            {
                return impl_.find_first_not_of(str, pos);
            }
            /** \brief Finds the first character not equal to one of the
             * characters in the given character sequence.
             *
             * \return The position of the found character or npos if not
             * found.
             */
            size_type find_first_not_of(char const* s, size_type pos,
                    size_type count)
            {
                return impl_.find_first_not_of(s, pos, count);
            }
            /** \brief Finds the first character not equal to one of the
             * characters in the given character sequence.
             *
             * \return The position of the found character or npos if not
             * found.
             */
            size_type find_first_not_of(char const* s, size_type pos=0)
            {
                return impl_.find_first_not_of(s, pos);
            }
            /** \brief Finds the first character not equal to ch.
             *
             * \return The position of the found character or npos if not
             * found.
             */
            size_type find_first_not_of(char ch, size_type pos=0)
            {
                return impl_.find_first_not_of(ch, pos);
            }

            /** \brief Finds the last character equal to one of the characters
             * in the given character sequence.
             *
             * \return The position of the found character or npos if not
             * found.
             */
            size_type find_last_of(std::string const& str,
                    size_type pos=std::string::npos)
            {
                return impl_.find_last_of(str, pos);
            }
            /** \brief Finds the last character equal to one of the characters
             * in the given character sequence.
             *
             * \return The position of the found character or npos if not
             * found.
             */
            size_type find_last_of(char const* s, size_type pos,
                    size_type count)
            {
                return impl_.find_last_of(s, pos, count);
            }
            /** \brief Finds the last character equal to one of the characters
             * in the given character sequence.
             *
             * \return The position of the found character or npos if not
             * found.
             */
            size_type find_last_of(char const* s,
                    size_type pos=std::string::npos)
            {
                return impl_.find_last_of(s, pos);
            }
            /** \brief Finds the last character equal to ch.
             *
             * \return The position of the found character or npos if not
             * found.
             */
            size_type find_last_of(char ch,
                    size_type pos=std::string::npos)
            {
                return impl_.find_last_of(ch, pos);
            }

            /** \brief Finds the last character not equal to one of the
             * characters in the given character sequence.
             *
             * \return The position of the found character or npos if not
             * found.
             */
            size_type find_last_not_of(std::string const& str,
                    size_type pos=std::string::npos)
            {
                return impl_.find_last_not_of(str, pos);
            }
            /** \brief Finds the last character not equal to one of the
             * characters in the given character sequence.
             *
             * \return The position of the found character or npos if not
             * found.
             */
            size_type find_last_not_of(char const* s, size_type pos,
                    size_type count)
            {
                return impl_.find_last_not_of(s, pos, count);
            }
            /** \brief Finds the last character not equal to one of the
             * characters in the given character sequence.
             *
             * \return The position of the found character or npos if not
             * found.
             */
            size_type find_last_not_of(char const* s,
                    size_type pos=std::string::npos)
            {
                return impl_.find_last_not_of(s, pos);
            }
            /** \brief Finds the last character not equal to ch.
             *
             * \return The position of the found character or npos if not
             * found.
             */
            size_type find_last_not_of(char ch,
                    size_type pos=std::string::npos)
            {
                return impl_.find_last_not_of(ch, pos);
            }

        private:
            ///////////////////////////////////////////////////////////////////
            // PrimitiveBase interface implementation
            impl::StringElementImpl impl_;

            // TODO: Fix the PrimitiveElement virtual functions to use
            // references automatically where appropriate.
            virtual std::string value_impl() const
            {
                return impl_.value();
            }

            // TODO: Fix the PrimitiveElement virtual functions to use
            // references automatically where appropriate.
            virtual void value_impl(std::string value)
            {
                impl_.value(value);
            }

            virtual bool has_default_impl() const
            {
                return impl_.has_default();
            }

            // TODO: Fix the PrimitiveElement virtual functions to use
            // references automatically where appropriate.
            virtual std::string get_default_impl() const
            {
                return impl_.get_default();
            }

            // TODO: Fix the PrimitiveElement virtual functions to use
            // references automatically where appropriate.
            virtual void set_default_impl(std::string default_value)
            {
                impl_.set_default(default_value);
            }

            // TODO: Fix the PrimitiveElement virtual functions to use
            // references automatically where appropriate.
            virtual std::string remove_default_impl()
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
    }; // class StringElement


    /// \brief Swap two binary elements.
    void swap(StringElement& a, StringElement& b);


    /** \brief Swap this element's value with another instance of the
     * value type.
     *
     * Only the value is swapped. The ID and default value are left
     * unchanged.
     */
    void swap(StringElement& a, std::string& b);


    /** \brief Addition operator.
     *
     * Cannot be provided by boost::addable due to being non-commutative.
     */
    StringElement operator+(StringElement lhs,
            StringElement const& rhs)
    {
        lhs += rhs;
        return lhs;
    }
    /** \brief Addition operator.
     *
     * Cannot be provided by boost::addable due to being non-commutative.
     */
    StringElement operator+(StringElement lhs, std::string const& rhs)
    {
        lhs += rhs;
        return lhs;
    }
    /** \brief Addition operator.
     *
     * Cannot be provided by boost::addable due to being non-commutative.
     */
    StringElement operator+(std::string const& lhs, StringElement rhs)
    {
        rhs.insert(0, lhs);
        return rhs;
    }
    /** \brief Addition operator.
     *
     * Cannot be provided by boost::addable due to being non-commutative.
     */
    StringElement operator+(StringElement lhs, char const* const rhs)
    {
        lhs += rhs;
        return lhs;
    }
    /** \brief Addition operator.
     *
     * Cannot be provided by boost::addable due to being non-commutative.
     */
    StringElement operator+(char const* const lhs, StringElement rhs)
    {
        rhs.insert(0, lhs);
        return rhs;
    }
    /** \brief Addition operator.
     *
     * Cannot be provided by boost::addable due to being non-commutative.
     */
    StringElement operator+(StringElement lhs, char rhs)
    {
        lhs += rhs;
        return lhs;
    }
    /** \brief Addition operator.
     *
     * Cannot be provided by boost::addable due to being non-commutative.
     */
    StringElement operator+(char lhs, StringElement rhs)
    {
        rhs.insert(0, 1, lhs);
        return rhs;
    }


    /// \brief Stream output operator.
    std::ostream& operator<<(std::ostream& o, StringElement const& rhs)
    {
        return o << rhs.impl_;
    }


    /// \brief Stream input operator.
    std::istream& operator>>(std::istream& i, StringElement& rhs)
    {
        std::string temp;
        i >> temp;
        rhs.value(temp);
        return i;
    }
}; // namespace celduin

/// @}
// group interfaces

#endif // !defined(STRING_ELEMENT_H_)

