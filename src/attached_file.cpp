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

#include <jonen/attached_file.h>

#include <jonen/jonen_config.h>
#include <jonen/ids.h>
#include <jonen/master_element_impl.h>

using namespace jonen;


///////////////////////////////////////////////////////////////////////////////
// Pimpl implementation
///////////////////////////////////////////////////////////////////////////////

class AttachedFile::Impl
{
    public:
        Impl()
            : desc_(ids::FileDescription, ""),
            name_(ids::FileName, ""),
            mime_(ids::FileMimeType, ""),
            data_(ids::FileData, std::vector<char>()),
            uid_(ids::FileUID, 0),
            master_impl_(true)
        {
        }

        Impl(std::string const& name, std::string const& mime,
                std::vector<char> const& data, unsigned long long int uid,
                std::string const& desc="")
            : desc_(ids::FileDescription, desc),
            name_(ids::FileName, name),
            mime_(ids::FileMimeType, mime),
            data_(ids::FileData, data),
            uid_(ids::FileUID, uid),
            master_impl_(true)
        {
        }

        std::streamsize body_size() const
        {
            std::streamsize body_size(0);
            body_size = name_.stored_size() + mime_.stored_size() +
                data_.stored_size() + uid_.stored_size();
            if (!desc_.empty())
            {
                body_size += desc_.stored_size();
            }
            return body_size + master_impl_.crc_size();
        }

        bool operator==(Impl const& rhs)
        {
            return desc_ == rhs.desc_ &&
                name_ == rhs.name_ &&
                mime_ == rhs.mime_ &&
                data_ == rhs.data_ &&
                uid_ == rhs.uid_ &&
                master_impl_.crc_enabled() == rhs.master_impl_.crc_enabled();
        }

        StringElement desc_;
        StringElement name_;
        StringElement mime_;
        BinaryElement data_;
        UIntElement uid_;
        impl::MasterElementImpl master_impl_;
};


///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

AttachedFile::AttachedFile(std::string const& name,
        std::string const& mime_type, std::vector<char> const& data,
        unsigned long long int uid, std::string const& desc)
    : ElementBase<AttachedFile>(ids::AttachedFile),
    pimpl_(new Impl(name, mime_type, data, uid, desc)),
    id_(ids::AttachedFile), offset_(0), writing_(false)
{
    if (name.empty())
    {
        throw ValueOutOfRange() << err_id(ids::FileName) <<
            err_par_id(ids::AttachedFile);
    }
    if (mime_type.empty())
    {
        throw ValueOutOfRange() << err_id(ids::FileMimeType) <<
            err_par_id(ids::AttachedFile);
    }
    if (uid == 0)
    {
        throw ValueOutOfRange() << err_id(ids::FileUID) <<
            err_par_id(ids::AttachedFile);
    }
}


AttachedFile::AttachedFile(AttachedFile const& rhs)
    : ElementBase<AttachedFile>(rhs.id_),
    pimpl_(new Impl(*rhs.pimpl_)), id_(rhs.id_), offset_(rhs.offset_),
    writing_(rhs.writing_)
{
}


AttachedFile::~AttachedFile()
{
    // Nothing to do; defined so that the smart pointer works.
}


///////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////

AttachedFile& AttachedFile::operator=(AttachedFile const& rhs)
{
    *pimpl_ = *rhs.pimpl_;
    id_ = rhs.id_;
    offset_ = rhs.offset_;
    writing_ = rhs.writing_;

    return *this;
}


void AttachedFile::swap(AttachedFile& other)
{
    using std::swap;

    swap(pimpl_, other.pimpl_);
    swap(id_, other.id_);
    swap(offset_, other.offset_);
    swap(writing_, other.writing_);
}


void jonen::swap(AttachedFile& a, AttachedFile& b)
{
    a.swap(b);
}


bool AttachedFile::operator==(AttachedFile const& rhs)
{
    return *pimpl_ == *rhs.pimpl_;
}


std::ostream& jonen::operator<<(std::ostream& o, AttachedFile const& rhs)
{
    return o << "Attached file " << rhs.description() << " ('" << rhs.name() <<
        "', " << rhs.data().size() << " bytes)";
}


///////////////////////////////////////////////////////////////////////////////
// AttachedFile interface
///////////////////////////////////////////////////////////////////////////////

StringElement const& AttachedFile::description() const
{
    return pimpl_->desc_;
}


void AttachedFile::description(StringElement const& description)
{
    pimpl_->desc_.value(description.value());
}


void AttachedFile::description(std::string const& description)
{
    pimpl_->desc_.value(description);
}


StringElement const& AttachedFile::name() const
{
    return pimpl_->name_;
}


void AttachedFile::name(StringElement const& name)
{
    if (name.empty())
    {
        throw ValueOutOfRange() << err_id(ids::FileName) <<
            err_par_id(ids::AttachedFile);
    }
    pimpl_->name_.value(name.value());
}


void AttachedFile::name(std::string const& name)
{
    if (name.empty())
    {
        throw ValueOutOfRange() << err_id(ids::FileName) <<
            err_par_id(ids::AttachedFile);
    }
    pimpl_->name_.value(name);
}


StringElement const& AttachedFile::mime_type() const
{
    return pimpl_->mime_;
}


void AttachedFile::mime_type(StringElement const& mime_type) const
{
    if (mime_type.empty())
    {
        throw ValueOutOfRange() << err_id(ids::FileMimeType) <<
            err_par_id(ids::AttachedFile);
    }
    pimpl_->mime_.value(mime_type.value());
}


void AttachedFile::mime_type(std::string const& mime_type) const
{
    if (mime_type.empty())
    {
        throw ValueOutOfRange() << err_id(ids::FileMimeType) <<
            err_par_id(ids::AttachedFile);
    }
    pimpl_->mime_.value(mime_type);
}


BinaryElement const& AttachedFile::data() const
{
    return pimpl_->data_;
}


void AttachedFile::data(BinaryElement const& data)
{
    pimpl_->data_.value(data.value());
}


void AttachedFile::data(std::vector<char> const& data)
{
    pimpl_->data_.value(data);
}


UIntElement const& AttachedFile::uid() const
{
    return pimpl_->uid_;
}


void AttachedFile::uid(UIntElement const& uid)
{
    if (uid == 0)
    {
        throw ValueOutOfRange() << err_id(ids::FileUID) <<
            err_par_id(ids::AttachedFile);
    }
    pimpl_->uid_.value(uid.value());
}


void AttachedFile::uid(unsigned long long int const& uid)
{
    if (uid == 0)
    {
        throw ValueOutOfRange() << err_id(ids::FileUID) <<
            err_par_id(ids::AttachedFile);
    }
    pimpl_->uid_.value(uid);
}


///////////////////////////////////////////////////////////////////////////////
// Element interface
///////////////////////////////////////////////////////////////////////////////

inline std::streamsize AttachedFile::body_stored_size() const
{
    return pimpl_->body_size();
}


std::streamsize AttachedFile::read_body(std::istream& i, std::streamsize size)
{
    std::streamsize read_bytes(0);

    bool no_name(true), no_mime(true), no_data(true), no_uid(true);

    // Start with a clean slate
    boost::scoped_ptr<Impl> new_pimpl(new Impl());

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
            case ids::FileDescription:
                body_read += new_pimpl->desc_.read(body_ss);
                break;
            case ids::FileName:
                body_read += new_pimpl->name_.read(body_ss);
                no_name = false;
                break;
            case ids::FileMimeType:
                body_read += new_pimpl->mime_.read(body_ss);
                no_mime = false;
                break;
            case ids::FileData:
                body_read += new_pimpl->data_.read(body_ss);
                no_data = false;
                break;
            case ids::FileUID:
                body_read += new_pimpl->uid_.read(body_ss);
                no_uid = false;
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
    if (no_name || no_mime || no_data || no_uid)
    {
        // Missing child
        throw MissingChild() << err_par_id(id_) << err_pos(offset_);
    }

    pimpl_.swap(new_pimpl);
    return read_bytes;
}


std::streamsize AttachedFile::start_body(std::iostream& io) const
{
    std::streamsize result(0);
    // Write the body data to a stringstream first. This can be used to
    // calculate the CRC32 value if necessary.
    std::stringstream body_ss;
    // Only write children for which there is a valid value, except for the
    // time code scale, which always gets written.
    if (!pimpl_->desc_.empty())
    {
        write(pimpl_->desc_, body_ss);
    }
    write(pimpl_->name_, body_ss);
    write(pimpl_->mime_, body_ss);
    write(pimpl_->data_, body_ss);
    write(pimpl_->uid_, body_ss);
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


std::streamsize AttachedFile::finish_body(std::iostream& io) const
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

void AttachedFile::enable_crc_impl()
{
    pimpl_->master_impl_.enable_crc();
}


void AttachedFile::disable_crc_impl()
{
    pimpl_->master_impl_.disable_crc();
}


bool AttachedFile::crc_enabled_impl() const
{
    return pimpl_->master_impl_.crc_enabled();
}

