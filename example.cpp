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

#include "example.hpp"
#include "pimpl_impl.hpp"

#include <cassert>
#include <vector>

// Usually a class that has behaviour we care about
// In this example, it's arbitrary
class example_impl
{
 public:
  example_impl(int x = 0) { insert(x); }
  example_impl(int a, int b) { insert(a); insert(b);}

  void insert(int x) { local_state.push_back(3 * x); }

  int retrieve() { return local_state.back(); }

  bool operator==(example_impl const &other)
  {
    return local_state == other.local_state;
  }
  bool operator!=(example_impl const &other) { return !(*this == other); }

 private:
  // Potentially exotic local state
  // For example, maybe we don't want std::vector in the header
  std::vector<int> local_state;
};

void example::first_method(int x)
{
  self()->insert(x);
}

int example::second_method()
{
  return self()->retrieve();
}

static_assert(sizeof(example_impl) == example::capacity,
              "example capacity has diverged");

static_assert(alignof(example_impl) == example::alignment,
              "example alignment has diverged");

namespace pimpl
{
  template class base<example_impl, sizeof(example_impl), alignof(example_impl)>;
  using targ = base<example_impl, sizeof(example_impl), alignof(example_impl)>;
  // Need to do something to instantiate the forwarding constructors since
  // the pimpl header cannot, in general, guess their types.
  template targ::base(int &&);
  template targ::base(int &&, int&&);
}
