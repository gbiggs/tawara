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

#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <iostream>
#include <tide/ebml_element.h>
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

    // Open a new file and write the EBML Header. This specifies that the file
    // is an EBML file, and is a Tide document.
    std::fstream stream(argv[1], std::ios::in|std::ios::out|std::ios::trunc);
    tide::EBMLElement ebml_el;
    ebml_el.write(stream);

    // Open a new segment in the file. This will write some initial meta-data
    // and place some padding at the start of the file for final meta-data to
    // be written after tracks, clusters, etc. have been written.
    tide::Segment segment;
    segment.write(stream);
    // Set up the segment information so it can be used while writing tracks
    // and clusters.
    // A UID is not required, but is highly recommended.
    boost::uuids::random_generator gen;
    boost::uuids::uuid uuid = gen();
    std::vector<char> uuid_data(uuid.size());
    std::copy(uuid.begin(), uuid.end(), uuid_data.begin());
    segment.info.uid(uuid_data);
    // The filename can be nice to know.
    segment.info.filename(argv[1]);
    // The segment's timecode scale is possibly the most important value in the
    // segment meta-data data. Without it, timely playback of frames is not
    // possible. It has a sensible default (defined in the Tide specification),
    // but here we set it to ten milliseconds for demonstrative purposes.
    segment.info.timecode_scale(10000000);
    // The segment's date should be set. It is the somewhat-awkward value of
    // the number of seconds since the start of the millenium. Boost::Date_Time
    // to the rescue!
    boost::posix_time::ptime basis(boost::gregorian::date(2001, 1, 1));
    boost::posix_time::ptime start(boost::posix_time::second_clock::local_time());
    boost::posix_time::time_duration td = start - basis;
    segment.info.date(td.total_seconds());
    // Let's give the segment an inspirational title.
    segment.info.title("Example segment");
    // It sometimes helps to know what created a Tide file.
    segment.info.muxing_app("libtide-0.1");
    segment.info.writing_app("tide_eg_write");

    // Set up the tracks meta-data and write it to the file.
    tide::Tracks tracks;
    // Each track is represented in the Tracks information by a TrackEntry.
    // This specifies such things as the track number, the track's UID and the
    // codec used.
    tide::TrackEntry::Ptr track(new tide::TrackEntry(1, 1, "string"));
    track->name("Example frames");
    track->codec_name("ASCII string");
    // Adding each level 1 element (only the first occurance, in the case of
    // clusters) to the index makes opening the file later much faster.
    segment.index.insert(std::make_pair(tracks.id(),
                segment.to_segment_offset(stream.tellp())));
    // Now we can write the Tracks element.
    tracks.insert(track);
    tracks.write(stream);

    // The data itself is stored in clusters. Each cluster contains a number of
    // blocks, with each block containing a single frame of data.  Different
    // cluster implementations are (will be) available using different
    // optimisations. Here, we use the implementation that stores all its
    // blocks in memory before writing them all to the file at once.
    // As with the segment, clusters must be opened for writing before blocks
    // are added. Once the cluster is complete, it is finalised.
    // How many blocks each cluster contains is relatively flexible: the only
    // limitation is on the range of block timecodes that can be stored. Each
    // timecode is a signed 16-bit integer, and usually blocks have timecodes
    // that are positive, limiting the range to 32767. The unit of this value
    // is the segment's timecode scale. The default timecode scale therefore
    // gives approximately 65 seconds of total range, with 32 seconds being
    // used.
    tide::MemoryCluster cluster;
    // Again, we add the cluster to the index for faster file opening.
    segment.index.insert(std::make_pair(cluster.id(),
                segment.to_segment_offset(stream.tellp())));
    // The cluster's timecode determines the basis for the timecodes of all
    // blocks in that cluster.
    boost::posix_time::ptime c_start(
            boost::posix_time::second_clock::local_time());
    boost::posix_time::time_duration c_td = c_start - start;
    cluster.timecode(c_td.total_microseconds() / 10000);
    // Open the cluster for writing so we can begin adding blocks.
    cluster.write(stream);
    // Here, a few blocks are added.
    for (int ii(1); ii <= 5; ++ii)
    {
        std::stringstream frame;
        frame << "Frame " << ii;
        // When creating a block, the track number must be specified. In our
        // case, all blocks belong to track 1. A timecode must also be given.
        // It is an offset from the cluster's timecode measured in the
        // segment's timecode scale.
        boost::posix_time::ptime b_start(
                boost::posix_time::second_clock::local_time());
        boost::posix_time::time_duration b_td = b_start - c_start;
        tide::BlockElement::Ptr block(new tide::SimpleBlock(1,
                    b_td.total_microseconds() / 10000));
        // Here the frame data itself is added to the block
        tide::Block::FramePtr frame_ptr(
            new tide::Block::Frame(frame.str().begin(), frame.str().end()));
        block->push_back(frame_ptr);
        // And then the block is added to its cluster.
        cluster.push_back(block);
    }
    // Finally, now that all blocks have been added, the cluster is finalised.
    cluster.finalise(stream);

    // Now that all the data has been written, the last thing to do is to
    // finalise the segment.
    segment.finalise(stream);
    // And close the file.
    stream.close();

    return 0;
}

