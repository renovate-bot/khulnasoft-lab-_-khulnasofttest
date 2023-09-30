########################################################################
# Note: CMake support is community-based. The maintainers do not use CMake
# internally.
#
# CMake build script for Khulnasoft Test.
#
# To run the tests for Khulnasoft Test itself on Linux, use 'make test' or
# ctest.  You can select which tests to run using 'ctest -R regex'.
# For more options, run 'ctest --help'.

# When other libraries are using a shared version of runtime libraries,
# Khulnasoft Test also has to use one.
option(
  ktest_force_shared_crt
  "Use shared (DLL) run-time lib even when Khulnasoft Test is built as static lib."
  OFF)

option(ktest_build_tests "Build all of ktest's own tests." OFF)

option(ktest_build_samples "Build ktest's sample programs." OFF)

option(ktest_disable_pthreads "Disable uses of pthreads in ktest." OFF)

option(
  ktest_hide_internal_symbols
  "Build ktest with internal symbols hidden in shared libraries."
  OFF)

# Defines pre_project_set_up_hermetic_build() and set_up_hermetic_build().
include(cmake/hermetic_build.cmake OPTIONAL)

if (COMMAND pre_project_set_up_hermetic_build)
  pre_project_set_up_hermetic_build()
endif()

########################################################################
#
# Project-wide settings

# Name of the project.
#
# CMake files in this project can refer to the root source directory
# as ${ktest_SOURCE_DIR} and to the root binary directory as
# ${ktest_BINARY_DIR}.
# Language "C" is required for find_package(Threads).

# Project version:

cmake_minimum_required(VERSION 3.13)
project(ktest VERSION ${KHULNASOFTTEST_VERSION} LANGUAGES CXX C)

if (COMMAND set_up_hermetic_build)
  set_up_hermetic_build()
endif()

# These commands only run if this is the main project
if(CMAKE_PROJECT_NAME STREQUAL "ktest" OR CMAKE_PROJECT_NAME STREQUAL "khulnasofttest-distribution")

  # BUILD_SHARED_LIBS is a standard CMake variable, but we declare it here to
  # make it prominent in the GUI.
  option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." OFF)

else()

  mark_as_advanced(
    ktest_force_shared_crt
    ktest_build_tests
    ktest_build_samples
    ktest_disable_pthreads
    ktest_hide_internal_symbols)

endif()


if (ktest_hide_internal_symbols)
  set(CMAKE_CXX_VISIBILITY_PRESET hidden)
  set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
endif()

# Define helper functions and macros used by Khulnasoft Test.
include(cmake/internal_utils.cmake)

config_compiler_and_linker()  # Defined in internal_utils.cmake.

# Needed to set the namespace for both the export targets and the
# alias libraries
set(cmake_package_name GTest CACHE INTERNAL "")

# Create the CMake package file descriptors.
if (INSTALL_KTEST)
  include(CMakePackageConfigHelpers)
  set(targets_export_name ${cmake_package_name}Targets CACHE INTERNAL "")
  set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated" CACHE INTERNAL "")
  set(cmake_files_install_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${cmake_package_name}")
  set(version_file "${generated_dir}/${cmake_package_name}ConfigVersion.cmake")
  write_basic_package_version_file(${version_file} VERSION ${KHULNASOFTTEST_VERSION} COMPATIBILITY AnyNewerVersion)
  install(EXPORT ${targets_export_name}
    COMPONENT "${PROJECT_NAME}"
    NAMESPACE ${cmake_package_name}::
    DESTINATION ${cmake_files_install_dir})
  set(config_file "${generated_dir}/${cmake_package_name}Config.cmake")
  configure_package_config_file("${ktest_SOURCE_DIR}/cmake/Config.cmake.in"
    "${config_file}" INSTALL_DESTINATION ${cmake_files_install_dir})
  install(FILES ${version_file} ${config_file}
    COMPONENT "${PROJECT_NAME}"
    DESTINATION ${cmake_files_install_dir})
endif()

# Where Khulnasoft Test's .h files can be found.
set(ktest_build_include_dirs
  "${ktest_SOURCE_DIR}/include"
  "${ktest_SOURCE_DIR}")
include_directories(${ktest_build_include_dirs})

########################################################################
#
# Defines the ktest & ktest_main libraries.  User tests should link
# with one of them.

# Khulnasoft Test libraries.  We build them using more strict warnings than what
# are used for other targets, to ensure that ktest can be compiled by a user
# aggressive about warnings.
cxx_library(ktest "${cxx_strict}" src/ktest-all.cc)
set_target_properties(ktest PROPERTIES VERSION ${KHULNASOFTTEST_VERSION})
if(KTEST_HAS_ABSL)
  target_compile_definitions(ktest PUBLIC KTEST_HAS_ABSL=1)
  target_link_libraries(ktest PUBLIC
    absl::failure_signal_handler
    absl::stacktrace
    absl::symbolize
    absl::flags_parse
    absl::flags_reflection
    absl::flags_usage
    absl::strings
    absl::any
    absl::optional
    absl::variant
    re2::re2
  )
endif()
cxx_library(ktest_main "${cxx_strict}" src/ktest_main.cc)
set_target_properties(ktest_main PROPERTIES VERSION ${KHULNASOFTTEST_VERSION})
string(REPLACE ";" "$<SEMICOLON>" dirs "${ktest_build_include_dirs}")
target_include_directories(ktest SYSTEM INTERFACE
  "$<BUILD_INTERFACE:${dirs}>"
  "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")
target_include_directories(ktest_main SYSTEM INTERFACE
  "$<BUILD_INTERFACE:${dirs}>"
  "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")
if(CMAKE_SYSTEM_NAME MATCHES "QNX")
  target_link_libraries(ktest PUBLIC regex)
endif()
target_link_libraries(ktest_main PUBLIC ktest)

########################################################################
#
# Install rules
install_project(ktest ktest_main)

########################################################################
#
# Samples on how to link user tests with ktest or ktest_main.
#
# They are not built by default.  To build them, set the
# ktest_build_samples option to ON.  You can do it by running ccmake
# or specifying the -Dktest_build_samples=ON flag when running cmake.

if (ktest_build_samples)
  cxx_executable(sample1_unittest samples ktest_main samples/sample1.cc)
  cxx_executable(sample2_unittest samples ktest_main samples/sample2.cc)
  cxx_executable(sample3_unittest samples ktest_main)
  cxx_executable(sample4_unittest samples ktest_main samples/sample4.cc)
  cxx_executable(sample5_unittest samples ktest_main samples/sample1.cc)
  cxx_executable(sample6_unittest samples ktest_main)
  cxx_executable(sample7_unittest samples ktest_main)
  cxx_executable(sample8_unittest samples ktest_main)
  cxx_executable(sample9_unittest samples ktest)
  cxx_executable(sample10_unittest samples ktest)
endif()

########################################################################
#
# Khulnasoft Test's own tests.
#
# You can skip this section if you aren't interested in testing
# Khulnasoft Test itself.
#
# The tests are not built by default.  To build them, set the
# ktest_build_tests option to ON.  You can do it by running ccmake
# or specifying the -Dktest_build_tests=ON flag when running cmake.

if (ktest_build_tests)
  # This must be set in the root directory for the tests to be run by
  # 'make test' or ctest.
  enable_testing()

  ############################################################
  # C++ tests built with standard compiler flags.

  cxx_test(khulnasofttest-death-test-test ktest_main)
  cxx_test(ktest_environment_test ktest)
  cxx_test(khulnasofttest-filepath-test ktest_main)
  cxx_test(khulnasofttest-listener-test ktest_main)
  cxx_test(ktest_main_unittest ktest_main)
  cxx_test(khulnasofttest-message-test ktest_main)
  cxx_test(ktest_no_test_unittest ktest)
  cxx_test(khulnasofttest-options-test ktest_main)
  cxx_test(khulnasofttest-param-test-test ktest
    test/khulnasofttest-param-test2-test.cc)
  cxx_test(khulnasofttest-port-test ktest_main)
  cxx_test(ktest_pred_impl_unittest ktest_main)
  cxx_test(ktest_premature_exit_test ktest
    test/ktest_premature_exit_test.cc)
  cxx_test(khulnasofttest-printers-test ktest_main)
  cxx_test(ktest_prod_test ktest_main
    test/production.cc)
  cxx_test(ktest_repeat_test ktest)
  cxx_test(ktest_sole_header_test ktest_main)
  cxx_test(ktest_stress_test ktest)
  cxx_test(khulnasofttest-test-part-test ktest_main)
  cxx_test(ktest_throw_on_failure_ex_test ktest)
  cxx_test(ktest-typed-test_test ktest_main
    test/ktest-typed-test2_test.cc)
  cxx_test(ktest_unittest ktest_main)
  cxx_test(ktest-unittest-api_test ktest)
  cxx_test(ktest_skip_in_environment_setup_test ktest_main)
  cxx_test(ktest_skip_test ktest_main)

  ############################################################
  # C++ tests built with non-standard compiler flags.

  # MSVC 7.1 does not support STL with exceptions disabled.
  if (NOT MSVC OR MSVC_VERSION GREATER 1310)
    cxx_library(ktest_no_exception "${cxx_no_exception}"
      src/ktest-all.cc)
    cxx_library(ktest_main_no_exception "${cxx_no_exception}"
      src/ktest-all.cc src/ktest_main.cc)
  endif()
  cxx_library(ktest_main_no_rtti "${cxx_no_rtti}"
    src/ktest-all.cc src/ktest_main.cc)

  cxx_test_with_flags(ktest-death-test_ex_nocatch_test
    "${cxx_exception} -DKTEST_ENABLE_CATCH_EXCEPTIONS_=0"
    ktest test/khulnasofttest-death-test_ex_test.cc)
  cxx_test_with_flags(ktest-death-test_ex_catch_test
    "${cxx_exception} -DKTEST_ENABLE_CATCH_EXCEPTIONS_=1"
    ktest test/khulnasofttest-death-test_ex_test.cc)

  cxx_test_with_flags(ktest_no_rtti_unittest "${cxx_no_rtti}"
    ktest_main_no_rtti test/ktest_unittest.cc)

  cxx_shared_library(ktest_dll "${cxx_default}"
    src/ktest-all.cc src/ktest_main.cc)

  cxx_executable_with_flags(ktest_dll_test_ "${cxx_default}"
    ktest_dll test/ktest_all_test.cc)
  set_target_properties(ktest_dll_test_
                        PROPERTIES
                        COMPILE_DEFINITIONS "KTEST_LINKED_AS_SHARED_LIBRARY=1")

  ############################################################
  # Python tests.

  cxx_executable(khulnasofttest-break-on-failure-unittest_ test ktest)
  py_test(khulnasofttest-break-on-failure-unittest)

  py_test(ktest_skip_check_output_test)
  py_test(ktest_skip_environment_check_output_test)

  # Visual Studio .NET 2003 does not support STL with exceptions disabled.
  if (NOT MSVC OR MSVC_VERSION GREATER 1310)  # 1310 is Visual Studio .NET 2003
    cxx_executable_with_flags(
      khulnasofttest-catch-exceptions-no-ex-test_
      "${cxx_no_exception}"
      ktest_main_no_exception
      test/khulnasofttest-catch-exceptions-test_.cc)
  endif()

  cxx_executable_with_flags(
    khulnasofttest-catch-exceptions-ex-test_
    "${cxx_exception}"
    ktest_main
    test/khulnasofttest-catch-exceptions-test_.cc)
  py_test(khulnasofttest-catch-exceptions-test)

  cxx_executable(khulnasofttest-color-test_ test ktest)
  py_test(khulnasofttest-color-test)

  cxx_executable(khulnasofttest-env-var-test_ test ktest)
  py_test(khulnasofttest-env-var-test)

  cxx_executable(khulnasofttest-filter-unittest_ test ktest)
  py_test(khulnasofttest-filter-unittest)

  cxx_executable(ktest_help_test_ test ktest_main)
  py_test(ktest_help_test)

  cxx_executable(khulnasofttest-list-tests-unittest_ test ktest)
  py_test(khulnasofttest-list-tests-unittest)

  cxx_executable(khulnasofttest-output-test_ test ktest)
  py_test(khulnasofttest-output-test --no_stacktrace_support)

  cxx_executable(khulnasofttest-shuffle-test_ test ktest)
  py_test(khulnasofttest-shuffle-test)

  # MSVC 7.1 does not support STL with exceptions disabled.
  if (NOT MSVC OR MSVC_VERSION GREATER 1310)
    cxx_executable(khulnasofttest-throw-on-failure-test_ test ktest_no_exception)
    set_target_properties(khulnasofttest-throw-on-failure-test_
      PROPERTIES
      COMPILE_FLAGS "${cxx_no_exception}")
    py_test(khulnasofttest-throw-on-failure-test)
  endif()

  cxx_executable(khulnasofttest-uninitialized-test_ test ktest)
  py_test(khulnasofttest-uninitialized-test)

  cxx_executable(ktest_list_output_unittest_ test ktest)
  py_test(ktest_list_output_unittest)

  cxx_executable(ktest_xml_outfile1_test_ test ktest_main)
  cxx_executable(ktest_xml_outfile2_test_ test ktest_main)
  py_test(ktest_xml_outfiles_test)
  py_test(khulnasofttest-json-outfiles-test)

  cxx_executable(ktest_xml_output_unittest_ test ktest)
  py_test(ktest_xml_output_unittest --no_stacktrace_support)
  py_test(khulnasofttest-json-output-unittest --no_stacktrace_support)
endif()
