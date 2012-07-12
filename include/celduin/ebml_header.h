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

#if !defined(EBML_HEADER_H_)
#define EBML_HEADER_H_

#include <celduin/element_base.h>
#include <celduin/master_element.h>
#include <celduin/win_dll.h>

#include <boost/operators.hpp>
#include <boost/scoped_ptr.hpp>
#include <string>

/// \addtogroup interfaces Interfaces
/// @{

namespace celduin
{
    /** \brief The EBML header element.
     *
     * This is the Header element as defined in the EBML RFC draft. It is
     * \b required to be the first element in an EBML document.
     *
     * The header defines various meta-data about the EBML document to follow.
     */
    class CELDUIN_EXPORT EBMLHeader
        : public ElementBase<EBMLHeader>,
        public MasterElement,
        boost::equality_comparable<EBMLHeader>
    {
        friend class ElementBase<EBMLHeader>;

        public:
            /** \brief Constructor.
             *
             * \param[in] doc_type The DocType that will be created. When
             * reading, it will be populated from the file, overwriting the
             * stored value.
             */
            EBMLHeader(std::string const& doc_type="celduin");

            /// \brief Copy constructor.
            EBMLHeader(EBMLHeader const& rhs);

            /// \brief Assignment operator.
            EBMLHeader& operator=(EBMLHeader const& rhs);

            /// \brief Destructor.
            ~EBMLHeader();

            /** \brief Swap this element with another.
             *
             * \param[in] other The other element to swap with.
             */
            void swap(EBMLHeader& other);

            /// \brief Equality operator
            bool operator==(EBMLHeader const& rhs);

            /** \brief Get the EBML version.
             *
             * The EBML version is the version of the parser used to create the
             * file. It is set internally by the Celduin EBML parser, and read
             * from the file.
             */
            unsigned int version() const;
            /** \brief Get the EBML read version.
             *
             * This property gives the minimum EBML parser version necessary to
             * read the file. It is set internally by the Celduin EBML parser,
             * and read from the file.
             */
            unsigned int read_version() const;
            /** \brief Get the maximum ID length.
             *
             * This is the maximum length of IDs that can be used in a file
             * with this header.
             */
            unsigned int max_id_length() const;
            /// \brief Set the maximum ID length.
            void max_id_length(unsigned int max_id_length);
            /** \brief Get the maximum size length.
             *
             * This is the maximum number of bytes that can be used for an
             * element size in a file with this header.
             */
            unsigned int max_size_length() const;
            /// \brief Set the maximum size length.
            void max_size_length(unsigned int max_size_length);
            /** \brief Get the document type.
             *
             * The document type is the type of EBML document that is read or
             * written.
             */
            std::string doc_type() const;
            /// \brief Set the document type.
            void doc_type(std::string const& doc_type);
            /** \brief Get the document type version.
             *
             * This is the version of the document type contained in the file.
             */
            unsigned int doc_version() const;
            /// \brief Set the document type version.
            void doc_version(unsigned int doc_version);
            /** \brief Get the minimum document type version to read.
             *
             * This is the minimum document type version that must be readable
             * for a Celduin parser to read this file.
             */
            unsigned int doc_read_version() const;
            /// \brief Set the minimum required document type version.
            void doc_read_version(unsigned int doc_read_version);

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
    }; // class EBMLHeader

    /// \brief Swap EBML header elements
    void swap(EBMLHeader& a, EBMLHeader& b);


    /// \brief Stream output operator.
    std::ostream& operator<<(std::ostream& o, EBMLHeader const& rhs);
}; // namespace celduin

/// @}
// group interfaces

#endif // !defined(EBML_HEADER_H_)

