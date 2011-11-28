/* Tide
 *
 * Header for the Cluster element's base class.
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

#if !defined(TIDE_CLUSTER_H_)
#define TIDE_CLUSTER_H_

#include <tide/master_element.h>
#include <tide/uint_element.h>
#include <tide/win_dll.h>

/// \addtogroup elements Elements
/// @{

namespace tide
{
    /** \brief The track number of a silent track.
     *
     * This element stores the track number of a track that has been made
     * silent within this cluster.
     */
    class SilentTrackNumber : public UIntElement
    {
        public:
            SilentTrackNumber(uint64_t track_number)
                : UIntElement(ids::SilentTrackNumber, track_number)
            {
            }
    };

    /** \brief The base Cluster, defining the common interface for Cluster
     * element implementations.
     *
     * This class is the base class for all Cluster implementations. Different
     * concrete implementations of this interface implement reading and writing
     * of blocks in different ways. The two most commonly-used implementations
     * are the in-memory cluster and the in-file cluster.
     */
    class TIDE_EXPORT Cluster : public MasterElement
    {
        public:
            /** \brief Construct a new Cluster
             *
             * \param[in] timecode The timecode of the cluster, in the units
             * specified by TimecodeScale.
             */
            Cluster(uint64_t timecode=0);

            /// \brief Destructor.
            virtual ~Cluster() {};

            /** \brief Get the cluster's timecode.
             *
             * This timecode defines the base timecode for all blocks in the
             * cluster. It is specified in units of the TimecodeScale found in
             * the SegmentInfo element for the same segment as the cluster.
             */
            uint64_t timecode() const { return timecode_; }
            /// \brief Set the cluster's timecode.
            void timecode(uint64_t timecode) { timecode_ = timecode; }

            /** \brief Get the list of silent tracks.
             *
             * Some tracks in a cluster may be marked as silent. This means
             * that all blocks for those tracks should be ignored within this
             * cluster. This property lists the \e track \e numbers of the
             * silent tracks.
             *
             * A track being made silent in this cluster has no effect on its
             * silence in other clusters.
             */
            std::vector<SilentTrackNumber>& silent_tracks()
                { return silent_tracks_; }

            /** \brief Get the position of this cluster in the segment.
             *
             * This property gives the byte-offset of this cluster in its segment.
             * This value is useful for re-synchronising damaged streams.
             *
             * If it is zero, then the cluster has not been written or was not
             * read from a byte stream.
             */
            uint64_t position() const;

            /** \brief Get the size of the previous cluster in the segment.
             *
             * This property gives the size of the previous cluster in bytes.
             * This can be used to jump straight to the start of the previous
             * cluster, rather than searching for it.
             *
             * It it is zero, the size of the previous cluster is unknown.
             */
            uint64_t previous_size() const { return prev_size_; }
            /// \brief Set the size of the previous cluster in the segment.
            void previous_size(uint64_t size) { prev_size_ = size; }

            /// \brief Get the size of the body of this element.
            virtual std::streamsize size() const;

            /// \brief Element body writing.
            virtual std::streamsize write_body(std::ostream& output);

            /// \brief Element body loading.
            virtual std::streamsize read_body(std::istream& input);

        protected:
            UIntElement timecode_;
            std::vector<SilentTrackNumber> silent_tracks_;
            UIntElement position_;
            UIntElement prev_size_;

            /// \brief Get the size of the blocks in this cluster.
            virtual std::streamsize blocks_size() const = 0;

            /** \brief Write the blocks in this cluster to the output stream.
             *
             * This function may be implemented to do nothing if the blocks are
             * written in some other way, but once it is called the cluster
             * should be considered final in the stream, including all the
             * cluster's meta-data.
             *
             * For example, if the blocks are actually written by an iterator
             * over a period of time, this function would be implemented to
             * write the complete size of the cluster in its header.
             *
             * \return The total size of the cluster's blocks. Even if only a
             * small quantity is actually written, this must return the
             * complete blocks size of the cluster in order to meet the Element
             * interface requirements.
             */
            virtual std::streamsize write_blocks(std::ostream& output) = 0;

            /** \brief Read the blocks in this cluster from the output stream.
             *
             * This function may not necessarily perform the actual reading,
             * but once called, the blocks should be accessible through
             * whatever interface the Cluster implementation in use provides.
             *
             * For example, if the blocks are actually read by an iterator,
             * calling this function should prepare for the iterators' use. It
             * might, for example, read the position of each block and store it
             * in an index.
             *
             * \return The total size of the cluster's blocks (as stored in the
             * stream). Even if only a small quantity of data is actually read,
             * it must return the complete blocks size of the cluster in order
             * to meet the Element interface requirements.
             */
            virtual std::streamsize read_blocks(std::istream& input) = 0;

            /** \brief Read the SilentTracks child element.
             *
             * \return The number of bytes read.
             */
            std::streamsize read_silent_tracks(std::istream& input);

            /// \brief Reset the cluster's members to default values.
            virtual void reset();
    }; // class Cluster
}; // namespace tide

/// @}
// group elements

#endif // TIDE_CLUSTER_H_

