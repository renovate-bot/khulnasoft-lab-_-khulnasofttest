// Copyright 2005, Khulnasoft Inc.
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

// The Khulnasoft C++ Testing and Mocking Framework (Khulnasoft Test)
//
// This header file defines internal utilities needed for implementing
// death tests.  They are subject to change without notice.

// IWYU pragma: private, include "ktest/ktest.h"
// IWYU pragma: friend ktest/.*
// IWYU pragma: friend kmock/.*

#ifndef KHULNASOFTTEST_INCLUDE_KTEST_INTERNAL_KTEST_DEATH_TEST_INTERNAL_H_
#define KHULNASOFTTEST_INCLUDE_KTEST_INTERNAL_KTEST_DEATH_TEST_INTERNAL_H_

#include <stdio.h>

#include <memory>
#include <string>

#include "ktest/ktest-matchers.h"
#include "ktest/internal/ktest-internal.h"

KTEST_DECLARE_string_(internal_run_death_test);

namespace testing {
namespace internal {

// Names of the flags (needed for parsing Khulnasoft Test flags).
const char kDeathTestStyleFlag[] = "death_test_style";
const char kDeathTestUseFork[] = "death_test_use_fork";
const char kInternalRunDeathTestFlag[] = "internal_run_death_test";

#ifdef KTEST_HAS_DEATH_TEST

KTEST_DISABLE_MSC_WARNINGS_PUSH_(4251 \
/* class A needs to have dll-interface to be used by clients of class B */)

// DeathTest is a class that hides much of the complexity of the
// KTEST_DEATH_TEST_ macro.  It is abstract; its static Create method
// returns a concrete class that depends on the prevailing death test
// style, as defined by the --ktest_death_test_style and/or
// --ktest_internal_run_death_test flags.

// In describing the results of death tests, these terms are used with
// the corresponding definitions:
//
// exit status:  The integer exit information in the format specified
//               by wait(2)
// exit code:    The integer code passed to exit(3), _exit(2), or
//               returned from main()
class KTEST_API_ DeathTest {
 public:
  // Create returns false if there was an error determining the
  // appropriate action to take for the current death test; for example,
  // if the ktest_death_test_style flag is set to an invalid value.
  // The LastMessage method will return a more detailed message in that
  // case.  Otherwise, the DeathTest pointer pointed to by the "test"
  // argument is set.  If the death test should be skipped, the pointer
  // is set to NULL; otherwise, it is set to the address of a new concrete
  // DeathTest object that controls the execution of the current test.
  static bool Create(const char* statement, Matcher<const std::string&> matcher,
                     const char* file, int line, DeathTest** test);
  DeathTest();
  virtual ~DeathTest() = default;

  // A helper class that aborts a death test when it's deleted.
  class ReturnSentinel {
   public:
    explicit ReturnSentinel(DeathTest* test) : test_(test) {}
    ~ReturnSentinel() { test_->Abort(TEST_ENCOUNTERED_RETURN_STATEMENT); }

   private:
    DeathTest* const test_;
    ReturnSentinel(const ReturnSentinel&) = delete;
    ReturnSentinel& operator=(const ReturnSentinel&) = delete;
  };

  // An enumeration of possible roles that may be taken when a death
  // test is encountered.  EXECUTE means that the death test logic should
  // be executed immediately.  OVERSEE means that the program should prepare
  // the appropriate environment for a child process to execute the death
  // test, then wait for it to complete.
  enum TestRole { OVERSEE_TEST, EXECUTE_TEST };

  // An enumeration of the three reasons that a test might be aborted.
  enum AbortReason {
    TEST_ENCOUNTERED_RETURN_STATEMENT,
    TEST_THREW_EXCEPTION,
    TEST_DID_NOT_DIE
  };

  // Assumes one of the above roles.
  virtual TestRole AssumeRole() = 0;

  // Waits for the death test to finish and returns its status.
  virtual int Wait() = 0;

  // Returns true if the death test passed; that is, the test process
  // exited during the test, its exit status matches a user-supplied
  // predicate, and its stderr output matches a user-supplied regular
  // expression.
  // The user-supplied predicate may be a macro expression rather
  // than a function pointer or functor, or else Wait and Passed could
  // be combined.
  virtual bool Passed(bool exit_status_ok) = 0;

  // Signals that the death test did not die as expected.
  virtual void Abort(AbortReason reason) = 0;

  // Returns a human-readable outcome message regarding the outcome of
  // the last death test.
  static const char* LastMessage();

  static void set_last_death_test_message(const std::string& message);

 private:
  // A string containing a description of the outcome of the last death test.
  static std::string last_death_test_message_;

  DeathTest(const DeathTest&) = delete;
  DeathTest& operator=(const DeathTest&) = delete;
};

KTEST_DISABLE_MSC_WARNINGS_POP_()  //  4251

// Factory interface for death tests.  May be mocked out for testing.
class DeathTestFactory {
 public:
  virtual ~DeathTestFactory() = default;
  virtual bool Create(const char* statement,
                      Matcher<const std::string&> matcher, const char* file,
                      int line, DeathTest** test) = 0;
};

// A concrete DeathTestFactory implementation for normal use.
class DefaultDeathTestFactory : public DeathTestFactory {
 public:
  bool Create(const char* statement, Matcher<const std::string&> matcher,
              const char* file, int line, DeathTest** test) override;
};

// Returns true if exit_status describes a process that was terminated
// by a signal, or exited normally with a nonzero exit code.
KTEST_API_ bool ExitedUnsuccessfully(int exit_status);

// A string passed to EXPECT_DEATH (etc.) is caught by one of these overloads
// and interpreted as a regex (rather than an Eq matcher) for legacy
// compatibility.
inline Matcher<const ::std::string&> MakeDeathTestMatcher(
    ::testing::internal::RE regex) {
  return ContainsRegex(regex.pattern());
}
inline Matcher<const ::std::string&> MakeDeathTestMatcher(const char* regex) {
  return ContainsRegex(regex);
}
inline Matcher<const ::std::string&> MakeDeathTestMatcher(
    const ::std::string& regex) {
  return ContainsRegex(regex);
}

// If a Matcher<const ::std::string&> is passed to EXPECT_DEATH (etc.), it's
// used directly.
inline Matcher<const ::std::string&> MakeDeathTestMatcher(
    Matcher<const ::std::string&> matcher) {
  return matcher;
}

// Traps C++ exceptions escaping statement and reports them as test
// failures. Note that trapping SEH exceptions is not implemented here.
#if KTEST_HAS_EXCEPTIONS
#define KTEST_EXECUTE_DEATH_TEST_STATEMENT_(statement, death_test)           \
  try {                                                                      \
    KTEST_SUPPRESS_UNREACHABLE_CODE_WARNING_BELOW_(statement);               \
  } catch (const ::std::exception& ktest_exception) {                        \
    fprintf(                                                                 \
        stderr,                                                              \
        "\n%s: Caught std::exception-derived exception escaping the "        \
        "death test statement. Exception message: %s\n",                     \
        ::testing::internal::FormatFileLocation(__FILE__, __LINE__).c_str(), \
        ktest_exception.what());                                             \
    fflush(stderr);                                                          \
    death_test->Abort(::testing::internal::DeathTest::TEST_THREW_EXCEPTION); \
  } catch (...) {                                                            \
    death_test->Abort(::testing::internal::DeathTest::TEST_THREW_EXCEPTION); \
  }

#else
#define KTEST_EXECUTE_DEATH_TEST_STATEMENT_(statement, death_test) \
  KTEST_SUPPRESS_UNREACHABLE_CODE_WARNING_BELOW_(statement)

#endif

// This macro is for implementing ASSERT_DEATH*, EXPECT_DEATH*,
// ASSERT_EXIT*, and EXPECT_EXIT*.
#define KTEST_DEATH_TEST_(statement, predicate, regex_or_matcher, fail)        \
  KTEST_AMBIGUOUS_ELSE_BLOCKER_                                                \
  if (::testing::internal::AlwaysTrue()) {                                     \
    ::testing::internal::DeathTest* ktest_dt;                                  \
    if (!::testing::internal::DeathTest::Create(                               \
            #statement,                                                        \
            ::testing::internal::MakeDeathTestMatcher(regex_or_matcher),       \
            __FILE__, __LINE__, &ktest_dt)) {                                  \
      goto KTEST_CONCAT_TOKEN_(ktest_label_, __LINE__);                        \
    }                                                                          \
    if (ktest_dt != nullptr) {                                                 \
      std::unique_ptr< ::testing::internal::DeathTest> ktest_dt_ptr(ktest_dt); \
      switch (ktest_dt->AssumeRole()) {                                        \
        case ::testing::internal::DeathTest::OVERSEE_TEST:                     \
          if (!ktest_dt->Passed(predicate(ktest_dt->Wait()))) {                \
            goto KTEST_CONCAT_TOKEN_(ktest_label_, __LINE__);                  \
          }                                                                    \
          break;                                                               \
        case ::testing::internal::DeathTest::EXECUTE_TEST: {                   \
          const ::testing::internal::DeathTest::ReturnSentinel ktest_sentinel( \
              ktest_dt);                                                       \
          KTEST_EXECUTE_DEATH_TEST_STATEMENT_(statement, ktest_dt);            \
          ktest_dt->Abort(::testing::internal::DeathTest::TEST_DID_NOT_DIE);   \
          break;                                                               \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  } else                                                                       \
    KTEST_CONCAT_TOKEN_(ktest_label_, __LINE__)                                \
        : fail(::testing::internal::DeathTest::LastMessage())
// The symbol "fail" here expands to something into which a message
// can be streamed.

// This macro is for implementing ASSERT/EXPECT_DEBUG_DEATH when compiled in
// NDEBUG mode. In this case we need the statements to be executed and the macro
// must accept a streamed message even though the message is never printed.
// The regex object is not evaluated, but it is used to prevent "unused"
// warnings and to avoid an expression that doesn't compile in debug mode.
#define KTEST_EXECUTE_STATEMENT_(statement, regex_or_matcher)    \
  KTEST_AMBIGUOUS_ELSE_BLOCKER_                                  \
  if (::testing::internal::AlwaysTrue()) {                       \
    KTEST_SUPPRESS_UNREACHABLE_CODE_WARNING_BELOW_(statement);   \
  } else if (!::testing::internal::AlwaysTrue()) {               \
    ::testing::internal::MakeDeathTestMatcher(regex_or_matcher); \
  } else                                                         \
    ::testing::Message()

// A class representing the parsed contents of the
// --ktest_internal_run_death_test flag, as it existed when
// RUN_ALL_TESTS was called.
class InternalRunDeathTestFlag {
 public:
  InternalRunDeathTestFlag(const std::string& a_file, int a_line, int an_index,
                           int a_write_fd)
      : file_(a_file), line_(a_line), index_(an_index), write_fd_(a_write_fd) {}

  ~InternalRunDeathTestFlag() {
    if (write_fd_ >= 0) posix::Close(write_fd_);
  }

  const std::string& file() const { return file_; }
  int line() const { return line_; }
  int index() const { return index_; }
  int write_fd() const { return write_fd_; }

 private:
  std::string file_;
  int line_;
  int index_;
  int write_fd_;

  InternalRunDeathTestFlag(const InternalRunDeathTestFlag&) = delete;
  InternalRunDeathTestFlag& operator=(const InternalRunDeathTestFlag&) = delete;
};

// Returns a newly created InternalRunDeathTestFlag object with fields
// initialized from the KTEST_FLAG(internal_run_death_test) flag if
// the flag is specified; otherwise returns NULL.
InternalRunDeathTestFlag* ParseInternalRunDeathTestFlag();

#endif  // KTEST_HAS_DEATH_TEST

}  // namespace internal
}  // namespace testing

#endif  // KHULNASOFTTEST_INCLUDE_KTEST_INTERNAL_KTEST_DEATH_TEST_INTERNAL_H_
