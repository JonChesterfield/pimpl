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

namespace detail
{
// Keeping these up to date is unfortunate
// More hassle when supporting various platforms
// with different ideas about these values.
const std::size_t capacity = 24;
const std::size_t alignment = 8;
}

class example final
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

  // No public state available (it's all in the implementation)
 private:
  // No private functions (they're also in the implementation)
  unsigned char state alignas(detail::alignment)[detail::capacity];
};

#endif
