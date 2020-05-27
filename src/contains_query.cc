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

#include "contains_query.hh"

#include <algorithm>
#include <iterator>
#include <string>
#include <vector>

#include "link_entry.hh"

namespace libjlinkdb {

using std::begin;
using std::end;
using std::string;

ContainsQuery::ContainsQuery(const std::vector<string>& terms) : terms_{terms}
{
}

bool
ContainsQuery::matches(const LinkEntry& entry) const
{
    return std::any_of(begin(terms_), end(terms_),
        [&](const string& term) { return entry_contains_term(entry, term); });
}

bool
ContainsQuery::entry_contains_term(
    const LinkEntry& entry, const string& term) const
{
    if (entry.location().find(term) != string::npos) {
        return true;
    }

    if (entry.name().find(term) != string::npos) {
        return true;
    }

    if (entry.description().find(term) != string::npos) {
        return true;
    }

    if (std::any_of(begin(entry.tags()), end(entry.tags()),
            [&](const std::string& tag) {
                return tag.find(term) != string::npos;
            })) {
        return true;
    }

    if (std::any_of(begin(entry.attributes()), end(entry.attributes()),
            [&](const std::pair<std::string, std::string>& item) {
                return item.first.find(term) != string::npos
                    || item.second.find(term) != string::npos;
            })) {
        return true;
    }

    return false;
}

} // namespace libjlinkdb
