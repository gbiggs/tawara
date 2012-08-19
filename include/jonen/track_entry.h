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

#if !defined(TRACK_ENTRY_H_)
#define TRACK_ENTRY_H_

#include <jonen/binary_element.h>
#include <jonen/element_base.h>
#include <jonen/float_element.h>
#include <jonen/integer_elements.h>
#include <jonen/master_element.h>
#include <jonen/string_element.h>
#include <jonen/win_dll.h>

#include <boost/operators.hpp>
#include <boost/scoped_ptr.hpp>
#include <string>
#include <vector>

/// \addtogroup elements Elements
/// @{

namespace jonen
{
    /** \brief Meta-data about a single track in the segment.
     *
     * This element stores all the information necessary to understand the data
     * in a single track in the segment. Such data includes the track's number,
     * its UID, the scale of its time codes and the encoding of the data.
     *
     * The TrackEntry element may optionally store private binary data. This
     * can be used for any purpose. For example, it can store a text
     * representation of the structure of the encoded data, or even an
     * implementation of a decoder for that data.
     *
     * TrackEntry elements are compared based only on their track number.
     *
     * The TrackEntry element uses a CRC32 value by default.
     *
     * This class implements the Element interface.
     *
     * This class implements the MasterElement interface.
     */
    class JONEN_EXPORT TrackEntry
        : public ElementBase<TrackEntry>,
        public MasterElement,
        boost::equality_comparable<TrackEntry>,
        boost::less_than_comparable<TrackEntry>
    {
        friend class ElementBase<TrackEntry>;

        public:
            /// \brief Allowable track types.
            enum TrackType
            {
                Subtitles = 0x11, ///< A subtitles track.
                Data = 0x70, ///< A data track.
            };

            /// \brief Constructor.
            TrackEntry();

            /// \brief Copy constructor.
            TrackEntry(TrackEntry const& rhs);

            /// \brief Assignment operator.
            TrackEntry& operator=(TrackEntry const& rhs);

            /// \brief Destructor.
            ~TrackEntry();

            /** \brief Swap this element with another.
             *
             * \param[in] other The other element to swap with.
             */
            void swap(TrackEntry& other);

            /// \brief Equality operator
            friend bool operator==(TrackEntry const& lhs, TrackEntry const& rhs);

            /// \brief Less-than operator
            friend bool operator<(TrackEntry const& lhs, TrackEntry const& rhs);

            /// \brief Get the track's number.
            UIntElement const& number() const;
            /// \brief Set the track's number.
            void number(UIntElement const& number);
            /// \brief Set the track's number.
            void number(unsigned long long int number);

            /// \brief Get the track's UID.
            UIntElement const& uid() const;
            /// \brief Set the track's UID.
            void uid(UIntElement const& uid);
            /// \brief Set the track's UID.
            void uid(unsigned long long int uid);

            /// \brief Get the track's type.
            TrackType type() const;
            /// \brief Set the track's type.
            void type(TrackType type);

            /** \brief Check if the track is enabled.
             *
             * Disabled tracks should be ignored.
             */
            bool is_enabled() const;
            /// \brief Enable the track.
            void enable();
            /// \brief Disable the track.
            void disable();

            /** \brief Check if the track is forced.
             *
             * Forced tracks must be used during playback.
             */
            bool is_forced() const;
            /// \brief Enable forcing the track.
            void enable_forced();
            /// \brief Disable forcing the track.
            void disable_forced();

            /// \brief Check if the track may contain blocks using lacing.
            bool may_lace() const;
            /// \brief Enable the potential use of lacing in this track.
            void allow_lacing();
            /// \brief Disable using lacing in this track.
            void disallow_lacing();

            /// \brief Get the track's name.
            StringElement const& name() const;
            /// \brief Set the track's name.
            void name(StringElement const& name);
            /// \brief Set the track's name.
            void name(std::string const& name);

            /// \brief Get the time code scale.
            FloatElement const& timecode_scale() const;
            /// \brief Set the time code scale.
            void timecode_scale(FloatElement const& tc_scale);
            /// \brief Set the time code scale.
            void timecode_scale(unsigned long long int const& tc_scale);

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
    }; // class TrackEntry


    /** \brief Equality operator
     *
     * Track entries are compared based on their track number.
     */
    bool operator==(TrackEntry const& lhs, TrackEntry const& rhs);


    /** \brief Less-than operator
     *
     * Track entries are compared based on their track number.
     */
    bool operator<(TrackEntry const& lhs, TrackEntry const& rhs);


    /// \brief Swap elements
    void swap(TrackEntry& a, TrackEntry& b);


    /// \brief Stream output operator.
    std::ostream& operator<<(std::ostream& o, TrackEntry const& rhs);
}; // namespace jonen

/// @}
// group elements

#endif // !defined(TRACK_ENTRY_H_)

