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

#include <jonen/track_join_blocks.h>

#include <jonen/ebml_integer.h>
#include <jonen/integer_elements.h>
#include <boost/foreach.hpp>

using namespace jonen;

///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

TrackJoinBlocks::TrackJoinBlocks()
    : ElementBase<TrackJoinBlocks>(ids::TrackJoinBlocks),
    master_impl_(true), id_(ids::TrackJoinBlocks), offset_(0), writing_(false)
{
}


TrackJoinBlocks::TrackJoinBlocks(TrackJoinBlocks const& rhs)
    : ElementBase<TrackJoinBlocks>(rhs.id_),
    vector_(rhs.vector_), master_impl_(rhs.master_impl_),
    id_(rhs.id_), offset_(rhs.offset_), writing_(rhs.writing_)
{
}


TrackJoinBlocks::~TrackJoinBlocks()
{
    // Nothing to do; defined so that the smart pointer works.
}


///////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////

TrackJoinBlocks& TrackJoinBlocks::operator=(TrackJoinBlocks const& rhs)
{
    vector_ = rhs.vector_;
    master_impl_ = rhs.master_impl_;
    id_ = rhs.id_;
    offset_ = rhs.offset_;
    writing_ = rhs.writing_;

    return *this;
}


void TrackJoinBlocks::swap(TrackJoinBlocks& other)
{
    using std::swap;

    swap(vector_, other.vector_);
    swap(master_impl_, other.master_impl_);
    swap(id_, other.id_);
    swap(offset_, other.offset_);
    swap(writing_, other.writing_);
}


void jonen::swap(TrackJoinBlocks& a, TrackJoinBlocks& b)
{
    a.swap(b);
}


bool jonen::operator==(TrackJoinBlocks const& lhs, TrackJoinBlocks const& rhs)
{
    return lhs.vector_ == rhs.vector_;
}


bool jonen::operator<(TrackJoinBlocks const& lhs, TrackJoinBlocks const& rhs)
{
    return lhs.vector_ < rhs.vector_;
}


std::ostream& jonen::operator<<(std::ostream& o, TrackJoinBlocks const& rhs)
{
    o << "TrackJoinBlocks with " << rhs.size() << " entries: [";
    BOOST_FOREACH(TrackJoinBlocks::value_type const& v, rhs)
    {
        o << v << ", ";
    }
    return o << ']';
}


///////////////////////////////////////////////////////////////////////////////
// TrackJoinBlocks interface
///////////////////////////////////////////////////////////////////////////////

TrackJoinBlocks::allocator_type TrackJoinBlocks::get_allocator() const
{
    return vector_.get_allocator();
}


void TrackJoinBlocks::assign(size_type count, value_type const& value)
{
    if (value == 0)
    {
        throw ValueOutOfRange() << err_id(ids::TrackJoinUID) <<
            err_par_id(id_);
    }
    vector_.assign(count, value);
}


TrackJoinBlocks::reference TrackJoinBlocks::at(size_type pos)
{
    return vector_.at(pos);
}


TrackJoinBlocks::const_reference TrackJoinBlocks::at(size_type pos) const
{
    return vector_.at(pos);
}


TrackJoinBlocks::reference TrackJoinBlocks::operator[](size_type pos)
{
    return vector_[pos];
}


TrackJoinBlocks::const_reference TrackJoinBlocks::operator[](
        size_type pos) const
{
    return vector_[pos];
}


TrackJoinBlocks::reference TrackJoinBlocks::front()
{
    return vector_.front();
}


TrackJoinBlocks::const_reference TrackJoinBlocks::front() const
{
    return vector_.front();
}


TrackJoinBlocks::reference TrackJoinBlocks::back()
{
    return vector_.back();
}


TrackJoinBlocks::const_reference TrackJoinBlocks::back() const
{
    return vector_.back();
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
TrackJoinBlocks::value_type* TrackJoinBlocks::data()
{
    return vector_.data();
}


TrackJoinBlocks::value_type const* TrackJoinBlocks::data() const
{
    return vector_.data();
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


TrackJoinBlocks::iterator TrackJoinBlocks::begin()
{
    return vector_.begin();
}


TrackJoinBlocks::const_iterator TrackJoinBlocks::begin() const
{
    return vector_.begin();
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
TrackJoinBlocks::const_iterator TrackJoinBlocks::cbegin() const
{
    return vector_.cbegin();
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


TrackJoinBlocks::iterator TrackJoinBlocks::end()
{
    return vector_.end();
}


TrackJoinBlocks::const_iterator TrackJoinBlocks::end() const
{
    return vector_.end();
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
TrackJoinBlocks::const_iterator TrackJoinBlocks::cend() const
{
    return vector_.cend();
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


TrackJoinBlocks::reverse_iterator TrackJoinBlocks::rbegin()
{
    return vector_.rbegin();
}


TrackJoinBlocks::const_reverse_iterator TrackJoinBlocks::rbegin() const
{
    return vector_.rbegin();
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
TrackJoinBlocks::const_reverse_iterator TrackJoinBlocks::crbegin() const
{
    return vector_.crbegin();
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


TrackJoinBlocks::reverse_iterator TrackJoinBlocks::rend()
{
    return vector_.rend();
}


TrackJoinBlocks::const_reverse_iterator TrackJoinBlocks::rend() const
{
    return vector_.rend();
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
TrackJoinBlocks::const_reverse_iterator TrackJoinBlocks::crend() const
{
    return vector_.crend();
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


bool TrackJoinBlocks::empty() const
{
    return vector_.empty();
}


TrackJoinBlocks::size_type TrackJoinBlocks::size() const
{
    return vector_.size();
}


TrackJoinBlocks::size_type TrackJoinBlocks::max_size() const
{
    return vector_.max_size();
}


void TrackJoinBlocks::reserve(size_type size)
{
    vector_.reserve(size);
}


TrackJoinBlocks::size_type TrackJoinBlocks::capacity() const
{
    return vector_.capacity();
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
void TrackJoinBlocks::shrink_to_fit()
{
    vector_.shrink_to_fit();
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


void TrackJoinBlocks::clear()
{
    vector_.clear();
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
TrackJoinBlocks::iterator TrackJoinBlocks::insert(TrackJoinBlocks::const_iterator pos,
        TrackJoinBlocks::value_type const& value)
#else // defined(JONEN_CPLUSPLUS11_SUPPORT)
TrackJoinBlocks::iterator TrackJoinBlocks::insert(TrackJoinBlocks::iterator pos,
        TrackJoinBlocks::value_type const& value)
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
{
    if (value == 0)
    {
        throw ValueOutOfRange() << err_id(ids::TrackJoinUID) <<
            err_par_id(id_);
    }
    return vector_.insert(pos, value);
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
TrackJoinBlocks::iterator TrackJoinBlocks::insert(TrackJoinBlocks::const_iterator pos,
        TrackJoinBlocks::value_type&& value)
{
    if (value == 0)
    {
        throw ValueOutOfRange() << err_id(ids::TrackJoinUID) <<
            err_par_id(id_);
    }
    return vector_.insert(pos, value);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
void TrackJoinBlocks::insert(TrackJoinBlocks::const_iterator pos,
        TrackJoinBlocks::size_type count, TrackJoinBlocks::value_type const& value)
#else // defined(JONEN_CPLUSPLUS11_SUPPORT)
void TrackJoinBlocks::insert(TrackJoinBlocks::iterator pos,
        TrackJoinBlocks::size_type count, TrackJoinBlocks::value_type const& value)
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
{
    if (value == 0)
    {
        throw ValueOutOfRange() << err_id(ids::TrackJoinUID) <<
            err_par_id(id_);
    }
    return vector_.insert(pos, count, value);
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
TrackJoinBlocks::iterator TrackJoinBlocks::insert(TrackJoinBlocks::const_iterator pos,
        std::initializer_list<TrackJoinBlocks::value_type> ilist)
{
    BOOST_FOREACH(value_type v, ilist)
    {
        if (v == 0)
        {
            throw ValueOutOfRange() << err_id(ids::TrackJoinUID) <<
                err_par_id(id_);
        }
    }
    return map.insert(pos, ilist);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
iterator TrackJoinBlocks::emplace(const_iterator pos, value_type value)
{
    if (value == 0)
    {
        throw ValueOutOfRange() << err_id(ids::TrackJoinUID) <<
            err_par_id(id_);
    }
    vector_.emplace(pos, value);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
TrackJoinBlocks::iterator TrackJoinBlocks::erase(TrackJoinBlocks::const_iterator position)
{
    return vector_.erase(position);
}
#else // defined(JONEN_CPLUSPLUS11_SUPPORT)
void TrackJoinBlocks::erase(TrackJoinBlocks::iterator position)
{
    vector_.erase(position);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
TrackJoinBlocks::iterator TrackJoinBlocks::erase(
        TrackJoinBlocks::const_iterator first,
        TrackJoinBlocks::const_iterator last)
{
    return vector_.erase(first, last);
}
#else // defined(JONEN_CPLUSPLUS11_SUPPORT)
void TrackJoinBlocks::erase(TrackJoinBlocks::iterator first,
        TrackJoinBlocks::iterator last)
{
    vector_.erase(first, last);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


void TrackJoinBlocks::push_back(TrackJoinBlocks::value_type const& value)
{
    if (value == 0)
    {
        throw ValueOutOfRange() << err_id(ids::TrackJoinUID) <<
            err_par_id(id_);
    }
    vector_.push_back(value);
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
void TrackJoinBlocks::push_back(TrackJoinBlocks::value_type&& value)
{
    if (value == 0)
    {
        throw ValueOutOfRange() << err_id(ids::TrackJoinUID) <<
            err_par_id(id_);
    }
    vector_.push_back(value);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
void TrackJoinBlocks::emplace_back(value_type value)
{
    if (value == 0)
    {
        throw ValueOutOfRange() << err_id(ids::TrackJoinUID) <<
            err_par_id(id_);
    }
    vector_.emplace_back(args);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


void TrackJoinBlocks::pop_back()
{
    vector_.pop_back();
}


void TrackJoinBlocks::resize(TrackJoinBlocks::size_type count,
        TrackJoinBlocks::value_type value)
{
    if (value == 0)
    {
        throw ValueOutOfRange() << err_id(ids::TrackJoinUID) <<
            err_par_id(id_);
    }
    vector_.resize(count, value);
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
void TrackJoinBlocks::resize(TrackJoinBlocks::size_type count)
{
    vector_.resize(count);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
void TrackJoinBlocks::resize(TrackJoinBlocks::size_type count,
        TrackJoinBlocks::value_type const& value)
{
    if (value == 0)
    {
        throw ValueOutOfRange() << err_id(ids::TrackJoinUID) <<
            err_par_id(id_);
    }
    vector_.resize(count, value);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


///////////////////////////////////////////////////////////////////////////////
// Element interface
///////////////////////////////////////////////////////////////////////////////

inline std::streamsize TrackJoinBlocks::body_stored_size() const
{
    std::streamsize vector_size(0);
    BOOST_FOREACH(value_type v, vector_)
    {
        vector_size += ids::size(ids::TrackJoinUID) +
            vint::size(ebml_int::size_u(v)) +
            ebml_int::size_u(v);
    }
    return vector_size + master_impl_.crc_size();
}


std::streamsize TrackJoinBlocks::read_body(std::istream& i, std::streamsize size)
{
    std::streamsize read_bytes(0);

    storage_type_ new_vector;
    impl::MasterElementImpl new_master(crc_enabled());

    // Get the body via a CRC check
    std::vector<char> body;
    read_bytes += new_master.read_with_crc(body, i, size);
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
        UIntElement uid(ids::Null, 0);
        switch(id)
        {
            case ids::TrackJoinUID:
                body_read += uid.read(body_ss);
                if (uid == 0)
                {
                    throw ValueOutOfRange() << err_id(id) << err_par_id(id_) <<
                    // The cast here makes Apple's LLVM compiler happy
                    err_pos(static_cast<std::streamsize>(i.tellg()) -
                            id_res.second - (body.size() - body_read));
                }
                new_vector.push_back(uid.value());
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

    vector_.swap(new_vector);
    master_impl_.swap(new_master);
    return read_bytes;
}


std::streamsize TrackJoinBlocks::start_body(std::iostream& io) const
{
    // Pre-condition: all UIDs must be greater than zero
    BOOST_FOREACH(TrackJoinBlocks::value_type v, vector_)
    {
        if (v == 0)
        {
            throw ValueOutOfRange() << err_id(ids::TrackJoinUID) <<
                err_par_id(id_);
        }
    }
    std::streamsize result(0);
    // Write the body data to a stringstream first. This can be used to
    // calculate the CRC32 value if necessary.
    std::stringstream body_ss;
    BOOST_FOREACH(TrackJoinBlocks::value_type v, vector_)
    {
        UIntElement uid(ids::TrackJoinUID, v);
        write(uid, body_ss);
    }
    // TODO: This is a lot of copying around just to get things in the right
    // data type.
    std::string body_str(body_ss.str());
    std::vector<char> body(body_str.begin(), body_str.end());
    result = master_impl_.write_with_crc(body, io);
    // Record the position after this element's body for use in
    // finish_body().
    body_end_ = io.tellp();
    return result;
}


std::streamsize TrackJoinBlocks::finish_body(std::iostream& io) const
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

void TrackJoinBlocks::enable_crc_impl()
{
    master_impl_.enable_crc();
}


void TrackJoinBlocks::disable_crc_impl()
{
    master_impl_.disable_crc();
}


bool TrackJoinBlocks::crc_enabled_impl() const
{
    return master_impl_.crc_enabled();
}


