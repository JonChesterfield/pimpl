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

example::~example()
{
  (reinterpret_cast<example_impl *>(&state))->~example_impl();
}

example::example(const example &other)
{
  const example_impl &impl =
      *(reinterpret_cast<const example_impl *>(&(other.state)));
  new (&state) example_impl(impl);
}

example &example::operator=(const example &other)
{
  *(reinterpret_cast<example_impl *>(&(this->state))) =
      *(reinterpret_cast<const example_impl *>(&(other.state)));
  return *this;
}

example::example(example &&other)
{
  example_impl &impl = *(reinterpret_cast<example_impl *>(&(other.state)));
  new (&state) example_impl(std::move(impl));
}

example &example::operator=(example &&other)
{
  *(reinterpret_cast<example_impl *>(&(this->state))) =
      std::move(*(reinterpret_cast<example_impl *>(&(other.state))));
  return *this;
}

TEST_CASE("init")
{
  example an_example;
  example another_example{3};

  example copied(an_example);
  example moved(std::move(another_example));
}
