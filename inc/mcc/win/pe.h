#ifndef PE_H
#define PE_H

#include "mcc/mcc_base.h"

#include <windows.h>
#include <winnt.h>

struct exe;

extern IMAGE_DOS_HEADER golden_dos_header;
extern BYTE golden_dos_stub[0x40];
extern IMAGE_NT_HEADERS golden_nt_headers;

#define NTHDR_FILE_ALIGN(nt_hdr) ((nt_hdr)->OptionalHeader.FileAlignment)
#define NTHDR_SEC_ALIGN(nt_hdr) ((nt_hdr)->OptionalHeader.SectionAlignment)
#define NTHDR_IMG_BASE(nt_hdr) ((nt_hdr)->OptionalHeader.ImageBase)

__BEGIN_DECLS

static inline int _EXE_FILE_HDR_SIZE(struct exe *exe)
{
	return sizeof(IMAGE_DOS_HEADER) + sizeof(golden_dos_stub) + sizeof(IMAGE_NT_HEADERS);
}

static inline int _EXE_SEC_HDR_SIZE(struct exe *exe)
{
	return sizeof(IMAGE_SECTION_HEADER);
}

void __need_init pe_verify_header();
void __need_init pe_verify_dos_header();
void __need_init pe_verify_nt_header();

__END_DECLS

#endif