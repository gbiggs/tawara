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

#include <jonen/jonen_config.h>
#include <jonen/exceptions.h>
#include <jonen/float_element_impl.h>

#include <iostream>
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
    #include <utility> // For std::swap
#else // defined(JONEN_CPLUSPLUS11_SUPPORT)
    #include <algorithm> // For std::swap
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)

using namespace jonen;
using namespace jonen::impl;


///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

FloatElementImpl::FloatElementImpl(double value, EBMLFloatPrecision precision)
    : value_(value), default_(0), has_default_(false), precision_(precision),
    body_end_(0)
{
}


FloatElementImpl::FloatElementImpl(double value, double default_val,
        EBMLFloatPrecision precision)
    : value_(value), default_(default_val), has_default_(true),
    precision_(precision), body_end_(0)
{
}


///////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////

void FloatElementImpl::swap(FloatElementImpl& other)
{
    using std::swap;

    swap(value_, other.value_);
    swap(default_, other.default_);
    swap(has_default_, other.has_default_);
    swap(body_end_, other.body_end_);
}


void jonen::impl::swap(FloatElementImpl& a, FloatElementImpl& b)
{
    a.swap(b);
}


void FloatElementImpl::swap(double& other)
{
    using std::swap;
    swap(value_, other);
}


void jonen::impl::swap(FloatElementImpl& a, double& b)
{
    a.swap(b);
}


///////////////////////////////////////////////////////////////////////////////
// Element interface
///////////////////////////////////////////////////////////////////////////////

std::streamsize FloatElementImpl::body_stored_size() const
{
    switch(precision_)
    {
        case EBML_FLOAT_PREC_SINGLE:
            return 4;
        case EBML_FLOAT_PREC_DOUBLE:
            return 8;
    };
}

std::streamsize FloatElementImpl::read_body(std::istream& i, std::streamsize size)
{
    if (size == 4)
    {
        float temp(0);
        i.read(reinterpret_cast<char*>(&temp), 4);
        if (!i)
        {
            throw ReadError() << err_pos(i.tellg());
        }
        value_ = temp;
        precision_ = EBML_FLOAT_PREC_SINGLE;
        return 4;
    }
    else if (size == 8)
    {
        double temp(0);
        i.read(reinterpret_cast<char*>(&temp), 8);
        if (!i)
        {
            throw ReadError() << err_pos(i.tellg());
        }
        std::swap(value_, temp);
        precision_ = EBML_FLOAT_PREC_DOUBLE;
        return 8;
    }
    else
    {
        std::vector<std::streamsize> valid_sizes;
        valid_sizes.push_back(4);
        valid_sizes.push_back(8);
        throw BadElementLength() <<
            err_valid_sizes(valid_sizes) << err_el_size(size);
    }
}

std::streamsize FloatElementImpl::start_body(std::ostream& o) const
{
    std::streamsize result(0);
    float temp(0);
    switch(precision_)
    {
        case EBML_FLOAT_PREC_SINGLE:
            temp = value_;
            o.write(reinterpret_cast<char*>(&temp), 4);
            if (!o)
            {
                throw WriteError() << err_pos(o.tellp());
            }
            result = 4;
            break;
        case EBML_FLOAT_PREC_DOUBLE:
            o.write(reinterpret_cast<char const*>(&value_), 8);
            if (!o)
            {
                throw WriteError() << err_pos(o.tellp());
            }
            result = 8;
            break;
    };
    // Record the position after this element's body for use in
    // finish_body().
    body_end_ = o.tellp();
    return result;
}

std::streamsize FloatElementImpl::finish_body(std::ostream& o) const
{
    // All writing was taken care of by start_body()
    // Ensure the post-condition that the write pointer is
    // placed after the element's body.
    o.seekp(body_end_);
    return body_stored_size();
}

