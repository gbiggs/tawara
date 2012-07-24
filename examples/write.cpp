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

#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <iostream>
#include <string>
#include <jonen/ebml_element.h>
#include <jonen/memory_cluster.h>
#include <jonen/segment.h>
#include <jonen/simple_block.h>
#include <jonen/jonen_impl.h>
#include <jonen/tracks.h>
#include <jonen/track_entry.h>

namespace bpt = boost::posix_time;


int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file name>\n";
        return 1;
    }

    // Open a new file and write the EBML Header. This specifies that the file
    // is an EBML file, and is a Jonen document.
    std::fstream stream(argv[1], std::ios::in|std::ios::out|std::ios::trunc);
    jonen::EBMLElement ebml_el;
    ebml_el.write(stream);

    // Open a new segment in the file. This will write some initial meta-data
    // and place some padding at the start of the file for final meta-data to
    // be written after tracks, clusters, etc. have been written.
    jonen::Segment segment;
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
    // possible. It has a sensible default (defined in the Jonen specification),
    // but here we set it to ten milliseconds for demonstrative purposes.
    segment.info.timecode_scale(10000000);
    // The segment's date should be set. It is the somewhat-awkward value of
    // the number of seconds since the start of the millenium. Boost::Date_Time
    // to the rescue!
    bpt::ptime basis(boost::gregorian::date(2001, 1, 1));
    bpt::ptime start(bpt::second_clock::local_time());
    bpt::time_duration td = start - basis;
    segment.info.date(td.total_seconds());
    // Let's give the segment an inspirational title.
    segment.info.title("Example segment");
    // It sometimes helps to know what created a Jonen file.
    segment.info.muxing_app("libjonen-0.1");
    segment.info.writing_app("jonen_eg_write");

    // Set up the tracks meta-data and write it to the file.
    jonen::Tracks tracks;
    // Each track is represented in the Tracks information by a TrackEntry.
    // This specifies such things as the track number, the track's UID and the
    // codec used.
    jonen::TrackEntry::Ptr track(new jonen::TrackEntry(1, 1, "string"));
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
    // cluster implementations are available using different optimisations.
    // Here, we use the implementation that stores all its blocks in memory
    // before writing them all to the file at once. As with the segment,
    // clusters must be opened for writing before blocks are added. Once the
    // cluster is complete, it is finalised. How many blocks each cluster
    // contains is relatively flexible: the only limitation is on the range of
    // block timecodes that can be stored. Each timecode is a signed 16-bit
    // integer, and usually blocks have timecodes that are positive, limiting
    // the range to 32767. The unit of this value is the segment's timecode
    // scale. The default timecode scale therefore gives approximately 65
    // seconds of total range, with 32 seconds being used.
    jonen::MemoryCluster cluster;
    // Again, we add the cluster to the index for faster file opening.
    segment.index.insert(std::make_pair(cluster.id(),
                segment.to_segment_offset(stream.tellp())));
    // The cluster's timecode determines the basis for the timecodes of all
    // blocks in that cluster.
    bpt::ptime c_start(bpt::second_clock::local_time());
    bpt::time_duration c_td = c_start - start;
    cluster.timecode(c_td.total_microseconds() / 10000);
    // Open the cluster for writing so we can begin adding blocks.
    cluster.write(stream);
    // Here, a few blocks are added.
    for (int ii(1); ii <= 5; ++ii)
    {
        std::string frame("frame 1");
        frame[6] = ii + '0';
        // When creating a block, the track number must be specified. In our
        // case, all blocks belong to track 1. A timecode must also be given.
        // It is an offset from the cluster's timecode measured in the
        // segment's timecode scale.
        bpt::ptime b_start(bpt::second_clock::local_time());
        bpt::time_duration b_td = b_start - c_start;
        jonen::BlockElement::Ptr block(new jonen::SimpleBlock(1,
                    b_td.total_microseconds() / 10000));
        // Here the frame data itself is added to the block
        jonen::Block::FramePtr frame_ptr(
            new jonen::Block::Frame(frame.begin(), frame.end()));
        block->push_back(frame_ptr);
        // And then the block is added to its cluster.
        cluster.push_back(block);
    }
    // Finally, now that all blocks have been added, the cluster is finalised.
    cluster.finalise(stream);

    // Now we'll add another cluster, this time using the in-file cluster
    // implementation.
    jonen::FileCluster cluster2;
    // This cluster does not need to be added to the index, as it is easily
    // found during reading by skipping past the first cluster.
    // Give the cluster a timecode.
    c_start = bpt::second_clock::local_time();
    c_td = c_start - start;
    cluster2.timecode(c_td.total_microseconds() / 10000);
    // Open the cluster for writing.
    cluster2.write(stream);
    // Add some blocks.
    for (int ii(0); ii < 10; ++ii)
    {
        std::string frame("frame a");
        frame[6] = ii + 'a';
        // The block's timecode
        bpt::ptime b_start(bpt::second_clock::local_time());
        bpt::time_duration b_td = b_start - c_start;
        jonen::BlockElement::Ptr block(new jonen::SimpleBlock(1,
                    b_td.total_microseconds() / 10000));
        // Add the frame data to the block.
        jonen::Block::FramePtr frame_ptr(
            new jonen::Block::Frame(frame.begin(), frame.end()));
        block->push_back(frame_ptr);
        // Add the block to the cluster.
        cluster2.push_back(block);
    }
    // Finally, now that all blocks have been added, the cluster is finalised.
    cluster2.finalise(stream);

    // Now that all the data has been written, the last thing to do is to
    // finalise the segment.
    segment.finalise(stream);
    // And close the file.
    stream.close();

    return 0;
}

