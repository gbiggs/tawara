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

#if !defined(METASEEK_H_)
#define METASEEK_H_

#include <jonen/element_base.h>
#include <jonen/master_element.h>
#include <jonen/win_dll.h>

#include <boost/operators.hpp>
#include <boost/scoped_ptr.hpp>

/// \addtogroup interfaces Interfaces
/// @{

namespace jonen
{
    /** \brief The Metaseek element, used as an index for a Jonen file.
     *
     * Generally, any Jonen segment should contain one, and only one, instance
     * of a Metaseek element. This element should provide the byte offsets in
     * the data stream of the other Level 1 elements (SegmentInfo, Tracks,
     * etc.) in the segment. This function also means that the Metaseek element
     * should be placed first in its segment, so that it can be found quickly
     * to speed up file loading.
     *
     * When writing Jonen files, it is advisable to reserve some space at the
     * start of the segment for the Metaseek element to be written in once the
     * file is complete.
     *
     * This class implements the Element interface.
     *
     * This class implements the MasterElement interface.
     */
    class JONEN_EXPORT Metaseek :
        public ElementBase<Metaseek>,
        public MasterElement,
        boost::equality_comparable<Metaseek>
    {
        friend class ElementBase<Metaseek>;

        public:
            /// \brief Constructor.
            Metaseek();

            /// \brief Copy constructor.
            Metaseek(Metaseek const& rhs);

            /// \brief Assignment operator.
            Metaseek& operator=(Metaseek const& rhs);

            /// \brief Destructor.
            ~Metaseek();

            /** \brief Swap this element with another.
             *
             * \param[in] other The other element to swap with.
             */
            void swap(Metaseek& other);

            /// \brief Equality operator
            bool operator==(Metaseek const& rhs);

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
    }; // class Metaseek


    /// \brief Swap EBML header elements
    void swap(Metaseek& a, Metaseek& b);


    /// \brief Stream output operator.
    std::ostream& operator<<(std::ostream& o, Metaseek const& rhs);
}; // namespace jonen

/// @}
/// group interfaces

#endif // !defined(METASEEK_H_)

