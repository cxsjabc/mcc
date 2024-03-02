#ifndef SECTION_HEADER_H
#define SECTION_HEADER_H

#include "mcc/mcc_base.h"

#include <windows.h>
#include <winnt.h>

#ifdef NO_WINNT
/* Copied from: C:\Program Files (x86)\Windows Kits\10\Include\10.0.17763.0\um\winnt.h */
typedef struct _IMAGE_SECTION_HEADER {
    BYTE    Name[IMAGE_SIZEOF_SHORT_NAME];
    union {
            DWORD   PhysicalAddress;
            DWORD   VirtualSize;
    } Misc;
    DWORD   VirtualAddress;
    DWORD   SizeOfRawData;
    DWORD   PointerToRawData;
    DWORD   PointerToRelocations;
    DWORD   PointerToLinenumbers;
    WORD    NumberOfRelocations;
    WORD    NumberOfLinenumbers;
    DWORD   Characteristics;
} IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;
#endif

#define _SECTOIN_SYM_CNT(section) (((Section)(section))->hdr.SizeOfRawData / sizeof(IMAGE_SYMBOL))
#define _SECTOIN_SYMBOL_OFFSET(section) (((Section)(section))->hdr.PointerToRawData)

typedef struct section
{
	int offset; // the first element, because it is used more frequently

	IMAGE_SECTION_HEADER hdr;

	unsigned char *data;
	unsigned int size;

	struct section *str_sec;
} *Section;

enum {
	SECTION_TYPE_TEXT,
	SECTION_TYPE_DATA,
	SECTION_TYPE_BSS,
	SECTION_TYPE_RDATA,
	SECTION_TYPE_IDATA,
	SECTION_TYPE_SYMBOL,
	SECTION_TYPE_SYMBOL_STR,
	SECTION_TYPE_RELOC,
	// the above sections are used for object file
	// the below sections are used for final executable file
	SECTION_TYPE_LINKSYM,
	SECTION_TYPE_LINKSYM_STR,

	SECTION_TYPE_MAX,
};

#define SEC_DATA_NEED_INIT(sec) ((sec)->hdr.Characteristics & IMAGE_SCN_CNT_INITIALIZED_DATA)

__BEGIN_DECLS

__END_DECLS

#endif