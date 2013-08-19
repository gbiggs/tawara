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
#include <tawara/exceptions.h>
#include <tawara/tawara_impl.h>

#include "test_consts.h"


namespace test_tawara
{
    void prepare_file(std::string name, std::fstream& file)
    {
        boost::filesystem::path src_path(test_source_dir / name);
        boost::filesystem::path path(test_bin_dir / name);
        if (boost::filesystem::exists(path))
        {
            boost::filesystem::remove(path);
        }
        boost::filesystem::copy(src_path, path);
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
}; // namespace test_tawara


TEST(Tawara, EmptyFile)
{
    boost::filesystem::path path(test_bin_dir / "empty_file.tawara");
    if (boost::filesystem::exists(path))
    {
        boost::filesystem::remove(path);
    }
    std::fstream file(path.string().c_str(),
            std::ios::in|std::ios::out|std::ios::app);
    EXPECT_NO_THROW(tawara::TawaraImpl t(file));
    file.close();
    // A Tawara file with just the EBML header should be 36 bytes
    EXPECT_EQ(34, boost::filesystem::file_size(path));
    boost::filesystem::remove(path);
}


TEST(Tawara, ExistingFile)
{
    std::fstream file;
    test_tawara::prepare_file("tawara.tawara", file);
    file.seekp(0, std::ios::end);
    std::streamsize file_size(file.tellp());
    file.seekp(0, std::ios::beg);
    EXPECT_NO_THROW(tawara::TawaraImpl t(file));
    file.seekp(0, std::ios::end);
    // The file should not be modified just by opening it
    EXPECT_EQ(file_size, file.tellp());
    test_tawara::cleanup_file("tawara.tawara", file);
}


TEST(Tawara, NotEBMLFile)
{
    std::fstream file;
    test_tawara::prepare_file("not_ebml.tawara", file);
    EXPECT_THROW(tawara::TawaraImpl t(file), tawara::NotEBML);
    test_tawara::cleanup_file("not_ebml.tawara", file);
}


TEST(Tawara, NotTawaraFile)
{
    std::fstream file;
    test_tawara::prepare_file("not_tawara.tawara", file);
    EXPECT_THROW(tawara::TawaraImpl t(file), tawara::NotTawara);
    test_tawara::cleanup_file("not_tawara.tawara", file);
}


TEST(Tawara, TruncatedTawaraFile)
{
    std::fstream file;
    test_tawara::prepare_file("truncated.tawara", file);
    EXPECT_THROW(tawara::TawaraImpl t(file), tawara::ReadError);
    test_tawara::cleanup_file("truncated.tawara", file);
}


TEST(Tawara, TextBeforeHeader)
{
    std::fstream file;
    test_tawara::prepare_file("with_text.tawara", file);
    EXPECT_NO_THROW(tawara::TawaraImpl t(file));
    test_tawara::cleanup_file("with_text.tawara", file);
}


TEST(Tawara, BadReadVersion)
{
    std::fstream file;
    test_tawara::prepare_file("badreadversion.tawara", file);
    EXPECT_THROW(tawara::TawaraImpl t(file), tawara::BadReadVersion);
    test_tawara::cleanup_file("badreadversion.tawara", file);
}


TEST(Tawara, BadDocReadVersion)
{
    std::fstream file;
    test_tawara::prepare_file("baddocreadversion.tawara", file);
    EXPECT_THROW(tawara::TawaraImpl t(file), tawara::BadDocReadVersion);
    test_tawara::cleanup_file("baddocreadversion.tawara", file);
}

