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
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "gtest/gtest.h"

#include "libjlinkdb.hh"

using libjlinkdb::LinkDatabase;
using libjlinkdb::LinkEntry;
using libjlinkdb::query::ContainsQuery;
using libjlinkdb::query::StringSearchOptions;
using std::begin;
using std::end;
using std::shared_ptr;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

constexpr const char EMPTY_LINKS[] = "{ \"links\": [] }";
constexpr const char BLANK_LINE[] = "{ \"links\":\n[] }";
constexpr const char BASIC_LINK[] = "{ \"links\": [ { } ] }";
constexpr const char WITH_ATTRIBUTES[] =
    "{ \"links\": ["
    "{"
    "\"location\": \"https://gentoo.org\","
    "\"name\": \"MyName\","
    "\"description\": \"My description.\","
    "\"tags\": ["
    "\"first tag\","
    "\"second tag\""
    "],"
    "\"attributes\": {"
    "\"ab\": \"12\","
    "\"cd\": \"34\""
    "}"
    "},"
    "{ }"
    "]"
    "}";

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

vector<LinkEntry>
gather_links_ordered(const LinkDatabase& db)
{
    vector<LinkEntry> result;
    for (auto iter = db.links_cbegin(); iter != db.links_cend(); ++iter)
        result.push_back(*iter->second);
    std::sort(result.begin(), result.end(),
        [](const LinkEntry& link1, const LinkEntry& link2) {
            return link1.location() < link2.location();
        });

    return result;
}

template <typename T>
bool
has_link(const T& links, const string& location)
{
    return std::find_if(begin(links), end(links),
               [&](shared_ptr<LinkEntry> entry) {
                   return entry->location() == location;
               })
        != links.end();
}

class FuncQuery : public libjlinkdb::query::Query {
public:
    FuncQuery(const std::function<bool(const LinkEntry&)>& func) : func_{func}
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
    auto query = db.search(FuncQuery{func});
    std::transform(begin(query), end(query), std::back_inserter(result),
        [](const std::pair<int, shared_ptr<LinkEntry>>& entry) {
            return entry.second;
        });
    return result;
}

LinkDatabase
database_from_string(const string& data_string)
{
    std::istringstream reader{data_string};
    return LinkDatabase{reader};
}

TEST(TestLinkEntry, TestConstructor)
{
    LinkEntry entry;
    EXPECT_EQ("", entry.location());
    EXPECT_EQ("", entry.name());
    EXPECT_EQ("", entry.description());
    EXPECT_TRUE(entry.tags().empty());
    EXPECT_TRUE(entry.attributes().empty());
}

TEST(TestLinkEntry, TestEquals)
{
    LinkEntry entry1{"a"};
    entry1.set_name("a");
    entry1.set_description("b");

    LinkEntry entry2{"a"};
    entry2.set_name("b");
    entry2.set_description("a");

    LinkEntry entry3{"b"};
    entry3.set_name("a");
    entry3.set_description("a");

    LinkEntry entry4 = entry1;
    EXPECT_EQ(entry1, entry1);
    EXPECT_EQ(entry1, entry4);
    EXPECT_NE(entry1, entry2);
    EXPECT_NE(entry2, entry3);
    EXPECT_NE(entry3, entry1);
}

namespace {

class LinkDatabaseTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        add_link(db1_, "a");
        add_link(db1_, "b");
        expected_entries_.push_back({});
        LinkEntry link{"https://gentoo.org"};
        link.set_name("MyName");
        link.set_description("My description.");
        link.add_tag("first tag");
        link.add_tag("second tag");
        link.set_attribute("ab", "12");
        link.set_attribute("cd", "34");
        expected_entries_.push_back(link);
    }

    // TODO(jason): Change these to db_ and db1_.
    LinkDatabase db_;
    LinkDatabase db1_;
    std::vector<LinkEntry> expected_entries_;
};

} // namespace

TEST_F(LinkDatabaseTest, TestIterator)
{
    auto links = gather_links(db1_);
    EXPECT_EQ(2, links.size());
    EXPECT_TRUE(has_link(links, "a"));
    EXPECT_TRUE(has_link(links, "b"));
}

TEST_F(LinkDatabaseTest, TestAddEntryId)
{
    int id1 = db_.add_entry(std::make_shared<LinkEntry>("a"));
    int id2 = db_.add_entry(std::make_shared<LinkEntry>("b"));
    EXPECT_EQ("a", db_.get_entry(id1)->location());
    EXPECT_EQ("b", db_.get_entry(id2)->location());
}

TEST_F(LinkDatabaseTest, TestHasLink)
{
    int id = db_.add_entry(std::make_shared<LinkEntry>("a"));
    EXPECT_TRUE(db_.has_entry(id));
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
        query_func(db1_, [](const LinkEntry&) { return false; }).empty());
}

TEST_F(LinkDatabaseTest, TestQueryAll)
{
    auto links = query_func(db1_, [](const LinkEntry&) { return true; });
    EXPECT_EQ(2, links.size());
    EXPECT_TRUE(has_link(links, "a"));
    EXPECT_TRUE(has_link(links, "b"));
}

TEST_F(LinkDatabaseTest, TestQueryBasic)
{
    auto links = query_func(
        db1_, [](const LinkEntry& entry) { return entry.location() == "a"; });
    EXPECT_EQ(1, links.size());
    EXPECT_TRUE(has_link(links, "a"));
}

TEST_F(LinkDatabaseTest, TestEmptyLinks)
{
    auto db = database_from_string(EMPTY_LINKS);
    EXPECT_EQ(0, db.links_count());
}

TEST_F(LinkDatabaseTest, TestBlankLine)
{
    auto db = database_from_string(BLANK_LINE);
    EXPECT_EQ(0, db.links_count());
}

TEST_F(LinkDatabaseTest, TestBasicLink)
{
    auto db = database_from_string(BASIC_LINK);
    auto links = gather_links(db);
    EXPECT_EQ(1, links.size());
    EXPECT_TRUE(has_link(links, ""));
}

TEST_F(LinkDatabaseTest, TestReadFull)
{
    auto db = database_from_string(WITH_ATTRIBUTES);
    auto links = gather_links_ordered(db);
    EXPECT_EQ(expected_entries_, links);
}

TEST_F(LinkDatabaseTest, TestWriteFull)
{
    auto db = database_from_string(WITH_ATTRIBUTES);
    std::ostringstream writer;
    db.write_to_stream(writer);
    std::istringstream reader{writer.str()};
    LinkDatabase db_copy{reader};
    auto links = gather_links_ordered(db_copy);
    EXPECT_EQ(expected_entries_, links);
}

TEST_F(LinkDatabaseTest, TestReadEmptyStream)
{
    ASSERT_THROW(database_from_string(""), libjlinkdb::JLinkDbError);
}

TEST_F(LinkDatabaseTest, TestReadNoLink)
{
    ASSERT_THROW(database_from_string("{}"), libjlinkdb::JLinkDbError);
}

TEST_F(LinkDatabaseTest, TestMalformedData)
{
    ASSERT_THROW(
        database_from_string("{ \"links\": {}"), libjlinkdb::JLinkDbError);
}

TEST_F(LinkDatabaseTest, TestExtraneousFieldsAllowed)
{
    ASSERT_NO_THROW(database_from_string(
        "{ \"links\": { \"dummy1\": \"a\" }, \"dummy2\": \"b\" }"));
}


TEST_F(LinkDatabaseTest, TestSearchBasic)
{
}

class ContainsQueryTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        entry1_.set_location("abc def ghi");
        entry2_.set_name("abc def ghi");
        entry3_.set_description("abc def ghi");
        entry3_.add_tag("abc");
        entry3_.add_tag("def");
        entry3_.add_tag("ghi");
        entry4_.set_attribute("abc", "def");
        entry4_.set_attribute("ghi", "jkl");
        entry5_.set_location("ab");
        entry5_.set_name("ab");
        entry5_.add_tag("ab");
        entry5_.set_attribute("ab", "xy");
    }

    ContainsQuery query_{{"abc", "xyz"}, {false, true}};

    LinkEntry entry1_;
    LinkEntry entry2_;
    LinkEntry entry3_;
    LinkEntry entry4_;
    LinkEntry entry5_;
};

TEST_F(ContainsQueryTest, TestContainsQueryEmpty)
{
    LinkEntry entry;
    ContainsQuery query;
    EXPECT_FALSE(query.matches(entry));
}

TEST_F(ContainsQueryTest, TestContainsQueryPositive)
{
    EXPECT_TRUE(query_.matches(entry1_));
    EXPECT_TRUE(query_.matches(entry2_));
    EXPECT_TRUE(query_.matches(entry3_));
    EXPECT_TRUE(query_.matches(entry4_));
}

TEST_F(ContainsQueryTest, TestContainsQueryNegative)
{
    EXPECT_FALSE(query_.matches(entry5_));
}

int
main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
