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

#include <jonen/binary_element.h>
#include <jonen/exceptions.h>

using namespace jonen;

///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

BinaryElement::BinaryElement(ids::ID id, std::vector<char> const& value)
    : ElementBase<BinaryElement>(id), impl_(value), id_(id),
    offset_(0), writing_(false)
{
}


BinaryElement::BinaryElement(jonen::ids::ID id, std::vector<char> const& value,
        std::vector<char> const& default_val)
    : ElementBase<BinaryElement>(id), impl_(value, default_val), id_(id),
    offset_(0), writing_(false)
{}


///////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////

void BinaryElement::swap(BinaryElement& other)
{
    using std::swap;

    swap(impl_, other.impl_);
    swap(id_, other.id_);
    swap(offset_, other.offset_);
    swap(writing_, other.writing_);
}


void BinaryElement::swap(std::vector<char>& other)
{
    using std::swap;
    swap(impl_, other);
}


void jonen::swap(BinaryElement& a, BinaryElement& b)
{
    a.swap(b);
}


void jonen::swap(BinaryElement& a, std::vector<char>& b)
{
    a.swap(b);
}


/// \brief Stream output operator.
std::ostream& jonen::operator<<(std::ostream& o, BinaryElement const& rhs)
{
    return o << rhs.impl_;
}


///////////////////////////////////////////////////////////////////////////////
// Element interface
///////////////////////////////////////////////////////////////////////////////

inline std::streamsize BinaryElement::body_stored_size() const
{
    return impl_.body_stored_size();
}

std::streamsize BinaryElement::read_body(std::istream& i, std::streamsize size)
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

std::streamsize BinaryElement::start_body(std::iostream& io) const
{
    return impl_.start_body(io);
}

std::streamsize BinaryElement::finish_body(std::iostream& io) const
{
    impl_.finish_body(io);
    return this->stored_size();
}

