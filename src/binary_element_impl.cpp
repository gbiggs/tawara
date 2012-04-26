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
#include <tide/binary_element_impl.h>

#include <iostream>

using namespace tide;
using namespace tide::impl;


///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

BinaryElementImpl::BinaryElementImpl(std::vector<char> const& value)
    : value_(value), has_default_(false), body_end_(0)
{
}


BinaryElementImpl::BinaryElementImpl(std::vector<char> const& value,
        std::vector<char> const& default_val)
    : value_(value), default_(default_val), has_default_(true), body_end_(0)
{
}


///////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////

void BinaryElementImpl::swap(BinaryElementImpl& other)
{
    using std::swap;

    swap(value_, other.value_);
    swap(default_, other.default_);
    swap(has_default_, other.has_default_);
    swap(body_end_, other.body_end_);
}


void BinaryElementImpl::swap(std::vector<char>& other)
{
    using std::swap;
    swap(value_, other);
}


void tide::impl::swap(BinaryElementImpl& a, BinaryElementImpl& b)
{
    a.swap(b);
}


void tide::impl::swap(BinaryElementImpl& a, std::vector<char>& b)
{
    a.swap(b);
}


std::ostream& tide::impl::operator<<(std::ostream& o,
        BinaryElementImpl const& rhs)
{
    for (unsigned int ii(0); ii < rhs.value_.size(); ++ii)
    {
        o << std::hex << std::setw(2) << std::setfill('0') <<
            +(rhs.value_[ii] & 0xFF);
    }
    return o;
}


///////////////////////////////////////////////////////////////////////////////
// Element interface
///////////////////////////////////////////////////////////////////////////////

std::streamsize BinaryElementImpl::body_stored_size() const
{
    return value_.size();
}

std::streamsize BinaryElementImpl::read_body(std::istream& i, std::streamsize size)
{
    std::vector<char> temp(size);
    i.read(&temp[0], size);
    if (!i)
    {
        throw ReadError() << err_pos(i.tellg());
    }
    temp.swap(value_);
    return value_.size();
}

std::streamsize BinaryElementImpl::start_body(std::ostream& o) const
{
    o.write(&value_[0], value_.size());
    if (!o)
    {
        throw WriteError() << err_pos(o.tellp());
    }
    // Record the position after this element's body for use in
    // finish_body().
    body_end_ = o.tellp();
    return value_.size();
}

std::streamsize BinaryElementImpl::finish_body(std::ostream& o) const
{
    // All writing was taken care of by start_body()
    // Ensure the post-condition that the write pointer is
    // placed after the element's body.
    o.seekp(body_end_);
    return body_stored_size();
}

