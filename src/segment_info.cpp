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

#include <jonen/segment_info.h>

#include <jonen/jonen_config.h>
#include <jonen/ids.h>
#include <jonen/master_element_impl.h>

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <set>

using namespace jonen;


// Default time code scale value
unsigned long long int const DEFAULT_TC_SCALE(1000000);

///////////////////////////////////////////////////////////////////////////////
// Pimpl implementation
///////////////////////////////////////////////////////////////////////////////

class SegmentInfo::Impl
{
    public:
        Impl()
            : uid_(ids::SegmentUID, std::vector<char>()),
            filename_(ids::SegmentFileName, ""),
            prev_uid_(ids::PrevUID, std::vector<char>()),
            prev_fn_(ids::PrevFileName, ""),
            next_uid_(ids::NextUID, std::vector<char>()),
            next_fn_(ids::NextFileName, ""),
            tc_scale_(ids::TimecodeScale, DEFAULT_TC_SCALE, DEFAULT_TC_SCALE),
            duration_(ids::Duration, 0),
            have_duration_(false),
            date_(ids::DateUTC, boost::posix_time::not_a_date_time),
            have_date_(false),
            title_(ids::Title, ""),
            muxing_app_(ids::MuxingApp, ""),
            writing_app_(ids::WritingApp, ""),
            master_impl_(true)
        {
        }

        std::streamsize body_size() const
        {
            std::streamsize body_size(0);
            if (!uid_.empty())
            {
                body_size += uid_.stored_size();
            }
            if (!filename_.empty())
            {
                body_size += filename_.stored_size();
            }
            if (!prev_uid_.empty())
            {
                body_size += prev_uid_.stored_size();
            }
            if (!prev_fn_.empty())
            {
                body_size += prev_fn_.stored_size();
            }
            if (!next_uid_.empty())
            {
                body_size += next_uid_.stored_size();
            }
            if (!next_fn_.empty())
            {
                body_size += next_fn_.stored_size();
            }
            BOOST_FOREACH(BinaryElement const& f, families_)
            {
                body_size += f.stored_size();
            }
            body_size += tc_scale_.stored_size();
            if (have_duration_)
            {
                body_size += duration_.stored_size();
            }
            if (have_date_)
            {
                body_size += date_.stored_size();
            }
            if (!title_.empty())
            {
                body_size += title_.stored_size();
            }
            if (!muxing_app_.empty())
            {
                body_size += muxing_app_.stored_size();
            }
            if (!writing_app_.empty())
            {
                body_size += writing_app_.stored_size();
            }
            return body_size + master_impl_.crc_size();
        }

        bool operator==(Impl const& rhs)
        {
            bool duration_eq(have_duration_ == rhs.have_duration_ &&
                (have_duration_ ? duration_ == rhs.duration_ : true));
            bool date_eq(have_date_ == rhs.have_date_ &&
                (have_date_ ? date_ == rhs.date_ : true));
            return uid_ == rhs.uid_ &&
                filename_ == rhs.filename_ &&
                prev_uid_ == rhs.prev_uid_ &&
                prev_fn_ == rhs.prev_fn_ &&
                next_uid_ == rhs.next_uid_ &&
                next_fn_ == rhs.next_fn_ &&
                families_ == rhs.families_ &&
                tc_scale_ == rhs.tc_scale_ &&
                duration_eq && date_eq &&
                title_ == rhs.title_ &&
                muxing_app_ == rhs.muxing_app_ &&
                writing_app_ == rhs.writing_app_ &&
                master_impl_.crc_enabled() == rhs.master_impl_.crc_enabled();
        }

        BinaryElement uid_;
        StringElement filename_;
        BinaryElement prev_uid_;
        StringElement prev_fn_;
        BinaryElement next_uid_;
        StringElement next_fn_;
        std::set<BinaryElement> families_;
        UIntElement tc_scale_;
        FloatElement duration_;
        bool have_duration_;
        DateElement date_;
        bool have_date_;
        StringElement title_;
        StringElement muxing_app_;
        StringElement writing_app_;
        impl::MasterElementImpl master_impl_;
};


///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

SegmentInfo::SegmentInfo()
    : ElementBase<SegmentInfo>(ids::EBML),
    pimpl_(new Impl()), id_(ids::EBML), offset_(0), writing_(false)
{
}


SegmentInfo::SegmentInfo(SegmentInfo const& rhs)
    : ElementBase<SegmentInfo>(rhs.id_),
    pimpl_(new Impl(*rhs.pimpl_)), id_(rhs.id_), offset_(rhs.offset_),
    writing_(rhs.writing_)
{
}


SegmentInfo::~SegmentInfo()
{
    // Nothing to do; defined so that the smart pointer works.
}


///////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////

SegmentInfo& SegmentInfo::operator=(SegmentInfo const& rhs)
{
    *pimpl_ = *rhs.pimpl_;
    id_ = rhs.id_;
    offset_ = rhs.offset_;
    writing_ = rhs.writing_;

    return *this;
}


void SegmentInfo::swap(SegmentInfo& other)
{
    using std::swap;

    swap(pimpl_, other.pimpl_);
    swap(id_, other.id_);
    swap(offset_, other.offset_);
    swap(writing_, other.writing_);
}


void jonen::swap(SegmentInfo& a, SegmentInfo& b)
{
    a.swap(b);
}


bool SegmentInfo::operator==(SegmentInfo const& rhs)
{
    return *pimpl_ == *rhs.pimpl_;
}


std::ostream& jonen::operator<<(std::ostream& o, SegmentInfo const& rhs)
{
    boost::uuids::uuid uid;
    std::copy(rhs.uid().value().begin(), rhs.uid().value().end(), uid.begin());
    return o << "SegmentInfo element for segment " << uid << " in file '" <<
        rhs.filename() << '\'';
}


///////////////////////////////////////////////////////////////////////////////
// SegmentInfo interface
///////////////////////////////////////////////////////////////////////////////

BinaryElement const& SegmentInfo::uid() const
{
    return pimpl_->uid_;
}

void SegmentInfo::uid(BinaryElement const& uid)
{
    pimpl_->uid_.value(uid.value());
}


void SegmentInfo::uid(std::vector<char> const& uid)
{
    pimpl_->uid_.value(uid);
}


StringElement const& SegmentInfo::filename() const
{
    return pimpl_->filename_;
}


void SegmentInfo::filename(StringElement const& filename)
{
    pimpl_->filename_.value(filename.value());
}


void SegmentInfo::filename(std::string const& filename)
{
    pimpl_->filename_.value(filename);
}


BinaryElement const& SegmentInfo::previous_uid() const
{
    return pimpl_->prev_uid_;
}


void SegmentInfo::previous_uid(BinaryElement const& previous_uid)
{
    pimpl_->prev_uid_.value(previous_uid.value());
}


void SegmentInfo::previous_uid(std::vector<char> const& previous_uid)
{
    pimpl_->prev_uid_.value(previous_uid);
}


StringElement const& SegmentInfo::previous_fn() const
{
    return pimpl_->prev_fn_;
}


void SegmentInfo::previous_fn(StringElement const& prev_fn)
{
    pimpl_->prev_fn_.value(prev_fn.value());
}


void SegmentInfo::previous_fn(std::string const& prev_fn)
{
    pimpl_->prev_fn_.value(prev_fn);
}


BinaryElement const& SegmentInfo::next_uid() const
{
    return pimpl_->next_uid_;
}


void SegmentInfo::next_uid(BinaryElement const& next_uid)
{
    pimpl_->next_uid_.value(next_uid.value());
}


void SegmentInfo::next_uid(std::vector<char> const& next_uid)
{
    pimpl_->next_uid_.value(next_uid);
}


StringElement const& SegmentInfo::next_fn() const
{
    return pimpl_->next_fn_;
}


void SegmentInfo::next_fn(StringElement const& next_fn)
{
    pimpl_->next_fn_.value(next_fn.value());
}


void SegmentInfo::next_fn(std::string const& next_fn)
{
    pimpl_->next_fn_.value(next_fn);
}


std::vector<std::vector<char> > SegmentInfo::family() const
{
    std::vector<std::vector<char> > result;
    BOOST_FOREACH(BinaryElement const& uid, pimpl_->families_)
    {
        result.push_back(uid.value());
    }
    return result;
}


void SegmentInfo::add_family(BinaryElement const& family)
{
    BinaryElement uid(ids::SegmentFamily, family.value());
    pimpl_->families_.insert(uid);
}


void SegmentInfo::add_family(std::vector<char> const& family)
{
    BinaryElement uid(ids::SegmentFamily, family);
    pimpl_->families_.insert(uid);
}


void SegmentInfo::remove_family(BinaryElement const& family)
{
    BinaryElement uid(ids::SegmentFamily, family.value());
    pimpl_->families_.erase(uid);
}


void SegmentInfo::remove_family(std::vector<char> const& family)
{
    BinaryElement uid(ids::SegmentFamily, family);
    pimpl_->families_.erase(uid);
}


UIntElement const& SegmentInfo::timecode_scale() const
{
    return pimpl_->tc_scale_;
}


void SegmentInfo::timecode_scale(UIntElement const& tc_scale)
{
    pimpl_->tc_scale_.value(tc_scale.value());
}


void SegmentInfo::timecode_scale(unsigned long long int const& tc_scale)
{
    pimpl_->tc_scale_.value(tc_scale);
}


FloatElement const& SegmentInfo::duration() const
{
    return pimpl_->duration_;
}


void SegmentInfo::duration(FloatElement const& duration)
{
    pimpl_->duration_.value(duration.value());
    pimpl_->have_duration_ = true;
}


void SegmentInfo::duration(double const& duration)
{
    pimpl_->duration_.value(duration);
    pimpl_->have_duration_ = true;
}


DateElement const& SegmentInfo::date() const
{
    return pimpl_->date_;
}


void SegmentInfo::date(DateElement const& date)
{
    pimpl_->date_.value(date.value());
    pimpl_->have_date_ = true;
}


void SegmentInfo::date(boost::posix_time::ptime const& date)
{
    pimpl_->date_.value(date);
    pimpl_->have_date_ = true;
}


StringElement const& SegmentInfo::title() const
{
    return pimpl_->title_;
}


void SegmentInfo::title(StringElement const& title)
{
    pimpl_->title_.value(title.value());
}


void SegmentInfo::title(std::string const& title)
{
    pimpl_->title_.value(title);
}


StringElement const& SegmentInfo::muxing_app() const
{
    return pimpl_->muxing_app_;
}


void SegmentInfo::muxing_app(StringElement const& muxing_app)
{
    pimpl_->muxing_app_.value(muxing_app.value());
}


void SegmentInfo::muxing_app(std::string const& muxing_app)
{
    pimpl_->muxing_app_.value(muxing_app);
}


StringElement const& SegmentInfo::writing_app() const
{
    return pimpl_->writing_app_;
}


void SegmentInfo::writing_app(StringElement const& writing_app)
{
    pimpl_->writing_app_.value(writing_app.value());
}


void SegmentInfo::writing_app(std::string const& writing_app)
{
    pimpl_->writing_app_.value(writing_app);
}


///////////////////////////////////////////////////////////////////////////////
// Element interface
///////////////////////////////////////////////////////////////////////////////

inline std::streamsize SegmentInfo::body_stored_size() const
{
    return pimpl_->body_size();
}


std::streamsize SegmentInfo::read_body(std::istream& i, std::streamsize size)
{
    std::streamsize read_bytes(0);

    // Start with a clean slate
    boost::scoped_ptr<Impl> new_pimpl(new Impl());

    bool no_tc_scale(true);

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
        BinaryElement uid(ids::SegmentFamily, std::vector<char>());
        switch(id)
        {
            case ids::SegmentUID:
                body_read += new_pimpl->uid_.read(body_ss);
                break;
            case ids::SegmentFileName:
                body_read += new_pimpl->filename_.read(body_ss);
                break;
            case ids::PrevUID:
                body_read += new_pimpl->prev_uid_.read(body_ss);
                break;
            case ids::PrevFileName:
                body_read += new_pimpl->prev_fn_.read(body_ss);
                break;
            case ids::NextUID:
                body_read += new_pimpl->next_uid_.read(body_ss);
                break;
            case ids::NextFileName:
                body_read += new_pimpl->next_fn_.read(body_ss);
                break;
            case ids::SegmentFamily:
                body_read += uid.read(body_ss);
                new_pimpl->families_.insert(uid);
                break;
            case ids::TimecodeScale:
                body_read += new_pimpl->tc_scale_.read(body_ss);
                no_tc_scale = false;
                break;
            case ids::Duration:
                body_read += new_pimpl->duration_.read(body_ss);
                new_pimpl->have_duration_ = true;
                if (new_pimpl->duration_.value() < 0)
                {
                    throw ValueOutOfRange() << err_id(ids::Duration) <<
                        // The cast here makes Apple's LLVM compiler happy
                        err_pos(static_cast<std::streamsize>(i.tellg()) -
                                id_res.second - (body.size() - body_read));
                }
                break;
            case ids::DateUTC:
                body_read += new_pimpl->date_.read(body_ss);
                new_pimpl->have_date_ = true;
                break;
            case ids::Title:
                body_read += new_pimpl->title_.read(body_ss);
                break;
            case ids::MuxingApp:
                body_read += new_pimpl->muxing_app_.read(body_ss);
                break;
            case ids::WritingApp:
                body_read += new_pimpl->writing_app_.read(body_ss);
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
    if (no_tc_scale)
    {
        // Not an error: use the default value, which is already set in the new
        // pimpl instance.
    }

    pimpl_.swap(new_pimpl);
    return read_bytes;
}


std::streamsize SegmentInfo::start_body(std::iostream& io) const
{
    std::streamsize result(0);
    // Write the body data to a stringstream first. This can be used to
    // calculate the CRC32 value if necessary.
    std::stringstream body_ss;
    // Only write children for which there is a valid value, except for the
    // time code scale, which always gets written.
    if (!pimpl_->uid_.empty())
    {
        write(pimpl_->uid_, body_ss);
    }
    if (!pimpl_->filename_.empty())
    {
        write(pimpl_->filename_, body_ss);
    }
    if (!pimpl_->prev_uid_.empty())
    {
        write(pimpl_->prev_uid_, body_ss);
    }
    if (!pimpl_->prev_fn_.empty())
    {
        write(pimpl_->prev_fn_, body_ss);
    }
    if (!pimpl_->next_uid_.empty())
    {
        write(pimpl_->next_uid_, body_ss);
    }
    if (!pimpl_->next_fn_.empty())
    {
        write(pimpl_->next_fn_, body_ss);
    }
    BOOST_FOREACH(BinaryElement const& f, pimpl_->families_)
    {
        write(f, body_ss);
    }
    write(pimpl_->tc_scale_, body_ss);
    if (pimpl_->have_duration_)
    {
        write(pimpl_->duration_, body_ss);
    }
    if (pimpl_->have_date_)
    {
        write(pimpl_->date_, body_ss);
    }
    if (!pimpl_->title_.empty())
    {
        write(pimpl_->title_, body_ss);
    }
    if (!pimpl_->muxing_app_.empty())
    {
        write(pimpl_->muxing_app_, body_ss);
    }
    if (!pimpl_->writing_app_.empty())
    {
        write(pimpl_->writing_app_, body_ss);
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


std::streamsize SegmentInfo::finish_body(std::iostream& io) const
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

void SegmentInfo::enable_crc_impl()
{
    pimpl_->master_impl_.enable_crc();
}


void SegmentInfo::disable_crc_impl()
{
    pimpl_->master_impl_.disable_crc();
}


bool SegmentInfo::crc_enabled_impl() const
{
    return pimpl_->master_impl_.crc_enabled();
}

