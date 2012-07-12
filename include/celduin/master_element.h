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

#if !defined(MASTER_ELEMENT_H_)
#define MASTER_ELEMENT_H_

#include <celduin/win_dll.h>

/// \addtogroup interfaces Interfaces
/// @{

namespace celduin
{
    /** \brief Interface to a master element.
     *
     * Master elements use primitive elements for their bodies. They do not
     * directly store data themselves.
     *
     * This interface defines the common methods that all master elements
     * support.
     */
    class CELDUIN_EXPORT MasterElement
    {
        public:
            /// \brief Destructor.
            virtual ~MasterElement() {};

            /** \brief Enable using a CRC value when writing the master element
             * to a stream.
             */
            void enable_crc() { enable_crc_impl(); }

            /** \brief Disable using a CRC value when writing the master
             * element to a stream.
             */
            void disable_crc() { disable_crc_impl(); }

            /// \brief Check if using the CRC is enabled.
            bool crc_enabled() const { return crc_enabled_impl(); }

        private:
            /** \brief Implementation of the enable_crc() method.
             *
             * Implementing classes must implement this to enable writing the
             * CRC value.
             */
            virtual void enable_crc_impl() = 0;

            /** \brief Implementation of the disable_crc() method.
             *
             * Implementing classes must implement this to disable writing the
             * CRC value.
             */
            virtual void disable_crc_impl() = 0;

            /** \brief Implementation of the use_crc() method.
             *
             * Implementing classes must implement this to return whether the
             * CRC value will be written.
             */
            virtual bool crc_enabled_impl() const = 0;
    }; // class MasterElement
}; // namespace celduin

/// @}
/// group interfaces

#endif // !defined(ELEMENT_H_)

