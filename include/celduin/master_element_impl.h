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

#if !defined(MASTER_ELEMENT_IMPL_H_)
#define MASTER_ELEMENT_IMPL_H_

#include <celduin/master_element_base.h>
#include <celduin/win_dll.h>

#include <ios>

/// \addtogroup implementations Internal implementations
/// @{

namespace celduin
{
    namespace impl
    {
        /** \brief Generic implementation of the master element functionality.
         *
         * This class implements the MasterElementBase CRTP requirements.
         */
        class MasterElementImpl
            : public MasterElementBase<MasterElementImpl>
        {
            friend class MasterElementBase<MasterElementImpl>;

            public:
                /** \brief Constructor.
                 *
                 * \param[in] use_crc The initial value of the flag to
                 * enable/disable using the CRC.
                 */
                MasterElementImpl(bool use_crc);

                /** \brief Swap this object with another instance.
                 *
                 * \param[in] other The other instance to swap with.
                 */
                void swap(MasterElementImpl& other);

                /** \brief Get the size of the CRC value when stored.
                 *
                 * This function will return the number of bytes required to
                 * store the CRC value, if using the CRC is enabled. Otherwise,
                 * it will return zero.
                 */
                std::streamsize crc_size() const;

                /** \brief Read and check the CRC value.
                 *
                 * This function will read a CRC value from the file and check
                 * that it is correct for the calling element.
                 *
                 * This function will also set the CRC enable flag to on so
                 * that, should the element be written to a file again, the CRC
                 * will be maintained.
                 *
                 * \exception BadCRCError if the CRC value does not match the
                 * calling element.
                 */
                std::streamsize read_crc(std::istream& o);

                /** \brief Calculate and write the CRC value.
                 *
                 * This function will calculate the CRC value for the calling
                 * element, then write it to the file.
                 * This function will only do anything when using the CRC is
                 * enabled. Otherwise, it is a no-op.
                 *
                 * \return The number of bytes written.
                 */
                std::streamsize write_crc(std::ostream& o);

            private:
                ///////////////////////////////////////////////////////////////////
                // MasterElementBase CRTP required members
                bool use_crc_;
        }; // class MasterElementImpl
    }; // namespace impl
}; // namespace celduin

/// @}
/// group implementations

#endif // !defined(MASTER_ELEMENT_IMPL_H_)
