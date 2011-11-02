/* TIDE
 *
 * Header file for the float element object.
 *
 * Copyright 2011 Geoffrey Biggs geoffrey.biggs@aist.go.jp
 *     RT-Synthesis Research Group
 *     Intelligent Systems Research Institute,
 *     National Institute of Advanced Industrial Science and Technology (AIST),
 *     Japan
 *     All rights reserved.
 *
 * This file is part of TIDE.
 *
 * TIDE is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * TIDE is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with TIDE. If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined(TIDE_FLOAT_ELEMENT_H_)
#define TIDE_FLOAT_ELEMENT_H_

#include <tide/prim_element.h>
#include <tide/win_dll.h>

/// \addtogroup implementations Implementations
/// @{

namespace tide
{
    /// \brief Precision of float elements.
    enum EBMLFloatPrec
    {
        /// Single precision
        EBML_FLOAT_PREC_SINGLE,
        /// Double precision
        EBML_FLOAT_PREC_DOUBLE
    };

    /** Float primitive element.
     *
     * This element stores an IEEE floating-point number. 4-byte and 8-byte
     * floats are allowed.
     */
    class TIDE_EXPORT FloatElement : public PrimitiveElement<double>
    {
        public:
            /** \brief Create a new float element with no default.
             *
             * \param[in] id The element's ID, as an unsigned integer up to 28
             * bits.
             * \param[in] value The element's value.
             * \param[in] precision The element's precision, single or double.
             * This only has an effect when writing the float to file.
             */
            FloatElement(uint32_t id, double value,
                    EBMLFloatPrec precision=EBML_FLOAT_PREC_DOUBLE);

            /** \brief Create a new float element with a default value.
             *
             * \param[in] id The element's ID, as an unsigned integer up to 28
             * bits.
             * \param[in] value The element's value.
             * \param[in] precision The element's precision, single or double.
             * This only has an effect when writing the float to a file.
             * \param[in] default_value The default value of the element.
             */
            FloatElement(uint32_t id, double value, double default_value,
                    EBMLFloatPrec precision=EBML_FLOAT_PREC_DOUBLE);

            /// \brief Value assignment operator.
            virtual FloatElement& operator=(double const& rhs);

            /// \brief Get the precision setting.
            virtual EBMLFloatPrec precision() const { return prec_; }
            /** \brief Set the precision setting
             *
             * This value determines if the float is single or double
             * precision. The precision value has no effect until the float is
             * written to a file, at which point single-precision floats are
             * written using 4 bytes while double-precision floats are written
             * using 8 bytes.
             */
            virtual void precision(EBMLFloatPrec precision)
            { prec_ = precision; }

            /** \brief Element ID writing.
             *
             * Writes the element's EBML ID to a byte stream providing a
             * std::basic_ostream<uint8_t> interface. Up to 4 bytes may be
             * written.
             *
             * \param[in] output The destination byte stream to write to.
             * \return The number of bytes written.
             * \exception WriteError if an error occurs writing data.
             */
            virtual std::streamsize write_id(
                    std::basic_ostream<uint8_t>& output);

            /** \brief Element body writing.
             *
             * Writes the element's size and body to a byte stream providing a
             * std::basic_ostream<uint8_t> interface.
             *
             * \return The number of bytes written.
             * \exception WriteError if an error occurs writing data.
             */
            virtual std::streamsize write_body(
                    std::basic_ostream<uint8_t>& output);

            /** \brief Element body loading.
             *
             * Reads the element from a byte stream providing a
             * std::basic_ostream<uint8_t> interface.
             *
             * This method assumes that the Element ID has already been read
             * (and thus used to construct the Element instance doing the
             * reading), which means that the file's read pointer should be
             * positioned at the first byte of the element's size.
             *
             * \return The number of bytes read.
             * \exception BadElementLength if the float element is an incorrect
             * length (i.e. not 4 or 8 bytes).
             * \exception ReadError if an error occurs reading data.
             */
            virtual std::streamsize read_body(
                    std::basic_istream<uint8_t>& input);

            /** \brief Get the size of the body of this element.
             *
             * Returns the size, in bytes, required to store this element's
             * body. This does not include the space required by the ID or the
             * data size value.
             *
             * See also total_size().
             *
             * \return The size of the element's body, in bytes.
             */
            virtual size_t size() const;

            /** \brief Get the total size of the element.
             *
             * Returns the size, in bytes, required to store this entire
             * element, including its ID, data size value and body.
             *
             * \return The size of the entire element, in bytes.
             */
            virtual size_t total_size() const;

        protected:
            EBMLFloatPrec prec_;
    }; // class FloatElement
}; // namespace tide

/// @}
// group implementations

#endif // TIDE_FLOAT_ELEMENT_H_

