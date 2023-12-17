#include <stdio.h>

#include <windows.h>

#include "mcc/log.h"

__BEGIN_DECLS

// TODO: no effect?
int prepare()
{
	BOOL r = SetDllDirectory(TEXT("./external/pthreads-w32-2-9-1/dll/x64"));
	if (!r) {
		fatal("SetDllDirectory failed: %s(%d)", TEXT("./external/pthreads-w32-2-9-1/dll/x64"), GetLastError());
	}
	debug("Prepare(%d)...\n", r);
	return 0;
}

__END_DECLS