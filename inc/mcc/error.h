#ifndef ERROR_H
#define ERROR_H

#include "mcc/log.h"

enum 
{
    ERR_NONE = 1,
    ERR_FAIL = -1,
    ERR_NO_MEM = -2,
    ERR_UNKNOWN = -3,
};

#define ERR_RETURN(r) { LH; return (r); }
#define OK_RETURN(r) { LH; return (r); }

#endif