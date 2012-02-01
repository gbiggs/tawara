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

#include <tide/file_cluster.h>

#include <tide/block_group.h>
#include <tide/exceptions.h>
#include <tide/simple_block.h>

using namespace tide;

///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

FileCluster::FileCluster(uint64_t timecode)
    : Cluster(timecode), ostream_(0), blocks_start_pos_(0), cur_write_pos_(0)
{
}

///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// I/O (Cluster interface)
///////////////////////////////////////////////////////////////////////////////

bool FileCluster::empty() const
{
    return cur_write_pos_ == blocks_start_pos_;
}


FileCluster::size_type FileCluster::count() const
{
    assert(false && "Not implemented");
}


void FileCluster::clear()
{
    assert(false && "Not implemented");
}


void FileCluster::erase(FileCluster::Iterator position)
{
    assert(false && "Not implemented");
}


void FileCluster::erase(FileCluster::Iterator first, FileCluster::Iterator last)
{
    assert(false && "Not implemented");
}


void FileCluster::push_back(FileCluster::value_type const& value)
{
    // TODO: Make this a compile-time error somehow (type traits?)
    if (!writing_)
    {
        throw NotWriting();
    }
    assert(ostream_ != 0 && "ostream_ was not initialised");

    // Preserve the current write position
    //std::streampos cur_pos(ostream_->tellp());
    // Jump to the cluster's current write position
    ostream_->seekp(cur_write_pos_);
    // Write the block
    value->write(*ostream_);
    // Update the cluster's current write position
    cur_write_pos_ = ostream_->tellp();
    // Return to the original write position
    //ostream_->seekp(cur_pos);
    // TODO: update the block size continuously so that it can be written
    // during finalise() without needing to be calculated from the file write
    // pointer position at that time.
    // TODO: benchmark the impact constantly seeking back to the previous
    // position has on performance to determine if it is worth doing or not.
}


std::streamsize FileCluster::finalise(std::ostream& output)
{
    // TODO: Make this a compile-time error somehow (type traits?)
    if (!writing_)
    {
        throw NotWriting();
    }

    // Preserve the current write position
    std::streampos cur_pos(output.tellp());

    // Go back and write the cluster's actual size in the element header
    // actual size = current write position (i.e. end of the
    // cluster) - cluster's start position - ID - 8-byte size.
    std::streamsize size(cur_write_pos_ - offset_ - ids::size(id_) - 8);
    output.seekp(static_cast<std::streamsize>(offset_) +
            ids::size(ids::Cluster));
    write_size(output);

    // Return to the original write position
    output.seekp(cur_pos);

    writing_ = false;
    return ids::size(id_) + 8 + size;
}


std::streamsize FileCluster::blocks_size() const
{
    // Return the size of the blocks written so far.
    return cur_write_pos_ - blocks_start_pos_;
}


std::streamsize FileCluster::write(std::ostream& output)
{
    // TODO: Make this a compile-time error somehow (type traits?)
    assert(!writing_ && "Already writing");
    // Store a pointer to the stream for push_back() to use.
    ostream_ = &output;
    std::streamsize result = Element::write(output);
    // Make a note of where to write the first block.
    blocks_start_pos_ = cur_write_pos_ = output.tellp();
    return result;
}


std::streamsize FileCluster::read_blocks(std::istream& input,
        std::streamsize size)
{
}

