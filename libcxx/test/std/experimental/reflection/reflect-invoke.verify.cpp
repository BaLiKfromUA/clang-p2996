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
// ADDITIONAL_COMPILE_FLAGS: -Wno-unneeded-internal-declaration -Wno-unused-variable -Wno-unused-value

// <experimental/reflection>
//
// [reflection]

#include <experimental/meta>
#include <type_traits>

namespace NS {
struct A {
  consteval int fn() const { return 24; }
};
} // namespace NS

struct A {
  constexpr int fn() const { return 42; }

  consteval void void_fn() const {
    // no-op
  };
};

template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
consteval T foo(T arg) {
  return arg + 1;
}

struct B {};

template <typename T>
struct C {
  T data;
};

int main() {
              // ======================
              // arguments validation
              // ======================
  reflect_invoke(^^foo, {^^int}, {std::meta::reflect_value(42)}); // ok

  reflect_invoke(^^foo, {^^NS}, {std::meta::reflect_value(42)});
  // expected-error-re@-1 {{call to consteval function 'std::meta::reflect_invoke<{{.*}}>' is not a constant expression}}
  // expected-note-re@-2 {{a reflection of {{.*}} cannot represent a template argument}}

  reflect_invoke(^^foo, {^^int}, {^^NS});
  // expected-error-re@-1 {{call to consteval function 'std::meta::reflect_invoke<{{.*}}>' is not a constant expression}}
  // expected-note-re@-2 {{a reflection of {{.*}} cannot represent a function argument}}

  reflect_invoke(^^NS, {});
  // expected-error-re@-1 {{call to consteval function 'std::meta::reflect_invoke<{{.*}}>' is not a constant expression}}
  // expected-note-re@-2 {{cannot invoke {{.*}}}}

  auto data = C<int>{3};
  reflect_invoke(^^C, {});
  // expected-error-re@-1 {{call to consteval function 'std::meta::reflect_invoke<{{.*}}>' is not a constant expression}}
  // expected-note-re@-2 {{cannot invoke {{.*}}}}

  reflect_invoke(^^foo, {^^data});
  // expected-error-re@-1 {{call to consteval function 'std::meta::reflect_invoke<{{.*}}>' is not a constant expression}}
  // expected-note-re@-2 {{no specialization of the function template {{.*}} matched the provided arguments}}

              // ======================
              // non-static member functions
              // ======================
 constexpr A expectedClass{};
 reflect_invoke(^^A::fn, {^^expectedClass}); // ok

 reflect_invoke(^^A::void_fn, {^^expectedClass});
 // expected-error-re@-1 {{call to consteval function 'std::meta::reflect_invoke<{{.*}}>' is not a constant expression}}
 // expected-note@-2 {{cannot invoke reflection of void-returning function}}

 reflect_invoke(^^A::fn, {});
 // expected-error-re@-1 {{call to consteval function 'std::meta::reflect_invoke<{{.*}}>' is not a constant expression}}
 // expected-note@-2 {{expected related object reflection as a first argument for invoking non-static member function}}

 reflect_invoke(^^A::fn, {std::meta::reflect_value(42)});
 // expected-error-re@-1 {{call to consteval function 'std::meta::reflect_invoke<{{.*}}>' is not a constant expression}}
 // expected-note@-2 {{expected related object reflection as a first argument for invoking non-static member function}}

 constexpr B differentClass{};
 reflect_invoke(^^A::fn, {^^differentClass});
 // expected-error-re@-1 {{call to consteval function 'std::meta::reflect_invoke<{{.*}}>' is not a constant expression}}
 // expected-note@-2 {{method is not a member of given object reflection}}

 constexpr NS::A differentNamespaceClass{};
 reflect_invoke(^^A::fn, {^^differentNamespaceClass});
 // expected-error-re@-1 {{call to consteval function 'std::meta::reflect_invoke<{{.*}}>' is not a constant expression}}
 // expected-note@-2 {{method is not a member of given object reflection}}

 // test that implementation workaround with getting constexpr method from pointer couldn't be abused
 constexpr int (A::*constexpr_pointer)() const = &A::fn;
 reflect_invoke(^^constexpr_pointer, {^^expectedClass}); // ok

 int (A::*pointer)() const = &A::fn;
 reflect_invoke(^^pointer, {^^expectedClass});
 // expected-error-re@-1 {{call to consteval function 'std::meta::reflect_invoke<{{.*}}>' is not a constant expression}}
}
