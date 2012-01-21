/* Tide
 *
 * Source for the Cluster element's base class.
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

#include <tide/cluster.h>

#include <boost/foreach.hpp>
#include <numeric>
#include <tide/el_ids.h>
#include <tide/exceptions.h>
#include <tide/vint.h>

using namespace tide;

///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

Cluster::Cluster(uint64_t timecode)
    : MasterElement(ids::Cluster),
    timecode_(ids::Timecode, timecode), position_(ids::Position, 0),
    prev_size_(ids::PrevSize, 0), writing_(false)
{
}


///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

uint64_t Cluster::position() const
{
    throw NotImplemented();
}


///////////////////////////////////////////////////////////////////////////////
// Element interface
///////////////////////////////////////////////////////////////////////////////

std::streamsize Cluster::size() const
{
    // The size of a cluster is always written using 8 bytes
    return tide::ids::size(id_) + 8 + body_size();
}


std::streamsize add_size(std::streamsize x, SilentTrackNumber stn)
{
    return x + stn.size();
}

std::streamsize Cluster::meta_size() const
{
    std::streamsize result(timecode_.size());

    if (!silent_tracks_.empty())
    {
        result += ids::size(ids::SilentTracks);
        std::streamsize st_size(std::accumulate(silent_tracks_.begin(),
                    silent_tracks_.end(), 0, std::ptr_fun(add_size)));
        result += vint::size(st_size) + st_size;
    }
    if (position_ != 0)
    {
        result += position_.size();
    }
    if (prev_size_ != 0)
    {
        result += prev_size_.size();
    }

    return result;
}


std::streamsize Cluster::write_size(std::ostream& output)
{
    return vint::write(body_size(), output, 8);
}


std::streamsize Cluster::write_body(std::ostream& output)
{
    std::streamsize written(0);
    writing_ = true;

    written += timecode_.write(output);
    if (!silent_tracks_.empty())
    {
        written += ids::write(ids::SilentTracks, output);
        std::streamsize st_size(std::accumulate(silent_tracks_.begin(),
                    silent_tracks_.end(), 0, std::ptr_fun(add_size)));
        written += tide::vint::write(st_size, output);
        BOOST_FOREACH(SilentTrackNumber& stn, silent_tracks_)
        {
            written += stn.write(output);
        }
    }
    if (position_ != 0)
    {
        written += position_.write(output);
    }
    if (prev_size_ != 0)
    {
        written += prev_size_.write(output);
    }

    return written;
}


std::streamsize Cluster::read_body(std::istream& input,
        std::streamsize size)
{
    // Reset to defaults
    reset();
    // Cannot write a cluster being read
    writing_ = false;

    std::streamsize read_bytes(0);
    // Read elements until the body is exhausted
    bool have_timecode(false);
    while (read_bytes < size)
    {
        // Read the ID
        ids::ReadResult id_res = ids::read(input);
        ids::ID id(id_res.first);
        read_bytes += id_res.second;
        switch (id)
        {
            case ids::Timecode:
                read_bytes += timecode_.read(input);
                have_timecode = true;
                break;
            case ids::SilentTracks:
                read_bytes += read_silent_tracks(input);
                break;
            case ids::Position:
                read_bytes += position_.read(input);
                break;
            case ids::PrevSize:
                read_bytes += prev_size_.read(input);
                break;
            case ids::SimpleBlock:
            case ids::BlockGroup:
                // Rewind to the element ID value
                input.seekg(-id_res.second, std::ios::cur);
                read_bytes -= id_res.second;
                // Read all the blocks - this will use up the rest of the block
                read_bytes += read_blocks(input, size - read_bytes);
                break;
            default:
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
    if (!have_timecode)
    {
        throw MissingChild() << err_id(ids::Timecode) << err_par_id(id_) <<
            err_pos(offset_);
    }

    return read_bytes;
}


///////////////////////////////////////////////////////////////////////////////
// Private functions
///////////////////////////////////////////////////////////////////////////////

std::streamsize Cluster::read_silent_tracks(std::istream& input)
{
    std::streampos el_start(input.tellg());
    // Get the element's body size
    vint::ReadResult result = tide::vint::read(input);
    std::streamsize body_size(result.first);
    std::streamsize size_size(result.second);
    std::streamsize read_bytes(result.second);
    // Read elements until the body is exhausted
    while (read_bytes < size_size + body_size)
    {
        // Read the ID
        ids::ReadResult id_res = ids::read(input);
        ids::ID id(id_res.first);
        read_bytes += id_res.second;
        if (id != ids::SilentTrackNumber)
        {
            throw InvalidChildID() << err_id(id) << err_par_id(id_) <<
                err_pos(input.tellg());
        }
        SilentTrackNumber stn(0);
        read_bytes += stn.read(input);
        silent_tracks_.push_back(stn);
    }
    if (read_bytes != size_size + body_size)
    {
        // Read more than was specified by the body size value
        throw BadBodySize() << err_id(id_) << err_el_size(body_size) <<
            err_pos(el_start);
    }

    return read_bytes;
}


void Cluster::reset()
{
    timecode_ = 0;
    silent_tracks_.clear();
    position_ = 0;
    prev_size_ = 0;
}

