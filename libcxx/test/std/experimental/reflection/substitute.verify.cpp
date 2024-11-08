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
#include <vector>

int main() {
  can_substitute(^^std::vector, {^^int}); //ok
  substitute(^^std::vector, {^^int}); //ok

  can_substitute(^^int, {^^int});
  // expected-error-re@-1 {{call to consteval function 'std::meta::can_substitute<{{.*}}>' is not a constant expression}}
  // expected-note-re@-2 {{expected a reflection of a template, but got {{.*}}}}

  substitute(^^int, {^^int});
  // expected-error-re@-1 {{call to consteval function 'std::meta::substitute<{{.*}}>' is not a constant expression}}
  // expected-note-re@-2 {{expected a reflection of a template, but got {{.*}}}}

  substitute(^^std::vector, {^^::});
  // expected-error-re@-1 {{call to consteval function 'std::meta::substitute<{{.*}}>' is not a constant expression}}
  // expected-note-re@-2 {{a reflection of {{.*}} cannot represent a template argument}}
}