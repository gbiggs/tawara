/* Tide
 *
 * Header file for the EBML element object.
 *
 * Copyright 2011 Geoffrey Biggs geoffrey.biggs@aist.go.jp
 *     RT-Synthesis Research Group
 *     Intelligent Systems Research Institute,
 *     National Institute of Advanced Industrial Science and Technology (AIST),
 *     Japan
 *     All rights reserved.
 *
 * This file is part of Tide.
 *
 * Tide is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * Tide is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Tide. If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined(TIDE_EBML_ELEMENT_H_)
#define TIDE_EBML_ELEMENT_H_

#include <tide/el_ids.h>
#include <tide/master_element.h>
#include <tide/tide_config.h>
#include <tide/string_element.h>
#include <tide/uint_element.h>
#include <tide/win_dll.h>

#include <string>

/// \addtogroup interfaces Interfaces
/// @{

namespace tide
{
    /** \brief The EBML Header element.
     *
     * This is the Header element as defined in the EBML RFC draft. It is
     * \b required to be the first element in an EBML document.
     *
     * The header defines various meta-data about the EBML document to follow.
     */
    class TIDE_EXPORT EBMLElement : public MasterElement
    {
        public:
            /** \brief Create a new Element.
             *
             * \param[in] doc_type The DocType that will be created. When
             * reading, if it is blank, it will be populated. If it is not
             * blank, then the value in the file will be checked against the
             * value specified here and IncorrectDocType will be thrown if they
             * do not match.
             */
            EBMLElement(std::string const& doc_type="tide");

            /// \brief Destructor.
            virtual ~EBMLElement() {};

            /// \brief Get the size of the body of this element.
            virtual size_t size() const;

            /** \brief Get the EBML version.
             *
             * The EBML version is the version of the parser used to create the
             * file. It is set internally by the Tide EBML parser, and read
             * from the file.
             */
            unsigned int version() const { return ver_.value(); }
            /** \brief Get the EBML read version.
             *
             * This property gives the minimum EBML parser version necessary to
             * read the file. It is set internally by the Tide EBML parser, and
             * read from the file.
             */
            unsigned int read_version() const { return read_ver_.value(); }
            /** \brief Get the maximum ID length.
             *
             * This is the maximum length of IDs that can be used in a file
             * with this header.
             */
            unsigned int max_id_length() const
                { return max_id_length_.value(); }
            /// \brief Set the maximum ID length.
            void max_id_length(unsigned int max_id_length)
                { max_id_length_.value(max_id_length); }
            /** \brief Get the maximum size length.
             *
             * This is the maximum number of bytes that can be used for an
             * element size in a file with this header.
             */
            unsigned int max_size_length() const
                { return max_size_length_.value(); }
            /// \brief Set the maximum size length.
            void max_size_length(unsigned int max_size_length)
                { max_size_length_.value(max_size_length); }
            /** \brief Get the document type.
             *
             * The document type is the type of EBML document that is read or
             * written.
             */
            std::string doc_type() const { return doc_type_.value(); }
            /// \brief Set the document type.
            void doc_type(std::string doc_type) { doc_type_.value(doc_type); }
            /** \brief Get the document type version.
             *
             * This is the version of the document type contained in the file.
             */
            unsigned int doc_version() const { return doc_type_ver_.value(); }
            /// \brief Set the document type version.
            void doc_version(unsigned int doc_version)
                { doc_type_ver_.value(doc_version); }
            /** \brief Get the minimum document type version to read.
             *
             * This is the minimum document type version that must be readable
             * for a Tide parser to read this file.
             */
            unsigned int doc_read_version() const
                { return doc_type_read_ver_.value(); }
            /// \brief Set the minimum required document type version.
            void doc_read_version(unsigned int doc_read_version)
                { doc_type_read_ver_.value(doc_read_version); }

            /// \brief Element body writing.
            virtual std::streamsize write_body(std::ostream& output);

            /// \brief Element body loading.
            virtual std::streamsize read_body(std::istream& input);

        protected:
            /// EBML version
            UIntElement ver_;
            /// EBML minimum-version-to-read
            UIntElement read_ver_;
            /// Maximum ID length in bytes
            UIntElement max_id_length_;
            /// Maximum size length in bytes
            UIntElement max_size_length_;
            /// EBML document type
            StringElement doc_type_;
            /// Document type version
            UIntElement doc_type_ver_;
            /// Minimum document type version necessary to read
            UIntElement doc_type_read_ver_;

            /// \brief Sets all child elements to their default values.
            void set_defaults_();
    }; // class Element
}; // namespace tide

/// @}
/// group interfaces

#endif // TIDE_EBML_ELEMENT_H_

