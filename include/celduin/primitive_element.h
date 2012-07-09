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

#if !defined(PRIMITIVE_ELEMENT_H_)
#define PRIMITIVE_ELEMENT_H_

#include <celduin/win_dll.h>

/// \addtogroup interfaces Interfaces
/// @{

namespace celduin
{
    /** \brief Interface to a primitive element.
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
     * - Raw binary blobs.
     *
     * A default value can be specified for the element. If a default is given,
     * it may allow the element to be skipped when writing to a store.
     *
     * Any type specified for T must initialise itself upon construction, be
     * copy-constructable, have an assignment operator, and perform its own
     * clean-up in its destructor. POD types qualify for this.
     *
     * This interface provides the interface common to all primitive elements.
     * This includes equality comparisons (\e Note that behavioural equality
     * for a primitive element is defined as being the behaviour of the type
     * T). Other interactions with instances of the stored type must be
     * supplied separately.
     */
    template<typename T>
    class CELDUIN_EXPORT PrimitiveElement
    {
        public:
            /// \brief Destructor.
            virtual ~PrimitiveElement() {};

            /// \brief Get the value.
            T value() const { return value_impl(); }
            /// \brief Set the value.
            void value(T value) { value_impl(value); }

            /** \brief Check if a default value is set.
             *
             * If a default value is set, and the current value is the default
             * value, this element may be skipped when writing to a store. When
             * reading from a store, if this element is not found in its parent
             * element, the default value is available.
             */
            bool has_default() const { return has_default_impl(); }
            /// \brief Get the default value.
            T get_default() const { return get_default_impl(); }
            /// \brief Set the default value.
            void set_default(T default_value)
                { set_default_impl(default_value); }
            /** \brief Remove the default value.
             *
             * \return The value of the default that was removed.
             */
            T remove_default() { return remove_default_impl(); }
            /** Check if this element is at the default value.
             *
             * If the current value is the same as the default value, this
             * element may not need to be stored when being written.
             */
            bool is_default() const { return is_default_impl(); }

            /// \brief Equality operator.
            friend bool operator==(PrimitiveElement const& lhs,
                    PrimitiveElement const& rhs)
            {
                return lhs.value() == rhs.value();
            }

            /// \brief Equality operator.
            friend bool operator==(PrimitiveElement const& lhs, T rhs)
            {
                return lhs.value() == rhs;
            }

            /// \brief Equality operator.
            friend bool operator==(T lhs, PrimitiveElement const& rhs)
            {
                return lhs == rhs.value();
            }

        private:
            /// \brief Implementation of the value() method.
            virtual T value_impl() const = 0;
            /// \brief Implementation of the value(T) method.
            virtual void value_impl(T value) = 0;

            /// \brief Implementation of the has_default() method.
            virtual bool has_default_impl() const = 0;
            /// \brief Implementation of the get_default() method.
            virtual T get_default_impl() const = 0;
            /// \brief Implementation of the set_default(T) method.
            virtual void set_default_impl(T default_value) = 0;
            /// \brief Implementation of the remove_default() method.
            virtual T remove_default_impl() = 0;
            /// \brief Implementation of the is_default() method.
            virtual bool is_default_impl() const = 0;
    }; // class PrimitiveElement
}; // namespace celduin

/// @}
/// group interfaces

#endif // !defined(PRIMITIVE_ELEMENT_H_)

