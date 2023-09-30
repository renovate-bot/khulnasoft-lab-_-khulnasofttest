// Copyright 2008, Khulnasoft Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Khulnasoft Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Low-level types and utilities for porting Khulnasoft Mock to various
// platforms.  All macros ending with _ and symbols defined in an
// internal namespace are subject to change without notice.  Code
// outside Khulnasoft Mock MUST NOT USE THEM DIRECTLY.  Macros that don't
// end with _ are part of Khulnasoft Mock's public API and can be used by
// code outside Khulnasoft Mock.

// IWYU pragma: private, include "kmock/kmock.h"
// IWYU pragma: friend kmock/.*

#ifndef KHULNASOFTMOCK_INCLUDE_KMOCK_INTERNAL_KMOCK_PORT_H_
#define KHULNASOFTMOCK_INCLUDE_KMOCK_INTERNAL_KMOCK_PORT_H_

#include <assert.h>
#include <stdlib.h>
#include <cstdint>
#include <iostream>

// Most of the utilities needed for porting Khulnasoft Mock are also
// required for Khulnasoft Test and are defined in ktest-port.h.
//
// Note to maintainers: to reduce code duplication, prefer adding
// portability utilities to Khulnasoft Test's ktest-port.h instead of
// here, as Khulnasoft Mock depends on Khulnasoft Test.  Only add a utility
// here if it's truly specific to Khulnasoft Mock.

#include "kmock/internal/custom/kmock-port.h"
#include "ktest/internal/ktest-port.h"

#ifdef KTEST_HAS_ABSL
#include "absl/flags/declare.h"
#include "absl/flags/flag.h"
#endif

// For MS Visual C++, check the compiler version. At least VS 2015 is
// required to compile Khulnasoft Mock.
#if defined(_MSC_VER) && _MSC_VER < 1900
#error "At least Visual C++ 2015 (14.0) is required to compile Khulnasoft Mock."
#endif

// Macro for referencing flags.  This is public as we want the user to
// use this syntax to reference Khulnasoft Mock flags.
#define KMOCK_FLAG_NAME_(name) kmock_##name
#define KMOCK_FLAG(name) FLAGS_kmock_##name

// Pick a command line flags implementation.
#ifdef KTEST_HAS_ABSL

// Macros for defining flags.
#define KMOCK_DEFINE_bool_(name, default_val, doc) \
  ABSL_FLAG(bool, KMOCK_FLAG_NAME_(name), default_val, doc)
#define KMOCK_DEFINE_int32_(name, default_val, doc) \
  ABSL_FLAG(int32_t, KMOCK_FLAG_NAME_(name), default_val, doc)
#define KMOCK_DEFINE_string_(name, default_val, doc) \
  ABSL_FLAG(std::string, KMOCK_FLAG_NAME_(name), default_val, doc)

// Macros for declaring flags.
#define KMOCK_DECLARE_bool_(name) \
  ABSL_DECLARE_FLAG(bool, KMOCK_FLAG_NAME_(name))
#define KMOCK_DECLARE_int32_(name) \
  ABSL_DECLARE_FLAG(int32_t, KMOCK_FLAG_NAME_(name))
#define KMOCK_DECLARE_string_(name) \
  ABSL_DECLARE_FLAG(std::string, KMOCK_FLAG_NAME_(name))

#define KMOCK_FLAG_GET(name) ::absl::GetFlag(KMOCK_FLAG(name))
#define KMOCK_FLAG_SET(name, value) \
  (void)(::absl::SetFlag(&KMOCK_FLAG(name), value))

#else  // KTEST_HAS_ABSL

// Macros for defining flags.
#define KMOCK_DEFINE_bool_(name, default_val, doc)  \
  namespace testing {                               \
  KTEST_API_ bool KMOCK_FLAG(name) = (default_val); \
  }                                                 \
  static_assert(true, "no-op to require trailing semicolon")
#define KMOCK_DEFINE_int32_(name, default_val, doc)    \
  namespace testing {                                  \
  KTEST_API_ int32_t KMOCK_FLAG(name) = (default_val); \
  }                                                    \
  static_assert(true, "no-op to require trailing semicolon")
#define KMOCK_DEFINE_string_(name, default_val, doc)         \
  namespace testing {                                        \
  KTEST_API_ ::std::string KMOCK_FLAG(name) = (default_val); \
  }                                                          \
  static_assert(true, "no-op to require trailing semicolon")

// Macros for declaring flags.
#define KMOCK_DECLARE_bool_(name)          \
  namespace testing {                      \
  KTEST_API_ extern bool KMOCK_FLAG(name); \
  }                                        \
  static_assert(true, "no-op to require trailing semicolon")
#define KMOCK_DECLARE_int32_(name)            \
  namespace testing {                         \
  KTEST_API_ extern int32_t KMOCK_FLAG(name); \
  }                                           \
  static_assert(true, "no-op to require trailing semicolon")
#define KMOCK_DECLARE_string_(name)                 \
  namespace testing {                               \
  KTEST_API_ extern ::std::string KMOCK_FLAG(name); \
  }                                                 \
  static_assert(true, "no-op to require trailing semicolon")

#define KMOCK_FLAG_GET(name) ::testing::KMOCK_FLAG(name)
#define KMOCK_FLAG_SET(name, value) (void)(::testing::KMOCK_FLAG(name) = value)

#endif  // KTEST_HAS_ABSL

#endif  // KHULNASOFTMOCK_INCLUDE_KMOCK_INTERNAL_KMOCK_PORT_H_
