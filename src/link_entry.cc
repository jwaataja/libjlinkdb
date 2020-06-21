// Copyright (c) 2020 Jason Waataja

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#include "link_entry.hh"

#include <string>
#include <unordered_map>
#include <unordered_set>

#include <nlohmann/json.hpp>
#include "LUrlParser.h"

#include "jlinkdb_error.hh"

namespace libjlinkdb {

using std::string;
using std::unordered_map;
using std::unordered_set;

LinkEntry::LinkEntry(const string& location) : location_{location}
{
    if (location.empty()) {
        return;
    }


    auto parsed = LUrlParser::ParseURL::parseURL(location);
    if (!parsed.isValid()) {
        throw JLinkDbError{"invalid url: " + location};
    }
}

bool
LinkEntry::operator==(const LinkEntry& other) const
{
    return location_ == other.location_ && name_ == other.name_
        && description_ == other.description_ && tags_ == other.tags_
        && attributes_ == other.attributes_;
}

bool
LinkEntry::operator!=(const LinkEntry& other) const
{
    return !(*this == other);
}

const string&
LinkEntry::location() const
{
    return location_;
}

void
LinkEntry::set_location(const string& location)
{
    if (!location.empty()) {
        auto parsed = LUrlParser::ParseURL::parseURL(location);
        if (!parsed.isValid()) {
            throw JLinkDbError{"invalid url: " + location};
        }
    }

    location_ = location;
}

const string&
LinkEntry::name() const
{
    return name_;
}

void
LinkEntry::set_name(const string& name)
{
    name_ = name;
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

const string
LinkEntry::get_attribute(const string& attribute) const
{
    auto iter = attributes_.find(attribute);
    if (iter != attributes_.end()) {
        return iter->second;
    } else {
        return "";
    }
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

} // namespace libjlinkdb
