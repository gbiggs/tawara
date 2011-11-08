/* Tide
 *
 * Test the Tide interface.
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
#include <gtest/gtest.h>
#include <tide/tide_impl.h>
#include <tide/exceptions.h>

#include "test_consts.h"


class TideTest : public ::testing::Test
{
    public:
        TideTest()
            : ::testing::Test(),
            empty_path(test_bin_dir / "empty_file.tide"),
            not_ebml_src_path(test_source_dir / "not_ebml.tide"),
            not_ebml_path(test_bin_dir / "not_ebml.tide"),
            not_tide_src_path(test_source_dir / "not_tide.tide"),
            not_tide_path(test_bin_dir / "not_tide.tide"),
            tide_src_path(test_source_dir / "tide.tide"),
            tide_path(test_bin_dir / "tide.tide")
        {
            empty_file.open(empty_path.string().c_str(),
                    std::ios::in|std::ios::out|std::ios::app);
            boost::filesystem3::copy(not_ebml_src_path, not_ebml_path);
            not_ebml_file.open(not_ebml_path.string().c_str(),
                    std::ios::in|std::ios::out|std::ios::app);
            boost::filesystem3::copy(not_tide_src_path, not_tide_path);
            not_tide_file.open(not_tide_path.string().c_str(),
                    std::ios::in|std::ios::out|std::ios::app);
            boost::filesystem3::copy(tide_src_path, tide_path);
            tide_file.open(tide_path.string().c_str(),
                    std::ios::in|std::ios::out|std::ios::app);
        }

        ~TideTest()
        {
            empty_file.close();
            boost::filesystem::remove(empty_path);
            not_ebml_file.close();
            boost::filesystem::remove(not_ebml_path);
            not_tide_file.close();
            boost::filesystem::remove(not_tide_path);
            tide_file.close();
            boost::filesystem::remove(tide_path);
        }

        boost::filesystem::path empty_path;
        boost::filesystem::path not_ebml_src_path;
        boost::filesystem::path not_ebml_path;
        boost::filesystem::path not_tide_src_path;
        boost::filesystem::path not_tide_path;
        boost::filesystem::path tide_src_path;
        boost::filesystem::path tide_path;

        std::fstream empty_file;
        std::fstream not_ebml_file;
        std::fstream not_tide_file;
        std::fstream tide_file;
}; // class TideTest


TEST_F(TideTest, EmptyFile)
{
    EXPECT_NO_THROW(tide::TideImpl t(empty_file));
    empty_file.close();
    // A Tide file with just the EBML header should be 36 bytes
    EXPECT_EQ(36, boost::filesystem::file_size(empty_path));
}


TEST_F(TideTest, ExistingFile)
{
    tide_file.seekp(0, std::ios::end);
    std::streamsize file_size(tide_file.tellp());
    tide_file.seekp(0, std::ios::beg);
    EXPECT_NO_THROW(tide::TideImpl t(tide_file));
    tide_file.seekp(0, std::ios::end);
    // The file should not be modified just by opening it
    EXPECT_EQ(file_size, tide_file.tellp());
}


TEST_F(TideTest, NotEBMLFile)
{
    EXPECT_THROW(tide::TideImpl t(not_ebml_file), tide::NotEBML);
}


TEST_F(TideTest, NotTideFile)
{
    EXPECT_THROW(tide::TideImpl t(not_tide_file), tide::NotTide);
}

