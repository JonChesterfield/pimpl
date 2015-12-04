// Copyright 2015 Jon Chesterfield. All rights reserved.

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

#include "pimpl.hpp"
#include "catch.hpp"

// Verify that we can compile the various operations
TEST_CASE("default constructor") { example a; }
TEST_CASE("other constructor") { example a{3}; }
TEST_CASE("copy ctor")
{
  example a;
  example b{a};
}
TEST_CASE("copy assign ctor")
{
  example a;
  example b = a;
}
TEST_CASE("move ctor")
{
  example a;
  example b{std::move(a)};
}
TEST_CASE("move assign ctor")
{
  example a;
  example b = std::move(a);
}

TEST_CASE("behaviour")
{
  example a;
  a.first_method(2);
  CHECK(a.second_method() == 6);
}

TEST_CASE("equality")
{
  CHECK(example {} == example {});
  CHECK(example{4} == example{4});
}

TEST_CASE("inequality")
{
  CHECK(example {0} != example{1});
}

