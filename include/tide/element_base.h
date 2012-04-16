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

#if !defined(ELEMENT_BASE_H_)
#define ELEMENT_BASE_H_

#include <tide/element.h>
#include <tide/exceptions.h>
#include <tide/ids.h>
#include <tide/id_utils.h>
#include <tide/vint.h>
#include <tide/win_dll.h>


/// \addtogroup interfaces Interfaces
/// @{

namespace tide
{
    /** \brief Standard implementations of the Element interface methods.
     *
     * This template interface provides the common implementations of the
     * Element interface methods used by most elements.
     *
     * As a template interface, this class places requirements on classes that
     * implement it. An implementing class should inherit from this class,
     * passing itself as the template parameter. The implementing class must
     * provide the following members:
     *
     * - \code
     *   tide::ids::ID id_;
     *   \endcode
     *   Storage for the element's ID. This must be initialised in the
     *   constructor and must not be alterable after construction.
     * - \code
     *   mutable std::streampos offset_;
     *   \endcode
     *   Storage for the element's offset in a byte stream. This must be
     *   initialised to std::numeric_limits<std::streampos>::max() and must not
     *   be alterable from outside the class. It must be mutable.
     * - \code
     *   std::streamsize body_stored_size() const;
     *   \endcode
     *   Provides the size of the element's body when stored in a byte stream.
     * - \code
     *   std::streamsize read_body(std::iostream& io, std::streamsize size);
     *   \endcode
     *   Reads the element's body from io. The expected size of the body is
     *   given as size. If the size actually read does not match this, it
     *   indicates a corrupt element, which should be reported by raising a
     *   BadBodySize() exception. If an error occurs, the value of the element
     *   stored in memory must be unchanged.
     * - \code
     *   std::streamsize start_body(std::iostream& io) const;
     *   \endcode
     *   Begins writing the element's body. When this method is called, the
     *   write_id() and write_size() methods will have been called. This method
     *   should be implemented in accordance with the requirements of the
     *   start_write() method of the Element interface.
     * - \code
     *   std::streamsize finish_body(std::iostream& io) const;
     *   \endcode
     *   Finishes writing the element's body. This method should be implemented
     *   in accordance with the requirements of the finish_write() method of
     *   the Element interface.
     *
     * Additionally, the implementing class may override the following virtual
     * members:
     *
     * - \code
     *   std::streamsize write_id(std::iostream& io) const;
     *   \endcode
     *   Writes the element's ID to the given byte stream. Returns the number
     *   of bytes written.
     * - \code
     *   std::streamsize write_stored_size(std::iostream& io) const;
     *   \endcode
     *   Writes the element's body's size to the given byte stream as a
     *   variable-width integer. Returns the number of bytes written.
     *
     * \tparam Derived The derived class implementing this template interface.
     */
    template<class Derived>
    class TIDE_EXPORT ElementBase : public Element
    {
        private:
            /** \brief Curiously Recurring Template Pattern: get a reference to
             * this in the derived class's type.
             */
            Derived& derived()
            {
                return *static_cast<Derived*>(this);
            }

            /** \brief Curiously Recurring Template Pattern: get a constant
             * reference to this in the derived class's type.
             */
            Derived const& derived() const
            {
                return *static_cast<Derived const*>(this);
            }

        protected:
            /** \brief Used by derived classes when they need to know the exact
             * base type.
             */
            typedef ElementBase<Derived> ElementBase_;

        public:
            /** \brief Constructor.
             *
             * \param[in] id The ID of the element. This value is validated but
             * not stored.
             * \exception InvalidEBMLID if the provided ID is invalid.
             */
            ElementBase(ids::ID id)
            {
                if (!ids::validate(id))
                {
                    throw InvalidElementID() << err_id(id);
                }
            }

            /// \brief Destructor.
            virtual ~ElementBase() {};

        private:
            ///////////////////////////////////////////////////////////////////
            // Template interface functions

            /** \brief Write the element's ID to a byte stream.
             *
             * The ID is written as a variable-width integer. Up to eight bytes
             * may be written.
             *
             * \param[in] io The destination byte stream to write to.
             * \return The number of bytes written.
             * \exception WriteError if an error occurs writing data.
             */
            virtual std::streamsize write_id(std::iostream& io) const
            {
                return ids::write(derived().id_, io);
            }

            /** \brief Write the element's body size to a byte stream.
             *
             * The size is written as a variable-width integer. Up to eight
             * bytes may be written.
             *
             * \param[in] io The destination byte stream to write to.
             * \return The number of bytes written.
             * \exception WriteError if an error occurs writing data.
             */
            virtual std::streamsize write_size(std::iostream& io) const
            {
                return vint::write(derived().body_stored_size(), io);
            }

            ///////////////////////////////////////////////////////////////////
            // Element interface virtual functions implementation

            /// \brief Get the element's ID.
            virtual ids::ID id_impl() const
            {
                return derived().id_;
            }

            /// \brief Get the element's offset.
            virtual std::streampos offset_impl() const
            {
                return derived().offset_;
            }

            /// \brief Get the stored size of the element.
            virtual std::streamsize stored_size_impl() const
            {
                std::streamsize b(derived().body_stored_size());
                return ids::size(derived().id_) + vint::size(b) + b;
            }

            /// \brief Read the element from a byte stream.
            virtual std::streamsize read_impl(std::iostream& io)
            {
                // Fill in the offset of this element in the byte stream.  The
                // input stream will be at the start of the size value, so:
                //
                //  offset = current position - size of ID
                derived().offset_ = io.tellg() - ids::size(derived().id_);
                // Get the element's body size
                vint::ReadResult result = vint::read(io);
                std::streamsize body_size(result.first);
                std::streamsize read_bytes(result.second);
                // The rest of the read is implemented in the derived class
                return read_bytes + derived().read_body(io, body_size);
            }

            /** \brief Begin writing the element to a byte stream.
             *
             * This implementation of start_write_impl() writes the element's
             * ID and size (as returned by stored_size_impl()) to the byte
             * stream, then calls the derived class's start_body() method to
             * handle the body. If the element being implemented relies on
             * child elements being written directly by the caller, then
             * start_body() may do nothing, or it may write some child elements
             * that are already available.
             */
            virtual std::streamsize start_write_impl(std::iostream& io) const
            {
                // Fill in the offset of this element in the byte stream.
                derived().offset_ = io.tellp();

                return write_id(io) + write_size(io) + derived().start_body(io);
            }

            /** \brief Finish writing the element to a byte stream.
             *
             * This implementation of finish_write_impl() merely calls through
             * to the derived class's implementation. If the element was
             * written by a two-step writing process, then that implementation
             * is responsible for filling in the correct size of the element.
             */
            virtual std::streamsize finish_write_impl(std::iostream& io) const
            {
                return derived().finish_body(io);
            }
    }; // class ElementBase
}; // namespace tide

/// @}
/// group interfaces

#endif // !defined(ELEMENT_BASE_H_)

