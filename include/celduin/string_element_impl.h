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

#if !defined(STRING_ELEMENT_IMPL_H_)
#define STRING_ELEMENT_IMPL_H_

#include <celduin/primitive_element_base.h>
#include <celduin/celduin_config.h>

#include <iosfwd>
#include <string>

/// \addtogroup implementations Internal implementations
/// @{

namespace celduin
{
    namespace impl
    {
        /** \brief Internal implementation of the string primitive element.
         *
         * This class implements the PrimitiveElementBase CRTP requirements.
         */
        class StringElementImpl
            : public PrimitiveElementBase<StringElementImpl, std::string>
        {
            friend class PrimitiveElementBase<StringElementImpl, std::string>;

            public:
                // Export the underlying type's typedefs
                typedef std::string::traits_type traits_type;
                typedef std::string::value_type value_type;
                typedef std::string::allocator_type allocator_type;
                typedef std::string::size_type size_type;
                typedef std::string::difference_type difference_type;
                typedef std::string::reference reference;
                typedef std::string::const_reference const_reference;
                typedef std::string::pointer pointer;
                typedef std::string::const_pointer const_pointer;
                typedef std::string::iterator iterator;
                typedef std::string::const_iterator const_iterator;
                typedef std::string::reverse_iterator reverse_iterator;
                typedef std::string::const_reverse_iterator
                    const_reverse_iterator;

                /** \brief Constructor.
                 *
                 * \param[in] value The value of the element.
                 */
                StringElementImpl(std::string const& value);

                /** \brief Constructor.
                 *
                 * \param[in] value The value of the element.
                 * \param[in] default_val The default value of the element.
                 */
                StringElementImpl(std::string const& value,
                        std::string const& default_val);

                /** \brief Swap this element with another.
                 *
                 * \param[in] other The other element to swap with.
                 */
                void swap(StringElementImpl& other);

                /** \brief Swap this element's value with another instance of
                 * the value type.
                 *
                 * Only the value is swapped. The ID and default value are left
                 * unchanged.
                 *
                 * \param[in] other The other value to swap with.
                 */
                void swap(std::string& other);

                /// \brief Less-than comparison operator.
                bool operator<(StringElementImpl const& rhs)
                {
                    return value_ < rhs.value_;
                }

                /// \brief Less-than comparison operator.
                bool operator<(std::string const& rhs)
                {
                    return value_ < rhs;
                }

                /// \brief Greater-than comparison operator.
                bool operator>(std::string const& rhs)
                {
                    return value_ > rhs;
                }

                /// \brief Assignment addition operator.
                StringElementImpl& operator+=(StringElementImpl const& rhs)
                {
                    value_ += rhs.value_;
                    return *this;
                }
                /// \brief Assignment addition operator.
                StringElementImpl& operator+=(std::string rhs)
                {
                    value_ += rhs;
                    return *this;
                }
                /// \brief Assignment addition operator.
                StringElementImpl& operator+=(char ch)
                {
                    value_ += ch;
                    return *this;
                }
                /// \brief Assignment addition operator.
                StringElementImpl& operator+=(char const* s)
                {
                    value_ += s;
                    return *this;
                }
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
                /// \brief Assignment addition operator.
                StringElementImpl& operator+=(std::initializer_list<char> ilist)
                {
                    value_ += ilist;
                    return *this;
                }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

                /// \brief Get the amount of padding used.
                unsigned int padding() const
                {
                    return padding_;
                }
                /// \brief Set the amount of padding to use.
                void padding(unsigned int padding)
                {
                    padding_ = padding;
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
                        StringElementImpl const& rhs);

                ///////////////////////////////////////////////////////////////////
                // String interface member functions

                /// \brief Replace the contents of the string.
                StringElementImpl& assign(size_type count, char value)
                {
                    value_.assign(count, value);
                    return *this;
                }
                /// \brief Replace the contents of the string.
                StringElementImpl& assign(std::string const& str)
                {
                    value_.assign(str);
                    return *this;
                }
                /// \brief Replace the contents of the string.
                StringElementImpl& assign(std::string const& str,
                        size_type pos, size_type count)
                {
                    value_.assign(str, pos, count);
                    return *this;
                }
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
                /** \brief Replace the contents of the string.
                 *
                 * Requires C++11 support.
                 */
                StringElementImpl& assign(std::string&& str)
                {
                    value_.assign(str);
                    return *this;
                }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)
                /// \brief Replace the contents of the string.
                StringElementImpl& assign(char const* s, size_type count)
                {
                    value_.assign(s, count);
                    return *this;
                }
                /// \brief Replace the contents of the string.
                StringElementImpl& assign(char const* s)
                {
                    value_.assign(s);
                    return *this;
                }
                /// \brief Replace the contents of the string.
                template<class InputIterator>
                StringElementImpl& assign(InputIterator first,
                        InputIterator last)
                {
                    value_.assign(first, last);
                    return *this;
                }
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
                /** \brief Replace the contents of the string.
                 *
                 * Requires C++11 support.
                 */
                StringElementImpl& assign(std::initializer_list<char> ilist)
                {
                    value_.assign(ilist);
                    return *this;
                }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

                /// \brief Get the allocator associated with the string.
                allocator_type get_allocator() const
                {
                    return value_.get_allocator();
                }

                /** \brief Get a reference to the character at the specified
                 * location.
                 */
                reference at(size_type pos)
                {
                    return value_.at(pos);
                }
                /** \brief Get a reference to the character at the specified
                 * location.
                 *
                 * \exception std::out_of_range if the location is invalid.
                 */
                const_reference at(size_type pos) const
                {
                    return value_.at(pos);
                }

                /** \brief Get a reference to the character at the specified
                 * location.
                 *
                 * No bounds checking is performed.
                 */
                reference operator[](size_type pos)
                {
                    return value_[pos];
                }
                /** \brief Get a reference to the character at the specified
                 * location.
                 *
                 * No bounds checking is performed.
                 */
                const_reference operator[](size_type pos) const
                {
                    return value_[pos];
                }

#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
                /** \brief Get a reference to the first character.
                 *
                 * Requires C++11 support.
                 */
                reference front()
                {
                    return value_.front();
                }
                /** \brief Get a reference to the first character.
                 *
                 * Requires C++11 support.
                 */
                const_reference front() const
                {
                    return value_.front();
                }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
                /** \brief Get a reference to the last character.
                 *
                 * Requires C++11 support.
                 */
                reference back()
                {
                    return value_.back();
                }
                /** \brief Get a reference to the last character.
                 *
                 * Requires C++11 support.
                 */
                const_reference back() const
                {
                    return value_.back();
                }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

                /// \brief Get a pointer to the underlying array.
                char const* data() const
                {
                    return value_.data();
                }

                /** \brief Get a pointer to a null-terminated character array
                 * with the equivalent data to that stored in the string.
                 */
                char const* c_str() const
                {
                    return value_.c_str();
                }

                /** \brief Get an iterator to the first character of the
                 * string.
                 */
                iterator begin()
                {
                    return value_.begin();
                }
                /** \brief Get an iterator to the first character of the
                 * string.
                 */
                const_iterator begin() const
                {
                    return value_.begin();
                }
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
                /** \brief Get an iterator to the first character of the
                 * string.
                 *
                 * Requires C++11 support.
                 */
                const_iterator cbegin() const
                {
                    return value_.cbegin();
                }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

                /** \brief Get an iterator to the character following the last
                 * character of the string.
                 */
                iterator end()
                {
                    return value_.end();
                }
                /** \brief Get an iterator to the character following the last
                 * character of the string.
                 */
                const_iterator end() const
                {
                    return value_.end();
                }
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
                /** \brief Get an iterator to the character following the last
                 * character of the string.
                 *
                 * Requires C++11 support.
                 */
                const_iterator cend() const
                {
                    return value_.cend();
                }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

                /** \brief Get a reverse iterator to the first character of the
                 * reversed string.
                 */
                reverse_iterator rbegin()
                {
                    return value_.rbegin();
                }
                /** \brief Get a reverse iterator to the first character of the
                 * reversed string.
                 */
                const_reverse_iterator rbegin() const
                {
                    return value_.rbegin();
                }
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
                /** \brief Get a reverse iterator to the first character of the
                 * reversed string.
                 *
                 * Requires C++11 support.
                 */
                const_reverse_iterator crbegin() const
                {
                    return value_.crbegin();
                }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

                /** \brief Get a reverse iterator to the character following the
                 * last character of the reversed string.
                 */
                reverse_iterator rend()
                {
                    return value_.rend();
                }
                /** \brief Get a reverse iterator to the character following the
                 * last character of the reversed string.
                 */
                const_reverse_iterator rend() const
                {
                    return value_.rend();
                }
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
                /** \brief Get a reverse iterator to the character following the
                 * last character of the reversed string.
                 *
                 * Requires C++11 support.
                 */
                const_reverse_iterator crend() const
                {
                    return value_.crend();
                }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

                /// \brief Check if the string is empty.
                bool empty() const
                {
                    return value_.empty();
                }

                /// \brief Get the number of characters in the string.
                size_type size() const
                {
                    return value_.size();
                }

                /// \brief Get the number of characters in the string.
                size_type length() const
                {
                    return value_.length();
                }

                /** \brief Get the maximum number of characters the string can
                 * hold.
                 */
                size_type max_size() const
                {
                    return value_.max_size();
                }

                /** \brief Set the capacity of the string to at least size,
                 * allocating new storage if necessary.
                 */
                void reserve(size_type size)
                {
                    value_.reserve(size);
                }

                /** \brief Get the number of characters that the string has
                 * currently allocated space for.
                 */
                size_type capacity() const
                {
                    return value_.capacity();
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
                    value_.shrink_to_fit();
                }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

                /** \brief Remove all characters from the string.
                 *
                 * The allocated memory will not be released.
                 */
                void clear()
                {
                    value_.clear();
                }

#if !defined(CELDUIN_CPLUSPLUS11_SUPPORT)
                /// \brief Insert value before the character pointed to by pos.
                iterator insert(iterator pos, char value)
                {
                    return value_.insert(pos, value);
                }
#endif // !defined(CELDUIN_CPLUSPLUS11_SUPPORT)
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
                /** \brief Insert value before the character pointed to by pos.
                 *
                 * Requires C++11 support.
                 */
                iterator insert(const_iterator pos, char value)
                {
                    return value_.insert(pos, value);
                }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)
                /** \brief Insert count copies of the value before the
                 * character pointed to by pos.
                 */
                void insert(iterator pos, size_type count, char value)
                {
                    value_.insert(pos, count, value);
                }
                /// Insert count copies of the value at the given index.
                StringElementImpl& insert(size_type index, size_type count,
                        char value)
                {
                    value_.insert(index, count, value);
                    return *this;
                }
                /// Insert a null-terminated character string at the given index.
                StringElementImpl& insert(size_type index, char const* s)
                {
                    value_.insert(index, s);
                    return *this;
                }
                /** Insert the first count characters from a null-terminated
                 * character string at the given index.
                 */
                StringElementImpl& insert(size_type index, char const* s,
                        size_type count)
                {
                    value_.insert(index, s, count);
                    return *this;
                }
                /// Insert the given string at the given index.
                StringElementImpl& insert(size_type index,
                        std::string const& str)
                {
                    value_.insert(index, str);
                    return *this;
                }
                /// Insert a substring at the given index.
                StringElementImpl& insert(size_type index,
                        std::string const& str, size_type index_str,
                        size_type count)
                {
                    value_.insert(index, str, index_str, count);
                    return *this;
                }
#if !defined(CELDUIN_CPLUSPLUS11_SUPPORT)
                /** \brief Insert characters from the range [first, last)
                 * before the character pointed to by pos.
                 */
                template<class InputIterator>
                void insert(iterator i, InputIterator first,
                        InputIterator last)
                {
                    value_.insert(i, first, last);
                }
#endif // !defined(CELDUIN_CPLUSPLUS11_SUPPORT)
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
                /** \brief Insert characters from the range [first, last)
                 * before the character pointed to by pos.
                 *
                 * Requires C++11 support.
                 */
                template<class InputIterator>
                iterator insert(const_iterator pos, InputIterator first,
                        InputIterator last)
                {
                    return value_.insert(pos, first, last);
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
                    return value_.insert(pos, ilist);
                }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

                /// \brief Removes count characters starting at index.
                StringElementImpl& erase(size_type index=0,
                        size_type count=std::string::npos)
                {
                    value_.erase(index, count);
                    return *this;
                }
#if !defined(CELDUIN_CPLUSPLUS11_SUPPORT)
                /// \brief Removes the character at pos.
                iterator erase(iterator pos)
                {
                    return value_.erase(pos);
                }
#endif // !defined(CELDUIN_CPLUSPLUS11_SUPPORT)
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
                /** \brief Removes the character at pos.
                 *
                 * Requires C++11 support.
                 */
                iterator erase(const_iterator pos)
                {
                    return value_.erase(pos);
                }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)
#if !defined(CELDUIN_CPLUSPLUS11_SUPPORT)
                /// \brief Removes the characters in the range [first, last).
                iterator erase(iterator first, iterator last)
                {
                    return value_.erase(first, last);
                }
#endif // !defined(CELDUIN_CPLUSPLUS11_SUPPORT)
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
                /** \brief Removes the characters in the range [first, last).
                 *
                 * Requires C++11 support.
                 */
                iterator erase(const_iterator first, const_iterator last)
                {
                    return value_.erase(first, last);
                }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

                /** \brief Appends the given character to the end of the
                 * string.
                 *
                 * If size() is not larger than capacity(), no iterators or
                 * references are invalidated. Otherwise all iterators and
                 * references are invalidated.
                 */
                void push_back(char value)
                {
                    return value_.push_back(value);
                }

#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
                /** \brief Removes the last character of the string.
                 *
                 * No iterators or references are invalidated.
                 */
                void pop_back()
                {
                    value_.pop_back();
                }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

                /// \brief Append count copies of the character.
                StringElementImpl& append(size_type count, char const& ch)
                {
                    value_.append(count, ch);
                    return *this;
                }
                /// \brief Append a string.
                StringElementImpl& append(std::string const& str)
                {
                    value_.append(str);
                    return *this;
                }
                /** \brief Append a substring.
                 *
                 * \exception std::out_of_range if pos >= str.size().
                 */
                StringElementImpl& append(std::string const& str,
                        size_type pos, size_type count)
                {
                    value_.append(str, pos, count);
                    return *this;
                }
                /** \brief Append the first count characters of a character
                 * string.
                 */
                StringElementImpl& append(char const* s, size_type count)
                {
                    value_.append(s, count);
                    return *this;
                }
                /// \brief Append a null-terminated character string.
                StringElementImpl& append(char const* s)
                {
                    value_.append(s);
                    return *this;
                }
                /// \brief Append characters in the range [first, last).
                template<class InputIterator>
                StringElementImpl& append(InputIterator first,
                        InputIterator last)
                {
                    value_.append(first, last);
                    return *this;
                }
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
                /** \brief Appends the characters in the initialiser list.
                 *
                 * Requires C++11 support.
                 */
                StringElementImpl& append(std::initializer_lst<char> ilist)
                {
                    value_.append(ilist);
                    return *this;
                }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)

                /// \brief Compares this string to str.
                int compare(std::string const& str) const
                {
                    return value_.compare(str);
                }
                /// \brief Compares a substring of this string to str.
                int compare(size_type pos1, size_type count1,
                        std::string const& str) const
                {
                    return value_.compare(pos1, count1, str);
                }
                /** \brief Compares a substring of this string to a substring
                 * of str.
                 */
                int compare(size_type pos1, size_type count1,
                        std::string const& str, size_type pos2,
                        size_type count2) const
                {
                    return value_.compare(pos1, count1, str, pos2, count2);
                }
                /// \brief Compares this string to s.
                int compare(char const* s) const
                {
                    return value_.compare(s);
                }
                /// \brief Compares a substring of this string to s.
                int compare(size_type pos1, size_type count1,
                        char const* s) const
                {
                    return value_.compare(pos1, count1, s);
                }
                /** \brief Compares a substring of this string to a substring
                 * of s.
                 */
                int compare(size_type pos1, size_type count1,
                        char const* s, size_type count2) const
                {
                    return value_.compare(pos1, count1, s, count2);
                }

                /// \brief Replace the indicated substring with str.
                StringElementImpl& replace(size_type pos, size_type count,
                        std::string const& str)
                {
                    value_.replace(pos, count, str);
                    return *this;
                }
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
                /// \brief Replace the indicated substring with str.
                StringElementImpl& replace(const_iterator first,
                        const_iterator last, std::string const& str)
                {
                    value_.replace(first, last, str);
                    return *this;
                }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)
                /** \brief Replace the indicated substring with a substring of
                 * str.
                 */
                StringElementImpl& replace(size_type pos, size_type count,
                        std::string const& str, size_type pos2,
                        size_type count2)
                {
                    value_.replace(pos, count, str, pos2, count2);
                    return *this;
                }
                /** \brief Replace the indicated substring with a substring of
                 * str.
                 */
                template<class InputIterator>
                StringElementImpl& replace(const_iterator first,
                        const_iterator last, InputIterator first2,
                        InputIterator last2)
                {
                    value_.replace(first, last, first2, last2);
                    return *this;
                }
                /** \brief Replace the indicated substring with the first
                 * count2 characters of cstr.
                 */
                StringElementImpl& replace(size_type pos, size_type count,
                        char const* cstr, size_type count2)
                {
                    value_.replace(pos, count, cstr, count2);
                    return *this;
                }
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
                /** \brief Replace the indicated substring with the first
                 * count2 characters of cstr.
                 */
                StringElementImpl& replace(const_iterator first,
                        const_iterator last, char const* cstr,
                        size_type count2)
                {
                    value_.replace(first, last, cstr, count2);
                    return *this;
                }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)
                /// \brief Replace the indicated substring with cstr.
                StringElementImpl& replace(size_type pos, size_type count,
                        char const* cstr)
                {
                    value_.replace(pos, count, cstr);
                    return *this;
                }
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
                /// \brief Replace the indicated substring with cstr.
                StringElementImpl& replace(const_iterator first,
                        const_iterator last, char const* cstr)
                {
                    value_.replace(first, last, cstr);
                    return *this;
                }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)
                /** \brief Replace the indicated substring with count2 copies
                 * of the given character.
                 */
                StringElementImpl& replace(size_type pos, size_type count,
                        size_type count2, char ch)
                {
                    value_.replace(pos, count, count2, ch);
                    return *this;
                }
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
                /** \brief Replace the indicated substring with count2 copies
                 * of the given character.
                 */
                StringElementImpl& replace(const_iterator first,
                        const_iterator last, size_type count2, char ch)
                {
                    value_.replace(first, last, count2, ch);
                    return *this;
                }
#endif // defined(CELDUIN_CPLUSPLUS11_SUPPORT)
#if defined(CELDUIN_CPLUSPLUS11_SUPPORT)
                /** \brief Replace the indicated substring with the characters
                 * in the initialiser list.
                 */
                StringElementImpl& replace(const_iterator first,
                        const_iterator last, std::initializer_list<char> ilist)
                {
                    value_.replace(first, last, ilist);
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
                    return value_.substr(pos, count);
                }

                /** \brief Copy the indicated substring to the character string
                 * pointed to by dest.
                 *
                 * \exception std::out_of_range if pos > size().
                 */
                size_type copy(char* dest, size_type count, size_type pos=0)
                {
                    return value_.copy(dest, count, pos);
                }

                /** \brief Resizes the string to contain count characters.
                 *
                 * If the current size is less than count, additional
                 * characters are appended and initialized with char().
                 *
                 * If the current size is greater than count, the string is
                 * reduced to its first count characters.
                 *
                 * Requires C++11 support.
                 */
                void resize(size_type count)
                {
                    value_.resize(count);
                }
                /** \brief Resizes the string to contain count characters.
                 *
                 * If the current size is less than count, additional
                 * characters are appended and initialized with value.
                 *
                 * If the current size is greater than count, the string is
                 * reduced to its first count characters.
                 */
                void resize(size_type count, char value)
                {
                    value_.resize(count, value);
                }

                /** \brief Finds the first substring equal to the given
                 * character sequence.
                 *
                 * \return The position of the first character of the found
                 * substring or npos if no such substring is found.
                 */
                size_type find(std::string const& str, size_type pos=0)
                {
                    return value_.find(str, pos);
                }
                /** \brief Finds the first substring equal to the first count
                 * characters in the given character sequence.
                 *
                 * \return The position of the first character of the found
                 * substring or npos if no such substring is found.
                 */
                size_type find(char const* s, size_type pos, size_type count)
                {
                    return value_.find(s, pos, count);
                }
                /** \brief Finds the first substring equal to the given
                 * character sequence.
                 *
                 * \return The position of the first character of the found
                 * substring or npos if no such substring is found.
                 */
                size_type find(char const* s, size_type pos=0)
                {
                    return value_.find(s, pos);
                }
                /** \brief Finds the first occurrence of the character ch.
                 *
                 * \return The position of the character or npos if not found.
                 */
                size_type find(char ch, size_type pos=0)
                {
                    return value_.find(ch, pos);
                }

                /** \brief Finds the last substring equal to the given
                 * character sequence.
                 *
                 * \return The position of the last character of the found
                 * substring or npos if no such substring is found.
                 */
                size_type rfind(std::string const& str,
                        size_type pos=std::string::npos)
                {
                    return value_.rfind(str, pos);
                }
                /** \brief Finds the last substring equal to the last count
                 * characters in the given character sequence.
                 *
                 * \return The position of the last character of the found
                 * substring or npos if no such substring is found.
                 */
                size_type rfind(char const* s, size_type pos, size_type count)
                {
                    return value_.rfind(s, pos, count);
                }
                /** \brief Finds the last substring equal to the given
                 * character sequence.
                 *
                 * \return The position of the last character of the found
                 * substring or npos if no such substring is found.
                 */
                size_type rfind(char const* s,
                        size_type pos=std::string::npos)
                {
                    return value_.rfind(s, pos);
                }
                /** \brief Finds the last occurrence of the character ch.
                 *
                 * \return The position of the character or npos if not found.
                 */
                size_type rfind(char ch,
                        size_type pos=std::string::npos)
                {
                    return value_.rfind(ch, pos);
                }

                /** \brief Finds the first character equal to one of the
                 * characters in the given character sequence.
                 *
                 * \return The position of the found character or npos if not
                 * found.
                 */
                size_type find_first_of(std::string const& str,
                        size_type pos=0)
                {
                    return value_.find_first_of(str, pos);
                }
                /** \brief Finds the first character equal to one of the
                 * characters in the given character sequence.
                 *
                 * \return The position of the found character or npos if not
                 * found.
                 */
                size_type find_first_of(char const* s, size_type pos,
                        size_type count)
                {
                    return value_.find_first_of(s, pos, count);
                }
                /** \brief Finds the first character equal to one of the
                 * characters in the given character sequence.
                 *
                 * \return The position of the found character or npos if not
                 * found.
                 */
                size_type find_first_of(char const* s, size_type pos=0)
                {
                    return value_.find_first_of(s, pos);
                }
                /** \brief Finds the first character equal to ch.
                 *
                 * \return The position of the found character or npos if not
                 * found.
                 */
                size_type find_first_of(char ch, size_type pos=0)
                {
                    return value_.find_first_of(ch, pos);
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
                    return value_.find_first_not_of(str, pos);
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
                    return value_.find_first_not_of(s, pos, count);
                }
                /** \brief Finds the first character not equal to one of the
                 * characters in the given character sequence.
                 *
                 * \return The position of the found character or npos if not
                 * found.
                 */
                size_type find_first_not_of(char const* s, size_type pos=0)
                {
                    return value_.find_first_not_of(s, pos);
                }
                /** \brief Finds the first character not equal to ch.
                 *
                 * \return The position of the found character or npos if not
                 * found.
                 */
                size_type find_first_not_of(char ch, size_type pos=0)
                {
                    return value_.find_first_not_of(ch, pos);
                }

                /** \brief Finds the last character equal to one of the
                 * characters in the given character sequence.
                 *
                 * \return The position of the found character or npos if not
                 * found.
                 */
                size_type find_last_of(std::string const& str,
                        size_type pos=std::string::npos)
                {
                    return value_.find_last_of(str, pos);
                }
                /** \brief Finds the last character equal to one of the
                 * characters in the given character sequence.
                 *
                 * \return The position of the found character or npos if not
                 * found.
                 */
                size_type find_last_of(char const* s, size_type pos,
                        size_type count)
                {
                    return value_.find_last_of(s, pos, count);
                }
                /** \brief Finds the last character equal to one of the
                 * characters in the given character sequence.
                 *
                 * \return The position of the found character or npos if not
                 * found.
                 */
                size_type find_last_of(char const* s,
                        size_type pos=std::string::npos)
                {
                    return value_.find_last_of(s, pos);
                }
                /** \brief Finds the last character equal to ch.
                 *
                 * \return The position of the found character or npos if not
                 * found.
                 */
                size_type find_last_of(char ch,
                        size_type pos=std::string::npos)
                {
                    return value_.find_last_of(ch, pos);
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
                    return value_.find_last_not_of(str, pos);
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
                    return value_.find_last_not_of(s, pos, count);
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
                    return value_.find_last_not_of(s, pos);
                }
                /** \brief Finds the last character not equal to ch.
                 *
                 * \return The position of the found character or npos if not
                 * found.
                 */
                size_type find_last_not_of(char ch,
                        size_type pos=std::string::npos)
                {
                    return value_.find_last_not_of(ch, pos);
                }

            private:
                ///////////////////////////////////////////////////////////////////
                // PrimitiveElement CRTP required members
                std::string value_;
                std::string default_;
                bool has_default_;
                unsigned int padding_;
                // Must be mutable to be updated in start_body()
                mutable std::streampos body_end_;
        }; // class StringElementImpl


        /// \brief Swap date element implementation objects.
        void swap(StringElementImpl& a, StringElementImpl& b);


        /** \brief Swap this element's value with another instance of the
         * value type.
         *
         * Only the value is swapped. The ID and default value are left
         * unchanged.
         */
        void swap(StringElementImpl& a, std::string& b);


        /// \brief Stream output operator.
        std::ostream& operator<<(std::ostream& o, StringElementImpl const& rhs);


        /// \brief Stream input operator.
        std::istream& operator>>(std::istream& i, StringElementImpl& rhs);
    }; // namespace impl
}; // namespace celduin

/// @}
// group implementations

#endif // !defined(STRING_ELEMENT_IMPL_H_

