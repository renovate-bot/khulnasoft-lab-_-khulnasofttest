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
//
// Khulnasoft Test UnitTestOptions tests
//
// This file tests classes and functions used internally by
// Khulnasoft Test.  They are subject to change without notice.
//
// This file is #included from ktest.cc, to avoid changing build or
// make-files on Windows and other platforms. Do not #include this file
// anywhere else!

#include <string>

#include "ktest/ktest.h"

#ifdef KTEST_OS_WINDOWS_MOBILE
#include <windows.h>
#elif defined(KTEST_OS_WINDOWS)
#include <direct.h>
#elif defined(KTEST_OS_OS2)
// For strcasecmp on OS/2
#include <strings.h>
#endif  // KTEST_OS_WINDOWS_MOBILE

#include "src/ktest-internal-inl.h"

namespace testing {
namespace internal {
namespace {

// Turns the given relative path into an absolute path.
FilePath GetAbsolutePathOf(const FilePath& relative_path) {
  return FilePath::ConcatPaths(FilePath::GetCurrentDir(), relative_path);
}

// Testing UnitTestOptions::GetOutputFormat/GetOutputFile.

TEST(XmlOutputTest, GetOutputFormatDefault) {
  KTEST_FLAG_SET(output, "");
  EXPECT_STREQ("", UnitTestOptions::GetOutputFormat().c_str());
}

TEST(XmlOutputTest, GetOutputFormat) {
  KTEST_FLAG_SET(output, "xml:filename");
  EXPECT_STREQ("xml", UnitTestOptions::GetOutputFormat().c_str());
}

TEST(XmlOutputTest, GetOutputFileDefault) {
  KTEST_FLAG_SET(output, "");
  EXPECT_EQ(GetAbsolutePathOf(FilePath("test_detail.xml")).string(),
            UnitTestOptions::GetAbsolutePathToOutputFile());
}

TEST(XmlOutputTest, GetOutputFileSingleFile) {
  KTEST_FLAG_SET(output, "xml:filename.abc");
  EXPECT_EQ(GetAbsolutePathOf(FilePath("filename.abc")).string(),
            UnitTestOptions::GetAbsolutePathToOutputFile());
}

TEST(XmlOutputTest, GetOutputFileFromDirectoryPath) {
  KTEST_FLAG_SET(output, "xml:path" KTEST_PATH_SEP_);
  const std::string expected_output_file =
      GetAbsolutePathOf(FilePath(std::string("path") + KTEST_PATH_SEP_ +
                                 GetCurrentExecutableName().string() + ".xml"))
          .string();
  const std::string& output_file =
      UnitTestOptions::GetAbsolutePathToOutputFile();
#ifdef KTEST_OS_WINDOWS
  EXPECT_STRCASEEQ(expected_output_file.c_str(), output_file.c_str());
#else
  EXPECT_EQ(expected_output_file, output_file.c_str());
#endif
}

TEST(OutputFileHelpersTest, GetCurrentExecutableName) {
  const std::string exe_str = GetCurrentExecutableName().string();
#ifdef KTEST_OS_WINDOWS
  const bool success =
      _strcmpi("khulnasofttest-options-test", exe_str.c_str()) == 0 ||
      _strcmpi("ktest-options-ex_test", exe_str.c_str()) == 0 ||
      _strcmpi("ktest_all_test", exe_str.c_str()) == 0 ||
      _strcmpi("ktest_dll_test", exe_str.c_str()) == 0;
#elif defined(KTEST_OS_OS2)
  const bool success =
      strcasecmp("khulnasofttest-options-test", exe_str.c_str()) == 0 ||
      strcasecmp("ktest-options-ex_test", exe_str.c_str()) == 0 ||
      strcasecmp("ktest_all_test", exe_str.c_str()) == 0 ||
      strcasecmp("ktest_dll_test", exe_str.c_str()) == 0;
#elif defined(KTEST_OS_FUCHSIA)
  const bool success = exe_str == "app";
#else
  const bool success =
      exe_str == "khulnasofttest-options-test" || exe_str == "ktest_all_test" ||
      exe_str == "lt-ktest_all_test" || exe_str == "ktest_dll_test";
#endif  // KTEST_OS_WINDOWS
  if (!success) FAIL() << "GetCurrentExecutableName() returns " << exe_str;
}

#ifndef KTEST_OS_FUCHSIA

class XmlOutputChangeDirTest : public Test {
 protected:
  void SetUp() override {
    original_working_dir_ = FilePath::GetCurrentDir();
    posix::ChDir("..");
    // This will make the test fail if run from the root directory.
    EXPECT_NE(original_working_dir_.string(),
              FilePath::GetCurrentDir().string());
  }

  void TearDown() override {
    posix::ChDir(original_working_dir_.string().c_str());
  }

  FilePath original_working_dir_;
};

TEST_F(XmlOutputChangeDirTest, PreserveOriginalWorkingDirWithDefault) {
  KTEST_FLAG_SET(output, "");
  EXPECT_EQ(
      FilePath::ConcatPaths(original_working_dir_, FilePath("test_detail.xml"))
          .string(),
      UnitTestOptions::GetAbsolutePathToOutputFile());
}

TEST_F(XmlOutputChangeDirTest, PreserveOriginalWorkingDirWithDefaultXML) {
  KTEST_FLAG_SET(output, "xml");
  EXPECT_EQ(
      FilePath::ConcatPaths(original_working_dir_, FilePath("test_detail.xml"))
          .string(),
      UnitTestOptions::GetAbsolutePathToOutputFile());
}

TEST_F(XmlOutputChangeDirTest, PreserveOriginalWorkingDirWithRelativeFile) {
  KTEST_FLAG_SET(output, "xml:filename.abc");
  EXPECT_EQ(
      FilePath::ConcatPaths(original_working_dir_, FilePath("filename.abc"))
          .string(),
      UnitTestOptions::GetAbsolutePathToOutputFile());
}

TEST_F(XmlOutputChangeDirTest, PreserveOriginalWorkingDirWithRelativePath) {
  KTEST_FLAG_SET(output, "xml:path" KTEST_PATH_SEP_);
  const std::string expected_output_file =
      FilePath::ConcatPaths(
          original_working_dir_,
          FilePath(std::string("path") + KTEST_PATH_SEP_ +
                   GetCurrentExecutableName().string() + ".xml"))
          .string();
  const std::string& output_file =
      UnitTestOptions::GetAbsolutePathToOutputFile();
#ifdef KTEST_OS_WINDOWS
  EXPECT_STRCASEEQ(expected_output_file.c_str(), output_file.c_str());
#else
  EXPECT_EQ(expected_output_file, output_file.c_str());
#endif
}

TEST_F(XmlOutputChangeDirTest, PreserveOriginalWorkingDirWithAbsoluteFile) {
#ifdef KTEST_OS_WINDOWS
  KTEST_FLAG_SET(output, "xml:c:\\tmp\\filename.abc");
  EXPECT_EQ(FilePath("c:\\tmp\\filename.abc").string(),
            UnitTestOptions::GetAbsolutePathToOutputFile());
#else
  KTEST_FLAG_SET(output, "xml:/tmp/filename.abc");
  EXPECT_EQ(FilePath("/tmp/filename.abc").string(),
            UnitTestOptions::GetAbsolutePathToOutputFile());
#endif
}

TEST_F(XmlOutputChangeDirTest, PreserveOriginalWorkingDirWithAbsolutePath) {
#ifdef KTEST_OS_WINDOWS
  const std::string path = "c:\\tmp\\";
#else
  const std::string path = "/tmp/";
#endif

  KTEST_FLAG_SET(output, "xml:" + path);
  const std::string expected_output_file =
      path + GetCurrentExecutableName().string() + ".xml";
  const std::string& output_file =
      UnitTestOptions::GetAbsolutePathToOutputFile();

#ifdef KTEST_OS_WINDOWS
  EXPECT_STRCASEEQ(expected_output_file.c_str(), output_file.c_str());
#else
  EXPECT_EQ(expected_output_file, output_file.c_str());
#endif
}

#endif  // !KTEST_OS_FUCHSIA

}  // namespace
}  // namespace internal
}  // namespace testing
