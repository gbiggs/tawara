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

#define BOOST_DATE_TIME_POSIX_TIME_STD_CONFIG

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/foreach.hpp>
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

namespace bpt = boost::posix_time;


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
            +(b & 0xFF);
    }
    std::cerr << std::dec;
    std::cerr << "\n\tFile name: " << segment.info.filename() << '\n';
    // This is the segment's timecode scale. It defines the units of all
    // timecodes in the segment as a multiple of nanoseconds.
    std::cerr << "\tTimecode scale: " << segment.info.timecode_scale() << '\n';
    // The segment's date is stored as the number of seconds since the start of
    // the millenium. Boost::Date_Time is invaluable here.
    bpt::ptime basis(boost::gregorian::date(2001, 1, 1));
    bpt::ptime start(basis + bpt::seconds(segment.info.date() / 1e9) +
            bpt::nanoseconds(segment.info.date() % 1000000000));
    std::cerr << "\tDate: " << start << " (" << segment.info.date() << ")\n";
    std::cerr << "\tTitle: " << segment.info.title() << '\n';
    std::cerr << "\tMuxing app: " << segment.info.muxing_app() << '\n';
    std::cerr << "\tWriting app: " << segment.info.writing_app() << "\n\n";

    // The segment is now open and we can start reading its child elements. To
    // begin with, we get the tracks element (their may be more than one, if
    // the document was created by merging other documents, but generally only
    // one will exist).
    // We can guarantee that there is at least one in the index because
    // otherwise the call to segment.read() would have thrown an error.
    std::streampos tracks_pos(segment.index.find(tide::ids::Tracks)->second);
    stream.seekg(segment.to_stream_offset(tracks_pos));
    // Read the tracks
    tide::ids::read(stream); // Read and ignore the Tracks ID
    tide::Tracks tracks;
    tracks.read(stream);
    // Now we can introspect the tracks available in the file.
    if (tracks.empty())
    {
        std::cerr << "No tracks found.\n";
        return 0;
    }
    std::cerr << "Tracks:\n";
    BOOST_FOREACH(tide::Tracks::value_type track, tracks)
    {
        std::cerr << "\tNumber: " << track.second->number() << '\n';
        std::cerr << "\tName: " << track.second->name() << '\n';
        std::cerr << "\tUID: " << track.second->uid() << '\n';
        std::cerr << "\tCodec: " << track.second->codec_name() << " (" <<
            track.second->codec_id() << ")\n\n";
    }

    // Now we will iterate over every cluster in the file and print out some
    // interesting statistics.
    std::cerr << "Clusters:\n";

    int cluster_num(0);
    for (tide::Segment::FileClusterIterator cluster(segment.clusters_begin_file(stream));
            cluster != segment.clusters_end_file(stream); ++cluster)
    {
        std::cerr << "\tCluster " << cluster_num++ << '\n';
        bpt::ptime c_start(start + bpt::microseconds(
                cluster->timecode() * segment.info.timecode_scale() / 1000));
        std::cerr << "\t\tTimecode: " << c_start << " (" <<
            cluster->timecode() << ")\n";
        std::cerr << "\t\tBlock count: " << cluster->count() << '\n';
        std::streamsize avg_frame_size(0);
        for (tide::FileCluster::Iterator block(cluster->begin());
                block != cluster->end(); ++block)
        {
            tide::BlockElement::FramePtr frame_data(*block->begin());
            avg_frame_size += frame_data->size();
        }
        if (cluster->count() != 0)
        {
            std::cerr << "\t\tAverage block size: " <<
                avg_frame_size / cluster->count() << '\n';
        }
        else
        {
            std::cerr <<
                "\t\tAverage block size: 0 (probably a corrupt file)\n";
        }
    }

    return 0;
}

