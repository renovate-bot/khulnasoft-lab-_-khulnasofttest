### Generic Build Instructions

#### Setup

To build KhulnasoftTest and your tests that use it, you need to tell your build
system where to find its headers and source files. The exact way to do it
depends on which build system you use, and is usually straightforward.

### Build with CMake

KhulnasoftTest comes with a CMake build script
([CMakeLists.txt](https://github.com/khulnasoft-lab/khulnasofttest/blob/main/CMakeLists.txt))
that can be used on a wide range of platforms ("C" stands for cross-platform.).
If you don't have CMake installed already, you can download it for free from
<https://cmake.org/>.

CMake works by generating native makefiles or build projects that can be used in
the compiler environment of your choice. You can either build KhulnasoftTest as a
standalone project or it can be incorporated into an existing CMake build for
another project.

#### Standalone CMake Project

When building KhulnasoftTest as a standalone project, the typical workflow starts
with

```
git clone https://github.com/khulnasoft-lab/khulnasofttest.git -b v1.14.0
cd khulnasofttest        # Main directory of the cloned repository.
mkdir build          # Create a directory to hold the build output.
cd build
cmake ..             # Generate native build scripts for KhulnasoftTest.
```

The above command also includes KhulnasoftMock by default. And so, if you want to
build only KhulnasoftTest, you should replace the last command with

```
cmake .. -DBUILD_KMOCK=OFF
```

If you are on a \*nix system, you should now see a Makefile in the current
directory. Just type `make` to build KhulnasoftTest. And then you can simply install
KhulnasoftTest if you are a system administrator.

```
make
sudo make install    # Install in /usr/local/ by default
```

If you use Windows and have Visual Studio installed, a `ktest.sln` file and
several `.vcproj` files will be created. You can then build them using Visual
Studio.

On Mac OS X with Xcode installed, a `.xcodeproj` file will be generated.

#### Incorporating Into An Existing CMake Project

If you want to use KhulnasoftTest in a project which already uses CMake, the easiest
way is to get installed libraries and headers.

*   Import KhulnasoftTest by using `find_package` (or `pkg_check_modules`). For
    example, if `find_package(GTest CONFIG REQUIRED)` succeeds, you can use the
    libraries as `GTest::ktest`, `GTest::kmock`.

And a more robust and flexible approach is to build KhulnasoftTest as part of that
project directly. This is done by making the KhulnasoftTest source code available to
the main build and adding it using CMake's `add_subdirectory()` command. This
has the significant advantage that the same compiler and linker settings are
used between KhulnasoftTest and the rest of your project, so issues associated with
using incompatible libraries (eg debug/release), etc. are avoided. This is
particularly useful on Windows. Making KhulnasoftTest's source code available to the
main build can be done a few different ways:

*   Download the KhulnasoftTest source code manually and place it at a known
    location. This is the least flexible approach and can make it more difficult
    to use with continuous integration systems, etc.
*   Embed the KhulnasoftTest source code as a direct copy in the main project's
    source tree. This is often the simplest approach, but is also the hardest to
    keep up to date. Some organizations may not permit this method.
*   Add KhulnasoftTest as a git submodule or equivalent. This may not always be
    possible or appropriate. Git submodules, for example, have their own set of
    advantages and drawbacks.
*   Use CMake to download KhulnasoftTest as part of the build's configure step. This
    approach doesn't have the limitations of the other methods.

The last of the above methods is implemented with a small piece of CMake code
that downloads and pulls the KhulnasoftTest code into the main build.

Just add to your `CMakeLists.txt`:

```cmake
include(FetchContent)
FetchContent_Declare(
  khulnasofttest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/khulnasoft-lab/khulnasofttest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(ktest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(khulnasofttest)

# Now simply link against ktest or ktest_main as needed. Eg
add_executable(example example.cpp)
target_link_libraries(example ktest_main)
add_test(NAME example_test COMMAND example)
```

Note that this approach requires CMake 3.14 or later due to its use of the
`FetchContent_MakeAvailable()` command.

##### Visual Studio Dynamic vs Static Runtimes

By default, new Visual Studio projects link the C runtimes dynamically but
KhulnasoftTest links them statically. This will generate an error that looks
something like the following: ktest.lib(ktest-all.obj) : error LNK2038: mismatch
detected for 'RuntimeLibrary': value 'MTd_StaticDebug' doesn't match value
'MDd_DynamicDebug' in main.obj

KhulnasoftTest already has a CMake option for this: `ktest_force_shared_crt`

Enabling this option will make ktest link the runtimes dynamically too, and
match the project in which it is included.

#### C++ Standard Version

An environment that supports C++14 is required in order to successfully build
KhulnasoftTest. One way to ensure this is to specify the standard in the top-level
project, for example by using the `set(CMAKE_CXX_STANDARD 14)` command along
with `set(CMAKE_CXX_STANDARD_REQUIRED ON)`. If this is not feasible, for example
in a C project using KhulnasoftTest for validation, then it can be specified by
adding it to the options for cmake via the`-DCMAKE_CXX_FLAGS` option.

### Tweaking KhulnasoftTest

KhulnasoftTest can be used in diverse environments. The default configuration may
not work (or may not work well) out of the box in some environments. However,
you can easily tweak KhulnasoftTest by defining control macros on the compiler
command line. Generally, these macros are named like `KTEST_XYZ` and you define
them to either 1 or 0 to enable or disable a certain feature.

We list the most frequently used macros below. For a complete list, see file
[include/ktest/internal/ktest-port.h](https://github.com/khulnasoft-lab/khulnasofttest/blob/main/khulnasofttest/include/ktest/internal/ktest-port.h).

### Multi-threaded Tests

KhulnasoftTest is thread-safe where the pthread library is available. After
`#include <ktest/ktest.h>`, you can check the
`KTEST_IS_THREADSAFE` macro to see whether this is the case (yes if the macro is
`#defined` to 1, no if it's undefined.).

If KhulnasoftTest doesn't correctly detect whether pthread is available in your
environment, you can force it with

```
-DKTEST_HAS_PTHREAD=1
```

or

```
-DKTEST_HAS_PTHREAD=0
```

When KhulnasoftTest uses pthread, you may need to add flags to your compiler and/or
linker to select the pthread library, or you'll get link errors. If you use the
CMake script, this is taken care of for you. If you use your own build script,
you'll need to read your compiler and linker's manual to figure out what flags
to add.

### As a Shared Library (DLL)

KhulnasoftTest is compact, so most users can build and link it as a static library
for the simplicity. You can choose to use KhulnasoftTest as a shared library (known
as a DLL on Windows) if you prefer.

To compile *ktest* as a shared library, add

```
-DKTEST_CREATE_SHARED_LIBRARY=1
```

to the compiler flags. You'll also need to tell the linker to produce a shared
library instead - consult your linker's manual for how to do it.

To compile your *tests* that use the ktest shared library, add

```
-DKTEST_LINKED_AS_SHARED_LIBRARY=1
```

to the compiler flags.

Note: while the above steps aren't technically necessary today when using some
compilers (e.g. GCC), they may become necessary in the future, if we decide to
improve the speed of loading the library (see
<https://gcc.gnu.org/wiki/Visibility> for details). Therefore you are
recommended to always add the above flags when using KhulnasoftTest as a shared
library. Otherwise a future release of KhulnasoftTest may break your build script.

### Avoiding Macro Name Clashes

In C++, macros don't obey namespaces. Therefore two libraries that both define a
macro of the same name will clash if you `#include` both definitions. In case a
KhulnasoftTest macro clashes with another library, you can force KhulnasoftTest to
rename its macro to avoid the conflict.

Specifically, if both KhulnasoftTest and some other code define macro FOO, you can
add

```
-DKTEST_DONT_DEFINE_FOO=1
```

to the compiler flags to tell KhulnasoftTest to change the macro's name from `FOO`
to `KTEST_FOO`. Currently `FOO` can be `ASSERT_EQ`, `ASSERT_FALSE`, `ASSERT_GE`,
`ASSERT_GT`, `ASSERT_LE`, `ASSERT_LT`, `ASSERT_NE`, `ASSERT_TRUE`,
`EXPECT_FALSE`, `EXPECT_TRUE`, `FAIL`, `SUCCEED`, `TEST`, or `TEST_F`. For
example, with `-DKTEST_DONT_DEFINE_TEST=1`, you'll need to write

```
KTEST_TEST(SomeTest, DoesThis) { ... }
```

instead of

```
TEST(SomeTest, DoesThis) { ... }
```

in order to define a test.
