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

#include "test_utilities.h"
#include <jonen/attached_file.h>
#include <jonen/binary_element.h>
#include <jonen/integer_elements.h>
#include <jonen/string_element.h>
#include <vector>

using namespace jonen;
using namespace test_utils;

namespace test_attached_file
{
    ///////////////////////////////////////////////////////////////////////////
    // Helper functions
    ///////////////////////////////////////////////////////////////////////////

    void populate_children_defaults(std::vector<ElPtr>& children)
    {
        children.push_back(ElPtr(new StringElement(ids::FileName, "a")));
        children.push_back(ElPtr(new StringElement(ids::FileMimeType, "a")));
        children.push_back(ElPtr(new BinaryElement(ids::FileData,
                        std::vector<char>())));
        children.push_back(ElPtr(new UIntElement(ids::FileUID, 1)));
    }

    void populate_children(std::vector<ElPtr>& children)
    {
        children.push_back(ElPtr(new StringElement(ids::FileDescription,
                        "desc")));
        children.push_back(ElPtr(new StringElement(ids::FileName, "name")));
        children.push_back(ElPtr(new StringElement(ids::FileMimeType, "mime")));
        children.push_back(ElPtr(new BinaryElement(ids::FileData,
                        std::vector<char>(42, 42))));
        children.push_back(ElPtr(new UIntElement(ids::FileUID, 42)));
    }

    void populate_element(AttachedFile& ee, std::vector<ElPtr> const& children)
    {
        using boost::dynamic_pointer_cast;

        int base = 0;
        if (children.size() == 5)
        {
            ee.description(*dynamic_pointer_cast<StringElement>(
                        children[0]));
            base = 1;
        }
        ee.name(*dynamic_pointer_cast<StringElement>(children[base + 0]));
        ee.mime_type(*dynamic_pointer_cast<StringElement>(children[base + 1]));
        ee.data(*dynamic_pointer_cast<BinaryElement>(children[base + 2]));
        ee.uid(*dynamic_pointer_cast<UIntElement>(children[base + 3]));
    }

    void check_children_match(AttachedFile const& ee,
            std::vector<ElPtr> const& children)
    {
        using boost::dynamic_pointer_cast;

        int base = 0;
        if (children.size() == 5)
        {
            EXPECT_PRED_FORMAT2(std_strings_eq,
                    dynamic_pointer_cast<StringElement>(children[0])->value(),
                    ee.description().value());
            base = 1;
        }
        EXPECT_PRED_FORMAT2(std_strings_eq,
                dynamic_pointer_cast<StringElement>(children[base +
                    0])->value(),
                ee.name().value());
        EXPECT_PRED_FORMAT2(std_strings_eq,
                dynamic_pointer_cast<StringElement>(children[base +
                    1])->value(),
                ee.mime_type().value());
        EXPECT_PRED_FORMAT2(std_vectors_eq,
                dynamic_pointer_cast<BinaryElement>(children[base +
                    2])->value(),
                ee.data().value());
        EXPECT_EQ(*dynamic_pointer_cast<UIntElement>(children[base + 3]),
                ee.uid());
    }


    ///////////////////////////////////////////////////////////////////////////
    // SegmentInto interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST(AttachedFile, Construction)
    {
        AttachedFile ee1("name", "mime", std::vector<char>(42, 42), 42);
        EXPECT_EQ(ids::AttachedFile, ee1.id());
        EXPECT_TRUE(ee1.description().empty());
        EXPECT_PRED_FORMAT2(std_strings_eq, "name", ee1.name().value());
        EXPECT_PRED_FORMAT2(std_strings_eq, "mime", ee1.mime_type().value());
        EXPECT_PRED_FORMAT2(std_vectors_eq, std::vector<char>(42, 42),
                ee1.data().value());
        EXPECT_EQ(42, ee1.uid());

        AttachedFile ee2("name", "mime", std::vector<char>(42, 42), 42,
                "desc");
        EXPECT_EQ(ids::AttachedFile, ee2.id());
        EXPECT_PRED_FORMAT2(std_strings_eq, "desc", ee2.description().value());
        EXPECT_PRED_FORMAT2(std_strings_eq, "name", ee2.name().value());
        EXPECT_PRED_FORMAT2(std_strings_eq, "mime", ee2.mime_type().value());
        EXPECT_PRED_FORMAT2(std_vectors_eq, std::vector<char>(42, 42),
                ee2.data().value());
        EXPECT_EQ(42, ee2.uid());

        EXPECT_THROW(AttachedFile("", "mime", std::vector<char>(), 1),
                ValueOutOfRange);
        EXPECT_THROW(AttachedFile("name", "", std::vector<char>(), 1),
                ValueOutOfRange);
        EXPECT_THROW(AttachedFile("name", "mime", std::vector<char>(), 0),
                ValueOutOfRange);
    }


    TEST(AttachedFile, CopyConstruction)
    {
        AttachedFile ee1("name", "mime", std::vector<char>(42, 42), 42,
                "desc");

        AttachedFile ee2(ee1);
        EXPECT_TRUE(ee1 == ee2);
    }


    TEST(AttachedFile, Assignment)
    {
        AttachedFile ee1("name", "mime", std::vector<char>(42, 42), 42,
                "desc");

        AttachedFile ee2("a", "a", std::vector<char>(), 1);
        EXPECT_FALSE(ee1 == ee2);
        ee2 = ee1;
        EXPECT_TRUE(ee1 == ee2);
    }


    TEST(AttachedFile, Swap)
    {
        AttachedFile ee1("name", "mime", std::vector<char>(42, 42), 42,
                "desc");
        AttachedFile ee2("a", "a", std::vector<char>(), 1);

        swap(ee1, ee2);

        EXPECT_TRUE(ee1.description().empty());
        EXPECT_PRED_FORMAT2(std_strings_eq, "a", ee1.name().value());
        EXPECT_PRED_FORMAT2(std_strings_eq, "a", ee1.mime_type().value());
        EXPECT_TRUE(ee1.data().empty());
        EXPECT_EQ(1, ee1.uid());

        EXPECT_PRED_FORMAT2(std_strings_eq, "desc", ee2.description().value());
        EXPECT_PRED_FORMAT2(std_strings_eq, "name", ee2.name().value());
        EXPECT_PRED_FORMAT2(std_strings_eq, "mime", ee2.mime_type().value());
        EXPECT_PRED_FORMAT2(std_vectors_eq, std::vector<char>(42, 42),
                ee2.data().value());
        EXPECT_EQ(42, ee2.uid());
    }


    TEST(AttachedFile, Equal)
    {
        AttachedFile ee1("a", "a", std::vector<char>(4, 1), 1, "a");
        AttachedFile ee2("b", "b", std::vector<char>(4, 2), 1, "b");
        AttachedFile ee3("a", "a", std::vector<char>(4, 2), 2, "a");

        EXPECT_TRUE(ee1 == ee2);
        EXPECT_FALSE(ee1 == ee3);
    }


    TEST(AttachedFile, LessThan)
    {
        AttachedFile ee1("a", "a", std::vector<char>(4, 2), 1, "a");
        AttachedFile ee2("a", "a", std::vector<char>(4, 2), 2, "a");

        EXPECT_TRUE(ee1 < ee2);
        EXPECT_FALSE(ee2 < ee1);
    }


    TEST(AttachedFile, LessThanEqual)
    {
        AttachedFile ee1("a", "a", std::vector<char>(4, 2), 1, "a");
        AttachedFile ee2("a", "a", std::vector<char>(4, 2), 1, "a");
        AttachedFile ee3("a", "a", std::vector<char>(4, 2), 2, "a");

        EXPECT_TRUE(ee1 <= ee2);
        EXPECT_TRUE(ee1 <= ee3);
        EXPECT_FALSE(ee3 <= ee2);
    }


    TEST(AttachedFile, NotEqual)
    {
        AttachedFile ee1("a", "a", std::vector<char>(4, 2), 1, "a");
        AttachedFile ee2("a", "a", std::vector<char>(4, 2), 1, "a");
        AttachedFile ee3("a", "a", std::vector<char>(4, 2), 2, "a");

        EXPECT_FALSE(ee1 != ee2);
        EXPECT_TRUE(ee1 != ee3);
    }


    TEST(AttachedFile, GreaterThanEqual)
    {
        AttachedFile ee1("a", "a", std::vector<char>(4, 2), 2, "a");
        AttachedFile ee2("a", "a", std::vector<char>(4, 2), 2, "a");
        AttachedFile ee3("a", "a", std::vector<char>(4, 2), 1, "a");

        EXPECT_TRUE(ee1 >= ee2);
        EXPECT_TRUE(ee1 >= ee3);
        EXPECT_FALSE(ee3 >= ee2);
    }


    TEST(AttachedFile, GreaterThan)
    {
        AttachedFile ee1("a", "a", std::vector<char>(4, 2), 2, "a");
        AttachedFile ee2("a", "a", std::vector<char>(4, 2), 1, "a");

        EXPECT_TRUE(ee1 > ee2);
        EXPECT_FALSE(ee2 > ee1);
    }


    TEST(AttachedFile, Description)
    {
        AttachedFile ee1("a", "a", std::vector<char>(), 1);

        EXPECT_TRUE(ee1.description().empty());
        ee1.description(StringElement(ids::Null, "element"));
        EXPECT_EQ(StringElement(ids::Null, "element"), ee1.description());
        ee1.description("string");
        EXPECT_PRED_FORMAT2(std_strings_eq, "string",
                ee1.description().value());

        AttachedFile ee2("a", "a", std::vector<char>(), 1, "ctor");
        EXPECT_PRED_FORMAT2(std_strings_eq, "ctor", ee2.description().value());
    }


    TEST(AttachedFile, Name)
    {
        AttachedFile ee("name", "a", std::vector<char>(), 1);

        EXPECT_EQ(StringElement(ids::Null, "name"), ee.name());
        ee.name(StringElement(ids::Null, "element"));
        EXPECT_PRED_FORMAT2(std_strings_eq, "element", ee.name().value());
        ee.name("string");
        EXPECT_PRED_FORMAT2(std_strings_eq, "string", ee.name().value());

        EXPECT_THROW(ee.name(""), ValueOutOfRange);
    }


    TEST(AttachedFile, MimeType)
    {
        AttachedFile ee("a", "mime", std::vector<char>(), 1);

        EXPECT_EQ(StringElement(ids::Null, "mime"), ee.mime_type());
        ee.mime_type(StringElement(ids::Null, "element"));
        EXPECT_PRED_FORMAT2(std_strings_eq, "element", ee.mime_type().value());
        ee.mime_type("string");
        EXPECT_PRED_FORMAT2(std_strings_eq, "string", ee.mime_type().value());

        EXPECT_THROW(ee.mime_type(""), ValueOutOfRange);
    }


    TEST(AttachedFile, Data)
    {
        AttachedFile ee("a", "a", std::vector<char>(), 1);

        EXPECT_TRUE(ee.data().empty());
        EXPECT_EQ(BinaryElement(ids::Null, std::vector<char>()), ee.data());
        ee.data(BinaryElement(ids::Null, std::vector<char>(10, 1)));
        EXPECT_PRED_FORMAT2(std_vectors_eq, std::vector<char>(10, 1),
                ee.data().value());
        ee.data(std::vector<char>(10, 2));
        EXPECT_PRED_FORMAT2(std_vectors_eq, std::vector<char>(10, 2),
                ee.data().value());
    }


    TEST(AttachedFile, UID)
    {
        AttachedFile ee("a", "a", std::vector<char>(), 1);

        EXPECT_EQ(UIntElement(ids::Null, 1), ee.uid());
        EXPECT_EQ(1, ee.uid());
        ee.uid(UIntElement(ids::Null, 2));
        EXPECT_EQ(2, ee.uid());
        ee.uid(3);
        EXPECT_EQ(3, ee.uid());
    }


    ///////////////////////////////////////////////////////////////////////////
    // MasterElement interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST(AttachedFile, EnableCRC)
    {
        AttachedFile ee("a", "a", std::vector<char>(), 1);
        EXPECT_TRUE(ee.crc_enabled());
        ee.disable_crc();
        EXPECT_FALSE(ee.crc_enabled());
        ee.enable_crc();
        EXPECT_TRUE(ee.crc_enabled());
    }


    ///////////////////////////////////////////////////////////////////////////
    // Element interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST(AttachedFile, ID)
    {
        AttachedFile ee("a", "a", std::vector<char>(), 1);
        EXPECT_EQ(ids::AttachedFile, ee.id());
    }


    TEST(AttachedFile, Offset)
    {
        AttachedFile ee("a", "a", std::vector<char>(), 1);
        EXPECT_EQ(0, ee.offset());

        std::stringstream ss;
        std::string input_val;

        // Set when read test
        IntElement ie(ids::Null, 42);
        write(ie, ss);
        std::vector<ElPtr> children;
        populate_children(children);
        fill_buffer(input_val, ids::AttachedFile, children, true, true, true);
        ss << input_val;
        ss.seekg(ie.stored_size() + ids::size(ids::AttachedFile));
        ee.read(ss);
        EXPECT_EQ(ie.stored_size(), ee.offset());

        // Set when written test
        write(ee, ss);
        EXPECT_EQ(ie.stored_size() + ee.stored_size(), ee.offset());
    }


    TEST(AttachedFile, StoredSize)
    {
        AttachedFile ee("a", "a", std::vector<char>(), 1);
        ee.disable_crc();

        std::vector<ElPtr> children;
        populate_children_defaults(children);

        // Basic size (no description, empty data)
        std::streamsize body_size(children_size(children));
        EXPECT_EQ(ids::size(ids::AttachedFile) +
                vint::size(body_size) + body_size, ee.stored_size());

        // Size with CRC enabled
        ee.enable_crc();
        body_size += 6;
        EXPECT_EQ(ids::size(ids::AttachedFile) +
                vint::size(body_size) + body_size, ee.stored_size());
        ee.disable_crc();

        // Size with all values set
        children.clear();
        populate_children(children);
        body_size = children_size(children);
        populate_element(ee, children);
        EXPECT_EQ(ids::size(ids::AttachedFile) +
                vint::size(body_size) + body_size, ee.stored_size());
    }


    TEST(AttachedFile, Read)
    {
        std::vector<ElPtr> children;

        // Reading an empty attachment without a description
        populate_children_defaults(children);
        std::string buffer;
        std::streamsize read_size = fill_buffer(buffer, ids::AttachedFile,
                children, false, true, true);
        std::stringstream input(buffer);
        AttachedFile ee("name", "mime", std::vector<char>(42, 42), 42, "desc");
        EXPECT_EQ(read_size, ee.read(input));
        check_children_match(ee, children);

        // Reading all values
        children.clear();
        std::string().swap(buffer);
        populate_children(children);
        read_size = fill_buffer(buffer, ids::AttachedFile, children, false,
                true, true);
        input.str(buffer);
        EXPECT_EQ(read_size, ee.read(input));
        check_children_match(ee, children);

        // Body size value wrong (too small)
        input.str(std::string());
        vint::write(2, input);
        write(*children[0], input);
        write(*children[3], input);
        EXPECT_THROW(ee.read(input), ReadError);
        // Post-condition test: error should leave element as before
        check_children_match(ee, children);

        // Invalid child
        input.str(std::string());
        UIntElement ue(ids::EBML, 0xFFFF);
        vint::write(ue.stored_size(), input);
        write(ue, input);
        EXPECT_THROW(ee.read(input), InvalidChildID);
        // Post-condition test: error should leave element as before
        check_children_match(ee, children);

        // No description should cause the current one to be emptied
        input.str(std::string());
        std::streamsize body_size = write_except(input, children, 0);
        input.str(std::string());
        vint::write(body_size, input);
        write_except(input, children, 0); // 0 = description
        EXPECT_FALSE(ee.description().empty());
        EXPECT_NO_THROW(read(ee, input));
        EXPECT_TRUE(ee.description().empty());

        // Missing children
        input.str(std::string());
        body_size = write_except(input, children, 1);
        input.str(std::string());
        vint::write(body_size, input);
        write_except(input, children, 1); // 1 = name
        EXPECT_THROW(read(ee, input), MissingChild);

        input.str(std::string());
        body_size = write_except(input, children, 2);
        input.str(std::string());
        vint::write(body_size, input);
        write_except(input, children, 2); // 2 = MIME type
        EXPECT_THROW(read(ee, input), MissingChild);

        input.str(std::string());
        body_size = write_except(input, children, 3);
        input.str(std::string());
        vint::write(body_size, input);
        write_except(input, children, 3); // 3 = data
        EXPECT_THROW(read(ee, input), MissingChild);

        input.str(std::string());
        body_size = write_except(input, children, 4);
        input.str(std::string());
        vint::write(body_size, input);
        write_except(input, children, 4); // 4 = UID
        EXPECT_THROW(read(ee, input), MissingChild);
    }


    TEST(AttachedFile, ReadWithCRC)
    {
        std::vector<ElPtr> children;
        populate_children(children);

        std::string buffer;
        std::streamsize read_size(0);
        read_size = fill_buffer(buffer, ids::AttachedFile, children, false, true, true,
                true);

        AttachedFile ee("a", "a", std::vector<char>(), 1);
        ee.disable_crc();
        std::stringstream input(buffer);
        EXPECT_EQ(read_size, read(ee, input));
        check_children_match(ee, children);

        // Bad CRC
        std::string().swap(buffer);
        fill_buffer(buffer, ids::AttachedFile, children, false, true, true, true);
        // Corrupt the CRC
        buffer[5] = 0x00;
        buffer[6] = 0x00;
        // Attempt to read
        input.str(buffer);
        EXPECT_THROW(read(ee, input), BadCRC);

        // Test automatic enabling/disabling of CRC
        ee.disable_crc();
        std::string().swap(buffer);
        fill_buffer(buffer, ids::AttachedFile, children, false, true, true, true);
        input.str(buffer);
        read(ee, input);
        EXPECT_TRUE(ee.crc_enabled());
        std::string().swap(buffer);
        fill_buffer(buffer, ids::AttachedFile, children, false, true, true, false);
        input.str(buffer);
        read(ee, input);
        EXPECT_FALSE(ee.crc_enabled());
    }


    TEST(AttachedFile, StartWrite)
    {
        std::stringstream output;
        AttachedFile ee("a", "a", std::vector<char>(), 1);
        ee.disable_crc();

        std::vector<ElPtr> children;
        populate_children_defaults(children);

        // Writing an empty attachment
        std::string expected;
        fill_buffer(expected, ids::AttachedFile, children, true, true, true);
        std::streamsize body_size(children_size(children));
        EXPECT_EQ(ids::size(ids::AttachedFile) + vint::size(body_size) +
                body_size, ee.start_write(output));
        EXPECT_PRED_FORMAT2(std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::AttachedFile) + vint::size(body_size) +
                body_size, output.tellp());

        // Write with everything set
        std::string().swap(expected);
        output.str(std::string());
        children.clear();
        populate_children(children);
        populate_element(ee, children);
        fill_buffer(expected, ids::AttachedFile, children, true, true, true);
        body_size = children_size(children);
        EXPECT_EQ(ids::size(ids::AttachedFile) + vint::size(body_size) +
                body_size, ee.start_write(output));
        EXPECT_PRED_FORMAT2(std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::AttachedFile) + vint::size(body_size) +
                body_size, output.tellp());
    }


    TEST(AttachedFile, FinishWrite)
    {
        std::stringstream output;
        AttachedFile ee("a", "a", std::vector<char>(), 1);
        ee.disable_crc();

        // Test for exception and no change to output when finishing without
        // starting
        EXPECT_THROW(ee.finish_write(output), NotWriting);
        EXPECT_EQ(0, output.tellp());

        std::vector<ElPtr> children;
        populate_children_defaults(children);

        // Writing with defaults should give a body with only a timecode scale.
        std::string expected;
        fill_buffer(expected, ids::AttachedFile, children, true, true, true);
        std::streamsize body_size(children_size(children));
        ee.start_write(output);
        EXPECT_EQ(ids::size(ids::AttachedFile) + vint::size(body_size) +
                body_size, ee.finish_write(output));
        EXPECT_PRED_FORMAT2(std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::AttachedFile) + vint::size(body_size) +
                body_size, output.tellp());

        // Write with everything set
        std::string().swap(expected);
        output.str(std::string());
        children.clear();
        populate_children(children);
        populate_element(ee, children);
        fill_buffer(expected, ids::AttachedFile, children, true, true, true);
        body_size = children_size(children);
        ee.start_write(output);
        EXPECT_EQ(ids::size(ids::AttachedFile) + vint::size(body_size) +
                body_size, ee.finish_write(output));
        EXPECT_PRED_FORMAT2(std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::AttachedFile) + vint::size(body_size) +
                body_size, output.tellp());

        // Test for double-finish raising an exception
        EXPECT_THROW(ee.finish_write(output), NotWriting);
    }


    TEST(AttachedFile, Write)
    {
        std::stringstream output;
        AttachedFile ee("a", "a", std::vector<char>(), 1);
        ee.disable_crc();

        std::vector<ElPtr> children;
        populate_children_defaults(children);

        // Writing with defaults should give a body with only a timecode scale.
        std::string expected;
        fill_buffer(expected, ids::AttachedFile, children, true, true, true);
        std::streamsize body_size(children_size(children));
        EXPECT_EQ(ids::size(ids::AttachedFile) + vint::size(body_size) +
                body_size, write(ee, output));
        EXPECT_PRED_FORMAT2(std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::AttachedFile) + vint::size(body_size) +
                body_size, output.tellp());

        // Write with everything set
        std::string().swap(expected);
        output.str(std::string());
        children.clear();
        populate_children(children);
        populate_element(ee, children);
        fill_buffer(expected, ids::AttachedFile, children, true, true, true);
        body_size = children_size(children);
        EXPECT_EQ(ids::size(ids::AttachedFile) + vint::size(body_size) +
                body_size, write(ee, output));
        EXPECT_PRED_FORMAT2(std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::AttachedFile) + vint::size(body_size) +
                body_size, output.tellp());
    }


    TEST(AttachedFile, WriteWithCRC)
    {
        std::stringstream output;
        std::string expected;
        AttachedFile ee("a", "a", std::vector<char>(), 1);

        std::vector<ElPtr> children;
        populate_children(children);

        ee.enable_crc();
        populate_element(ee, children);
        std::streamsize written_size(0);
        written_size = fill_buffer(expected, ids::AttachedFile, children, true,
                true, true, true);
        EXPECT_EQ(written_size, write(ee, output));
        EXPECT_PRED_FORMAT2(std_buffers_eq, output.str(), expected);
        // Post-condition test
        EXPECT_EQ(written_size, output.tellp());
    }


    TEST(AttachedFile, RoundTrip)
    {
        std::vector<ElPtr> children;
        populate_children(children);

        AttachedFile ee1("a", "a", std::vector<char>(), 1);
        populate_element(ee1, children);

        std::stringstream io;
        std::streamsize written_bytes = write(ee1, io);
        // Skip the ID
        io.seekg(ids::size(ids::AttachedFile), std::ios::beg);
        AttachedFile ee2("a", "a", std::vector<char>(), 1);
        std::streamsize read_bytes = read(ee2, io);

        check_children_match(ee2, children);
        EXPECT_EQ(written_bytes - ids::size(ids::AttachedFile), read_bytes);
    }
}; // namespace test_attached_file

