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

#include "query/attribute_query.hh"

#include <string>

#include "link_entry.hh"
#include "query/string_search_options.hh"
#include "string_utils.hh"

namespace libjlinkdb {

namespace query {

using std::string;

AttributeQuery::AttributeQuery(const string& attr_name,
    const string& attr_value, const StringSearchOptions& options)
    : attr_name_{attr_name}, attr_value_{attr_value}, options_{options}
{
}

bool
AttributeQuery::matches(const LinkEntry& entry) const
{
    if (!entry.has_attribute(attr_name_)) {
        return false;
    }

    return search_string(
        entry.get_attribute(attr_name_), attr_value_, options_);
}

}  // namespace query

}  // namespace libjlinkdb
