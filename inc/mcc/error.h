#ifndef ERROR_H
#define ERROR_H

#include "mcc/mcc_base.h"
#include "mcc/log.h"

enum 
{
	OK = 1,
	ERR_NONE = OK,
	ERR_FAIL = -1,
	ERR_NO_MEM = -2,
	ERR_UNKNOWN = -3,
	ERR_FORK = -4,
	ERR_INVALID_FORMAT = -5,
	ERR_INVALID_PARAM = -6,
	ERR_EMPTY_PARAM = -7,
	ERR_EXIST = -8,
};

// Add preserved right return values are no more than 0xFF.
#define ERR_MASK 0xFF

#define TRUE 1
#define FALSE 0

#define ERR_RETURN(r) { LOG_HERE_ARGS("Error\n"); return (r); }
#define OK_RETURN(r) { return (r); }

#define NULL_RETURN(r, ret, err_str, ...) \
{ \
	if (!(r)) { \
		LOG_HERE_ARGS(err_str, ##__VA_ARGS__); \
		return (ret); \
	} \
}

#define NULL_ABORT(r, abort_str) \
{ \
	if (!(r)) { \
		LOG_HERE_ARGS(abort_str); \
		abort(); \
	} \
}

#define COND_GOTO(cond, label, info_str, ...) \
{ \
	if (cond) { \
		LOG_HERE_ARGS(info_str, ##__VA_ARGS__); \
		goto label; \
	} \
}

#endif