#ifndef KHULNASOFTMOCK_INCLUDE_KMOCK_INTERNAL_KMOCK_PP_H_
#define KHULNASOFTMOCK_INCLUDE_KMOCK_INTERNAL_KMOCK_PP_H_

// Expands and concatenates the arguments. Constructed macros reevaluate.
#define KMOCK_PP_CAT(_1, _2) KMOCK_PP_INTERNAL_CAT(_1, _2)

// Expands and stringifies the only argument.
#define KMOCK_PP_STRINGIZE(...) KMOCK_PP_INTERNAL_STRINGIZE(__VA_ARGS__)

// Returns empty. Given a variadic number of arguments.
#define KMOCK_PP_EMPTY(...)

// Returns a comma. Given a variadic number of arguments.
#define KMOCK_PP_COMMA(...) ,

// Returns the only argument.
#define KMOCK_PP_IDENTITY(_1) _1

// Evaluates to the number of arguments after expansion.
//
//   #define PAIR x, y
//
//   KMOCK_PP_NARG() => 1
//   KMOCK_PP_NARG(x) => 1
//   KMOCK_PP_NARG(x, y) => 2
//   KMOCK_PP_NARG(PAIR) => 2
//
// Requires: the number of arguments after expansion is at most 15.
#define KMOCK_PP_NARG(...) \
  KMOCK_PP_INTERNAL_16TH(  \
      (__VA_ARGS__, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))

// Returns 1 if the expansion of arguments has an unprotected comma. Otherwise
// returns 0. Requires no more than 15 unprotected commas.
#define KMOCK_PP_HAS_COMMA(...) \
  KMOCK_PP_INTERNAL_16TH(       \
      (__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0))

// Returns the first argument.
#define KMOCK_PP_HEAD(...) KMOCK_PP_INTERNAL_HEAD((__VA_ARGS__, unusedArg))

// Returns the tail. A variadic list of all arguments minus the first. Requires
// at least one argument.
#define KMOCK_PP_TAIL(...) KMOCK_PP_INTERNAL_TAIL((__VA_ARGS__))

// Calls CAT(_Macro, NARG(__VA_ARGS__))(__VA_ARGS__)
#define KMOCK_PP_VARIADIC_CALL(_Macro, ...) \
  KMOCK_PP_IDENTITY(                        \
      KMOCK_PP_CAT(_Macro, KMOCK_PP_NARG(__VA_ARGS__))(__VA_ARGS__))

// If the arguments after expansion have no tokens, evaluates to `1`. Otherwise
// evaluates to `0`.
//
// Requires: * the number of arguments after expansion is at most 15.
//           * If the argument is a macro, it must be able to be called with one
//             argument.
//
// Implementation details:
//
// There is one case when it generates a compile error: if the argument is macro
// that cannot be called with one argument.
//
//   #define M(a, b)  // it doesn't matter what it expands to
//
//   // Expected: expands to `0`.
//   // Actual: compile error.
//   KMOCK_PP_IS_EMPTY(M)
//
// There are 4 cases tested:
//
// * __VA_ARGS__ possible expansion has no unparen'd commas. Expected 0.
// * __VA_ARGS__ possible expansion is not enclosed in parenthesis. Expected 0.
// * __VA_ARGS__ possible expansion is not a macro that ()-evaluates to a comma.
//   Expected 0
// * __VA_ARGS__ is empty, or has unparen'd commas, or is enclosed in
//   parenthesis, or is a macro that ()-evaluates to comma. Expected 1.
//
// We trigger detection on '0001', i.e. on empty.
#define KMOCK_PP_IS_EMPTY(...)                                               \
  KMOCK_PP_INTERNAL_IS_EMPTY(KMOCK_PP_HAS_COMMA(__VA_ARGS__),                \
                             KMOCK_PP_HAS_COMMA(KMOCK_PP_COMMA __VA_ARGS__), \
                             KMOCK_PP_HAS_COMMA(__VA_ARGS__()),              \
                             KMOCK_PP_HAS_COMMA(KMOCK_PP_COMMA __VA_ARGS__()))

// Evaluates to _Then if _Cond is 1 and _Else if _Cond is 0.
#define KMOCK_PP_IF(_Cond, _Then, _Else) \
  KMOCK_PP_CAT(KMOCK_PP_INTERNAL_IF_, _Cond)(_Then, _Else)

// Similar to KMOCK_PP_IF but takes _Then and _Else in parentheses.
//
// KMOCK_PP_GENERIC_IF(1, (a, b, c), (d, e, f)) => a, b, c
// KMOCK_PP_GENERIC_IF(0, (a, b, c), (d, e, f)) => d, e, f
//
#define KMOCK_PP_GENERIC_IF(_Cond, _Then, _Else) \
  KMOCK_PP_REMOVE_PARENS(KMOCK_PP_IF(_Cond, _Then, _Else))

// Evaluates to the number of arguments after expansion. Identifies 'empty' as
// 0.
//
//   #define PAIR x, y
//
//   KMOCK_PP_NARG0() => 0
//   KMOCK_PP_NARG0(x) => 1
//   KMOCK_PP_NARG0(x, y) => 2
//   KMOCK_PP_NARG0(PAIR) => 2
//
// Requires: * the number of arguments after expansion is at most 15.
//           * If the argument is a macro, it must be able to be called with one
//             argument.
#define KMOCK_PP_NARG0(...) \
  KMOCK_PP_IF(KMOCK_PP_IS_EMPTY(__VA_ARGS__), 0, KMOCK_PP_NARG(__VA_ARGS__))

// Expands to 1 if the first argument starts with something in parentheses,
// otherwise to 0.
#define KMOCK_PP_IS_BEGIN_PARENS(...)                              \
  KMOCK_PP_HEAD(KMOCK_PP_CAT(KMOCK_PP_INTERNAL_IBP_IS_VARIADIC_R_, \
                             KMOCK_PP_INTERNAL_IBP_IS_VARIADIC_C __VA_ARGS__))

// Expands to 1 is there is only one argument and it is enclosed in parentheses.
#define KMOCK_PP_IS_ENCLOSED_PARENS(...)             \
  KMOCK_PP_IF(KMOCK_PP_IS_BEGIN_PARENS(__VA_ARGS__), \
              KMOCK_PP_IS_EMPTY(KMOCK_PP_EMPTY __VA_ARGS__), 0)

// Remove the parens, requires KMOCK_PP_IS_ENCLOSED_PARENS(args) => 1.
#define KMOCK_PP_REMOVE_PARENS(...) KMOCK_PP_INTERNAL_REMOVE_PARENS __VA_ARGS__

// Expands to _Macro(0, _Data, e1) _Macro(1, _Data, e2) ... _Macro(K -1, _Data,
// eK) as many of KMOCK_INTERNAL_NARG0 _Tuple.
// Requires: * |_Macro| can be called with 3 arguments.
//           * |_Tuple| expansion has no more than 15 elements.
#define KMOCK_PP_FOR_EACH(_Macro, _Data, _Tuple)                        \
  KMOCK_PP_CAT(KMOCK_PP_INTERNAL_FOR_EACH_IMPL_, KMOCK_PP_NARG0 _Tuple) \
  (0, _Macro, _Data, _Tuple)

// Expands to _Macro(0, _Data, ) _Macro(1, _Data, ) ... _Macro(K - 1, _Data, )
// Empty if _K = 0.
// Requires: * |_Macro| can be called with 3 arguments.
//           * |_K| literal between 0 and 15
#define KMOCK_PP_REPEAT(_Macro, _Data, _N)           \
  KMOCK_PP_CAT(KMOCK_PP_INTERNAL_FOR_EACH_IMPL_, _N) \
  (0, _Macro, _Data, KMOCK_PP_INTENRAL_EMPTY_TUPLE)

// Increments the argument, requires the argument to be between 0 and 15.
#define KMOCK_PP_INC(_i) KMOCK_PP_CAT(KMOCK_PP_INTERNAL_INC_, _i)

// Returns comma if _i != 0. Requires _i to be between 0 and 15.
#define KMOCK_PP_COMMA_IF(_i) KMOCK_PP_CAT(KMOCK_PP_INTERNAL_COMMA_IF_, _i)

// Internal details follow. Do not use any of these symbols outside of this
// file or we will break your code.
#define KMOCK_PP_INTENRAL_EMPTY_TUPLE (, , , , , , , , , , , , , , , )
#define KMOCK_PP_INTERNAL_CAT(_1, _2) _1##_2
#define KMOCK_PP_INTERNAL_STRINGIZE(...) #__VA_ARGS__
#define KMOCK_PP_INTERNAL_CAT_5(_1, _2, _3, _4, _5) _1##_2##_3##_4##_5
#define KMOCK_PP_INTERNAL_IS_EMPTY(_1, _2, _3, _4)                             \
  KMOCK_PP_HAS_COMMA(KMOCK_PP_INTERNAL_CAT_5(KMOCK_PP_INTERNAL_IS_EMPTY_CASE_, \
                                             _1, _2, _3, _4))
#define KMOCK_PP_INTERNAL_IS_EMPTY_CASE_0001 ,
#define KMOCK_PP_INTERNAL_IF_1(_Then, _Else) _Then
#define KMOCK_PP_INTERNAL_IF_0(_Then, _Else) _Else

// Because of MSVC treating a token with a comma in it as a single token when
// passed to another macro, we need to force it to evaluate it as multiple
// tokens. We do that by using a "IDENTITY(MACRO PARENTHESIZED_ARGS)" macro. We
// define one per possible macro that relies on this behavior. Note "_Args" must
// be parenthesized.
#define KMOCK_PP_INTERNAL_INTERNAL_16TH(_1, _2, _3, _4, _5, _6, _7, _8, _9, \
                                        _10, _11, _12, _13, _14, _15, _16,  \
                                        ...)                                \
  _16
#define KMOCK_PP_INTERNAL_16TH(_Args) \
  KMOCK_PP_IDENTITY(KMOCK_PP_INTERNAL_INTERNAL_16TH _Args)
#define KMOCK_PP_INTERNAL_INTERNAL_HEAD(_1, ...) _1
#define KMOCK_PP_INTERNAL_HEAD(_Args) \
  KMOCK_PP_IDENTITY(KMOCK_PP_INTERNAL_INTERNAL_HEAD _Args)
#define KMOCK_PP_INTERNAL_INTERNAL_TAIL(_1, ...) __VA_ARGS__
#define KMOCK_PP_INTERNAL_TAIL(_Args) \
  KMOCK_PP_IDENTITY(KMOCK_PP_INTERNAL_INTERNAL_TAIL _Args)

#define KMOCK_PP_INTERNAL_IBP_IS_VARIADIC_C(...) 1 _
#define KMOCK_PP_INTERNAL_IBP_IS_VARIADIC_R_1 1,
#define KMOCK_PP_INTERNAL_IBP_IS_VARIADIC_R_KMOCK_PP_INTERNAL_IBP_IS_VARIADIC_C \
  0,
#define KMOCK_PP_INTERNAL_REMOVE_PARENS(...) __VA_ARGS__
#define KMOCK_PP_INTERNAL_INC_0 1
#define KMOCK_PP_INTERNAL_INC_1 2
#define KMOCK_PP_INTERNAL_INC_2 3
#define KMOCK_PP_INTERNAL_INC_3 4
#define KMOCK_PP_INTERNAL_INC_4 5
#define KMOCK_PP_INTERNAL_INC_5 6
#define KMOCK_PP_INTERNAL_INC_6 7
#define KMOCK_PP_INTERNAL_INC_7 8
#define KMOCK_PP_INTERNAL_INC_8 9
#define KMOCK_PP_INTERNAL_INC_9 10
#define KMOCK_PP_INTERNAL_INC_10 11
#define KMOCK_PP_INTERNAL_INC_11 12
#define KMOCK_PP_INTERNAL_INC_12 13
#define KMOCK_PP_INTERNAL_INC_13 14
#define KMOCK_PP_INTERNAL_INC_14 15
#define KMOCK_PP_INTERNAL_INC_15 16
#define KMOCK_PP_INTERNAL_COMMA_IF_0
#define KMOCK_PP_INTERNAL_COMMA_IF_1 ,
#define KMOCK_PP_INTERNAL_COMMA_IF_2 ,
#define KMOCK_PP_INTERNAL_COMMA_IF_3 ,
#define KMOCK_PP_INTERNAL_COMMA_IF_4 ,
#define KMOCK_PP_INTERNAL_COMMA_IF_5 ,
#define KMOCK_PP_INTERNAL_COMMA_IF_6 ,
#define KMOCK_PP_INTERNAL_COMMA_IF_7 ,
#define KMOCK_PP_INTERNAL_COMMA_IF_8 ,
#define KMOCK_PP_INTERNAL_COMMA_IF_9 ,
#define KMOCK_PP_INTERNAL_COMMA_IF_10 ,
#define KMOCK_PP_INTERNAL_COMMA_IF_11 ,
#define KMOCK_PP_INTERNAL_COMMA_IF_12 ,
#define KMOCK_PP_INTERNAL_COMMA_IF_13 ,
#define KMOCK_PP_INTERNAL_COMMA_IF_14 ,
#define KMOCK_PP_INTERNAL_COMMA_IF_15 ,
#define KMOCK_PP_INTERNAL_CALL_MACRO(_Macro, _i, _Data, _element) \
  _Macro(_i, _Data, _element)
#define KMOCK_PP_INTERNAL_FOR_EACH_IMPL_0(_i, _Macro, _Data, _Tuple)
#define KMOCK_PP_INTERNAL_FOR_EACH_IMPL_1(_i, _Macro, _Data, _Tuple) \
  KMOCK_PP_INTERNAL_CALL_MACRO(_Macro, _i, _Data, KMOCK_PP_HEAD _Tuple)
#define KMOCK_PP_INTERNAL_FOR_EACH_IMPL_2(_i, _Macro, _Data, _Tuple)    \
  KMOCK_PP_INTERNAL_CALL_MACRO(_Macro, _i, _Data, KMOCK_PP_HEAD _Tuple) \
  KMOCK_PP_INTERNAL_FOR_EACH_IMPL_1(KMOCK_PP_INC(_i), _Macro, _Data,    \
                                    (KMOCK_PP_TAIL _Tuple))
#define KMOCK_PP_INTERNAL_FOR_EACH_IMPL_3(_i, _Macro, _Data, _Tuple)    \
  KMOCK_PP_INTERNAL_CALL_MACRO(_Macro, _i, _Data, KMOCK_PP_HEAD _Tuple) \
  KMOCK_PP_INTERNAL_FOR_EACH_IMPL_2(KMOCK_PP_INC(_i), _Macro, _Data,    \
                                    (KMOCK_PP_TAIL _Tuple))
#define KMOCK_PP_INTERNAL_FOR_EACH_IMPL_4(_i, _Macro, _Data, _Tuple)    \
  KMOCK_PP_INTERNAL_CALL_MACRO(_Macro, _i, _Data, KMOCK_PP_HEAD _Tuple) \
  KMOCK_PP_INTERNAL_FOR_EACH_IMPL_3(KMOCK_PP_INC(_i), _Macro, _Data,    \
                                    (KMOCK_PP_TAIL _Tuple))
#define KMOCK_PP_INTERNAL_FOR_EACH_IMPL_5(_i, _Macro, _Data, _Tuple)    \
  KMOCK_PP_INTERNAL_CALL_MACRO(_Macro, _i, _Data, KMOCK_PP_HEAD _Tuple) \
  KMOCK_PP_INTERNAL_FOR_EACH_IMPL_4(KMOCK_PP_INC(_i), _Macro, _Data,    \
                                    (KMOCK_PP_TAIL _Tuple))
#define KMOCK_PP_INTERNAL_FOR_EACH_IMPL_6(_i, _Macro, _Data, _Tuple)    \
  KMOCK_PP_INTERNAL_CALL_MACRO(_Macro, _i, _Data, KMOCK_PP_HEAD _Tuple) \
  KMOCK_PP_INTERNAL_FOR_EACH_IMPL_5(KMOCK_PP_INC(_i), _Macro, _Data,    \
                                    (KMOCK_PP_TAIL _Tuple))
#define KMOCK_PP_INTERNAL_FOR_EACH_IMPL_7(_i, _Macro, _Data, _Tuple)    \
  KMOCK_PP_INTERNAL_CALL_MACRO(_Macro, _i, _Data, KMOCK_PP_HEAD _Tuple) \
  KMOCK_PP_INTERNAL_FOR_EACH_IMPL_6(KMOCK_PP_INC(_i), _Macro, _Data,    \
                                    (KMOCK_PP_TAIL _Tuple))
#define KMOCK_PP_INTERNAL_FOR_EACH_IMPL_8(_i, _Macro, _Data, _Tuple)    \
  KMOCK_PP_INTERNAL_CALL_MACRO(_Macro, _i, _Data, KMOCK_PP_HEAD _Tuple) \
  KMOCK_PP_INTERNAL_FOR_EACH_IMPL_7(KMOCK_PP_INC(_i), _Macro, _Data,    \
                                    (KMOCK_PP_TAIL _Tuple))
#define KMOCK_PP_INTERNAL_FOR_EACH_IMPL_9(_i, _Macro, _Data, _Tuple)    \
  KMOCK_PP_INTERNAL_CALL_MACRO(_Macro, _i, _Data, KMOCK_PP_HEAD _Tuple) \
  KMOCK_PP_INTERNAL_FOR_EACH_IMPL_8(KMOCK_PP_INC(_i), _Macro, _Data,    \
                                    (KMOCK_PP_TAIL _Tuple))
#define KMOCK_PP_INTERNAL_FOR_EACH_IMPL_10(_i, _Macro, _Data, _Tuple)   \
  KMOCK_PP_INTERNAL_CALL_MACRO(_Macro, _i, _Data, KMOCK_PP_HEAD _Tuple) \
  KMOCK_PP_INTERNAL_FOR_EACH_IMPL_9(KMOCK_PP_INC(_i), _Macro, _Data,    \
                                    (KMOCK_PP_TAIL _Tuple))
#define KMOCK_PP_INTERNAL_FOR_EACH_IMPL_11(_i, _Macro, _Data, _Tuple)   \
  KMOCK_PP_INTERNAL_CALL_MACRO(_Macro, _i, _Data, KMOCK_PP_HEAD _Tuple) \
  KMOCK_PP_INTERNAL_FOR_EACH_IMPL_10(KMOCK_PP_INC(_i), _Macro, _Data,   \
                                     (KMOCK_PP_TAIL _Tuple))
#define KMOCK_PP_INTERNAL_FOR_EACH_IMPL_12(_i, _Macro, _Data, _Tuple)   \
  KMOCK_PP_INTERNAL_CALL_MACRO(_Macro, _i, _Data, KMOCK_PP_HEAD _Tuple) \
  KMOCK_PP_INTERNAL_FOR_EACH_IMPL_11(KMOCK_PP_INC(_i), _Macro, _Data,   \
                                     (KMOCK_PP_TAIL _Tuple))
#define KMOCK_PP_INTERNAL_FOR_EACH_IMPL_13(_i, _Macro, _Data, _Tuple)   \
  KMOCK_PP_INTERNAL_CALL_MACRO(_Macro, _i, _Data, KMOCK_PP_HEAD _Tuple) \
  KMOCK_PP_INTERNAL_FOR_EACH_IMPL_12(KMOCK_PP_INC(_i), _Macro, _Data,   \
                                     (KMOCK_PP_TAIL _Tuple))
#define KMOCK_PP_INTERNAL_FOR_EACH_IMPL_14(_i, _Macro, _Data, _Tuple)   \
  KMOCK_PP_INTERNAL_CALL_MACRO(_Macro, _i, _Data, KMOCK_PP_HEAD _Tuple) \
  KMOCK_PP_INTERNAL_FOR_EACH_IMPL_13(KMOCK_PP_INC(_i), _Macro, _Data,   \
                                     (KMOCK_PP_TAIL _Tuple))
#define KMOCK_PP_INTERNAL_FOR_EACH_IMPL_15(_i, _Macro, _Data, _Tuple)   \
  KMOCK_PP_INTERNAL_CALL_MACRO(_Macro, _i, _Data, KMOCK_PP_HEAD _Tuple) \
  KMOCK_PP_INTERNAL_FOR_EACH_IMPL_14(KMOCK_PP_INC(_i), _Macro, _Data,   \
                                     (KMOCK_PP_TAIL _Tuple))

#endif  // KHULNASOFTMOCK_INCLUDE_KMOCK_INTERNAL_KMOCK_PP_H_
