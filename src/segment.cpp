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

Segment::Segment(std::streamsize pad_size)
    : MasterElement(ids::Segment), pad_size_(pad_size), size_(pad_size),
    writing_(false)
{
}


///////////////////////////////////////////////////////////////////////////////
// Miscellaneous member functions
///////////////////////////////////////////////////////////////////////////////

std::streamsize Segment::to_segment_offset(std::streamsize stream_offset)
{
    return stream_offset - offset_ - ids::size(ids::Segment) - 8;
}


std::streamsize Segment::to_stream_offset(std::streamsize seg_offset)
{
    return seg_offset + offset_ + ids::size(ids::Segment) + 8;
}


///////////////////////////////////////////////////////////////////////////////
// I/O
///////////////////////////////////////////////////////////////////////////////

std::streamsize Segment::size() const
{
    // The size of a segment is always written using 8 bytes
    return tide::ids::size(id_) + 8 + body_size();
}


std::streamsize Segment::finalise(std::iostream& stream)
{
    if (!writing_)
    {
        throw NotWriting();
    }

    // Store the current end of the file
    std::streamoff end_pos(stream.tellp());
    // Store the current read point
    std::streamoff cur_read(stream.tellg());

    // Move to the beginning of the segment, skipping the dummy size value
    stream.seekg(static_cast<std::streamsize>(offset_) +
            ids::size(ids::Segment) + 8, std::ios::beg);
    // Get the size of the void element that is providing padding
    std::streamoff pad_start(stream.tellg());
    std::streamsize pad_size(0);
    ids::ReadResult id_res = ids::read(stream);
    if (id_res.first != ids::Void)
    {
        pad_size = 0;
    }
    else
    {
        VoidElement ve(2);
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
            wrote_seekhead = true;
        }
        else if (!wrote_seginfo && info.size() < pad_size - written)
        {
            written += info.write(stream);
            wrote_seginfo = true;
        }
    }
    // Re-do the padding
    if (pad_size - written != 0)
    {
        VoidElement ve(pad_size - written, false);
        ve.write(stream);
    }
    // Move to the end of the file
    stream.seekp(end_pos);
    if (!wrote_seekhead)
    {
        // Write the index at the end
        index.write(stream);
        end_pos = stream.tellp();
    }
    if (!wrote_seginfo)
    {
        // Write the segment info at the end
        info.write(stream);
        end_pos = stream.tellp();
    }

    // Calculate the size of the segment
    size_ = stream.tellp() - offset_ - ids::size(ids::Segment) - 8;
    // Write the size way back at the beginning of the segment
    stream.seekp(static_cast<std::streamsize>(offset_) +
            ids::size(ids::Segment), std::ios::beg);
    write_size(stream);
    // Reset pointers
    stream.seekp(end_pos);
    stream.seekg(cur_read);

    writing_ = false;
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
    VoidElement ve(pad_size_, true);
    return ve.write(output);
}


std::streamsize Segment::read_body(std::istream& input, std::streamsize size)
{
    index.clear();
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
        read_bytes -= id_res.second;
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
                    to_segment_offset(
                        static_cast<std::streamsize>(input.tellg()) -
                        id_res.second)));
                read_bytes += skip_read(input, false);
                break;
            case ids::Tracks:
                have_tracks = true;
                index.insert(std::make_pair(ids::Tracks,
                    to_segment_offset(
                        static_cast<std::streamsize>(input.tellg()) -
                        id_res.second)));
                read_bytes += skip_read(input, false);
                break;
            case ids::Cluster:
                if (!have_clusters)
                {
                    // Only store the first cluster in the index
                    have_clusters = true;
                    index.insert(std::make_pair(ids::Cluster,
                        to_segment_offset(
                            static_cast<std::streamsize>(input.tellg()) -
                            id_res.second)));
                }
                read_bytes += skip_read(input, false);
                break;
            case ids::Cues:
            case ids::Attachments:
            case ids::Chapters:
            case ids::Tags:
                index.insert(std::make_pair(id,
                    to_segment_offset(
                        static_cast<std::streamsize>(input.tellg()) -
                        id_res.second)));
                read_bytes += skip_read(input, false);
                break;
            case ids::Void:
                read_bytes += skip_read(input, false);
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
        input.seekg(to_stream_offset(index.find(ids::Info)->second));
        // Check the ID is correct
        ids::ReadResult id_res = ids::read(input);
        if (id_res.first != ids::Info)
        {
            throw NoSegmentInfo() << err_pos(index.find(ids::Info)->second);
        }
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
    return last_read_end - offset_ - ids::size(ids::Segment) - 8;
}

