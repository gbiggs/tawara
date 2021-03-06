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

#include <gtest/gtest.h>
#include <tawara/attachments.h>
#include <tawara/el_ids.h>
#include <tawara/exceptions.h>
#include <tawara/vint.h>

#include "test_utils.h"


///////////////////////////////////////////////////////////////////////////////
// AttachedFile tests
///////////////////////////////////////////////////////////////////////////////

TEST(AttachedFile, Create)
{
    tawara::AttachedFile f1;
    EXPECT_EQ("", f1.description());
    EXPECT_EQ("", f1.name());
    EXPECT_EQ("", f1.mime_type());
    EXPECT_EQ(1, f1.uid());
    EXPECT_TRUE(!f1.data());

    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tawara::FileData::Ptr fd(new tawara::FileData(*blob));
    tawara::AttachedFile f2("name", "mime", fd, 42);
    EXPECT_EQ("", f2.description());
    EXPECT_EQ("name", f2.name());
    EXPECT_EQ("mime", f2.mime_type());
    EXPECT_EQ(42, f2.uid());
    EXPECT_TRUE(*fd == *f2.data());
}


TEST(AttachedFile, Description)
{
    tawara::AttachedFile f;
    EXPECT_EQ("", f.description());
    f.description("desc");
    EXPECT_EQ("desc", f.description());
}


TEST(AttachedFile, Name)
{
    tawara::AttachedFile f;
    EXPECT_EQ("", f.name());
    f.name("name");
    EXPECT_EQ("name", f.name());
}


TEST(AttachedFile, Mime)
{
    tawara::AttachedFile f;
    EXPECT_EQ("", f.mime_type());
    f.mime_type("mime");
    EXPECT_EQ("mime", f.mime_type());
}


TEST(AttachedFile, Data)
{
    tawara::AttachedFile f;
    EXPECT_TRUE(!f.data());
    tawara::FileData::Ptr data(new tawara::FileData(std::vector<char>()));
    EXPECT_THROW(f.data(data), tawara::NoAttachedData);
    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tawara::FileData::Ptr data2(new tawara::FileData(*blob));
    f.data(data2);
    EXPECT_TRUE(*data2 == *f.data());
}


TEST(AttachedFile, UID)
{
    tawara::AttachedFile f;
    EXPECT_EQ(1, f.uid());
    f.uid(42);
    EXPECT_EQ(42, f.uid());
    EXPECT_THROW(f.uid(0), tawara::ValueOutOfRange);
}


TEST(AttachedFile, Size)
{
    tawara::StringElement desc(tawara::ids::FileDescription, "");
    tawara::StringElement name(tawara::ids::FileName, "");
    tawara::StringElement mime(tawara::ids::FileMimeType, "");
    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tawara::FileData::Ptr data(new tawara::FileData(*blob));
    tawara::UIntElement uid(tawara::ids::FileUID, 1);

    tawara::AttachedFile f1;
    f1.data(data);
    std::streamsize body_size(name.size() + mime.size() + data->size() +
            uid.size());
    EXPECT_EQ(tawara::ids::size(tawara::ids::AttachedFile) +
            tawara::vint::size(body_size) + body_size, f1.size());

    desc = "desc";
    name = "name";
    mime = "mime";
    uid = 42;

    tawara::AttachedFile f2("name", "mime", data, 42);
    f2.description("desc");
    body_size = desc.size() + name.size() + mime.size() +
            data->size() + uid.size();
    EXPECT_EQ(tawara::ids::size(tawara::ids::AttachedFile) +
            tawara::vint::size(body_size) + body_size, f2.size());
}


TEST(AttachedFile, Write)
{
    std::ostringstream output;
    std::stringstream expected;
    tawara::StringElement desc(tawara::ids::FileDescription, "");
    tawara::StringElement name(tawara::ids::FileName, "");
    tawara::StringElement mime(tawara::ids::FileMimeType, "");
    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tawara::FileData::Ptr data(new tawara::FileData(*blob));
    tawara::UIntElement uid(tawara::ids::FileUID, 21);

    tawara::AttachedFile f1;
    f1.data(data);
    std::streamsize body_size(name.size() + mime.size() + data->size() +
            uid.size());
    tawara::ids::write(tawara::ids::AttachedFile, expected);
    tawara::vint::write(body_size, expected);
    name.write(expected);
    mime.write(expected);
    data->write(expected);
    uid.write(expected);
    EXPECT_EQ(tawara::ids::size(tawara::ids::AttachedFile) +
            tawara::vint::size(body_size) + body_size, f1.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            output.str());

    desc = "desc";
    name = "name";
    mime = "mime";
    uid = 42;

    expected.str(std::string());
    output.str(std::string());
    tawara::AttachedFile f2("name", "mime", data, 42);
    f2.description("desc");
    body_size = desc.size() + name.size() + mime.size() + data->size() +
        uid.size();
    tawara::ids::write(tawara::ids::AttachedFile, expected);
    tawara::vint::write(body_size, expected);
    desc.write(expected);
    name.write(expected);
    mime.write(expected);
    data->write(expected);
    uid.write(expected);
    EXPECT_EQ(tawara::ids::size(tawara::ids::AttachedFile) +
            tawara::vint::size(body_size) + body_size, f2.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            output.str());
}


TEST(AttachedFile, Read)
{
    std::stringstream input;
    tawara::StringElement desc(tawara::ids::FileDescription, "");
    tawara::StringElement name(tawara::ids::FileName, "");
    tawara::StringElement mime(tawara::ids::FileMimeType, "");
    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tawara::FileData::Ptr data(new tawara::FileData(*blob));
    tawara::UIntElement uid(tawara::ids::FileUID, 21);

    tawara::AttachedFile f;
    std::streamsize body_size(name.size() + mime.size() + data->size() +
            uid.size());
    tawara::vint::write(body_size, input);
    name.write(input);
    mime.write(input);
    data->write(input);
    uid.write(input);
    EXPECT_EQ(tawara::vint::size(body_size) + body_size, f.read(input));
    EXPECT_EQ("", f.description());
    EXPECT_EQ("", f.name());
    EXPECT_EQ("", f.mime_type());
    EXPECT_TRUE(*data == *f.data());
    EXPECT_EQ(21, f.uid());

    desc = "desc";
    name = "name";
    mime = "mime";
    uid = 42;

    input.str(std::string());
    body_size = desc.size() + name.size() + mime.size() + data->size() +
        uid.size();
    tawara::vint::write(body_size, input);
    desc.write(input);
    name.write(input);
    mime.write(input);
    data->write(input);
    uid.write(input);
    EXPECT_EQ(tawara::vint::size(body_size) + body_size, f.read(input));
    EXPECT_EQ("desc", f.description());
    EXPECT_EQ("name", f.name());
    EXPECT_EQ("mime", f.mime_type());
    EXPECT_TRUE(*data == *f.data());
    EXPECT_EQ(42, f.uid());

    // Body size value wrong (too small)
    input.str(std::string());
    tawara::vint::write(2, input);
    name.write(input);
    mime.write(input);
    EXPECT_THROW(f.read(input), tawara::BadBodySize);
    // Invalid child
    input.str(std::string());
    tawara::UIntElement ue(tawara::ids::EBML, 0xFFFF);
    tawara::vint::write(ue.size(), input);
    ue.write(input);
    EXPECT_THROW(f.read(input), tawara::InvalidChildID);
    // No name
    input.str(std::string());
    tawara::vint::write(mime.size() + data->size() + uid.size(), input);
    mime.write(input);
    data->write(input);
    uid.write(input);
    EXPECT_THROW(f.read(input), tawara::MissingChild);
    // No MIME type
    input.str(std::string());
    tawara::vint::write(name.size() + data->size() + uid.size(), input);
    name.write(input);
    data->write(input);
    uid.write(input);
    EXPECT_THROW(f.read(input), tawara::MissingChild);
    // No data
    input.str(std::string());
    tawara::vint::write(name.size() + mime.size() + uid.size(), input);
    name.write(input);
    mime.write(input);
    uid.write(input);
    EXPECT_THROW(f.read(input), tawara::MissingChild);
    // No UID
    input.str(std::string());
    tawara::vint::write(name.size() + mime.size() + data->size(), input);
    name.write(input);
    mime.write(input);
    data->write(input);
    EXPECT_THROW(f.read(input), tawara::MissingChild);
    // UID is zero
    input.str(std::string());
    uid = 0;
    tawara::vint::write(uid.size(), input);
    uid.write(input);
    EXPECT_THROW(f.read(input), tawara::ValueOutOfRange);
}


///////////////////////////////////////////////////////////////////////////////
// Attachments tests
///////////////////////////////////////////////////////////////////////////////

TEST(Attachments, Create)
{
    tawara::Attachments a;
    EXPECT_TRUE(a.empty());
}


TEST(Attachments, Assignment)
{
    tawara::Attachments a1;
    tawara::Attachments a2;
    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tawara::FileData::Ptr fd(new tawara::FileData(*blob));
    tawara::AttachedFile f("name", "mime", fd, 42);
    a1.push_back(f);

    EXPECT_TRUE(a2.empty());
    a2 = a1;
    EXPECT_FALSE(a2.empty());
    EXPECT_TRUE(a2[0].data() == fd);
    EXPECT_TRUE(*(a2[0].data()) == *fd);
}


TEST(Attachments, At)
{
    tawara::Attachments a;
    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tawara::FileData::Ptr fd(new tawara::FileData(*blob));
    tawara::AttachedFile f("name", "mime", fd, 42);
    a.push_back(f);
    EXPECT_TRUE(a[0] == a.at(0));
    EXPECT_TRUE(*a.at(0).data() == *fd);
    EXPECT_THROW(a.at(1), std::out_of_range);
}


TEST(Attachments, SubscriptOperator)
{
    tawara::Attachments a;
    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tawara::FileData::Ptr fd(new tawara::FileData(*blob));
    tawara::AttachedFile f("name", "mime", fd, 42);
    a.push_back(f);
    EXPECT_TRUE(a[0] == a.at(0));
    EXPECT_TRUE(*a[0].data() == *fd);
    EXPECT_NO_THROW(a[1]);

    boost::shared_ptr<std::vector<char> > blob2(test_utils::make_blob(5));
    tawara::FileData::Ptr fd2(new tawara::FileData(*blob));
    tawara::AttachedFile f2("name2", "mime2", fd, 84);
    a[0] = f2;
    EXPECT_TRUE(a[0] == f2);
    EXPECT_TRUE(*a[0].data() == *fd2);
}


TEST(Attachments, BeginEnd)
{
    tawara::Attachments a;
    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tawara::FileData::Ptr fd(new tawara::FileData(*blob));
    tawara::AttachedFile f("name", "mime", fd, 42);

    EXPECT_TRUE(a.begin() == a.end());
    EXPECT_TRUE(a.rbegin() == a.rend());
    a.push_back(f);
    EXPECT_FALSE(a.begin() == a.end());
    EXPECT_FALSE(a.rbegin() == a.rend());
}


TEST(Attachments, Counts)
{
    tawara::Attachments a;
    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tawara::FileData::Ptr fd(new tawara::FileData(*blob));
    tawara::AttachedFile f("name", "mime", fd, 42);

    EXPECT_TRUE(a.empty());
    a.push_back(f);
    EXPECT_FALSE(a.empty());
    EXPECT_EQ(1, a.count());
}


TEST(Attachments, Clear)
{
    tawara::Attachments a;
    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tawara::FileData::Ptr fd(new tawara::FileData(*blob));
    tawara::AttachedFile f("name", "mime", fd, 42);
    a.push_back(f);
    EXPECT_FALSE(a.empty());
    a.clear();
    EXPECT_TRUE(a.empty());
}


TEST(Attachments, push_back)
{
    tawara::Attachments a;
    boost::shared_ptr<std::vector<char> > blob1(test_utils::make_blob(5));
    tawara::FileData::Ptr fd1(new tawara::FileData(*blob1));
    tawara::AttachedFile f1("name", "mime", fd1, 42);
    boost::shared_ptr<std::vector<char> > blob2(test_utils::make_blob(5));
    tawara::FileData::Ptr fd2(new tawara::FileData(*blob2));
    tawara::AttachedFile f2("name", "mime", fd2, 42);

    EXPECT_TRUE(a.empty());
    a.push_back(f1);
    EXPECT_FALSE(a.empty());
    EXPECT_EQ(1, a.count());
    EXPECT_TRUE(*a[0].data() == *fd1);
    a.push_back(f2);
    EXPECT_EQ(2, a.count());
    EXPECT_TRUE(*a[0].data() == *fd1);
    EXPECT_TRUE(*a[1].data() == *fd2);
}


TEST(Attachments, Erase)
{
    tawara::Attachments a;
    boost::shared_ptr<std::vector<char> > blob1(test_utils::make_blob(5));
    tawara::FileData::Ptr fd1(new tawara::FileData(*blob1));
    tawara::AttachedFile f1("name", "mime", fd1, 42);
    boost::shared_ptr<std::vector<char> > blob2(test_utils::make_blob(5));
    tawara::FileData::Ptr fd2(new tawara::FileData(*blob2));
    tawara::AttachedFile f2("name", "mime", fd2, 42);

    a.push_back(f1);
    a.push_back(f2);
    EXPECT_EQ(2, a.count());
    a.erase(a.begin());
    EXPECT_EQ(1, a.count());
    a.erase(a.begin());
    EXPECT_EQ(0, a.count());

    a.push_back(f1);
    a.push_back(f2);
    EXPECT_FALSE(a.empty());
    a.erase(a.begin(), a.end());
    EXPECT_TRUE(a.empty());
}


TEST(Attachments, Swap)
{
    tawara::Attachments a1, a2;
    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tawara::FileData::Ptr fd(new tawara::FileData(*blob));
    tawara::AttachedFile f("name", "mime", fd, 42);
    a1.push_back(f);

    EXPECT_FALSE(a1.empty());
    EXPECT_TRUE(a2.empty());
    a2.swap(a1);
    EXPECT_TRUE(a1.empty());
    EXPECT_FALSE(a2.empty());
    EXPECT_TRUE(*a2[0].data() == *fd);
}


TEST(Attachments, Equality)
{
    tawara::Attachments a1, a2;
    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tawara::FileData::Ptr fd(new tawara::FileData(*blob));
    tawara::AttachedFile f("name", "mime", fd, 42);

    EXPECT_TRUE(a1 == a2);
    EXPECT_FALSE(a1 != a2);

    a1.push_back(f);
    EXPECT_FALSE(a1 == a2);
    EXPECT_TRUE(a1 != a2);
}


TEST(Attachments, Size)
{
    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tawara::FileData::Ptr fd(new tawara::FileData(*blob));
    tawara::AttachedFile f("name", "mime", fd, 42);

    tawara::Attachments a;
    a.push_back(f);
    EXPECT_EQ(tawara::ids::size(tawara::ids::Attachments) +
            tawara::vint::size(f.size()) + f.size(), a.size());
}


TEST(Attachments, Write)
{
    std::ostringstream output;
    std::stringstream expected;
    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tawara::FileData::Ptr fd(new tawara::FileData(*blob));
    tawara::AttachedFile f("name", "mime", fd, 42);
    tawara::Attachments a;

    // No attached files
    EXPECT_THROW(a.write(output), tawara::NoAttachments);

    output.str(std::string());
    a.push_back(f);
    std::streamsize body_size(f.size());
    tawara::ids::write(tawara::ids::Attachments, expected);
    tawara::vint::write(body_size, expected);
    f.write(expected);
    EXPECT_EQ(tawara::ids::size(tawara::ids::Attachments) +
            tawara::vint::size(body_size) + body_size, a.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            output.str());
}


TEST(Attachments, Read)
{
    std::stringstream input;
    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tawara::FileData::Ptr fd(new tawara::FileData(*blob));
    tawara::AttachedFile f("name", "mime", fd, 42);

    tawara::Attachments a;
    std::streamsize body_size(f.size());
    tawara::vint::write(body_size, input);
    f.write(input);
    EXPECT_EQ(tawara::vint::size(body_size) + body_size, a.read(input));
    EXPECT_FALSE(a.empty());
    EXPECT_TRUE(*a[0].data() == *fd);

    // Body size value wrong (too small)
    input.str(std::string());
    tawara::vint::write(2, input);
    f.write(input);
    EXPECT_THROW(a.read(input), tawara::BadBodySize);
    // Invalid child
    input.str(std::string());
    tawara::UIntElement ue(tawara::ids::EBML, 0xFFFF);
    tawara::vint::write(ue.size(), input);
    ue.write(input);
    EXPECT_THROW(a.read(input), tawara::InvalidChildID);
    // No attachments
    input.str(std::string());
    tawara::vint::write(0, input);
    EXPECT_THROW(a.read(input), tawara::NoAttachments);
}

