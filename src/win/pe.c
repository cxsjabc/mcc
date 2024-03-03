#include <stdio.h>
#include <windows.h>

#include "mcc/mcc_base.h"

#include "mcc/log.h"

__BEGIN_DECLS

// This comes from one .exe file!
// Fixed value!
IMAGE_DOS_HEADER golden_dos_header = {
	0x5A4D,         /* WORD   e_magic;          Magic number */
	0x0090,         /* WORD   e_cblp;           Bytes on last page of file */
	0x0003,         /* WORD   e_cp;             Pages in file */
	0x0000,         /* WORD   e_crlc;           Relocations */
	0x0004,         /* WORD   e_cparhdr;        Size of header in paragraphs */
	0x0000,         /* WORD   e_minalloc;       Minimum extra paragraphs needed */
	0xFFFF,         /* WORD   e_maxalloc;       Maximum extra paragraphs needed */
	0x0000,         /* WORD   e_ss;             Initial (relative) SS value */
	0x00B8,         /* WORD   e_sp;             Initial SP value */
	0x0000,         /* WORD   e_csum;           Checksum */
	0x0000,         /* WORD   e_ip;             Initial IP value */
	0x0000,         /* WORD   e_cs;             Initial (relative) CS value */
	0x0040,         /* WORD   e_lfarlc;         File address of relocation table */
	0x0000,         /* WORD   e_ovno;           Overlay number */
	{0, 0, 0, 0},   /* WORD   e_res[4];         Reserved words */
	0x0000,         /* WORD   e_oemid;          OEM identifier (for e_oeminfo) */
	0x0000,         /* WORD   e_oeminfo;        OEM information; e_oemid specific */
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},     /* WORD   e_res2[10];  Reserved words */
	0x00000080,     /* LONG   e_lfanew;         File address of new exe header */
};

// Fixed value!
BYTE golden_dos_stub[0x40] = {
	0x0E, 0x1F, 0xBA, 0x0E, 0x00, 0xB4, 0x09, 0xCD, 0x21, 0xB8, 0x01, 0x4C, 0xCD, 0x21, 0x54, 0x68,
	0x69, 0x73, 0x20, 0x70, 0x72, 0x6F, 0x67, 0x72, 0x61, 0x6D, 0x20, 0x63, 0x61, 0x6E, 0x6E, 0x6F,
	0x74, 0x20, 0x62, 0x65, 0x20, 0x72, 0x75, 0x6E, 0x20, 0x69, 0x6E, 0x20, 0x44, 0x4F, 0x53, 0x20,
	0x6D, 0x6F, 0x64, 0x65, 0x2E, 0x0D, 0x0D, 0x0A, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

// The header may be changed!
IMAGE_NT_HEADERS32 golder_nt_header = {
	0x00004550, /* DWORD Signature */
	{
		/* IMAGE_FILE_HEADER FileHeader */
		0x0000,		/* WORD    Machine; // Updated! */
		0x0000,		/* WORD    NumberOfSections; // Updated! */
		0x00000000,	/* DWORD   TimeDateStamp; */
		0x00000000,	/* DWORD   PointerToSymbolTable; // Updated! */
		0x00000000,	/* DWORD   NumberOfSymbols; // Updated! */
		0x00E0,		/* WORD    SizeOfOptionalHeader;  // Fixed on PE32 */
		0x0000,		/* WORD    Characteristics; // Updated! */
	},
	{
		/* IMAGE_OPTIONAL_HEADER OptionalHeader */
		0x010B,		/*WORD    Magic; // Fixed */
		0x06,		/*BYTE    MajorLinkerVersion; // Updated! */
		0x00,		/*BYTE    MinorLinkerVersion; // Updated! */
		0x00000000,	/*DWORD   SizeOfCode; */
		0x00000000,	/*DWORD   SizeOfInitializedData; */
		0x00000000,	/*DWORD   SizeOfUninitializedData; */
		0x00000000,	/*DWORD   AddressOfEntryPoint; */
		0x00000000,	/*DWORD   BaseOfCode; */
		0x00000000,	/*DWORD   BaseOfData; */
		
		0x00400000,	/*DWORD   ImageBase; */
		0x00001000,	/*DWORD   SectionAlignment; */
		0x00000200,	/*DWORD   FileAlignment; */
		0x0004,		/*WORD    MajorOperatingSystemVersion; */
		0x0000,		/*WORD    MinorOperatingSystemVersion; */
		0x0000,		/*WORD    MajorImageVersion; */
		0x0000,		/*WORD    MinorImageVersion; */
		0x0004,		/*WORD    MajorSubsystemVersion; */
		0x0000,		/*WORD    MinorSubsystemVersion; */
		0x00000000,	/*DWORD   Win32VersionValue; */
		0x00000000,	/*DWORD   SizeOfImage; */
		0x00000200,	/*DWORD   SizeOfHeaders; */
		0x00000000,	/*DWORD   CheckSum; */
		0x0003,		/*WORD    Subsystem; */
		0x0000,		/*WORD    DllCharacteristics; */
		0x00100000,	/*DWORD   SizeOfStackReserve; */
		0x00001000,	/*DWORD   SizeOfStackCommit; */
		0x00100000,	/*DWORD   SizeOfHeapReserve; */
		0x00001000,	/*DWORD   SizeOfHeapCommit; */
		0x00000000,	/*DWORD   LoaderFlags; */
		0x00000010,	/*DWORD   NumberOfRvaAndSizes; */
				
		/* IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES]; */
		{0},
	},
};

__END_DECLS