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

#if !defined(ELEMENT_H_)
#define ELEMENT_H_

#include <jonen/ids.h>
#include <jonen/win_dll.h>


/// \addtogroup interfaces Interfaces
/// @{

namespace jonen
{
    /** \brief The Element interface, a basic interface to an element object.
     *
     * Jonen documents store their data in elements. Like in XML, the elements
     * form a tree of information. Each element contains a single value \e or
     * zero or more sub-elements. Each element type has a unique ID within the
     * format.
     *
     * This interface defines the basic element facilities. It provides the
     * element's ID and an abstract interface to read and write elements to a
     * byte stream.
     */
    class JONEN_EXPORT Element
    {
        public:
            /// \brief Destructor.
            virtual ~Element() {};

            /** Get the element's ID.
             *
             * The element's ID is an unsigned integer with a maximum size of
             * 28 bits. Some IDs are invalid:
             * - 0
             * - 0xFF
             * - 0xFFFF
             * - 0xFFFFFF
             * - 0xFFFFFFFF
             * - 0x100000000 or greater
             *
             * IDs are divided into four classes:
             * - Class A (0 to 2^7 - 1)
             * - Class B (2^7 to 2^14 - 1)
             * - Class C (2^14 to 2^21 - 1)
             * - Class D (2^21 to 2^28 - 1)
             *
             * Typically in a Jonen document, the higher classes use more
             * storage space and so are more likely to be unique within the
             * document. This allows them to be used for synchronisation in the
             * event of file corruption. To take advantage of this, you should
             * use higher IDs for elements that occur less frequently, such as
             * the top-level elements.
             */
            uint32_t id() const { return id_impl(); }

            /** Get the element's offset in the byte stream.
             *
             * If the element has been written, or was read from a byte stream,
             * this value will contain its position in that stream. It is
             * updated every time the element is read or written, so reading
             * from one offset and then writing to another will change the
             * stored offset.
             *
             * If the offset is std::numeric_limits<std::streampos>::max(),
             * then the element has not yet been read or written.
             */
            std::streampos offset() const { return offset_impl(); }

            /** \brief Get the stored size of the element in bytes.
             *
             * Returns the size required to store this entire element,
             * including its ID, data size value and body.
             *
             * \return The stored size of the entire element, in bytes.
             */
            std::streamsize stored_size() const { return stored_size_impl(); }

            /** \brief Element reading.
             *
             * Reads the element from a byte stream providing a std::iostream
             * interface.
             *
             * This method assumes that the Element ID has already been read
             * (and thus used to construct the Element instance doing the
             * reading), which means that the file's read pointer should be
             * positioned at the first byte of the element's size.
             *
             * \post If the read fails, the element will be in the state it was
             * in before reading began.
             * \invariant If reading fails, the contents of the element stored
             * in memory will remain unchanged.
             *
             * \param[in] i The source byte stream to read from.
             * \return The number of bytes read.
             * \exception ReadError if an error occurs reading data.
             * \exception BadBodySize if the size read from the element's
             * header doesn't match its actual size. Only occurs with master
             * elements.
             * \exception InvalidChildID if a child element is found in the
             * body of a master element to which it doesn't belong.
             * \exception MissingChild if a child element that must be present
             * in a master element is not found.
             * \exception ValueOutOfRange if a child element is read with a
             * value that is out of range.
             * \exception ValueSizeOutOfRange if a child element is read with a
             * size that is not in the allowable range of sizes.
             */
            std::streamsize read(std::istream& i) { return read_impl(i); }

            /** \brief Open the element for writing.
             *
             * Elements are written using a two-step process that supports both
             * all-at-once writing and streamed writing. This process requires
             * that an element is first opened for writing, then closed once
             * writing of the streamed child elements (if any) is complete.
             *
             * In the Jonen format, very few element types actually use
             * stream-based writing. The most notable is the Cluster element.
             *
             * The sequence of method calls that must be made to completely
             * write any element is:
             *
             * \verbatim
             *  element.start_write(io)
             *          ||
             *          \/
             *  [Write child elements, if necessary]
             *          ||
             *          \/
             *  element.finish_write(io)
             * \endverbatim
             *
             * For most elements, the middle stage is not necessary, as their
             * bodies are written completely by the start_write() and
             * finish_write() methods.
             *
             * The purpose of the start_write() step is to prepare the output
             * for the element's body to be written. This includes such things
             * as writing the element's ID and a place-holder size value, and
             * any child elements that must appear before the streamed child
             * elements.
             *
             * The purpose of the finish_write() step is to finalise the
             * element in the storage, mainly by updating the place-holder size
             * value to the real size of the element.
             *
             * When start_write() is called, the element will be placed at the
             * current write offset in the byte stream. The caller should
             * ensure that the write pointer is in the correct position in the
             * stream (typically, at the end of the stream) before calling
             * start_write(). start_write() will ensure that the write pointer
             * is in the correct position to write any child elements when it
             * completes.
             *
             * When finish_write() is called, the write offset in the byte
             * stream \e must be placed at the first byte \e after the end of
             * the written element's body. After finish_write() completes, the
             * write pointer will remain in this position.
             *
             * \pre The write pointer of io must be at the place where the
             * element is to be written.
             * \post The write pointer will be after the end of the element's
             * body, or at the correct position to write child elements.
             *
             * \param[in] io The destination byte stream to write to.
             * \return The number of bytes written.
             * \exception WriteError if an error occurs writing data.
             */
            std::streamsize start_write(std::iostream& io) const
                { return start_write_impl(io); }

            /** \brief Finish writing the element.
             *
             * See start_write() for a description of the element writing
             * process.
             *
             * \pre The write pointer of io must be at the first byte after the
             * end of the element's body.
             * \post The write pointer will be at the first byte after the end
             * of the element's body.
             *
             * \param[in] io The destination byte stream to write to.
             * \return The final size of the element.
             * \exception WriteError if an error occurs writing data.
             */
            std::streamsize finish_write(std::iostream& io) const
                { return finish_write_impl(io); }

        private:
            /** \brief Implementation of the id() method.
             *
             * Implementing classes must implement this to return the ID of the
             * element.
             */
            virtual uint32_t id_impl() const = 0;

            /** \brief Implementation of the offset() method.
             *
             * Implementing classes must implement this to return the offset of
             * the element in the byte stream it was read from or written to,
             * or std::numeric_limits<std::streampos>::max() if the element was
             * not read or written. The value is measured in bytes.
             */
            virtual std::streampos offset_impl() const = 0;

            /** \brief Implementation of the stored_size() method.
             *
             * Implementing classes must implement this to return the total
             * size, in bytes, required by the element when written to a byte
             * stream.
             */
            virtual std::streamsize stored_size_impl() const = 0;

            /** \brief Implementation of the read() method.
             *
             * Implementing classes must implement this to read the element
             * from the given byte stream. What will be read (primitive element
             * value, child elements, etc.) will depend on the specific element
             * type being implemented.
             *
             * If an error occurs during element reading, the element's value
             * must remain unchanged.
             */
            virtual std::streamsize read_impl(std::istream& i) = 0;

            /** \brief Implementation of the start_write() method.
             *
             * Implementing classes must implement this suitably to the element
             * type being implemented.
             *
             * If the element can be written in a single pass, the correct
             * approach is to write the element completely during
             * start_write_impl() and do nothing in finish_write_impl().
             *
             * If the element must be written with the multi-step method, with
             * child elements added to the byte stream directly by the caller,
             * then start_write_impl() must prepare the byte stream to receive
             * the child elements. It must write this element's ID and a
             * place-holder size value that can be filled in later by
             * finish_write_impl(). It should store the start of the element in
             * the byte stream for use by finish_write_impl() later. It may
             * optionally write any child elements that can be written
             * immediately. It must leave the byte stream's write pointer in
             * the correct position to begin writing child elements.
             *
             * The pre- and post-conditions of start_write() and finish_write()
             * are strict. Additionally, it is important that both of them
             * manage the write pointer of io carefully to ensure that data is
             * put in the correct place, as other write operations may take
             * place between the two calls.
             */
            virtual std::streamsize start_write_impl(std::iostream& io) const = 0;

            /** \brief Implementation of the finish_write() method.
             *
             * Implementing classes must implement this suitably to the element
             * type being implemented.
             *
             * If the element can be written in a single pass, the correct
             * approach is to write the element completely during
             * start_write_impl() and do nothing in finish_write_impl().
             *
             * If the element must be written with the multi-step method, with
             * child elements added to the byte stream directly by the caller,
             * then finish_write_impl() must finalise the element. The caller
             * is required to place the byte stream's write pointer at the
             * first byte after the element's body before calling
             * finish_write_impl(), so finish_write_impl() can use this and the
             * start position of the element stored by start_write_impl() to
             * calculate the final size of the element. This value should be
             * written over the place-holder value written by
             * start_write_impl(). When it completes, finish_write_impl() must
             * leave the byte stream's write pointer at the same position it
             * was when finish_write_impl() was called.
             */
            virtual std::streamsize finish_write_impl(std::iostream& io) const = 0;
    }; // class Element

    /** \brief Element reading.
     *
     * Reads the element from a byte stream providing a std::iostream
     * interface.
     *
     * This method assumes that the Element ID has already been read
     * (and thus used to construct the Element instance doing the
     * reading), which means that the file's read pointer should be
     * positioned at the first byte of the element's size.
     *
     * \post If the read fails, the element will be in the state it was
     * in before reading began.
     * \invariant If reading fails, the contents of the element stored
     * in memory will remain unchanged.
     *
     * \param[in] e The element instance to read into.
     * \param[in] i The source byte stream to read from.
     * \return The number of bytes read.
     * \exception ReadError if an error occurs reading data.
     * \exception BadBodySize if the size read from the element's
     * header doesn't match its actual size. Only occurs with master
     * elements.
     * \exception InvalidChildID if a child element is found in the
     * body of a master element to which it doesn't belong.
     * \exception MissingChild if a child element that must be present
     * in a master element is not found.
     * \exception ValueOutOfRange if a child element is read with a
     * value that is out of range.
     * \exception ValueSizeOutOfRange if a child element is read with a
     * size that is not in the allowable range of sizes.
     */
    std::streamsize read(Element& e, std::istream& i);


    /** \brief Write an element.
     *
     * \relates Element
     *
     * The element writing process is typically performed in two steps,
     * allowing time in between for writing additional child elements as
     * appropriate to the type of element being written.
     *
     * However, the majority of elements in the Jonen format do not need
     * additional child elements to be written, and so can be written in a
     * single pass. This function can be used with those elements to perform a
     * complete write.
     *
     * \pre The write pointer of io must be at the place where the
     * element is to be written.
     * \post The write pointer will be at the first byte after the end
     * of the element's body.
     *
     * \param[in] e The element to write.
     * \param[in] io The destination byte stream to write to.
     * \return The number of bytes written (i.e. the size of the element).
     * \exception WriteError if an error occurs writing data.
     */
    std::streamsize write(Element const& e, std::iostream& io);
}; // namespace jonen

/// @}
/// group interfaces

#endif // !defined(ELEMENT_H_)

