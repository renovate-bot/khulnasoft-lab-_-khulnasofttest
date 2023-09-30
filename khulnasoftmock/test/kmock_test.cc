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

// Khulnasoft Mock - a framework for writing C++ mock classes.
//
// This file tests code in kmock.cc.

#include "kmock/kmock.h"

#include <string>

#include "ktest/ktest.h"
#include "ktest/internal/custom/ktest.h"

#if !defined(KTEST_CUSTOM_INIT_KHULNASOFT_TEST_FUNCTION_)

using testing::InitKhulnasoftMock;

// Verifies that calling InitKhulnasoftMock() on argv results in new_argv,
// and the kmock_verbose flag's value is set to expected_kmock_verbose.
template <typename Char, int M, int N>
void TestInitKhulnasoftMock(const Char* (&argv)[M], const Char* (&new_argv)[N],
                        const ::std::string& expected_kmock_verbose) {
  const ::std::string old_verbose = KMOCK_FLAG_GET(verbose);

  int argc = M - 1;
  InitKhulnasoftMock(&argc, const_cast<Char**>(argv));
  ASSERT_EQ(N - 1, argc) << "The new argv has wrong number of elements.";

  for (int i = 0; i < N; i++) {
    EXPECT_STREQ(new_argv[i], argv[i]);
  }

  EXPECT_EQ(expected_kmock_verbose, KMOCK_FLAG_GET(verbose));
  KMOCK_FLAG_SET(verbose, old_verbose);  // Restores the kmock_verbose flag.
}

TEST(InitKhulnasoftMockTest, ParsesInvalidCommandLine) {
  const char* argv[] = {nullptr};

  const char* new_argv[] = {nullptr};

  TestInitKhulnasoftMock(argv, new_argv, KMOCK_FLAG_GET(verbose));
}

TEST(InitKhulnasoftMockTest, ParsesEmptyCommandLine) {
  const char* argv[] = {"foo.exe", nullptr};

  const char* new_argv[] = {"foo.exe", nullptr};

  TestInitKhulnasoftMock(argv, new_argv, KMOCK_FLAG_GET(verbose));
}

TEST(InitKhulnasoftMockTest, ParsesSingleFlag) {
  const char* argv[] = {"foo.exe", "--kmock_verbose=info", nullptr};

  const char* new_argv[] = {"foo.exe", nullptr};

  TestInitKhulnasoftMock(argv, new_argv, "info");
}

TEST(InitKhulnasoftMockTest, ParsesMultipleFlags) {
  int old_default_behavior = KMOCK_FLAG_GET(default_mock_behavior);
  const wchar_t* argv[] = {L"foo.exe", L"--kmock_verbose=info",
                           L"--kmock_default_mock_behavior=2", nullptr};

  const wchar_t* new_argv[] = {L"foo.exe", nullptr};

  TestInitKhulnasoftMock(argv, new_argv, "info");
  EXPECT_EQ(2, KMOCK_FLAG_GET(default_mock_behavior));
  EXPECT_NE(2, old_default_behavior);
  KMOCK_FLAG_SET(default_mock_behavior, old_default_behavior);
}

TEST(InitKhulnasoftMockTest, ParsesUnrecognizedFlag) {
  const char* argv[] = {"foo.exe", "--non_kmock_flag=blah", nullptr};

  const char* new_argv[] = {"foo.exe", "--non_kmock_flag=blah", nullptr};

  TestInitKhulnasoftMock(argv, new_argv, KMOCK_FLAG_GET(verbose));
}

TEST(InitKhulnasoftMockTest, ParsesKhulnasoftMockFlagAndUnrecognizedFlag) {
  const char* argv[] = {"foo.exe", "--non_kmock_flag=blah",
                        "--kmock_verbose=error", nullptr};

  const char* new_argv[] = {"foo.exe", "--non_kmock_flag=blah", nullptr};

  TestInitKhulnasoftMock(argv, new_argv, "error");
}

TEST(WideInitKhulnasoftMockTest, ParsesInvalidCommandLine) {
  const wchar_t* argv[] = {nullptr};

  const wchar_t* new_argv[] = {nullptr};

  TestInitKhulnasoftMock(argv, new_argv, KMOCK_FLAG_GET(verbose));
}

TEST(WideInitKhulnasoftMockTest, ParsesEmptyCommandLine) {
  const wchar_t* argv[] = {L"foo.exe", nullptr};

  const wchar_t* new_argv[] = {L"foo.exe", nullptr};

  TestInitKhulnasoftMock(argv, new_argv, KMOCK_FLAG_GET(verbose));
}

TEST(WideInitKhulnasoftMockTest, ParsesSingleFlag) {
  const wchar_t* argv[] = {L"foo.exe", L"--kmock_verbose=info", nullptr};

  const wchar_t* new_argv[] = {L"foo.exe", nullptr};

  TestInitKhulnasoftMock(argv, new_argv, "info");
}

TEST(WideInitKhulnasoftMockTest, ParsesMultipleFlags) {
  int old_default_behavior = KMOCK_FLAG_GET(default_mock_behavior);
  const wchar_t* argv[] = {L"foo.exe", L"--kmock_verbose=info",
                           L"--kmock_default_mock_behavior=2", nullptr};

  const wchar_t* new_argv[] = {L"foo.exe", nullptr};

  TestInitKhulnasoftMock(argv, new_argv, "info");
  EXPECT_EQ(2, KMOCK_FLAG_GET(default_mock_behavior));
  EXPECT_NE(2, old_default_behavior);
  KMOCK_FLAG_SET(default_mock_behavior, old_default_behavior);
}

TEST(WideInitKhulnasoftMockTest, ParsesUnrecognizedFlag) {
  const wchar_t* argv[] = {L"foo.exe", L"--non_kmock_flag=blah", nullptr};

  const wchar_t* new_argv[] = {L"foo.exe", L"--non_kmock_flag=blah", nullptr};

  TestInitKhulnasoftMock(argv, new_argv, KMOCK_FLAG_GET(verbose));
}

TEST(WideInitKhulnasoftMockTest, ParsesKhulnasoftMockFlagAndUnrecognizedFlag) {
  const wchar_t* argv[] = {L"foo.exe", L"--non_kmock_flag=blah",
                           L"--kmock_verbose=error", nullptr};

  const wchar_t* new_argv[] = {L"foo.exe", L"--non_kmock_flag=blah", nullptr};

  TestInitKhulnasoftMock(argv, new_argv, "error");
}

#endif  // !defined(KTEST_CUSTOM_INIT_KHULNASOFT_TEST_FUNCTION_)

// Makes sure Khulnasoft Mock flags can be accessed in code.
TEST(FlagTest, IsAccessibleInCode) {
  bool dummy =
      KMOCK_FLAG_GET(catch_leaked_mocks) && KMOCK_FLAG_GET(verbose).empty();
  (void)dummy;  // Avoids the "unused local variable" warning.
}
