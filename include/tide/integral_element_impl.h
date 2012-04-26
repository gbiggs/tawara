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

#if !defined(INTEGER_ELEMENT_IMPL_H_)
#define INTEGER_ELEMENT_IMPL_H_

#include <tide/ebml_integer.h>
#include <tide/primitive_element_base.h>
#include <tide/win_dll.h>

#include <boost/type_traits/add_lvalue_reference.hpp>

/// \addtogroup implementations Internal implementations
/// @{

namespace tide
{
    namespace impl
    {
        /** \brief Internal implementation of an integral primitive element.
         *
         * This class provides the implementation of an integral element.
         *
         * This class implements the PrimitivElementBase CRTP requirements.
         */
        template<typename T, class RWSPolicy>
        class TIDE_EXPORT IntegralElementImpl
            : public PrimitiveElementBase<IntegralElementImpl<T,
                RWSPolicy>, T>,
            public RWSPolicy
        {
            friend class PrimitiveElementBase<IntegralElementImpl<T,
                RWSPolicy>, T>;

            public:
                /** \brief Constructor.
                 *
                 * \param[in] value The value of the element.
                 */
                explicit IntegralElementImpl(
                        typename boost::add_lvalue_reference<T>::type value)
                    : value_(value), default_(0), has_default_(false),
                    body_end_(0)
                {}

                /** \brief Constructor.
                 *
                 * \param[in] value The value of the element.
                 * \param[in] default_val The default value of the element.
                 */
                IntegralElementImpl(
                        typename boost::add_lvalue_reference<T>::type value,
                        typename boost::add_lvalue_reference<T>::type default_val)
                    : value_(value), default_(default_val), has_default_(true),
                    body_end_(0)
                {}

                /** \brief Swap this integral element with another.
                 *
                 * \param[in] other The other integral element to swap with.
                 */
                void swap(IntegralElementImpl& other)
                {
                    using std::swap;

                    swap(value_, other.value_);
                    swap(default_, other.default_);
                    swap(has_default_, other.has_default_);
                    swap(body_end_, other.body_end_);
                }

                /** \brief Swap this element's value with another instance of
                 * the value type.
                 *
                 * Only the value is swapped. The ID and default value are left
                 * unchanged.
                 *
                 * \param[in] other The other value to swap with.
                 */
                void swap(T& other)
                {
                    using std::swap;
                    swap(value_, other);
                }

                /// \brief Less-than comparison operator.
                bool operator<(IntegralElementImpl const& rhs)
                {
                    return value_ < rhs.value_;
                }

                /// \brief Less-than comparison operator.
                bool operator<(T const rhs)
                {
                    return value_ < rhs;
                }

                /// \brief Greater-than comparison operator.
                bool operator>(IntegralElementImpl const& rhs)
                {
                    return value_ > rhs.value_;
                }

                /// \brief Greater-than comparison operator.
                bool operator>(T const rhs)
                {
                    return value_ > rhs;
                }

                /// \brief Assignment addition operator.
                IntegralElementImpl& operator+=(IntegralElementImpl const& rhs)
                {
                    value_ += rhs.value_;
                    return *this;
                }

                /// \brief Assignment addition operator.
                IntegralElementImpl& operator+=(T const rhs)
                {
                    value_ += rhs;
                    return *this;
                }

                /// \brief Assignment subtraction operator.
                IntegralElementImpl& operator-=(IntegralElementImpl const& rhs)
                {
                    value_ -= rhs.value_;
                    return *this;
                }

                /// \brief Assignment subtraction operator.
                IntegralElementImpl& operator-=(T const rhs)
                {
                    value_ -= rhs;
                    return *this;
                }

                /// \brief Assignment multiplication operator.
                IntegralElementImpl& operator*=(IntegralElementImpl const& rhs)
                {
                    value_ *= rhs.value_;
                    return *this;
                }

                /// \brief Assignment multiplication operator.
                IntegralElementImpl& operator*=(T const rhs)
                {
                    value_ *= rhs;
                    return *this;
                }

                /// \brief Assignment division operator.
                IntegralElementImpl& operator/=(IntegralElementImpl const& rhs)
                {
                    value_ /= rhs.value_;
                    return *this;
                }

                /// \brief Assignment division operator.
                IntegralElementImpl& operator/=(T const rhs)
                {
                    value_ /= rhs;
                    return *this;
                }

                /// \brief Assignment modulus operator.
                IntegralElementImpl& operator%=(IntegralElementImpl const& rhs)
                {
                    value_ %= rhs.value_;
                    return *this;
                }

                /// \brief Assignment modulus operator.
                IntegralElementImpl& operator%=(T const rhs)
                {
                    value_ %= rhs;
                    return *this;
                }

                /// \brief Assignment logical-or operator.
                IntegralElementImpl& operator|=(IntegralElementImpl const& rhs)
                {
                    value_ |= rhs.value_;
                    return *this;
                }

                /// \brief Assignment logical-or operator.
                IntegralElementImpl& operator|=(T const rhs)
                {
                    value_ |= rhs;
                    return *this;
                }

                /// \brief Assignment logical-and operator.
                IntegralElementImpl& operator&=(IntegralElementImpl const& rhs)
                {
                    value_ &= rhs.value_;
                    return *this;
                }

                /// \brief Assignment logical-and operator.
                IntegralElementImpl& operator&=(T const rhs)
                {
                    value_ &= rhs;
                    return *this;
                }

                /// \brief Assignment logical-xor operator.
                IntegralElementImpl& operator^=(IntegralElementImpl const& rhs)
                {
                    value_ ^= rhs.value_;
                    return *this;
                }

                /// \brief Assignment logical-xor operator.
                IntegralElementImpl& operator^=(T const rhs)
                {
                    value_ ^= rhs;
                    return *this;
                }

                /// \brief Assignment left-shift operator.
                IntegralElementImpl& operator<<=(IntegralElementImpl const& rhs)
                {
                    value_ <<= rhs.value_;
                    return *this;
                }

                /// \brief Assignment left-shift operator.
                IntegralElementImpl& operator<<=(T const rhs)
                {
                    value_ <<= rhs;
                    return *this;
                }

                /// \brief Assignment right-shift operator.
                IntegralElementImpl& operator>>=(IntegralElementImpl const& rhs)
                {
                    value_ >>= rhs.value_;
                    return *this;
                }

                /// \brief Assignment right-shift operator.
                IntegralElementImpl& operator>>=(T const rhs)
                {
                    value_ >>= rhs;
                    return *this;
                }

                /// \brief Pre-increment operator.
                IntegralElementImpl& operator++()
                {
                    ++value_;
                    return *this;
                }

                /// \brief Pre-decrement operator.
                IntegralElementImpl& operator--()
                {
                    --value_;
                    return *this;
                }

                /// \brief Get the size of the stored value.
                std::streamsize body_stored_size() const
                {
                    return RWSPolicy::size(value_);
                }

                /// \brief Read the stored value from a byte stream.
                std::streamsize read_body(std::istream& i, std::streamsize size)
                {
                    T temp;
                    temp = RWSPolicy::read(i, size);
                    std::swap(value_, temp);
                    return size;
                }

                /// \brief Begin writing the stored value to a byte stream.
                std::streamsize start_body(std::ostream& o) const
                {
                    std::streamsize result;
                    result = RWSPolicy::write(value_, o);
                    // Record the position after this element's body for use in
                    // finish_body().
                    body_end_ = o.tellp();
                    return result;
                }

                /// \brief Finish writing the stored value to a byte stream.
                std::streamsize finish_body(std::ostream& o) const
                {
                    // All writing was taken care of by start_body()
                    // Ensure the post-condition that the write pointer is
                    // placed after the element's body.
                    o.seekp(body_end_);
                    return body_stored_size();
                }

            private:
                ///////////////////////////////////////////////////////////////////
                // PrimitiveElement CRTP required members
                T value_;
                T default_;
                bool has_default_;
                // Must be mutable to be updated in start_body()
                mutable std::streampos body_end_;
        }; // class IntegralElementImpl
    }; // namespace impl


    /// \brief Swap integral element implementation objects.
    template<typename T, class RWSPolicy>
    void swap(impl::IntegralElementImpl<T, RWSPolicy>& a,
            impl::IntegralElementImpl<T, RWSPolicy>& b)
    {
        a.swap(b);
    }


    /** \brief Swap this element's value with another instance of the
     * value type.
     *
     * Only the value is swapped. The ID and default value are left
     * unchanged.
     */
    template<typename T, class RWSPolicy>
    void swap(impl::IntegralElementImpl<T, RWSPolicy>& a, T& b)
    {
        a.swap(b);
    }
}; //namespace tide

/// @}
/// group implementations

#endif // !defined(INTEGER_ELEMENT_IMPL_H_)

