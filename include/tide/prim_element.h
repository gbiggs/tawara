/* TIDE
 *
 * Header file for the primitive data element object.
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

#if !defined(TIDE_PRIM_ELEMENT_H_)
#define TIDE_PRIM_ELEMENT_H_

#include <tide/element.h>
#include <tide/exceptions.h>
#include <tide/win_dll.h>

#include <stdint.h>
#include <string>

/// \addtogroup interfaces Interfaces
/// @{

namespace tide
{
    /** \brief The primitive data element interface.
     *
     * Primitive elements store one of the EBML primitive data types. These
     * are:
     *
     * - Signed integers, up to 8 bytes.
     * - Unsigned integers, up to 8 bytes.
     * - IEEE 4-byte and 8-byte floats.
     * - Strings, with or without null-byte padding on the end.
     * - Dates, represented as a 64-bit integer giving the number of
     *   nanoseconds since 2001-01-01 00:00:00.
     * - Raw binary data.
     *
     * A default value can be specified for the element. If a default is given,
     * it may allow the element to be skipped when writing to a store.
     *
     * Any type specified for T must initialise itself upon construction, be
     * copy-constructable, have an assignment operator, and perform its own
     * clean-up in its destructor. POD types qualify for this.
     */
    template<typename T>
    class TIDE_EXPORT PrimitiveElement : public Element
    {
        public:
            /** \brief Create a new element with no default.
             *
             * \param[in] id The element's ID, as an unsigned integer up to 28
             * bits.
             * \param[in] value The element's value.
             */
            PrimitiveElement(uint32_t id, T value)
                : Element(id),
                value_(value), has_default_(false)
            {
            }

            /** \brief Create a signed integer element with a default value.
             *
             * \param[in] id The element's ID, as an unsigned integer up to 28
             * bits.
             * \param[in] value The element's value.
             * \param[in] default_value The default value of the element.
             */
            PrimitiveElement(uint32_t id, T value, T default_value)
                : Element(id),
                value_(value), default_(default_value), has_default_(true)
            {
            }

            /// \brief Value assignment operator.
            virtual PrimitiveElement& operator=(T const& rhs)
            {
                value_ = rhs;
                return *this;
            }

            /// Get the element's ID.
            virtual uint32_t id() const { return Element::id(); }

            /** \brief Set the element's ID.
             *
             * \param[in] id The element's new ID, as an unsigned integer up to
             * 28 bits.
             */
            virtual void id(uint32_t id)
            {
                if (id == 0 ||
                        id == 0xFF ||
                        id == 0xFFFF ||
                        id == 0xFFFFFF ||
                        id == 0xFFFFFFFF)
                {
                    throw InvalidElementID() << err_id(id);
                }
                id_ = id;
            }

            /// \brief Get the value.
            virtual T value() const { return value_; }
            /// \brief Set the value.
            virtual void value(T value) { value_ = value; }

            /// \brief Check if a default value is set.
            virtual bool has_default() const { return has_default_; }
            /// \brief Get the default value.
            virtual T get_default() const { return default_; }
            /// \brief Set the default value.
            virtual void set_default(T default_value)
            {
                default_ = default_value;
                has_default_ = true;
            }
            /** \brief Remove the default value.
             *
             * \return The value of the default that was removed.
             */
            virtual T remove_default()
            {
                has_default_ = false;
                return default_;
            }
            /** Check if this element is at the default value.
             *
             * If the current value is the same as the default value, this
             * element may not need to be stored when being written.
             */
            virtual bool is_default() const
                { return value_ == default_ && has_default_; }

        protected:
            T value_;
            T default_;
            bool has_default_;
    }; // class Element
}; // namespace tide

/// @}
/// group interfaces

#endif // TIDE_PRIM_ELEMENT_H_

