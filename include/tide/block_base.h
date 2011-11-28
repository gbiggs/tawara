/* Tide
 *
 * Header file for the base element object.
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

#if !defined(TIDE_BLOCK_BASE_H_)
#define TIDE_BLOCK_BASE_H_

#include <stdint.h>
#include <tide/win_dll.h>
#include <vector>

/// \addtogroup elements Elements
/// @{

namespace tide
{
    /** \brief Base class for blocks.
     *
     * There are two types of blocks
     */
    class TIDE_EXPORT BlockBase
    {
        protected:
            /// \brief The type of the frame storage.
            typedef std::vector<std::vector<char> > storage_type_;

        public:
            /// \brief Lacing types.
            enum LacingType
            {
                /// No lacing
                LACING_NONE,
                /// EBML-style lacing
                LACING_EBML,
                /// Fixed-size lacing
                LACING_FIXED
            };
            /// \brief The value type of this container.
            typedef storage_type_::value_type value_type;
            /// \brief The size type of this container.
            typedef storage_type_::size_type size_type;
            /// \brief The reference type.
            typedef storage_type_::reference reference;
            /// \brief The constant reference type.
            typedef storage_type_::const_reference const_reference;
            /// \brief The random access iterator type.
            typedef storage_type_::iterator iterator;
            /// \brief The constant random access iterator type.
            typedef storage_type_::const_iterator const_iterator;
            /// \brief The reversed random access iterator type.
            typedef storage_type_::reverse_iterator reverse_iterator;
            /// \brief The constant reversed random access iterator type.
            typedef storage_type_::const_reverse_iterator
                const_reverse_iterator;

            /** \brief Base constructor.
             *
             * \param[in] track_number The track number this block belongs to.
             * \param[in] timecode The timecode of this block.
             * \param[in] lacing The type of lacing to use.
             */
            BlockBase(uint64_t track_number, int16_t timecode,
                    LacingType lacing=LACING_NONE);

            /// \brief Base desctructor.
            virtual ~BlockBase() = 0;

            /** \brief The block's track number.
             *
             * This property specifies the track that this block belongs to.
             * The data stored in the block should be interpreted by the codec
             * for its track.
             */
            uint64_t track_number() const { return track_num_; }
            /// \brief Set the block's track number.
            void track_number(uint64_t track_number)
                { track_num_ = track_number; }

            /** \brief The timecode of this block.
             *
             * Each block has a timecode relative to its containing cluster. It
             * is measured in the units specified by the containing segment's
             * TimecodeScale, and is a 16-bit signed integer.
             */
            int16_t timecode() const { return timecode_; }
            /// \brief Set the block's timecode.
            void timecode(int16_t timecode) { timecode_ = timecode; }

            /** \brief If this block is invisible.
             *
             * Invisible blocks should be decoded by the codec (thus updating
             * codec state) but not used for playback.
             */
            bool invisible() const { return invisible_; }
            /// \brief Set if this block is invisible.
            void invisible(bool invisible) { invisible_ = invisible; }

            /** \brief Get the lacing type in use.
             *
             * The data in a block is typically a single frame, but sometimes
             * multiple frames may be stored. This is called "lacing," and is
             * usually used to reduce overhead when the size of the data itself
             * is small. However, lacing also reduces seekability of the file,
             * so laces should usually be kept small. A common number is up to
             * three frames in a lace.
             */
            LacingType lacing() const { return lacing_; }
            /// \brief Set the lacing type in use.
            void lacing(LacingType lacing) { lacing_ = lacing; }

            /// \brief Replace the content of this block with another block.
            BlockBase& operator=(BlockBase const& other);

            /** \brief Get the frame at the given position, with bounds
             * checking.
             *
             * \return A reference to the specified frame's data.
             * \throw std::out_of_range if the position is invalid.
             */
            value_type& at(size_type pos)
                { return frames_.at(pos); }
            /** \brief Get the frame at the given position, with bounds
             * checking.
             *
             * \return A reference to the specified frame's data.
             * \throw std::out_of_range if the position is invalid.
             */
            value_type const& at(size_type pos) const
                { return frames_.at(pos); }

            /** \brief Get a reference to a frame. No bounds checking is
             * performed.
             *
             * \return A reference to the binary data of a frame stored in this
             * block.
             */
            value_type& operator[](size_type pos)
                { return frames_[pos]; }
            /** \brief Get a reference to a frame. No bounds checking is
             * performed.
             *
             * \return A reference to the binary data of a frame stored in this
             * block.
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

            /** \brief Check if there are no frames.
             *
             * Empty blocks cannot be written. If this returns true, an error
             * will occur when write() is called.
             */
            bool empty() const { return frames_.empty(); }
            /// \brief Get the number of frames.
            size_type count() const { return frames_.size(); }
            /** \brief Get the maximum number of frames.
             *
             * If lacing is not enabled, this will always return 1.
             *
             * If lacing is enabled, this will be the maximum number of frames
             * that can be stored in a lace within a single block.
             */
            size_type max_count() const;

            /// \brief Remove all frames.
            void clear() { frames_.clear(); }

            /** \brief Erase the frame at the specified iterator.
             *
             * \param[in] position The position to erase at.
             */
            void erase(iterator position)
                { frames_.erase(position); }
            /** \brief Erase a range of frames.
             *
             * \param[in] first The start of the range.
             * \param[in] last The end of the range.
             */
            void erase(iterator first, iterator last)
                { frames_.erase(first, last); }

            /** \brief Add a frame to this block.
             *
             * When lacing is enabled, this will append an additional frame to
             * the block to be stored.
             *
             * When lacing is not enabled, the value of frame_count() must be
             * zero or an error will occur.
             *
             * \throw MaxLaceSizeExceeded if the new size is incompatible with
             * the lacing type.
             * \throw EmptyFrame if the frame data is empty.
             */
            void push_back(value_type const& value);

            /** \brief Resizes the frames storage.
             *
             * When lacing is not enabled, the new size must be 1 or an error
             * will occur.
             *
             * If the current size is less than the new size, additional empty
             * frames will be added. These should be filled with data before
             * calling write() or an error will occur.
             *
             * If the current size is greater than the new size, frames past
             * the new end will be dropped.
             *
             * \throw MaxLaceSizeExceeded if the new size is incompatible with
             * the lacing type.
             */
            void resize(size_type count);

            /** \brief Swaps the contents of this block with another.
             *
             * \param[in] other The other block to swap with.
             */
            void swap(BlockBase& other);

            /// \brief Equality operator.
            friend bool operator==(BlockBase const& lhs, BlockBase const& rhs);

        protected:
            uint64_t track_num_;
            int16_t timecode_;
            bool invisible_;
            LacingType lacing_;
            std::vector<std::vector<char> > frames_;

            /** \brief Checks that the block is in a good condition to write.
             *
             * This function \e must be called before performing any operations
             * to write the block.
             */
            void validate() const;
    }; // class BlockBase

    /// \brief Equality operator for blocks.
    bool operator==(BlockBase const& lhs, BlockBase const& rhs);
}; // namespace tide

/// @}
// group elements

#endif // TIDE_BLOCK_BASE_H_

