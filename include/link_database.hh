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

#ifndef JLINKDB_LINK_DATABASE_H
#define JLINKDB_LINK_DATABASE_H

#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>
#include <sigc++/sigc++.h>

#include "link_entry.hh"
#include "query.hh"

namespace libjlinkdb {

class LinkDatabase {
public:
    using LinkEntryIterator =
        typename std::unordered_map<int, std::shared_ptr<LinkEntry>>::iterator;
    using ConstLinkEntryIterator = typename std::unordered_map<int,
        std::shared_ptr<LinkEntry>>::const_iterator;

    LinkDatabase();
    explicit LinkDatabase(std::istream& reader);
    explicit LinkDatabase(const std::string& path);
    LinkDatabase(const LinkDatabase& other) = default;
    LinkDatabase(LinkDatabase&& other) = default;

    LinkDatabase& operator=(const LinkDatabase& other) = default;
    LinkDatabase& operator=(LinkDatabase&& other) = default;

    LinkEntryIterator links_begin();
    LinkEntryIterator links_end();
    ConstLinkEntryIterator links_cbegin() const;
    ConstLinkEntryIterator links_cend() const;

    // Returns the number of links in the database.
    std::size_t links_count() const;
    // Returns whether there exists a link entry with the given id.
    bool has_entry(int id) const;
    // Returns the entry with the given id if it exists, and a null pointer
    // otherwise.
    std::shared_ptr<LinkEntry> get_entry(int id) const;
    // Returns the new id.
    int add_entry(std::shared_ptr<LinkEntry> entry);
    // Deletes the entry with the given id.
    void delete_entry(int id);

    // Returns the collection of entries in the database that match query. Each
    // element of the result is a pair containing the id of the entry and the
    // entry itself.
    std::vector<std::pair<int, std::shared_ptr<LinkEntry>>> search(
        const Query& query) const;

    void write_to_stream(std::ostream& writer) const;
    void write_to_file(const std::string& path) const;

    sigc::signal<void, int>& signal_entry_added();
    sigc::signal<void, int>& signal_entry_deleted();

private:
    std::unordered_map<int, std::shared_ptr<LinkEntry>> links_;
    int highest_id_ = 0;

    sigc::signal<void, int> entry_added_;
    sigc::signal<void, int> entry_deleted_;
};

} // namespace libjlinkdb

#endif // JLINKDB_LINK_DATABASE_H
