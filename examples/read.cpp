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
    std::fstream stream(argv[1], std::ios::in);
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
    if (ebml_el.read_version() > tide::TideVersionMajor)
    {
        std::cerr << "Tide document requires read version " <<
            ebml_el.read_version() << ".\n";
        return 1;
    }
    if (ebml_el.doc_read_version() > tide::TideVersionMajor)
    {
        std::cerr << "Tide document requires doc read version " <<
            ebml_el.read_version() << ".\n";
        return 1;
    }
    std::cerr << "Found EBML header\n";

    // Open the file's segment. This will read some meta-data about the segment
    // and read (or build, if necessary) an index of the level 1 elements. With
    // this index, we will be able to quickly jump to important elements such
    // as the Tracks and the first Cluster.
    tide::Segment segment;
    segment.read(stream);
    // Inspect the segment for some interesting information.
    std::cerr << "Segment information:\n\tUUID: ";
    BOOST_FOREACH(char b, segment.info.uid())
    {
        std::cerr << std::hex << std::setw(2) << std::setfill('0') <<
            +(b & 0xFF);
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

    return 0;
}

