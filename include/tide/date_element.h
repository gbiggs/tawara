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

#if !defined(DATE_ELEMENT_H_)
#define DATE_ELEMENT_H_

#include <tide/date_element_impl.h>
#include <tide/element_base.h>
#include <tide/primitive_element.h>
#include <tide/win_dll.h>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/operators.hpp>
#include <iostream>


/// \addtogroup interfaces Interfaces
/// @{

namespace tide
{
    /** \brief Interface to a date primitive element.
     *
     * The date element is one type of primitive element. This defines the
     * interface to a date element. The interface is compatible with the
     * Boost.Date_Time library's Ptime interface.
     *
     * This class implements the Element interface.
     *
     * This class implements the PrimitiveElement interface.
     *
     * Copy-construction and assignment are provided by the compiler.
     */
    class TIDE_EXPORT DateElement
        : public ElementBase<DateElement>,
        public PrimitiveElement<boost::posix_time::ptime>,
        boost::less_than_comparable<DateElement>,
        boost::less_than_comparable<DateElement, boost::posix_time::ptime>,
        boost::equality_comparable<DateElement>,
        boost::equality_comparable<DateElement, boost::posix_time::ptime>,
        boost::additive<DateElement, boost::gregorian::days>,
        boost::additive<DateElement, boost::posix_time::time_duration>
    {
        friend class ElementBase<DateElement>;

        public:
            /** \brief Constructor.
             *
             * \param[in] id The ID of the element.
             * \param[in] value The value of the element.
             */
            DateElement(ids::ID id, boost::posix_time::ptime const& value);

            /** \brief Constructor.
             *
             * \param[in] id The ID of the element.
             * \param[in] value The value of the element.
             * \param[in] default_val The default value of the element.
             */
            DateElement(tide::ids::ID id, boost::posix_time::ptime const& value,
                    boost::posix_time::ptime const& default_val);

            /** \brief Swap this element's value with another's.
             *
             * \param[in] other The other element to swap with.
             */
            void swap(DateElement& other);

            /// \brief Less-than comparison operator.
            friend bool operator<(DateElement const& lhs,
                    DateElement const& rhs)
            {
                return lhs.value() < rhs.value();
            }

            /// \brief Less-than comparison operator.
            friend bool operator<(DateElement const& lhs,
                    boost::posix_time::ptime const& rhs)
            {
                return lhs.value() < rhs;
            }

            /// \brief Greater-than comparison operator.
            friend bool operator>(DateElement const& lhs,
                    boost::posix_time::ptime const& rhs)
            {
                return lhs.value() > rhs;
            }

            /// \brief Assignment addition operator (days).
            DateElement& operator+=(boost::gregorian::days const& rhs)
            {
                impl_ += rhs;
                return *this;
            }

            /// \brief Assignment subtraction operator (days).
            DateElement& operator-=(boost::gregorian::days const& rhs)
            {
                impl_ -= rhs;
                return *this;
            }

            /// \brief Assignment addition operator (time durations).
            DateElement& operator+=(boost::posix_time::time_duration const& rhs)
            {
                impl_ += rhs;
                return *this;
            }

            /// \brief Assignment subtraction operator (time durations).
            DateElement& operator-=(boost::posix_time::time_duration const& rhs)
            {
                impl_ -= rhs;
                return *this;
            }

            /// \brief Get the date part of the time.
            boost::gregorian::date date() const
            {
                return impl_.date();
            }

            /// \brief Get the time offset in the day.
            boost::posix_time::time_duration time_of_day() const
            {
                return impl_.time_of_day();
            }

            /// \brief Check if the time is infinity.
            bool is_infinity() const
            {
                return impl_.is_infinity();
            }

            /// \brief Check if the time is negative infinity.
            bool is_neg_infinity() const
            {
                return impl_.is_neg_infinity();
            }

            /// \brief Check if the time is positive infinity.
            bool is_pos_infinity() const
            {
                return impl_.is_pos_infinity();
            }

            /// \brief Check if the time is valid.
            bool is_not_a_date_time() const
            {
                return impl_.is_not_a_date_time();
            }

            /// \brief Check if the time is a special value.
            bool is_special() const
            {
                return impl_.is_special();
            }

        private:
            ///////////////////////////////////////////////////////////////////
            // PrimitiveBase interface implementation
            impl::DateElementImpl impl_;

            virtual boost::posix_time::ptime value_impl() const
            {
                return impl_.value();
            }

            // TODO: Add automatic-reference-adding to the parameter for
            // primitive elements using non-built-in types for T.
            virtual void value_impl(boost::posix_time::ptime value)
            {
                impl_.value(value);
            }

            virtual bool has_default_impl() const
            {
                return impl_.has_default();
            }

            virtual boost::posix_time::ptime get_default_impl() const
            {
                return impl_.get_default();
            }

            // TODO: Add automatic-reference-adding to the parameter for
            // primitive elements using non-built-in types for T.
            virtual void set_default_impl(
                    boost::posix_time::ptime default_value)
            {
                impl_.set_default(default_value);
            }

            virtual boost::posix_time::ptime remove_default_impl()
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
    }; // class DateElement


    /// \brief Swap floating point element values.
    void swap(DateElement& a, DateElement& b)
    {
        a.swap(b);
    }


    /// \brief Subtraction operator.
    template<class T>
    boost::posix_time::time_duration operator-(DateElement const& lhs,
            T const& rhs)
    {
        return lhs.value() - rhs;
    }


    /// \brief Subtraction operator.
    template<class T>
    boost::posix_time::time_duration operator-(T const& lhs,
            DateElement const& rhs)
    {
        return lhs - rhs.value();
    }


    /// \brief Subtraction operator.
    boost::posix_time::time_duration operator-(DateElement const& lhs,
            DateElement const& rhs)
    {
        return lhs.value() - rhs.value();
    }


    /// \brief Stream output operator.
    std::ostream& operator<<(std::ostream& o, DateElement const& rhs)
    {
        return o << rhs.value();
    }


    /// \brief Stream input operator.
    std::istream& operator>>(std::istream& i, DateElement& rhs)
    {
        boost::posix_time::ptime temp;
        i >> temp;
        rhs.value(temp);
        return i;
    }
}; // namespace tide

/// @}
// group interfaces

#endif // !defined(DATE_ELEMENT_H_)

