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

#ifndef PIMPL_HPP
#define PIMPL_HPP
#include <cstddef>

namespace pimpl
{
template <typename Derived, std::size_t C, std::size_t A>
class base
{
 public:
  static const std::size_t capacity{C};
  static const std::size_t alignment{A};

 protected:
  base(){};
  unsigned char state alignas(A)[C];

 private:
  Derived *derived() { return static_cast<Derived *>(this); }
  const Derived *derived() const { return static_cast<Derived const *>(this); }
};
}

class example final : public pimpl::base<example, 24, 8>
{
 public:
  // Constructors
  example();
  example(int);

  // Some methods
  void first_method(int);
  int second_method();

  // Set of standard operations
  ~example();
  example(const example &);
  example &operator=(const example &);
  example(example &&);
  example &operator=(example &&);
};

#endif
