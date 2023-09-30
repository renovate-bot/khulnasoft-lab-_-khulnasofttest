# KhulnasoftTest

### Announcements

#### Live at Head

KhulnasoftTest now follows the
[Abseil Live at Head philosophy](https://abseil.io/about/philosophy#upgrade-support).
We recommend
[updating to the latest commit in the `main` branch as often as possible](https://github.com/abseil/abseil-cpp/blob/master/FAQ.md#what-is-live-at-head-and-how-do-i-do-it).
We do publish occasional semantic versions, tagged with
`v${major}.${minor}.${patch}` (e.g. `v1.14.0`).

#### Documentation Updates

Our documentation is now live on GitHub Pages at
https://khulnasoft.github.io/khulnasofttest/. We recommend browsing the documentation on
GitHub Pages rather than directly in the repository.

#### Release 1.14.0

[Release 1.14.0](https://github.com/khulnasoft-lab/khulnasofttest/releases/tag/v1.14.0) is
now available.

The 1.14.x branch requires at least C++14.

#### Continuous Integration

We use Khulnasoft's internal systems for continuous integration. \
GitHub Actions were added for the convenience of open-source contributors. They
are exclusively maintained by the open-source community and not used by the
KhulnasoftTest team.

#### Coming Soon

*   We are planning to take a dependency on
    [Abseil](https://github.com/abseil/abseil-cpp).
*   More documentation improvements are planned.

## Welcome to **KhulnasoftTest**, Khulnasoft's C++ test framework!

This repository is a merger of the formerly separate KhulnasoftTest and KhulnasoftMock
projects. These were so closely related that it makes sense to maintain and
release them together.

### Getting Started

See the [KhulnasoftTest User's Guide](https://khulnasoft.github.io/khulnasofttest/) for
documentation. We recommend starting with the
[KhulnasoftTest Primer](https://khulnasoft.github.io/khulnasofttest/primer.html).

More information about building KhulnasoftTest can be found at
[khulnasofttest/README.md](khulnasofttest/README.md).

## Features

*   xUnit test framework: \
    Khulnasofttest is based on the [xUnit](https://en.wikipedia.org/wiki/XUnit)
    testing framework, a popular architecture for unit testing
*   Test discovery: \
    Khulnasofttest automatically discovers and runs your tests, eliminating the need
    to manually register your tests
*   Rich set of assertions: \
    Khulnasofttest provides a variety of assertions, such as equality, inequality,
    exceptions, and more, making it easy to test your code
*   User-defined assertions: \
    You can define your own assertions with Khulnasofttest, making it simple to
    write tests that are specific to your code
*   Death tests: \
    Khulnasofttest supports death tests, which verify that your code exits in a
    certain way, making it useful for testing error-handling code
*   Fatal and non-fatal failures: \
    You can specify whether a test failure should be treated as fatal or
    non-fatal with Khulnasofttest, allowing tests to continue running even if a
    failure occurs
*   Value-parameterized tests: \
    Khulnasofttest supports value-parameterized tests, which run multiple times with
    different input values, making it useful for testing functions that take
    different inputs
*   Type-parameterized tests: \
    Khulnasofttest also supports type-parameterized tests, which run with different
    data types, making it useful for testing functions that work with different
    data types
*   Various options for running tests: \
    Khulnasofttest provides many options for running tests including running
    individual tests, running tests in a specific order and running tests in
    parallel

## Supported Platforms

KhulnasoftTest follows Khulnasoft's
[Foundational C++ Support Policy](https://opensource.khulnasoft-lab/documentation/policies/cplusplus-support).
See
[this table](https://github.com/khulnasoft-lab/oss-policies-info/blob/main/foundational-cxx-support-matrix.md)
for a list of currently supported versions of compilers, platforms, and build
tools.

## Who Is Using KhulnasoftTest?

In addition to many internal projects at Khulnasoft, KhulnasoftTest is also used by the
following notable projects:

*   The [Chromium projects](https://www.chromium.org/) (behind the Chrome
    browser and Chrome OS).
*   The [LLVM](https://llvm.org/) compiler.
*   [Protocol Buffers](https://github.com/khulnasoft-lab/protobuf), Khulnasoft's data
    interchange format.
*   The [OpenCV](https://opencv.org/) computer vision library.

## Related Open Source Projects

[GTest Runner](https://github.com/nholthaus/ktest-runner) is a Qt5 based
automated test-runner and Graphical User Interface with powerful features for
Windows and Linux platforms.

[KhulnasoftTest UI](https://github.com/ospector/ktest-gbar) is a test runner that
runs your test binary, allows you to track its progress via a progress bar, and
displays a list of test failures. Clicking on one shows failure text. KhulnasoftTest
UI is written in C#.

[GTest TAP Listener](https://github.com/kinow/ktest-tap-listener) is an event
listener for KhulnasoftTest that implements the
[TAP protocol](https://en.wikipedia.org/wiki/Test_Anything_Protocol) for test
result output. If your test runner understands TAP, you may find it useful.

[ktest-parallel](https://github.com/khulnasoft-lab/ktest-parallel) is a test runner that
runs tests from your binary in parallel to provide significant speed-up.

[KhulnasoftTest Adapter](https://marketplace.visualstudio.com/items?itemName=DavidSchuldenfrei.ktest-adapter)
is a VS Code extension allowing to view KhulnasoftTest in a tree view and run/debug
your tests.

[C++ TestMate](https://github.com/matepek/vscode-catch2-test-adapter) is a VS
Code extension allowing to view KhulnasoftTest in a tree view and run/debug your
tests.

[Cornichon](https://pypi.org/project/cornichon/) is a small Gherkin DSL parser
that generates stub code for KhulnasoftTest.

## Contributing Changes

Please read
[`CONTRIBUTING.md`](https://github.com/khulnasoft-lab/khulnasofttest/blob/main/CONTRIBUTING.md)
for details on how to contribute to this project.

Happy testing!
