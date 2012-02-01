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

#if !defined(TIDE_FILE_CLUSTER_H_)
#define TIDE_FILE_CLUSTER_H_

#include <tide/block_element.h>
#include <tide/cluster.h>
#include <tide/win_dll.h>

/// \addtogroup elements Elements
/// @{

namespace tide
{
    /** \brief The in-file Cluster implementation.
     *
     * This implementation of the cluster interface stores only a minimal amount
     * of information in memory. When reading, each block is left in the file
     * until needed. When writing, blocks are written to the file immediately as
     * they become available. It provides a lower memory footprint than the
     * MemoryCluster implementation, at the expense of slower block retrieval
     * and addition.
     */
    class TIDE_EXPORT FileCluster : public Cluster
    {
        public:
            /// \brief Pointer to a file-based cluster.
            typedef boost::shared_ptr<FileCluster> Ptr;

            /** \brief Construct a new cluster.
             *
             * \param[in] timecode The timecode in the cluster, in the units
             * specified by TimecodeScale.
             */
            FileCluster(uint64_t timecode=0);

            //////////////////////////////////////////////////////////////////
            // Iterator types
            //////////////////////////////////////////////////////////////////

            template <typename BlockType>
            class TIDE_EXPORT IteratorBase
                : public boost::iterator_facade<
                    IteratorBase<BlockType>, BlockType,
                    boost::forward_traversal_tag>
            {
                private:
                    struct enabler {};

                public:
                    /** \brief Base constructor.
                     *
                     * \param[in] cluster The cluster containing the blocks.
                     * \param[in] stream The stream to read blocks from.
                     */
                    IteratorBase(FileCluster const* cluster,
                            std::istream& stream)
                        : cluster_(cluster), stream_(stream)
                    {
                    }

                    /** \brief Templated base constructor.
                     *
                     * Used to provide interoperability with compatible
                     * iterators.
                     */
                    template <typename OtherType>
                    IteratorBase(IteratorBase<OtherType> const& other)
                        : cluster_(other.cluster_), stream_(other.stream_)
                    {
                    }

                protected:
                    // Necessary for Boost::iterator implementation.
                    friend class boost::iterator_core_access;

                    // Integrate with owning container.
                    friend class FileCluster;

                    FileCluster const* cluster_;
                    std::istream& stream_;
                    boost::shared_ptr<BlockType> block_;

                    /// \brief Increment the iterator to the next block.
                    void increment()
                    {
                        assert(false && "Not implemented");
                    }

                    /** \brief Test for equality with another iterator.
                     *
                     * \param[in] other The other iterator.
                     */
                    template <typename OtherType>
                    bool equal(IteratorBase<OtherType> const& other) const
                    {
                        assert(false && "Not implemented");
                        return false;
                    }

                    /** \brief Dereference the iterator to get a pointer to the
                     * block.
                     */
                    BlockType& dereference() const
                    {
                        return *block_;
                    }
            }; // class IteratorBase

            /** \brief File-based cluster iterator interface.
             *
             * This interface provides access to the blocks in the cluster.
             */
            typedef IteratorBase<BlockElement::Ptr> Iterator;
            /** \brief Block const iterator interface.
             *
             * This interface provides access to the blocks in the cluster.
             * The access is const, preventing modification of the blocks.
             */
            typedef IteratorBase<Block::ConstPtr> ConstIterator;

            //////////////////////////////////////////////////////////////////
            // Iterator access
            //////////////////////////////////////////////////////////////////

            //////////////////////////////////////////////////////////////////
            // Cluster interface
            //////////////////////////////////////////////////////////////////

            /// \brief Check if there are no blocks.
            virtual bool empty() const;
            /// \brief Get the number of blocks.
            virtual size_type count() const;
            /** \brief Remove all blocks.
             *
             * This will not erase the blocks from the file. It will instead
             * overwrite them with a void element, hiding them.
             */
            virtual void clear();

            /** \brief Erase the block at the specified iterator.
             *
             * This will not erase the block from the file. It will instead
             * overwrite it with a void element, hiding the data.
             *
             * \param[in] position The position to erase at.
             */
            virtual void erase(Iterator position);
            /** \brief Erase a range of blocks.
             *
             * This will not erase the blocks from the file. It will instead
             * overwrite them with a void element, hiding them.
             *
             * \param[in] first The start of the range.
             * \param[in] last The end of the range.
             */
            virtual void erase(Iterator first, Iterator last);

            /** \brief Add a block to this cluster.
             *
             * The cluster must be in the writable state. This means that
             * write() has been called and finalise() has not been called.
             */
            virtual void push_back(value_type const& value);

            /// \brief Element writing.
            std::streamsize write(std::ostream& output);

            /// \brief Finalise writing of the cluster.
            std::streamsize finalise(std::ostream& output);

        protected:
            std::ostream* ostream_;
            std::streampos blocks_start_pos_;
            std::streampos cur_write_pos_;

            /// \brief Get the size of the blocks in this cluster.
            std::streamsize blocks_size() const;

            /// \brief Read the blocks in this cluster from the output stream.
            std::streamsize read_blocks(std::istream& input,
                    std::streamsize size);
    }; // class FileCluster
}; // namespace tide

/// @}
// group elements

#endif // TIDE_FILE_CLUSTER_H_

