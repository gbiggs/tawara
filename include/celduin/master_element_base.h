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

#if !defined(MASTER_ELEMENT_BASE_H_)
#define MASTER_ELEMENT_BASE_H_

#include <celduin/master_element.h>
#include <celduin/win_dll.h>


/// \addtogroup interfaces Interfaces
/// @{

namespace celduin
{
    /** \brief Standard implementations of the MasterElement interface methods.
     *
     * This template interface provides the common implementations of the
     * MasterElement interface methods used by most master elements.
     *
     * As a template interface, this class places requirements on classes that
     * implement it. An implementing class should inherit from this class,
     * passing itself as the template parameter. The implementing class must
     * provide the following members:
     *
     * - \code
     *   bool use_crc_;
     *   \endcode
     *   A flag to track if the master element will write a CRC value when its
     *   body is written. This should be initialised appropriately for the
     *   element.
     *
     * Additionally, the implementing class must declare this class as a
     * friend.
     *
     * \tparam Derived The derived class implementing this template interface.
     */
    template<class Derived>
    class CELDUIN_EXPORT MasterElementBase : public MasterElement
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
            typedef MasterElementBase<Derived> MasterElementBase_;

        private:
            ///////////////////////////////////////////////////////////////////
            // MasterElement interface virtual functions implementation

            /// \brief Enable the CRC value.
            virtual void enable_crc_impl()
            {
                derived().use_crc_ = true;
            }

            /// \brief Disable the CRC value.
            virtual void disable_crc_impl()
            {
                derived().use_crc_ = false;
            }

            /// \brief Check if the CRC value is enabled.
            virtual bool crc_enabled_impl() const
            {
                return derived().use_crc_;
            }
    }; // class MasterElementBase
}; // namespace celduin

/// @}
/// group interfaces

#endif // !defined(MASTER_ELEMENT_BASE_H_)

