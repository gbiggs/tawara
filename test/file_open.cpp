/* TIDE
 *
 * Open file tests.
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

#include <gtest/gtest.h>
#include <tide/tide_file.h>
#include <tide/exceptions.h>

TEST(OpenFile, NoFileName)
{
    // Create an instance of a TIDE object without specifying a file name
    EXPECT_THROW(tide::TideFile("", tide::MODE_READ), tide::NoName);
    EXPECT_THROW(tide::TideFile("", tide::MODE_WRITE), tide::NoName);
    // Versions with a stream object (different constructor)
    tide::NullOStream elog;
    EXPECT_THROW(tide::TideFile("", tide::MODE_READ, elog), tide::NoName);
    EXPECT_THROW(tide::TideFile("", tide::MODE_WRITE, elog), tide::NoName);
}

TEST(OpenFile, NewObject)
{
}

TEST(OpenFile, ExistingFilePresentOverwrite)
{
}

TEST(OpenFile, ExistingFilePresentNoOverwrite)
{
}

TEST(OpenFile, OpenForReadingNotExist)
{
}

TEST(OpenFile, NotEBMLFile)
{
}

TEST(OpenFile, NotTIDEFile)
{
}

