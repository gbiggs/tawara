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
#include <tide/exceptions.h>
#include <tide/tide_impl.h>

#include "test_consts.h"


namespace test_tide
{
    void prepare_file(std::string name, std::fstream& file)
    {
        boost::filesystem::path src_path(test_source_dir / name);
        boost::filesystem::path path(test_bin_dir / name);
        if (boost::filesystem::exists(path))
        {
            boost::filesystem::remove(path);
        }
        boost::filesystem3::copy(src_path, path);
        file.open(path.string().c_str(),
                std::ios::in|std::ios::out|std::ios::app);
        file.seekg(0, std::ios::beg);
    }

    void cleanup_file(std::string name, std::fstream& file)
    {
        file.close();
        boost::filesystem::path path(test_bin_dir / name);
        boost::filesystem::remove(path);
    }
}; // namespace test_tide


TEST(Tide, EmptyFile)
{
    boost::filesystem::path path(test_bin_dir / "empty_file.tide");
    if (boost::filesystem::exists(path))
    {
        boost::filesystem::remove(path);
    }
    std::fstream file(path.string().c_str(),
            std::ios::in|std::ios::out|std::ios::app);
    EXPECT_NO_THROW(tide::TideImpl t(file));
    file.close();
    // A Tide file with just the EBML header should be 36 bytes
    EXPECT_EQ(34, boost::filesystem::file_size(path));
    boost::filesystem::remove(path);
}


TEST(Tide, ExistingFile)
{
    std::fstream file;
    test_tide::prepare_file("tide.tide", file);
    file.seekp(0, std::ios::end);
    std::streamsize file_size(file.tellp());
    file.seekp(0, std::ios::beg);
    EXPECT_NO_THROW(tide::TideImpl t(file));
    file.seekp(0, std::ios::end);
    // The file should not be modified just by opening it
    EXPECT_EQ(file_size, file.tellp());
    test_tide::cleanup_file("tide.tide", file);
}


TEST(Tide, NotEBMLFile)
{
    std::fstream file;
    test_tide::prepare_file("not_ebml.tide", file);
    EXPECT_THROW(tide::TideImpl t(file), tide::NotEBML);
    test_tide::cleanup_file("not_ebml.tide", file);
}


TEST(Tide, NotTideFile)
{
    std::fstream file;
    test_tide::prepare_file("not_tide.tide", file);
    EXPECT_THROW(tide::TideImpl t(file), tide::NotTide);
    test_tide::cleanup_file("not_tide.tide", file);
}


TEST(Tide, TruncatedTideFile)
{
    std::fstream file;
    test_tide::prepare_file("truncated.tide", file);
    EXPECT_THROW(tide::TideImpl t(file), tide::ReadError);
    test_tide::cleanup_file("truncated.tide", file);
}


TEST(Tide, TextBeforeHeader)
{
    std::fstream file;
    test_tide::prepare_file("with_text.tide", file);
    EXPECT_NO_THROW(tide::TideImpl t(file));
    test_tide::cleanup_file("with_text.tide", file);
}


TEST(Tide, BadReadVersion)
{
    std::fstream file;
    test_tide::prepare_file("badreadversion.tide", file);
    EXPECT_THROW(tide::TideImpl t(file), tide::BadReadVersion);
    test_tide::cleanup_file("badreadversion.tide", file);
}


TEST(Tide, BadDocReadVersion)
{
    std::fstream file;
    test_tide::prepare_file("baddocreadversion.tide", file);
    EXPECT_THROW(tide::TideImpl t(file), tide::BadDocReadVersion);
    test_tide::cleanup_file("baddocreadversion.tide", file);
}

