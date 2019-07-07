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

#ifndef JLINKDB_JLINKDB_H
#define JLINKDB_JLINKDB_H

#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace jlinkdb {

class LinkEntry {
public:
    LinkEntry() = default;
    LinkEntry(const std::string& link);

    LinkEntry(const LinkEntry& other) = default;
    LinkEntry(LinkEntry&& other) = default;

    LinkEntry& operator=(const LinkEntry& other) = default;
    LinkEntry& operator=(LinkEntry&& other) = default;

    const std::string& link() const;
    void set_link(const std::string& link);

    const std::string& name() const;
    void set_name(const std::string& name);

    const std::string& description() const;
    void set_description(const std::string& description);

    const std::unordered_set<std::string>& tags() const;
    bool has_tag(const std::string& tag) const;
    void add_tag(const std::string& tag);
    void remove_tag(const std::string& tag);
    void clear_tags();

    const std::unordered_map<std::string, std::string>& attributes() const;
    bool has_attribute(const std::string& attribute) const;
    const std::string& get_attribute(const std::string& attribute) const;
    void set_attribute(const std::string& attribute, const std::string& value);
    void remove_attribute(const std::string& attribute);
    void clear_attributes();

private:
    std::string link_;
    std::string description_;
    std::unordered_set<std::string> tags_;
    std::unordered_map<std::string, std::string> attributes_;
};

class Query {
public:
    virtual bool matches(const LinkEntry& entry) const = 0;
};

class LinkDatabase {
public:
    using LinkEntryIterator =
        typename std::unordered_map<int, std::shared_ptr<LinkEntry>>::iterator;
    using ConstLinkEntryIterator = typename std::unordered_map<int,
        std::shared_ptr<LinkEntry>>::const_iterator;

    LinkDatabase();
    LinkDatabase(const LinkDatabase& other) = default;
    LinkDatabase(LinkDatabase&& other) = default;

    LinkDatabase& operator=(const LinkDatabase& other) = default;
    LinkDatabase& operator=(LinkDatabase&& other) = default;

    LinkEntryIterator links_begin();
    LinkEntryIterator links_end();
    ConstLinkEntryIterator links_cbegin() const;
    ConstLinkEntryIterator links_cend() const;

    std::size_t links_count() const;
    std::shared_ptr<LinkEntry> get_entry(int id) const;
    // Returns the new id.
    int add_entry(std::shared_ptr<LinkEntry> entry);
    void delete_entry(int id);

    std::vector<std::pair<int, std::shared_ptr<LinkEntry>>> query(
        const Query& query) const;

private:
    std::unordered_map<int, std::shared_ptr<LinkEntry>> links_;
    int highest_id_;
};

} // namespace jlinkdb

#endif // JLINKDB_JLINKDB_H

