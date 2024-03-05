#ifndef SIZE_H
#define SIZE_H

#include "mcc/mcc_base.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// This is compiler-dependent.

#define BOOLEAN_T_SIZE 1
#define CHAR_T_SIZE 1
#define SHORT_T_SIZE 2
#define INT_T_SIZE 4
#define LONG_T_SIZE 4
#define LLONG_T_SIZE 8

#define FLOAT_T_SIZE 4
#define DOUBLE_T_SIZE 8
#define LDOUBLE_T_SIZE 8
#define LLDOUBLE_T_SIZE 8

#define PTR_T_SIZE 4
#define VOID_T_SIZE 1

#define UCHAR_T_SIZE CHAR_T_SIZE
#define USHORT_T_SIZE SHORT_T_SIZE
#define UINT_T_SIZE INT_T_SIZE
#define ULONG_T_SIZE LONG_T_SIZE
#define ULLONG_T_SIZE LLONG_T_SIZE

// align
#define BOOLEAN_T_ALIGN BOOLEAN_T_SIZE
#define CHAR_T_ALIGN CHAR_T_SIZE
#define SHORT_T_ALIGN SHORT_T_SIZE
#define INT_T_ALIGN INT_T_SIZE
#define LONG_T_ALIGN LONG_T_SIZE
#define LLONG_T_ALIGN LLONG_T_SIZE

#define FLOAT_T_ALIGN FLOAT_T_SIZE
#define DOUBLE_T_ALIGN DOUBLE_T_SIZE
#define LDOUBLE_T_ALIGN LDOUBLE_T_SIZE
#define LLDOUBLE_T_ALIGN LLDOUBLE_T_SIZE

#define PTR_T_ALIGN PTR_T_SIZE
#define VOID_T_ALIGN VOID_T_SIZE

#define UCHAR_T_ALIGN CHAR_T_ALIGN
#define USHORT_T_ALIGN SHORT_T_ALIGN
#define UINT_T_ALIGN INT_T_ALIGN
#define ULONG_T_ALIGN LONG_T_ALIGN
#define ULLONG_T_ALIGN LLONG_T_ALIGN

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#define asize ARRAY_SIZE

#define SIZEOF(type) ((unsigned int)(sizeof(type)))

__BEGIN_DECLS

void check_type_size();

__END_DECLS

#endif
