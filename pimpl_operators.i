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
#error "Expect to be included from PIMPL_HPP
#endif
#ifdef PIMPL_OPERATORS_I
#error "Expect to be included exactly once"
#endif
#define PIMPL_OPERATORS_I

// Forward logical operators that are defined as member functions
private:
template <class T, class = decltype(std::declval<T>() >= std::declval<T>())>
static std::true_type supports_more_or_equal_test(const T &);
static std::false_type supports_more_or_equal_test(...);

public:
template <typename X = decltype(supports_more_or_equal_test(std::declval<Target>()))>
bool operator>=(base const &other)
{
  return *self() >= extract(other);
}

private:
template <class T, class = decltype(std::declval<T>() == std::declval<T>())>
static std::true_type supports_equality_test(const T &);
static std::false_type supports_equality_test(...);

public:
template <typename X = decltype(supports_equality_test(std::declval<Target>()))>
bool operator==(base const &other)
{
  return *self() == extract(other);
}

private:
template <class T, class = decltype(std::declval<T>() <= std::declval<T>())>
static std::true_type supports_less_or_equal_test(const T &);
static std::false_type supports_less_or_equal_test(...);

public:
template <typename X = decltype(supports_less_or_equal_test(std::declval<Target>()))>
bool operator<=(base const &other)
{
  return *self() <= extract(other);
}

private:
template <class T, class = decltype(std::declval<T>() != std::declval<T>())>
static std::true_type supports_inequality_test(const T &);
static std::false_type supports_inequality_test(...);

public:
template <typename X = decltype(supports_inequality_test(std::declval<Target>()))>
bool operator!=(base const &other)
{
  return *self() != extract(other);
}

private:
template <class T, class = decltype(std::declval<T>() < std::declval<T>())>
static std::true_type supports_less_than_test(const T &);
static std::false_type supports_less_than_test(...);

public:
template <typename X = decltype(supports_less_than_test(std::declval<Target>()))>
bool operator<(base const &other)
{
  return *self() < extract(other);
}

private:
template <class T, class = decltype(std::declval<T>() > std::declval<T>())>
static std::true_type supports_more_than_test(const T &);
static std::false_type supports_more_than_test(...);

public:
template <typename X = decltype(supports_more_than_test(std::declval<Target>()))>
bool operator>(base const &other)
{
  return *self() > extract(other);
}

