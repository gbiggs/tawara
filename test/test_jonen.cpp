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
#include <jonen/exceptions.h>
#include <jonen/jonen_impl.h>

#include "test_consts.h"


namespace test_jonen
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
}; // namespace test_jonen


TEST(Jonen, EmptyFile)
{
    boost::filesystem::path path(test_bin_dir / "empty_file.jonen");
    if (boost::filesystem::exists(path))
    {
        boost::filesystem::remove(path);
    }
    std::fstream file(path.string().c_str(),
            std::ios::in|std::ios::out|std::ios::app);
    EXPECT_NO_THROW(jonen::JonenImpl t(file));
    file.close();
    // A Jonen file with just the EBML header should be 36 bytes
    EXPECT_EQ(34, boost::filesystem::file_size(path));
    boost::filesystem::remove(path);
}


TEST(Jonen, ExistingFile)
{
    std::fstream file;
    test_jonen::prepare_file("jonen.jonen", file);
    file.seekp(0, std::ios::end);
    std::streamsize file_size(file.tellp());
    file.seekp(0, std::ios::beg);
    EXPECT_NO_THROW(jonen::JonenImpl t(file));
    file.seekp(0, std::ios::end);
    // The file should not be modified just by opening it
    EXPECT_EQ(file_size, file.tellp());
    test_jonen::cleanup_file("jonen.jonen", file);
}


TEST(Jonen, NotEBMLFile)
{
    std::fstream file;
    test_jonen::prepare_file("not_ebml.jonen", file);
    EXPECT_THROW(jonen::JonenImpl t(file), jonen::NotEBML);
    test_jonen::cleanup_file("not_ebml.jonen", file);
}


TEST(Jonen, NotJonenFile)
{
    std::fstream file;
    test_jonen::prepare_file("not_jonen.jonen", file);
    EXPECT_THROW(jonen::JonenImpl t(file), jonen::NotJonen);
    test_jonen::cleanup_file("not_jonen.jonen", file);
}


TEST(Jonen, TruncatedJonenFile)
{
    std::fstream file;
    test_jonen::prepare_file("truncated.jonen", file);
    EXPECT_THROW(jonen::JonenImpl t(file), jonen::ReadError);
    test_jonen::cleanup_file("truncated.jonen", file);
}


TEST(Jonen, TextBeforeHeader)
{
    std::fstream file;
    test_jonen::prepare_file("with_text.jonen", file);
    EXPECT_NO_THROW(jonen::JonenImpl t(file));
    test_jonen::cleanup_file("with_text.jonen", file);
}


TEST(Jonen, BadReadVersion)
{
    std::fstream file;
    test_jonen::prepare_file("badreadversion.jonen", file);
    EXPECT_THROW(jonen::JonenImpl t(file), jonen::BadReadVersion);
    test_jonen::cleanup_file("badreadversion.jonen", file);
}


TEST(Jonen, BadDocReadVersion)
{
    std::fstream file;
    test_jonen::prepare_file("baddocreadversion.jonen", file);
    EXPECT_THROW(jonen::JonenImpl t(file), jonen::BadDocReadVersion);
    test_jonen::cleanup_file("baddocreadversion.jonen", file);
}

