/* Tide
 *
 * Source for the Segment element.
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

#include <tide/segment.h>

#include <tide/el_ids.h>
#include <tide/exceptions.h>
#include <tide/vint.h>

using namespace tide;

///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

Segment::Segment()
    : MasterElement(ids::Segment), size_(0)
{
}


///////////////////////////////////////////////////////////////////////////////
// I/O
///////////////////////////////////////////////////////////////////////////////

std::streamsize Segment::finalise(std::ostream& output)
{
    // Calculate the size of the segment
    size_ = output.tellp() - offset_;
    // Write the size way back at the beginning of the segment
    std::streamoff cur_pos(output.tellp());
    output.seekp(offset_ + ids::size(ids::Segment), std::ios::beg);
    write_size(output);
    output.seekp(cur_pos);
    return size();
}


std::streamsize Segment::write_size(std::ostream& output)
{
    return vint::write(body_size(), output, 8);
}


std::streamsize Segment::read_body(std::istream& input, std::streamsize size)
{
    // +2 for the size values (which must be at least 1 byte each)
    if (size < ids::size(ids::Tracks) + ids::size(ids::Cluster) + 2)
    {
        // 0 is a bad size for the body because there must be at least enough
        // room for the tracks and at least one cluster
        throw BadBodySize() << err_id(id_) << err_pos(offset_);
    }

    // Store the segment's size
    size_ = size;

    bool have_seekhead(false);
    bool have_segmentinfo(false);
    bool have_tracks(false);
    bool have_clusters(false);
    std::streamsize read_bytes(0);

    // Check if the first child is the meta-seek
    ids::ReadResult id_res = ids::read(input);
    read_bytes += id_res.second;
    if (id_res.first == ids::SeekHead)
    {
        have_seekhead = true;
        // Read the SeekHead element size
        vint::ReadResult size_res = vint::read(input);
        read_bytes += size_res.second;
        read_bytes += read_seekhead(input, size_res.first);
        if (index.find(ids::SegmentInfo))
        {
            have_segmentinfo = true;
        }
        if (index.find(ids::Tracks))
        {
            have_tracks = true;
        }
        if (index.find(ids::Cluster))
        {
            have_clusters = true;
        }
    }

    if (!have_segmentinfo)
    {
    }

    default:
        throw InvalidChildID() << err_id(id) << err_par_id(id_) <<
            // The cast here makes Apple's LLVM compiler happy
            err_pos(static_cast<std::streamsize>(input.tellg()) -
                    id_res.second);
}

