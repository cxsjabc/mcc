#ifndef SECTION_H
#define SECTION_H

#include "mcc/mcc_base.h"

#include "mcc/type.h"

#if defined(_WINDOWS) || defined(_MSC_VER) || 1
#include "mcc/win/section_header.h"
#else
#include "mcc/section_header.h"
#endif

__BEGIN_DECLS

void __need_init global_section_init();
void global_section_free();

Section section_alloc(const char *name, int charactics);
void section_update_header(Section s, const char *name, int charactics);

void section_free(Section sec);

unsigned int section_cal_type_offset(Section s, Type *t);
int section_realloc(Section s, unsigned int new_size);
Section section_alloc_global_space(Type *t, int section_type, unsigned int *addr);
Section section_alloc_data_space(Type *t, int store_type, int need_init, unsigned int *addr);

__END_DECLS

#endif