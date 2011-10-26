/* TIDE
 *
 * Header file for the file implementation of the TIDE interface.
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

#if !defined(TIDE_TIDE_FILE_H_)
#define TIDE_TIDE_FILE_H_

#include <tide/tide.h>
#include <tide/win_dll.h>

#include <fstream>
#include <iostream>
#include <string>

/// \addtogroup implementations Implementations
/// @{

namespace tide
{
    /** \brief File-based implementation of the Tide interface.
     *
     * See \see Tide for method documentation.
     */
    class TIDE_EXPORT TideFile : public Tide
    {
        public:
            /** \brief Constructor for a file-based Tide object.
             *
             * If \ref MODE_WRITE is selected and the file exists, the existing
             * file will be deleted.
             */
            TideFile(std::string name, MODE mode);

            /** \brief Constructor for a file-based Tide object.
             *
             * If \ref MODE_WRITE is selected and the file exists, the existing
             * file will be deleted.
             */
            TideFile(std::string name, MODE mode, std::ostream& verb_out);

            /** \brief Destructor for a file-based Tide object.
             *
             * The destructor will close the file and clean up any internal
             * buffers in use. Any objects referencing the file, such as
             * Channel or Tag objects, will become invalid.
             */
            virtual ~TideFile();

        protected:
            /// The name of the file to be operated on.
            std::string const name_;
            /// The open-mode of the file, one of the values of \ref MODE
            MODE const mode_;
            /// The verbose output destination.
            std::ostream& verb_;
            /// The file being operated on.
            std::fstream file_;

            /// \brief Opens the target file.
            void open();
            /// \brief Finalises and closes the target file.
            void close();
            /// \brief Checks that the EBML header is intact and correct
            bool validate_ebml_header();
    }; // class TideFile
}; // namespace tide

/// @}
// group implementations

#endif // TIDE_TIDE_FILE_H_

