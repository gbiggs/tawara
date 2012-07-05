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

#if !defined(CELDUIN_CELDUIN_H_)
#define CELDUIN_CELDUIN_H_

#include <celduin/win_dll.h>

#include <boost/utility.hpp>
#include <iostream>
#include <stdint.h>

/// \addtogroup interfaces Interfaces
/// @{

namespace celduin
{
    /** \brief The Celduin interface, the interface provided by all Celduin objects.
     *
     * This class defines the Celduin interface. All implementations must implement
     * these methods.  A Celduin implementation may use whatever it desires to
     * provide the functionality. Two commonly-used implementations are a file
     * store and a memory store.
     */
    class CELDUIN_EXPORT Celduin : private boost::noncopyable
    {
        public:
            /** \brief Create a new Celduin object.
             *
             * This constructor makes a new Celduin object around an input/output
             * stream.
             *
             * The stream will be checked for its current contents. If it is
             * empty (i.e. the size of its contents is zero), then it will be
             * initialised as a new EBML file. Otherwise, it will be treated as
             * an existing EBML file and read. In this case, if an EBML header
             * cannot be found or the DocType is incorrect, NotCelduin will be
             * raised.
             *
             * EBML supports placing text before the EBML header. Anything up
             * to the first 0x1A byte is ignored. This means you can place data
             * into the stream before passing it to Celduin::Celduin(). Celduin will
             * treat the current position in the stream when this constructor
             * is called as the beginning of the stream.
             *
             * \param[in] stream A reference to the std::iostream object to
             * read from and write to.
             * \exception NotEBML if the stream is not empty and does not
             * contain an EBML header.
             * \exception NotCelduin if the stream is not empty and does not
             * contain valid EBML with the "celduin" DocType.
             */
            Celduin(std::iostream& stream) {}

            /** \brief Destroy the Celduin object.
             *
             * All resources used by the object will be cleaned up when its
             * destructor is called. Any other objects, such as Element
             * objects, referencing this Celduin object will become invalid and
             * should not be used. Behaviour if they are used is undefined.
             *
             * The stream passed to the constructor is \e not closed.
             */
            virtual ~Celduin() {};
    }; // class Celduin
}; // namespace celduin

/// @}
// group interfaces

#endif // CELDUIN_CELDUIN_H_

