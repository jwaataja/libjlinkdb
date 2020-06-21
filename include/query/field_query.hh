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

#ifndef LIBJLINKDB_QUERY_FIELD_QUERY_HH_
#define LIBJLINKDB_QUERY_FIELD_QUERY_HH_

#include <string>

#include "link_entry.hh"
#include "query/query.hh"
#include "query/string_search_options.hh"
#include "string_utils.hh"

namespace libjlinkdb {

namespace query {

// A Query that matches if some particular field of an entry matches a
// search term. The particular field depends on the extractor. The type F
// must be a callabe type that takes a const LinkEntry& and returns a
// string.
template <typename F>
class FieldQuery : public Query {
public:
    // Constructs a FieldQuery that uses extractor to extract fields from
    // entries and searches them for the given term. The search obeys
    // options.
    FieldQuery(const F& extractor, const std::string& term,
        const StringSearchOptions& options);

    // Returns the query's search term.
    const std::string& term() const;
    // Returns the query's search options.
    const StringSearchOptions& options() const;

    // Returns true if and only if the string extracted from entry contains
    // the search term, according to options.
    bool matches(const LinkEntry& entry) const override;

private:
    F extractor_;
    std::string term_;
    StringSearchOptions options_;
};

template <typename F>
FieldQuery<F>::FieldQuery(const F& extractor, const std::string& term,
    const StringSearchOptions& options)
    : extractor_{extractor}, term_{term}, options_{options}
{
}

template <typename F>
bool
FieldQuery<F>::matches(const LinkEntry& entry) const
{
    std::string field{extractor_(entry)};
    return search_string(field, term_, options_);
}

template <typename F>
const std::string&
FieldQuery<F>::term() const
{
    return term_;
}

template <typename F>
const StringSearchOptions&
FieldQuery<F>::options() const
{
    return options_;
}

}  // namespace query

}  // namespace libjlinkdb

#endif  // LIBJLINKDB_QUERY_FIELD_QUERY_HH_
