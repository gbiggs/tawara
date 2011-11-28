/* Tide
 *
 * Header for the track operation elements.
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

#include <boost/operators.hpp>
#include <boost/smart_ptr.hpp>
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

            /// \brief Base type of a track operation pointer.
            typedef boost::shared_ptr<TrackOperationBase> Ptr;
    }; // class TrackOperationBase


    /** \brief JoinBlocks track operation.
     *
     * This track operation is used to join the blocks of the source tracks
     * into a single virtual track. Usually it is used to join tracks that are
     * distinct in time. Joining tracks overlapped in time will lead to
     * interleaved blocks of data at best and undefined results at worst, and
     * should be avoided.
     */
    class TIDE_EXPORT TrackJoinBlocks : public TrackOperationBase,
            public boost::equality_comparable<TrackJoinBlocks>
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
            void append(uint64_t uid);

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
            uint64_t operator[](unsigned int pos) const;

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

            /// \brief Equality operator.
            friend bool operator==(TrackJoinBlocks const& lhs,
                    TrackJoinBlocks const& rhs);

        protected:
            std::vector<UIntElement> uids_;

            /// \brief Element body loading.
            virtual std::streamsize read_body(std::istream& input,
                    std::streamsize size);
    }; // class TrackJoinBlocks


    /// Equality operator for TrackJoinBlocks.
    inline bool operator==(TrackJoinBlocks const& lhs,
            TrackJoinBlocks const& rhs)
    {
        return lhs.uids_ == rhs.uids_;
    }
}; // namespace tide

/// @}
// group elements

#endif // TIDE_TRACK_OPERATION_H_

