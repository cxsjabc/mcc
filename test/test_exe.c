#define NO_DEBUG 1

#include <assert.h>
#include <stdio.h>

#if defined(_WINDOWS) || defined(_MSC_VER)
#include <windows.h>
#endif

#include "mcc/exe.h"
#include "mcc/log.h"
#include "mcc/mem.h"

#include "test.h"

// Debug used for some environment, i.e. Windows, but the IDE doesn't think _WINDOWS is defined, caused that 
// some defines are not highlighted!
// #define _WINDOWS

__BEGIN_DECLS

#if defined(_WINDOWS) || defined(_MSC_VER)
void test_pe()
{
	IMAGE_DOS_HEADER *dos_hdr = &golden_dos_header;

	TEST_BEGIN;
	mem_dump(dos_hdr, sizeof(*dos_hdr), 1, 16);

	mem_dump(dos_hdr, sizeof(*dos_hdr), 2, 8);

	mem_dump(dos_hdr, sizeof(*dos_hdr), 4, 4);

	mem_dump(dos_hdr, sizeof(*dos_hdr), 8, 2);
	TEST_END;
}
#endif

void test_exe()
{
#if defined(_WINDOWS) || defined(_MSC_VER)
	test_pe();
#endif
}

__END_DECLS