#ifndef DECL_H
#define DECL_H

#include "mcc/mcc_base.h"

#include "mcc/type.h"

__BEGIN_DECLS

int parse_global_decl();
int parse_declarator(Type *t, int *val);
void parse_parameter(Type *t);
int parse_type_specifier(Type *t);
void skip(int tok);

__END_DECLS

#endif