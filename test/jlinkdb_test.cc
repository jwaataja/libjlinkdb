/*
 * Copyright (c) 2019 Jason Waataja
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <algorithm>
#include <functional>
#include <iterator>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "gtest/gtest.h"

#include "jlinkdb.hh"

using jlinkdb::LinkDatabase;
using jlinkdb::LinkEntry;
using std::begin;
using std::end;
using std::shared_ptr;
using std::string;
using std::unordered_set;
using std::vector;

void
add_link(LinkDatabase& db, const string& link)
{
    db.add_entry(std::make_shared<LinkEntry>(link));
}

unordered_set<shared_ptr<LinkEntry>>
gather_links(const LinkDatabase& db)
{
    unordered_set<shared_ptr<LinkEntry>> result;
    for (auto iter = db.links_cbegin(); iter != db.links_cend(); ++iter)
        result.insert(iter->second);

    return result;
}

template <typename T>
bool
has_link(const T& links, const string& link)
{
    return std::find_if(begin(links), end(links),
               [&](shared_ptr<LinkEntry> entry) {
                   return entry->link() == link;
               })
        != links.end();
}

class FuncQuery : public jlinkdb::Query {
public:
    FuncQuery(const std::function<bool(const LinkEntry&)>& func)
        : func_{ func }
    {
    }

    bool matches(const LinkEntry& entry) const override
    {
        return func_(entry);
    }

private:
    std::function<bool(const LinkEntry&)> func_;
};

vector<shared_ptr<LinkEntry>>
query_func(
    const LinkDatabase& db, const std::function<bool(const LinkEntry&)>& func)
{
    vector<shared_ptr<LinkEntry>> result;
    auto query = db.query(FuncQuery{ func });
    std::transform(begin(query), end(query), std::back_inserter(result),
        [](const std::pair<int, shared_ptr<LinkEntry>>& entry) {
            return entry.second;
        });
    return result;
}

TEST(TestLinkEntry, TestConstructor)
{
    LinkEntry entry;
    EXPECT_EQ("", entry.link());
    EXPECT_EQ("", entry.name());
    EXPECT_EQ("", entry.description());
    EXPECT_TRUE(entry.tags().empty());
    EXPECT_TRUE(entry.attributes().empty());
}

namespace {

class LinkDatabaseTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        add_link(db1, "a");
        add_link(db1, "b");
    }

    LinkDatabase db;
    LinkDatabase db1;
};

} // namespace

TEST_F(LinkDatabaseTest, TestIterator)
{
    auto links = gather_links(db1);
    EXPECT_EQ(2, links.size());
    EXPECT_TRUE(has_link(links, "a"));
    EXPECT_TRUE(has_link(links, "b"));
}

TEST_F(LinkDatabaseTest, TestAddEntryId)
{
    int id1 = db.add_entry(std::make_shared<LinkEntry>("a"));
    int id2 = db.add_entry(std::make_shared<LinkEntry>("b"));
    EXPECT_EQ("a", db.get_entry(id1)->link());
    EXPECT_EQ("b", db.get_entry(id2)->link());
}

TEST_F(LinkDatabaseTest, TestHasLink)
{
    int id = db.add_entry(std::make_shared<LinkEntry>("a"));
    EXPECT_TRUE(db.has_entry(id));
}

TEST_F(LinkDatabaseTest, TestRemoveLink)
{
    LinkDatabase db;
    int id = db.add_entry(std::make_shared<LinkEntry>("a"));
    EXPECT_TRUE(db.has_entry(id));
    db.delete_entry(id);
    EXPECT_FALSE(db.has_entry(id));
}

TEST_F(LinkDatabaseTest, TestQueryNone)
{
    EXPECT_TRUE(
        query_func(db1, [](const LinkEntry&) { return false; }).empty());
}

TEST_F(LinkDatabaseTest, TestQueryAll)
{
    auto links = query_func(db1, [](const LinkEntry&) { return true; });
    EXPECT_EQ(2, links.size());
    EXPECT_TRUE(has_link(links, "a"));
    EXPECT_TRUE(has_link(links, "b"));
}

TEST_F(LinkDatabaseTest, TestQueryBasic)
{
    auto links = query_func(db1, [](const LinkEntry& entry) { return
        entry.link() == "a"; });
    EXPECT_EQ(1, links.size());
    EXPECT_TRUE(has_link(links, "a"));
}

int
main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
