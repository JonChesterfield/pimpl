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

#ifndef EXAMPLE_H
#define EXAMPLE_H

#include "pimpl.hpp"

class example_impl;
class example final : public pimpl::base<example_impl, 24, 8>
{
 public:
  using base::base;

  // Some methods
  void first_method(int);
  int second_method();
};

#endif // EXAMPLE_H
