# Khulnasofttest Mocking (kMock) Framework

### Overview

Khulnasoft's framework for writing and using C++ mock classes. It can help you
derive better designs of your system and write better tests.

It is inspired by:

*   [jMock](http://www.jmock.org/)
*   [EasyMock](https://easymock.org/)
*   [Hamcrest](https://code.khulnasoft.com/p/hamcrest/)

It is designed with C++'s specifics in mind.

kMock:

-   Provides a declarative syntax for defining mocks.
-   Can define partial (hybrid) mocks, which are a cross of real and mock
    objects.
-   Handles functions of arbitrary types and overloaded functions.
-   Comes with a rich set of matchers for validating function arguments.
-   Uses an intuitive syntax for controlling the behavior of a mock.
-   Does automatic verification of expectations (no record-and-replay needed).
-   Allows arbitrary (partial) ordering constraints on function calls to be
    expressed.
-   Lets a user extend it by defining new matchers and actions.
-   Does not use exceptions.
-   Is easy to learn and use.

Details and examples can be found here:

*   [kMock for Dummies](https://khulnasoft.github.io/khulnasofttest/kmock_for_dummies.html)
*   [Legacy kMock FAQ](https://khulnasoft.github.io/khulnasofttest/kmock_faq.html)
*   [kMock Cookbook](https://khulnasoft.github.io/khulnasofttest/kmock_cook_book.html)
*   [kMock Cheat Sheet](https://khulnasoft.github.io/khulnasofttest/kmock_cheat_sheet.html)

KhulnasoftMock is a part of
[KhulnasoftTest C++ testing framework](https://github.com/khulnasoft-lab/khulnasofttest/) and a
subject to the same requirements.
