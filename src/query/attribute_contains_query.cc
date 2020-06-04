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

#include "query/attribute_contains_query.hh"

#include <algorithm>
#include <iterator>
#include <utility>

#include "link_entry.hh"
#include "query/string_search_options.hh"
#include "string_utils.hh"

namespace libjlinkdb {

namespace query {

using std::string;

AttributeContainsQuery::AttributeContainsQuery(
    const string& term, const StringSearchOptions& options)
    : term_{term}, options_{options}
{
}

bool
AttributeContainsQuery::matches(const LinkEntry& entry) const
{
    auto matcher = [&](const std::pair<string, string>& attr) {
        return search_string(attr.first, term_, options_)
            || search_string(attr.second, term_, options_);
    };
    return std::any_of(
        std::begin(entry.attributes()), std::end(entry.attributes()), matcher);
}

} // namespace query

} // namespace libjlinkdb
