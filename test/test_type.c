#define NO_DEBUG 1

#include <assert.h>
#include <stdio.h>

#include "mcc/type.h"
#include "mcc/log.h"

__BEGIN_DECLS

void test_type()
{
	int i;
	
	init_types();

	for (i = BOOL; i <= VOID; ++i) {
		const char *type_name;
		PType p;

		debug("i: %d\n", i);
		p = get_type_info(i);
		assert(p);

		type_name = get_type_names(i);
		assert(type_name);
		always("Type: %s --> %d, size: %d, align: %d\n", type_name, i, p->size, p->align);
		assert(p->size != 0 && p->align != 0);
	}

	return;
}

__END_DECLS