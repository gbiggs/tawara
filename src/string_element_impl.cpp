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
#include <tide/string_element_impl.h>

#include <iostream>

using namespace tide;
using namespace tide::impl;


///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

StringElementImpl::StringElementImpl(std::string const& value)
    : value_(value), has_default_(false), padding_(0), body_end_(0)
{
}


StringElementImpl::StringElementImpl(std::string const& value,
        std::string const& default_val)
    : value_(value), default_(default_val), has_default_(true), padding_(0),
    body_end_(0)
{
}


///////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////

void StringElementImpl::swap(StringElementImpl& other)
{
    using std::swap;

    swap(value_, other.value_);
    swap(default_, other.default_);
    swap(has_default_, other.has_default_);
    swap(body_end_, other.body_end_);
}


void StringElementImpl::swap(std::string& other)
{
    using std::swap;
    swap(value_, other);
}


void tide::impl::swap(StringElementImpl& a, StringElementImpl& b)
{
    a.swap(b);
}


void tide::impl::swap(StringElementImpl& a, std::string& b)
{
    a.swap(b);
}


std::ostream& tide::impl::operator<<(std::ostream& o,
        StringElementImpl const& rhs)
{
    return o << rhs.value();
}


std::istream& tide::impl::operator>>(std::istream& i, StringElementImpl& rhs)
{
    std::string temp;
    i >> temp;
    rhs.value(temp);
    return i;
}


///////////////////////////////////////////////////////////////////////////////
// Element interface
///////////////////////////////////////////////////////////////////////////////

std::streamsize StringElementImpl::body_stored_size() const
{
    return value_.size() + padding_;
}

std::streamsize StringElementImpl::read_body(std::istream& i,
        std::streamsize size)
{
    std::vector<char> temp(size);
    i.read(&temp[0], size);
    if (!i)
    {
        throw ReadError() << err_pos(i.tellg()) << err_reqsize(size);
    }
    std::string temp2(temp.begin(), temp.end());
    temp2 = temp2.substr(0, temp2.find('\0'));
    temp2.swap(value_);
    padding_ = size - value_.size();
    return value_.size() + padding_;
}

std::streamsize StringElementImpl::start_body(std::ostream& o) const
{
    o.write(value_.c_str(), value_.size());
    std::streamsize result = value_.size();
    if (!o)
    {
        throw WriteError() << err_pos(o.tellp());
    }
    for (unsigned int ii(0); ii < padding_; ++ii)
    {
        o.put(0x00);
        ++result;
    }
    // Record the position after this element's body for use in
    // finish_body().
    body_end_ = o.tellp();
    return result;
}

std::streamsize StringElementImpl::finish_body(std::ostream& o) const
{
    // All writing was taken care of by start_body()
    // Ensure the post-condition that the write pointer is
    // placed after the element's body.
    o.seekp(body_end_);
    return body_stored_size();
}


