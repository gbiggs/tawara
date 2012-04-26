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

#include <tide/binary_element_impl.h>
#include <tide/element_base.h>
#include <tide/primitive_element.h>
#include <tide/win_dll.h>

#include <boost/operators.hpp>
#include <iostream>
#include <vector>

/// \addtogroup interfaces Interfaces
/// @{

namespace tide
{
    /** \brief Interface to a binary primitive element.
     *
     * The binary element is the primitive element used to store arbitrary
     * data. This defines the interface to a binary element.
     *
     * This class implements the Element interface.
     *
     * This class implements the PrimitiveElement interface.
     *
     * This class implements the std::vector<char> interface.
     *
     * Copy-construction and assignment are provided by the compiler.
     */
    class TIDE_EXPORT BinaryElement
        : public ElementBase<BinaryElement>,
        public PrimitiveElement<std::vector<char> >,
        boost::totally_ordered<BinaryElement>,
        boost::totally_ordered<BinaryElement, std::vector<char> >,
        boost::equality_comparable<BinaryElement>,
        boost::equality_comparable<BinaryElement, std::vector<char> >
    {
        friend class ElementBase<BinaryElement>;

        public:
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
            BinaryElement(tide::ids::ID id, std::vector<char> const& value,
                    std::vector<char> const& default_val);

            /** \brief Swap this element's value with another's.
             *
             * \param[in] other The other element to swap with.
             */
            void swap(BinaryElement& other);

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

            /// \brief Streaming output oeprator.
            friend std::ostream& operator<<(std::ostream& o,
                    BinaryElement const& rhs);

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


    /// \brief Swap floating point element values.
    void swap(BinaryElement& a, BinaryElement& b)
    {
        a.swap(b);
    }


    /// \brief Stream output operator.
    std::ostream& operator<<(std::ostream& o, BinaryElement const& rhs)
    {
        return o << rhs.impl_;
    }
}; // namespace tide

/// @}
// group interfaces

#endif // !defined(BINARY_ELEMENT_H_

