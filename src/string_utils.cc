
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

#include "string_utils.hh"

#include <algorithm>
#include <cctype>
#include <iterator>
#include <string>

#include "query/string_search_options.hh"

namespace libjlinkdb {

using std::string;

void
to_lower_in_place(string& str)
{
    std::transform(str.begin(), str.end(), str.begin(),
        [](unsigned char c) { return std::tolower(c); });
}

bool
search_string(const string& str, const string& target,
    const query::StringSearchOptions& options)
{
    // TODO(jwaataja): Optimize in particular cases to avoid copying. For
    // example, use an equals_ignore_case_method.
    string str_copy{str};
    string target_copy{target};

    if (options.ignore_case) {
        to_lower_in_place(str_copy);
        to_lower_in_place(target_copy);
    }

    if (options.match_full_string) {
        return str_copy == target_copy;
    } else {
        return str_copy.find(target_copy) != string::npos;
    }
}

}  // namespace libjlinkdb
