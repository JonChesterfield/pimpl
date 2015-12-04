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

#include <cassert>
#include <vector>

// Usually a class that has behaviour we care about
// In this example, it's arbitrary
class example_impl
{
 public:
  example_impl(int x = 0) { insert(x); }

  void insert(int x) { local_state.push_back(3 * x); }

  int retrieve() { return local_state.back(); }

 private:
  // Potentially exotic local state
  // For example, maybe we don't want std::vector in the header
  std::vector<int> local_state;
};

static_assert(sizeof(example_impl) == example::capacity,
              "example capacity has diverged");

static_assert(alignof(example_impl) == example::alignment,
              "example alignment has diverged");

// Forwarding methods - free to vary the names relative to the api
void example::first_method(int x)
{
  example_impl &impl = *(reinterpret_cast<example_impl *>(&(this->state)));

  impl.insert(x);
}

int example::second_method()
{
  example_impl &impl = *(reinterpret_cast<example_impl *>(&(this->state)));

  return impl.retrieve();
}

// A whole lot of boilerplate forwarding the standard operations
// This is (believe it or not...) written for clarity, so none call each other

example::example() { new (&state) example_impl{}; }
example::example(int x) { new (&state) example_impl{x}; }

namespace pimpl
{
  template class base<example_impl>;
}

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
