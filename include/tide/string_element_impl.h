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

#include <tide/primitive_element_base.h>
#include <tide/tide_config.h>

#include <iosfwd>
#include <string>

/// \addtogroup implementations Internal implementations
/// @{

namespace tide
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
    }; // namespace impl
}; // namespace tide

/// @}
// group implementations

#endif // !defined(STRING_ELEMENT_IMPL_H_

