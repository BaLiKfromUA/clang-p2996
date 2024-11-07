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
#include <utility>

template <typename T>
class A {};

int foo() {
  return 42;
}

struct B {
  constexpr B() {}

  constexpr ~B() {}

  struct InnerCls {};
};

int main() {
                  // ==============
                  // operator_of
                  // ==============
  std::meta::operator_of(^^int);
  // expected-error@-1 {{call to consteval function 'std::meta::operator_of' is not a constant expression}}
  // expected-note-re@-2 {{{{.*}} is not an operator or operator template}}

                  // ==============
                  // type_of
                  // ==============
  constexpr int a = 3;
  std::meta::type_of(^^a); // ok

  std::meta::type_of(^^int);
  // expected-error@-1 {{call to consteval function 'std::meta::type_of' is not a constant expression}}
  // expected-note-re@-2 {{{{.*}} has no type}}

  std::meta::type_of(^^::);
  // expected-error@-1 {{call to consteval function 'std::meta::type_of' is not a constant expression}}
  // expected-note-re@-2 {{{{.*}} has no type}}

  std::meta::type_of(^^A<int>);
  // expected-error@-1 {{call to consteval function 'std::meta::type_of' is not a constant expression}}
  // expected-note-re@-2 {{{{.*}} has no type}}

  const auto [x, y] = std::pair{1, 2};
  std::meta::type_of(^^x);
  // expected-error@-1 {{call to consteval function 'std::meta::type_of' is not a constant expression}}
  // expected-note-re@-2 {{cannot query the type of {{.*}}}}

  std::meta::type_of(^^B::~B);
  // expected-error@-1 {{call to consteval function 'std::meta::type_of' is not a constant expression}}
  // expected-note-re@-2 {{cannot query the type of {{.*}}}}

                  // ==============
                  // parent_of
                  // ==============
  parent_of(^^B::InnerCls); // ok

  std::meta::parent_of(^^::);
  // expected-error@-1 {{call to consteval function 'std::meta::parent_of' is not a constant expression}}
  // expected-note-re@-2 {{{{.*}} has no parent}}

  std::meta::parent_of(std::meta::reflect_value(42));
  // expected-error@-1 {{call to consteval function 'std::meta::parent_of' is not a constant expression}}
  // expected-note-re@-2 {{{{.*}} has no parent}}

  constexpr int b = 1;
  std::meta::parent_of(object_of(^^b));
  // expected-error@-1 {{call to consteval function 'std::meta::parent_of' is not a constant expression}}
  // expected-note-re@-2 {{{{.*}} has no parent}}

  std::meta::parent_of(^^int);
  // expected-error@-1 {{call to consteval function 'std::meta::parent_of' is not a constant expression}}

                  // ==============
                  // template_of
                  // ==============
  template_of(^^A<int>); // ok

  template_of(^^B);
  // expected-error@-1 {{call to consteval function 'std::meta::template_of' is not a constant expression}}
  // expected-note@-2 {{expected a reflection of a template specialization}}

  template_of(^^foo);
  // expected-error@-1 {{call to consteval function 'std::meta::template_of' is not a constant expression}}
  // expected-note@-2 {{expected a reflection of a template specialization}}

  template_of(^^int);
  // expected-error@-1 {{call to consteval function 'std::meta::template_of' is not a constant expression}}
  // expected-note@-2 {{expected a reflection of a template specialization}}

  template_of(^^::);
  // expected-error@-1 {{call to consteval function 'std::meta::template_of' is not a constant expression}}
  // expected-note@-2 {{expected a reflection of a template specialization}}
}