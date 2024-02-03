#ifndef TYPE_H
#define TYPE_H

#include "mcc/mcc_base.h"

struct sym;

enum {
	MT_CHAR = 1,
	MT_SHORT = 2,
	MT_INT = 3,
	MT_LLONG = 4,
	MT_FLOAT = 5,
	MT_DOUBLE = 6,
	MT_VOID = 7,
	MT_FUNC = 8,
	MT_ARRAY = 9,

	MT_SIGNED = 0x10,
	MT_UNSIGNED = 0x20,

	MT_EXTERN = 0x100,
	MT_STATIC = 0x200,
	MT_REGISTER = 0x400,

	MT_LONG = 0x1000,
};

#define BASIC_TYPE(t) (t & 0xF)
#define IS_INT(t) (((t) & 0xF) == MT_INT)
#define IS_FUNC(t) (((t) & 0xF) == MT_FUNC)

enum {
	BOOLEAN_T,
	CHAR_T, UCHAR_T,
	SHORT_T, USHORT_T,
	INT_T, UINT_T,
	LONG_T, ULONG_T, LLONG_T, ULLONG_T,
	FLOAT_T, DOUBLE_T, LDOUBLE_T, LLDOUBLE_T,
	PTR_T, VOID_T,
	FUNC_T, ARRAY_T, UNION_T, STRUCT_T,
	STR_T,
};

typedef struct type
{
	int t;
	int category; // INT_T or UINT_T or LONG_T ...
	unsigned int is_unsigned : 1;
	unsigned int is_long : 2; // b'11 means "long long"
	int size;   // int: 4 bytes
	int align;  // int: 4 bytes

	struct sym *sym; // used for pointer types...
} Type, *PType;

__BEGIN_DECLS

void init_types();

PType get_type_info(int type);
const char * get_type_names(int i);

const char *type_basic_info(Type *t);

__END_DECLS

#endif