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

#include "link_database.hh"

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iterator>
#include <memory>
#include <string>
#include <utility>

#include <nlohmann/json.hpp>
#include <sigc++/sigc++.h>

using nlohmann::json;
using std::shared_ptr;
using std::string;
using std::unique_ptr;
using std::vector;

namespace libjlinkdb {

// Conversions.
void to_json(json& j, const LinkEntry& link);
void from_json(const json& j, LinkEntry& link);

void to_json(json& j, const LinkDatabase& database);
void from_json(const json& j, LinkDatabase& database);

LinkDatabase::LinkDatabase()
{
}

LinkDatabase::LinkDatabase(std::istream& reader) : LinkDatabase{}
{
    json data;
    reader >> data;
    data.get_to(*this);
}

LinkDatabase::LinkDatabase(const string& path) : LinkDatabase{}
{
    // TODO(Jason): Error handling?
    std::ifstream reader{path};
    json data;
    reader >> data;
    data.get_to(*this);
}

LinkDatabase::LinkEntryIterator
LinkDatabase::links_begin()
{
    return links_.begin();
}

LinkDatabase::LinkEntryIterator
LinkDatabase::links_end()
{
    return links_.end();
}

LinkDatabase::ConstLinkEntryIterator
LinkDatabase::links_cbegin() const
{
    return links_.cbegin();
}

LinkDatabase::ConstLinkEntryIterator
LinkDatabase::links_cend() const
{
    return links_.cend();
}

std::size_t
LinkDatabase::links_count() const
{
    return links_.size();
}

bool
LinkDatabase::has_entry(int id) const
{
    return links_.find(id) != links_.end();
}

shared_ptr<LinkEntry>
LinkDatabase::get_entry(int id) const
{
    auto position = links_.find(id);
    if (position != links_.end())
        return position->second;
    else
        return {};
}

int
LinkDatabase::add_entry(shared_ptr<LinkEntry> entry)
{
    int id = highest_id_;
    links_[id] = entry;
    ++highest_id_;
    entry_added_(id);
    return id;
}

void
LinkDatabase::delete_entry(int id)
{
    if (links_.erase(id) > 0)
        entry_deleted_(id);
}

vector<std::pair<int, shared_ptr<LinkEntry>>>
LinkDatabase::query(const Query& query) const
{
    vector<std::pair<int, shared_ptr<LinkEntry>>> result;
    std::copy_if(links_.begin(), links_.end(), std::back_inserter(result),
        [&query](const std::pair<int, shared_ptr<LinkEntry>>& entry) {
            return query.matches(*entry.second.get());
        });
    return result;
}

void
LinkDatabase::write_to_stream(std::ostream& writer) const
{
    json data{*this};
    writer << data;
}

void
LinkDatabase::write_to_file(const string& path) const
{
    std::ofstream writer{path};

    write_to_stream(writer);
}

sigc::signal<void, int>&
LinkDatabase::signal_entry_added()
{
    return entry_added_;
}

sigc::signal<void, int>&
LinkDatabase::signal_entry_deleted()
{
    return entry_deleted_;
}

void
to_json(json& j, const LinkEntry& link)
{
    j = {{"location", link.location()}, {"name", link.name()},
        {"description", link.description()}, {"tags", link.tags()},
        {"attributes", link.attributes()}};
}

void
from_json(const json& j, LinkEntry& link)
{
    auto location = j.find("location");
    if (location != j.end()) {
        link.set_location(location->get<string>());
    }

    auto name = j.find("name");
    if (name != j.end()) {
        link.set_name(name->get<string>());
    }

    auto description = j.find("description");
    if (description != j.end()) {
        link.set_description(description->get<string>());
    }

    auto tags = j.find("tags");
    if (tags != j.end()) {
        for (const auto& tag : *tags) {
            link.add_tag(tag.get<string>());
        }
    }

    auto attributes = j.find("attributes");
    if (attributes != j.end()) {
        for (const auto& attribute : attributes->items()) {
            link.set_attribute(
                attribute.key(), attribute.value().get<string>());
        }
    }
}

void
to_json(json& j, const LinkDatabase& database)
{
    auto links = json::array();
    for (auto it = database.links_cbegin(); it != database.links_cend();
         ++it) {
        links.push_back(*it->second);
    }

    j = {"links", links};
}

void
from_json(const json& j, LinkDatabase& database)
{
    for (const auto& link : j.at("links")) {
        auto entry = std::make_shared<LinkEntry>();
        /* link.get_to(*entry); */
        from_json(link, *entry);
        database.add_entry(entry);
    }
}

} // namespace libjlinkdb
