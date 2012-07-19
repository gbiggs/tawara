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

#include <celduin/ebml_header.h>

#include <celduin/celduin_config.h>
#include <celduin/ids.h>
#include <celduin/id_utils.h>
#include <celduin/integer_elements.h>
#include <celduin/master_element_impl.h>
#include <celduin/string_element.h>

using namespace celduin;

///////////////////////////////////////////////////////////////////////////////
// Pimpl implementation
///////////////////////////////////////////////////////////////////////////////

class EBMLHeader::Impl
{
    public:
        Impl(std::string const& doc_type)
            : ver_(ids::EBMLVersion, CelduinEBMLVersion, 1),
            read_ver_(ids::EBMLReadVersion, CelduinEBMLVersion, 1),
            max_id_length_(ids::EBMLMaxIDLength, 4, 4),
            max_size_length_(ids::EBMLMaxSizeLength, 8, 8),
            doc_type_(ids::DocType, doc_type, CelduinDocType),
            doc_type_ver_(ids::DocTypeVersion, CelduinVersionMajor, 1),
            doc_type_read_ver_(ids::DocTypeReadVersion, CelduinVersionMajor, 1),
            master_impl_(true)
        {
        }

        std::streamsize body_size() const
        {
            return master_impl_.crc_size() + 
                ver_.stored_size() +
                read_ver_.stored_size() +
                max_id_length_.stored_size() +
                max_size_length_.stored_size() +
                doc_type_.stored_size() +
                doc_type_ver_.stored_size() +
                doc_type_read_ver_.stored_size();
        }

        bool operator==(Impl const& rhs)
        {
            return ver_ == rhs.ver_ &&
                read_ver_ == rhs.read_ver_ &&
                max_id_length_ == rhs.max_id_length_ &&
                max_size_length_ == rhs.max_size_length_ &&
                doc_type_ == rhs.doc_type_ &&
                doc_type_ver_ == rhs.doc_type_ver_ &&
                doc_type_read_ver_ == rhs.doc_type_read_ver_ &&
                master_impl_.crc_enabled() == rhs.master_impl_.crc_enabled();
        }

        UIntElement ver_;
        UIntElement read_ver_;
        UIntElement max_id_length_;
        UIntElement max_size_length_;
        StringElement doc_type_;
        UIntElement doc_type_ver_;
        UIntElement doc_type_read_ver_;
        impl::MasterElementImpl master_impl_;
};

///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

EBMLHeader::EBMLHeader(std::string const& doc_type)
    : ElementBase<EBMLHeader>(ids::EBML),
    pimpl_(new Impl(doc_type)), id_(ids::EBML), offset_(0), writing_(false)
{
}


EBMLHeader::EBMLHeader(EBMLHeader const& rhs)
    : ElementBase<EBMLHeader>(rhs.id_),
    pimpl_(new Impl(*rhs.pimpl_)), id_(rhs.id_), offset_(rhs.offset_),
    writing_(rhs.writing_)
{
}


EBMLHeader::~EBMLHeader()
{
    // Nothing to do; defined so that the smart pointer works.
}


///////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////


EBMLHeader& EBMLHeader::operator=(EBMLHeader const& rhs)
{
    *pimpl_ = *rhs.pimpl_;
    id_ = rhs.id_;
    offset_ = rhs.offset_;
    writing_ = rhs.writing_;

    return *this;
}


void EBMLHeader::swap(EBMLHeader& other)
{
    using std::swap;

    swap(pimpl_, other.pimpl_);
    swap(id_, other.id_);
    swap(offset_, other.offset_);
    swap(writing_, other.writing_);
}


void celduin::swap(EBMLHeader& a, EBMLHeader& b)
{
    a.swap(b);
}


bool EBMLHeader::operator==(EBMLHeader const& rhs)
{
    return *pimpl_ == *rhs.pimpl_;
}


std::ostream& celduin::operator<<(std::ostream& o, EBMLHeader const& rhs)
{
    return o << "EBML Header for document type '" << rhs.doc_type() << "' (" <<
        rhs.stored_size() << " bytes)";
}

///////////////////////////////////////////////////////////////////////////////
// EBMLHeader interface
///////////////////////////////////////////////////////////////////////////////

unsigned int EBMLHeader::version() const
{
    return pimpl_->ver_.value();
}


unsigned int EBMLHeader::read_version() const
{
    return pimpl_->read_ver_.value();
}


unsigned int EBMLHeader::max_id_length() const
{
    return pimpl_->max_id_length_.value();
}


void EBMLHeader::max_id_length(unsigned int max_id_length)
{
    pimpl_->max_id_length_ = max_id_length;
}


unsigned int EBMLHeader::max_size_length() const
{
    return pimpl_->max_size_length_.value();
}


void EBMLHeader::max_size_length(unsigned int max_size_length)
{
    pimpl_->max_size_length_ = max_size_length;
}


std::string EBMLHeader::doc_type() const
{
    return pimpl_->doc_type_.value();
}


void EBMLHeader::doc_type(std::string const& doc_type)
{
    pimpl_->doc_type_ = doc_type;
}


unsigned int EBMLHeader::doc_version() const
{
    return pimpl_->doc_type_ver_.value();
}


void EBMLHeader::doc_version(unsigned int doc_version)
{
    pimpl_->doc_type_ver_ = doc_version;
}


unsigned int EBMLHeader::doc_read_version() const
{
    return pimpl_->doc_type_read_ver_.value();
}


void EBMLHeader::doc_read_version(unsigned int doc_read_version)
{
    pimpl_->doc_type_read_ver_ = doc_read_version;
}


///////////////////////////////////////////////////////////////////////////////
// Element interface
///////////////////////////////////////////////////////////////////////////////

inline std::streamsize EBMLHeader::body_stored_size() const
{
    return pimpl_->body_size();
}


std::streamsize EBMLHeader::read_body(std::istream& i, std::streamsize size)
{
    std::streamsize read_bytes(0);

    boost::scoped_ptr<Impl> new_pimpl(new Impl(*pimpl_));

    bool no_ver(true);
    bool no_read_ver(true);
    bool no_max_id_len(true);
    bool no_max_size_len(true);
    bool no_doc_type(true);
    bool no_doc_type_ver(true);
    bool no_doc_type_read_ver(true);

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
            case ids::EBMLVersion:
                body_read += new_pimpl->ver_.read(body_ss);
                no_ver = false;
                break;
            case ids::EBMLReadVersion:
                body_read += new_pimpl->read_ver_.read(body_ss);
                no_read_ver = false;
                break;
            case ids::EBMLMaxIDLength:
                body_read += new_pimpl->max_id_length_.read(body_ss);
                no_max_id_len = false;
                break;
            case ids::EBMLMaxSizeLength:
                body_read += new_pimpl->max_size_length_.read(body_ss);
                no_max_size_len = false;
                break;
            case ids::DocType:
                body_read += new_pimpl->doc_type_.read(body_ss);
                no_doc_type = false;
                break;
            case ids::DocTypeVersion:
                body_read += new_pimpl->doc_type_ver_.read(body_ss);
                no_doc_type_ver = false;
                break;
            case ids::DocTypeReadVersion:
                body_read += new_pimpl->doc_type_read_ver_.read(body_ss);
                no_doc_type_read_ver = false;
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
    if (no_ver || no_read_ver || no_max_id_len || no_max_size_len ||
            no_doc_type || no_doc_type_ver || no_doc_type_read_ver)
    {
        // Missing child (all children are compulsory for the EBML header)
        throw MissingChild() << err_par_id(id_) << err_pos(offset_);
    }

    pimpl_.swap(new_pimpl);
    return read_bytes;
}


std::streamsize EBMLHeader::start_body(std::iostream& io) const
{
    std::streamsize result(0);
    // Write the body data to a stringstream first. This can be used to
    // calculate the CRC32 value if necessary.
    std::stringstream body_ss;
    // The EBML header element always writes every value, regardless of if it
    // is the default or not. If it did not, other implementations may use
    // different defaults and things would go very wrong, very quickly.
    write(pimpl_->ver_, body_ss);
    write(pimpl_->read_ver_, body_ss);
    write(pimpl_->max_id_length_, body_ss);
    write(pimpl_->max_size_length_, body_ss);
    write(pimpl_->doc_type_, body_ss);
    write(pimpl_->doc_type_ver_, body_ss);
    write(pimpl_->doc_type_read_ver_, body_ss);
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


std::streamsize EBMLHeader::finish_body(std::iostream& io) const
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

void EBMLHeader::enable_crc_impl()
{
    pimpl_->master_impl_.enable_crc();
}


void EBMLHeader::disable_crc_impl()
{
    pimpl_->master_impl_.disable_crc();
}


bool EBMLHeader::crc_enabled_impl() const
{
    return pimpl_->master_impl_.crc_enabled();
}

