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
// Out of line implementation of extract
template <typename D>
template <typename Internal>
const Internal &base<D>::extract<Internal>::operator()(base<D> const &instance)
{
  return *(reinterpret_cast<const Internal *>(&(instance.state)));
}

template <typename D>
template <typename Internal>
Internal &base<D>::extract<Internal>::operator()(base<D> &instance)
{
  return *(reinterpret_cast<Internal *>(&(instance.state)));
}

template <typename D>
template <typename Internal>
const Internal *base<D>::extract<Internal>::operator()(base<D> const *instance)
{
  return reinterpret_cast<const Internal *>(&(instance->state));
}

template <typename D>
template <typename Internal>
Internal *base<D>::extract<Internal>::operator()(base<D> *instance)
{
  return reinterpret_cast<Internal *>(&(instance->state));
}

// Public functions in terms of extract<>
template <typename D>
base<D>::~base()
{
  extract<example_impl>{}(*this).~example_impl();
}
template <typename D>
base<D>::base(const base<D> &other)
{
  const example_impl &impl = extract<example_impl>{}(other);
  new (extract<example_impl>{}(this)) example_impl(impl);
}
template <typename D>
base<D> &base<D>::operator=(const base<D> &other)
{
  extract<example_impl>{}(*this) = extract<example_impl>{}(other);
  return *this;
}
template <typename D>
base<D>::base(base<D> &&other)
{
  example_impl &impl = extract<example_impl>{}(other);
  new (extract<example_impl>{}(this)) example_impl(std::move(impl));
}
template <typename D>
base<D> &base<D>::operator=(base<D> &&other)
{
  extract<example_impl>{}(*this) = std::move(extract<example_impl>{}(other));
  return *this;
}

// Instantiate the combination of types we have in mind explicitly
template struct base<example>::extract<example_impl>;
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
