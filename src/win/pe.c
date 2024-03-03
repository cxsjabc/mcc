#include <stdio.h>
#include <windows.h>

#include "mcc/mcc_base.h"

#include "mcc/exe.h"
#include "mcc/log.h"

__BEGIN_DECLS

// This comes from any .exe file!
IMAGE_DOS_HEADER golden_dos_header = { // Fixed value!
	.e_magic = 0x5A4D,         /* WORD   e_magic;    Magic number */
	// ..... ..... ..... ..... ..... ..... ..... ..... (Don't care!)
	.e_lfanew = 0x00000080,    /* LONG   e_lfanew;   File address of new exe header */
};

// Actually, can clear dos stub to zeros, also works normally on Windows i386.
BYTE golden_dos_stub[0x40] = { 0x0 }; // Don't care, use fixed values!

// The header may be changed!
IMAGE_NT_HEADERS32 golden_nt_header = {
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
		0x010B,		/*WORD    Magic; // PE32 */
		0x00,		/*BYTE    MajorLinkerVersion; // Updated? */
		0x00,		/*BYTE    MinorLinkerVersion; // Updated? */
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
		{ { 0 } },
	},
};

void __need_init pe_verify_header()
{
	pe_verify_dos_header();
	pe_verify_nt_header();
}

void __need_init pe_verify_dos_header()
{
	if (golden_dos_header.e_magic != 0x5A4D)
		fatal("DOS header: magic != 0x%x\n", 0x5A4D);
	if (golden_dos_header.e_lfanew != 0x00000080)
		fatal("DOS header: new exe header != 0x%x\n", 0x00000080);
}

void __need_init pe_verify_nt_header()
{
	if (golden_nt_header.Signature != 0x4550)
		fatal("NT header: signature != 0x%x\n", 0x4550);
	if (golden_nt_header.OptionalHeader.Magic != 0x010B)
		fatal("NT header: machine != 0x%x\n", 0x010B);
}

__END_DECLS