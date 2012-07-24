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

#if !defined(PRIMITIVE_ELEMENT_BASE_H_)
#define PRIMITIVE_ELEMENT_BASE_H_

#include <jonen/primitive_element.h>
#include <jonen/win_dll.h>

/// \addtogroup interfaces Interfaces
/// @{

namespace jonen
{
    /** \brief Standard implementations of the PrimitiveElement interface
     * methods.
     *
     * This template interface provides the common implementations of the
     * PrimitiveElement interface methods used by most primitive elements.
     *
     * This interface provides default implementations of many of the
     * PrimitiveElement interface's methods, accessible using CRTP. An
     * implementing class that does not override all virtual methods must
     * provide the following members:
     *
     * - \code
     *   T value_;
     *   \endcode
     *   Storage for the element's value.
     * - \code
     *   T default_;
     *   \endcode
     *   Storage for the element's default value.
     * - \code
     *   bool has_default_;
     *   \endcode
     *   Storage for a boolean tracking if the default value has been set or
     *   not.
     *
     * Additionally, the implementing class must declare this class as a
     * friend.
     */
    template<class Derived, typename T>
    class JONEN_EXPORT PrimitiveElementBase
        : public PrimitiveElement<T>
    {
        private:
            /** \brief Curiously Recurring Template Pattern: get a reference to
             * this in the derived class's type.
             */
            Derived& derived()
            {
                return *static_cast<Derived*>(this);
            }

            /** \brief Curiously Recurring Template Pattern: get a constant
             * reference to this in the derived class's type.
             */
            Derived const& derived() const
            {
                return *static_cast<Derived const*>(this);
            }

        protected:
            /** \brief Used by derived classes when they need to know the exact
             * base type.
             */
            typedef PrimitiveElementBase<Derived, T> PrimitiveElementBase_;

        public:
            /// \brief Destructor.
            virtual ~PrimitiveElementBase() {};

        private:
            /// \brief Implementation of the value() method.
            virtual T value_impl() const { return derived().value_; }
            /// \brief Implementation of the value(T) method.
            virtual void value_impl(T value) { derived().value_ = value; }

            /// \brief Implementation of the has_default() method.
            virtual bool has_default_impl() const
                { return derived().has_default_; }
            /// \brief Implementation of the get_default() method.
            virtual T get_default_impl() const { return derived().default_; }
            /// \brief Implementation of the set_default(T) method.
            virtual void set_default_impl(T default_value)
            {
                derived().default_ = default_value;
                derived().has_default_ = true;
            }
            /// \brief Implementation of the remove_default() method.
            virtual T remove_default_impl()
            {
                derived().has_default_ = false;
                return derived().default_;
            }
            /// \brief Implementation of the is_default() method.
            virtual bool is_default_impl() const
            {
                return derived().has_default_ &&
                    derived().value_ == derived().default_;
            }
    }; // class PrimitiveElement
}; // namespace jonen

/// @}
/// group interfaces

#endif // !defined(PRIMITIVE_ELEMENT_BASE_H_)

