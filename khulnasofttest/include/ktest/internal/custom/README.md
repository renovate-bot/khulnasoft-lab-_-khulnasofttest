# Customization Points

The custom directory is an injection point for custom user configurations.

## Header `ktest.h`

### The following macros can be defined:

*   `KTEST_OS_STACK_TRACE_GETTER_` - The name of an implementation of
    `OsStackTraceGetterInterface`.
*   `KTEST_CUSTOM_TEMPDIR_FUNCTION_` - An override for `testing::TempDir()`. See
    `testing::TempDir` for semantics and signature.

## Header `ktest-port.h`

The following macros can be defined:

### Logging:

*   `KTEST_LOG_(severity)`
*   `KTEST_CHECK_(condition)`
*   Functions `LogToStderr()` and `FlushInfoLog()` have to be provided too.

### Threading:

*   `KTEST_HAS_NOTIFICATION_` - Enabled if Notification is already provided.
*   `KTEST_HAS_MUTEX_AND_THREAD_LOCAL_` - Enabled if `Mutex` and `ThreadLocal`
    are already provided. Must also provide `KTEST_DECLARE_STATIC_MUTEX_(mutex)`
    and `KTEST_DEFINE_STATIC_MUTEX_(mutex)`
*   `KTEST_EXCLUSIVE_LOCK_REQUIRED_(locks)`
*   `KTEST_LOCK_EXCLUDED_(locks)`

### Underlying library support features

*   `KTEST_HAS_CXXABI_H_`

### Exporting API symbols:

*   `KTEST_API_` - Specifier for exported symbols.

## Header `ktest-printers.h`

*   See documentation at `ktest/ktest-printers.h` for details on how to define a
    custom printer.
