#ifndef SECTION_H
#define SECTION_H

#include "mcc/mcc_base.h"

#include "mcc/type.h"

#if defined(_WINDOWS) || defined(_MSC_VER)
#include "mcc/win/section_header.h"
#else
#include "mcc/linux/section_header.h"
#endif

#define OBJ_SECTION_COUNT (SECTION_TYPE_RELOC - SECTION_TYPE_TEXT + 1)
#define EXE_SECTION_COUNT (SECTION_TYPE_LINKSYM_STR - SECTION_TYPE_TEXT + 1)
#define SECTION_AT(idx) (Secs[(idx)])

#define SECTION_SYM_CNT(section) _SECTOIN_SYM_CNT(section)
#define SECTION_SYMBOL_OFFSET(section) _SECTOIN_SYMBOL_OFFSET(section)

extern Section Secs[SECTION_TYPE_MAX];

__BEGIN_DECLS

void __need_init global_section_init();
void global_section_free();

Section section_alloc(const char *name, int charactics);
void section_update_header(Section s, const char *name, int charactics);

void section_free(Section sec);

unsigned int section_cal_type_offset(Section s, Type *t);
void *section_realloc(Section s, unsigned int new_size);

void *section_use(Section s, unsigned int size);
char *section_use_str(Section sec, const char *str);

Section section_alloc_global_space(Type *t, int section_type, unsigned int *addr);
Section section_alloc_data_space(Type *t, int store_type, int need_init, unsigned int *addr);

void section_update_header_pointer(Section s, unsigned int data_offset);

__END_DECLS

#endif