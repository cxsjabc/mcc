#ifndef SIZE_H
#define SIZE_H

#include "mcc/mcc_base.h"

// This is compiler-dependent.

#define BOOL_SIZE 1
#define CHAR_SIZE 1
#define SHORT_SIZE 2
#define INT_SIZE 4
#define LONG_SIZE 4
#define LLONG_SIZE 8

#define FLOAT_SIZE 4
#define DOUBLE_SIZE 8
#define LDOUBLE_SIZE 8
#define LLDOUBLE_SIZE 8

#define PTR_SIZE 4
#define VOID_SIZE 1

#define UCHAR_SIZE CHAR_SIZE
#define USHORT_SIZE SHORT_SIZE
#define UINT_SIZE INT_SIZE
#define ULONG_SIZE LONG_SIZE
#define ULLONG_SIZE LLONG_SIZE

// align
#define BOOL_ALIGN BOOL_SIZE
#define CHAR_ALIGN CHAR_SIZE
#define SHORT_ALIGN SHORT_SIZE
#define INT_ALIGN INT_SIZE
#define LONG_ALIGN LONG_SIZE
#define LLONG_ALIGN LLONG_SIZE

#define FLOAT_ALIGN FLOAT_SIZE
#define DOUBLE_ALIGN DOUBLE_SIZE
#define LDOUBLE_ALIGN LDOUBLE_SIZE
#define LLDOUBLE_ALIGN LLDOUBLE_SIZE

#define PTR_ALIGN PTR_SIZE
#define VOID_ALIGN VOID_SIZE

#define UCHAR_ALIGN CHAR_ALIGN
#define USHORT_ALIGN SHORT_ALIGN
#define UINT_ALIGN INT_ALIGN
#define ULONG_ALIGN LONG_ALIGN
#define ULLONG_ALIGN LLONG_ALIGN

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#define SIZEOF(type) ((unsigned int)(sizeof(type)))

__BEGIN_DECLS

void check_type_size();

__END_DECLS

#endif
