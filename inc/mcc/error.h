#ifndef ERROR_H
#define ERROR_H

#include "mcc/features.h"
#include "mcc/log.h"

enum 
{
    ERR_NONE = 1,
    ERR_FAIL = -1,
    ERR_NO_MEM = -2,
    ERR_UNKNOWN = -3,
};

#define ERR_RETURN(r) { LOG_HERE_ARGS("Error\n"); return (r); }
#define OK_RETURN(r) { return (r); }

#define NULL_RETURN(r, err_str) \
{ \
    if (!(r)) { \
        LOG_HERE_ARGS(err_str); \
        return NULL; \
    } \
}

#define NULL_ABORT(r, abort_str) \
{ \
    if (!(r)) { \
        LOG_HERE_ARGS(abort_str); \
        abort(); \
    } \
}

#endif