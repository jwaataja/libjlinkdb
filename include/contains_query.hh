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

#ifndef LIBJLINKDB_CONTAINS_QUERY_H
#define LIBJLINKDB_CONTAINS_QUERY_H

#include <unordered_set>
#include <vector>

#include "link_entry.hh"
#include "query.hh"

namespace libjlinkdb {

// A query with search terms that matches an entry if it contains one of the
// search terms in any of its fields.
class ContainsQuery : public Query {
public:
    // Constructs a query with no terms that will match nothing.
    ContainsQuery() = default;
    // Contstructs a query with the given terms. Duplicates are ignored.
    explicit ContainsQuery(const std::vector<std::string>& terms);

    // Returns whether any field in entry contains any of the search terms.
    bool matches(const LinkEntry& entry) const override;

private:
    std::vector<std::string> terms_;

    bool entry_contains_term(
        const LinkEntry& entry, const std::string& term) const;
};
} // namespace libjlinkdb

#endif // LIBJLINKDB_CONTAINS_QUERY_H
