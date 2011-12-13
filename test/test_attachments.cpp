/* Tide
 *
 * Test the attachments elements.
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

#include <gtest/gtest.h>
#include <tide/attachments.h>
#include <tide/el_ids.h>
#include <tide/exceptions.h>
#include <tide/vint.h>

#include "test_utils.h"


///////////////////////////////////////////////////////////////////////////////
// AttachedFile tests
///////////////////////////////////////////////////////////////////////////////

TEST(AttachedFile, Create)
{
    tide::AttachedFile f1;
    EXPECT_EQ("", f1.description());
    EXPECT_EQ("", f1.name());
    EXPECT_EQ("", f1.mime_type());
    EXPECT_EQ(1, f1.uid());
    EXPECT_TRUE(!f1.data());

    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tide::FileData::Ptr fd(new tide::FileData(*blob));
    tide::AttachedFile f2("name", "mime", fd, 42);
    EXPECT_EQ("", f2.description());
    EXPECT_EQ("name", f2.name());
    EXPECT_EQ("mime", f2.mime_type());
    EXPECT_EQ(42, f2.uid());
    EXPECT_TRUE(*fd == *f2.data());
}


TEST(AttachedFile, Description)
{
    tide::AttachedFile f;
    EXPECT_EQ("", f.description());
    f.description("desc");
    EXPECT_EQ("desc", f.description());
}


TEST(AttachedFile, Name)
{
    tide::AttachedFile f;
    EXPECT_EQ("", f.name());
    f.name("name");
    EXPECT_EQ("name", f.name());
}


TEST(AttachedFile, Mime)
{
    tide::AttachedFile f;
    EXPECT_EQ("", f.mime_type());
    f.mime_type("mime");
    EXPECT_EQ("mime", f.mime_type());
}


TEST(AttachedFile, Data)
{
    tide::AttachedFile f;
    EXPECT_TRUE(!f.data());
    tide::FileData::Ptr data(new tide::FileData(std::vector<char>()));
    EXPECT_THROW(f.data(data), tide::NoAttachedData);
    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tide::FileData::Ptr data2(new tide::FileData(*blob));
    f.data(data2);
    EXPECT_TRUE(*data2 == *f.data());
}


TEST(AttachedFile, UID)
{
    tide::AttachedFile f;
    EXPECT_EQ(1, f.uid());
    f.uid(42);
    EXPECT_EQ(42, f.uid());
    EXPECT_THROW(f.uid(0), tide::ValueOutOfRange);
}


TEST(AttachedFile, Size)
{
    tide::StringElement desc(tide::ids::FileDescription, "");
    tide::StringElement name(tide::ids::FileName, "");
    tide::StringElement mime(tide::ids::FileMimeType, "");
    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tide::FileData::Ptr data(new tide::FileData(*blob));
    tide::UIntElement uid(tide::ids::FileUID, 1);

    tide::AttachedFile f1;
    f1.data(data);
    std::streamsize body_size(name.size() + mime.size() + data->size() +
            uid.size());
    EXPECT_EQ(tide::ids::size(tide::ids::AttachedFile) +
            tide::vint::size(body_size) + body_size, f1.size());

    desc = "desc";
    name = "name";
    mime = "mime";
    uid = 42;

    tide::AttachedFile f2("name", "mime", data, 42);
    f2.description("desc");
    body_size = desc.size() + name.size() + mime.size() +
            data->size() + uid.size();
    EXPECT_EQ(tide::ids::size(tide::ids::AttachedFile) +
            tide::vint::size(body_size) + body_size, f2.size());
}


TEST(AttachedFile, Write)
{
    std::ostringstream output;
    std::stringstream expected;
    tide::StringElement desc(tide::ids::FileDescription, "");
    tide::StringElement name(tide::ids::FileName, "");
    tide::StringElement mime(tide::ids::FileMimeType, "");
    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tide::FileData::Ptr data(new tide::FileData(*blob));
    tide::UIntElement uid(tide::ids::FileUID, 21);

    tide::AttachedFile f1;
    f1.data(data);
    std::streamsize body_size(name.size() + mime.size() + data->size() +
            uid.size());
    tide::ids::write(tide::ids::AttachedFile, expected);
    tide::vint::write(body_size, expected);
    name.write(expected);
    mime.write(expected);
    data->write(expected);
    uid.write(expected);
    EXPECT_EQ(tide::ids::size(tide::ids::AttachedFile) +
            tide::vint::size(body_size) + body_size, f1.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            output.str());

    desc = "desc";
    name = "name";
    mime = "mime";
    uid = 42;

    expected.str(std::string());
    output.str(std::string());
    tide::AttachedFile f2("name", "mime", data, 42);
    f2.description("desc");
    body_size = desc.size() + name.size() + mime.size() + data->size() +
        uid.size();
    tide::ids::write(tide::ids::AttachedFile, expected);
    tide::vint::write(body_size, expected);
    desc.write(expected);
    name.write(expected);
    mime.write(expected);
    data->write(expected);
    uid.write(expected);
    EXPECT_EQ(tide::ids::size(tide::ids::AttachedFile) +
            tide::vint::size(body_size) + body_size, f2.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            output.str());
}


TEST(AttachedFile, Read)
{
    std::stringstream input;
    tide::StringElement desc(tide::ids::FileDescription, "");
    tide::StringElement name(tide::ids::FileName, "");
    tide::StringElement mime(tide::ids::FileMimeType, "");
    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tide::FileData::Ptr data(new tide::FileData(*blob));
    tide::UIntElement uid(tide::ids::FileUID, 21);

    tide::AttachedFile f;
    std::streamsize body_size(name.size() + mime.size() + data->size() +
            uid.size());
    tide::vint::write(body_size, input);
    name.write(input);
    mime.write(input);
    data->write(input);
    uid.write(input);
    EXPECT_EQ(tide::vint::size(body_size) + body_size, f.read(input));
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
    tide::vint::write(body_size, input);
    desc.write(input);
    name.write(input);
    mime.write(input);
    data->write(input);
    uid.write(input);
    EXPECT_EQ(tide::vint::size(body_size) + body_size, f.read(input));
    EXPECT_EQ("desc", f.description());
    EXPECT_EQ("name", f.name());
    EXPECT_EQ("mime", f.mime_type());
    EXPECT_TRUE(*data == *f.data());
    EXPECT_EQ(42, f.uid());

    // Body size value wrong (too small)
    input.str(std::string());
    tide::vint::write(2, input);
    name.write(input);
    mime.write(input);
    EXPECT_THROW(f.read(input), tide::BadBodySize);
    // Invalid child
    input.str(std::string());
    tide::UIntElement ue(tide::ids::EBML, 0xFFFF);
    tide::vint::write(ue.size(), input);
    ue.write(input);
    EXPECT_THROW(f.read(input), tide::InvalidChildID);
    // No name
    input.str(std::string());
    tide::vint::write(mime.size() + data->size() + uid.size(), input);
    mime.write(input);
    data->write(input);
    uid.write(input);
    EXPECT_THROW(f.read(input), tide::MissingChild);
    // No MIME type
    input.str(std::string());
    tide::vint::write(name.size() + data->size() + uid.size(), input);
    name.write(input);
    data->write(input);
    uid.write(input);
    EXPECT_THROW(f.read(input), tide::MissingChild);
    // No data
    input.str(std::string());
    tide::vint::write(name.size() + mime.size() + uid.size(), input);
    name.write(input);
    mime.write(input);
    uid.write(input);
    EXPECT_THROW(f.read(input), tide::MissingChild);
    // No UID
    input.str(std::string());
    tide::vint::write(name.size() + mime.size() + data->size(), input);
    name.write(input);
    mime.write(input);
    data->write(input);
    EXPECT_THROW(f.read(input), tide::MissingChild);
    // UID is zero
    input.str(std::string());
    uid = 0;
    tide::vint::write(uid.size(), input);
    uid.write(input);
    EXPECT_THROW(f.read(input), tide::ValueOutOfRange);
}


///////////////////////////////////////////////////////////////////////////////
// Attachments tests
///////////////////////////////////////////////////////////////////////////////

TEST(Attachments, Create)
{
    tide::Attachments a;
    EXPECT_TRUE(a.empty());
}


TEST(Attachments, Assignment)
{
    tide::Attachments a1;
    tide::Attachments a2;
    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tide::FileData::Ptr fd(new tide::FileData(*blob));
    tide::AttachedFile f("name", "mime", fd, 42);
    a1.push_back(f);

    EXPECT_TRUE(a2.empty());
    a2 = a1;
    EXPECT_FALSE(a2.empty());
    EXPECT_TRUE(a2[0].data() == fd);
    EXPECT_TRUE(*(a2[0].data()) == *fd);
}


TEST(Attachments, At)
{
    tide::Attachments a;
    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tide::FileData::Ptr fd(new tide::FileData(*blob));
    tide::AttachedFile f("name", "mime", fd, 42);
    a.push_back(f);
    EXPECT_TRUE(a[0] == a.at(0));
    EXPECT_TRUE(*a.at(0).data() == *fd);
    EXPECT_THROW(a.at(1), std::out_of_range);
}


TEST(Attachments, SubscriptOperator)
{
    tide::Attachments a;
    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tide::FileData::Ptr fd(new tide::FileData(*blob));
    tide::AttachedFile f("name", "mime", fd, 42);
    a.push_back(f);
    EXPECT_TRUE(a[0] == a.at(0));
    EXPECT_TRUE(*a[0].data() == *fd);
    EXPECT_NO_THROW(a[1]);

    boost::shared_ptr<std::vector<char> > blob2(test_utils::make_blob(5));
    tide::FileData::Ptr fd2(new tide::FileData(*blob));
    tide::AttachedFile f2("name2", "mime2", fd, 84);
    a[0] = f2;
    EXPECT_TRUE(a[0] == f2);
    EXPECT_TRUE(*a[0].data() == *fd2);
}


TEST(Attachments, BeginEnd)
{
    tide::Attachments a;
    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tide::FileData::Ptr fd(new tide::FileData(*blob));
    tide::AttachedFile f("name", "mime", fd, 42);

    EXPECT_TRUE(a.begin() == a.end());
    EXPECT_TRUE(a.rbegin() == a.rend());
    a.push_back(f);
    EXPECT_FALSE(a.begin() == a.end());
    EXPECT_FALSE(a.rbegin() == a.rend());
}


TEST(Attachments, Counts)
{
    tide::Attachments a;
    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tide::FileData::Ptr fd(new tide::FileData(*blob));
    tide::AttachedFile f("name", "mime", fd, 42);

    EXPECT_TRUE(a.empty());
    a.push_back(f);
    EXPECT_FALSE(a.empty());
    EXPECT_EQ(1, a.count());
}


TEST(Attachments, Clear)
{
    tide::Attachments a;
    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tide::FileData::Ptr fd(new tide::FileData(*blob));
    tide::AttachedFile f("name", "mime", fd, 42);
    a.push_back(f);
    EXPECT_FALSE(a.empty());
    a.clear();
    EXPECT_TRUE(a.empty());
}


TEST(Attachments, push_back)
{
    tide::Attachments a;
    boost::shared_ptr<std::vector<char> > blob1(test_utils::make_blob(5));
    tide::FileData::Ptr fd1(new tide::FileData(*blob1));
    tide::AttachedFile f1("name", "mime", fd1, 42);
    boost::shared_ptr<std::vector<char> > blob2(test_utils::make_blob(5));
    tide::FileData::Ptr fd2(new tide::FileData(*blob2));
    tide::AttachedFile f2("name", "mime", fd2, 42);

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
    tide::Attachments a;
    boost::shared_ptr<std::vector<char> > blob1(test_utils::make_blob(5));
    tide::FileData::Ptr fd1(new tide::FileData(*blob1));
    tide::AttachedFile f1("name", "mime", fd1, 42);
    boost::shared_ptr<std::vector<char> > blob2(test_utils::make_blob(5));
    tide::FileData::Ptr fd2(new tide::FileData(*blob2));
    tide::AttachedFile f2("name", "mime", fd2, 42);

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
    tide::Attachments a1, a2;
    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tide::FileData::Ptr fd(new tide::FileData(*blob));
    tide::AttachedFile f("name", "mime", fd, 42);
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
    tide::Attachments a1, a2;
    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tide::FileData::Ptr fd(new tide::FileData(*blob));
    tide::AttachedFile f("name", "mime", fd, 42);

    EXPECT_TRUE(a1 == a2);
    EXPECT_FALSE(a1 != a2);

    a1.push_back(f);
    EXPECT_FALSE(a1 == a2);
    EXPECT_TRUE(a1 != a2);
}


TEST(Attachments, Size)
{
    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tide::FileData::Ptr fd(new tide::FileData(*blob));
    tide::AttachedFile f("name", "mime", fd, 42);

    tide::Attachments a;
    a.push_back(f);
    EXPECT_EQ(tide::ids::size(tide::ids::Attachments) +
            tide::vint::size(f.size()) + f.size(), a.size());
}


TEST(Attachments, Write)
{
    std::ostringstream output;
    std::stringstream expected;
    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tide::FileData::Ptr fd(new tide::FileData(*blob));
    tide::AttachedFile f("name", "mime", fd, 42);
    tide::Attachments a;

    // No attached files
    EXPECT_THROW(a.write(output), tide::NoAttachments);

    output.str(std::string());
    a.push_back(f);
    std::streamsize body_size(f.size());
    tide::ids::write(tide::ids::Attachments, expected);
    tide::vint::write(body_size, expected);
    f.write(expected);
    EXPECT_EQ(tide::ids::size(tide::ids::Attachments) +
            tide::vint::size(body_size) + body_size, a.write(output));
    EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, expected.str(),
            output.str());
}


TEST(Attachments, Read)
{
    std::stringstream input;
    boost::shared_ptr<std::vector<char> > blob(test_utils::make_blob(5));
    tide::FileData::Ptr fd(new tide::FileData(*blob));
    tide::AttachedFile f("name", "mime", fd, 42);

    tide::Attachments a;
    std::streamsize body_size(f.size());
    tide::vint::write(body_size, input);
    f.write(input);
    EXPECT_EQ(tide::vint::size(body_size) + body_size, a.read(input));
    EXPECT_FALSE(a.empty());
    EXPECT_TRUE(*a[0].data() == *fd);

    // Body size value wrong (too small)
    input.str(std::string());
    tide::vint::write(2, input);
    f.write(input);
    EXPECT_THROW(a.read(input), tide::BadBodySize);
    // Invalid child
    input.str(std::string());
    tide::UIntElement ue(tide::ids::EBML, 0xFFFF);
    tide::vint::write(ue.size(), input);
    ue.write(input);
    EXPECT_THROW(a.read(input), tide::InvalidChildID);
    // No attachments
    input.str(std::string());
    tide::vint::write(0, input);
    EXPECT_THROW(a.read(input), tide::NoAttachments);
}

