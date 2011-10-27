/* TIDE
 *
 * Exceptions header file.
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

#if !defined(TIDE_EXCEPTIONS_H_)
#define TIDE_EXCEPTIONS_H_

#include <tide/win_dll.h>

#include <boost/exception/all.hpp>
#include <exception>

/// \addtogroup exceptions Exceptions
/// @{

namespace tide
{
    /// \brief Base error type.
    struct TideError : virtual std::exception, virtual boost::exception {};

///////////////////////////////////////////////////////////////////////////////
// Error types
///////////////////////////////////////////////////////////////////////////////

    /** \brief No name error.
     *
     * Encountered when passing an empty string as the name for a new Tide
     * instance.
     */
    struct NoName : virtual TideError {};

    /** \brief Object does not exist error.
     *
     * Usually encountered when attempting to create a new Tide instance in
     * read mode based on a name that is not in use (in other words, the file
     * does not exist).
     *
     * Often, an err_name tag will be attached.
     */
    struct NoObject : virtual TideError {};

    /** \brief Object is not a Tide object.
     *
     * Usually encountered when trying to open an existing name (either for
     * reading or appending) that is not a Tide object (e.g. a file that is not
     * a Tide file).
     *
     * Often, an err_name tag will be attached.
     */
    struct NotTide : virtual TideError {};

    /** \brief An invalid variable-length integer was found.
     *
     * Encountered when reading a value stored as a variable-length integer,
     * such as a tag or an element size value, that is incorrectly-formatted.
     * This indicates that the file is corrupted.
     *
     * An err_pos tag will often be included indicating where the bad
     * variable-length integer was encountered.
     */
    struct InvalidVarInt : virtual TideError {};

    /** \brief A variable-length integer is too large to be encoded.
     *
     * Encountered when encoding an integer as a variable-length integer for
     * writing to a Tide object. The maximum allowable size of a
     * variable-length integer is given in the EBML specification.
     *
     * The err_varint tag will often be included, indicating the value that
     * was attempted to be encoded.
     *
     * The err_bufsize tag may be included to indicate the size of a buffer
     * that the variable-length integer was to be written to.
     */
    struct VarIntTooBig : virtual TideError {};

    /** \brief A buffer was too small for the data.
     *
     * Encountered in any situation where data will be written to a buffer. For
     * example, attempting to write a large variable-length integer to a buffer
     * too small to hold all the bytes will trigger this error.
     *
     * The err_bufsize tag may be included to indicate the size of the buffer.
     *
     * The err_reqsize tag may be included to indicate the required size.
     */
    struct BufferTooSmall : virtual TideError {};

    /** \brief A read error was encountered during a read.
     *
     * This error may occur anywhere that involves reading a file or file-like
     * stream. It most commonly indicates an end-of-file error, i.e. a
     * truncated file.
     *
     * The err_name tag may be included to indicate the name of the file.
     *
     * The err_pos tag may be included to indicate where in the file the error
     * occured.
     */
    struct ReadError : virtual TideError {};

    /** \brief A write error was encountered during a write.
     *
     * This error may occur anywhere that involves writing a file or file-like
     * stream. It most commonly indicates that there is no more space available
     * for writing to.
     *
     * The err_name tag may be included to indicate the name of the file.
     *
     * The err_pos tag may be included to indicate where in the file the error
     * occured.
     */
    struct WriteError : virtual TideError {};

    /** \brief An invalid Element ID was provided.
     *
     * When setting the ID of an Element, if the ID is one of the invalid
     * values, this error will occur.
     *
     * The err_id tag may be provided, indicating the invalid ID.
     */
    struct InvalidElementID : virtual TideError {};

///////////////////////////////////////////////////////////////////////////////
// Error information tags
///////////////////////////////////////////////////////////////////////////////

    /// \brief Name of the Tide object.
    typedef boost::error_info<struct tag_name, std::string> err_name;

    /// \brief Position in a Tide object, e.g. byte offset from the start.
    typedef boost::error_info<struct tag_pos, long long int> err_pos;

    /// \brief Value of a variable-length integer.
    typedef boost::error_info<struct tag_varint, long long int> err_varint;

    /// \brief The size of a buffer.
    typedef boost::error_info<struct tag_bufsize, size_t> err_bufsize;

    /// \brief The required size of a buffer.
    typedef boost::error_info<struct tag_reqsize, size_t> err_reqsize;

    /// \brief An Element ID.
    typedef boost::error_info<struct tag_id, size_t> err_id;
}; // namespace tide

/// @}
// group exceptions

#endif // TIDE_EXCEPTIONS_H_
