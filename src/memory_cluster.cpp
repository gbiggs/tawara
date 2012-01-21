/* Tide
 *
 * Source for the in-memory cluster implementation.
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

#include <tide/memory_cluster.h>

#include <boost/foreach.hpp>
#include <numeric>
#include <tide/block_group.h>
#include <tide/exceptions.h>
#include <tide/simple_block.h>

using namespace tide;

///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

MemoryCluster::MemoryCluster(uint64_t timecode)
    : Cluster(timecode)
{
}


///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

MemoryCluster::Iterator MemoryCluster::begin()
{
    return Iterator(blocks_.begin());
}


MemoryCluster::ConstIterator MemoryCluster::begin() const
{
    return ConstIterator(blocks_.begin());
}


MemoryCluster::Iterator MemoryCluster::end()
{
    return Iterator(blocks_.end());
}


MemoryCluster::ConstIterator MemoryCluster::end() const
{
    return ConstIterator(blocks_.end());
}

///////////////////////////////////////////////////////////////////////////////
// I/O (Cluster interface)
///////////////////////////////////////////////////////////////////////////////

std::streamsize MemoryCluster::finalise(std::ostream& output)
{
    if (!writing_)
    {
        throw NotWriting();
    }

    std::streamsize written(0);

    // Write the blocks to the file
    BOOST_FOREACH(BlockElement::Ptr& block, blocks_)
    {
        written += block->write(output);
    }

    // Go back and write the cluster's actual size in the element header
    std::streampos cluster_end(output.tellp());
    std::streamsize size = cluster_end - offset_ - 8 - ids::size(id_);
    output.seekp(static_cast<std::streamsize>(offset_) +
            ids::size(ids::Cluster));
    write_size(output);
    // And return back to the end of the cluster again
    output.seekp(cluster_end);

    writing_ = false;
    return ids::size(id_) + 8 + meta_size() + written;
}


std::streamsize add_size(std::streamsize x, BlockElement::Ptr b)
{
    return x + b->size();
}

std::streamsize MemoryCluster::blocks_size() const
{
    return std::accumulate(blocks_.begin(), blocks_.end(), 0,
            std::ptr_fun(add_size));
}


std::streamsize MemoryCluster::read_blocks(std::istream& input,
        std::streamsize size)
{
    // Clear any existing blocks
    blocks_.clear();

    std::streamsize read_bytes(0);
    // Read elements until the body is exhausted
    while (read_bytes < size)
    {
        // Read the ID
        ids::ReadResult id_res = ids::read(input);
        ids::ID id(id_res.first);
        read_bytes += id_res.second;
        BlockElement::Ptr new_block;
        if (id == ids::SimpleBlock)
        {
            BlockElement::Ptr new_block(new SimpleBlock(0, 0));
            read_bytes += new_block->read(input);
            blocks_.push_back(new_block);
        }
        else if (id == ids::BlockGroup)
        {
            BlockElement::Ptr new_block(new BlockGroup(0, 0));
            read_bytes += new_block->read(input);
            blocks_.push_back(new_block);
        }
        else
        {
            throw InvalidChildID() << err_id(id) << err_par_id(id_) <<
                // The cast here makes Apple's LLVM compiler happy
                err_pos(static_cast<std::streamsize>(input.tellg()) -
                        id_res.second);
        }
    }
    if (read_bytes != size)
    {
        // Read more than was specified by the body size value
        throw BadBodySize() << err_id(id_) << err_el_size(size) <<
            err_pos(offset_);
    }

    return read_bytes;
}

