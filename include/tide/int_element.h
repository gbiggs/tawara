/* TIDE
 *
 * Header file for the signed integer element object.
 *
 * Copyright 2011 Geoffrey Biggs geoffrey.biggs@aist.go.jp
 *     RT-Synthesis Research Group
 *     Intelligent Systems Research Institute,
 *     National Institute of Advanced Industrial Science and Technology (AIST),
 *     Japan
 *     All rights reserved.
 *
 * This file is part of TIDE.
 *
 * TIDE is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * TIDE is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with TIDE. If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined(TIDE_INT_ELEMENT_H_)
#define TIDE_INT_ELEMENT_H_

#include <tide/element.h>
#include <tide/win_dll.h>

#include <stdint.h>

/// \addtogroup interfaces Interfaces
/// @{

namespace tide
{
    /** \brief The signed integer element interface.
     *
     * This element stores an signed integer. The maximum size of the value
     * is 2^64.
     */
    class TIDE_EXPORT IntElement : public Element
    {
        public:
            /** \brief Create a new signed integer element with no default.
             *
             * \param[in] id The element's ID, as an signed integer up to 28
             * bits.
             * \param[in] value The element's value.
             */
            IntElement(uint32_t id, int64_t value);
            /** \brief Create a signed integer element with a default value.
             *
             * \param[in] id The element's ID, as an signed integer up to 28
             * bits.
             * \param[in] value The element's value.
             * \param[in] default_value The default value of the element.
             */
            IntElement(uint32_t id, int64_t value, int64_t default_value);

            /// \brief Copy constructor.
            IntElement(IntElement const& rhs);

            /// \brief Destructor.
            virtual ~IntElement() {};

            /// \brief Assignment operator.
            virtual IntElement& operator=(IntElement const& rhs);

            /// \brief Get the value.
            virtual int64_t value() const { return value_; }
            /** \brief Set the value.
             *
             * A signed integer element can hold up to 64 bits.
             */
            virtual void value(int64_t value) { value_ = value; }

            /// \brief Check if a default value is set.
            virtual bool has_default() const { return has_default_; }
            /// \brief Get the default value.
            virtual int64_t get_default() const { return default_; }
            /// \brief Set the default value.
            virtual void set_default(int64_t default_value);
            /** \brief Remove the default value.
             *
             * \return The value of the default that was removed.
             */
            virtual int64_t remove_default();
            /** Check if this element is at the default value.
             *
             * If the current value is the same as the default value, this
             * element may not need to be stored when being written.
             */
            virtual bool is_default() const
                { return value_ == default_ && has_default_; }

        private:
            int64_t value_;
            int64_t default_;
            bool has_default_;
    }; // class Element
}; // namespace tide

/// @}
/// group interfaces

#endif // TIDE_INT_ELEMENT_H_

