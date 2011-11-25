/* Tide
 *
 * Source for the Tracks element.
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

#include <tide/tracks.h>

#include <algorithm>
#include <boost/foreach.hpp>
#include <functional>
#include <stdexcept>
#include <tide/el_ids.h>
#include <tide/exceptions.h>
#include <tide/vint.h>

using namespace tide;

///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

Tracks::Tracks()
    : MasterElement(ids::Tracks)
{
}


///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

Tracks::mapped_type& Tracks::operator[](Tracks::key_type const& key)
{
    if (entries_.find(key) == entries_.end())
    {
        std::stringstream str;
        str << key;
        throw std::out_of_range(str.str());
    }
    return entries_[key];
}


Tracks::mapped_type const& Tracks::operator[](Tracks::key_type const& key) const
{
    if (entries_.find(key) == entries_.end())
    {
        std::stringstream str;
        str << key;
        throw std::out_of_range(str.str());
    }
    // Cannot use entries_[key] because it is not a const function.
    return entries_.find(key)->second;
}


std::pair<Tracks::iterator, bool> Tracks::insert(
        Tracks::mapped_type const& value)
{
    verify_not_duplicate(value);
    value_type new_val(value->number(), value);
    return entries_.insert(new_val);
}


void Tracks::insert(const_iterator first, const_iterator last)
{
    const_iterator ii(first);
    while(ii != last)
    {
        verify_not_duplicate(ii->second);
        ++ii;
    }
    entries_.insert(first, last);
}


std::streamsize Tracks::size() const
{
    std::streamsize size(0);
    BOOST_FOREACH(value_type te, entries_)
    {
        size += te.second->total_size();
    }
    return size;
}


///////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////

bool tide::operator==(Tracks const& lhs, Tracks const& rhs)
{
    return lhs.entries_ == rhs.entries_;
}


///////////////////////////////////////////////////////////////////////////////
// I/O
///////////////////////////////////////////////////////////////////////////////

std::streamsize Tracks::write_body(std::ostream& output)
{
    // There must be at least one TrackEntry
    if (entries_.empty())
    {
        throw EmptyTracksElement();
    }

    // Check there are no duplicate track numbers or UIDs
    validate_entries();

    std::streamsize written(0);
    BOOST_FOREACH(value_type te, entries_)
    {
        written += te.second->write(output);
    }
    return written;
}


std::streamsize Tracks::read_body(std::istream& input)
{
    std::streampos el_start(input.tellg());

    // Clear the entries
    entries_.clear();
    // Get the element's body size
    vint::ReadResult result = tide::vint::read(input);
    std::streamsize body_size(result.first);
    std::streamsize size_size(result.second);
    std::streamsize read_bytes(result.second);
    // Read elements until the body is exhausted
    while (read_bytes < size_size + body_size)
    {
        // Read the ID
        ids::ReadResult id_res = tide::ids::read(input);
        ids::ID id(id_res.first);
        read_bytes += id_res.second;
        if (id != ids::TrackEntry)
        {
            // Only TrackEntry elements may be in the Tracks element
            throw InvalidChildID() << err_id(id) << err_par_id(id_) <<
                err_pos(input.tellg());
        }
        // Read the body
        TrackEntry::Ptr entry(new tide::TrackEntry(1, 1, "Empty"));
        read_bytes += entry->read_body(input);
        insert(entry);
    }
    if (read_bytes != size_size + body_size)
    {
        // Read more than was specified by the body size value
        throw BadBodySize() << err_id(id_) << err_el_size(body_size) <<
            err_pos(el_start);
    }
    if (entries_.empty())
    {
        // No TrackEntries is bad.
        throw EmptyTracksElement() << err_pos(el_start);
    }
    // Check there are no duplicate track numbers or UIDs
    validate_entries();

    return read_bytes;
}


///////////////////////////////////////////////////////////////////////////////
// Private functions
///////////////////////////////////////////////////////////////////////////////

void Tracks::validate_entries() const
{
    std::vector<uint64_t> seen_numbers;
    std::vector<uint64_t> seen_uids;
    BOOST_FOREACH(value_type te, entries_)
    {
        if (std::find(seen_numbers.begin(), seen_numbers.end(),
                    te.second->number()) != seen_numbers.end())
        {
            throw DuplicateTrackNumber() << err_track_num(te.second->number());
        }
        if (std::find(seen_uids.begin(), seen_uids.end(),
                    te.second->uid()) != seen_uids.end())
        {
            throw DuplicateUID() << err_int_uid(te.second->uid());
        }
        seen_numbers.push_back(te.second->number());
        seen_uids.push_back(te.second->uid());
    }
}


bool comp_uid(Tracks::value_type entry, uint64_t uid)
{
    return entry.second->uid() == uid;
}

void Tracks::verify_not_duplicate(TrackEntry::Ptr entry) const
{
    if (entries_.find(entry->number()) != entries_.end())
    {
        throw DuplicateTrackNumber() << err_track_num(entry->number());
    }
    if (std::find_if(entries_.begin(), entries_.end(),
                std::bind2nd(std::ptr_fun(comp_uid), entry->uid())) !=
            entries_.end())
    {
        throw DuplicateUID() << err_int_uid(entry->uid());
    }
}

