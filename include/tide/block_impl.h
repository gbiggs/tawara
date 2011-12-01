/* Tide
 *
 * Header file for the common Block functionality object.
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

#if !defined(TIDE_BLOCK_IMPL_H_)
#define TIDE_BLOCK_IMPL_H_

#include <stdint.h>
#include <tide/block.h>
#include <tide/win_dll.h>
#include <utility>

/// \addtogroup implementations Implementations
/// @{

namespace tide
{
    /** \brief Common block functionality implementation.
     *
     * This class provides an implementation of the Block interface
     * functionality.
     */
    class TIDE_EXPORT BlockImpl : public Block,
        public boost::equality_comparable<BlockImpl>
    {
        public:
            /// \brief Constructor.
            BlockImpl(uint64_t track_number, int16_t timecode,
                    LacingType lacing=LACING_NONE);

            /// \brief The block's track number.
            uint64_t track_number() const { return track_num_; }
            /// \brief Set the block's track number.
            void track_number(uint64_t track_number)
                { track_num_ = track_number; }

            /// \brief The timecode of this block.
            int16_t timecode() const { return timecode_; }
            /// \brief Set the block's timecode.
            void timecode(int16_t timecode) { timecode_ = timecode; }

            /// \brief If this block is invisible.
            bool invisible() const { return invisible_; }
            /// \brief Set if this block is invisible.
            void invisible(bool invisible) { invisible_ = invisible; }

            /// \brief Get the lacing type in use.
            LacingType lacing() const { return lacing_; }
            /// \brief Set the lacing type in use.
            void lacing(LacingType lacing) { lacing_ = lacing; }

            /// \brief Replace the content of this block with another block.
            BlockImpl& operator=(BlockImpl const& other);

            /** \brief Get the frame at the given position, with bounds
             * checking.
             */
            value_type& at(size_type pos)
                { return frames_.at(pos); }
            /** \brief Get the frame at the given position, with bounds
             * checking.
             */
            value_type const& at(size_type pos) const
                { return frames_.at(pos); }

            /** \brief Get a reference to a frame. No bounds checking is
             * performed.
             */
            value_type& operator[](size_type pos)
                { return frames_[pos]; }
            /** \brief Get a reference to a frame. No bounds checking is
             * performed.
             */
            value_type const& operator[](size_type pos) const
                { return frames_[pos]; }

            /// \brief Get an iterator to the first frame.
            iterator begin() { return frames_.begin(); }
            /// \brief Get an iterator to the first frame.
            const_iterator begin() const { return frames_.begin(); }
            /// \brief Get an iterator to the position past the last frame.
            iterator end() { return frames_.end(); }
            /// \brief Get an iterator to the position past the last frame.
            const_iterator end() const { return frames_.end(); }
            /// \brief Get a reverse iterator to the last frame.
            reverse_iterator rbegin() { return frames_.rbegin(); }
            /// \brief Get a reverse iterator to the last frame.
            const_reverse_iterator rbegin() const { return frames_.rbegin(); }
            /** \brief Get a reverse iterator to the position before the first
             * frame.
             */
            reverse_iterator rend() { return frames_.rend(); }
            /** \brief Get a reverse iterator to the position before the first
             * frame.
             */
            const_reverse_iterator rend() const { return frames_.rend(); }

            /// \brief Check if there are no frames.
            bool empty() const { return frames_.empty(); }
            /// \brief Get the number of frames.
            size_type count() const { return frames_.size(); }
            /// \brief Get the maximum number of frames.
            size_type max_count() const;

            /// \brief Remove all frames.
            void clear() { frames_.clear(); }

            /// \brief Erase the frame at the specified iterator.
            void erase(iterator position)
                { frames_.erase(position); }
            /// \brief Erase a range of frames.
            void erase(iterator first, iterator last)
                { frames_.erase(first, last); }

            /// \brief Add a frame to this block.
            void push_back(value_type const& value);

            /// \brief Resizes the frames storage.
            void resize(size_type count);

            /// \brief Swaps the contents of this block with another.
            void swap(BlockImpl& other);

            /** \brief Gets the size of the block data
             *
             * This includes the block header bytes, the frame data, and the
             * lacing header (if present).
             */
            std::streamsize size() const;

            /** \brief Write the block data to an output stream.
             *
             * This function performs the write of the block header and all
             * stored frames.
             *
             * Before performing the write, the block is validated. This may
             * cause exceptions to be raised if the block is in an invalid
             * state.
             *
             * \param[in] output The output byte stream to write data to.
             * \param[in] extra_flags Extra flags to add to the flags contained
             * in this block.
             * \return The number of bytes written.
             * \exception EmptyFrame if an empty frame is found.
             * \exception BadLacedFrameSize if fixed lacing is used and all
             * frames are not the same size.
             * \exception WriteError if an error occurs writing data.
             */
            std::streamsize write(std::ostream& output, uint8_t extra_flags);

            /** \brief The return result of a read.
             *
             * The first contains the number of bytes read.
             * The second contains any extra flags that were stored in the
             * block.
             */
            typedef std::pair<std::streamsize, uint8_t> ReadResult;
            /** \brief Read the block data from an input stream.
             *
             * This function reads a block header and all stored frames.
             *
             * \param[in] input The input byte stream to read from.
             * \param[in] size The number of bytes used by the block.
             * \return The number of bytes read and any extra flags that were
             * present in the block.
             * \exception ReadError if an error occurs reading data.
             * \exception BadBlockSize if the block size is too small or too
             * big.
             */
            ReadResult read(std::istream& input, std::streamsize size);

            /// \brief Equality operator.
            friend bool operator==(BlockImpl const& lhs, BlockImpl const& rhs);

        protected:
            uint64_t track_num_;
            int16_t timecode_;
            bool invisible_;
            LacingType lacing_;
            std::vector<boost::shared_ptr<std::vector<char> > > frames_;

            /// \brief Checks that the block is in a good condition to write.
            void validate() const;

            /// \brief Resets this block to an empty state.
            void reset();

            /** \brief Reads frames laced using EBML lacing, including the lace
             * header.
             *
             * \param[in] input The input byte stream to read from.
             * \param[in] size The number of bytes available.
             * \return The number of bytes read.
             * \exception ReadError if an error occurs reading data.
             * \exception BadElementSize if the block size is too small or too
             * big.
             */
            std::streamsize read_ebml_laced_frames(std::istream& input,
                    std::streamsize size);

            /** \brief Reads frames laced using fixed lacing.
             *
             * The number of frames to read is specified. The value of size
             * must be evenly dividable by this count with no remainder (i.e.
             * every frame must be the same size).
             *
             * \param[in] input The input byte stream to read from.
             * \param[in] size The number of bytes available.
             * \param[in] count The number of frames to read.
             * \return The number of bytes read.
             * \exception ReadError if an error occurs reading data.
             * \exception BadLacedFrameSize if the block size is too small or
             * too big.
             */
            std::streamsize read_fixed_frames(std::istream& input,
                    std::streamsize size, unsigned int count);
    }; // class BlockImpl

    /// \brief Equality operator for BlockImpl objects.
    bool operator==(BlockImpl const& lhs, BlockImpl const& rhs);
}; // namespace tide

/// @}
// group implementations

#endif // TIDE_BLOCK_IMPL_H_

