// Copyright 2007, Khulnasoft Inc.
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

// Khulnasoft Mock - a framework for writing C++ mock classes.
//
// This is the main header file a user should include.

#ifndef KHULNASOFTMOCK_INCLUDE_KMOCK_KMOCK_H_
#define KHULNASOFTMOCK_INCLUDE_KMOCK_KMOCK_H_

// This file implements the following syntax:
//
//   ON_CALL(mock_object, Method(...))
//     .With(...) ?
//     .WillByDefault(...);
//
// where With() is optional and WillByDefault() must appear exactly
// once.
//
//   EXPECT_CALL(mock_object, Method(...))
//     .With(...) ?
//     .Times(...) ?
//     .InSequence(...) *
//     .WillOnce(...) *
//     .WillRepeatedly(...) ?
//     .RetiresOnSaturation() ? ;
//
// where all clauses are optional and WillOnce() can be repeated.

#include "kmock/kmock-actions.h"  // IWYU pragma: export
#include "kmock/kmock-cardinalities.h"  // IWYU pragma: export
#include "kmock/kmock-function-mocker.h"  // IWYU pragma: export
#include "kmock/kmock-matchers.h"  // IWYU pragma: export
#include "kmock/kmock-more-actions.h"  // IWYU pragma: export
#include "kmock/kmock-more-matchers.h"  // IWYU pragma: export
#include "kmock/kmock-nice-strict.h"  // IWYU pragma: export
#include "kmock/internal/kmock-internal-utils.h"
#include "kmock/internal/kmock-port.h"

// Declares Khulnasoft Mock flags that we want a user to use programmatically.
KMOCK_DECLARE_bool_(catch_leaked_mocks);
KMOCK_DECLARE_string_(verbose);
KMOCK_DECLARE_int32_(default_mock_behavior);

namespace testing {

// Initializes Khulnasoft Mock.  This must be called before running the
// tests.  In particular, it parses the command line for the flags
// that Khulnasoft Mock recognizes.  Whenever a Khulnasoft Mock flag is seen,
// it is removed from argv, and *argc is decremented.
//
// No value is returned.  Instead, the Khulnasoft Mock flag variables are
// updated.
//
// Since Khulnasoft Test is needed for Khulnasoft Mock to work, this function
// also initializes Khulnasoft Test and parses its flags, if that hasn't
// been done.
KTEST_API_ void InitKhulnasoftMock(int* argc, char** argv);

// This overloaded version can be used in Windows programs compiled in
// UNICODE mode.
KTEST_API_ void InitKhulnasoftMock(int* argc, wchar_t** argv);

// This overloaded version can be used on Arduino/embedded platforms where
// there is no argc/argv.
KTEST_API_ void InitKhulnasoftMock();

}  // namespace testing

#endif  // KHULNASOFTMOCK_INCLUDE_KMOCK_KMOCK_H_
