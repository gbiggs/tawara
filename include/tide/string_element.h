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

#include <tide/string_element_impl.h>
#include <tide/element_base.h>
#include <tide/primitive_element.h>
#include <tide/win_dll.h>

#include <boost/operators.hpp>
#include <iosfwd>
#include <string>

/// \addtogroup interfaces Interfaces
/// @{

namespace tide
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
    class TIDE_EXPORT StringElement
        : public ElementBase<StringElement>,
        public PrimitiveElement<std::string>,
        boost::totally_ordered<StringElement>,
        boost::totally_ordered<StringElement, std::string>,
        boost::equality_comparable<StringElement>,
        boost::equality_comparable<StringElement, std::string>,
        boost::addable<StringElement>,
        boost::addable<StringElement, std::string>
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
            StringElement(tide::ids::ID id, std::string const& value,
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


    /// \brief Stream output operator.
    std::ostream& operator<<(std::ostream& o, StringElement const& rhs)
    {
        return o << rhs.impl_;
    }
}; // namespace tide

/// @}
// group interfaces

#endif // !defined(STRING_ELEMENT_H_)

