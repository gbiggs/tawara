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

            /// \brief Offsets a signed integer into unsigned territory.
            uint64_t s_to_u(int64_t value) const;

            /// \brief Offsets an unsigned integer into signed territory.
            int64_t u_to_s(uint64_t value) const;
    }; // class BlockImpl

    /// \brief Equality operator for BlockImpl objects.
    bool operator==(BlockImpl const& lhs, BlockImpl const& rhs);
}; // namespace tide

/// @}
// group implementations

#endif // TIDE_BLOCK_IMPL_H_

