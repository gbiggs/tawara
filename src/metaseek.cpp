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

#include <jonen/metaseek.h>

#include <jonen/ids.h>
#include <jonen/master_element_impl.h>
#include <jonen/seek.h>

#include <boost/foreach.hpp>

using namespace jonen;

///////////////////////////////////////////////////////////////////////////////
// Pimpl implementation
///////////////////////////////////////////////////////////////////////////////

class Metaseek::Impl
{
    public:
        Impl()
            : master_impl_(true)
        {
        }

        std::streamsize body_size() const
        {
            std::streamsize map_size(0);
            BOOST_FOREACH(Metaseek::value_type v, map_)
            {
                // Size of a child element is the ID size, the space to store
                // the body size, and the body size
                std::streamsize id_size = ids::size(ids::SeekID) +
                    vint::size(ids::size(v.first)) + ids::size(v.first);
                std::streamsize offset_size = ids::size(ids::SeekID) +
                    vint::size(ebml_int::size_u(v.second)) +
                    ebml_int::size_u(v.second);
                // Total size of the Seek element
                map_size += ids::size(ids::Seek) +
                    vint::size(id_size + offset_size) + id_size + offset_size;
            }
            return map_size + master_impl_.crc_size();
        }

        bool operator==(Impl const& rhs)
        {
            return map_ == rhs.map_ &&
                master_impl_.crc_enabled() ==
                rhs.master_impl_.crc_enabled();
        }


        bool operator<(Impl const& rhs)
        {
            return map_ < rhs.map_;
        }

        Metaseek::storage_type_ map_;
        impl::MasterElementImpl master_impl_;
};

///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

Metaseek::Metaseek()
    : ElementBase<Metaseek>(ids::Metaseek),
    pimpl_(new Impl()), id_(ids::Metaseek), offset_(0), writing_(false)
{
}


Metaseek::Metaseek(Metaseek const& rhs)
    : ElementBase<Metaseek>(rhs.id_),
    pimpl_(new Impl(*rhs.pimpl_)), id_(rhs.id_), offset_(rhs.offset_),
    writing_(rhs.writing_)
{
}


Metaseek::~Metaseek()
{
    // Nothing to do; defined so that the smart pointer works.
}


///////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////

Metaseek& Metaseek::operator=(Metaseek const& rhs)
{
    *pimpl_ = *rhs.pimpl_;
    id_ = rhs.id_;
    offset_ = rhs.offset_;
    writing_ = rhs.writing_;

    return *this;
}


void Metaseek::swap(Metaseek& other)
{
    using std::swap;

    swap(pimpl_, other.pimpl_);
    swap(id_, other.id_);
    swap(offset_, other.offset_);
    swap(writing_, other.writing_);
}


void jonen::swap(Metaseek& a, Metaseek& b)
{
    a.swap(b);
}


bool Metaseek::operator==(Metaseek const& rhs)
{
    return *pimpl_ == *rhs.pimpl_;
}


bool Metaseek::operator<(Metaseek const& rhs)
{
    return *pimpl_ < *rhs.pimpl_;
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
Metaseek::value_type jonen::make_ms_entry(Metaseek::key_type&& key,
        Metaseek::mapped_type&& offset)
{
    return std::make_pair(key, offset);
}
#else // defined(JONEN_CPLUSPLUS11_SUPPORT)
Metaseek::value_type jonen::make_ms_entry(Metaseek::key_type key,
        Metaseek::mapped_type offset)
{
    return std::make_pair(key, offset);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


std::ostream& jonen::operator<<(std::ostream& o, Metaseek const& rhs)
{
    o << "Metaseek with " << rhs.size() << " entries: [";
    BOOST_FOREACH(Metaseek::value_type v, rhs)
    {
        o << v.first << ':' << v.second << ", ";
    }
    return o << ']';
}


///////////////////////////////////////////////////////////////////////////////
// Metaseek interface
///////////////////////////////////////////////////////////////////////////////

Metaseek::allocator_type Metaseek::get_allocator() const
{
    return pimpl_->map_.get_allocator();
}


Metaseek::iterator Metaseek::begin()
{
    return pimpl_->map_.begin();
}


Metaseek::const_iterator Metaseek::begin() const
{
    return pimpl_->map_.begin();
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
Metaseek::const_iterator Metaseek::cbegin() const
{
    return pimpl_->map_.cbegin();
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


Metaseek::iterator Metaseek::end()
{
    return pimpl_->map_.end();
}


Metaseek::const_iterator Metaseek::end() const
{
    return pimpl_->map_.end();
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
Metaseek::const_iterator Metaseek::cend() const
{
    return pimpl_->map_.cend();
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


Metaseek::reverse_iterator Metaseek::rbegin()
{
    return pimpl_->map_.rbegin();
}


Metaseek::const_reverse_iterator Metaseek::rbegin() const
{
    return pimpl_->map_.rbegin();
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
Metaseek::const_reverse_iterator Metaseek::crbegin() const
{
    return pimpl_->map_.crbegin();
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


Metaseek::reverse_iterator Metaseek::rend()
{
    return pimpl_->map_.rend();
}


Metaseek::const_reverse_iterator Metaseek::rend() const
{
    return pimpl_->map_.rend();
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
Metaseek::const_reverse_iterator Metaseek::crend() const
{
    return pimpl_->map_.crend();
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


bool Metaseek::empty() const
{
    return pimpl_->map_.empty();
}


Metaseek::size_type Metaseek::size() const
{
    return pimpl_->map_.size();
}


Metaseek::size_type Metaseek::max_size() const
{
    return pimpl_->map_.max_size();
}


void Metaseek::clear()
{
    pimpl_->map_.clear();
}


Metaseek::iterator Metaseek::insert(Metaseek::value_type const& value)
{
    return pimpl_->map_.insert(value);
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
Metaseek::iterator Metaseek::insert(Metaseek::value_type&& value)
{
    return pimpl_->map_.insert(value);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
Metaseek::iterator Metaseek::insert(Metaseek::const_iterator hint,
        Metaseek::value_type const& value)
#else // defined(JONEN_CPLUSPLUS11_SUPPORT)
Metaseek::iterator Metaseek::insert(Metaseek::iterator hint,
        Metaseek::value_type const& value)
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
{
    return pimpl_->map_.insert(hint, value);
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
Metaseek::iterator Metaseek::insert(Metaseek::const_iterator hint,
        Metaseek::value_type&& value)
{
    return pimpl_->map.insert(hint, value);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


void Metaseek::insert(Metaseek::const_iterator first,
        Metaseek::const_iterator last)
{
    pimpl_->map_.insert(first, last);
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
Metaseek::iterator Metaseek::emplace(Metaseek::key_type id,
        Metaseek::mapped_type offset)
{
    return pimpl_->map_.emplace(id, offset);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
Metaseek::iterator Metaseek::emplace_hint(Metaseek::const_iterator hint,
        Metaseek::key_type id, Metaseek::mapped_type offset)
{
    return pimpl_->map_.emplace_hint(hint, id, offset);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)

#if defined(JONEN_CPLUSPLUS11_SUPPORT)
Metaseek::iterator Metaseek::erase(Metaseek::const_iterator position)
{
    return pimpl_->map_.erase(position);
}
#else // defined(JONEN_CPLUSPLUS11_SUPPORT)
void Metaseek::erase(Metaseek::iterator position)
{
    pimpl_->map_.erase(position);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
Metaseek::iterator Metaseek::erase(Metaseek::const_iterator first,
        Metaseek::const_iterator last)
{
    return pimpl_->map_.erase(first, last);
}
#else // defined(JONEN_CPLUSPLUS11_SUPPORT)
void Metaseek::erase(Metaseek::iterator first, Metaseek::iterator last)
{
    pimpl_->map_.erase(first, last);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


Metaseek::size_type Metaseek::erase(Metaseek::key_type id)
{
    return pimpl_->map_.erase(id);
}


Metaseek::size_type Metaseek::count(Metaseek::key_type id) const
{
    return pimpl_->map_.count(id);
}


Metaseek::iterator Metaseek::find(Metaseek::key_type id)
{
    return pimpl_->map_.find(id);
}


Metaseek::const_iterator Metaseek::find(Metaseek::key_type id) const
{
    return pimpl_->map_.find(id);
}


std::pair<Metaseek::iterator, Metaseek::iterator> Metaseek::equal_range(
        Metaseek::key_type id)
{
    return pimpl_->map_.equal_range(id);
}


std::pair<Metaseek::const_iterator, Metaseek::const_iterator> Metaseek::equal_range(
        Metaseek::key_type id) const
{
    return pimpl_->map_.equal_range(id);
}


Metaseek::iterator Metaseek::lower_bound(Metaseek::key_type id)
{
    return pimpl_->map_.lower_bound(id);
}


Metaseek::const_iterator Metaseek::lower_bound(Metaseek::key_type id) const
{
    return pimpl_->map_.lower_bound(id);
}


Metaseek::iterator Metaseek::upper_bound(Metaseek::key_type id)
{
    return pimpl_->map_.upper_bound(id);
}


Metaseek::const_iterator Metaseek::upper_bound(Metaseek::key_type id) const
{
    return pimpl_->map_.upper_bound(id);
}


Metaseek::key_compare Metaseek::key_comp() const
{
    return pimpl_->map_.key_comp();
}


Metaseek::value_compare Metaseek::value_comp() const
{
    return pimpl_->map_.value_comp();
}


///////////////////////////////////////////////////////////////////////////////
// Element interface
///////////////////////////////////////////////////////////////////////////////

inline std::streamsize Metaseek::body_stored_size() const
{
    return pimpl_->body_size();
}


std::streamsize Metaseek::read_body(std::istream& i, std::streamsize size)
{
    std::streamsize read_bytes(0);

    boost::scoped_ptr<Impl> new_pimpl(new Impl(*pimpl_));

    // Get the body via a CRC check
    std::vector<char> body;
    read_bytes += new_pimpl->master_impl_.read_with_crc(body, i, size);
    // TODO: This is a lot of copying around just to get things in the right
    // data type.
    std::string temp_str(body.begin(), body.end());
    std::stringstream body_ss(temp_str);

    // Read IDs until the body is exhausted
    std::streamsize body_read(0);
    while (body_read < body.size())
    {
        ids::ReadResult id_res = ids::read(body_ss);
        ids::ID id(id_res.first);
        body_read += id_res.second;
        impl::Seek seek_entry(ids::Null, 0);
        switch(id)
        {
            case ids::Seek:
                body_read += seek_entry.read(body_ss);
                new_pimpl->map_.insert(make_ms_entry(seek_entry.element_id(),
                            seek_entry.segment_offset().value()));
                break;
            default:
                throw InvalidChildID() << err_id(id) << err_par_id(id_) <<
                    // The cast here makes Apple's LLVM compiler happy
                    err_pos(static_cast<std::streamsize>(i.tellg()) -
                            id_res.second - (body.size() - body_read));
        }
    }
    if (read_bytes != size)
    {
        // Read more than was specified by the body size value
        throw BadBodySize() << err_id(id_) << err_el_size(size) <<
            err_pos(offset_);
    }

    pimpl_.swap(new_pimpl);
    return read_bytes;
}


std::streamsize Metaseek::start_body(std::iostream& io) const
{
    std::streamsize result(0);
    // Write the body data to a stringstream first. This can be used to
    // calculate the CRC32 value if necessary.
    std::stringstream body_ss;
    BOOST_FOREACH(Metaseek::value_type v, pimpl_->map_)
    {
        impl::Seek seek_entry(v.first, v.second);
        write(seek_entry, body_ss);
    }
    // TODO: This is a lot of copying around just to get things in the right
    // data type.
    std::string body_str(body_ss.str());
    std::vector<char> body(body_str.begin(), body_str.end());
    result = pimpl_->master_impl_.write_with_crc(body, io);
    // Record the position after this element's body for use in
    // finish_body().
    body_end_ = io.tellp();
    return result;
}


std::streamsize Metaseek::finish_body(std::iostream& io) const
{
    // All writing was taken care of by start_body()
    // Ensure the post-condition that the write pointer is
    // placed after the element's body.
    io.seekp(body_end_);
    return this->stored_size();
}


///////////////////////////////////////////////////////////////////////////////
// MasterElement interface
///////////////////////////////////////////////////////////////////////////////

void Metaseek::enable_crc_impl()
{
    pimpl_->master_impl_.enable_crc();
}


void Metaseek::disable_crc_impl()
{
    pimpl_->master_impl_.disable_crc();
}


bool Metaseek::crc_enabled_impl() const
{
    return pimpl_->master_impl_.crc_enabled();
}

