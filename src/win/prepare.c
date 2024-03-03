#include <stdio.h>

#include <windows.h>

#include "mcc/exe.h"
#include "mcc/log.h"

__BEGIN_DECLS

// TODO: no effect?
int pre_dll()
{
	BOOL r = SetDllDirectory(TEXT("./external/pthreads-w32-2-9-1/dll/x64"));
	if (!r) {
		fatal("SetDllDirectory failed: %s(%d)", TEXT("./external/pthreads-w32-2-9-1/dll/x64"), GetLastError());
	}
	return 0;
}

int os_prepare()
{
	int r = 0;

	(void)r;

	pe_verify_header();
	
	pre_dll();

	silence("Prepare(%d)...\n", r);
	return 0;
}

__END_DECLS