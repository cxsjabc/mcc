#include <stdio.h>

#include "mcc/ver.h"

int main()
{
	printf("Mcc init(ver: %s)\n", VERSION);
	printf("Build version: %s\n", gen_build_ver());
	return 0;
}