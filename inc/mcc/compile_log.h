#ifndef COMPILE_LOG_H
#define COMPILE_LOG_H

#include <stdlib.h>

#include "mcc/mcc_base.h"
#include "mcc/log.h"

__BEGIN_DECLS

#ifndef NO_CLOG_FILE_LINE
#define NO_CLOG_FILE_LINE 1
#endif

#define CLOG_INFO(...)   do { if (CLOG_LEVEL <= LOG_LEVEL_INFO)  { if (!NO_CLOG_FILE_LINE) LOG_FILE_LINE; log_print(__VA_ARGS__); } } while(0)
#define CLOG_WARN(...)   do { if (CLOG_LEVEL <= LOG_LEVEL_WARN)  { if (!NO_CLOG_FILE_LINE) LOG_FILE_LINE; log_print(__VA_ARGS__); } } while(0)
#define CLOG_ERROR(...)  do { if (CLOG_LEVEL <= LOG_LEVEL_ERROR) { if (!NO_CLOG_FILE_LINE) LOG_FILE_LINE; log_print(__VA_ARGS__); } } while(0)
#define CLOG_FATAL(...)  do { if (CLOG_LEVEL <= LOG_LEVEL_FATAL) { if (!NO_CLOG_FILE_LINE) LOG_FILE_LINE; log_print(__VA_ARGS__); abort(); } } while(0)

#define CLOG_LEVEL LOG_LEVEL_INFO

#define cdebug CLOG_DEBUG
#define cinfo  CLOG_INFO
#define cwarn  CLOG_WARN
#define cerr   CLOG_ERROR
#define cfatal CLOG_FATAL

#define cerror cerr

// verbose log
// It's used for '-v' option verbose logging
#define CLOG_VERBOSE(...)   do { if (is_verbose_enabled)  { if (!NO_CLOG_FILE_LINE) LOG_FILE_LINE; log_print(__VA_ARGS__); } } while(0)
#define verbose CLOG_VERBOSE

extern int is_verbose_enabled;
void set_verbose_enabled(int enabled);

__END_DECLS

#endif