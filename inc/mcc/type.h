#ifndef TYPE_H
#define TYPE_H

#include "mcc/mcc_base.h"

enum {
	BOOLEAN_T,
	CHAR_T, UCHAR_T,
	SHORT_T, USHORT_T,
	INT_T, UINT_T,
	LONG_T, ULONG_T, LLONG_T, ULLONG_T,
	FLOAT_T, DOUBLE_T, LDOUBLE_T, LLDOUBLE_T,
	PTR_T, VOID_T,
	FUNC_T, ARRAY_T, UNION_T, STRUCT_T,
};

typedef struct type
{
	int category; // INT_T or UINT_T or LONG_T ...
	unsigned int is_unsigned : 1;
	unsigned int is_long : 2; // b'11 means "long long"
	int size;   // int: 4 bytes
	int align;  // int: 4 bytes
} Type, *PType;

__BEGIN_DECLS

void init_types();

PType get_type_info(int type);
const char * get_type_names(int i);

__END_DECLS

#endif