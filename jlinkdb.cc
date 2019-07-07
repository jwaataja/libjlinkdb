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

#include "jlinkdb.hh"

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

using std::shared_ptr;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

namespace jlinkdb {

LinkEntry::LinkEntry(const string& link) : link_{ link }
{
}

const string&
LinkEntry::link() const
{
    return link_;
}

void
LinkEntry::set_link(const string& link)
{
    link_ = link;
}

const string&
LinkEntry::description() const
{
    return description_;
}

const unordered_set<string>&
LinkEntry::tags() const
{
    return tags_;
}

bool
LinkEntry::has_tag(const string& tag) const
{
    return tags_.find(tag) != tags_.end();
}

void
LinkEntry::add_tag(const string& tag)
{
    tags_.insert(tag);
}

void
LinkEntry::remove_tag(const string& tag)
{
    tags_.erase(tag);
}

void
LinkEntry::clear_tags()
{
    tags_.clear();
}

const unordered_map<string, string>&
LinkEntry::attributes() const
{
    return attributes_;
}

bool
LinkEntry::has_attribute(const string& attribute) const
{
    return attributes_.find(attribute) != attributes_.end();
}

void
LinkEntry::set_description(const string& description)
{
    description_ = description;
}

const string&
LinkEntry::get_attribute(const string& attribute) const
{
    return attributes_.at(attribute);
}

void
LinkEntry::set_attribute(const string& attribute, const string& value)
{
    attributes_[attribute] = value;
}

void
LinkEntry::remove_attribute(const string& attribute)
{
    attributes_.erase(attribute);
}

void
LinkEntry::clear_attributes()
{
    attributes_.clear();
}

LinkDatabase::LinkDatabase() : highest_id_(0)
{
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
    return id;
}

void
LinkDatabase::delete_entry(int id)
{
    links_.erase(id);
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
} // namespace jlinkdb
