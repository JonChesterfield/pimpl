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
#include <cstddef>      // std::size_t
#include <type_traits>  // std::enable_if

namespace pimpl
{
template <typename A, typename B>
using copy_ctor_workaround = typename std::enable_if<
    !std::is_base_of<A, typename std::remove_reference<B>::type>::value>::type;

template <typename Target, std::size_t Cap, std::size_t Align>
class base
{
public:
  typedef Target ImplType;
  static const std::size_t capacity{Cap};
  static const std::size_t alignment{Align};
  
 protected:
  unsigned char state alignas(alignment)[capacity];

 protected:
  static const Target &extract(base const &instance);
  static Target &extract(base &instance);
  Target *self();

public:
  // Forward constructors through to implementation
  base();
  template <typename A, typename X = pimpl::copy_ctor_workaround<base, A>>
  base(A &&a);
  template <typename A, typename B, typename... Args>
  base(A &&a, B &&b, Args &&... args);

  // Forward destructor, copy, move
  ~base();
  base(const base &other);
  base &operator=(const base &other);
  base(base &&other);
  base &operator=(base &&other);

  // Logical operators
private:
  template <class T, class = decltype(std::declval<T>() == std::declval<T>())>
  static std::true_type supports_equality_test(const T &);
  static std::false_type supports_equality_test(...);
public:
  template <typename X =
	    decltype(supports_equality_test(std::declval<Target>()))>
  bool operator==(base const &other);
private:
  template <class T, class = decltype(std::declval<T>() != std::declval<T>())>
  static std::true_type supports_inequality_test(const T &);
  static std::false_type supports_inequality_test(...);
public:
  template <typename X =
	    decltype(supports_inequality_test(std::declval<Target>()))>
  bool operator!=(base const &other);
  
};
}

#endif
