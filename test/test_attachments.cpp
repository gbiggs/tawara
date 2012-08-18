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
#include <boost/foreach.hpp>
#include <jonen/attached_file.h>
#include <jonen/attachments.h>
#include <vector>

using namespace jonen;
using namespace test_utils;

namespace test_attachments
{
    ///////////////////////////////////////////////////////////////////////////
    // Helper functions
    ///////////////////////////////////////////////////////////////////////////

    void populate_children(std::vector<ElPtr>& children)
    {
        children.push_back(ElPtr(new AttachedFile("a", "a",
                        std::vector<char>(4, 1), 1, "a")));
        children.push_back(ElPtr(new AttachedFile("b", "b",
                        std::vector<char>(4, 2), 2, "b")));
        children.push_back(ElPtr(new AttachedFile("c", "c",
                        std::vector<char>(4, 3), 3, "c")));
    }

    void populate_element(Attachments& ee, std::vector<ElPtr> const& children)
    {
        using boost::dynamic_pointer_cast;

        BOOST_FOREACH(ElPtr const& v, children)
        {
            ee.push_back(*dynamic_pointer_cast<AttachedFile>(v));
        }
    }

    void check_children_match(Attachments const& ee,
            std::vector<ElPtr> const& children)
    {
        using boost::dynamic_pointer_cast;

        EXPECT_TRUE(ee.size() == children.size());
        Attachments::const_iterator aa(ee.begin());
        for(int ii(0); ii < children.size(); ++aa, ++ii)
        {
            boost::shared_ptr<AttachedFile> af(
                    dynamic_pointer_cast<AttachedFile>(children[ii]));
            EXPECT_EQ(*af, *aa);
            EXPECT_EQ(af->description(), aa->description());
            EXPECT_EQ(af->name(), aa->name());
            EXPECT_EQ(af->mime_type(), aa->mime_type());
            EXPECT_EQ(af->data(), aa->data());
            EXPECT_EQ(af->uid(), aa->uid());
        }
    }


    ///////////////////////////////////////////////////////////////////////////
    // Attachments interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST(Attachments, Construction)
    {
        Attachments ee;
        EXPECT_EQ(ids::Attachments, ee.id());
        EXPECT_EQ(ee.begin(), ee.end());
        EXPECT_TRUE(ee.empty());
        EXPECT_EQ(0, ee.size());
    }


    TEST(Attachments, CopyConstruction)
    {
        Attachments ee1;
        std::vector<ElPtr> children;
        populate_children(children);
        populate_element(ee1, children);

        Attachments ee2(ee1);
        EXPECT_TRUE(ee1 == ee2);
    }


    TEST(Attachments, Assignment)
    {
        Attachments ee1;
        std::vector<ElPtr> children;
        populate_children(children);
        populate_element(ee1, children);

        Attachments ee2;
        EXPECT_FALSE(ee1 == ee2);
        ee2 = ee1;
        EXPECT_TRUE(ee1 == ee2);
    }


    TEST(Attachments, Swap)
    {
        Attachments ee1;
        std::vector<ElPtr> children;
        populate_children(children);
        populate_element(ee1, children);

        Attachments ee2;
        swap(ee1, ee2);

        EXPECT_TRUE(ee1.empty());
        check_children_match(ee2, children);
    }


    ///////////////////////////////////////////////////////////////////////////
    // List interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST(Attachments, GetAllocator)
    {
        // TODO: How can this be tested?
    }


    TEST(Attachments, Equal)
    {
        Attachments ee1;
        ee1.push_back(AttachedFile("a", "a", std::vector<char>(), 1));
        Attachments ee2;
        ee2.push_back(AttachedFile("a", "a", std::vector<char>(), 1));
        Attachments ee3;
        ee3.push_back(AttachedFile("a", "a", std::vector<char>(), 2));

        EXPECT_TRUE(ee1 == ee2);
        EXPECT_FALSE(ee1 == ee3);
    }


    TEST(Attachments, LessThan)
    {
        Attachments ee1;
        ee1.push_back(AttachedFile("a", "a", std::vector<char>(), 1));
        Attachments ee2;
        ee2.push_back(AttachedFile("a", "a", std::vector<char>(), 2));

        EXPECT_TRUE(ee1 < ee2);
        EXPECT_FALSE(ee2 < ee1);
    }


    TEST(Attachments, LessThanEqual)
    {
        Attachments ee1;
        ee1.push_back(AttachedFile("a", "a", std::vector<char>(), 1));
        Attachments ee2;
        ee2.push_back(AttachedFile("a", "a", std::vector<char>(), 1));
        Attachments ee3;
        ee3.push_back(AttachedFile("a", "a", std::vector<char>(), 2));

        EXPECT_TRUE(ee1 <= ee2);
        EXPECT_TRUE(ee1 <= ee3);
        EXPECT_FALSE(ee3 <= ee2);
    }


    TEST(Attachments, NotEqual)
    {
        Attachments ee1;
        ee1.push_back(AttachedFile("a", "a", std::vector<char>(), 1));
        Attachments ee2;
        ee2.push_back(AttachedFile("a", "a", std::vector<char>(), 1));
        Attachments ee3;
        ee3.push_back(AttachedFile("a", "a", std::vector<char>(), 2));

        EXPECT_FALSE(ee1 != ee2);
        EXPECT_TRUE(ee1 != ee3);
    }


    TEST(Attachments, GreaterThanEqual)
    {
        Attachments ee1;
        ee1.push_back(AttachedFile("a", "a", std::vector<char>(), 2));
        Attachments ee2;
        ee2.push_back(AttachedFile("a", "a", std::vector<char>(), 2));
        Attachments ee3;
        ee3.push_back(AttachedFile("a", "a", std::vector<char>(), 1));

        EXPECT_TRUE(ee1 >= ee2);
        EXPECT_TRUE(ee1 >= ee3);
        EXPECT_FALSE(ee3 >= ee2);
    }


    TEST(Attachments, GreaterThan)
    {
        Attachments ee1;
        ee1.push_back(AttachedFile("a", "a", std::vector<char>(), 2));
        Attachments ee2;
        ee2.push_back(AttachedFile("a", "a", std::vector<char>(), 1));

        EXPECT_TRUE(ee1 > ee2);
        EXPECT_FALSE(ee2 > ee1);
    }


    TEST(Attachments, Front)
    {
        using boost::dynamic_pointer_cast;

        std::vector<ElPtr> children;
        populate_children(children);
        Attachments ee;
        populate_element(ee, children);

        EXPECT_EQ(dynamic_pointer_cast<AttachedFile>(children[0])->uid(),
                ee.front().uid());
    }


    TEST(Attachments, Back)
    {
        using boost::dynamic_pointer_cast;

        std::vector<ElPtr> children;
        populate_children(children);
        Attachments ee;
        populate_element(ee, children);

        EXPECT_EQ(dynamic_pointer_cast<AttachedFile>(children[2])->uid(),
                ee.back().uid());
    }


    TEST(Attachments, Begin)
    {
        using boost::dynamic_pointer_cast;

        std::vector<ElPtr> children;
        populate_children(children);
        Attachments ee;
        populate_element(ee, children);

        EXPECT_EQ(dynamic_pointer_cast<AttachedFile>(children[0])->uid(),
            ee.begin()->uid());
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        EXPECT_EQ(dynamic_pointer_cast<AttachedFile>(children[0])->uid(),
            ee.cbegin()->uid());
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
        ee.begin()->uid(4);
        EXPECT_EQ(4, ee.begin()->uid());
        Attachments const ee_const(ee);
        EXPECT_EQ(4, ee_const.begin()->uid());
    }


    TEST(Attachments, End)
    {
        using boost::dynamic_pointer_cast;

        std::vector<ElPtr> children;
        populate_children(children);
        Attachments ee;
        populate_element(ee, children);

        EXPECT_EQ(dynamic_pointer_cast<AttachedFile>(children[2])->uid(),
            (--ee.end())->uid());
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        EXPECT_EQ(dynamic_pointer_cast<AttachedFile>(children[2])->uid(),
            (--ee.cend())->uid());
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
        (--ee.end())->uid(4);
        EXPECT_EQ(4, (--ee.end())->uid());
        Attachments const ee_const(ee);
        EXPECT_EQ(4, (--ee_const.end())->uid());
    }


    TEST(Attachments, RBegin)
    {
        using boost::dynamic_pointer_cast;

        std::vector<ElPtr> children;
        populate_children(children);
        Attachments ee;
        populate_element(ee, children);

        EXPECT_EQ(dynamic_pointer_cast<AttachedFile>(children[2])->uid(),
            ee.rbegin()->uid());
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        EXPECT_EQ(dynamic_pointer_cast<AttachedFile>(children[2])->uid(),
            ee.crbegin()->uid());
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
        ee.rbegin()->uid(4);
        EXPECT_EQ(4, ee.rbegin()->uid());
        Attachments const ee_const(ee);
        EXPECT_EQ(4, ee_const.rbegin()->uid());
    }


    TEST(Attachments, REnd)
    {
        using boost::dynamic_pointer_cast;

        std::vector<ElPtr> children;
        populate_children(children);
        Attachments ee;
        populate_element(ee, children);

        EXPECT_EQ(dynamic_pointer_cast<AttachedFile>(children[0])->uid(),
            (--ee.rend())->uid());
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        EXPECT_EQ(dynamic_pointer_cast<AttachedFile>(children[0])->uid(),
            (--ee.crend())->uid());
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
        (--ee.rend())->uid(4);
        EXPECT_EQ(4, (--ee.rend())->uid());
        Attachments const ee_const(ee);
        EXPECT_EQ(4, (--ee_const.rend())->uid());
    }


    TEST(Attachments, Empty)
    {
        Attachments ee;
        EXPECT_TRUE(ee.empty());

        std::vector<ElPtr> children;
        populate_children(children);
        populate_element(ee, children);
        EXPECT_FALSE(ee.empty());
    }


    TEST(Attachments, Size)
    {
        Attachments ee;
        EXPECT_EQ(0, ee.size());

        std::vector<ElPtr> children;
        populate_children(children);
        populate_element(ee, children);
        EXPECT_EQ(children.size(), ee.size());
    }


    TEST(Attachments, MaxSize)
    {
        // This value differs based on platform and standard library
        // implementation, so the best that can be done is test that the
        // underlying list's value is being passed correctly.
        Attachments ee;
        std::list<AttachedFile> list;
        EXPECT_EQ(list.max_size(), ee.max_size());
    }


    TEST(Attachments, Clear)
    {
        std::vector<ElPtr> children;
        populate_children(children);
        Attachments ee;
        populate_element(ee, children);
        EXPECT_FALSE(ee.empty());
        ee.clear();
        EXPECT_TRUE(ee.empty());
    }


    TEST(Attachments, Insert)
    {
        Attachments ee;
        AttachedFile af("a", "a", std::vector<char>(), 1);
        // Non-move version in C++11
        ee.insert(ee.begin(), af);
        EXPECT_EQ(1, ee.size());
        EXPECT_EQ(1, ee.begin()->uid());
        // Move version in C++11
        ee.insert(ee.begin(), AttachedFile("a", "a", std::vector<char>(), 2));
        EXPECT_EQ(2, ee.size());
        EXPECT_EQ(2, ee.begin()->uid());

        ee.clear();
        ee.insert(ee.begin(), 3, AttachedFile("a", "a", std::vector<char>(),
                    1));
        EXPECT_EQ(3, ee.size());
        EXPECT_EQ(1, ee.begin()->uid());

        std::vector<ElPtr> children;
        populate_children(children);
        Attachments ee2;
        populate_element(ee2, children);
        ee.clear();
        ee.insert(ee.begin(), ee2.begin(), ee2.end());

#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        ee.clear();
        ee.insert({AttachedFile("a", "a", std::vector<char>(), 1),
                AttachedFile("a", "a", std::vector<char>(), 2),
                AttachedFile("a", "a", std::vector<char>(), 3)});
        EXPECT_EQ(3, ee.size());
        EXPECT_EQ(1, ee.begin()->uid());
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
    }


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
    TEST(Attachments, Emplace)
    {
        Attachments ee;
        ee.emplace(ee.begin(), "a", "a", std::vector<char>(), 1);
        EXPECT_EQ(1, ee.size());
        EXPECT_EQ(1, ee.begin()->uid());
    }
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


    TEST(Attachments, erase)
    {
        using boost::dynamic_pointer_cast;

        std::vector<ElPtr> children;
        populate_children(children);
        Attachments ee;
        populate_element(ee, children);
        EXPECT_EQ(children.size(), ee.size());
        EXPECT_EQ(dynamic_pointer_cast<AttachedFile>(children[1])->uid(),
            (++ee.begin())->uid());
        ee.erase(++ee.begin());
        EXPECT_EQ(children.size() - 1, ee.size());
        EXPECT_EQ(dynamic_pointer_cast<AttachedFile>(children[2])->uid(),
            (++ee.begin())->uid());

        ee.clear();
        populate_element(ee, children);
        ee.erase(ee.begin(), ++(++ee.begin()));
        EXPECT_EQ(children.size() - 2, ee.size());
        EXPECT_EQ(dynamic_pointer_cast<AttachedFile>(children[2])->uid(),
            ee.begin()->uid());
    }


    TEST(Attachments, PushBack)
    {
        std::vector<ElPtr> children;
        populate_children(children);
        Attachments ee;
        populate_element(ee, children);

        EXPECT_EQ(children.size(), ee.size());
        AttachedFile af("a", "a", std::vector<char>(), 42);
        ee.push_back(af);
        EXPECT_EQ(children.size() + 1, ee.size());
        EXPECT_EQ(42, (--ee.end())->uid());

        // Move version under C++11
        ee.push_back(AttachedFile("a", "a", std::vector<char>(), 24));
        EXPECT_EQ(children.size() + 2, ee.size());
        EXPECT_EQ(24, (--ee.end())->uid());
    }


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
    TEST(Attachements, EmplaceBack)
    {
        std::vector<ElPtr> children;
        populate_children(children);
        Attachments ee;
        populate_element(ee, children);

        ee.emplace_back("a", "a", std::vector<char>(), 42);
        EXPECT_EQ(children.size() + 1, ee.size());
        EXPECT_EQ(42, (--ee.end())->uid());
    }
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


    TEST(Attachments, PopBack)
    {
        std::vector<ElPtr> children;
        populate_children(children);
        Attachments ee;
        populate_element(ee, children);

        EXPECT_EQ(children.size(), ee.size());
        ee.pop_back();
        EXPECT_EQ(children.size() - 1, ee.size());
        EXPECT_EQ(2, (--ee.end())->uid());
    }


    TEST(Attachments, PushFront)
    {
        std::vector<ElPtr> children;
        populate_children(children);
        Attachments ee;
        populate_element(ee, children);

        EXPECT_EQ(children.size(), ee.size());
        AttachedFile af("a", "a", std::vector<char>(), 42);
        ee.push_front(af);
        EXPECT_EQ(children.size() + 1, ee.size());
        EXPECT_EQ(42, ee.begin()->uid());

        // Move version under C++11
        ee.push_front(AttachedFile("a", "a", std::vector<char>(), 24));
        EXPECT_EQ(children.size() + 2, ee.size());
        EXPECT_EQ(24, ee.begin()->uid());
    }


#if defined(JONEN_CPLUSPLUS11_SUPPORT)
    TEST(Attachements, EmplaceFront)
    {
        std::vector<ElPtr> children;
        populate_children(children);
        Attachments ee;
        populate_element(ee, children);

        ee.emplace_front("a", "a", std::vector<char>(), 42);
        EXPECT_EQ(children.size() + 1, ee.size());
        EXPECT_EQ(42, ee.begin()->uid());
    }
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)


    TEST(Attachments, PopFront)
    {
        std::vector<ElPtr> children;
        populate_children(children);
        Attachments ee;
        populate_element(ee, children);

        EXPECT_EQ(children.size(), ee.size());
        ee.pop_front();
        EXPECT_EQ(children.size() - 1, ee.size());
        EXPECT_EQ(2, ee.begin()->uid());
    }


    TEST(Attachments, Resize)
    {
        std::vector<ElPtr> children;
        populate_children(children);
        Attachments ee;
        populate_element(ee, children);

        EXPECT_EQ(children.size(), ee.size());
        ee.resize(4, AttachedFile("a", "a", std::vector<char>(), 4));
        EXPECT_EQ(children.size() + 1, ee.size());
        EXPECT_EQ(4, (--ee.end())->uid());
        ee.resize(3, AttachedFile("a", "a", std::vector<char>(), 4));
        EXPECT_EQ(children.size(), ee.size());
        EXPECT_EQ(3, (--ee.end())->uid());

#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        ee.resize(2);
        EXPECT_EQ(children.size() - 1, ee.size());
        EXPECT_EQ(2, (--ee.end())->uid());
        ee.resize(3, AttachedFile("c", "c", std::vector<char>(4, 3), 3));
        EXPECT_EQ(children.size(), ee.size());
        EXPECT_EQ(3, (--ee.end())->uid());
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
    }


    bool MergeTestCmp(AttachedFile const& lhs, AttachedFile const& rhs)
    {
        return lhs < rhs;
    }


    TEST(Attachments, Merge)
    {
        std::vector<ElPtr> children;
        populate_children(children);
        Attachments ee1;
        ee1.push_back(AttachedFile("a", "a", std::vector<char>(4, 1), 1));
        ee1.push_back(AttachedFile("c", "c", std::vector<char>(4, 3), 3));
        ee1.push_back(AttachedFile("e", "e", std::vector<char>(4, 5), 5));
        Attachments ee2;
        ee2.push_back(AttachedFile("b", "b", std::vector<char>(4, 2), 2));
        ee2.push_back(AttachedFile("d", "d", std::vector<char>(4, 4), 4));

        ee1.merge(ee2);
        EXPECT_EQ(5, ee1.size());
        EXPECT_EQ(1, ee1.begin()->uid());
        EXPECT_EQ(2, (++ee1.begin())->uid());
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        ee1.clear();
        ee1.push_back(AttachedFile("a", "a", std::vector<char>(4, 1), 1));
        ee1.push_back(AttachedFile("c", "c", std::vector<char>(4, 3), 3));
        ee1.push_back(AttachedFile("e", "e", std::vector<char>(4, 5), 5));
        ee1.emerge(std::move(ee2));
        EXPECT_EQ(5, ee1.size());
        EXPECT_EQ(1, ee1.begin()->uid());
        EXPECT_EQ(2, (++ee1.begin())->uid());
        ee2.clear();
        ee2.push_back(AttachedFile("b", "b", std::vector<char>(4, 2), 2));
        ee2.push_back(AttachedFile("d", "d", std::vector<char>(4, 4), 4));
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)

        /* TODO Fix the interface so this test will compile
        ee1.merge(ee2, MergeTestCmp);
        EXPECT_EQ(5, ee1.size());
        EXPECT_EQ(1, ee1.begin()->uid());
        EXPECT_EQ(2, ee1.begin()->uid());
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        ee1.clear();
        ee1.push_back(AttachedFile("a", "a", std::vector<char>(4, 1), 1));
        ee1.push_back(AttachedFile("c", "c", std::vector<char>(4, 3), 3));
        ee1.push_back(AttachedFile("e", "e", std::vector<char>(4, 5), 5));
        ee1.emerge(std::move(ee2), MergeTestCmp);
        EXPECT_EQ(5, ee1.size());
        EXPECT_EQ(1, ee1.begin()->uid());
        EXPECT_EQ(2, ee1.begin()->uid());
        ee2.clear();
        ee2.push_back(AttachedFile("b", "b", std::vector<char>(4, 2), 2));
        ee2.push_back(AttachedFile("d", "d", std::vector<char>(4, 4), 4));
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)*/
    }


    TEST(Attachments, Splice)
    {
        Attachments ee1;
        ee1.push_back(AttachedFile("a", "a", std::vector<char>(4, 1), 1));
        ee1.push_back(AttachedFile("c", "c", std::vector<char>(4, 3), 3));
        ee1.push_back(AttachedFile("e", "e", std::vector<char>(4, 5), 5));
        Attachments ee2;
        ee2.push_back(AttachedFile("b", "b", std::vector<char>(4, 2), 2));
        ee2.push_back(AttachedFile("d", "d", std::vector<char>(4, 4), 4));

        ee1.splice(ee1.begin(), ee2);
        EXPECT_EQ(5, ee1.size());
        EXPECT_EQ(2, ee1.begin()->uid());
        EXPECT_EQ(4, (++ee1.begin())->uid());
        EXPECT_EQ(1, (++(++ee1.begin()))->uid());
        EXPECT_TRUE(ee2.empty());

#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        ee1.clear();
        ee2.clear();
        ee1.push_back(AttachedFile("a", "a", std::vector<char>(4, 1), 1));
        ee1.push_back(AttachedFile("c", "c", std::vector<char>(4, 3), 3));
        ee1.push_back(AttachedFile("e", "e", std::vector<char>(4, 5), 5));
        ee2.push_back(AttachedFile("b", "b", std::vector<char>(4, 2), 2));
        ee2.push_back(AttachedFile("d", "d", std::vector<char>(4, 4), 4));
        ee1.splice(ee1.begin(), std::move(ee2));
        EXPECT_EQ(5, ee1.size());
        EXPECT_EQ(2, ee1.begin()->uid());
        EXPECT_EQ(4, (++ee1.begin())->uid());
        EXPECT_EQ(1, (++(++ee1.begin()))->uid());
        EXPECT_TRUE(ee2.empty());
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)

        ee1.clear();
        ee2.clear();
        ee1.push_back(AttachedFile("a", "a", std::vector<char>(4, 1), 1));
        ee1.push_back(AttachedFile("c", "c", std::vector<char>(4, 3), 3));
        ee1.push_back(AttachedFile("e", "e", std::vector<char>(4, 5), 5));
        ee2.push_back(AttachedFile("b", "b", std::vector<char>(4, 2), 2));
        ee2.push_back(AttachedFile("d", "d", std::vector<char>(4, 4), 4));
        ee1.splice(ee1.begin(), ee2, ++ee2.begin());
        EXPECT_EQ(4, ee1.size());
        EXPECT_EQ(4, ee1.begin()->uid());
        EXPECT_EQ(1, (++ee1.begin())->uid());
        EXPECT_EQ(1, ee2.size());

#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        ee1.clear();
        ee2.clear();
        ee1.push_back(AttachedFile("a", "a", std::vector<char>(4, 1), 1));
        ee1.push_back(AttachedFile("c", "c", std::vector<char>(4, 3), 3));
        ee1.push_back(AttachedFile("e", "e", std::vector<char>(4, 5), 5));
        ee2.push_back(AttachedFile("b", "b", std::vector<char>(4, 2), 2));
        ee2.push_back(AttachedFile("d", "d", std::vector<char>(4, 4), 4));
        ee1.splice(ee1.begin(), std::move(ee2), ++ee2.begin());
        EXPECT_EQ(4, ee1.size());
        EXPECT_EQ(4, ee1.begin()->uid());
        EXPECT_EQ(1, (++ee1.begin())->uid());
        EXPECT_EQ(1, ee2.size());
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)

        ee1.clear();
        ee2.clear();
        ee1.push_back(AttachedFile("a", "a", std::vector<char>(4, 1), 1));
        ee1.push_back(AttachedFile("c", "c", std::vector<char>(4, 3), 3));
        ee1.push_back(AttachedFile("e", "e", std::vector<char>(4, 5), 5));
        ee2.push_back(AttachedFile("b", "b", std::vector<char>(4, 2), 2));
        ee2.push_back(AttachedFile("d", "d", std::vector<char>(4, 4), 4));
        ee1.splice(ee1.begin(), ee2, ee2.begin(), ee2.end());
        EXPECT_EQ(5, ee1.size());
        EXPECT_EQ(2, ee1.begin()->uid());
        EXPECT_EQ(4, (++ee1.begin())->uid());
        EXPECT_EQ(1, (++(++ee1.begin()))->uid());
        EXPECT_TRUE(ee2.empty());
#if defined(JONEN_CPLUSPLUS11_SUPPORT)
        ee1.clear();
        ee2.clear();
        ee1.push_back(AttachedFile("a", "a", std::vector<char>(4, 1), 1));
        ee1.push_back(AttachedFile("c", "c", std::vector<char>(4, 3), 3));
        ee1.push_back(AttachedFile("e", "e", std::vector<char>(4, 5), 5));
        ee2.push_back(AttachedFile("b", "b", std::vector<char>(4, 2), 2));
        ee2.push_back(AttachedFile("d", "d", std::vector<char>(4, 4), 4));
        ee1.splice(ee1.begin(), std::move(ee2), ee2.begin(), ee2.end());
        EXPECT_EQ(5, ee1.size());
        EXPECT_EQ(2, ee1.begin()->uid());
        EXPECT_EQ(4, (++ee1.begin())->uid());
        EXPECT_EQ(1, (++(++ee1.begin()))->uid());
        EXPECT_TRUE(ee2.empty());
#endif // defined(JONEN_CPLUSPLUS11_SUPPORT)
    }


    TEST(Attachments, Remove)
    {
        using boost::dynamic_pointer_cast;

        std::vector<ElPtr> children;
        populate_children(children);
        Attachments ee;
        populate_element(ee, children);

        ee.remove(*dynamic_pointer_cast<AttachedFile>(children[0]));
        EXPECT_EQ(children.size() - 1, ee.size());
        EXPECT_EQ(dynamic_pointer_cast<AttachedFile>(children[1])->uid(),
                ee.begin()->uid());
    }


    bool RemoveIfPred(AttachedFile const& a)
    {
        if (a.uid() == 1)
        {
            return true;
        }
        return false;
    }


    TEST(Attachments, RemoveIf)
    {
        /* TODO Fix the interface so this test will compile
        using boost::dynamic_pointer_cast;

        std::vector<ElPtr> children;
        populate_children(children);
        Attachments ee;
        populate_element(ee, children);

        ee.remove_if(RemoveIfPred);
        EXPECT_EQ(children.size() - 1, ee.size());
        EXPECT_EQ(dynamic_pointer_cast<AttachedFile>(children[2])->uid(),
                ee.begin()->uid());*/
    }


    TEST(Attachments, Reverse)
    {
        using boost::dynamic_pointer_cast;

        std::vector<ElPtr> children;
        populate_children(children);
        Attachments ee;
        populate_element(ee, children);

        ee.reverse();
        EXPECT_EQ(dynamic_pointer_cast<AttachedFile>(children[2])->uid(),
                ee.begin()->uid());
    }


    bool EqualPred(AttachedFile const& a, AttachedFile const& b)
    {
        return a == b;
    }


    TEST(Attachments, Unique)
    {
        Attachments ee;
        ee.push_back(AttachedFile("a", "a", std::vector<char>(4, 1), 1));
        ee.push_back(AttachedFile("b", "b", std::vector<char>(4, 2), 2));
        ee.push_back(AttachedFile("b", "b", std::vector<char>(4, 2), 2));
        ee.push_back(AttachedFile("b", "b", std::vector<char>(4, 2), 2));
        ee.push_back(AttachedFile("c", "c", std::vector<char>(4, 3), 3));

        EXPECT_EQ(5, ee.size());
        ee.unique();
        EXPECT_EQ(3, ee.size());
        EXPECT_EQ(3, (++(++ee.begin()))->uid());

        /* TODO Fix the interface so this test will compile
        ee.insert(++ee.begin(),
                AttachedFile("b", "b", std::vector<char>(4, 2), 2));
        ee.insert(++ee.begin(),
                AttachedFile("b", "b", std::vector<char>(4, 2), 2));
        EXPECT_EQ(5, ee.size());
        ee.unique(EqualPred);
        EXPECT_EQ(3, ee.size());
        EXPECT_EQ(3, (++(++ee.begin()))->uid());*/
    }


    TEST(Attachments, Sort)
    {
        Attachments ee;
        ee.push_back(AttachedFile("c", "c", std::vector<char>(4, 3), 3));
        ee.push_back(AttachedFile("d", "d", std::vector<char>(4, 4), 4));
        ee.push_back(AttachedFile("b", "b", std::vector<char>(4, 2), 2));
        ee.push_back(AttachedFile("a", "a", std::vector<char>(4, 1), 1));

        ee.sort();
        EXPECT_EQ(1, ee.begin()->uid());
        EXPECT_EQ(2, (++ee.begin())->uid());
        EXPECT_EQ(3, (++(++ee.begin()))->uid());
        EXPECT_EQ(4, (++(++(++ee.begin())))->uid());

        /* TODO Fix the interface so this test will compile
        ee.clear();
        ee.push_back(AttachedFile("c", "c", std::vector<char>(4, 3), 3));
        ee.push_back(AttachedFile("d", "d", std::vector<char>(4, 4), 4));
        ee.push_back(AttachedFile("b", "b", std::vector<char>(4, 2), 2));
        ee.push_back(AttachedFile("a", "a", std::vector<char>(4, 1), 1));
        ee.sort(EqualPred);
        EXPECT_EQ(1, ee.begin()->uid());
        EXPECT_EQ(2, (++ee.begin())->uid());
        EXPECT_EQ(3, (++(++ee.begin()))->uid());
        EXPECT_EQ(4, (++(++(++ee.begin())))->uid());*/
    }


    ///////////////////////////////////////////////////////////////////////////
    // MasterElement interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST(Attachments, EnableCRC)
    {
        Attachments ee;
        EXPECT_TRUE(ee.crc_enabled());
        ee.disable_crc();
        EXPECT_FALSE(ee.crc_enabled());
        ee.enable_crc();
        EXPECT_TRUE(ee.crc_enabled());
    }


    ///////////////////////////////////////////////////////////////////////////
    // Element interface tests
    ///////////////////////////////////////////////////////////////////////////

    TEST(Attachments, ID)
    {
        Attachments ee;
        EXPECT_EQ(ids::Attachments, ee.id());
    }


    TEST(Attachments, Offset)
    {
        Attachments ee;
        EXPECT_EQ(0, ee.offset());

        std::stringstream ss;
        std::string input_val;

        // Set when read test
        IntElement ie(ids::Null, 42);
        write(ie, ss);
        std::vector<ElPtr> children;
        populate_children(children);
        fill_buffer(input_val, ids::Attachments, children, true, true, true);
        ss << input_val;
        ss.seekg(ie.stored_size() + ids::size(ids::Attachments));
        ee.read(ss);
        EXPECT_EQ(ie.stored_size(), ee.offset());

        // Set when written test
        write(ee, ss);
        EXPECT_EQ(ie.stored_size() + ee.stored_size(), ee.offset());
    }


    TEST(Attachments, StoredSize)
    {
        Attachments ee;
        ee.disable_crc();

        // Basic size (empty data)
        EXPECT_EQ(ids::size(ids::Attachments) +
                vint::size(0), ee.stored_size());

        // Size with data
        std::vector<ElPtr> children;
        populate_children(children);
        std::streamsize body_size(children_size(children));
        populate_element(ee, children);
        EXPECT_EQ(ids::size(ids::Attachments) +
                vint::size(body_size) + body_size, ee.stored_size());

        // Size with CRC enabled
        ee.enable_crc();
        body_size += 6;
        EXPECT_EQ(ids::size(ids::Attachments) +
                vint::size(body_size) + body_size, ee.stored_size());
        ee.disable_crc();
    }


    TEST(Attachments, Read)
    {
        std::vector<ElPtr> children;
        populate_children(children);
        std::streamsize body_size(children_size(children));
        std::string buffer;
        fill_buffer(buffer, ids::Attachments, children, false, true, true);
        std::stringstream input(buffer);

        Attachments ee;
        EXPECT_EQ(vint::size(body_size) + body_size, ee.read(input));
        check_children_match(ee, children);

        // Body size value wrong (too small)
        input.str(std::string());
        vint::write(2, input);
        write(*children[0], input);
        write(*children[1], input);
        EXPECT_THROW(ee.read(input), ReadError);
        // Post-condition test: error should leave element as before
        check_children_match(ee, children);

        // Invalid child
        input.str(std::string());
        input.clear();
        UIntElement ue(ids::EBML, 0xFFFF);
        vint::write(ue.stored_size(), input);
        write(ue, input);
        EXPECT_THROW(ee.read(input), InvalidChildID);
        // Post-condition test: error should leave element as before
        check_children_match(ee, children);
    }


    TEST(Attachments, ReadWithCRC)
    {
        std::vector<ElPtr> children;
        populate_children(children);
        std::streamsize body_size(children_size(children) + 6); // +6 for CRC
        std::string buffer;
        fill_buffer(buffer, ids::Attachments, children, false, true, true, true);
        std::stringstream input(buffer);

        Attachments ee;
        ee.disable_crc();
        EXPECT_EQ(vint::size(body_size) + body_size, read(ee, input));
        check_children_match(ee, children);
        EXPECT_TRUE(ee.crc_enabled());

        // Bad CRC
        std::string().swap(buffer);
        fill_buffer(buffer, ids::Attachments, children, false, true, true, true);
        // Corrupt the CRC
        buffer[3] = 0x01;
        buffer[4] = 0x02;
        // Attempt to read
        input.str(buffer);
        EXPECT_THROW(read(ee, input), BadCRC);
        // Post-condition test: error should leave element as before
        check_children_match(ee, children);
        EXPECT_TRUE(ee.crc_enabled());

        // Test automatic enabling/disabling of CRC
        ee.disable_crc();
        std::string().swap(buffer);
        fill_buffer(buffer, ids::Attachments, children, false, true, true, true);
        input.str(buffer);
        read(ee, input);
        EXPECT_TRUE(ee.crc_enabled());
        std::string().swap(buffer);
        fill_buffer(buffer, ids::Attachments, children, false, true, true, false);
        input.str(buffer);
        read(ee, input);
        EXPECT_FALSE(ee.crc_enabled());
    }


    TEST(Attachments, StartWrite)
    {
        std::vector<ElPtr> children;
        populate_children(children);
        Attachments ee;
        populate_element(ee, children);
        ee.disable_crc();

        std::string expected;
        fill_buffer(expected, ids::Attachments, children, true, true, true);

        std::stringstream output;
        std::streamsize body_size(children_size(children));
        EXPECT_EQ(ids::size(ids::Attachments) + vint::size(body_size) +
                body_size, ee.start_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
                expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Attachments) + vint::size(body_size) +
                body_size, output.tellp());
    }


    TEST(Attachments, FinishWrite)
    {
        std::vector<ElPtr> children;
        populate_children(children);
        Attachments ee;
        populate_element(ee, children);
        ee.disable_crc();

        // Test for exception and no change to output when finishing without
        // starting
        std::stringstream output;
        EXPECT_THROW(ee.finish_write(output), NotWriting);
        EXPECT_EQ(0, output.tellp());

        std::string expected;
        fill_buffer(expected, ids::Attachments, children, true, true, true);

        std::streamsize body_size(children_size(children));
        ee.start_write(output);
        EXPECT_EQ(ids::size(ids::Attachments) + vint::size(body_size) +
                body_size, ee.finish_write(output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
                expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Attachments) + vint::size(body_size) +
                body_size, output.tellp());

        // Test for double-finish raising an exception
        EXPECT_THROW(ee.finish_write(output), NotWriting);
    }


    TEST(Attachments, Write)
    {
        std::vector<ElPtr> children;
        populate_children(children);
        Attachments ee;
        populate_element(ee, children);
        ee.disable_crc();

        std::string expected;
        fill_buffer(expected, ids::Attachments, children, true, true, true);

        std::stringstream output;
        std::streamsize body_size(children_size(children));
        EXPECT_EQ(ids::size(ids::Attachments) + vint::size(body_size) +
                body_size, write(ee, output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
                expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Attachments) + vint::size(body_size) +
                body_size, output.tellp());

        // Test for double-finish raising an exception
        EXPECT_THROW(ee.finish_write(output), NotWriting);
    }


    TEST(Attachments, WriteWithCRC)
    {
        std::vector<ElPtr> children;
        populate_children(children);
        Attachments ee;
        populate_element(ee, children);
        ee.disable_crc();

        std::string expected;
        fill_buffer(expected, ids::Attachments, children, true, true, true,
                true);

        ee.enable_crc();
        std::stringstream output;
        std::streamsize body_size(children_size(children) + 6); // +6 for CRC
        EXPECT_EQ(ids::size(ids::Attachments) + vint::size(body_size) +
                body_size, write(ee, output));
        EXPECT_PRED_FORMAT2(test_utils::std_buffers_eq, output.str(),
                expected);
        // Post-condition test
        EXPECT_EQ(ids::size(ids::Attachments) + vint::size(body_size) +
                body_size, output.tellp());
    }


    TEST(Attachments, RoundTrip)
    {
        std::vector<ElPtr> children;
        populate_children(children);
        Attachments ee1;
        populate_element(ee1, children);

        std::stringstream io;
        std::streamsize written_bytes = write(ee1, io);
        // Skip the ID
        io.seekg(ids::size(ids::Attachments), std::ios::beg);
        Attachments ee2;
        std::streamsize read_bytes = read(ee2, io);

        check_children_match(ee2, children);
        EXPECT_TRUE(ee2.crc_enabled());
    }
}; // namespace test_attachments

