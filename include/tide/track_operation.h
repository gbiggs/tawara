/* Tide
 *
 * Header for the TrackOperation element and related elements.
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

#if !defined(TIDE_TRACK_OPERATION_H_)
#define TIDE_TRACK_OPERATION_H_

#include <tide/el_ids.h>
#include <tide/master_element.h>
#include <tide/uint_element.h>
#include <tide/win_dll.h>
#include <vector>

/// \addtogroup elements Elements
/// @{

namespace tide
{
    /** \brief Abstract base class for individual track operations.
     *
     * All track operations must inherit from this base class.
     *
     * This base class is somewhat poorly named. It is \e not the base class of
     * the tide::TrackOperation class.
     */
    class TIDE_EXPORT TrackOperationBase : public MasterElement
    {
        public:
            /// \brief Constructor - this must be called to set the Class ID.
            TrackOperationBase(ids::ID id)
                : MasterElement(id)
            {}

            /// \brief Desctructor.
            virtual ~TrackOperationBase() {}

            /** \brief Get the type of operation to be performed.
             *
             * \return A string describing the operation.
             */
            virtual std::string type() const = 0;
    }; // class TrackOperationBase


    /** \brief JoinBlocks track operation.
     *
     * This track operation is used to join the blocks of the source tracks
     * into a single virtual track. Usually it is used to join tracks that are
     * distinct in time. Joining tracks overlapped in time will lead to
     * interleaved blocks of data at best and undefined results at worst, and
     * should be avoided.
     */
    class TIDE_EXPORT TrackJoinBlocks : public TrackOperationBase
    {
        public:
            /// \brief Construct a new JoinBlocks operation.
            TrackJoinBlocks();

            /// \brief Destructor.
            ~TrackJoinBlocks() {}

            /// \brief Get the type of operation to be performed.
            std::string type() const
            {
                return "joinblocks";
            }

            /** \brief Append a new UID to this operation.
             *
             * \param[in] uid The UID to append.
             * \throw ValueOutOfRange if a zero-value UID is appended.
             */
            void append(uint64_t uid) = 0;

            /** \brief Remove a UID.
             *
             * \param[in] pos The position of the UID to remove.
             * \return The removed UID.
             */
            uint64_t remove(unsigned int pos);

            /** \brief Const subscript operator.
             *
             * Gets the UID at the specified position.
             */
            uint64_t const& operator[](unsigned int pos) const;

            /** \brief Subscript operator.
             *
             * Gets the UID at the specified position.
             */
            uint64_t& operator[](unsigned int pos);

            /// \brief Get the number of UIDs stored.
            unsigned int count() const { return uids_.size(); }

            /// TODO: Proper vector model for UIDs.

            /// \brief Get the size of the body of this element.
            virtual std::streamsize size() const;

            /** \brief Element body writing.
             *
             * \throw ValueOutOfRange if a zero-value UID is written.
             */
            virtual std::streamsize write_body(std::ostream& output);

            /// \brief Element body loading.
            virtual std::streamsize read_body(std::istream& input);

        protected:
            std::vector<UIntElement> uids_;
    }; // class TrackJoinBlocks


    /** \brief The TrackOperation element stores all operations for a track.
     *
     * The TrackOperation element contains a list of operations that are
     * performed on other tracks to create a virtual track. Currently only the
     * JoinBlocks operation is supported, which combines the blocks from the
     * specified tracks into a single virtual stream.
     */
    class TIDE_EXPORT TrackOperation : public MasterElement
    {
        public:
            /// \brief Construct a new TrackOperation element.
            TrackOperation();

            /// \brief Destructor.
            virtual ~TrackOperation() {}

            /** \brief Append a new operation to this list of operations.
             *
             * \param[in] op The operation to append.
             * \throw ValueOutOfRange if an empty operation is appended.
             */
            void append(TrackOperationBase const& op);

            /** \brief Remove an operation.
             *
             * \param[in] pos The position of the operation to remove.
             * \return The removed operation.
             */
            TrackOperationBase remove(unsigned int pos);

            /** \brief Const subscript operator.
             *
             * Gets the operation at the specified position.
             */
            TrackOperationBase const& operator[](unsigned int pos) const;

            /** \brief Subscript operator.
             *
             * Gets the operation at the specified position.
             */
            TrackOperationBase& operator[](unsigned int pos);

            /// \brief Get the number of operations stored.
            unsigned int count() const { return operations_.size(); }

            /// TODO: Proper vector model for operations.

            /// \brief Get the size of the body of this element.
            virtual std::streamsize size() const;

            /// \brief Element body writing.
            virtual std::streamsize write_body(std::ostream& output);

            /// \brief Element body loading.
            virtual std::streamsize read_body(std::istream& input);

        protected:
            std::vector<TrackOperationBase> operations_;
    }; // class TrackOperation
}; // namespace tide

/// @}
// group elements

#endif // TIDE_TRACK_OPERATION_H_

