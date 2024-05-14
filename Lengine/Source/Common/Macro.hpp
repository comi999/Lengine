#pragma once

#define EXPAND(x) x

#define GET_MACRO_1(_1, x, ...) x
#define GET_MACRO_2(_1, _2, x, ...) x
#define GET_MACRO_3(_1, _2, _3, x, ...) x
#define GET_MACRO_4(_1, _2, _3, _4, x, ...) x
#define GET_MACRO_5(_1, _2, _3, _4, _5, x, ...) x
#define GET_MACRO_6(_1, _2, _3, _4, _5, _6, x, ...) x
#define GET_MACRO_7(_1, _2, _3, _4, _5, _6, _7, x, ...) x
#define GET_MACRO_8(_1, _2, _3, _4, _5, _6, _7, _8, x, ...) x
#define GET_MACRO_9(_1, _2, _3, _4, _5, _6, _7, _8, _9, x, ...) x
#define GET_MACRO_10(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, x, ...) x
#define GET_MACRO_11(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, x, ...) x
#define GET_MACRO_12(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, x, ...) x
#define GET_MACRO_13(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, x, ...) x
#define GET_MACRO_14(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, x, ...) x
#define GET_MACRO_15(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, x, ...) x
#define GET_MACRO_16(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, x, ...) x
#define GET_MACRO_17(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, x, ...) x
#define GET_MACRO_18(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, x, ...) x
#define GET_MACRO_19(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, x, ...) x
#define GET_MACRO_20(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, x, ...) x

#define COMBINE_1(_1) _1
#define COMBINE_2(_1, _2) _1##_2
#define COMBINE_3(_1, _2, _3) _1##_2##_3
#define COMBINE_4(_1, _2, _3, _4) _1##_2##_3##_4
#define COMBINE_5(_1, _2, _3, _4, _5) _1##_2##_3##_4##_5
#define COMBINE_6(_1, _2, _3, _4, _5, _6) _1##_2##_3##_4##_5##_6
#define COMBINE_7(_1, _2, _3, _4, _5, _6, _7) _1##_2##_3##_4##_5##_6##_7
#define COMBINE_8(_1, _2, _3, _4, _5, _6, _7, _8) _1##_2##_3##_4##_5##_6##_7##_8
#define COMBINE_9(_1, _2, _3, _4, _5, _6, _7, _8, _9) _1##_2##_3##_4##_5##_6##_7##_8##_9
#define COMBINE_10(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10
#define COMBINE_11(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11
#define COMBINE_12(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11##_12
#define COMBINE_13(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11##_12##_13
#define COMBINE_14(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11##_12##_13##_14
#define COMBINE_15(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11##_12##_13##_14##_15
#define COMBINE_16(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11##_12##_13##_14##_15##_16
#define COMBINE_17(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11##_12##_13##_14##_15##_16##_17
#define COMBINE_18(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11##_12##_13##_14##_15##_16##_17##_18
#define COMBINE_19(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11##_12##_13##_14##_15##_16##_17##_18##_19
#define COMBINE_20(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11##_12##_13##_14##_15##_16##_17##_18##_19##_20
#define COMBINE(...) EXPAND(GET_MACRO_20(__VA_ARGS__, \
COMBINE_20, \
COMBINE_19, \
COMBINE_18, \
COMBINE_17, \
COMBINE_16, \
COMBINE_15, \
COMBINE_14, \
COMBINE_13, \
COMBINE_12, \
COMBINE_11, \
COMBINE_10, \
COMBINE_9, \
COMBINE_8, \
COMBINE_7, \
COMBINE_6, \
COMBINE_5, \
COMBINE_4, \
COMBINE_3, \
COMBINE_2, \
COMBINE_1  )(__VA_ARGS__))

#define WRAP(x, ...) x(__VA_ARGS__)
#define SURROUND(x, y) COMBINE(x, y, x)
#define CHR(x) #@x
#define STR(x) #x

#define GET_FIRST_1(_1) _1
#define GET_FIRST_N(_1, ...) _1
#define GET_FIRST(...) EXPAND(GET_MACRO_20(__VA_ARGS__, \
GET_FIRST_N, \
GET_FIRST_N, \
GET_FIRST_N, \
GET_FIRST_N, \
GET_FIRST_N, \
GET_FIRST_N, \
GET_FIRST_N, \
GET_FIRST_N, \
GET_FIRST_N, \
GET_FIRST_N, \
GET_FIRST_N, \
GET_FIRST_N, \
GET_FIRST_N, \
GET_FIRST_N, \
GET_FIRST_N, \
GET_FIRST_N, \
GET_FIRST_N, \
GET_FIRST_N, \
GET_FIRST_N, \
GET_FIRST_1  )(__VA_ARGS__))

#define GET_NOT_FIRST_1(_1)
#define GET_NOT_FIRST_N(_1, ...) __VA_ARGS__
#define GET_NOT_FIRST(...) EXPAND(GET_MACRO_20(__VA_ARGS__, \
GET_NOT_FIRST_N, \
GET_NOT_FIRST_N, \
GET_NOT_FIRST_N, \
GET_NOT_FIRST_N, \
GET_NOT_FIRST_N, \
GET_NOT_FIRST_N, \
GET_NOT_FIRST_N, \
GET_NOT_FIRST_N, \
GET_NOT_FIRST_N, \
GET_NOT_FIRST_N, \
GET_NOT_FIRST_N, \
GET_NOT_FIRST_N, \
GET_NOT_FIRST_N, \
GET_NOT_FIRST_N, \
GET_NOT_FIRST_N, \
GET_NOT_FIRST_N, \
GET_NOT_FIRST_N, \
GET_NOT_FIRST_N, \
GET_NOT_FIRST_N, \
GET_NOT_FIRST_1  )(__VA_ARGS__))

#define GET_LAST_1(_1) _1
#define GET_LAST_2(_1, _2) _2
#define GET_LAST_3(_1, _2, _3) _3
#define GET_LAST_4(_1, _2, _3, _4) _4
#define GET_LAST_5(_1, _2, _3, _4, _5) _5
#define GET_LAST_6(_1, _2, _3, _4, _5, _6) _6
#define GET_LAST_7(_1, _2, _3, _4, _5, _6, _7) _7
#define GET_LAST_8(_1, _2, _3, _4, _5, _6, _7, _8) _8
#define GET_LAST_9(_1, _2, _3, _4, _5, _6, _7, _8, _9) _9
#define GET_LAST_10(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10) _10
#define GET_LAST_11(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11) _11
#define GET_LAST_12(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12) _12
#define GET_LAST_13(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13) _13
#define GET_LAST_14(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14) _14
#define GET_LAST_15(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15) _15
#define GET_LAST_16(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16) _16
#define GET_LAST_17(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17) _17
#define GET_LAST_18(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18) _18
#define GET_LAST_19(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19) _19
#define GET_LAST_20(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20) _20
#define GET_LAST(...) EXPAND(GET_MACRO_20(__VA_ARGS__, \
GET_LAST_20, \
GET_LAST_19, \
GET_LAST_18, \
GET_LAST_17, \
GET_LAST_16, \
GET_LAST_15, \
GET_LAST_14, \
GET_LAST_13, \
GET_LAST_12, \
GET_LAST_11, \
GET_LAST_10, \
GET_LAST_9, \
GET_LAST_8, \
GET_LAST_7, \
GET_LAST_6, \
GET_LAST_5, \
GET_LAST_4, \
GET_LAST_3, \
GET_LAST_2, \
GET_LAST_1  )(__VA_ARGS__))

#define GET_NOT_LAST_1(_1)
#define GET_NOT_LAST_2(_1, _2) _1
#define GET_NOT_LAST_3(_1, _2, _3) _1, _2
#define GET_NOT_LAST_4(_1, _2, _3, _4) _1, _2, _3
#define GET_NOT_LAST_5(_1, _2, _3, _4, _5) _1, _2, _3, _4
#define GET_NOT_LAST_6(_1, _2, _3, _4, _5, _6) _1, _2, _3, _4, _5
#define GET_NOT_LAST_7(_1, _2, _3, _4, _5, _6, _7) _1, _2, _3, _4, _5, _6
#define GET_NOT_LAST_8(_1, _2, _3, _4, _5, _6, _7, _8) _1, _2, _3, _4, _5, _6, _7
#define GET_NOT_LAST_9(_1, _2, _3, _4, _5, _6, _7, _8, _9) _1, _2, _3, _4, _5, _6, _7, _8
#define GET_NOT_LAST_10(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10) _1, _2, _3, _4, _5, _6, _7, _8, _9
#define GET_NOT_LAST_11(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11) _1, _2, _3, _4, _5, _6, _7, _8, _9, _10
#define GET_NOT_LAST_12(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12) _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11
#define GET_NOT_LAST_13(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13) _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12
#define GET_NOT_LAST_14(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14) _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13
#define GET_NOT_LAST_15(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15) _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14
#define GET_NOT_LAST_16(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16) _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15
#define GET_NOT_LAST_17(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17) _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16
#define GET_NOT_LAST_18(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18) _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17
#define GET_NOT_LAST_19(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19) _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18
#define GET_NOT_LAST_20(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20) _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19
#define GET_NOT_LAST(...) EXPAND(GET_MACRO_20(__VA_ARGS__, \
GET_NOT_LAST_20, \
GET_NOT_LAST_19, \
GET_NOT_LAST_18, \
GET_NOT_LAST_17, \
GET_NOT_LAST_16, \
GET_NOT_LAST_15, \
GET_NOT_LAST_14, \
GET_NOT_LAST_13, \
GET_NOT_LAST_12, \
GET_NOT_LAST_11, \
GET_NOT_LAST_10, \
GET_NOT_LAST_9, \
GET_NOT_LAST_8, \
GET_NOT_LAST_7, \
GET_NOT_LAST_6, \
GET_NOT_LAST_5, \
GET_NOT_LAST_4, \
GET_NOT_LAST_3, \
GET_NOT_LAST_2, \
GET_NOT_LAST_1  )(__VA_ARGS__))

#define GET_REVERSED_1(...) __VA_ARGS__
#define GET_REVERSED_2(...) GET_LAST(__VA_ARGS__), GET_FIRST(__VA_ARGS__)
#define GET_REVERSED_3(...) GET_LAST(__VA_ARGS__), GET_REVERSED_1(GET_NOT_LAST(GET_NOT_FIRST(__VA_ARGS__))), GET_FIRST(__VA_ARGS__)
#define GET_REVERSED_4(...) GET_LAST(__VA_ARGS__), GET_REVERSED_2(GET_NOT_LAST(GET_NOT_FIRST(__VA_ARGS__))), GET_FIRST(__VA_ARGS__)
#define GET_REVERSED_5(...) GET_LAST(__VA_ARGS__), GET_REVERSED_3(GET_NOT_LAST(GET_NOT_FIRST(__VA_ARGS__))), GET_FIRST(__VA_ARGS__)
#define GET_REVERSED_6(...) GET_LAST(__VA_ARGS__), GET_REVERSED_4(GET_NOT_LAST(GET_NOT_FIRST(__VA_ARGS__))), GET_FIRST(__VA_ARGS__)
#define GET_REVERSED_7(...) GET_LAST(__VA_ARGS__), GET_REVERSED_5(GET_NOT_LAST(GET_NOT_FIRST(__VA_ARGS__))), GET_FIRST(__VA_ARGS__)
#define GET_REVERSED_8(...) GET_LAST(__VA_ARGS__), GET_REVERSED_6(GET_NOT_LAST(GET_NOT_FIRST(__VA_ARGS__))), GET_FIRST(__VA_ARGS__)
#define GET_REVERSED_9(...) GET_LAST(__VA_ARGS__), GET_REVERSED_7(GET_NOT_LAST(GET_NOT_FIRST(__VA_ARGS__))), GET_FIRST(__VA_ARGS__)
#define GET_REVERSED_10(...) GET_LAST(__VA_ARGS__), GET_REVERSED_8(GET_NOT_LAST(GET_NOT_FIRST(__VA_ARGS__))), GET_FIRST(__VA_ARGS__)
#define GET_REVERSED_11(...) GET_LAST(__VA_ARGS__), GET_REVERSED_9(GET_NOT_LAST(GET_NOT_FIRST(__VA_ARGS__))), GET_FIRST(__VA_ARGS__)
#define GET_REVERSED_12(...) GET_LAST(__VA_ARGS__), GET_REVERSED_10(GET_NOT_LAST(GET_NOT_FIRST(__VA_ARGS__))), GET_FIRST(__VA_ARGS__)
#define GET_REVERSED_13(...) GET_LAST(__VA_ARGS__), GET_REVERSED_11(GET_NOT_LAST(GET_NOT_FIRST(__VA_ARGS__))), GET_FIRST(__VA_ARGS__)
#define GET_REVERSED_14(...) GET_LAST(__VA_ARGS__), GET_REVERSED_12(GET_NOT_LAST(GET_NOT_FIRST(__VA_ARGS__))), GET_FIRST(__VA_ARGS__)
#define GET_REVERSED_15(...) GET_LAST(__VA_ARGS__), GET_REVERSED_13(GET_NOT_LAST(GET_NOT_FIRST(__VA_ARGS__))), GET_FIRST(__VA_ARGS__)
#define GET_REVERSED_16(...) GET_LAST(__VA_ARGS__), GET_REVERSED_14(GET_NOT_LAST(GET_NOT_FIRST(__VA_ARGS__))), GET_FIRST(__VA_ARGS__)
#define GET_REVERSED_17(...) GET_LAST(__VA_ARGS__), GET_REVERSED_15(GET_NOT_LAST(GET_NOT_FIRST(__VA_ARGS__))), GET_FIRST(__VA_ARGS__)
#define GET_REVERSED_18(...) GET_LAST(__VA_ARGS__), GET_REVERSED_16(GET_NOT_LAST(GET_NOT_FIRST(__VA_ARGS__))), GET_FIRST(__VA_ARGS__)
#define GET_REVERSED_19(...) GET_LAST(__VA_ARGS__), GET_REVERSED_17(GET_NOT_LAST(GET_NOT_FIRST(__VA_ARGS__))), GET_FIRST(__VA_ARGS__)
#define GET_REVERSED_20(...) GET_LAST(__VA_ARGS__), GET_REVERSED_18(GET_NOT_LAST(GET_NOT_FIRST(__VA_ARGS__))), GET_FIRST(__VA_ARGS__)
#define GET_REVERSED(...) EXPAND(GET_MACRO_20(__VA_ARGS__, \
GET_REVERSED_20, \
GET_REVERSED_19, \
GET_REVERSED_18, \
GET_REVERSED_17, \
GET_REVERSED_16, \
GET_REVERSED_15, \
GET_REVERSED_14, \
GET_REVERSED_13, \
GET_REVERSED_12, \
GET_REVERSED_11, \
GET_REVERSED_10, \
GET_REVERSED_9, \
GET_REVERSED_8, \
GET_REVERSED_7, \
GET_REVERSED_6, \
GET_REVERSED_5, \
GET_REVERSED_4, \
GET_REVERSED_3, \
GET_REVERSED_2, \
GET_REVERSED_1  )(__VA_ARGS__))

#define FOR_EACH_1(MACRO, _1) MACRO(_1)
#define FOR_EACH_2(MACRO, _1, _2) MACRO(_1),MACRO(_2)
#define FOR_EACH_3(MACRO, _1, _2, _3) MACRO(_1),MACRO(_2),MACRO(_3)
#define FOR_EACH_4(MACRO, _1, _2, _3, _4) MACRO(_1),MACRO(_2),MACRO(_3),MACRO(_4)
#define FOR_EACH_5(MACRO, _1, _2, _3, _4, _5) MACRO(_1),MACRO(_2),MACRO(_3),MACRO(_4),MACRO(_5)
#define FOR_EACH_6(MACRO, _1, _2, _3, _4, _5, _6) MACRO(_1),MACRO(_2),MACRO(_3),MACRO(_4),MACRO(_5),MACRO(_6)
#define FOR_EACH_7(MACRO, _1, _2, _3, _4, _5, _6, _7) MACRO(_1),MACRO(_2),MACRO(_3),MACRO(_4),MACRO(_5),MACRO(_6),MACRO(_7)
#define FOR_EACH_8(MACRO, _1, _2, _3, _4, _5, _6, _7, _8) MACRO(_1),MACRO(_2),MACRO(_3),MACRO(_4),MACRO(_5),MACRO(_6),MACRO(_7),MACRO(_8)
#define FOR_EACH_9(MACRO, _1, _2, _3, _4, _5, _6, _7, _8, _9) MACRO(_1),MACRO(_2),MACRO(_3),MACRO(_4),MACRO(_5),MACRO(_6),MACRO(_7),MACRO(_8),MACRO(_9)
#define FOR_EACH_10(MACRO, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10) MACRO(_1),MACRO(_2),MACRO(_3),MACRO(_4),MACRO(_5),MACRO(_6),MACRO(_7),MACRO(_8),MACRO(_9),MACRO(_10)
#define FOR_EACH_11(MACRO, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11) MACRO(_1),MACRO(_2),MACRO(_3),MACRO(_4),MACRO(_5),MACRO(_6),MACRO(_7),MACRO(_8),MACRO(_9),MACRO(_10),MACRO(_11)
#define FOR_EACH_12(MACRO, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12) MACRO(_1),MACRO(_2),MACRO(_3),MACRO(_4),MACRO(_5),MACRO(_6),MACRO(_7),MACRO(_8),MACRO(_9),MACRO(_10),MACRO(_11),MACRO(_12)
#define FOR_EACH_13(MACRO, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13) MACRO(_1),MACRO(_2),MACRO(_3),MACRO(_4),MACRO(_5),MACRO(_6),MACRO(_7),MACRO(_8),MACRO(_9),MACRO(_10),MACRO(_11),MACRO(_12),MACRO(_13)
#define FOR_EACH_14(MACRO, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14) MACRO(_1),MACRO(_2),MACRO(_3),MACRO(_4),MACRO(_5),MACRO(_6),MACRO(_7),MACRO(_8),MACRO(_9),MACRO(_10),MACRO(_11),MACRO(_12),MACRO(_13),MACRO(_14)
#define FOR_EACH_15(MACRO, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15) MACRO(_1),MACRO(_2),MACRO(_3),MACRO(_4),MACRO(_5),MACRO(_6),MACRO(_7),MACRO(_8),MACRO(_9),MACRO(_10),MACRO(_11),MACRO(_12),MACRO(_13),MACRO(_14),MACRO(_15)
#define FOR_EACH_16(MACRO, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16) MACRO(_1),MACRO(_2),MACRO(_3),MACRO(_4),MACRO(_5),MACRO(_6),MACRO(_7),MACRO(_8),MACRO(_9),MACRO(_10),MACRO(_11),MACRO(_12),MACRO(_13),MACRO(_14),MACRO(_15),MACRO(_16)
#define FOR_EACH_17(MACRO, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17) MACRO(_1),MACRO(_2),MACRO(_3),MACRO(_4),MACRO(_5),MACRO(_6),MACRO(_7),MACRO(_8),MACRO(_9),MACRO(_10),MACRO(_11),MACRO(_12),MACRO(_13),MACRO(_14),MACRO(_15),MACRO(_16),MACRO(_17)
#define FOR_EACH_18(MACRO, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18) MACRO(_1),MACRO(_2),MACRO(_3),MACRO(_4),MACRO(_5),MACRO(_6),MACRO(_7),MACRO(_8),MACRO(_9),MACRO(_10),MACRO(_11),MACRO(_12),MACRO(_13),MACRO(_14),MACRO(_15),MACRO(_16),MACRO(_17),MACRO(_18)
#define FOR_EACH_19(MACRO, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19) MACRO(_1),MACRO(_2),MACRO(_3),MACRO(_4),MACRO(_5),MACRO(_6),MACRO(_7),MACRO(_8),MACRO(_9),MACRO(_10),MACRO(_11),MACRO(_12),MACRO(_13),MACRO(_14),MACRO(_15),MACRO(_16),MACRO(_17),MACRO(_18),MACRO(_19)
#define FOR_EACH_20(MACRO, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20) MACRO(_1),MACRO(_2),MACRO(_3),MACRO(_4),MACRO(_5),MACRO(_6),MACRO(_7),MACRO(_8),MACRO(_9),MACRO(_10),MACRO(_11),MACRO(_12),MACRO(_13),MACRO(_14),MACRO(_15),MACRO(_16),MACRO(_17),MACRO(_18),MACRO(_19),MACRO(_20)
#define FOR_EACH(MACRO, ...) EXPAND(GET_MACRO_20(__VA_ARGS__, \
FOR_EACH_20, \
FOR_EACH_19, \
FOR_EACH_18, \
FOR_EACH_17, \
FOR_EACH_16, \
FOR_EACH_15, \
FOR_EACH_14, \
FOR_EACH_13, \
FOR_EACH_12, \
FOR_EACH_11, \
FOR_EACH_10, \
FOR_EACH_9, \
FOR_EACH_8, \
FOR_EACH_7, \
FOR_EACH_6, \
FOR_EACH_5, \
FOR_EACH_4, \
FOR_EACH_3, \
FOR_EACH_2, \
FOR_EACH_1  )(MACRO, __VA_ARGS__))

#define ADD_COMMA(x) x,

#define EXPAND_COMMA_SEPARATED_1(_1) _1
#define EXPAND_COMMA_SEPARATED_N(...) FOR_EACH(ADD_COMMA, GET_NOT_LAST(__VA_ARGS__)) GET_LAST(__VA_ARGS__)
#define EXPAND_COMMA_SEPARATED(...) EXPAND(GET_MACRO_20(__VA_ARGS__, \
EXPAND_COMMA_SEPARATED_N, \
EXPAND_COMMA_SEPARATED_N, \
EXPAND_COMMA_SEPARATED_N, \
EXPAND_COMMA_SEPARATED_N, \
EXPAND_COMMA_SEPARATED_N, \
EXPAND_COMMA_SEPARATED_N, \
EXPAND_COMMA_SEPARATED_N, \
EXPAND_COMMA_SEPARATED_N, \
EXPAND_COMMA_SEPARATED_N, \
EXPAND_COMMA_SEPARATED_N, \
EXPAND_COMMA_SEPARATED_N, \
EXPAND_COMMA_SEPARATED_N, \
EXPAND_COMMA_SEPARATED_N, \
EXPAND_COMMA_SEPARATED_N, \
EXPAND_COMMA_SEPARATED_N, \
EXPAND_COMMA_SEPARATED_N, \
EXPAND_COMMA_SEPARATED_N, \
EXPAND_COMMA_SEPARATED_N, \
EXPAND_COMMA_SEPARATED_N, \
EXPAND_COMMA_SEPARATED_1  )(__VA_ARGS__))