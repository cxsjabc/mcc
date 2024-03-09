#include <stdio.h>
#include <windows.h>

#include "mcc/mcc_base.h"

#include "mcc/align.h"
#include "mcc/error.h"
#include "mcc/exe.h"
#include "mcc/log.h"

__BEGIN_DECLS

// This comes from any .exe file!
IMAGE_DOS_HEADER golden_dos_header = { // Fixed value!
	.e_magic = 0x5A4D,         /* WORD   e_magic;    ** Magic number */
	// ..... ..... ..... ..... ..... ..... ..... ..... (Don't care!)
	.e_lfanew = 0x00000080,    /* LONG   e_lfanew;   ** File address of new exe header */
};

// Actually, can clear dos stub to zeros, also works normally on Windows XP+ 32bit.
BYTE golden_dos_stub[0x40] = { 0x0 }; // Don't care, use fixed zero values!

// The header may be changed!
IMAGE_NT_HEADERS32 golden_nt_header = {
	0x00004550, /* DWORD Signature ** */
	{
		/* IMAGE_FILE_HEADER FileHeader */
		0x0000,		/* WORD    Machine; ** Updated! */
		0x0000,		/* WORD    NumberOfSections; ** Updated! */
		0x00000000,	/* DWORD   TimeDateStamp; ? */
		0x00000000,	/* DWORD   PointerToSymbolTable; ? */
		0x00000000,	/* DWORD   NumberOfSymbols;  ? */
		0x00E0,		/* WORD    SizeOfOptionalHeader;  **  Fixed on PE32 */
		0x0000,		/* WORD    Characteristics; ** Updated! */
	},
	{
		/* IMAGE_OPTIONAL_HEADER OptionalHeader */
		0x010B,		/*WORD    Magic; ** PE32 +0x18H */
		0x00,		/*BYTE    MajorLinkerVersion; ? */
		0x00,		/*BYTE    MinorLinkerVersion; ? */
		0x00000000,	/*DWORD   SizeOfCode; ** */
		0x00000000,	/*DWORD   SizeOfInitializedData; */
		0x00000000,	/*DWORD   SizeOfUninitializedData; */
		0x00000000,	/*DWORD   AddressOfEntryPoint; +0x28H */
		0x00000000,	/*DWORD   BaseOfCode; ** */
		0x00000000,	/*DWORD   BaseOfData; ** */
		
		0x00400000,	/*DWORD   ImageBase; ** EXE base +0x34H */
		0x00001000,	/*DWORD   SectionAlignment; ** 4KB */
		0x00000200,	/*DWORD   FileAlignment; ** 512B */
		0x0004,		/*WORD    MajorOperatingSystemVersion; Phased out? +0x40H */
		0x0000,		/*WORD    MinorOperatingSystemVersion; Phased out? */
		0x0000,		/*WORD    MajorImageVersion; ? */
		0x0000,		/*WORD    MinorImageVersion; ? */
		// Refer to: https://learn.microsoft.com/en-us/Dotnet/visual-basic/reference/command-line-compiler/subsystemversion
		// Use: 5.1 for Windows XP 32bit compatibility
		0x0005,		/*WORD    MajorSubsystemVersion; ** On Win10, > 06, applicaton crash */
		0x0001,		/*WORD    MinorSubsystemVersion; ** On Win10, > 02, applicaton crash */
		0x00000000,	/*DWORD   Win32VersionValue; ** Must 0 */
		0x00000000,	/*DWORD   SizeOfImage; ** +0x50H */
		0x00000200,	/*DWORD   SizeOfHeaders; ** */
		0x00000000,	/*DWORD   CheckSum; ** */
		0x0003,		/*WORD    Subsystem; ** IMAGE_SUBSYSTEM_WINDOWS_CUI: 3 */
		0x0000,		/*WORD    DllCharacteristics; For exe, ? */
		0x00000000,	/*DWORD   SizeOfStackReserve; ? +0x60H  */
		0x00000000,	/*DWORD   SizeOfStackCommit; ? */
		0x00000000,	/*DWORD   SizeOfHeapReserve; ? */
		0x00000000,	/*DWORD   SizeOfHeapCommit; ? */
		0x00000000,	/*DWORD   LoaderFlags; Phased out */
		0x00000010,	/*DWORD   NumberOfRvaAndSizes; ** */
				
		/* IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES]; ** */
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

unsigned int exe_get_file_align(Exe exe)
{
	return NTHDR_FILE_ALIGN(&golden_nt_header);
}

unsigned int exe_get_header_aligned(Exe exe)
{
	unsigned int hdr_size, r;

	hdr_size = _EXE_FILE_HDR_SIZE(exe) + _EXE_SEC_HDR_SIZE(exe) * exe->sec_cnt;

	r = ALIGN(hdr_size, NTHDR_FILE_ALIGN(&golden_nt_header));
	return r;
}

int _exe_write_file_header(Exe exe)
{
	FILE *f;
	int r = OK;

	f = exe->f;

	fseek(f, 0, SEEK_SET);
	r = fwrite(&golden_dos_header, sizeof(golden_dos_header), 1, f);
	if (r != 1)
		return ERR_FAIL;
	r = fwrite(&golden_dos_stub, sizeof(golden_dos_stub), 1, f);
	if (r != 1)
		return ERR_FAIL;
	r = fwrite(&golden_nt_header, sizeof(golden_nt_header), 1, f);
	if (r != 1)
		return ERR_FAIL;

	return r == 1;
}

__END_DECLS