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

#ifndef LIBJLINKDB_QUERY_OR_COLLECTION_H
#define LIBJLINKDB_QUERY_OR_COLLECTION_H

#include <memory>
#include <vector>

#include "link_entry.hh"
#include "query/query.hh"

namespace libjlinkdb {

namespace query {

// A query with a collection of subqueries that matches if and only if at least
// one query in the collection matches.
class OrCollection : public Query {
public:
    // Constructs an empty collection.
    OrCollection() = default;
    // Constructs a collection containing all queries in the range [first,
    // last). The iterator should point to type shared_ptr<Query>.
    template <typename InputIterator>
    OrCollection(InputIterator first, InputIterator last);
    // Constructs a collection containing all queries listed in queries.
    OrCollection(const std::vector<std::shared_ptr<Query>>& queries);

    // Returns true if and only if every query in the collection matches entry.
    bool matches(const LinkEntry& entry) const override;

private:
    std::vector<std::shared_ptr<Query>> queries_;
};

template <typename InputIterator>
OrCollection::OrCollection(InputIterator first, InputIterator last)
    : queries_{first, last}
{
}

} // namespace query

} // namespace libjlinkdb

#endif // LIBJLINKDB_QUERY_OR_COLLECTION_H
