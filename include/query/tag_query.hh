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

#ifndef LIBJLINKDB_QUERY_TAG_QUERY_H
#define LIBJLINKDB_QUERY_TAG_QUERY_H

#include <string>

#include "link_entry.hh"
#include "query/query.hh"
#include "query/string_search_options.hh"

namespace libjlinkdb {

namespace query {

// A query that searches a link's tag for a term and succeeds if any tag
// matches.
class TagQuery : public Query {
public:
    // Constructs a query that searchs tags for term, obeying options.
    TagQuery(const std::string& term, const StringSearchOptions& options);

    // Returns true if and only if at least one of entry's tags contains the
    // term, according to options.
    bool matches(const LinkEntry& entry) const override;

private:
    std::string term_;
    StringSearchOptions options_;
};

} // namespace query

} // namespace libjlinkdb

#endif // LIBJLINKDB_QUERY_TAG_QUERY_H
