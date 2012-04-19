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

#if !defined(FLOAT_ELEMENT_H_)
#define FLOAT_ELEMENT_H_

#include <tide/element_base.h>
#include <tide/float_element_impl.h>
#include <tide/float_element_precision.h>
#include <tide/primitive_element.h>
#include <tide/win_dll.h>

#include <boost/operators.hpp>
#include <iostream>

/// \addtogroup interfaces Interfaces
/// @{

namespace tide
{
    /** \brief Interface to a floating point primitive element.
     *
     * Floating point elements form one type of primitive element. This defines
     * the interface to a floating point element.
     *
     * This class implements the Element interface.
     *
     * This class implements the PrimitiveElement interface.
     *
     * Copy-construction and assignment are provided by the compiler.
     */
    class TIDE_EXPORT FloatElement
        : public ElementBase<FloatElement>,
        public PrimitiveElement<double>,
        boost::partially_ordered<FloatElement>,
        boost::partially_ordered<FloatElement, double>,
        boost::equality_comparable<FloatElement>,
        boost::equality_comparable<FloatElement, double>,
        boost::arithmetic<FloatElement>,
        boost::arithmetic<FloatElement, double>,
        boost::unit_steppable<FloatElement>
    {
        friend class ElementBase<FloatElement>;
        public:
            /** \brief Constructor.
             *
             * \param[in] id The ID of the element.
             * \param[in] value The value of the element.
             * \param[in] precision The element's precision, single or double.
             * This only has an effect when writing the float to a file.
             */
            FloatElement(ids::ID id, double value,
                    EBMLFloatPrecision precision=EBML_FLOAT_PREC_DOUBLE);

            /** \brief Constructor.
             *
             * \param[in] id The ID of the element.
             * \param[in] value The value of the element.
             * \param[in] default_val The default value of the element.
             * \param[in] precision The element's precision, single or double.
             * This only has an effect when writing the float to a file.
             */
            FloatElement(tide::ids::ID id, double value, double default_val,
                    EBMLFloatPrecision precision=EBML_FLOAT_PREC_DOUBLE);

            /** \brief Swap this element's value with another's.
             *
             * \param[in] other The other element to swap with.
             */
            void swap(FloatElement& other);

            /// \brief Less-than comparison operator.
            friend bool operator<(FloatElement const& lhs,
                    FloatElement const& rhs)
            {
                return lhs.value() < rhs.value();
            }

            /// \brief Less-than comparison operator.
            friend bool operator<(FloatElement const& lhs, double const rhs)
            {
                return lhs.value() < rhs;
            }

            /// \brief Greater-than comparison operator.
            friend bool operator>(FloatElement const& lhs, double const rhs)
            {
                return lhs.value() > rhs;
            }

            /// \brief Assignment addition operator.
            FloatElement& operator+=(FloatElement const& rhs)
            {
                impl_ += rhs.impl_;
                return *this;
            }

            /// \brief Assignment addition operator.
            FloatElement& operator+=(double const rhs)
            {
                impl_ += rhs;
                return *this;
            }

            /// \brief Assignment subtraction operator.
            FloatElement& operator-=(FloatElement const& rhs)
            {
                impl_ -= rhs.impl_;
                return *this;
            }

            /// \brief Assignment subtraction operator.
            FloatElement& operator-=(double const rhs)
            {
                impl_ -= rhs;
                return *this;
            }

            /** \brief Subtraction operator.
             *
             * Note: provided manually because this class does not function
             * with NRVO.
             */
            friend double operator-(double lhs, FloatElement const& rhs)
            {
                lhs -= rhs.value();
                return lhs;
            }

            /// \brief Assignment multiplication operator.
            FloatElement& operator*=(FloatElement const& rhs)
            {
                impl_ *= rhs.impl_;
                return *this;
            }

            /// \brief Assignment multiplication operator.
            FloatElement& operator*=(double const rhs)
            {
                impl_ *= rhs;
                return *this;
            }

            /// \brief Assignment division operator.
            FloatElement& operator/=(FloatElement const& rhs)
            {
                impl_ /= rhs.impl_;
                return *this;
            }

            /// \brief Assignment division operator.
            FloatElement& operator/=(double const rhs)
            {
                impl_ /= rhs;
                return *this;
            }

            /** \brief Division operator.
             *
             * Note: provided manually because this class does not function
             * with NRVO.
             */
            friend double operator/(double lhs, FloatElement const& rhs)
            {
                lhs /= rhs.value();
                return lhs;
            }

            /// \brief Pre-increment operator.
            FloatElement& operator++()
            {
                ++impl_;
                return *this;
            }

            /// \brief Pre-decrement operator.
            FloatElement& operator--()
            {
                --impl_;
                return *this;
            }

            /// \brief Get the precision setting.
            EBMLFloatPrecision precision() const { return impl_.precision(); }
            /** \brief Set the precision setting
             *
             * This value determines if the float is single or double
             * precision. The precision value has no effect until the float is
             * written to a file, at which point single-precision floats are
             * written using 4 bytes while double-precision floats are written
             * using 8 bytes.
             */
            void precision(EBMLFloatPrecision precision)
                { impl_.precision(precision); }

        private:
            ///////////////////////////////////////////////////////////////////
            // PrimitiveBase interface implementation
            impl::FloatElementImpl impl_;

            virtual double value_impl() const
            {
                return impl_.value();
            }

            virtual void value_impl(double value)
            {
                impl_.value(value);
            }

            virtual bool has_default_impl() const
            {
                return impl_.has_default();
            }

            virtual double get_default_impl() const
            {
                return impl_.get_default();
            }

            virtual void set_default_impl(double default_value)
            {
                impl_.set_default(default_value);
            }

            virtual double remove_default_impl()
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
    }; // class FloatElement

    /// \brief Swap floating point element values.
    void swap(FloatElement& a, FloatElement& b)
    {
        a.swap(b);
    }


    /// \brief Stream output operator.
    std::ostream& operator<<(std::ostream& o, FloatElement const& rhs)
    {
        return o << rhs.value();
    }


    /// \brief Stream input operator.
    std::istream& operator>>(std::istream& i, FloatElement& rhs)
    {
        double temp;
        i >> temp;
        rhs.value(temp);
        return i;
    }
}; // namespace tide

/// @}
// group interfaces

#endif // !defined(FLOAT_ELEMENT_H_)

