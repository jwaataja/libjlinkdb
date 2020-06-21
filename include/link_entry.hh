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

#ifndef JLINKDB_LINK_ENTRY_HH_
#define JLINKDB_LINK_ENTRY_HH_

#include <string>
#include <unordered_map>
#include <unordered_set>

namespace libjlinkdb {

// A value representing a single link entry. Will contain either an empty URL
// location or a valid URL.
class LinkEntry {
public:
    // Constructs a LinkEntry with an empty location.
    LinkEntry() = default;
    // Constructs a LinkEntry with the given location. The location must be
    // empty or a valid URL. Otherwise, a JLinkDbError is thrown.
    explicit LinkEntry(const std::string& location);

    LinkEntry(const LinkEntry& other) = default;
    LinkEntry(LinkEntry&& other) = default;

    LinkEntry& operator=(const LinkEntry& other) = default;
    LinkEntry& operator=(LinkEntry&& other) = default;

    // Returns true if and only if all fields match other.
    bool operator==(const LinkEntry& other) const;
    // Returns true if and only if at least one field is distinct from other.
    bool operator!=(const LinkEntry& other) const;

    // Returns the link location.
    const std::string& location() const;
    // Sets the location to given location. The location must be empty or a
    // valid URL.  Otherwise, a JLinkDbError is thrown.
    void set_location(const std::string& location);

    // Returns the name of the entry.
    const std::string& name() const;
    // Sets the name of the entry to name.
    void set_name(const std::string& name);

    // Returns the description of the entry.
    const std::string& description() const;
    // Sets the description of the entry to description.
    void set_description(const std::string& description);

    // Returns the tags of the entry.
    const std::unordered_set<std::string>& tags() const;
    // Returns whether the entry has the given tag.
    bool has_tag(const std::string& tag) const;
    // Adds tag to the entry's tags.
    void add_tag(const std::string& tag);
    // Deletes tag from the entry's tags.
    void remove_tag(const std::string& tag);
    // Deletes all tags from the entry.
    void clear_tags();

    // Returns a map of attribute names to values.
    const std::unordered_map<std::string, std::string>& attributes() const;
    // Returns whether the entry has an attribute with name attribute.
    bool has_attribute(const std::string& attribute) const;
    // Returns the value of the attribute with name attribute if it exists.
    // Otherwise, returns the empty string.
    const std::string get_attribute(const std::string& attribute) const;
    // Sets the attribute with name attribute to the given value. Creates the
    // attribute if it doesn't exist.
    void set_attribute(const std::string& attribute, const std::string& value);
    // Removes the attribute with name attribute, if it exists.
    void remove_attribute(const std::string& attribute);
    // Removes all attributes from the entry.
    void clear_attributes();

private:
    std::string location_;
    std::string name_;
    std::string description_;
    std::unordered_set<std::string> tags_;
    std::unordered_map<std::string, std::string> attributes_;
};

}  // namespace libjlinkdb

#endif  // JLINKDB_LINK_ENTRY_HH_
