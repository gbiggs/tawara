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

#if !defined(SEGMENT_INFO_H_)
#define SEGMENT_INFO_H_

#include <jonen/binary_element.h>
#include <jonen/date_element.h>
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
    /** \brief The Segment Information element.
     *
     * The Segment Information element provides the important meta-data about a
     * segment in a Jonen document, such as the title, date, timecode scale
     * and links to other segments.
     *
     * Some values in the Segment Information cannot be written until after the
     * segment's data is known:
     *  - Duration
     * When writing a segment, it may be necessary to write part of the Segment
     * Information after the segment's data has been written. When writing,
     * enough space should be left at the start of the file to write the
     * Segment Information after filling in the segment's data. Often the
     * Segment Information can be written with a place-holder value for the
     * Duration at the start of writing, and then over-written with the correct
     * value after the segment's data has been written.
     *
     * A UID will be generated automatically for new SegmentInfo instances. It
     * can be retrieved as a binary blob for use in other segments.
     *
     * Most values in the Segment Information element are not required. For a
     * list of which values will not be written to the file unless set, see the
     * Jonen format specification.
     *
     * The SegmentInfo element uses a CRC32 value by default.
     *
     * This class implements the Element interface.
     *
     * This class implements the MasterElement interface.
     */
    class JONEN_EXPORT SegmentInfo
        : public ElementBase<SegmentInfo>,
        public MasterElement,
        boost::equality_comparable<SegmentInfo>
    {
        friend class ElementBase<SegmentInfo>;

        public:
            /// \brief Constructor.
            SegmentInfo();

            /// \brief Copy constructor.
            SegmentInfo(SegmentInfo const& rhs);

            /// \brief Assignment operator.
            SegmentInfo& operator=(SegmentInfo rhs);

            /// \brief Destructor.
            ~SegmentInfo();

            /** \brief Swap this element with another.
             *
             * \param[in] other The other element to swap with.
             */
            void swap(SegmentInfo& other);

            /// \brief Equality operator
            bool operator==(SegmentInfo const& rhs);

            /// \brief Get the segment's UID.
            BinaryElement const& uid() const;
            /// \brief Set the segment's UID.
            void uid(BinaryElement const& uid);
            /// \brief Set the segment's UID.
            void uid(std::vector<char> const& uid);

            /// \brief Get the segment's file name.
            StringElement const& filename() const;
            /// \brief Set the segment's file name.
            void filename(StringElement const& filename);
            /// \brief Set the segment's file name.
            void filename(std::string const& filename);

            /** \brief Get the UID of the previous segment in the chain.
             *
             * If there is no previous segment, this value will be empty.
             */
            BinaryElement const& previous_uid() const;
            /// \brief Set the UID of the previous segment in the chain.
            void previous_uid(BinaryElement const& previous_uid);
            /// \brief Set the UID of the previous segment in the chain.
            void previous_uid(std::vector<char> const& previous_uid);

            /** \brief Get the file name of the file containing the previous
             * segment in the chain.
             *
             * If there is no previous segment, this value will be empty.
             */
            StringElement const& previous_fn() const;
            /// \brief Set the file name of the file containing the previous
            void previous_fn(StringElement const& prev_fn);
            /// \brief Set the file name of the file containing the previous
            void previous_fn(std::string const& prev_fn);

            /** \brief Get the UID of the next segment in the chain.
             *
             * If there is no next segment, this value will be empty.
             */
            BinaryElement const& next_uid() const;
            /// \brief Set the UID of the next segment in the chain.
            void next_uid(BinaryElement const& next_uid);
            /// \brief Set the UID of the next segment in the chain.
            void next_uid(std::vector<char> const& next_uid);

            /** \brief Get the file name of the file containing the next
             * segment in the chain.
             *
             * If there is no next segment, this value will be empty.
             */
            StringElement const& next_fn() const;
            /// \brief Set the file name of the file containing the next
            void next_fn(StringElement const& next_fn);
            /// \brief Set the file name of the file containing the previous
            void next_fn(std::string const& next_fn);

            /** \brief Get the segment family UID(s).
             *
             * All segments with the same family UID are considered part of a
             * chain.
             *
             * A segment may be a member of multiple families.
             */
            std::vector<std::vector<char> > families() const;
            /// \brief Add a segment family UID.
            void add_family(BinaryElement const& family);
            /// \brief Add a segment family UID.
            void add_family(std::vector<char> const& family);
            /// \brief Remove a segment family UID.
            void remove_family(BinaryElement const& family);
            /// \brief Remove a segment family UID.
            void remove_family(std::vector<char> const& family);

            /** \brief Get the time code scale.
             *
             * This value is measured in nanoseconds.
             */
            UIntElement const& timecode_scale() const;
            /// \brief Set the time code scale.
            void timecode_scale(UIntElement const& tc_scale);
            /// \brief Set the time code scale.
            void timecode_scale(unsigned long long int const& tc_scale);

            /** \brief Get the duration of the segment.
             *
             * This value is measured in the units defined by the time code
             * scale.
             *
             * Typically, this value won't be known until the segment is
             * complete. File writing may therefore require that the
             * Segment Information is written twice, with the second write
             * over-writing the first and containing the final values.
             */
            FloatElement const& duration() const;
            /// \brief Set the duration of the segment.
            void duration(FloatElement const& duration);
            /// \brief Set the duration of the segment.
            void duration(double const& duration);

            /** \brief Get the date of the origin of the segment.
             *
             * The segment's date is given in UTC as the number of seconds
             * since 2001-01-01T00:00:00,0 UTC. The value is signed, allowing
             * dates before the origin.
             *
             * The first cluster takes place at the origin.
             */
            DateElement const& date() const;
            /// \brief Set the date of the origin of the segment.
            void date(DateElement const& date);
            /// \brief Set the date of the origin of the segment.
            void date(boost::posix_time::ptime const& date);

            /// \brief Get the title of the segment.
            StringElement const& title() const;
            /// \brief Set the title of the segment.
            void title(StringElement const& title);
            /// \brief Set the title of the segment.
            void title(std::string const& title);

            /// \brief Get the name of the muxing application or library.
            StringElement const& muxing_app() const;
            /// \brief Set the name of the muxing application or library.
            void muxing_app(StringElement const& muxing_app);
            /// \brief Set the name of the muxing application or library.
            void muxing_app(std::string const& muxing_app);

            /// \brief Get the name of the writing application or library.
            StringElement const& writing_app() const;
            /// \brief Set the name of the writing application or library.
            void writing_app(StringElement const& writing_app);
            /// \brief Set the name of the writing application or library.
            void writing_app(std::string const& writing_app);

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
    }; // class SegmentInfo


    /// \brief Stream output operator.
    std::ostream& operator<<(std::ostream& o, SegmentInfo const& rhs);
}; // namespace jonen


namespace std
{
    /// \brief Swap elements
    template<> void swap<jonen::SegmentInfo>(jonen::SegmentInfo& a,
            jonen::SegmentInfo& b)
    {
        a.swap(b);
    }
}; // namespace std

/// @}
// group elements

#endif // !defined(SEGMENT_INFO_H_)

