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
            return master_impl_.crc_size();
        }

        bool operator==(Impl const& rhs)
        {
            return master_impl_.crc_enabled() ==
                rhs.master_impl_.crc_enabled();
        }

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


std::ostream& jonen::operator<<(std::ostream& o, Metaseek const& rhs)
{
    return o << "Metaseek with " << "???" << " entries";
}


///////////////////////////////////////////////////////////////////////////////
// Metaseek interface
///////////////////////////////////////////////////////////////////////////////


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
        switch(id)
        {
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

