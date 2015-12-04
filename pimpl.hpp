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
  base() {}
  
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
  /*
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
  */
  
  template <typename I>
  void base_destroy()
  {
    self()->~I();
  }
  template <typename I>
  void base_copy(const base &other)
  {
    const I &impl = extract(other);
    new (self()) I(impl);
  }
  template <typename I>
  base &base_copy_assign(const base &other)
  {
    *self() = extract(other);
    return *this;
  }
  template <typename I>
  void base_move(base &&other)
  {
    I &impl = extract(other);
    new (self()) I(std::move(impl));
  }
  template <typename I>
  base &base_move_assign(base &&other)
  {
    *self() = std::move(extract(other));
    return *this;
  }

  ~base();
  base(const base &other);
  base &operator=(const base &other);
  base(base &&other);
  base &operator=(base &&other);
  

// Forward operators, provided they exist
// #include "pimpl_operators.i"

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
  example();
  example(int);

  // Some methods
  void first_method(int);
  int second_method();
};

#endif
