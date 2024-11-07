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
// ADDITIONAL_COMPILE_FLAGS: -freflection -freflection-new-syntax
// ADDITIONAL_COMPILE_FLAGS: -Wno-unused-variable -Wno-unused-value

// <experimental/reflection>
//
// [reflection]

#include <experimental/meta>

class A {
public:
  consteval int get_a() const {
    return a;
  }
private:
  const int a = 42;
};

int main() {
  constexpr int i = 1;
  int non_constexpr_i = 42;
  constexpr A obj{};

              // ==============
              // value_of
              // ==============
  value_of(^^i); // ok

  value_of(^^non_constexpr_i);
  // expected-error@-1 {{call to consteval function 'std::meta::value_of' is not a constant expression}}
  // expected-note@-2 {{cannot query the value of a variable not usable in constant expressions}}

  value_of(object_of(^^obj));
  // expected-error@-1 {{call to consteval function 'std::meta::value_of' is not a constant expression}}
  // expected-note@-2 {{cannot query the value of an object of non-structural type}}

  value_of(^^A::get_a);
  // expected-error@-1 {{call to consteval function 'std::meta::value_of' is not a constant expression}}
  // expected-note-re@-2 {{cannot query the value of {{.*}}}}

  value_of(^^int);
  // expected-error@-1 {{call to consteval function 'std::meta::value_of' is not a constant expression}}
  // expected-note-re@-2 {{cannot query the value of {{.*}}}}

  value_of(^^::);
  // expected-error@-1 {{call to consteval function 'std::meta::value_of' is not a constant expression}}
  // expected-note-re@-2 {{cannot query the value of {{.*}}}}

              // ==============
              // object_of
              // ==============
  object_of(^^obj); // ok

  object_of(^^A::get_a);
  // expected-error@-1 {{call to consteval function 'std::meta::object_of' is not a constant expression}}
  // expected-note-re@-2 {{cannot query the object of {{.*}}}}

  object_of(^^int);
  // expected-error@-1 {{call to consteval function 'std::meta::object_of' is not a constant expression}}
  // expected-note-re@-2 {{cannot query the object of {{.*}}}}

  object_of(std::meta::reflect_value(42));
  // expected-error@-1 {{call to consteval function 'std::meta::object_of' is not a constant expression}}
  // expected-note-re@-2 {{cannot query the object of {{.*}}}}

  object_of(^^::);
  // expected-error@-1 {{call to consteval function 'std::meta::object_of' is not a constant expression}}
  // expected-note-re@-2 {{cannot query the object of {{.*}}}}
}