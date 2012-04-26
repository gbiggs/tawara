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

#if !defined(DATE_ELEMENT_IMPL_H_)
#define DATE_ELEMENT_IMPL_H_

#include <tide/primitive_element_base.h>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/operators.hpp>
#include <iostream>

/// \addtogroup implementations Internal implementations
/// @{

namespace tide
{
    namespace impl
    {
        /** \brief Internal implementation of a date element.
         *
         * This class provides the implementation of a date element.
         *
         * This class implements the PrimitiveElementBase CRTP requirements.
         */
        class DateElementImpl
            : public PrimitiveElementBase<DateElementImpl,
                boost::posix_time::ptime>
        {
            friend class PrimitiveElementBase<DateElementImpl,
                boost::posix_time::ptime>;

            public:
                /** \brief Constructor.
                 *
                 * \param[in] value The value of the element.
                 */
                DateElementImpl(boost::posix_time::ptime const& value);

                /** \brief Constructor.
                 *
                 * \param[in] value The value of the element.
                 * \param[in] default_val The default value of the element.
                 */
                DateElementImpl(boost::posix_time::ptime const& value,
                        boost::posix_time::ptime const& default_val);

                /** \brief Swap this element's value with another's.
                 *
                 * \param[in] other The other element to swap with.
                 */
                void swap(DateElementImpl& other);

                /** \brief Swap this element's value with another instance of
                 * the value type.
                 *
                 * Only the value is swapped. The ID and default value are left
                 * unchanged.
                 *
                 * \param[in] other The other value to swap with.
                 */
                void swap(boost::posix_time::ptime& other);

                /// \brief Less-than comparison operator.
                bool operator<(DateElementImpl const& rhs)
                {
                    return value_ < rhs.value_;
                }

                /// \brief Less-than comparison operator.
                bool operator<(boost::posix_time::ptime const& rhs)
                {
                    return value_ < rhs;
                }

                /// \brief Greater-than comparison operator.
                bool operator>(boost::posix_time::ptime const& rhs)
                {
                    return value_ > rhs;
                }

                /// \brief Assignment addition operator (days).
                DateElementImpl& operator+=(boost::gregorian::days const& rhs)
                {
                    value_ += rhs;
                    return *this;
                }

                /// \brief Assignment subtraction operator (days).
                DateElementImpl& operator-=(boost::gregorian::days const& rhs)
                {
                    value_ -= rhs;
                    return *this;
                }

                /// \brief Assignment addition operator (time durations).
                DateElementImpl& operator+=(boost::posix_time::time_duration const& rhs)
                {
                    value_ += rhs;
                    return *this;
                }

                /// \brief Assignment subtraction operator (time durations).
                DateElementImpl& operator-=(boost::posix_time::time_duration const& rhs)
                {
                    value_ -= rhs;
                    return *this;
                }

                /// \brief Get the date part of the time.
                boost::gregorian::date date() const
                {
                    return value_.date();
                }

                /// \brief Get the time offset in the day.
                boost::posix_time::time_duration time_of_day() const
                {
                    return value_.time_of_day();
                }

                /// \brief Check if the time is infinity.
                bool is_infinity() const
                {
                    return value_.is_infinity();
                }

                /// \brief Check if the time is negative infinity.
                bool is_neg_infinity() const
                {
                    return value_.is_neg_infinity();
                }

                /// \brief Check if the time is positive infinity.
                bool is_pos_infinity() const
                {
                    return value_.is_pos_infinity();
                }

                /// \brief Check if the time is valid.
                bool is_not_a_date_time() const
                {
                    return value_.is_not_a_date_time();
                }

                /// \brief Check if the time is a special value.
                bool is_special() const
                {
                    return value_.is_special();
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
                boost::posix_time::ptime value_;
                boost::posix_time::ptime default_;
                bool has_default_;
                // Must be mutable to be updated in start_body()
                mutable std::streampos body_end_;
        }; // class DateElementImpl


        /// \brief Swap date element implementation objects.
        void swap(DateElementImpl& a, DateElementImpl& b);


        /** \brief Swap this element's value with another instance of the
         * value type.
         *
         * Only the value is swapped. The ID and default value are left
         * unchanged.
         */
        void swap(DateElementImpl& a, boost::posix_time::ptime& b);
    }; // namespace impl
}; // namespace tide

/// @}
// group implementations

#endif // !defined(DATE_ELEMENT_IMPL_H_

