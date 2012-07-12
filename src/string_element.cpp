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

#include <celduin/string_element.h>
#include <celduin/exceptions.h>

using namespace celduin;

///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

StringElement::StringElement(ids::ID id, std::string const& value)
    : ElementBase<StringElement>(id), impl_(value), id_(id),
    offset_(0), writing_(false)
{
}


StringElement::StringElement(celduin::ids::ID id, std::string const& value,
        std::string const& default_val)
    : ElementBase<StringElement>(id), impl_(value, default_val), id_(id),
    offset_(0), writing_(false)
{}


///////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////

void StringElement::swap(StringElement& other)
{
    using std::swap;

    swap(impl_, other.impl_);
    swap(id_, other.id_);
    swap(offset_, other.offset_);
    swap(writing_, other.writing_);
}


void StringElement::swap(std::string& other)
{
    using std::swap;
    swap(impl_, other);
}


void celduin::swap(StringElement& a, StringElement& b)
{
    a.swap(b);
}


void celduin::swap(StringElement& a, std::string& b)
{
    a.swap(b);
}


StringElement celduin::operator+(StringElement lhs,
        StringElement const& rhs)
{
    lhs += rhs;
    return lhs;
}


StringElement celduin::operator+(StringElement lhs, std::string const& rhs)
{
    lhs += rhs;
    return lhs;
}


StringElement celduin::operator+(std::string const& lhs, StringElement rhs)
{
    rhs.insert(0, lhs);
    return rhs;
}


StringElement celduin::operator+(StringElement lhs, char const* const rhs)
{
    lhs += rhs;
    return lhs;
}


StringElement celduin::operator+(char const* const lhs, StringElement rhs)
{
    rhs.insert(0, lhs);
    return rhs;
}


StringElement celduin::operator+(StringElement lhs, char rhs)
{
    lhs += rhs;
    return lhs;
}


StringElement celduin::operator+(char lhs, StringElement rhs)
{
    rhs.insert(0, 1, lhs);
    return rhs;
}


std::ostream& celduin::operator<<(std::ostream& o, StringElement const& rhs)
{
    return o << rhs.impl_;
}


std::istream& celduin::operator>>(std::istream& i, StringElement& rhs)
{
    std::string temp;
    i >> temp;
    rhs.value(temp);
    return i;
}


///////////////////////////////////////////////////////////////////////////////
// Element interface
///////////////////////////////////////////////////////////////////////////////

inline std::streamsize StringElement::body_stored_size() const
{
    return impl_.body_stored_size();
}

std::streamsize StringElement::read_body(std::istream& i, std::streamsize size)
{
    try
    {
        return impl_.read_body(i, size);
    }
    catch (boost::exception& e)
    {
        // Add the ID and the offset of this element in the file
        // for easy debugging
        e << err_id(id_) << err_pos(offset_);
        throw;
    }
}

std::streamsize StringElement::start_body(std::iostream& io) const
{
    return impl_.start_body(io);
}

std::streamsize StringElement::finish_body(std::iostream& io) const
{
    impl_.finish_body(io);
    return this->stored_size();
}


