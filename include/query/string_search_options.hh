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

#ifndef LIBJLINKDB_QUERY_STRING_SEARCH_OPTIONS_HH_
#define LIBJLINKDB_QUERY_STRING_SEARCH_OPTIONS_HH_

namespace libjlinkdb {

namespace query {

// Options for searching strings.
struct StringSearchOptions {
    // If true, the search should succeed if and only if the whole search
    // string matches the target.
    bool match_full_string;
    // If true, the search should ignore case entirely.
    bool ignore_case;
};

}  // namespace query

}  // namespace libjlinkdb

#endif  // LIBJLINKDB_QUERY_STRING_SEARCH_OPTIONS_HH_
