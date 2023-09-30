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

// Tests the --ktest_repeat=number flag.

#include <stdlib.h>

#include <iostream>

#include "ktest/ktest.h"
#include "src/ktest-internal-inl.h"

namespace {

// We need this when we are testing Khulnasoft Test itself and therefore
// cannot use Khulnasoft Test assertions.
#define KTEST_CHECK_INT_EQ_(expected, actual)                      \
  do {                                                             \
    const int expected_val = (expected);                           \
    const int actual_val = (actual);                               \
    if (::testing::internal::IsTrue(expected_val != actual_val)) { \
      ::std::cout << "Value of: " #actual "\n"                     \
                  << "  Actual: " << actual_val << "\n"            \
                  << "Expected: " #expected "\n"                   \
                  << "Which is: " << expected_val << "\n";         \
      ::testing::internal::posix::Abort();                         \
    }                                                              \
  } while (::testing::internal::AlwaysFalse())

// Used for verifying that global environment set-up and tear-down are
// inside the --ktest_repeat loop.

int g_environment_set_up_count = 0;
int g_environment_tear_down_count = 0;

class MyEnvironment : public testing::Environment {
 public:
  MyEnvironment() = default;
  void SetUp() override { g_environment_set_up_count++; }
  void TearDown() override { g_environment_tear_down_count++; }
};

// A test that should fail.

int g_should_fail_count = 0;

TEST(FooTest, ShouldFail) {
  g_should_fail_count++;
  EXPECT_EQ(0, 1) << "Expected failure.";
}

// A test that should pass.

int g_should_pass_count = 0;

TEST(FooTest, ShouldPass) { g_should_pass_count++; }

// A test that contains a thread-safe death test and a fast death
// test.  It should pass.

int g_death_test_count = 0;

TEST(BarDeathTest, ThreadSafeAndFast) {
  g_death_test_count++;

  KTEST_FLAG_SET(death_test_style, "threadsafe");
  EXPECT_DEATH_IF_SUPPORTED(::testing::internal::posix::Abort(), "");

  KTEST_FLAG_SET(death_test_style, "fast");
  EXPECT_DEATH_IF_SUPPORTED(::testing::internal::posix::Abort(), "");
}

int g_param_test_count = 0;

const int kNumberOfParamTests = 10;

class MyParamTest : public testing::TestWithParam<int> {};

TEST_P(MyParamTest, ShouldPass) {
  KTEST_CHECK_INT_EQ_(g_param_test_count % kNumberOfParamTests, GetParam());
  g_param_test_count++;
}
INSTANTIATE_TEST_SUITE_P(MyParamSequence, MyParamTest,
                         testing::Range(0, kNumberOfParamTests));

// Resets the count for each test.
void ResetCounts() {
  g_environment_set_up_count = 0;
  g_environment_tear_down_count = 0;
  g_should_fail_count = 0;
  g_should_pass_count = 0;
  g_death_test_count = 0;
  g_param_test_count = 0;
}

// Checks that the count for each test is expected.
void CheckCounts(int expected) {
  KTEST_CHECK_INT_EQ_(expected, g_environment_set_up_count);
  KTEST_CHECK_INT_EQ_(expected, g_environment_tear_down_count);
  KTEST_CHECK_INT_EQ_(expected, g_should_fail_count);
  KTEST_CHECK_INT_EQ_(expected, g_should_pass_count);
  KTEST_CHECK_INT_EQ_(expected, g_death_test_count);
  KTEST_CHECK_INT_EQ_(expected * kNumberOfParamTests, g_param_test_count);
}

// Tests the behavior of Khulnasoft Test when --ktest_repeat is not specified.
void TestRepeatUnspecified() {
  ResetCounts();
  KTEST_CHECK_INT_EQ_(1, RUN_ALL_TESTS());
  CheckCounts(1);
}

// Tests the behavior of Khulnasoft Test when --ktest_repeat has the given value.
void TestRepeat(int repeat) {
  KTEST_FLAG_SET(repeat, repeat);
  KTEST_FLAG_SET(recreate_environments_when_repeating, true);

  ResetCounts();
  KTEST_CHECK_INT_EQ_(repeat > 0 ? 1 : 0, RUN_ALL_TESTS());
  CheckCounts(repeat);
}

// Tests using --ktest_repeat when --ktest_filter specifies an empty
// set of tests.
void TestRepeatWithEmptyFilter(int repeat) {
  KTEST_FLAG_SET(repeat, repeat);
  KTEST_FLAG_SET(recreate_environments_when_repeating, true);
  KTEST_FLAG_SET(filter, "None");

  ResetCounts();
  KTEST_CHECK_INT_EQ_(0, RUN_ALL_TESTS());
  CheckCounts(0);
}

// Tests using --ktest_repeat when --ktest_filter specifies a set of
// successful tests.
void TestRepeatWithFilterForSuccessfulTests(int repeat) {
  KTEST_FLAG_SET(repeat, repeat);
  KTEST_FLAG_SET(recreate_environments_when_repeating, true);
  KTEST_FLAG_SET(filter, "*-*ShouldFail");

  ResetCounts();
  KTEST_CHECK_INT_EQ_(0, RUN_ALL_TESTS());
  KTEST_CHECK_INT_EQ_(repeat, g_environment_set_up_count);
  KTEST_CHECK_INT_EQ_(repeat, g_environment_tear_down_count);
  KTEST_CHECK_INT_EQ_(0, g_should_fail_count);
  KTEST_CHECK_INT_EQ_(repeat, g_should_pass_count);
  KTEST_CHECK_INT_EQ_(repeat, g_death_test_count);
  KTEST_CHECK_INT_EQ_(repeat * kNumberOfParamTests, g_param_test_count);
}

// Tests using --ktest_repeat when --ktest_filter specifies a set of
// failed tests.
void TestRepeatWithFilterForFailedTests(int repeat) {
  KTEST_FLAG_SET(repeat, repeat);
  KTEST_FLAG_SET(recreate_environments_when_repeating, true);
  KTEST_FLAG_SET(filter, "*ShouldFail");

  ResetCounts();
  KTEST_CHECK_INT_EQ_(1, RUN_ALL_TESTS());
  KTEST_CHECK_INT_EQ_(repeat, g_environment_set_up_count);
  KTEST_CHECK_INT_EQ_(repeat, g_environment_tear_down_count);
  KTEST_CHECK_INT_EQ_(repeat, g_should_fail_count);
  KTEST_CHECK_INT_EQ_(0, g_should_pass_count);
  KTEST_CHECK_INT_EQ_(0, g_death_test_count);
  KTEST_CHECK_INT_EQ_(0, g_param_test_count);
}

}  // namespace

int main(int argc, char **argv) {
  testing::InitKhulnasoftTest(&argc, argv);

  testing::AddGlobalTestEnvironment(new MyEnvironment);

  TestRepeatUnspecified();
  TestRepeat(0);
  TestRepeat(1);
  TestRepeat(5);

  TestRepeatWithEmptyFilter(2);
  TestRepeatWithEmptyFilter(3);

  TestRepeatWithFilterForSuccessfulTests(3);

  TestRepeatWithFilterForFailedTests(4);

  // It would be nice to verify that the tests indeed loop forever
  // when KTEST_FLAG(repeat) is negative, but this test will be quite
  // complicated to write.  Since this flag is for interactive
  // debugging only and doesn't affect the normal test result, such a
  // test would be an overkill.

  printf("PASS\n");
  return 0;
}
