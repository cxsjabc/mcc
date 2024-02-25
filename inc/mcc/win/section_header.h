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

typedef struct section
{
	int offset; // the first element, because it is used more frequently

	IMAGE_SECTION_HEADER hdr;

	unsigned char *data;
	unsigned int size;
} *Section;

enum {
	SECTION_TYPE_TEXT,
	SECTION_TYPE_DATA,
	SECTION_TYPE_BSS,
	SECTION_TYPE_RDATA,
	SECTION_TYPE_IDATA,

	SECTION_TYPE_MAX,
};

#define SEC_DATA_NEED_INIT(sec) ((sec)->hdr.Characteristics & IMAGE_SCN_CNT_INITIALIZED_DATA)

__BEGIN_DECLS

__END_DECLS

#endif