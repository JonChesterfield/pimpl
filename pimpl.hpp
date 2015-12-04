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

namespace detail
{
// Keeping these up to date is unfortunate
// More hassle when supporting various platforms
// with different ideas about these values.
const std::size_t capacity = 24;
const std::size_t alignment = 8;
}

namespace pimpl
{
template <typename Target>
class base
{
 private:
  // A layer of indirection
  template <typename Internal>
  struct extract
  {
    const Internal &operator()(base const &instance)
    {
      return *(reinterpret_cast<const Internal *>(&(instance.state)));
    }

    Internal &operator()(base &instance)
    {
      return *(reinterpret_cast<Internal *>(&(instance.state)));
    }

    const Internal *operator()(base const *instance)
    {
      return reinterpret_cast<const Internal *>(&(instance->state));
    }

    Internal *operator()(base *instance)
    {
      return reinterpret_cast<Internal *>(&(instance->state));
    }
  };

 protected:
  base(){};
  unsigned char state alignas(detail::alignment)[detail::capacity];

 public:
  static const std::size_t capacity{detail::capacity};
  static const std::size_t alignment{detail::alignment};

  // Another layer of indirection
  template <typename I>
  void base_destroy()
  {
    extract<I>{}(*this).~I();
  }
  template <typename I>
  void base_copy(const base &other)
  {
    const I &impl = extract<I>{}(other);
    new (extract<I>{}(this)) I(impl);
  }
  template <typename I>
  base &base_copy_assign(const base &other)
  {
    extract<I>{}(*this) = extract<I>{}(other);
    return *this;
  }
  template <typename I>
  void base_move(base &&other)
  {
    I &impl = extract<I>{}(other);
    new (extract<I>{}(this)) I(std::move(impl));
  }
  template <typename I>
  base &base_move_assign(base &&other)
  {
    extract<I>{}(*this) = std::move(extract<I>{}(other));
    return *this;
  }

  // The incomplete type is sufficient to define these functions
  ~base() { base_destroy<Target>(); }
  base(const base &other) { base_copy<Target>(other); }
  base &operator=(const base &other) { return base_copy_assign<Target>(other); }
  base(base &&other) { base_move<Target>(std::move(other)); }
  base &operator=(base &&other)
  {
    return base_move_assign<Target>(std::move(other));
  }
};
}

// Sadly, we leak the name of the internal class
// I can't currently see a way around this that doesn't involve
// typing out the base(...) functions in the implementation.
class example_impl;
class example final : public pimpl::base<example_impl>
{
 public:
  // Constructors
  example();
  example(int);

  // Some methods
  void first_method(int);
  int second_method();
};

#endif
