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

#if !defined(ATTACHED_FILE_H_)
#define ATTACHED_FILE_H_

#include <jonen/binary_element.h>
#include <jonen/element_base.h>
#include <jonen/integer_elements.h>
#include <jonen/master_element.h>
#include <jonen/string_element.h>
#include <jonen/win_dll.h>

#include <boost/operators.hpp>
#include <boost/scoped_ptr.hpp>
#include <string>
#include <vector>

/// \addtogroup elements Elements
/// @{

namespace jonen
{
    /** \brief An attached file.
     *
     * This element contains a file attached to a segment. Attached files have
     * a file name and a MIME type to identify the type of data stored.
     *
     * The AttachedFile element uses a CRC32 value by default.
     *
     * This class implements the Element interface.
     *
     * This class implements the MasterElement interface.
     */
    class JONEN_EXPORT AttachedFile
        : public ElementBase<AttachedFile>,
        public MasterElement,
        boost::equality_comparable<AttachedFile>
    {
        friend class ElementBase<AttachedFile>;

        public:
            /** \brief Constructor.
             *
             * \param[in] name The file name.
             * \param[in] mime_type The file's MIME type.
             * \param[in] data The file's data.
             * \param[in] uid A unique identifier for the file. Must not be
             * zero.
             * \param[in] desc A human-readable description of the file.
             *
             * \exception ValueOutOfRange if the name or mime_type are empty or
             * the uid value is zero.
             */
            AttachedFile(std::string const& name, std::string const& mime_type,
                    std::vector<char> const& data, unsigned long long int uid,
                    std::string const& desc="");

            /// \brief Copy constructor.
            AttachedFile(AttachedFile const& rhs);

            /// \brief Assignment operator.
            AttachedFile& operator=(AttachedFile const& rhs);

            /// \brief Destructor.
            ~AttachedFile();

            /** \brief Swap this element with another.
             *
             * \param[in] other The other element to swap with.
             */
            void swap(AttachedFile& other);

            /// \brief Equality operator
            bool operator==(AttachedFile const& rhs);

            /// \brief Get the human-friendly name for the file.
            StringElement const& description() const;
            /// \brief Set the human-friendly name for the file.
            void description(StringElement const& description);
            /// \brief Set the human-friendly name for the file.
            void description(std::string const& description);

            /// \brief Get the name of the file.
            StringElement const& name() const;
            /** \brief Set the name of the file.
             *
             * \exception ValueOutOfRange if the provided name is empty.
             * \post The stored value will be unchanged in the event of an
             * error.
             */
            void name(StringElement const& name);
            /** \brief Set the name of the file.
             *
             * \exception ValueOutOfRange if the provided name is empty.
             * \post The stored value will be unchanged in the event of an
             * error.
             */
            void name(std::string const& name);

            /// \brief Get the MIME type of the file.
            StringElement const& mime_type() const;
            /** \brief Set the MIME type of the file.
             *
             * \exception ValueOutOfRange if the provided MIME type is empty.
             * \post The stored value will be unchanged in the event of an
             * error.
             */
            void mime_type(StringElement const& mime_type) const;
            /** \brief Set the MIME type of the file.
             *
             * \exception ValueOutOfRange if the provided MIME type is empty.
             * \post The stored value will be unchanged in the event of an
             * error.
             */
            void mime_type(std::string const& mime_type) const;

            /// \brief Get the file data.
            BinaryElement const& data() const;
            /// \brief Set the file data.
            void data(BinaryElement const& data);
            /// \brief Set the file data.
            void data(std::vector<char> const& data);

            /// \brief Get the file's unique ID.
            UIntElement const& uid() const;
            /** \brief Set the file's unique ID.
             *
             * \exception ValueOutOfRange if the provided unique ID is zero.
             * \post The stored value will be unchanged in the event of an
             * error.
             */
            void uid(UIntElement const& uid);
            /** \brief Set the file's unique ID.
             *
             * \exception ValueOutOfRange if the provided unique ID is zero.
             * \post The stored value will be unchanged in the event of an
             * error.
             */
            void uid(unsigned long long int const& uid);

        private:
            // Pimpl member
            class Impl;
            boost::scoped_ptr<Impl> pimpl_;
            // Must be mutable to be updated in start_body()
            mutable std::streampos body_end_;

            ///////////////////////////////////////////////////////////////////
            // ElementBase CRTP required members
            ids::ID id_;
            mutable std::streampos offset_;
            mutable bool writing_;

            std::streamsize body_stored_size() const;

            std::streamsize read_body(std::istream& i, std::streamsize size);

            std::streamsize start_body(std::iostream& io) const;

            std::streamsize finish_body(std::iostream& io) const;

            ///////////////////////////////////////////////////////////////////
            // MasterElement interface implementation
            void enable_crc_impl();

            void disable_crc_impl();

            bool crc_enabled_impl() const;
    }; // class SegmentInfo


    /// \brief Swap elements
    void swap(AttachedFile& a, AttachedFile& b);


    /// \brief Stream output operator.
    std::ostream& operator<<(std::ostream& o, AttachedFile const& rhs);
}; // namespace jonen

/// @}
// group elements

#endif // !defined(ATTACHED_FILE_H_)

