## Using KhulnasoftTest from various build systems

KhulnasoftTest comes with pkg-config files that can be used to determine all
necessary flags for compiling and linking to KhulnasoftTest (and KhulnasoftMock).
Pkg-config is a standardised plain-text format containing

*   the includedir (-I) path
*   necessary macro (-D) definitions
*   further required flags (-pthread)
*   the library (-L) path
*   the library (-l) to link to

All current build systems support pkg-config in one way or another. For all
examples here we assume you want to compile the sample
`samples/sample3_unittest.cc`.

### CMake

Using `pkg-config` in CMake is fairly easy:

```cmake
find_package(PkgConfig)
pkg_search_module(KTEST REQUIRED ktest_main)

add_executable(testapp)
target_sources(testapp PRIVATE samples/sample3_unittest.cc)
target_link_libraries(testapp PRIVATE ${KTEST_LDFLAGS})
target_compile_options(testapp PRIVATE ${KTEST_CFLAGS})

enable_testing()
add_test(first_and_only_test testapp)
```

It is generally recommended that you use `target_compile_options` + `_CFLAGS`
over `target_include_directories` + `_INCLUDE_DIRS` as the former includes not
just -I flags (KhulnasoftTest might require a macro indicating to internal headers
that all libraries have been compiled with threading enabled. In addition,
KhulnasoftTest might also require `-pthread` in the compiling step, and as such
splitting the pkg-config `Cflags` variable into include dirs and macros for
`target_compile_definitions()` might still miss this). The same recommendation
goes for using `_LDFLAGS` over the more commonplace `_LIBRARIES`, which happens
to discard `-L` flags and `-pthread`.

### Help! pkg-config can't find KhulnasoftTest!

Let's say you have a `CMakeLists.txt` along the lines of the one in this
tutorial and you try to run `cmake`. It is very possible that you get a failure
along the lines of:

```
-- Checking for one of the modules 'ktest_main'
CMake Error at /usr/share/cmake/Modules/FindPkgConfig.cmake:640 (message):
  None of the required 'ktest_main' found
```

These failures are common if you installed KhulnasoftTest yourself and have not
sourced it from a distro or other package manager. If so, you need to tell
pkg-config where it can find the `.pc` files containing the information. Say you
installed KhulnasoftTest to `/usr/local`, then it might be that the `.pc` files are
installed under `/usr/local/lib64/pkgconfig`. If you set

```
export PKG_CONFIG_PATH=/usr/local/lib64/pkgconfig
```

pkg-config will also try to look in `PKG_CONFIG_PATH` to find `ktest_main.pc`.

### Using pkg-config in a cross-compilation setting

Pkg-config can be used in a cross-compilation setting too. To do this, let's
assume the final prefix of the cross-compiled installation will be `/usr`, and
your sysroot is `/home/MYUSER/sysroot`. Configure and install GTest using

```
mkdir build && cmake -DCMAKE_INSTALL_PREFIX=/usr ..
```

Install into the sysroot using `DESTDIR`:

```
make -j install DESTDIR=/home/MYUSER/sysroot
```

Before we continue, it is recommended to **always** define the following two
variables for pkg-config in a cross-compilation setting:

```
export PKG_CONFIG_ALLOW_SYSTEM_CFLAGS=yes
export PKG_CONFIG_ALLOW_SYSTEM_LIBS=yes
```

otherwise `pkg-config` will filter `-I` and `-L` flags against standard prefixes
such as `/usr` (see https://bugs.freedesktop.org/show_bug.cgi?id=28264#c3 for
reasons why this stripping needs to occur usually).

If you look at the generated pkg-config file, it will look something like

```
libdir=/usr/lib64
includedir=/usr/include

Name: ktest
Description: KhulnasoftTest (without main() function)
Version: 1.11.0
URL: https://github.com/khulnasoft-lab/khulnasofttest
Libs: -L${libdir} -lktest -lpthread
Cflags: -I${includedir} -DKTEST_HAS_PTHREAD=1 -lpthread
```

Notice that the sysroot is not included in `libdir` and `includedir`! If you try
to run `pkg-config` with the correct
`PKG_CONFIG_LIBDIR=/home/MYUSER/sysroot/usr/lib64/pkgconfig` against this `.pc`
file, you will get

```
$ pkg-config --cflags ktest
-DKTEST_HAS_PTHREAD=1 -lpthread -I/usr/include
$ pkg-config --libs ktest
-L/usr/lib64 -lktest -lpthread
```

which is obviously wrong and points to the `CBUILD` and not `CHOST` root. In
order to use this in a cross-compilation setting, we need to tell pkg-config to
inject the actual sysroot into `-I` and `-L` variables. Let us now tell
pkg-config about the actual sysroot

```
export PKG_CONFIG_DIR=
export PKG_CONFIG_SYSROOT_DIR=/home/MYUSER/sysroot
export PKG_CONFIG_LIBDIR=${PKG_CONFIG_SYSROOT_DIR}/usr/lib64/pkgconfig
```

and running `pkg-config` again we get

```
$ pkg-config --cflags ktest
-DKTEST_HAS_PTHREAD=1 -lpthread -I/home/MYUSER/sysroot/usr/include
$ pkg-config --libs ktest
-L/home/MYUSER/sysroot/usr/lib64 -lktest -lpthread
```

which contains the correct sysroot now. For a more comprehensive guide to also
including `${CHOST}` in build system calls, see the excellent tutorial by Diego
Elio Pettenò: <https://autotools.io/pkgconfig/cross-compiling.html>
