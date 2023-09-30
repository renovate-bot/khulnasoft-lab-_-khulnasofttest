// Copyright 2015, Khulnasoft Inc.
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
// This header file defines the KTEST_OS_* macro.
// It is separate from ktest-port.h so that custom/ktest-port.h can include it.

#ifndef KHULNASOFTTEST_INCLUDE_KTEST_INTERNAL_KTEST_PORT_ARCH_H_
#define KHULNASOFTTEST_INCLUDE_KTEST_INTERNAL_KTEST_PORT_ARCH_H_

// Determines the platform on which Khulnasoft Test is compiled.
#ifdef __CYGWIN__
#define KTEST_OS_CYGWIN 1
#elif defined(__MINGW__) || defined(__MINGW32__) || defined(__MINGW64__)
#define KTEST_OS_WINDOWS_MINGW 1
#define KTEST_OS_WINDOWS 1
#elif defined _WIN32
#define KTEST_OS_WINDOWS 1
#ifdef _WIN32_WCE
#define KTEST_OS_WINDOWS_MOBILE 1
#elif defined(WINAPI_FAMILY)
#include <winapifamily.h>
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
#define KTEST_OS_WINDOWS_DESKTOP 1
#elif WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_PHONE_APP)
#define KTEST_OS_WINDOWS_PHONE 1
#elif WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
#define KTEST_OS_WINDOWS_RT 1
#elif WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_TV_TITLE)
#define KTEST_OS_WINDOWS_PHONE 1
#define KTEST_OS_WINDOWS_TV_TITLE 1
#else
// WINAPI_FAMILY defined but no known partition matched.
// Default to desktop.
#define KTEST_OS_WINDOWS_DESKTOP 1
#endif
#else
#define KTEST_OS_WINDOWS_DESKTOP 1
#endif  // _WIN32_WCE
#elif defined __OS2__
#define KTEST_OS_OS2 1
#elif defined __APPLE__
#define KTEST_OS_MAC 1
#include <TargetConditionals.h>
#if TARGET_OS_IPHONE
#define KTEST_OS_IOS 1
#endif
#elif defined __DragonFly__
#define KTEST_OS_DRAGONFLY 1
#elif defined __FreeBSD__
#define KTEST_OS_FREEBSD 1
#elif defined __Fuchsia__
#define KTEST_OS_FUCHSIA 1
#elif defined(__GNU__)
#define KTEST_OS_GNU_HURD 1
#elif defined(__GLIBC__) && defined(__FreeBSD_kernel__)
#define KTEST_OS_GNU_KFREEBSD 1
#elif defined __linux__
#define KTEST_OS_LINUX 1
#if defined __ANDROID__
#define KTEST_OS_LINUX_ANDROID 1
#endif
#elif defined __MVS__
#define KTEST_OS_ZOS 1
#elif defined(__sun) && defined(__SVR4)
#define KTEST_OS_SOLARIS 1
#elif defined(_AIX)
#define KTEST_OS_AIX 1
#elif defined(__hpux)
#define KTEST_OS_HPUX 1
#elif defined __native_client__
#define KTEST_OS_NACL 1
#elif defined __NetBSD__
#define KTEST_OS_NETBSD 1
#elif defined __OpenBSD__
#define KTEST_OS_OPENBSD 1
#elif defined __QNX__
#define KTEST_OS_QNX 1
#elif defined(__HAIKU__)
#define KTEST_OS_HAIKU 1
#elif defined ESP8266
#define KTEST_OS_ESP8266 1
#elif defined ESP32
#define KTEST_OS_ESP32 1
#elif defined(__XTENSA__)
#define KTEST_OS_XTENSA 1
#elif defined(__hexagon__)
#define KTEST_OS_QURT 1
#elif defined(CPU_QN9090) || defined(CPU_QN9090HN)
#define KTEST_OS_NXP_QN9090 1
#elif defined(NRF52)
#define KTEST_OS_NRF52 1
#endif  // __CYGWIN__

#endif  // KHULNASOFTTEST_INCLUDE_KTEST_INTERNAL_KTEST_PORT_ARCH_H_
