//===----------------------------------------------------------------------===//
//
// Copyright 2024 Bloomberg Finance L.P.
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03 || c++11 || c++14 || c++17 || c++20
// ADDITIONAL_COMPILE_FLAGS: -freflection
// ADDITIONAL_COMPILE_FLAGS: -freflection-new-syntax
// ADDITIONAL_COMPILE_FLAGS: -fparameter-reflection
// ADDITIONAL_COMPILE_FLAGS: -Wno-unneeded-internal-declaration -Wno-unused-variable -Wno-unused-value

// <experimental/reflection>
//
// [reflection]

#include <experimental/meta>

struct A {
  constexpr A([[maybe_unused]] int _a) {}

  constexpr ~A() {}

  consteval A operator+(int num) const { return A(num); }

  template <typename T>
  consteval static void foo() {}

  operator int() const { return 42; }

  template <typename T>
  operator double() const {
    return 42.0f;
  }

  template <typename T>
  void operator()([[maybe_unused]] T lhs, [[maybe_unused]] T rhs) {}
};

int main() {
            // ======================
            // identifier_of
            // ======================
  int var;
  identifier_of(^^var); // ok

  identifier_of(^^int);
  // expected-error@-1 {{call to consteval function 'std::meta::identifier_of' is not a constant expression}}
  // expected-note-re@-2 {{reflected {{.*}} is anonymous and has no associated identifier}}

  identifier_of(^^A::foo<int>);
  // expected-error@-1 {{call to consteval function 'std::meta::identifier_of' is not a constant expression}}
  // expected-note@-2 {{names of template specializations are not identifiers}}

  /* TODO: clarify if test with constructor should fail or not
  identifier_of(^^A::A);
  // {{call to consteval function 'std::meta::identifier_of' is not a constant expression}}
  // {{names of constructors are not identifiers}}
  */

  identifier_of(^^A::~A);
  // expected-error@-1 {{call to consteval function 'std::meta::identifier_of' is not a constant expression}}
  // expected-note@-2 {{names of destructors are not identifiers}}

  identifier_of(^^A::operator+);
  // expected-error@-1 {{call to consteval function 'std::meta::identifier_of' is not a constant expression}}
  // expected-note@-2 {{names of operators are not identifiers}}

  identifier_of(^^A::operator int);
  // expected-error@-1 {{call to consteval function 'std::meta::identifier_of' is not a constant expression}}
  // expected-note@-2 {{names of conversion functions are not identifiers}}

  // todo: come up with test for constructor template

  identifier_of(^^A::operator());
  // expected-error@-1 {{call to consteval function 'std::meta::identifier_of' is not a constant expression}}
  // expected-note@-2 {{names of operator templates are not identifiers}}

  identifier_of(^^A::operator double);
  // expected-error@-1 {{call to consteval function 'std::meta::identifier_of' is not a constant expression}}
  // expected-note@-2 {{names of conversion function templates are not identifiers}}

  identifier_of(^^::);
  // expected-error@-1 {{call to consteval function 'std::meta::identifier_of' is not a constant expression}}
  // expected-note@-2 {{the global namespace has no associated identifier}}
}