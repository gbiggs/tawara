/* Tide
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

#include <boost/filesystem.hpp>
#include <cstdio>
#include <gtest/gtest.h>
#include <tide/tide_file.h>
#include <tide/exceptions.h>

#include "test_consts.h"

#include <boost/iostreams/device/null.hpp>
#include <boost/iostreams/stream.hpp>


TEST(OpenFile, NoName)
{
    // Create an instance of a Tide object without specifying a file name
    EXPECT_THROW(tide::TideFile("", tide::MODE_READ), tide::NoName);
    EXPECT_THROW(tide::TideFile("", tide::MODE_WRITE), tide::NoName);
    // Versions with a stream object (different constructor)
    boost::iostreams::stream<boost::iostreams::null_sink> null;
    null.open(boost::iostreams::null_sink());
    EXPECT_THROW(tide::TideFile("", tide::MODE_READ, null), tide::NoName);
    EXPECT_THROW(tide::TideFile("", tide::MODE_WRITE, null), tide::NoName);
}


TEST(OpenFile, NewObject)
{
    // Create a new object
    /*EXPECT_NOTHROW(
    {
        tide::TideFile t(tmpnam(), tide::MODE_
    });*/
}


TEST(OpenFile, ExistingFilePresentOverwrite)
{
    // After truncation, the file size should not be zero due to the EBML
    // header added upon opening
    // ASSERT_?(file size != sizeof(not_ebml.tide.in))
}


TEST(OpenFile, ExistingFilePresentNoOverwrite)
{
}


TEST(OpenFile, OpenForReadingNotExist)
{
    EXPECT_THROW(tide::TideFile("file_that_does_not_exist", tide::MODE_READ),
        tide::NoObject);
    boost::iostreams::stream<boost::iostreams::null_sink> null;
    null.open(boost::iostreams::null_sink());
    EXPECT_THROW(tide::TideFile("file_that_does_not_exist", tide::MODE_READ,
        null), tide::NoObject);
}


TEST(OpenFile, OpenForAppending)
{
    boost::filesystem::path existing(test_bin_dir / "existing.tide");

    EXPECT_NO_THROW(tide::TideFile(existing.string(), tide::MODE_APPEND));
    EXPECT_EQ(true, boost::filesystem::is_regular_file(existing));
    boost::filesystem::remove(existing);

    boost::filesystem3::copy(test_source_dir / "empty_ebml.tide.in", existing);
    EXPECT_NO_THROW(tide::TideFile(existing.string(), tide::MODE_APPEND));
    boost::filesystem::remove(existing);

    boost::filesystem3::copy(test_source_dir / "not_ebml.tide.in", existing);
    EXPECT_THROW(tide::TideFile(existing.string(), tide::MODE_APPEND),
        tide::NotTide);
    boost::filesystem::remove(existing);
}


TEST(OpenFile, NotEBMLFile)
{
    boost::filesystem::path not_ebml(test_bin_dir / "not_ebml.tide");
    boost::filesystem3::copy(test_source_dir / "not_ebml.tide.in", not_ebml);
    EXPECT_THROW(tide::TideFile(not_ebml.native(), tide::MODE_READ),
        tide::NotTide);
    EXPECT_THROW(tide::TideFile(not_ebml.string(), tide::MODE_APPEND),
        tide::NotTide);
    // MODE_WRITE should truncate the file, and so not complain
    EXPECT_NO_THROW(tide::TideFile(not_ebml.string(), tide::MODE_WRITE));
    boost::filesystem::remove(not_ebml);
}


TEST(OpenFile, NotTIDEFile)
{
}


TEST(OpenFile, NotStartWith1A)
{
    // EBML files are supposed to ignore everything up to the first occurence
    // of 0x1A.
}

