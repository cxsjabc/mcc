#ifndef SECTION_HEADER_H
#define SECTION_HEADER_H

#include "mcc/mcc_base.h"

#include <winnt.h>
__BEGIN_DECLS

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
	int offset;
	unsigned char *data;
	unsigned int size;

	IMAGE_SECTION_HEADER hdr;
} *Section;

#endif