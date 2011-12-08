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
#include <tide/seek_element.h>
#include <tide/vint.h>
#include <tide/void_element.h>

using namespace tide;

///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

Segment::Segment()
    : MasterElement(ids::Segment), size_(4096), writing_(false)
{
}


///////////////////////////////////////////////////////////////////////////////
// I/O
///////////////////////////////////////////////////////////////////////////////

std::streamsize Segment::finalise(std::iostream& stream)
{
    if (!writing_)
    {
        throw NotWriting();
    }

    // Store the current end of the file
    std::streamoff cur_pos(stream.tellp());

    // Move to the beginning of the segment
    stream.seekp(offset_ + ids::size(ids::Segment), std::ios::beg);
    // Skip the (dummy) size value
    stream.seekp(8, std::ios::cur);
    // Get the size of the void element that is providing padding
    std::streamoff pad_start(stream.tellp());
    std::streamsize pad_size(0);
    ids::ReadResult id_res = ids::read(stream);
    if (id_res.first != ids::Void)
    {
        pad_size = 0;
    }
    else
    {
        VoidElement ve(0);
        ve.read(stream);
        pad_size = ve.size();
    }
    stream.seekp(pad_start);

    // TODO: Add the SegmentInfo to the SeekHead (not having it is not a
    // disaster if it's placed immediately afterwards).
    std::streamsize written(0);
    bool wrote_seekhead(false), wrote_seginfo(false);
    while(written < pad_size && (!wrote_seekhead || !wrote_seginfo))
    {
        if (!wrote_seekhead && index.size() < pad_size - written)
        {
            written += index.write(stream);
        }
        else if (!wrote_seginfo && info.size() < pad_size - written)
        {
            written += info.write(stream);
        }
    }
    // Re-do the padding
    VoidElement ve(pad_size - written, true);
    ve.write(stream);
    // Move to the end of the file
    stream.seekp(cur_pos);
    if (!wrote_seekhead)
    {
        // Write the index at the end
        index.write(stream);
    }
    if (!wrote_seginfo)
    {
        // Write the segment info at the end
        info.write(stream);
    }

    // Calculate the size of the segment
    size_ = stream.tellp() - offset_;
    // Write the size way back at the beginning of the segment
    stream.seekp(offset_ + ids::size(ids::Segment), std::ios::beg);
    write_size(stream);

    return size();
}


std::streamsize Segment::write_size(std::ostream& output)
{
    return vint::write(body_size(), output, 8);
}


std::streamsize Segment::write_body(std::ostream& output)
{
    writing_ = true;
    // Write some padding
    VoidElement ve(4096, true);
    return ve.write(output);
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

    // Segments being read cannot be written.
    writing_ = false;

    // Store the segment's size
    size_ = size;

    bool have_seekhead(false);
    bool have_segmentinfo(false);
    bool have_tracks(false);
    bool have_clusters(false);
    std::streamsize read_bytes(0);
    std::streamoff last_read_end(0); // Tracks where the read pointer should be
                                     // placed when this method returns.

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
    else
    {
        // Rewind back to the ID for the search
        input.seekg(-id_res.second, std::ios::cur);
    }
    last_read_end = input.tellg();

    // Search for the other necessary elements
    while (read_bytes < size &&
        (!have_seekhead || !have_segmentinfo || !have_tracks || !have_clusters))
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
                last_read_end = input.tellg();
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
                if (!have_clusters)
                {
                    // Only store the first cluster in the index
                    have_clusters = true;
                    index.insert(std::make_pair(ids::Cluster,
                                input.tellg() - id_res.second));
                }
                break;
            case ids::Cues:
            case ids::Attachments:
            case ids::Chapters:
            case ids::Tags:
                index.insert(std::make_pair(id,
                            input.tellg() - id_res.second));
                skip_read(input, false);
                break;
            case ids::Void:
                skip_read(input, false);
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
    else
    {
        input.seekg(index.find(ids::Info)->second);
        info.read(input);
        if (input.tellg() > last_read_end)
        {
            last_read_end = input.tellg();
        }
    }
    if (!have_tracks)
    {
        throw NoTracks() << err_pos(offset_);
    }
    if (!have_clusters)
    {
        throw NoClusters() << err_pos(offset_);
    }

    input.seekg(last_read_end);
    return read_bytes;
}

