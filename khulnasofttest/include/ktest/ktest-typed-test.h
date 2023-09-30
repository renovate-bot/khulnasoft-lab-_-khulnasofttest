// Copyright 2008 Khulnasoft Inc.
// All Rights Reserved.
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

// IWYU pragma: private, include "ktest/ktest.h"
// IWYU pragma: friend ktest/.*
// IWYU pragma: friend kmock/.*

#ifndef KHULNASOFTTEST_INCLUDE_KTEST_KTEST_TYPED_TEST_H_
#define KHULNASOFTTEST_INCLUDE_KTEST_KTEST_TYPED_TEST_H_

// This header implements typed tests and type-parameterized tests.

// Typed (aka type-driven) tests repeat the same test for types in a
// list.  You must know which types you want to test with when writing
// typed tests. Here's how you do it:

#if 0

// First, define a fixture class template.  It should be parameterized
// by a type.  Remember to derive it from testing::Test.
template <typename T>
class FooTest : public testing::Test {
 public:
  ...
  typedef std::list<T> List;
  static T shared_;
  T value_;
};

// Next, associate a list of types with the test suite, which will be
// repeated for each type in the list.  The typedef is necessary for
// the macro to parse correctly.
typedef testing::Types<char, int, unsigned int> MyTypes;
TYPED_TEST_SUITE(FooTest, MyTypes);

// If the type list contains only one type, you can write that type
// directly without Types<...>:
//   TYPED_TEST_SUITE(FooTest, int);

// Then, use TYPED_TEST() instead of TEST_F() to define as many typed
// tests for this test suite as you want.
TYPED_TEST(FooTest, DoesBlah) {
  // Inside a test, refer to the special name TypeParam to get the type
  // parameter.  Since we are inside a derived class template, C++ requires
  // us to visit the members of FooTest via 'this'.
  TypeParam n = this->value_;

  // To visit static members of the fixture, add the TestFixture::
  // prefix.
  n += TestFixture::shared_;

  // To refer to typedefs in the fixture, add the "typename
  // TestFixture::" prefix.
  typename TestFixture::List values;
  values.push_back(n);
  ...
}

TYPED_TEST(FooTest, HasPropertyA) { ... }

// TYPED_TEST_SUITE takes an optional third argument which allows to specify a
// class that generates custom test name suffixes based on the type. This should
// be a class which has a static template function GetName(int index) returning
// a string for each type. The provided integer index equals the index of the
// type in the provided type list. In many cases the index can be ignored.
//
// For example:
//   class MyTypeNames {
//    public:
//     template <typename T>
//     static std::string GetName(int) {
//       if (std::is_same<T, char>()) return "char";
//       if (std::is_same<T, int>()) return "int";
//       if (std::is_same<T, unsigned int>()) return "unsignedInt";
//     }
//   };
//   TYPED_TEST_SUITE(FooTest, MyTypes, MyTypeNames);

#endif  // 0

// Type-parameterized tests are abstract test patterns parameterized
// by a type.  Compared with typed tests, type-parameterized tests
// allow you to define the test pattern without knowing what the type
// parameters are.  The defined pattern can be instantiated with
// different types any number of times, in any number of translation
// units.
//
// If you are designing an interface or concept, you can define a
// suite of type-parameterized tests to verify properties that any
// valid implementation of the interface/concept should have.  Then,
// each implementation can easily instantiate the test suite to verify
// that it conforms to the requirements, without having to write
// similar tests repeatedly.  Here's an example:

#if 0

// First, define a fixture class template.  It should be parameterized
// by a type.  Remember to derive it from testing::Test.
template <typename T>
class FooTest : public testing::Test {
  ...
};

// Next, declare that you will define a type-parameterized test suite
// (the _P suffix is for "parameterized" or "pattern", whichever you
// prefer):
TYPED_TEST_SUITE_P(FooTest);

// Then, use TYPED_TEST_P() to define as many type-parameterized tests
// for this type-parameterized test suite as you want.
TYPED_TEST_P(FooTest, DoesBlah) {
  // Inside a test, refer to TypeParam to get the type parameter.
  TypeParam n = 0;
  ...
}

TYPED_TEST_P(FooTest, HasPropertyA) { ... }

// Now the tricky part: you need to register all test patterns before
// you can instantiate them.  The first argument of the macro is the
// test suite name; the rest are the names of the tests in this test
// case.
REGISTER_TYPED_TEST_SUITE_P(FooTest,
                            DoesBlah, HasPropertyA);

// Finally, you are free to instantiate the pattern with the types you
// want.  If you put the above code in a header file, you can #include
// it in multiple C++ source files and instantiate it multiple times.
//
// To distinguish different instances of the pattern, the first
// argument to the INSTANTIATE_* macro is a prefix that will be added
// to the actual test suite name.  Remember to pick unique prefixes for
// different instances.
typedef testing::Types<char, int, unsigned int> MyTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(My, FooTest, MyTypes);

// If the type list contains only one type, you can write that type
// directly without Types<...>:
//   INSTANTIATE_TYPED_TEST_SUITE_P(My, FooTest, int);
//
// Similar to the optional argument of TYPED_TEST_SUITE above,
// INSTANTIATE_TEST_SUITE_P takes an optional fourth argument which allows to
// generate custom names.
//   INSTANTIATE_TYPED_TEST_SUITE_P(My, FooTest, MyTypes, MyTypeNames);

#endif  // 0

#include "ktest/internal/ktest-internal.h"
#include "ktest/internal/ktest-port.h"
#include "ktest/internal/ktest-type-util.h"

// Implements typed tests.

// INTERNAL IMPLEMENTATION - DO NOT USE IN USER CODE.
//
// Expands to the name of the typedef for the type parameters of the
// given test suite.
#define KTEST_TYPE_PARAMS_(TestSuiteName) ktest_type_params_##TestSuiteName##_

// Expands to the name of the typedef for the NameGenerator, responsible for
// creating the suffixes of the name.
#define KTEST_NAME_GENERATOR_(TestSuiteName) \
  ktest_type_params_##TestSuiteName##_NameGenerator

#define TYPED_TEST_SUITE(CaseName, Types, ...)                          \
  typedef ::testing::internal::GenerateTypeList<Types>::type            \
      KTEST_TYPE_PARAMS_(CaseName);                                     \
  typedef ::testing::internal::NameGeneratorSelector<__VA_ARGS__>::type \
  KTEST_NAME_GENERATOR_(CaseName)

#define TYPED_TEST(CaseName, TestName)                                        \
  static_assert(sizeof(KTEST_STRINGIFY_(TestName)) > 1,                       \
                "test-name must not be empty");                               \
  template <typename ktest_TypeParam_>                                        \
  class KTEST_TEST_CLASS_NAME_(CaseName, TestName)                            \
      : public CaseName<ktest_TypeParam_> {                                   \
   private:                                                                   \
    typedef CaseName<ktest_TypeParam_> TestFixture;                           \
    typedef ktest_TypeParam_ TypeParam;                                       \
    void TestBody() override;                                                 \
  };                                                                          \
  static bool ktest_##CaseName##_##TestName##_registered_                     \
      KTEST_ATTRIBUTE_UNUSED_ = ::testing::internal::TypeParameterizedTest<   \
          CaseName,                                                           \
          ::testing::internal::TemplateSel<KTEST_TEST_CLASS_NAME_(CaseName,   \
                                                                  TestName)>, \
          KTEST_TYPE_PARAMS_(                                                 \
              CaseName)>::Register("",                                        \
                                   ::testing::internal::CodeLocation(         \
                                       __FILE__, __LINE__),                   \
                                   KTEST_STRINGIFY_(CaseName),                \
                                   KTEST_STRINGIFY_(TestName), 0,             \
                                   ::testing::internal::GenerateNames<        \
                                       KTEST_NAME_GENERATOR_(CaseName),       \
                                       KTEST_TYPE_PARAMS_(CaseName)>());      \
  template <typename ktest_TypeParam_>                                        \
  void KTEST_TEST_CLASS_NAME_(CaseName,                                       \
                              TestName)<ktest_TypeParam_>::TestBody()

// Legacy API is deprecated but still available
#ifndef KTEST_REMOVE_LEGACY_TEST_CASEAPI_
#define TYPED_TEST_CASE                                                \
  static_assert(::testing::internal::TypedTestCaseIsDeprecated(), ""); \
  TYPED_TEST_SUITE
#endif  // KTEST_REMOVE_LEGACY_TEST_CASEAPI_

// Implements type-parameterized tests.

// INTERNAL IMPLEMENTATION - DO NOT USE IN USER CODE.
//
// Expands to the namespace name that the type-parameterized tests for
// the given type-parameterized test suite are defined in.  The exact
// name of the namespace is subject to change without notice.
#define KTEST_SUITE_NAMESPACE_(TestSuiteName) ktest_suite_##TestSuiteName##_

// INTERNAL IMPLEMENTATION - DO NOT USE IN USER CODE.
//
// Expands to the name of the variable used to remember the names of
// the defined tests in the given test suite.
#define KTEST_TYPED_TEST_SUITE_P_STATE_(TestSuiteName) \
  ktest_typed_test_suite_p_state_##TestSuiteName##_

// INTERNAL IMPLEMENTATION - DO NOT USE IN USER CODE DIRECTLY.
//
// Expands to the name of the variable used to remember the names of
// the registered tests in the given test suite.
#define KTEST_REGISTERED_TEST_NAMES_(TestSuiteName) \
  ktest_registered_test_names_##TestSuiteName##_

// The variables defined in the type-parameterized test macros are
// static as typically these macros are used in a .h file that can be
// #included in multiple translation units linked together.
#define TYPED_TEST_SUITE_P(SuiteName)              \
  static ::testing::internal::TypedTestSuitePState \
  KTEST_TYPED_TEST_SUITE_P_STATE_(SuiteName)

// Legacy API is deprecated but still available
#ifndef KTEST_REMOVE_LEGACY_TEST_CASEAPI_
#define TYPED_TEST_CASE_P                                                 \
  static_assert(::testing::internal::TypedTestCase_P_IsDeprecated(), ""); \
  TYPED_TEST_SUITE_P
#endif  // KTEST_REMOVE_LEGACY_TEST_CASEAPI_

#define TYPED_TEST_P(SuiteName, TestName)                           \
  namespace KTEST_SUITE_NAMESPACE_(SuiteName) {                     \
  template <typename ktest_TypeParam_>                              \
  class TestName : public SuiteName<ktest_TypeParam_> {             \
   private:                                                         \
    typedef SuiteName<ktest_TypeParam_> TestFixture;                \
    typedef ktest_TypeParam_ TypeParam;                             \
    void TestBody() override;                                       \
  };                                                                \
  static bool ktest_##TestName##_defined_ KTEST_ATTRIBUTE_UNUSED_ = \
      KTEST_TYPED_TEST_SUITE_P_STATE_(SuiteName).AddTestName(       \
          __FILE__, __LINE__, KTEST_STRINGIFY_(SuiteName),          \
          KTEST_STRINGIFY_(TestName));                              \
  }                                                                 \
  template <typename ktest_TypeParam_>                              \
  void KTEST_SUITE_NAMESPACE_(                                      \
      SuiteName)::TestName<ktest_TypeParam_>::TestBody()

// Note: this won't work correctly if the trailing arguments are macros.
#define REGISTER_TYPED_TEST_SUITE_P(SuiteName, ...)                         \
  namespace KTEST_SUITE_NAMESPACE_(SuiteName) {                             \
  typedef ::testing::internal::Templates<__VA_ARGS__> ktest_AllTests_;      \
  }                                                                         \
  static const char* const KTEST_REGISTERED_TEST_NAMES_(                    \
      SuiteName) KTEST_ATTRIBUTE_UNUSED_ =                                  \
      KTEST_TYPED_TEST_SUITE_P_STATE_(SuiteName).VerifyRegisteredTestNames( \
          KTEST_STRINGIFY_(SuiteName), __FILE__, __LINE__, #__VA_ARGS__)

// Legacy API is deprecated but still available
#ifndef KTEST_REMOVE_LEGACY_TEST_CASEAPI_
#define REGISTER_TYPED_TEST_CASE_P                                           \
  static_assert(::testing::internal::RegisterTypedTestCase_P_IsDeprecated(), \
                "");                                                         \
  REGISTER_TYPED_TEST_SUITE_P
#endif  // KTEST_REMOVE_LEGACY_TEST_CASEAPI_

#define INSTANTIATE_TYPED_TEST_SUITE_P(Prefix, SuiteName, Types, ...)     \
  static_assert(sizeof(KTEST_STRINGIFY_(Prefix)) > 1,                     \
                "test-suit-prefix must not be empty");                    \
  static bool ktest_##Prefix##_##SuiteName KTEST_ATTRIBUTE_UNUSED_ =      \
      ::testing::internal::TypeParameterizedTestSuite<                    \
          SuiteName, KTEST_SUITE_NAMESPACE_(SuiteName)::ktest_AllTests_,  \
          ::testing::internal::GenerateTypeList<Types>::type>::           \
          Register(KTEST_STRINGIFY_(Prefix),                              \
                   ::testing::internal::CodeLocation(__FILE__, __LINE__), \
                   &KTEST_TYPED_TEST_SUITE_P_STATE_(SuiteName),           \
                   KTEST_STRINGIFY_(SuiteName),                           \
                   KTEST_REGISTERED_TEST_NAMES_(SuiteName),               \
                   ::testing::internal::GenerateNames<                    \
                       ::testing::internal::NameGeneratorSelector<        \
                           __VA_ARGS__>::type,                            \
                       ::testing::internal::GenerateTypeList<Types>::type>())

// Legacy API is deprecated but still available
#ifndef KTEST_REMOVE_LEGACY_TEST_CASEAPI_
#define INSTANTIATE_TYPED_TEST_CASE_P                                      \
  static_assert(                                                           \
      ::testing::internal::InstantiateTypedTestCase_P_IsDeprecated(), ""); \
  INSTANTIATE_TYPED_TEST_SUITE_P
#endif  // KTEST_REMOVE_LEGACY_TEST_CASEAPI_

#endif  // KHULNASOFTTEST_INCLUDE_KTEST_KTEST_TYPED_TEST_H_
