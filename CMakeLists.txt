# Note: CMake support is community-based. The maintainers do not use CMake
# internally.

cmake_minimum_required(VERSION 3.13)

project(khulnasofttest-distribution)
set(KHULNASOFTTEST_VERSION 1.14.0)

if(NOT CYGWIN AND NOT MSYS AND NOT ${CMAKE_SYSTEM_NAME} STREQUAL QNX)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()

enable_testing()

include(CMakeDependentOption)
include(GNUInstallDirs)

#Note that khulnasoftmock target already builds khulnasofttest
option(BUILD_KMOCK "Builds the khulnasoftmock subproject" ON)
option(INSTALL_KTEST "Enable installation of khulnasofttest. (Projects embedding khulnasofttest may want to turn this OFF.)" ON)
option(KTEST_HAS_ABSL "Use Abseil and RE2. Requires Abseil and RE2 to be separately added to the build." OFF)

if(BUILD_KMOCK)
  add_subdirectory( khulnasoftmock )
else()
  add_subdirectory( khulnasofttest )
endif()
