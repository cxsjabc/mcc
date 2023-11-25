#include <string.h>

#include "mcc/time.h"

static const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

int trans_month_to_int(const char *mon)
{
    int i;

    for(i = 0; i < sizeof(months) / sizeof(char *); ++i)
    {
        if (strcmp(months[i], mon) == 0)
            return i + 1;
    }
    return -1;
}