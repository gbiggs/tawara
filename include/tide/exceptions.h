/* Tide
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

#if !defined(TIDE_EXCEPTIONS_H_)
#define TIDE_EXCEPTIONS_H_

#include <tide/el_ids.h>
#include <tide/win_dll.h>

#include <boost/exception/all.hpp>
#include <exception>
#include <stdint.h>
#include <vector>

/// \addtogroup exceptions Exceptions
/// @{

namespace tide
{
    /// \brief Base error type.
    struct TideError : virtual std::exception, virtual boost::exception {};

///////////////////////////////////////////////////////////////////////////////
// Error types
///////////////////////////////////////////////////////////////////////////////

    /** \brief File is not an EBML file.
     *
     * Tide uses EBML for its file format. If an opened file is not an EBML
     * file, this error will occur.
     */
    struct NotEBML : virtual TideError {};

    /** \brief File is not a Tide file.
     *
     * Usually encountered when trying to open an existing name (either for
     * reading or appending) that is not a Tide object (e.g. a file that is not
     * a Tide file).
     */
    struct NotTide : virtual TideError {};

    /** \brief The required EBML read version is too high.
     *
     * The EBML header specifies the minimum EBML parser version necessary to
     * be able to read the EBML file. If it is too high, this error occurs.
     *
     * The version from the file may be attached as an err_ver tag.
     */
    struct BadReadVersion : virtual TideError {};

    /** \brief The required Tide read version is too high.
     *
     * The EBML header specifies the minimum Tide parser version necessary to
     * be able to read the Tide file. If it is too high, this error occurs.
     *
     * The version from the file may be attached as an err_ver tag.
     */
    struct BadDocReadVersion : virtual TideError {};

    /** \brief An invalid EBML class ID was found.
     *
     * EBML class IDs are encoded as variable-length integers. This means they
     * must occupy certain ranges within each set of bytes used. If a
     * variable-length integer outside one of the valid ranges is found while
     * reading or writing IDs, this error occurs.
     *
     * An err_varint tag may be included, giving the invalid ID.
     *
     * An err_pos tag may be included, indicating where the bad ID was
     * encountered.
     */
    struct InvalidEBMLID : virtual TideError {};

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

    /** \brief A specified size for a variable-length integer is too small.
     *
     * Encountered when encoding a variable-length integer into a fixed size.
     * Usually, this is a size that is larger than would normally be necessary,
     * but if the variable-length integer requires more bytes than the
     * specified fixed size, this error will occur.
     *
     * The err_varint tag may be included, indicating the value that was
     * attempted to be encoded.
     *
     * The err_reqsize tag may be included, indicating the size is required to
     * encode the integer.
     *
     * The err_specsize tag may be included, giving the size that was
     * requested.
     */
    struct SpecSizeTooSmall : virtual TideError {};

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
     *
     * The err_reqsize tag may be included to indicate the size of the read
     * that was attempted.
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

    /** \brief A fixed-length element is truncated or lengthened in the file.
     *
     * Some elements are stored with a fixed length in the file:
     * - Date elements are always 8 bytes.
     * - Float elements are 4 or 8 bytes, depending on necessary size.
     *
     * If one of these elements is found with an incorrect size, this error is
     * raised.
     *
     * The err_pos tag may be included to indicate where in the file the error
     * occured.
     *
     * The err_id tag may be provided to indicate the ID of the element that is
     * corrupted.
     *
     * The err_el_size tag may be provided, giving the size found.
     *
     * The err_valid_sizes tag may be provided, giving the valid sizes.
     */
    struct BadElementLength : virtual TideError {};

    /** \brief A read body size does not match the actual body size in the
     * file.
     *
     * The body size given at the start of a master element must match the
     * total size of all child elements. If it does not, this error occurs.
     */
    struct BadBodySize : virtual TideError {};

    /** \brief A child element was found where it doesn't belong.
     *
     * When reading an element's children, if an element ID is found that does
     * not belong in the parent, this error is raised.
     *
     * The err_id tag may be included to indicate the invalid ID.
     *
     * The err_par_id tag may be included to indicate the parent element being
     * read.
     *
     * The err_pos tag may be included to indicate where in the file the error
     * occured.
     */
    struct InvalidChildID : virtual TideError {};

    /** \brief A necessary child element was missing.
     *
     * Some child elements are required to be present, but don't have default
     * values. When a child element is not found, this error occurs.
     *
     * The err_id tag may be included to give the ID of the missing child
     * element.
     *
     * The err_par_id tag may be included to give the ID of the parent element.
     *
     * The err_pos tag may be included to give the approximate position in the
     * file where the error occured.
     */
    struct MissingChild : virtual TideError{};

    /** \brief A child element's value was set outside the allowable range.
     *
     * Often, child elements will have an allowable range of values, such as
     * not zero or positive integers. This error occurs when a child element's
     * value is set outside its allowable range. See the Tide format
     * specification for the allowable range of each element.
     *
     * The err_id tag may be included to give the ID of the bad child
     * element.
     *
     * The err_par_id tag may be included to give the ID of the parent element.
     */
    struct ValueOutOfRange : virtual TideError{};

    /** \brief A child element's size is below or above the required size.
     *
     * Some string or binary child elements have a required size, such as the
     * 8 bytes necessary for the UID elements in a SegmentInfo element. If a
     * child element's value is set to something with an incorrect size, this
     * error occurs.
     *
     * The err_id tag may be included to give the ID of the bad child
     * element.
     *
     * The err_par_id tag may be included to give the ID of the parent element.
     */
    struct ValueSizeOutOfRange : virtual TideError{};


///////////////////////////////////////////////////////////////////////////////
// Error information tags
///////////////////////////////////////////////////////////////////////////////

    /// \brief A version.
    typedef boost::error_info<struct tag_ver, std::streamsize> err_ver;

    /// \brief Position in a Tide file.
    typedef boost::error_info<struct tag_pos, std::streamsize> err_pos;

    /// \brief Value of a variable-length integer.
    typedef boost::error_info<struct tag_varint, uint64_t> err_varint;

    /// \brief The size of a buffer.
    typedef boost::error_info<struct tag_bufsize, size_t> err_bufsize;

    /// \brief The required size of a buffer or a file read.
    typedef boost::error_info<struct tag_reqsize, size_t> err_reqsize;

    /// \brief The specified size to encode a variable-length integer into.
    typedef boost::error_info<struct tag_specsize, size_t> err_specsize;

    /// \brief An Element ID.
    typedef boost::error_info<struct tag_id, ids::ID> err_id;

    /// \brief A parent element ID.
    typedef boost::error_info<struct tag_par_id, uint32_t> err_par_id;

    /// \brief A set of valid element sizes.
    typedef boost::error_info<struct tag_valid_sizes, std::vector<size_t> >
        err_valid_sizes;

    /// \brief The size of an element.
    typedef boost::error_info<struct tag_el_size, size_t> err_el_size;
}; // namespace tide

/// @}
// group exceptions

#endif // TIDE_EXCEPTIONS_H_

