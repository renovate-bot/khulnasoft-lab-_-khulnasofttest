# Customization Points

The custom directory is an injection point for custom user configurations.

## Header `kmock-port.h`

The following macros can be defined:

### Flag related macros:

*   `KMOCK_DECLARE_bool_(name)`
*   `KMOCK_DECLARE_int32_(name)`
*   `KMOCK_DECLARE_string_(name)`
*   `KMOCK_DEFINE_bool_(name, default_val, doc)`
*   `KMOCK_DEFINE_int32_(name, default_val, doc)`
*   `KMOCK_DEFINE_string_(name, default_val, doc)`
*   `KMOCK_FLAG_GET(flag_name)`
*   `KMOCK_FLAG_SET(flag_name, value)`
