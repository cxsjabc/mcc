#define NO_DEBUG 1

#include <stdio.h>
#include <stddef.h>

#include "mcc/type.h"
#include "mcc/size.h"
#include "mcc/log.h"

#define TYPE_INIT(type)	types[type].size = type##_SIZE, types[type].align = type##_ALIGN;

__BEGIN_DECLS

// basic types
Type types[VOID - BOOL + 1];

static const char * type_names[] = {
	"bool",
	"char", "uchar",
	"short", "ushort",
	"int", "uint",
	"long", "ulong", "longlong", "ulonglong",
	"float", "double", "ldouble", "lldouble",
	"pointer", "void",
	"function", "array", "union","struct",
};

void init_types()
{
	TYPE_INIT(BOOL);
	TYPE_INIT(CHAR);
	TYPE_INIT(UCHAR);
	TYPE_INIT(SHORT);
	TYPE_INIT(USHORT);
	TYPE_INIT(INT);
	TYPE_INIT(UINT);
	TYPE_INIT(LONG);
	TYPE_INIT(ULONG);
	TYPE_INIT(LLONG);
	TYPE_INIT(ULLONG);
	TYPE_INIT(FLOAT);
	TYPE_INIT(DOUBLE);
	TYPE_INIT(LDOUBLE);
	TYPE_INIT(LLDOUBLE);
	TYPE_INIT(PTR);
	TYPE_INIT(VOID);
}

PType get_type_info(int type)
{
	if (type < 0 || type > VOID) {
		debug("type: %d\n", type);
		return NULL;
	}
	debug("%p\n", &types[type]);
	return &types[type];
}

const char * get_type_names(int i)
{
	if (i < 0 || (unsigned int)i > ARRAY_SIZE(type_names))
		return NULL;
	return type_names[i];
}

__END_DECLS