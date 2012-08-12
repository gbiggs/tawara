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

#if !defined(EBML_HEADER_H_)
#define EBML_HEADER_H_

#include <jonen/binary_element.h>
#include <jonen/element_base.h>
#include <jonen/integer_elements.h>
#include <jonen/master_element.h>
#include <jonen/win_dll.h>

#include <boost/operators.hpp>
#include <boost/scoped_ptr.hpp>

/// \addtogroup elements Elements
/// @{

namespace jonen
{
    namespace impl
    {
        /** \brief An entry in the Metaseek index.
         *
         * Each intry in the Metaseek index contains an ID and the byte-offset
         * in the segment that an element with that ID is located at. An ID may
         * occur in the index multiple times, indicating that there are
         * multiple elements with that ID in the segment.
         *
         * The Seek element does not use a CRC32 value by default.
         *
         * This class implements the Element interface.
         *
         * This class implements the MasterElement interface.
         */
        class JONEN_EXPORT Seek
            : public ElementBase<Seek>,
            public MasterElement,
            boost::equality_comparable<Seek>
        {
            friend class ElementBase<Seek>;

            public:
                /** \brief Constructor.
                 *
                 * \param[in] id The ID of the element to be indexed.
                 * \param[in] offset The byte-offset in the segment of the
                 * element.
                 */
                Seek(ids::ID id, std::streamoff offset);

                /** \brief Constructor.
                 *
                 * \param[in] id The ID of the element to be indexed.
                 * \param[in] offset The byte-offset in the segment of the
                 * element.
                 */
                Seek(BinaryElement const& id, std::streamoff offset);

                /** \brief Constructor.
                 *
                 * \param[in] id The ID of the element to be indexed.
                 * \param[in] offset The byte-offset in the segment of the
                 * element.
                 */
                Seek(ids::ID id, UIntElement const& offset);

                /** \brief Constructor.
                 *
                 * \param[in] id The ID of the element to be indexed.
                 * \param[in] offset The byte-offset in the segment of the
                 * element.
                 */
                Seek(BinaryElement const& id, UIntElement const& offset);

                /// \brief Copy constructor.
                Seek(Seek const& rhs);

                /// \brief Assignment operator.
                Seek& operator=(Seek const& rhs);

                /// \brief Destructor.
                ~Seek();

                /** \brief Swap this element with another.
                 *
                 * \param[in] other The other element to swap with.
                 */
                void swap(Seek& other);

                /// \brief Equality operator
                bool operator==(Seek const& rhs);

                /// \brief Get the ID of the indexed element.
                ids::ID element_id() const;
                /// \brief Set the ID of the indexed element.
                void element_id(ids::ID id);
                /// \brief Set the ID of the indexed element.
                void element_id(BinaryElement const& id);

                /// \brief Get the offset of the indexed element.
                UIntElement const& segment_offset() const;
                /// \brief Set the offset of the indexed element.
                void segment_offset(std::streamoff offset);
                /// \brief Set the offset of the indexed element.
                void segment_offset(UIntElement const& offset);

            private:
                // Pimpl member
                class Impl;
                boost::scoped_ptr<Impl> pimpl_;
                // Must be mutable to be updated in start_body()
                mutable std::streampos body_end_;

                ///////////////////////////////////////////////////////////////////
                // ElementBase CRTP required members
                ids::ID id_;
                mutable std::streampos offset_;
                mutable bool writing_;

                std::streamsize body_stored_size() const;

                std::streamsize read_body(std::istream& i, std::streamsize size);

                std::streamsize start_body(std::iostream& io) const;

                std::streamsize finish_body(std::iostream& io) const;

                ///////////////////////////////////////////////////////////////////
                // MasterElement interface implementation
                void enable_crc_impl();

                void disable_crc_impl();

                bool crc_enabled_impl() const;
        }; // class Seek


        /// \brief Swap Seek elements
        void swap(Seek& a, Seek& b);


        /// \brief Stream output operator.
        std::ostream& operator<<(std::ostream& o, Seek const& rhs);
    }; // namespace impl
}; // namespace jonen

/// @}
// group elements

#endif // !defined(EBML_HEADER_H_)

