/* Tide
 *
 * Source file for the (extremely) simple writer example.
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

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/foreach.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <iostream>
#include <tide/tide_config.h>
#include <tide/ebml_element.h>
#include <tide/el_ids.h>
#include <tide/memory_cluster.h>
#include <tide/segment.h>
#include <tide/simple_block.h>
#include <tide/tide_impl.h>
#include <tide/tracks.h>
#include <tide/track_entry.h>


int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file name>\n";
        return 1;
    }

    // Open the file and check for the EBML header. This confirms that the file
    // is an EBML file, and is a Tide document.
    std::ifstream stream(argv[1], std::ios::in);
    tide::ids::ReadResult id = tide::ids::read(stream);
    if (id.first != tide::ids::EBML)
    {
        std::cerr << "File does not begin with an EBML header.\n";
        return 1;
    }
    tide::EBMLElement ebml_el;
    ebml_el.read(stream);
    if (ebml_el.doc_type() != tide::TideDocType)
    {
        std::cerr << "Specified EBML file is not a Tide document.\n";
        return 1;
    }
    if (ebml_el.read_version() > tide::TideEBMLVersion)
    {
        std::cerr << "This Tide document requires read version " <<
            ebml_el.read_version() << ".\n";
        return 1;
    }
    if (ebml_el.doc_read_version() > tide::TideVersionMajor)
    {
        std::cerr << "This Tide document requires doc read version " <<
            ebml_el.read_version() << ".\n";
        return 1;
    }
    std::cerr << "Found EBML header\n";

    // Open the file's segment. This will read some meta-data about the segment
    // and read (or build, if necessary) an index of the level 1 elements. With
    // this index, we will be able to quickly jump to important elements such
    // as the Tracks and the first Cluster.
    id = tide::ids::read(stream);
    if (id.first != tide::ids::Segment)
    {
        std::cerr << "Segment element not found\n";
        return 1;
    }
    tide::Segment segment;
    segment.read(stream);
    // Inspect the segment for some interesting information.
    std::cerr << "Segment information:\n\tUUID: ";
    BOOST_FOREACH(char b, segment.info.uid())
    {
        std::cerr << std::hex << std::setw(2) << std::setfill('0') <<
            +(b & 0xFF) << std::dec;
    }
    std::cerr << "\n\tFile name: " << segment.info.filename() << '\n';
    // This is the segment's timecode scale. It defines the units of all
    // timecodes in the segment as a multiple of nanoseconds.
    std::cerr << "\tTimecode scale: " << segment.info.timecode_scale() << '\n';
    // The segment's date is stored as the number of seconds since the start of
    // the millenium. Boost::Date_Time is invaluable here.
    boost::posix_time::ptime basis(boost::gregorian::date(2001, 1, 1));
    boost::posix_time::time_duration sd(
            boost::posix_time::seconds(segment.info.date()));
    boost::posix_time::ptime start(basis + sd);
    std::cerr << "\tDate: " << start.date() << " (" << start << ")\n";
    std::cerr << "\tTitle: " << segment.info.title() << '\n';
    std::cerr << "\tMuxing app: " << segment.info.muxing_app() << '\n';
    std::cerr << "\tWriting app: " << segment.info.writing_app() << "\n\n";

    // The segment is now open and we can start reading its child elements. To
    // begin with, we get the tracks element (their may be more than one, if
    // the document was created by merging other documents) but generally only
    // one will exist).
    // We can guarantee that there is at least one in the index because
    // otherwise the call to segment.read() would have thrown an error.
    std::streampos tracks_pos(segment.index.find(tide::ids::Tracks)->second);
    stream.seekg(segment.to_stream_offset(tracks_pos));
    // To be sure, we can check it really is a Tracks element, but this is
    // usually not necessary.
    id = tide::ids::read(stream);
    if (id.first != tide::ids::Tracks)
    {
        std::cerr << "Tracks element not at indicated position.\n";
        return 1;
    }
    // Read the tracks
    tide::Tracks tracks;
    tracks.read(stream);
    // Now we can introspect the tracks available in the file.
    if (tracks.empty())
    {
        std::cerr << "No tracks found.\n";
        return 1;
    }
    std::cerr << "Tracks:\n";
    BOOST_FOREACH(tide::Tracks::value_type track, tracks)
    {
        std::cerr << "\tNumber: " << track.second->number() << '\n';
        std::cerr << "\tName: " << track.second->name() << '\n';
        std::cerr << "\tUID: " << track.second->uid() << '\n';
        std::cerr << "\tCodec: " << track.second->codec_name() << " (" <<
            track.second->codec_id() << ")\n";
    }

    // Before we start reading the clusters, let's perform a sanity check to
    // ensure the data is what we expect.
    if (tracks.begin()->second->codec_id() != "string")
    {
        std::cerr << "Track 1 has incorrect codec\n";
        return 1;
    }

    // Now we can start reading the clusters. Get an iterator to the clusters
    // in the segment.
    // In this case, we are using a memory-based cluster implementation, which
    // reads all blocks in the cluster in one go and stores them in memory. For
    // larger quantities of data, using the in-file cluster implementation is
    // typically better.
    int cluster_num(0);
    for (tide::Segment::MemClusterIterator cluster(segment.clusters_begin(stream));
            cluster != segment.clusters_end(stream); ++cluster)
    {
        std::cerr << "Cluster " << cluster_num++ << '\n';
        std::cerr << "\tTimecode: " << cluster->timecode() << '\n';
        std::cerr << "\tBlock count: " << cluster->count() << '\n';
        std::cerr << "\tFrames:\n";
        for (tide::MemoryCluster::Iterator block(cluster->begin());
                block != cluster->end(); ++block)
        {
            // Some blocks may actually contain multiple frames in a lace. In
            // this case, we are reading blocks that do not use lacing, so
            // there is only one frame per block. This is the general case;
            // lacing is typically only used when the frame size is very small
            // to reduce overhead.
            tide::BlockElement::Ptr first_block(*block);
            tide::BlockElement::FramePtr frame_data(*(first_block->begin()));
            std::string frame(frame_data->begin(), frame_data->end());
            std::cerr << "\t\t" << frame << '\n';
            std::cerr << "\t\t\tTrack number: " << first_block->track_number() <<
                '\n';
            std::cerr << "\t\t\tTime code: " << first_block->timecode() << '\n';
        }
    }

/*
    // The call to segment.read() will have placed the first Cluster in the
    // file into the index, so we can jump straight to it.
    std::streampos first_cluster(
            segment.index.find(tide::ids::Cluster)->second);
    stream.seekg(segment.to_stream_offset(first_cluster));
    // To be sure, we can check it really is a Cluster element, but this is
    // typically not necessary.
    id = tide::ids::read(stream);
    if (id.first != tide::ids::Cluster)
    {
        std::cerr << "Cluster element not at indicated position.\n";
        return 1;
    }
    // Read the cluster. This is using the in-memory cluster, which reads all
    // blocks in the cluster in one go and stores them in memory. For larger
    // quantities of data, using the in-file cluster is better.
    tide::MemoryCluster cluster;
    cluster.read(stream);
    // We can now iterate over the blocks in the cluster.
    // Coming soon: iterating over the clusters in a segment, and iterating
    // over all blocks in a segment!
    // Coming a little later: filtering iterators that iterate over all blocks
    // matching given criteria (track number, time step, etc.)!
    std::cerr << "Frames:\n";
    for (tide::MemoryCluster::Iterator block(cluster.begin());
            block != cluster.end(); ++block)
    {
        // Some blocks may actually contain multiple frames in a lace. In this
        // case, we are reading blocks that do not use lacing, so there is only
        // one frame per block. This is the general case; lacing is typically
        // only used when the frame size is very small to reduce overhead.
        tide::BlockElement::Ptr first_block(*block);
        tide::BlockElement::FramePtr frame_data(*(first_block->begin()));
        std::string frame(frame_data->begin(), frame_data->end());
        std::cerr << '\t' << frame << '\n';
        std::cerr << "\t\tTrack number: " << first_block->track_number() << '\n';
        std::cerr << "\t\tTime code: " << first_block->timecode() << '\n';
    }
    */

    return 0;
}

