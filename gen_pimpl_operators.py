#!/usr/bin/env/python

# Copyright 2015 Jon Chesterfield. All rights reserved.

# This file is part of pimpl.

# pimpl is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# pimpl is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with pimpl.  If not, see <http://www.gnu.org/licenses/>.

# This is a throwaway script used to generate various operator forwards

import datetime

now = datetime.datetime.now()

logical_op_map = {"==" :"equality",
                 "!=" :"inequality",
                 "<" : "less_than",
                 ">": "more_than",
                 "<=":"less_or_equal",
                 ">=":"more_or_equal",
                }


print """// Copyright {0} Jon Chesterfield. All rights reserved.

// This file is part of pimpl.

// pimpl is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// pimpl is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with pimpl.  If not, see <http://www.gnu.org/licenses/>.
""".format(now.year)

print """#ifndef PIMPL_HPP
#error "Expect to be included from PIMPL_HPP
#endif
#ifdef PIMPL_OPERATORS_I
#error "Expect to be included exactly once"
#endif
#define PIMPL_OPERATORS_I
"""

print "// Forward logical operators that are defined as member functions"
for key,value in logical_op_map.iteritems():
    print """private:
template <class T, class = decltype(std::declval<T>() {0} std::declval<T>())>
static std::true_type supports_{1}_test(const T &);
static std::false_type supports_{1}_test(...);

public:
template <typename X = decltype(supports_{1}_test(std::declval<Target>()))>
bool operator{0}(base const &other)
{{
  return *self() {0} extract(other);
}}
""".format(key,value)


