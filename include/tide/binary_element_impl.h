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

#if !defined(BINARY_ELEMENT_IMPL_H_)
#define BINARY_ELEMENT_IMPL_H_

#include <tide/primitive_element_base.h>

#include <ios>
#include <vector>

/// \addtogroup implmentations Internal implementations
/// @{

namespace tide
{
    namespace impl
    {
        /** \brief Internal implementation of the binary primitive element.
         *
         * This class implements the PrimitiveElementBase CRTP requirements.
         */
        class BinaryElementImpl
            : public PrimitiveElementBase<BinaryElementImpl,
                std::vector<char> >
        {
            friend class PrimitiveElementBase<BinaryElementImpl,
                   std::vector<char> >;

            public:
                /** \brief Constructor.
                 *
                 * \param[in] value The value of the element.
                 */
                BinaryElementImpl(std::vector<char> const& value);

                /** \brief Constructor.
                 *
                 * \param[in] value The value of the element.
                 * \param[in] default_val The default value of the element.
                 */
                BinaryElementImpl(std::vector<char> const& value,
                        std::vector<char> const& default_val);

                /** \brief Swap this element's value with another's.
                 *
                 * \param[in] other The other floating point element to swap
                 * with.
                 */
                void swap(BinaryElementImpl& other);

                /// \brief Less-than comparison operator.
                bool operator<(BinaryElementImpl const& rhs)
                {
                    return value_ < rhs.value_;
                }

                /// \brief Less-than comparison operator.
                bool operator<(std::vector<char> const& rhs)
                {
                    return value_ < rhs;
                }

                /// \brief Greater-than comparison operator.
                bool operator>(std::vector<char> const& rhs)
                {
                    return value_ > rhs;
                }

                /// \brief Get the size of the stored value.
                std::streamsize body_stored_size() const;

                /// \brief Read the stored value from a byte stream.
                std::streamsize read_body(std::istream& i, std::streamsize size);

                /// \brief Begin writing the stored value to a byte stream.
                std::streamsize start_body(std::ostream& o) const;

                /// \brief Finish writing the stored value to a byte stream.
                std::streamsize finish_body(std::ostream& o) const;

                /** \brief Stream output operator.
                 *
                 * This function is a friend so it can access the value_ member
                 * without copying.
                 * TODO: remove friendship when PrimitiveElement supports
                 * references in its virtual interface.
                 */
                friend std::ostream& operator<<(std::ostream& o,
                        BinaryElementImpl const& rhs);

            private:
                ///////////////////////////////////////////////////////////////////
                // PrimitiveElement CRTP required members
                std::vector<char> value_;
                std::vector<char> default_;
                bool has_default_;
                // Must be mutable to be updated in start_body()
                mutable std::streampos body_end_;
        }; // class BinaryElementImpl


        /// \brief Swap date element implementation objects.
        void swap(BinaryElementImpl& a, BinaryElementImpl& b);

        /// \brief Stream output operator.
        std::ostream& operator<<(std::ostream& o, BinaryElementImpl const& rhs);
    }; // namespace impl
}; // namespace tide

/// @}
// group implementations

#endif // !defined(BINARY_ELEMENT_IMPL_H_

