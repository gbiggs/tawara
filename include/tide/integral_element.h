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

#if !defined(INTEGRAL_ELEMENT_H_)
#define INTEGRAL_ELEMENT_H_

#include <tide/ebml_integer.h>
#include <tide/element_base.h>
#include <tide/integral_element_impl.h>
#include <tide/primitive_element.h>
#include <tide/win_dll.h>

#include <boost/operators.hpp>
#include <iostream>

/// \addtogroup interfaces Interfaces
/// @{

namespace tide
{
    /** \brief Interface to an integer primitive element.
     *
     * Integer elements form one type of primitive element. This defines the
     * interface to an integer element.
     *
     * This class implements the PrimitiveElement interface.
     *
     * Copy-construction and assignment are provided by the compiler.
     */
    template<typename T>
    class TIDE_EXPORT IntegralElement
        : public ElementBase<IntegralElement<T> >,
        public PrimitiveElement<T>,
        boost::totally_ordered<IntegralElement<T> >,
        boost::totally_ordered<IntegralElement<T>, T >,
        boost::integer_arithmetic<IntegralElement<T> >,
        boost::integer_arithmetic<IntegralElement<T>, T>,
        boost::bitwise<IntegralElement<T> >,
        boost::bitwise<IntegralElement<T>, T >,
        boost::unit_steppable<IntegralElement<T> >,
        boost::shiftable<IntegralElement<T> >,
        boost::shiftable<IntegralElement<T>, T >
    {
        friend class ElementBase<IntegralElement<T> >;
        public:
            /** \brief Constructor.
             *
             * \param[in] id The ID of the element.
             * \param[in] value The value of the element.
             */
            IntegralElement(ids::ID id,
                    T value)
                : ElementBase<IntegralElement>(id), impl_(value), id_(id),
                offset_(0), writing_(false)
            {}

            /** \brief Constructor.
             *
             * \param[in] id The ID of the element.
             * \param[in] value The value of the element.
             */
            IntegralElement(tide::ids::ID id,
                    typename boost::add_lvalue_reference<T>::type value)
                : ElementBase<IntegralElement>(id), impl_(value), id_(id),
                offset_(0), writing_(false)
            {}

            /** \brief Constructor.
             *
             * \param[in] id The ID of the element.
             * \param[in] value The value of the element.
             * \param[in] default_val The default value of the element.
             */
            IntegralElement(tide::ids::ID id, T value, T default_val)
                : ElementBase<IntegralElement>(id), impl_(value, default_val),
                id_(id), offset_(0), writing_(false)
            {}

            /** \brief Constructor.
             *
             * \param[in] id The ID of the element.
             * \param[in] value The value of the element.
             * \param[in] default_val The default value of the element.
             */
            IntegralElement(tide::ids::ID id,
                    typename boost::add_lvalue_reference<T>::type value,
                    typename boost::add_lvalue_reference<T>::type default_val)
                : ElementBase<IntegralElement>(id), impl_(value, default_val),
                id_(id), offset_(0), writing_(false)
            {}

            /** \brief Swap this element's value with another's.
             *
             * \param[in] other The other element to swap with.
             */
            void swap(IntegralElement& other)
            {
                using std::swap;

                swap(impl_, other.impl_);
                swap(id_, other.id_);
                swap(offset_, other.offset_);
                swap(writing_, other.writing_);
            }

            /// \brief Less-than comparison operator.
            friend bool operator<(IntegralElement const& lhs,
                    IntegralElement const& rhs)
            {
                return lhs.value() < rhs.value();
            }

            /// \brief Less-than comparison operator.
            friend bool operator<(IntegralElement const& lhs, T const rhs)
            {
                return lhs.value() < rhs;
            }

            /// \brief Greater-than comparison operator.
            friend bool operator>(IntegralElement const& lhs, T const rhs)
            {
                return lhs.value() > rhs;
            }

            /// \brief Assignment addition operator.
            IntegralElement& operator+=(IntegralElement const& rhs)
            {
                impl_ += rhs.impl_;
                return *this;
            }

            /// \brief Assignment addition operator.
            IntegralElement& operator+=(T const rhs)
            {
                impl_ += rhs;
                return *this;
            }

            /// \brief Assignment subtraction operator.
            IntegralElement& operator-=(IntegralElement const& rhs)
            {
                impl_ -= rhs.impl_;
                return *this;
            }

            /// \brief Assignment subtraction operator.
            IntegralElement& operator-=(T const rhs)
            {
                impl_ -= rhs;
                return *this;
            }

            /** \brief Subtraction operator.
             *
             * Note: provided manually because this class does not function
             * with NRVO.
             */
            friend T operator-(T lhs, IntegralElement const& rhs)
            {
                lhs -= rhs.value();
                return lhs;
            }

            /// \brief Assignment multiplication operator.
            IntegralElement& operator*=(IntegralElement const& rhs)
            {
                impl_ *= rhs.impl_;
                return *this;
            }

            /// \brief Assignment multiplication operator.
            IntegralElement& operator*=(T const rhs)
            {
                impl_ *= rhs;
                return *this;
            }

            /// \brief Assignment division operator.
            IntegralElement& operator/=(IntegralElement const& rhs)
            {
                impl_ /= rhs.impl_;
                return *this;
            }

            /// \brief Assignment division operator.
            IntegralElement& operator/=(T const rhs)
            {
                impl_ /= rhs;
                return *this;
            }

            /** \brief Division operator.
             *
             * Note: provided manually because this class does not function
             * with NRVO.
             */
            friend T operator/(T lhs, IntegralElement const& rhs)
            {
                lhs /= rhs.value();
                return lhs;
            }

            /// \brief Assignment modulus operator.
            IntegralElement& operator%=(IntegralElement const& rhs)
            {
                impl_ %= rhs.impl_;
                return *this;
            }

            /// \brief Assignment modulus operator.
            IntegralElement& operator%=(T const rhs)
            {
                impl_ %= rhs;
                return *this;
            }

            /** \brief Modulus operator.
             *
             * Note: provided manually because this class does not function
             * with NRVO.
             */
            friend T operator%(T lhs, IntegralElement const& rhs)
            {
                lhs %= rhs.value();
                return lhs;
            }

            /// \brief Assignment logical-or operator.
            IntegralElement& operator|=(IntegralElement const& rhs)
            {
                impl_ |= rhs.impl_;
                return *this;
            }

            /// \brief Assignment logical-or operator.
            IntegralElement& operator|=(T const rhs)
            {
                impl_ |= rhs;
                return *this;
            }

            /// \brief Assignment logical-and operator.
            IntegralElement& operator&=(IntegralElement const& rhs)
            {
                impl_ &= rhs.impl_;
                return *this;
            }

            /// \brief Assignment logical-and operator.
            IntegralElement& operator&=(T const rhs)
            {
                impl_ &= rhs;
                return *this;
            }

            /// \brief Assignment logical-xor operator.
            IntegralElement& operator^=(IntegralElement const& rhs)
            {
                impl_ ^= rhs.impl_;
                return *this;
            }

            /// \brief Assignment logical-xor operator.
            IntegralElement& operator^=(T const rhs)
            {
                impl_ ^= rhs;
                return *this;
            }

            /// \brief Assignment left-shift operator.
            IntegralElement& operator<<=(IntegralElement const& rhs)
            {
                impl_ <<= rhs.impl_;
                return *this;
            }

            /// \brief Assignment left-shift operator.
            IntegralElement& operator<<=(T const rhs)
            {
                impl_ <<= rhs;
                return *this;
            }

            /// \brief Assignment right-shift operator.
            IntegralElement& operator>>=(IntegralElement const& rhs)
            {
                impl_ >>= rhs.impl_;
                return *this;
            }

            /// \brief Assignment right-shift operator.
            IntegralElement& operator>>=(T const rhs)
            {
                impl_ >>= rhs;
                return *this;
            }

            /// \brief Pre-increment operator.
            IntegralElement& operator++()
            {
                ++impl_;
                return *this;
            }

            /// \brief Pre-decrement operator.
            IntegralElement& operator--()
            {
                --impl_;
                return *this;
            }

        private:
            ///////////////////////////////////////////////////////////////////
            // PrimitiveBase interface implementation
            impl::IntegralElementImpl<T> impl_;

            virtual T value_impl() const
            {
                return impl_.value();
            }

            virtual void value_impl(T value)
            {
                impl_.value(value);
            }

            virtual bool has_default_impl() const
            {
                return impl_.has_default();
            }

            virtual T get_default_impl() const
            {
                return impl_.get_default();
            }

            virtual void set_default_impl(T default_value)
            {
                impl_.set_default(default_value);
            }

            virtual T remove_default_impl()
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

            std::streamsize body_stored_size() const
            {
                return impl_.body_stored_size();
            }

            std::streamsize read_body(std::istream& i, std::streamsize size)
            {
                return impl_.read_body(i, size);
            }

            std::streamsize start_body(std::iostream& io) const
            {
                return impl_.start_body(io);
            }

            std::streamsize finish_body(std::iostream& io) const
            {
                impl_.finish_body(io);
                return this->stored_size();
            }
    }; // class IntegralElement


    /// \brief Swap integral element values.
    template<typename T>
    void swap(IntegralElement<T>& a, IntegralElement<T>& b)
    {
        a.swap(b);
    }


    /// \brief Stream output operator.
    template<typename T>
    std::ostream& operator<<(std::ostream& o, IntegralElement<T> const& rhs)
    {
        return o << rhs.value();
    }


    /// \brief Stream input operator.
    template<typename T>
    std::istream& operator>>(std::istream& i, IntegralElement<T>& rhs)
    {
        T temp;
        i >> temp;
        rhs.set_value(temp);
        return i;
    }
}; //namespace tide

/// @}
/// group interfaces

#endif // !defined(INTEGRAL_ELEMENT_H_)

