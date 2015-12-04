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
  const Target &extract(base const &instance)
  {
    return *(reinterpret_cast<const Target *>(&(instance.state)));
  }
  Target &extract(base &instance)
  {
    return *(reinterpret_cast<Target *>(&(instance.state)));
  }
  Target *self() { return &(extract(*this)); }

 protected:
  base(){};
  unsigned char state alignas(detail::alignment)[detail::capacity];

 public:
  static const std::size_t capacity{detail::capacity};
  static const std::size_t alignment{detail::alignment};

  ~base() { self()->~Target(); }
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
