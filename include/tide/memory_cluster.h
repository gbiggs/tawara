/* Tide
 *
 * Header for the in-memory Cluster.
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

#if !defined(TIDE_MEMORY_CLUSTER_H_)
#define TIDE_MEMORY_CLUSTER_H_

#include <boost/iterator/iterator_facade.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>
#include <tide/block_element.h>
#include <tide/cluster.h>
#include <tide/win_dll.h>

/// \addtogroup elements Elements
/// @{

namespace tide
{
    /** \brief The in-memory Cluster implementation.
     *
     * This implementation of the Cluster interface stores the entire cluster
     * in memory, using a std::vector to store the Blocks. It provides rapid
     * access to the blocks at the expense of a larger memory footprint and
     * longer loading time when opening the cluster.
     */
    class TIDE_EXPORT MemoryCluster : public Cluster
    {
        protected:
            /// Block storage type
            typedef std::vector<BlockElement::Ptr> BlockStore;

        public:
            /** \brief Construct a new cluster.
             *
             * \param[in] timecode The timecode of the cluster, in the units
             * specified by TimecodeScale.
             */
            MemoryCluster(uint64_t timecode=0);

            //////////////////////////////////////////////////////////////////
            // Iterator types
            //////////////////////////////////////////////////////////////////

            template <typename BlockType, typename IterType>
            class TIDE_EXPORT IteratorBase
                : public boost::iterator_facade<
                    IteratorBase<BlockType, IterType>,
                    BlockType, boost::bidirectional_traversal_tag>
            {
                private:
                    struct enabler {};

                public:
                    /// \brief Base constructor.
                    IteratorBase()
                    {
                    }

                    /** \brief Constructor.
                     *
                     * \param[in] iter The storage iterator to wrap.
                     */
                    IteratorBase(IterType iter)
                        : iter_(iter)
                    {
                    }

                    /** \brief Templated base constructor.
                     *
                     * Used to provide interoperability with compatible
                     * iterators.
                     */
                    template <typename OtherType, typename OtherIterType>
                    IteratorBase(IteratorBase<OtherType, OtherIterType> const& other)
                        : iter_(other.iter_)
                    {
                    }

                protected:
                    // Necessary for Boost::iterator implementation.
                    friend class boost::iterator_core_access;

                    // Integrate with owning container
                    friend class MemoryCluster;

                    IterType iter_;

                    /// \brief Increment the Iterator to the next block.
                    void increment()
                    {
                        ++iter_;
                    }

                    /// \brief Decrement the Iterator to the previous block.
                    void decrement()
                    {
                        --iter_;
                    }

                    /** \brief Test for equality with another Iterator.
                     *
                     * \param[in] other The other iterator.
                     */
                    template <typename OtherType, typename OtherIterType>
                    bool equal(
                            IteratorBase<OtherType, OtherIterType> const& other) const
                    {
                        return iter_ == other.iter_;
                    }

                    /** \brief Dereference the iterator to get the Block
                     * pointer.
                     */
                    BlockType& dereference() const
                    {
                        return *iter_;
                    }
            }; // class IteratorBase

            /** \brief Block iterator interface
             *
             * This interface provides access to the blocks in the cluster,
             * sorted in ascending time order.
             */
            typedef IteratorBase<BlockElement::Ptr, BlockStore::iterator> Iterator;
            /** \brief Block const iterator interface
             *
             * This interface provides access to the blocks in the cluster,
             * sorted in ascending time order. The access is const, preventing
             * modification of the blocks.
             */
            typedef IteratorBase<Block::ConstPtr, BlockStore::const_iterator>
                ConstIterator;

            //////////////////////////////////////////////////////////////////
            // Iterator access
            //////////////////////////////////////////////////////////////////

            /** \brief Access the start of the blocks.
             *
             * Gets an iterator pointing to the first block in the cluster.
             */
            Iterator begin();
            /** \brief Access the start of the blocks.
             *
             * Gets an iterator pointing to the first block in the cluster.
             */
            ConstIterator begin() const;
            /** \brief Access the end of the blocks.
             *
             * Gets an iterator pointing beyond the last block in the cluster.
             */
            Iterator end();
            /** \brief Access the end of the blocks.
             *
             * Gets an iterator pointing beyond the last block in the cluster.
             */
            ConstIterator end() const;

            //////////////////////////////////////////////////////////////////
            // Cluster interface
            //////////////////////////////////////////////////////////////////

            /// \brief Check if there are no blocks.
            virtual bool empty() const { return blocks_.empty(); }
            /// \brief Get the number of blocks.
            virtual size_type count() const { return blocks_.size(); }
            /// \brief Remove all blocks.
            virtual void clear() { blocks_.clear(); }

            /** \brief Erase the block at the specified iterator.
             *
             * \param[in] position The position to erase at.
             */
            virtual void erase(Iterator position)
                { blocks_.erase(position.iter_); }
            /** \brief Erase a range of blocks.
             *
             * \param[in] first The start of the range.
             * \param[in] last The end of the range.
             */
            virtual void erase(Iterator first, Iterator last)
                { blocks_.erase(first.iter_, last.iter_); }

            /** \brief Add a block to this cluster.
             *
             * The cluster must be in the writable state. This means that
             * write() has been called and finish_write() has not been called.
             */
            virtual void push_back(value_type const& value)
                { blocks_.push_back(value); }

            /// \brief Finalise writing of the cluster.
            std::streamsize finalise(std::ostream& output);

        protected:
            /// Block storage
            BlockStore blocks_;

            /// \brief Get the size of the blocks in this cluster.
            std::streamsize blocks_size() const;

            /// \brief Write the blocks in this cluster to the output stream.
            std::streamsize write_blocks(std::ostream& output);

            /// \brief Read the blocks in this cluster from the output stream.
            std::streamsize read_blocks(std::istream& input,
                    std::streamsize size);
    }; // class MemoryCluster
}; // namespace tide

/// @}
// group elements

#endif // TIDE_MEMORY_CLUSTER_H_
