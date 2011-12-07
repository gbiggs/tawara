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

    // Write the metaseek element.

    return size();
}


std::streamsize Segment::write_size(std::ostream& output)
{
    return vint::write(body_size(), output, 8);
}


std::streamsize Segment::write_body(std::ostream& output)
{
    return 0;
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
        // Read the SeekHead element
        read_bytes += index.read(input);
        if (index.find(ids::Info) != index.end())
        {
            have_segmentinfo = true;
        }
        if (index.find(ids::Tracks) != index.end())
        {
            have_tracks = true;
        }
        if (index.find(ids::Cluster) != index.end())
        {
            have_clusters = true;
        }
    }

    // Search for the other necessary elements
    while (read_bytes < size &&
            (!have_segmentinfo || !have_tracks || !have_clusters))
    {
        // Read the ID
        ids::ReadResult id_res = ids::read(input);
        ids::ID id(id_res.first);
        read_bytes += id_res.second;
        switch(id)
        {
            case ids::SeekHead:
                if (have_seekhead)
                {
                    throw MultipleSeekHeads() << err_pos(offset_);
                }
                // Read the SeekHead element
                read_bytes += index.read(input);
                if (index.find(ids::Info) != index.end())
                {
                    have_segmentinfo = true;
                }
                if (index.find(ids::Tracks) != index.end())
                {
                    have_tracks = true;
                }
                if (index.find(ids::Cluster) != index.end())
                {
                    have_clusters = true;
                }
                break;
            case ids::Info:
                have_segmentinfo = true;
                index.insert(std::make_pair(ids::Info,
                            input.tellg() - id_res.second));
                break;
            case ids::Tracks:
                have_tracks = true;
                index.insert(std::make_pair(ids::Tracks,
                            input.tellg() - id_res.second));
                break;
            case ids::Cluster:
                have_clusters = true;
                index.insert(std::make_pair(ids::Cluster,
                            input.tellg() - id_res.second));
                break;
            case ids::Cues:
            case ids::Attachments:
            case ids::Chapters:
            case ids::Tags:
                index.insert(std::make_pair(id,
                            input.tellg() - id_res.second));
                break;
            default:
                throw InvalidChildID() << err_id(id) << err_par_id(id_) <<
                    // The cast here makes Apple's LLVM compiler happy
                    err_pos(static_cast<std::streamsize>(input.tellg()) -
                            id_res.second);
        }
    }

    if (!have_segmentinfo)
    {
        throw NoSegmentInfo() << err_pos(offset_);
    }
    if (!have_tracks)
    {
        throw NoTracks() << err_pos(offset_);
    }
    if (!have_clusters)
    {
        throw NoClusters() << err_pos(offset_);
    }

    return read_bytes;
}

