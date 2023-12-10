#define NO_DEBUG 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc/ver.h"
#include "mcc/time.h"
#include "mcc/string.h"
#include "mcc/log.h"

__BEGIN_DECLS

const char *gen_build_ver()
{
	const char *build_date = BUILD_VER_DATE; // like: "Nov 25 2023"
	const char *build_time = BUILD_VER_TIME; // like: "17:10:36"

	static char build_ver[128];
	int year, mon, day;
	int len, len1;
	char *pstart;
	char build_date_copy[32], build_time_copy[32];

	build_date_copy[0] = '\0';
	build_time_copy[0] = '\0';
	strcpy(build_date_copy, build_date);
	strcpy(build_time_copy, build_time);

	debug("date: %s, time: %s\n", build_date_copy, build_time_copy);
	// get month string
	len = get_string_until_char(build_date_copy, &pstart, ' ');
	if (len <= 0) {
		fprintf(stderr, "Failed to get build month string.\n");
		return NULL;
	}
	debug("month: %s, len: %d\n", pstart, len);

	pstart[len] = '\0';
	// translate month string to month interger
	mon = trans_month_to_int(pstart);
	if (mon < 1) {
		fprintf(stderr, "Failed to translate month.\n");
		return NULL;
	}

	// get day
	len = get_string_until_char(pstart + len + 1, &pstart, ' ');
	if (len <= 0) {
		fprintf(stderr, "Failed to get build day string, len(%d), p(%p, %c).\n", len, pstart, *pstart);
		return NULL;
	}
	pstart[len] = '\0';
	day = atoi(pstart);

	// get year
	len = get_string_until_char(pstart + len + 1, &pstart, ' ');
	if (len <= 0) {
		fprintf(stderr, "Failed to get build year string.\n");
		return NULL;
	}
	pstart[len] = '\0';
	year = atoi(pstart);

	len = sprintf(build_ver, "%d%d%d", year, mon, day);

	// get timestamp
	len1 = copy_ignore_char(build_ver + len, build_time_copy, 128 - strlen(build_ver) - 1, ':');
	build_ver[len + len1] = '\0';

	return build_ver;
}

__END_DECLS