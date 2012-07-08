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

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/foreach.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <iostream>
#include <celduin/celduin_config.h>
#include <celduin/ebml_element.h>
#include <celduin/el_ids.h>
#include <celduin/memory_cluster.h>
#include <celduin/segment.h>
#include <celduin/simple_block.h>
#include <celduin/celduin_impl.h>
#include <celduin/tracks.h>
#include <celduin/track_entry.h>

namespace bpt = boost::posix_time;


int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file name>\n";
        return 1;
    }

    // Open the file and check for the EBML header. This confirms that the file
    // is an EBML file, and is a Celduin document.
    std::ifstream stream(argv[1], std::ios::in);
    celduin::ids::ReadResult id = celduin::ids::read(stream);
    if (id.first != celduin::ids::EBML)
    {
        std::cerr << "File does not begin with an EBML header.\n";
        return 1;
    }
    celduin::EBMLElement ebml_el;
    ebml_el.read(stream);
    if (ebml_el.doc_type() != celduin::CelduinDocType)
    {
        std::cerr << "Specified EBML file is not a Celduin document.\n";
        return 1;
    }
    if (ebml_el.read_version() > celduin::CelduinEBMLVersion)
    {
        std::cerr << "This Celduin document requires read version " <<
            ebml_el.read_version() << ".\n";
        return 1;
    }
    if (ebml_el.doc_read_version() > celduin::CelduinVersionMajor)
    {
        std::cerr << "This Celduin document requires doc read version " <<
            ebml_el.read_version() << ".\n";
        return 1;
    }
    std::cerr << "Found EBML header\n";

    // Open the file's segment. This will read some meta-data about the segment
    // and read (or build, if necessary) an index of the level 1 elements. With
    // this index, we will be able to quickly jump to important elements such
    // as the Tracks and the first Cluster.
    id = celduin::ids::read(stream);
    if (id.first != celduin::ids::Segment)
    {
        std::cerr << "Segment element not found\n";
        return 1;
    }
    celduin::Segment segment;
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
    // The segment's date is stored as the number of nanoseconds since the
    // start of the millenium. Boost::Date_Time is invaluable here.
    bpt::ptime basis(boost::gregorian::date(2001, 1, 1));
    bpt::time_duration sd(bpt::seconds(segment.info.date()));
    bpt::ptime start(basis + sd);
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
    std::streampos tracks_pos(segment.index.find(celduin::ids::Tracks)->second);
    stream.seekg(segment.to_stream_offset(tracks_pos));
    // To be sure, we can check it really is a Tracks element, but this is
    // usually not necessary.
    id = celduin::ids::read(stream);
    if (id.first != celduin::ids::Tracks)
    {
        std::cerr << "Tracks element not at indicated position.\n";
        return 1;
    }
    // Read the tracks
    celduin::Tracks tracks;
    tracks.read(stream);
    // Now we can introspect the tracks available in the file.
    if (tracks.empty())
    {
        std::cerr << "No tracks found.\n";
        return 1;
    }
    std::cerr << "Tracks:\n";
    BOOST_FOREACH(celduin::Tracks::value_type track, tracks)
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
    for (celduin::Segment::MemClusterIterator cluster(segment.clusters_begin_mem(stream));
            cluster != segment.clusters_end_mem(stream); ++cluster)
    {
        std::cerr << "Cluster " << cluster_num++ << '\n';
        bpt::ptime c_start(start + bpt::microseconds(
                cluster->timecode() * segment.info.timecode_scale() / 1000));
        std::cerr << "\tTimecode: " << c_start << " (" <<
            cluster->timecode() << ")\n";
        std::cerr << "\tBlock count: " << cluster->count() << '\n';
        std::cerr << "\tFrames:\n";
        for (celduin::MemoryCluster::Iterator block(cluster->begin());
                block != cluster->end(); ++block)
        {
            // Some blocks may actually contain multiple frames in a lace. In
            // this case, we are reading blocks that do not use lacing, so
            // there is only one frame per block. This is the general case;
            // lacing is typically only used when the frame size is very small
            // to reduce overhead.
            celduin::BlockElement::Ptr first_block(*block);
            celduin::BlockElement::FramePtr frame_data(*(first_block->begin()));
            std::string frame(frame_data->begin(), frame_data->end());
            std::cerr << "\t\t" << frame << '\n';
            std::cerr << "\t\t\tTrack number: " << first_block->track_number() <<
                '\n';
            bpt::ptime blk_time(c_start + bpt::microseconds(
                    first_block->timecode() * segment.info.timecode_scale() / 1000));
            std::cerr << "\t\t\tTime code: " << blk_time << " (" <<
                first_block->timecode() << ")\n";
        }
    }

    // Do the same thing, using the all-blocks-in-a-segment iterator and the
    // in-file cluster implementation.
    std::cerr << "All blocks in the segment:\n";
    for (celduin::Segment::FileBlockIterator block(segment.blocks_begin_file(stream));
            block != segment.blocks_end_file(stream); ++block)
    {
        celduin::BlockElement::FramePtr frame_data(*block->begin());
        std::string frame(frame_data->begin(), frame_data->end());
        std::cerr << "\t" << frame << '\n';
        std::cerr << "\t\tTrack number: " << block->track_number() <<
            '\n';
        std::cerr << "\t\tTime code: " << block->timecode() << '\n';
    }

    return 0;
}

