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
#include <vector>

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
                explicit MasterElementImpl(bool use_crc);

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
                 * Note that checking the CRC is correct requires reading the
                 * element's body, which must then be read again by the calling
                 * element if it wants that body. If reading twice impacts
                 * performance more than the memory required to store the
                 * entire element body, then read_with_crc() should be used
                 * instead.
                 *
                 * \param[out] body A stringstream to place the body data in
                 * after checking its CRC32 value.
                 * \param[in] i The input stream to read data from.
                 * \param[in] size The size of the entire element's body,
                 * including any CRC-32 value if present.
                 * \exception BadCRC if the CRC value does not match the
                 * calling element.
                 * \pre The read pointer of i will be at the first byte of the
                 * element's body, which may or may not be a CRC-32 element.
                 * \post The read pointer of i will be at the first byte of
                 * the element's remaining body, whether or not a CRC-32 value
                 * was present.
                 * \post crc_enabled() will return true.
                 */
                std::streamsize read_crc(std::istream& i,
                        std::streamsize size);

                /** \brief Read and check the CRC value and read the element
                 * body.
                 *
                 * This function will read a CRC value from the file and check
                 * that it is correct for the calling element. It will
                 * read the remainder of the element body as well, storing it
                 * in a string passed to this function.
                 *
                 * This function will also set the CRC enable flag to on so
                 * that, should the element be written to a file again, the CRC
                 * will be maintained.
                 *
                 * \param[out] body A stringstream to place the body data in
                 * after checking its CRC32 value.
                 * \param[in] i The input stream to read data from.
                 * \exception BadCRC if the CRC value does not match the
                 * calling element.
                 * \param[in] size The size of the entire element's body,
                 * including any CRC-32 value if present.
                 * \pre The read pointer of i will be at the first byte of the
                 * element's body, which may or may not be a CRC-32 element.
                 * \post The read pointer of i will be at the first byte after
                 * the element's body.
                 * \post crc_enabled() will return true.
                 * \post The body of the element, excluding the CRC32 element,
                 * will be stored in the vector referenced by body.
                 */
                std::streamsize read_with_crc(std::vector<char>& body,
                        std::istream& i, std::streamsize size);

                /** \brief Calculate and write the CRC value.
                 *
                 * This function will calculate the CRC value for the calling
                 * element, then write it to the file. It \emph will \emph not
                 * write the body data. This function will only do anything
                 * when using the CRC is enabled. Otherwise, it is a no-op.
                 *
                 * \param[in] body The data of the element as it will appear in
                 * the file. The CRC-32 value will be calculated from this.
                 * \param[in] io The destination stream to write to.
                 * \return The number of bytes written.
                 * \pre body is an empty vector.
                 * \post The write pointer of io will be placed after the
                 * CRC-32 element's body, such that the element's body can be
                 * written.
                 */
                std::streamsize write_crc(std::vector<char> const& body,
                        std::iostream& io);

                /** \brief Calculate and write the CRC value and element body.
                 *
                 * This function will calculate the CRC value for the calling
                 * element, then write it to the file. It \emph will write the
                 * body data. This function will only write the body when using
                 * the CRC is disabled.
                 *
                 * \param[in] body The data of the element as it will appear in
                 * the file. The CRC-32 value will be calculated from this.
                 * \param[in] io The destination stream to write to.
                 * \return The number of bytes written.
                 * \post The write pointer of io will be placed after the end
                 * of the element's body.
                 */
                std::streamsize write_with_crc(std::vector<char> const& body,
                        std::iostream& io);
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
