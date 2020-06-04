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

#include "query/and_collection.hh"

#include <algorithm>
#include <iterator>
#include <memory>
#include <vector>

#include "link_entry.hh"
#include "query/query.hh"

namespace libjlinkdb {

namespace query {

using std::begin;
using std::end;
using std::shared_ptr;
using std::vector;

AndCollection::AndCollection(const vector<shared_ptr<Query>>& queries)
    : queries_{queries}
{
}

bool
AndCollection::matches(const LinkEntry& entry) const
{
    return std::all_of(begin(queries_), end(queries_),
        [&](const shared_ptr<Query>& query) { return query->matches(entry); });
}

} // namespace query

} // namespace libjlinkdb
