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

#include "catch.hpp"
#include "example.hpp"

// Verify that we can compile the various operations
TEST_CASE("default constructor") { example a; }
TEST_CASE("unary constructor") { example a{3}; }
TEST_CASE("binary constructor") { example a{3,5}; }
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
  CHECK(example{} == example{});
  CHECK(example{1} == example{1});
}

TEST_CASE("inequality")
{
  CHECK(example{} != example{1});
  CHECK(example{1} != example{2});
}


