#define NO_DEBUG 1

#include <stdio.h>
#include <stddef.h>

#include "mcc/type.h"
#include "mcc/size.h"
#include "mcc/log.h"

#define TYPE_INIT(type)	types[type].size = type##_SIZE, types[type].align = type##_ALIGN;

__BEGIN_DECLS

// basic types
Type types[VOID_T - BOOLEAN_T + 1];

static const char * type_names[] = {
	"bool",
	"char", "uchar",
	"short", "ushort",
	"int", "uint",
	"long", "ulong", "longlong", "ulonglong",
	"float", "double", "ldouble", "lldouble",
	"pointer", "void",
	"function", "array", "union","struct",
	"string",
};

void init_types()
{
	TYPE_INIT(BOOLEAN_T);
	TYPE_INIT(CHAR_T);
	TYPE_INIT(UCHAR_T);
	TYPE_INIT(SHORT_T);
	TYPE_INIT(USHORT_T);
	TYPE_INIT(INT_T);
	TYPE_INIT(UINT_T);
	TYPE_INIT(LONG_T);
	TYPE_INIT(ULONG_T);
	TYPE_INIT(LLONG_T);
	TYPE_INIT(ULLONG_T);
	TYPE_INIT(FLOAT_T);
	TYPE_INIT(DOUBLE_T);
	TYPE_INIT(LDOUBLE_T);
	TYPE_INIT(LLDOUBLE_T);
	TYPE_INIT(PTR_T);
	TYPE_INIT(VOID_T);
}

PType get_type_info(int type)
{
	if (type < 0 || type > VOID_T) {
		debug("type: %d\n", type);
		return NULL;
	}
	debug("%p\n", &types[type]);
	return &types[type];
}

const char *get_type_names(int i)
{
	if (i < 0 || (unsigned int)i > ARRAY_SIZE(type_names))
		return NULL;
	return type_names[i];
}

const char *type_basic_info(Type *t)
{
	int a = t->t;

	if ((a & 0xF) == MT_CHAR)
		return "char";
	else if ((a & 0xF) == MT_SHORT)
		return "short";
	else if ((a & 0xF) == MT_INT)
		return "int";
	else if ((a & 0xF) == MT_LLONG)
		return "long";
	else if ((a & 0xF) == MT_FLOAT)
		return "float";
	else if ((a & 0xF) == MT_DOUBLE)
		return "double";
	else if ((a & 0xF) == MT_VOID)
		return "void";
	else if ((a & 0xF) == MT_FUNC)
		return "function";
	
	return "<NA>";
}

unsigned int type_size(Type *t)
{
	int base_type = BASIC_TYPE(t->t);
	int size;

	switch (base_type) {
	case MT_CHAR:
		size = 1;
		break;
	case MT_SHORT:
		size = 2;
		break;
	case MT_INT:
		size = 4;
		break;
	case MT_LLONG:
		size = 8;
		break;
	case MT_FLOAT:
		size = 4;
		break;
	case MT_DOUBLE:
		size = 8;
		break;
	case MT_VOID:
		size = 1;
		break;
	case MT_FUNC:
		size = 4;
		break;
	default:
		size = 1;
		break;
	}

	return size;
}

unsigned int type_align(Type *t)
{
	int base_type = BASIC_TYPE(t->t);
	int align;

	switch (base_type) {
	case MT_CHAR:
		align = 1;
		break;
	case MT_SHORT:
		align = 2;
		break;
	case MT_INT:
		align = 4;
		break;
	case MT_LLONG:
		align = 8;
		break;
	case MT_FLOAT:
		align = 4;
		break;
	case MT_DOUBLE:
		align = 8;
		break;
	case MT_VOID:
		align = 1;
		break;
	case MT_FUNC:
		align = 4;
		break;
	default:
		align = 1;
		break;
	}

	return align;
}

__END_DECLS