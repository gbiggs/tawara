/* Tide
 *
 * Header for the Segment element.
 *
 * Copyright 2011 Geoffrey Biggs geoffrey.biggs@aist.go.jp
 *     RT-Synthesis Research Group
 *     Intelligent Systems Research Institute,
 *     National Institute of Advanced Industrial Science and Technology (AIST),
 *     Japan
 *     All rights reserved.
 *
 * This file is part of Tide.
 *
 * Tide is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * Tide is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Tide. If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined(TIDE_SEGMENT_H_)
#define TIDE_SEGMENT_H_

#include <map>
#include <tide/master_element.h>
#include <tide/metaseek.h>
#include <tide/segment_info.h>
#include <tide/win_dll.h>

/// \addtogroup elements Elements
/// @{

namespace tide
{
    /** \brief The Segment element.
     *
     * A segment makes up the body of a Tide document. It is the only top-level
     * element, excluding the EBMLHeader. All other Tide elements are contained
     * within it. Usually, a Tide document contains a single segment.
     *
     * The segment contains all the document's blocks (stored in clusters), and
     * all relevant meta-data, such as document information, tags, attachments
     * and cue points.
     *
     * Because the body of a segment contains data that arrives at differing
     * times, segments cannot be written in a single operation. Instead, a
     * segment is opened for writing by calling write(), which provides it with
     * its target output byte stream.  This prepares the byte stream for the
     * segment to be written. Once writing is complete, the segment is
     * finalised by calling finalise(). Finalising a segment completes it,
     * writing such things as the segment's final size into the output stream.
     *
     * Likewise, segments are not read in a single operation. Trying to do so
     * for any reasonably-sized file would require a significant quantity of
     * memory and be inefficient. Instead, the segment is opened for reading by
     * calling the read() method, which prepares the segment for reading by the
     * meta-seek element (if present) and filling in the index table. The child
     * elements are then read directly from the file as needed. The segment
     * does not need to be closed once reading is complete.
     */
    class TIDE_EXPORT Segment : public MasterElement
    {
        public:
            /** \brief Create a new segment element.
             *
             * \param[in] pad_size The size of the padding to place at the
             * start of the segment when opening it for writing.
             */
            Segment(std::streamsize pad_size=200);

            /** \brief Get the padding size.
             *
             * When the segment is opened for writing, it places a certain
             * quantity of padding at the start of the segment using a Void
             * element. This padding is over-written during finalisation with
             * the SeekHead and SegmentInfo elements. The length of the padding
             * is controlled by this property. Setting it to a value that is
             * too small for one or both of the SeekHead and SegmentInfo
             * elements will result in those that don't fit being written at
             * the end of the segment. In the case of the SeekHead, this can
             * significantly degrade start-up performance when reading the
             * segment.
             */
            std::streamsize pad_size() const { return pad_size_; }
            /// \brief Set the padding size.
            void pad_size(std::streamsize pad_size) { pad_size_ = pad_size; }

            /// \brief Get the total size of the element.
            std::streamsize size() const;

            /** \brief Finalise writing of the segment.
             *
             * Calculates the total size of the segment and writes it into the
             * segment header, followed by writing in the SeekHead element as
             * the first child of the Segment, thus finalising the segment.
             *
             * The write pointer in stream must be positioned at the first byte
             * after the last byte of the segment before this method is called.
             *
             * \param[in] stream The byte stream to write the segment to.
             * \return The final size, in bytes, of the segment (including the
             * element header).
             * \throw NotWriting if the segment has not yet been opened for
             * writing by calling write().
             */
            std::streamsize finalise(std::iostream& stream);

            /** \brief The segment index.
             *
             * All known level 1 elements are included in this index. It is a
             * mapping from element ID (the key) to position, in bytes, in the
             * segment (the value). The first level 1 element has a position of
             * 0. An ID may occur multiple times.
             *
             * There may be additional level 1 elements that are not mentioned
             * in this index. Typically, all but the first cluster are not
             * found in the index.
             */
            SeekHead index;

            /** \brief The segment information.
             *
             * This property stores all the segment's meta-data, such as the
             * origin timecode of the segment and its timecode scale.
             */
            SegmentInfo info;

            /** \brief Calculate an offset within the segment.
             *
             * This function turns an offset in the output stream into an
             * offset in this segment, which is necessary for the index.
             */
            std::streamsize to_segment_offset(std::streamsize stream_offset);

            /** \brief Calculate the offset in a stream of a position in the
             * segment.
             *
             * This function turns an offset within the segment into an
             * absolute position in a byte stream.
             */
            std::streamsize to_stream_offset(std::streamsize seg_offset);

        protected:
            /// The size of the padding to place at the start of the file.
            std::streamsize pad_size_;
            /// The size of the segment, as read from the file.
            std::streamsize size_;
            /// If the segment is currently being written.
            bool writing_;

            /** \brief Get the size of the body of this element.
             *
             * This function will not return the actual size of the segment
             * until either the segment is read from a Tide document or it is
             * finalised in a Tide document.
             */
            std::streamsize body_size() const
                { return size_; }

            /** \brief Element size writing.
             *
             * Writes the element's size to a byte stream providing a
             * std::ostream interface, using the full 8 bytes no matter what
             * the required size is.
             *
             * \return The number of bytes written.
             * \exception WriteError if an error occurs writing data.
             */
            std::streamsize write_size(std::ostream& output);

            /** \brief Element body writing.
             *
             * This function, which opens up a segment for writing, does not
             * actually write any final content. Instead, it preserves a chunk
             * of space at the beginning of the file using a Void element for
             * the SeekHead and SegmentInfo elements to be written into by
             * finalise().
             *
             * The size of space reserved is controlled by pad_size().
             */
            std::streamsize write_body(std::ostream& output);

            /** \brief Element body loading.
             *
             * This function does not actually read the body of the segment.
             * Instead, it reads some of the level 1 elements and validates
             * that the segment is usable:
             *
             * - Reads the meta-seek element (if present), filling in the
             *   segment's index.
             * - Reads the SegmentInfo element. If not present, raises an
             *   error.
             * - Checks for the existence of the Tracks element. Raises an
             *   error if not present.
             *   Checks for the existence of at least one Cluster element.
             *   Raises an error if not present.
             *
             * When this function returns, the stream's read pointer will be
             * placed at the end of the SegmentInformation element, unless the
             * SeekHead element occurs after it, in which case it will be
             * placed at the end of the SeekHead element.
             *
             * \return The number of bytes read from the stream, excluding
             * bytes read during element presence checks. In other words, this
             * is the number of bytes into the segment that the read pointer
             * has moved.
             * \throw MultipleSeekHeads if the segment has more than one index.
             * \throw NoSegmentInfo if the SegmentInfo element is not found.
             * \throw NoTracks if the Tracks element is not found.
             * \throw NoClusters if at least one Cluster element is not found.
             */
            std::streamsize read_body(std::istream& input,
                    std::streamsize size);
    }; // class Segment
}; // namespace tide

/// @}

#endif // TIDE_SEGMENT_H_

