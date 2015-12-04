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

#include <utility>  // std::move

namespace pimpl
{
template <typename Target, std::size_t Cap, std::size_t Align>
class base
{
 protected:
  const Target &extract(base const &instance)
  {
    return *(reinterpret_cast<const Target *>(&(instance.state)));
  }
  Target &extract(base &instance)
  {
    return *(reinterpret_cast<Target *>(&(instance.state)));
  }
  Target *self() { return &(extract(*this)); }

 public:
  static const std::size_t capacity{Cap};
  static const std::size_t alignment{Align};

  // Forward constructors through to implementation
  base() { new (&state) Target{}; }
  template <typename A, typename B>
  using copy_ctor_workaround = typename std::enable_if<
      !std::is_base_of<A,
                       typename std::remove_reference<B>::type>::value>::type;
  template <typename T, typename X = copy_ctor_workaround<base, T>>
  base(T &&t)
  {
    new (&state) Target{std::forward<T>(t)};
  }
  template <typename A, typename B, typename... Args>
  base(A &&a, B &&b, Args &&... args)
  {
    new (&state) Target{std::forward<A>(a), std::forward<B>(b),
                        std::forward<Args>(args)...};
  }
  // Forward destructor through to implementation
  ~base() { self()->~Target(); }

  // Forward copy & move through to implementation
  base(const base &other)
  {
    const Target &impl = extract(other);
    new (self()) Target(impl);
  }
  base &operator=(const base &other)
  {
    *self() = extract(other);
    return *this;
  }
  base(base &&other)
  {
    Target &impl = extract(other);
    new (self()) Target(std::move(impl));
  }
  base &operator=(base &&other)
  {
    *self() = std::move(extract(other));
    return *this;
  }

// Forward operators, provided they exist
#include "pimpl_operators.i"

 protected:
  unsigned char state alignas(alignment)[capacity];
};
}

// Sadly, we leak the name of the internal class
// We also explicitly specify the required size and alignment.
class example_impl;
class example final : public pimpl::base<example_impl, 24, 8>
{
 public:
  using base::base;  // Pull constructors out of the example_impl

  // Some methods
  void first_method(int);
  int second_method();
};

#endif
