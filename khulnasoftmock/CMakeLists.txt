########################################################################
# Note: CMake support is community-based. The maintainers do not use CMake
# internally.
#
# CMake build script for Khulnasoft Mock.
#
# To run the tests for Khulnasoft Mock itself on Linux, use 'make test' or
# ctest.  You can select which tests to run using 'ctest -R regex'.
# For more options, run 'ctest --help'.

option(kmock_build_tests "Build all of Khulnasoft Mock's own tests." OFF)

# A directory to find Khulnasoft Test sources.
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/ktest/CMakeLists.txt")
  set(ktest_dir ktest)
else()
  set(ktest_dir ../khulnasofttest)
endif()

# Defines pre_project_set_up_hermetic_build() and set_up_hermetic_build().
include("${ktest_dir}/cmake/hermetic_build.cmake" OPTIONAL)

if (COMMAND pre_project_set_up_hermetic_build)
  # Khulnasoft Test also calls hermetic setup functions from add_subdirectory,
  # although its changes will not affect things at the current scope.
  pre_project_set_up_hermetic_build()
endif()

########################################################################
#
# Project-wide settings

# Name of the project.
#
# CMake files in this project can refer to the root source directory
# as ${kmock_SOURCE_DIR} and to the root binary directory as
# ${kmock_BINARY_DIR}.
# Language "C" is required for find_package(Threads).
cmake_minimum_required(VERSION 3.13)
project(kmock VERSION ${KHULNASOFTTEST_VERSION} LANGUAGES CXX C)

if (COMMAND set_up_hermetic_build)
  set_up_hermetic_build()
endif()

# Instructs CMake to process Khulnasoft Test's CMakeLists.txt and add its
# targets to the current scope.  We are placing Khulnasoft Test's binary
# directory in a subdirectory of our own as VC compilation may break
# if they are the same (the default).
add_subdirectory("${ktest_dir}" "${kmock_BINARY_DIR}/${ktest_dir}")


# These commands only run if this is the main project
if(CMAKE_PROJECT_NAME STREQUAL "kmock" OR CMAKE_PROJECT_NAME STREQUAL "khulnasofttest-distribution")
  # BUILD_SHARED_LIBS is a standard CMake variable, but we declare it here to
  # make it prominent in the GUI.
  option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." OFF)
else()
  mark_as_advanced(kmock_build_tests)
endif()

# Although Khulnasoft Test's CMakeLists.txt calls this function, the
# changes there don't affect the current scope.  Therefore we have to
# call it again here.
config_compiler_and_linker()  # from ${ktest_dir}/cmake/internal_utils.cmake

# Adds Khulnasoft Mock's and Khulnasoft Test's header directories to the search path.
set(kmock_build_include_dirs
  "${kmock_SOURCE_DIR}/include"
  "${kmock_SOURCE_DIR}"
  "${ktest_SOURCE_DIR}/include"
  # This directory is needed to build directly from Khulnasoft Test sources.
  "${ktest_SOURCE_DIR}")
include_directories(${kmock_build_include_dirs})

########################################################################
#
# Defines the kmock & kmock_main libraries.  User tests should link
# with one of them.

# Khulnasoft Mock libraries.  We build them using more strict warnings than what
# are used for other targets, to ensure that Khulnasoft Mock can be compiled by
# a user aggressive about warnings.
if (MSVC)
  cxx_library(kmock
              "${cxx_strict}"
              "${ktest_dir}/src/ktest-all.cc"
              src/kmock-all.cc)

  cxx_library(kmock_main
              "${cxx_strict}"
              "${ktest_dir}/src/ktest-all.cc"
              src/kmock-all.cc
              src/kmock_main.cc)
else()
  cxx_library(kmock "${cxx_strict}" src/kmock-all.cc)
  target_link_libraries(kmock PUBLIC ktest)
  set_target_properties(kmock PROPERTIES VERSION ${KHULNASOFTTEST_VERSION})
  cxx_library(kmock_main "${cxx_strict}" src/kmock_main.cc)
  target_link_libraries(kmock_main PUBLIC kmock)
  set_target_properties(kmock_main PROPERTIES VERSION ${KHULNASOFTTEST_VERSION})
endif()

string(REPLACE ";" "$<SEMICOLON>" dirs "${kmock_build_include_dirs}")
target_include_directories(kmock SYSTEM INTERFACE
  "$<BUILD_INTERFACE:${dirs}>"
  "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")
target_include_directories(kmock_main SYSTEM INTERFACE
  "$<BUILD_INTERFACE:${dirs}>"
  "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

########################################################################
#
# Install rules
install_project(kmock kmock_main)

########################################################################
#
# Khulnasoft Mock's own tests.
#
# You can skip this section if you aren't interested in testing
# Khulnasoft Mock itself.
#
# The tests are not built by default.  To build them, set the
# kmock_build_tests option to ON.  You can do it by running ccmake
# or specifying the -Dkmock_build_tests=ON flag when running cmake.

if (kmock_build_tests)
  # This must be set in the root directory for the tests to be run by
  # 'make test' or ctest.
  enable_testing()

  if (MINGW OR CYGWIN)
    add_compile_options("-Wa,-mbig-obj")
  endif()

  ############################################################
  # C++ tests built with standard compiler flags.

  cxx_test(kmock-actions_test kmock_main)
  cxx_test(kmock-cardinalities_test kmock_main)
  cxx_test(kmock_ex_test kmock_main)
  cxx_test(kmock-function-mocker_test kmock_main)
  cxx_test(kmock-internal-utils_test kmock_main)
  cxx_test(kmock-matchers-arithmetic_test kmock_main)
  cxx_test(kmock-matchers-comparisons_test kmock_main)
  cxx_test(kmock-matchers-containers_test kmock_main)
  cxx_test(kmock-matchers-misc_test kmock_main)
  cxx_test(kmock-more-actions_test kmock_main)
  cxx_test(kmock-nice-strict_test kmock_main)
  cxx_test(kmock-port_test kmock_main)
  cxx_test(kmock-spec-builders_test kmock_main)
  cxx_test(kmock_link_test kmock_main test/kmock_link2_test.cc)
  cxx_test(kmock_test kmock_main)

  if (DEFINED KTEST_HAS_PTHREAD)
    cxx_test(kmock_stress_test kmock)
  endif()

  # kmock_all_test is commented to save time building and running tests.
  # Uncomment if necessary.
  # cxx_test(kmock_all_test kmock_main)

  ############################################################
  # C++ tests built with non-standard compiler flags.

  if (MSVC)
    cxx_library(kmock_main_no_exception "${cxx_no_exception}"
      "${ktest_dir}/src/ktest-all.cc" src/kmock-all.cc src/kmock_main.cc)

    cxx_library(kmock_main_no_rtti "${cxx_no_rtti}"
      "${ktest_dir}/src/ktest-all.cc" src/kmock-all.cc src/kmock_main.cc)

  else()
    cxx_library(kmock_main_no_exception "${cxx_no_exception}" src/kmock_main.cc)
    target_link_libraries(kmock_main_no_exception PUBLIC kmock)

    cxx_library(kmock_main_no_rtti "${cxx_no_rtti}" src/kmock_main.cc)
    target_link_libraries(kmock_main_no_rtti PUBLIC kmock)
  endif()
  cxx_test_with_flags(kmock-more-actions_no_exception_test "${cxx_no_exception}"
    kmock_main_no_exception test/kmock-more-actions_test.cc)

  cxx_test_with_flags(kmock_no_rtti_test "${cxx_no_rtti}"
    kmock_main_no_rtti test/kmock-spec-builders_test.cc)

  cxx_shared_library(shared_kmock_main "${cxx_default}"
    "${ktest_dir}/src/ktest-all.cc" src/kmock-all.cc src/kmock_main.cc)

  # Tests that a binary can be built with Khulnasoft Mock as a shared library.  On
  # some system configurations, it may not possible to run the binary without
  # knowing more details about the system configurations. We do not try to run
  # this binary. To get a more robust shared library coverage, configure with
  # -DBUILD_SHARED_LIBS=ON.
  cxx_executable_with_flags(shared_kmock_test_ "${cxx_default}"
    shared_kmock_main test/kmock-spec-builders_test.cc)
  set_target_properties(shared_kmock_test_
    PROPERTIES
    COMPILE_DEFINITIONS "KTEST_LINKED_AS_SHARED_LIBRARY=1")

  ############################################################
  # Python tests.

  cxx_executable(kmock_leak_test_ test kmock_main)
  py_test(kmock_leak_test)

  cxx_executable(kmock_output_test_ test kmock)
  py_test(kmock_output_test)
endif()
