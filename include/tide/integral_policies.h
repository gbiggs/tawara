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

#if !defined(INTEGRAL_READ_WRITE_POLICIES_H_)
#define INTEGRAL_READ_WRITE_POLICIES_H_

#include <tide/ebml_integer.h>
#include <tide/win_dll.h>

#include <ios>

/// \addtogroup policies Policies
/// @{

namespace tide
{
    namespace policies
    {
        /** \brief Policy to read and write signed integers.
         *
         * This policy defines how signed EBML integers are read and written.
         * It provides a size() method, a read() method and a write() method.
         */
        class TIDE_EXPORT RWSSigned
        {
            protected:
                std::streamsize size(long long int v) const
                {
                    return ebml_int::size_s(v);
                }

                long long int read(std::istream& i, std::streamsize n) const
                {
                    return ebml_int::read_s(i, n);
                }

                std::streamsize write(long long int value,
                        std::ostream& o) const
                {
                    return ebml_int::write_s(value, o);
                }
        }; // class ReadWriteSigned


        /** \brief Policy to read and write unsigned integers.
         *
         * This policy defines how unsigned EBML integers are read and written.
         * It provides a size() method, a read() method and a write() method.
         */
        class TIDE_EXPORT RWSUnsigned
        {
            protected:
                std::streamsize size(unsigned long long int v) const
                {
                    return ebml_int::size_u(v);
                }

                unsigned long long int read(std::istream& i,
                        std::streamsize n) const
                {
                    return ebml_int::read_u(i, n);
                }

                std::streamsize write(unsigned long long int value,
                        std::ostream& o) const
                {
                    return ebml_int::write_u(value, o);
                }
        }; // class ReadWriteUnsigned
    }; // namespace policies
}; // namespace tide

/// @}
// group policies

#endif // !defined(INTEGRAL_READ_WRITE_POLICIES_H_)

