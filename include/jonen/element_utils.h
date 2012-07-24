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

#if !defined(ELEMENT_UTILS_H_)
#define ELEMENT_UTILS_H_

#include <jonen/element.h>
#include <jonen/ids.h>
#include <jonen/win_dll.h>

/// \addtogroup element_funcs Element functions
/// @{

namespace jonen
{
    /** \brief Validate an EBML ID.
     *
     * Checks if an ID is in the allowable ranges as defined by the Jonen/EBML
     * specification, and throws an exception if it is not.
     *
     * \param[in] id The ID to check.
     * \exception InvalidElementID if the ID is not in the allowable range.
     */
    void validate_id(ids::ID id);


    /** \brief Skip an element in a byte stream.
     *
     * This function skips past an element, placing the read pointer at the ID
     * of the next element.
     *
     * \param[in] io The byte stream to seek the read pointer in.
     * \param[in] and_id If true, the read pointer is expected to be at the ID
     * of the element to skip when the function is called, and the ID will also
     * be skipped. If false, the read pointer is expected to be placed before
     * the element's size value.
     * \return The number of bytes skipped.
     */
    std::streamsize skip_read(std::iostream& io, bool and_id);


    /** \brief Skip an element in a byte/output stream.
     *
     * This function skips past an element, placing the write pointer after the
     * end of the element's body.
     *
     * \param[in] io The byte stream to seek the write pointer in.
     * \param[in] and_id If true, the write pointer is expected to be at the ID
     * of the element to skip when the function is called, and the ID will also
     * be skipped. If false, the write pointer is expected to be placed before
     * the element's size value.
     * \return The number of bytes skipped.
     */
    std::streamsize skip_write(std::iostream& io, bool and_id);
}; // namespace jonen

/// @}
/// group element_funcs

#endif // !defined(ELEMENT_UTILS_H_)

