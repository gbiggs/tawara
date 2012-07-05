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

#include <boost/filesystem.hpp>
#include <gtest/gtest.h>
#include <celduin/exceptions.h>
#include <celduin/celduin_impl.h>

#include "test_consts.h"


namespace test_celduin
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
}; // namespace test_celduin


TEST(Celduin, EmptyFile)
{
    boost::filesystem::path path(test_bin_dir / "empty_file.celduin");
    if (boost::filesystem::exists(path))
    {
        boost::filesystem::remove(path);
    }
    std::fstream file(path.string().c_str(),
            std::ios::in|std::ios::out|std::ios::app);
    EXPECT_NO_THROW(celduin::CelduinImpl t(file));
    file.close();
    // A Celduin file with just the EBML header should be 36 bytes
    EXPECT_EQ(34, boost::filesystem::file_size(path));
    boost::filesystem::remove(path);
}


TEST(Celduin, ExistingFile)
{
    std::fstream file;
    test_celduin::prepare_file("celduin.celduin", file);
    file.seekp(0, std::ios::end);
    std::streamsize file_size(file.tellp());
    file.seekp(0, std::ios::beg);
    EXPECT_NO_THROW(celduin::CelduinImpl t(file));
    file.seekp(0, std::ios::end);
    // The file should not be modified just by opening it
    EXPECT_EQ(file_size, file.tellp());
    test_celduin::cleanup_file("celduin.celduin", file);
}


TEST(Celduin, NotEBMLFile)
{
    std::fstream file;
    test_celduin::prepare_file("not_ebml.celduin", file);
    EXPECT_THROW(celduin::CelduinImpl t(file), celduin::NotEBML);
    test_celduin::cleanup_file("not_ebml.celduin", file);
}


TEST(Celduin, NotCelduinFile)
{
    std::fstream file;
    test_celduin::prepare_file("not_celduin.celduin", file);
    EXPECT_THROW(celduin::CelduinImpl t(file), celduin::NotCelduin);
    test_celduin::cleanup_file("not_celduin.celduin", file);
}


TEST(Celduin, TruncatedCelduinFile)
{
    std::fstream file;
    test_celduin::prepare_file("truncated.celduin", file);
    EXPECT_THROW(celduin::CelduinImpl t(file), celduin::ReadError);
    test_celduin::cleanup_file("truncated.celduin", file);
}


TEST(Celduin, TextBeforeHeader)
{
    std::fstream file;
    test_celduin::prepare_file("with_text.celduin", file);
    EXPECT_NO_THROW(celduin::CelduinImpl t(file));
    test_celduin::cleanup_file("with_text.celduin", file);
}


TEST(Celduin, BadReadVersion)
{
    std::fstream file;
    test_celduin::prepare_file("badreadversion.celduin", file);
    EXPECT_THROW(celduin::CelduinImpl t(file), celduin::BadReadVersion);
    test_celduin::cleanup_file("badreadversion.celduin", file);
}


TEST(Celduin, BadDocReadVersion)
{
    std::fstream file;
    test_celduin::prepare_file("baddocreadversion.celduin", file);
    EXPECT_THROW(celduin::CelduinImpl t(file), celduin::BadDocReadVersion);
    test_celduin::cleanup_file("baddocreadversion.celduin", file);
}

