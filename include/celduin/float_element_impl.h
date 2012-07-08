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

#if !defined(FLOAT_ELEMENT_IMPL_H_)
#define FLOAT_ELEMENT_IMPL_H_

#include <celduin/float_element_precision.h>
#include <celduin/primitive_element_base.h>

#include <boost/type_traits/add_lvalue_reference.hpp>
#include <ios>

/// \addtogroup implementations Internal implementations
/// @{

namespace celduin
{
    namespace impl
    {
        /** \brief Internal implementation of a floating point primitive
         * element.
         *
         * This class implements the PrimitivElementBase CRTP requirements.
         */
        class FloatElementImpl
            : public PrimitiveElementBase<FloatElementImpl, double>
        {
            friend class PrimitiveElementBase<FloatElementImpl, double>;

            public:
                /** \brief Constructor.
                 *
                 * \param[in] value The value of the element.
                 * \param[in] precision The element's precision, single or
                 * double.  This only has an effect when writing the float to a
                 * file.
                 */
                FloatElementImpl(double value, EBMLFloatPrecision precision);

                /** \brief Constructor.
                 *
                 * \param[in] value The value of the element.
                 * \param[in] default_val The default value of the element.
                 * \param[in] precision The element's precision, single or
                 * double.  This only has an effect when writing the float to a
                 * file.
                 */
                FloatElementImpl(double value, double default_val,
                        EBMLFloatPrecision precision);

                /** \brief Swap this element with another.
                 *
                 * \param[in] other The other floating point element to swap
                 * with.
                 */
                void swap(FloatElementImpl& other);

                /** \brief Swap this element's value with another instance of
                 * the value type.
                 *
                 * Only the value is swapped. The ID and default value are left
                 * unchanged.
                 *
                 * \param[in] other The other value to swap with.
                 */
                void swap(double& other);

                /// \brief Less-than comparison operator.
                bool operator<(FloatElementImpl const& rhs)
                {
                    return value_ < rhs.value_;
                }

                /// \brief Less-than comparison operator.
                bool operator<(double rhs)
                {
                    return value_ < rhs;
                }

                /// \brief Greater-than comparison operator.
                bool operator>(double rhs)
                {
                    return value_ > rhs;
                }

                /// \brief Assignment addition operator.
                FloatElementImpl& operator+=(FloatElementImpl const& rhs)
                {
                    value_ += rhs.value_;
                    return *this;
                }

                /// \brief Assignment addition operator.
                FloatElementImpl& operator+=(double rhs)
                {
                    value_ += rhs;
                    return *this;
                }

                /// \brief Assignment subtraction operator.
                FloatElementImpl& operator-=(FloatElementImpl const& rhs)
                {
                    value_ -= rhs.value_;
                    return *this;
                }

                /// \brief Assignment subtraction operator.
                FloatElementImpl& operator-=(double rhs)
                {
                    value_ -= rhs;
                    return *this;
                }

                /// \brief Assignment multiplication operator.
                FloatElementImpl& operator*=(FloatElementImpl const& rhs)
                {
                    value_ *= rhs.value_;
                    return *this;
                }

                /// \brief Assignment multiplication operator.
                FloatElementImpl& operator*=(double rhs)
                {
                    value_ *= rhs;
                    return *this;
                }

                /// \brief Assignment division operator.
                FloatElementImpl& operator/=(FloatElementImpl const& rhs)
                {
                    value_ /= rhs.value_;
                    return *this;
                }

                /// \brief Assignment division operator.
                FloatElementImpl& operator/=(double rhs)
                {
                    value_ /= rhs;
                    return *this;
                }

                /// \brief Pre-increment operator.
                FloatElementImpl& operator++()
                {
                    ++value_;
                    return *this;
                }

                /// \brief Pre-decrement operator.
                FloatElementImpl& operator--()
                {
                    --value_;
                    return *this;
                }

                /// \brief Get the precision setting.
                EBMLFloatPrecision precision() const { return precision_; }
                /** \brief Set the precision setting
                 *
                 * This value determines if the float is single or double
                 * precision. The precision value has no effect until the float is
                 * written to a file, at which point single-precision floats are
                 * written using 4 bytes while double-precision floats are written
                 * using 8 bytes.
                 */
                void precision(EBMLFloatPrecision precision)
                {
                    precision_ = precision;
                }

                /// \brief Get the size of the stored value.
                std::streamsize body_stored_size() const;

                /// \brief Read the stored value from a byte stream.
                std::streamsize read_body(std::istream& i, std::streamsize size);

                /// \brief Begin writing the stored value to a byte stream.
                std::streamsize start_body(std::ostream& o) const;

                /// \brief Finish writing the stored value to a byte stream.
                std::streamsize finish_body(std::ostream& o) const;

            private:
                ///////////////////////////////////////////////////////////////////
                // PrimitiveElement CRTP required members
                double value_;
                double default_;
                bool has_default_;
                // Must be mutable to be updated in start_body()
                mutable std::streampos body_end_;

                ///////////////////////////////////////////////////////////////////
                // Other float element members
                EBMLFloatPrecision precision_;
        }; // class FloatElementImpl


        /// \brief Swap date element implementation objects.
        void swap(FloatElementImpl& a, FloatElementImpl& b);


        /** \brief Swap this element's value with another instance of the
         * value type.
         *
         * Only the value is swapped. The ID and default value are left
         * unchanged.
         */
        void swap(FloatElementImpl& a, double& b);
    }; // namespace impl
}; //namespace celduin

/// @}
/// group implementations

#endif // !defined(FLOAT_ELEMENT_IMPL_H_)

