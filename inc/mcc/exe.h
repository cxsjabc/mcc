#ifndef EXE_H
#define EXE_H

#include "mcc/mcc_base.h"

#include "mcc/array.h"
#include "mcc/section.h"
#include "mcc/type.h"

#if defined(_WINDOWS) || defined(_MSC_VER)
#include "mcc/win/pe.h"
#else
#include "mcc/linux/elf.h"
#endif

typedef struct exe
{
	short machine;
	DynArray secs;
	int sec_cnt;

	FILE *f;
	int file_align;
	int mem_align;
} *Exe;

#define EXE_MACHINE(exe) ((exe)->machine)
#define EXE_SEC_CNT(exe) ((exe)->sec_cnt)
#define EXE_SECTION_COUNT (SECTION_TYPE_LINKSYM_STR - SECTION_TYPE_TEXT + 1)

#define EXE_SEC(exe, idx) vec_at((exe)->secs, idx)

__BEGIN_DECLS

Exe exe_alloc();
void exe_free(Exe c);

#define EXE_FILE_HDR_SIZE(exe) _EXE_FILE_HDR_SIZE(exe)
#define EXE_SEC_HDR_SIZE(exe) _EXE_SEC_HDR_SIZE(exe)
#define EXE_SEC_HDR_TOTAL_SIZE(exe) (EXE_SEC_HDR_SIZE(exe) * ((exe)->sec_cnt))
#define EXE_DATA_OFFSET(exe) (EXE_FILE_HDR_SIZE(exe) + EXE_SEC_HDR_TOTAL_SIZE(exe))

int exe_gen(const char *name);
int exe_write_file_header(Exe exe);
int exe_write_section_data(Exe exe);
int exe_write_section_header(Exe exe);

unsigned int exe_get_file_align(Exe exe);
unsigned int exe_get_header_aligned(Exe exe);

__END_DECLS

#endif