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

#include "query/contains_query.hh"

#include <algorithm>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include "link_entry.hh"
#include "query/attribute_contains_query.hh"
#include "query/description_extractor.hh"
#include "query/field_query.hh"
#include "query/location_extractor.hh"
#include "query/name_extractor.hh"
#include "query/or_collection.hh"
#include "query/query.hh"
#include "query/string_search_options.hh"
#include "query/tag_query.hh"
#include "string_utils.hh"

namespace libjlinkdb {

namespace query {

using std::begin;
using std::end;
using std::make_shared;
using std::shared_ptr;
using std::string;
using std::vector;

ContainsQuery::ContainsQuery(
    const std::vector<string>& terms, const StringSearchOptions& options)
{
    vector<shared_ptr<Query>> queries;
    std::transform(begin(terms), end(terms), std::back_inserter(queries),
        [&](const string& term) { return query_for_term(term, options); });
    underlying_query_ = OrCollection{queries};
}

bool
ContainsQuery::matches(const LinkEntry& entry) const
{
    return underlying_query_.matches(entry);
}

std::shared_ptr<Query>
ContainsQuery::query_for_term(
    const string& term, const StringSearchOptions& options)
{
    vector<shared_ptr<Query>> queries;
    queries.push_back(make_shared<FieldQuery<LocationExtractor>>(
        LocationExtractor{}, term, options));
    queries.push_back(make_shared<FieldQuery<NameExtractor>>(
        NameExtractor{}, term, options));
    queries.push_back(make_shared<FieldQuery<DescriptionExtractor>>(
        DescriptionExtractor{}, term, options));
    queries.push_back(make_shared<TagQuery>(term, options));
    queries.push_back(make_shared<AttributeContainsQuery>(term, options));
    return make_shared<OrCollection>(queries);
}

} // namespace query

} // namespace libjlinkdb
