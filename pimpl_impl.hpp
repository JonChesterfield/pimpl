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

#ifndef PIMPL_IMPL_HPP
#define PIMPL_IMPL_HPP

#include "pimpl.hpp"
#include <utility>

namespace pimpl
{
// Constructors
template <typename D, std::size_t C, std::size_t A>
base<D, C, A>::base()
{
  new (&state) ImplType{};
}

template <typename D, std::size_t C, std::size_t Align>
template <typename A, typename X>
base<D, C, Align>::base(A &&a)
{
  new (&state) ImplType{std::forward<A>(a)};
}

template <typename D, std::size_t C, std::size_t Align>
template <typename A, typename B, typename... Args>
base<D, C, Align>::base(A &&a, B &&b, Args &&... args)
{
  new (&state) ImplType{std::forward<A>(a), std::forward<B>(b),
                        std::forward<Args>(args)...};
}

// Destructor
template <typename D, std::size_t C, std::size_t A>
base<D, C, A>::~base()
{
  self()->~ImplType();
}

// Helper functions
template <typename D, std::size_t C, std::size_t A>
const typename base<D, C, A>::ImplType &base<D, C, A>::extract(
    const base<D, C, A> &instance)
{
  return *(reinterpret_cast<const ImplType *>(&(instance.state)));
}

template <typename D, std::size_t C, std::size_t A>
typename base<D, C, A>::ImplType &base<D, C, A>::extract(
    base<D, C, A> &instance)
{
  return *(reinterpret_cast<ImplType *>(&(instance.state)));
}

template <typename D, std::size_t C, std::size_t A>
typename base<D, C, A>::ImplType *base<D, C, A>::self()
{
  return &(extract(*this));
}

// Copy and move
template <typename D, std::size_t C, std::size_t A>
base<D, C, A>::base(const base<D, C, A> &other)
{
  const ImplType &impl = extract(other);
  new (self()) ImplType(impl);
}
template <typename D, std::size_t C, std::size_t A>
base<D, C, A> &base<D, C, A>::operator=(const base<D, C, A> &other)
{
  *self() = extract(other);
  return *this;
}
template <typename D, std::size_t C, std::size_t A>
base<D, C, A>::base(base<D, C, A> &&other)
{
  ImplType &impl = extract(other);
  new (self()) ImplType(std::move(impl));
}
template <typename D, std::size_t C, std::size_t A>
base<D, C, A> &base<D, C, A>::operator=(base<D, C, A> &&other)
{
  *self() = std::move(extract(other));
  return *this;
}

// Logical operators
template <typename D, std::size_t C, std::size_t A>
template <typename X>
bool base<D, C, A>::operator==(base<D, C, A> const &other)
{
  return *self() == extract(other);
}
template <typename D, std::size_t C, std::size_t A>
template <typename X>
bool base<D, C, A>::operator!=(base<D, C, A> const &other)
{
  return *self() != extract(other);
}

namespace detail
{
// Instantiate applicable logical operators
template <typename T, typename D, std::size_t C, std::size_t A>
struct use_equality
{
  void operator()() {}
};
template <typename D, std::size_t C, std::size_t A>
struct use_equality<std::true_type, D, C, A>
{
  void operator()(base<D, C, A> &self) { static_cast<void>(self == self); }
};
template <typename T, typename D, std::size_t C, std::size_t A>
struct use_inequality
{
  void operator()() {}
};
template <typename D, std::size_t C, std::size_t A>
struct use_inequality<std::true_type, D, C, A>
{
  void operator()(base<D, C, A> &self) { static_cast<void>(self != self); }
};
}

template <typename D, std::size_t C, std::size_t A>
void base<D, C, A>::instantiator()
{
  detail::use_equality<decltype(supports_equality_test(std::declval<D>())), D,
                       C, A>{}(*this);
  detail::use_inequality<decltype(supports_inequality_test(std::declval<D>())),
                         D, C, A>{}(*this);
}
}

#endif  // PIMPL_IMPL_HPP
