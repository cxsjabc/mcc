#ifndef TYPE_H
#define TYPE_H

#include "mcc/features.h"

enum {
    BOOL,
    CHAR, UCHAR,
    SHORT, USHORT,
    INT, UINT,
    LONG, ULONG, LLONG, ULLONG,
    FLOAT, DOUBLE, LDOUBLE, LLDOUBLE,
    PTR, VOID,
    FUNC, ARRAY, UNION, STRUCT,
};

typedef struct type
{
    int category; // INT or UINT or LONG ...
    int size;   // int: 4 bytes
    int align;  // int: 4 bytes
} Type, *PType;

__BEGIN_DECLS

void init_types();

PType get_type_info(int type);
const char * get_type_names(int i);

__END_DECLS

#endif