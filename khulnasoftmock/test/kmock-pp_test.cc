#include "kmock/internal/kmock-pp.h"

// Used to test MSVC treating __VA_ARGS__ with a comma in it as one value
#define KMOCK_TEST_REPLACE_comma_WITH_COMMA_I_comma ,
#define KMOCK_TEST_REPLACE_comma_WITH_COMMA(x) \
  KMOCK_PP_CAT(KMOCK_TEST_REPLACE_comma_WITH_COMMA_I_, x)

// Static assertions.
namespace testing {
namespace internal {
namespace kmockpp {

static_assert(KMOCK_PP_CAT(1, 4) == 14, "");
static_assert(KMOCK_PP_INTERNAL_INTERNAL_16TH(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
                                              12, 13, 14, 15, 16, 17, 18) == 16,
              "");
static_assert(KMOCK_PP_NARG() == 1, "");
static_assert(KMOCK_PP_NARG(x) == 1, "");
static_assert(KMOCK_PP_NARG(x, y) == 2, "");
static_assert(KMOCK_PP_NARG(x, y, z) == 3, "");
static_assert(KMOCK_PP_NARG(x, y, z, w) == 4, "");
static_assert(!KMOCK_PP_HAS_COMMA(), "");
static_assert(KMOCK_PP_HAS_COMMA(b, ), "");
static_assert(!KMOCK_PP_HAS_COMMA((, )), "");
static_assert(KMOCK_PP_HAS_COMMA(KMOCK_TEST_REPLACE_comma_WITH_COMMA(comma)),
              "");
static_assert(
    KMOCK_PP_HAS_COMMA(KMOCK_TEST_REPLACE_comma_WITH_COMMA(comma(unrelated))),
    "");
static_assert(!KMOCK_PP_IS_EMPTY(, ), "");
static_assert(!KMOCK_PP_IS_EMPTY(a), "");
static_assert(!KMOCK_PP_IS_EMPTY(()), "");
static_assert(KMOCK_PP_IF(1, 1, 2) == 1, "");
static_assert(KMOCK_PP_IF(0, 1, 2) == 2, "");
static_assert(KMOCK_PP_NARG0(x) == 1, "");
static_assert(KMOCK_PP_NARG0(x, y) == 2, "");
static_assert(KMOCK_PP_HEAD(1) == 1, "");
static_assert(KMOCK_PP_HEAD(1, 2) == 1, "");
static_assert(KMOCK_PP_HEAD(1, 2, 3) == 1, "");
static_assert(KMOCK_PP_TAIL(1, 2) == 2, "");
static_assert(KMOCK_PP_HEAD(KMOCK_PP_TAIL(1, 2, 3)) == 2, "");
static_assert(!KMOCK_PP_IS_BEGIN_PARENS(sss), "");
static_assert(!KMOCK_PP_IS_BEGIN_PARENS(sss()), "");
static_assert(!KMOCK_PP_IS_BEGIN_PARENS(sss() sss), "");
static_assert(KMOCK_PP_IS_BEGIN_PARENS((sss)), "");
static_assert(KMOCK_PP_IS_BEGIN_PARENS((sss)ss), "");
static_assert(!KMOCK_PP_IS_ENCLOSED_PARENS(sss), "");
static_assert(!KMOCK_PP_IS_ENCLOSED_PARENS(sss()), "");
static_assert(!KMOCK_PP_IS_ENCLOSED_PARENS(sss() sss), "");
static_assert(!KMOCK_PP_IS_ENCLOSED_PARENS((sss)ss), "");
static_assert(KMOCK_PP_REMOVE_PARENS((1 + 1)) * 2 == 3, "");
static_assert(KMOCK_PP_INC(4) == 5, "");

template <class... Args>
struct Test {
  static constexpr int kArgs = sizeof...(Args);
};
#define KMOCK_PP_INTERNAL_TYPE_TEST(_i, _Data, _element) \
  KMOCK_PP_COMMA_IF(_i) _element
static_assert(Test<KMOCK_PP_FOR_EACH(KMOCK_PP_INTERNAL_TYPE_TEST, ~,
                                     (int, float, double, char))>::kArgs == 4,
              "");
#define KMOCK_PP_INTERNAL_VAR_TEST_1(_x) 1
#define KMOCK_PP_INTERNAL_VAR_TEST_2(_x, _y) 2
#define KMOCK_PP_INTERNAL_VAR_TEST_3(_x, _y, _z) 3

#define KMOCK_PP_INTERNAL_VAR_TEST(...) \
  KMOCK_PP_VARIADIC_CALL(KMOCK_PP_INTERNAL_VAR_TEST_, __VA_ARGS__)
static_assert(KMOCK_PP_INTERNAL_VAR_TEST(x, y) == 2, "");
static_assert(KMOCK_PP_INTERNAL_VAR_TEST(silly) == 1, "");
static_assert(KMOCK_PP_INTERNAL_VAR_TEST(x, y, z) == 3, "");

// TODO(iserna): The following asserts fail in --config=lexan.
#define KMOCK_PP_INTERNAL_IS_EMPTY_TEST_1
static_assert(KMOCK_PP_IS_EMPTY(KMOCK_PP_INTERNAL_IS_EMPTY_TEST_1), "");
static_assert(KMOCK_PP_IS_EMPTY(), "");
static_assert(KMOCK_PP_IS_ENCLOSED_PARENS((sss)), "");
static_assert(KMOCK_PP_IS_EMPTY(KMOCK_PP_TAIL(1)), "");
static_assert(KMOCK_PP_NARG0() == 0, "");

}  // namespace kmockpp
}  // namespace internal
}  // namespace testing
