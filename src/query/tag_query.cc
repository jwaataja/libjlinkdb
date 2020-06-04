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

#include "query/tag_query.hh"

#include <algorithm>
#include <iterator>
#include <string>

#include "link_entry.hh"
#include "query/query.hh"
#include "query/string_search_options.hh"
#include "string_utils.hh"

namespace libjlinkdb {

namespace query {

using std::begin;
using std::end;
using std::string;

TagQuery::TagQuery(const string& term, const StringSearchOptions& options)
    : term_{term}, options_{options}
{
}

bool
TagQuery::matches(const LinkEntry& entry) const
{
    return std::any_of(
        begin(entry.tags()), end(entry.tags()), [&](const string& tag) {
            return search_string(tag, term_, options_);
        });
}

} // namespace query

} // namespace libjlinkdb
