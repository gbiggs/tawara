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

#include <tide/exceptions.h>
#include <tide/tide_config.h>
#include <tide/date_element_impl.h>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <iostream>

using namespace tide;
using namespace tide::impl;
namespace bpt = boost::posix_time;
namespace bgr = boost::gregorian;
///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

DateElementImpl::DateElementImpl(bpt::ptime const& value)
    : value_(value), default_(bpt::not_a_date_time), has_default_(false),
    body_end_(0)
{
}


DateElementImpl::DateElementImpl(bpt::ptime const& value,
        bpt::ptime const& default_val)
    : value_(value), default_(default_val), has_default_(true), body_end_(0)
{}


///////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////

void DateElementImpl::swap(DateElementImpl& other)
{
    using std::swap;

    swap(value_, other.value_);
    swap(default_, other.default_);
    swap(has_default_, other.has_default_);
    swap(body_end_, other.body_end_);
}


void tide::impl::swap(DateElementImpl& a, DateElementImpl& b)
{
    a.swap(b);
}


///////////////////////////////////////////////////////////////////////////////
// Element interface
///////////////////////////////////////////////////////////////////////////////

inline std::streamsize DateElementImpl::body_stored_size() const
{
    // Dates are always stored as the full 8 bytes
    return 8;
}

std::streamsize DateElementImpl::read_body(std::istream& i, std::streamsize size)
{
    // This is not an assert because it is most likely to be caused by file
    // corruption.
    if (size != 8)
    {
        throw BadElementLength() <<
            err_valid_sizes(std::vector<std::streamsize>(1, 8)) <<
            err_el_size(size);
    }

    long long int temp;
    i.read(reinterpret_cast<char*>(&temp), 8);
    if (!i)
    {
        throw ReadError() << err_pos(i.tellg());
    }
    bpt::ptime basis(bgr::date(2001, 1, 1));
#if defined(TIDE_USE_NANOSECONDS)
    // time_durations don't like being built from a 64-bit value
    bpt::seconds secs(temp / 1000000000);
    bpt::nanoseconds nsecs(temp % 1000000000);
    bpt::time_duration d(secs + nsecs);
#else
    // time_durations don't like being built from a 64-bit value
    bpt::seconds secs(temp / 1000000000);
    bpt::microseconds usecs((temp / 1000) % 1000000);
    bpt::time_duration d(secs + usecs);
#endif // defined(TIDE_USE_NANOSECONDS)
    value_ = basis + d;

    return 8;
}

std::streamsize DateElementImpl::start_body(std::ostream& o) const
{
    bpt::ptime basis(bgr::date(2001, 1, 1));
    bpt::time_duration d(value_ - basis);
#if defined(TIDE_USE_NANOSECONDS)
    long long int temp = d.total_nanoseconds();
#else
    long long int temp = d.total_microseconds() * 1000;
#endif // defined(TIDE_USE_NANOSECONDS)
    o.write(reinterpret_cast<char*>(&temp), 8);
    if (!o)
    {
        throw WriteError() << err_pos(o.tellp());
    }
    // Record the position after this element's body for use in
    // finish_body().
    body_end_ = o.tellp();
    return 8;
}

std::streamsize DateElementImpl::finish_body(std::ostream& o) const
{
    // All writing was taken care of by start_body()
    // Ensure the post-condition that the write pointer is
    // placed after the element's body.
    o.seekp(body_end_);
    return body_stored_size();
}

