#include <stdio.h>

#include "mcc/ver.h"
#include "mcc/time.h"
#include "mcc/string.h"

const char *gen_ver()
{
    const char *build_date = BUILD_VER_DATE; // like: "Nov 25 2023"
    const char *build_time = BUILD_VER_TIME; // like: "17:10:36"

    static char ver[128];
    int year, mon, day;
    int len;
    char *pstart;
    char build_date_copy[32], build_time_copy[32], year_str[16];

    strcpy(ver, VERSION);

    build_date_copy[0] = '\0';
    build_time_copy[0] = '\0';
    year_str[0] = '\0';
    strcpy(build_date_copy, build_date);
    strcpy(build_time_copy, build_time);

    len = get_string_until_char(build_date_copy, &pstart, ' ');
    if (len <= 0) {
        fprintf(stderr, "Failed to get build month string.\n");
        return NULL;
    }
    strncpy(year_str, pstart, len);
    year_str[len] = '\0';
    mon = trans_month_to_int(year_str);
    if (mon < 1) {
        fprintf(stderr, "Failed to translate month.\n");
        return NULL;
    }

    return ver;
}