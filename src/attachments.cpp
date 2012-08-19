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

#include <jonen/attachments.h>

#include <boost/foreach.hpp>

using namespace jonen;

///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

Attachments::Attachments()
    : ElementBase<Attachments>(ids::Attachments),
    master_impl_(true), id_(ids::Attachments), offset_(0), writing_(false)
{
}


Attachments::Attachments(Attachments const& rhs)
    : ElementBase<Attachments>(rhs.id_),
    list_(rhs.list_), master_impl_(rhs.master_impl_),
    id_(rhs.id_), offset_(rhs.offset_), writing_(rhs.writing_)
{
}


Attachments::~Attachments()
{
    // Nothing to do; defined so that the smart pointer works.
}


///////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////

Attachments& Attachments::operator=(Attachments const& rhs)
{
    list_ = rhs.list_;
    master_impl_ = rhs.master_impl_;
    id_ = rhs.id_;
    offset_ = rhs.offset_;
    writing_ = rhs.writing_;

    return *this;
}


void Attachments::swap(Attachments& other)
{
    using std::swap;

    swap(list_, other.list_);
    swap(master_impl_, other.master_impl_);
    swap(id_, other.id_);
    swap(offset_, other.offset_);
    swap(writing_, other.writing_);
}


void jonen::swap(Attachments& a, Attachments& b)
{
    a.swap(b);
}


bool jonen::operator==(Attachments const& lhs, Attachments const& rhs)
{
    return lhs.list_ == rhs.list_;
}


bool jonen::operator<(Attachments const& lhs, Attachments const& rhs)
{
    return lhs.list_ < rhs.list_;
}


std::ostream& jonen::operator<<(std::ostream& o, Attachments const& rhs)
{
    o << "Attachments with " << rhs.size() << " entries: [";
    BOOST_FOREACH(Attachments::value_type const& v, rhs)
    {
        o << v.uid() << ':' << v.name() << ", ";
    }
    return o << ']';
}


///////////////////////////////////////////////////////////////////////////////
// Attachments interface
///////////////////////////////////////////////////////////////////////////////

Attachments::allocator_type Attachments::get_allocator() const
{
    return list_.get_allocator();
}


void Attachments::assign(size_type count, value_type const& value)
{
    list_.assign(count, value);
}


template<typename InputIt>
void Attachments::assign(InputIt first, InputIt last)
{
    list_.assign(first, last);
}


Attachments::reference Attachments::front()
{
    return list_.front();
}


Attachments::const_reference Attachments::front() const
{
    return list_.front();
}


Attachments::reference Attachments::back()
{
    return list_.back();
}


Attachments::const_reference Attachments::back() const
{
    return list_.back();
}


Attachments::iterator Attachments::begin()
{
    return list_.begin();
}


Attachments::const_iterator Attachments::begin() const
{
    return list_.begin();
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
Attachments::const_iterator Attachments::cbegin() const
{
    return list_.cbegin();
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


Attachments::iterator Attachments::end()
{
    return list_.end();
}


Attachments::const_iterator Attachments::end() const
{
    return list_.end();
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
Attachments::const_iterator Attachments::cend() const
{
    return list_.cend();
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


Attachments::reverse_iterator Attachments::rbegin()
{
    return list_.rbegin();
}


Attachments::const_reverse_iterator Attachments::rbegin() const
{
    return list_.rbegin();
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
Attachments::const_reverse_iterator Attachments::crbegin() const
{
    return list_.crbegin();
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


Attachments::reverse_iterator Attachments::rend()
{
    return list_.rend();
}


Attachments::const_reverse_iterator Attachments::rend() const
{
    return list_.rend();
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
Attachments::const_reverse_iterator Attachments::crend() const
{
    return list_.crend();
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


bool Attachments::empty() const
{
    return list_.empty();
}


Attachments::size_type Attachments::size() const
{
    return list_.size();
}


Attachments::size_type Attachments::max_size() const
{
    return list_.max_size();
}


void Attachments::clear()
{
    list_.clear();
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
Attachments::iterator Attachments::insert(Attachments::const_iterator pos,
        Attachments::value_type const& value)
#else // defined(JONEN_CPLUSPLUS11_SUPPORT)
Attachments::iterator Attachments::insert(Attachments::iterator pos,
        Attachments::value_type const& value)
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
{
    return list_.insert(pos, value);
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
Attachments::iterator Attachments::insert(Attachments::const_iterator pos,
        Attachments::value_type&& value)
{
    return list_.insert(pos, value);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
void Attachments::insert(Attachments::const_iterator pos,
        Attachments::size_type count, Attachments::value_type const& value)
#else // defined(JONEN_CPLUSPLUS11_SUPPORT)
void Attachments::insert(Attachments::iterator pos,
        Attachments::size_type count, Attachments::value_type const& value)
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
{
    return list_.insert(pos, count, value);
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
Attachments::iterator Attachments::insert(Attachments::const_iterator pos,
        std::initializer_list<Attachments::value_type> ilist)
{
    return map.insert(pos, ilist);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
template<typename... Args>
Attachments::iterator Attachments::emplace(Attachments::const_iterator pos,
        std::string const& name, std::string const& mime_type,
        std::vector<char> const& data, unsigned long long int uid,
        std::string const& desc)
{
    return list_.emplace(pos, name, mime_type, data, uid, desc);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
Attachments::iterator Attachments::erase(Attachments::const_iterator position)
{
    return list_.erase(position);
}
#else // defined(JONEN_CPLUSPLUS11_SUPPORT)
void Attachments::erase(Attachments::iterator position)
{
    list_.erase(position);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
Attachments::iterator Attachments::erase(Attachments::const_iterator first,
        Attachments::const_iterator last)
{
    return list_.erase(first, last);
}
#else // defined(JONEN_CPLUSPLUS11_SUPPORT)
void Attachments::erase(Attachments::iterator first, Attachments::iterator last)
{
    list_.erase(first, last);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


void Attachments::push_back(Attachments::value_type const& value)
{
    list_.push_back(value);
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
void Attachments::push_back(Attachments::value_type&& value)
{
    list_.push_back(value);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
void Attachments::emplace_back(std::string const& name,
                    std::string const& mime_type,
                    std::vector<char> const& data, unsigned long long int uid,
                    std::string const& desc)
{
    list_.emplace_back(name, mime_type, data, uid, desc);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


void Attachments::pop_back()
{
    list_.pop_back();
}


void Attachments::push_front(Attachments::value_type const& value)
{
    list_.push_front(value);
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
void Attachments::push_front(Attachments::value_type&& value)
{
    list_.push_front(value);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
void Attachments::emplace_front(std::string const& name,
                    std::string const& mime_type,
                    std::vector<char> const& data, unsigned long long int uid,
                    std::string const& desc)
{
    list_.emplace_front(name, mime_type, data, uid, desc);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


void Attachments::pop_front()
{
    list_.pop_front();
}


void Attachments::resize(Attachments::size_type count,
        Attachments::value_type value)
{
    list_.resize(count, value);
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
void Attachments::resize(Attachments::size_type count)
{
    list_.resize(count);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
void Attachments::resize(Attachments::size_type count,
        Attachments::value_type const& value)
{
    list_.resize(count, value);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


void Attachments::merge(Attachments& other)
{
    list_.merge(other.list_);
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
void Attachments::merge(Attachments&& other)
{
    list_.merge(other.list_);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


void Attachments::splice(Attachments::iterator pos, Attachments& other)
{
    list_.splice(pos, other.list_);
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
void Attachments::splice(Attachments::const_iterator pos, Attachments&& other)
{
    list_.splice(pos, other);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


void Attachments::splice(Attachments::iterator pos, Attachments& other,
        Attachments::iterator it)
{
    list_.splice(pos, other.list_, it);
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
void Attachments::splice(Attachments::const_iterator pos, Attachments&& other,
        Attachments::const_iterator it)
{
    list_.splice(pos, other.list_, it);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


void Attachments::splice(Attachments::iterator pos, Attachments& other,
        Attachments::iterator first, Attachments::iterator last)
{
    list_.splice(pos, other.list_, first, last);
}


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
void Attachments::splice(Attachments::const_iterator pos, Attachments&& other,
        Attachments::const_iterator first, Attachments::const_iterator last)
{
    list_.splice(pos, other.list_, first, last);
}
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


void Attachments::remove(Attachments::value_type const& value)
{
    list_.remove(value);
}


void Attachments::reverse()
{
    list_.reverse();
}


void Attachments::unique()
{
    list_.unique();
}


void Attachments::sort()
{
    list_.sort();
}


///////////////////////////////////////////////////////////////////////////////
// Element interface
///////////////////////////////////////////////////////////////////////////////

inline std::streamsize Attachments::body_stored_size() const
{
    std::streamsize list_size(0);
    BOOST_FOREACH(value_type const& v, list_)
    {
        list_size += v.stored_size();
    }
    return list_size + master_impl_.crc_size();
}


std::streamsize Attachments::read_body(std::istream& i, std::streamsize size)
{
    std::streamsize read_bytes(0);

    storage_type_ new_list;
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
        AttachedFile file("a", "a", std::vector<char>(), 1);
        switch(id)
        {
            case ids::AttachedFile:
                body_read += file.read(body_ss);
                new_list.push_back(file);
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

    list_.swap(new_list);
    master_impl_.swap(new_master);
    return read_bytes;
}


std::streamsize Attachments::start_body(std::iostream& io) const
{
    std::streamsize result(0);
    // Write the body data to a stringstream first. This can be used to
    // calculate the CRC32 value if necessary.
    std::stringstream body_ss;
    BOOST_FOREACH(Attachments::value_type const& v, list_)
    {
        write(v, body_ss);
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


std::streamsize Attachments::finish_body(std::iostream& io) const
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

void Attachments::enable_crc_impl()
{
    master_impl_.enable_crc();
}


void Attachments::disable_crc_impl()
{
    master_impl_.disable_crc();
}


bool Attachments::crc_enabled_impl() const
{
    return master_impl_.crc_enabled();
}

