#ifndef LOG_H
#define LOG_H

#include <stdlib.h>

#include "mcc/mcc_base.h"

#ifndef NO_DEBUG
#define NO_DEBUG 0
#endif

#ifndef NO_LOG_FILE_LINE
#define NO_LOG_FILE_LINE 0
#endif

#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO  1
#define LOG_LEVEL_WARN  2
#define LOG_LEVEL_ERROR 3
#define LOG_LEVEL_FATAL 4

#define log_print printf

#define LOG_FILE_LINE log_print("[%s:%d]", __FILE__, __LINE__)

#define LOG_HERE  do { log_print("[%s:%d]\n", __FILE__, __LINE__); } while(0)
#define LOG_HERE_ARGS(fmt, ...) do { log_print("[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__); } while(0)

#define LOG_DEBUG(...)  do { if (!NO_DEBUG && LOG_LEVEL <= LOG_LEVEL_DEBUG)  { if (!NO_LOG_FILE_LINE) LOG_FILE_LINE; log_print(__VA_ARGS__); } } while(0)
#define LOG_HERE_DEBUG  do { if (!NO_DEBUG && LOG_LEVEL <= LOG_LEVEL_DEBUG) log_print("[%s:%d]\n", __FILE__, __LINE__); } while(0)
#define LOG_INFO(...)   do { if (LOG_LEVEL <= LOG_LEVEL_INFO)  { if (!NO_LOG_FILE_LINE) LOG_FILE_LINE; log_print(__VA_ARGS__); } } while(0)
#define LOG_WARN(...)   do { if (LOG_LEVEL <= LOG_LEVEL_WARN)  { if (!NO_LOG_FILE_LINE) LOG_FILE_LINE; log_print(__VA_ARGS__); } } while(0)
#define LOG_ERROR(...)  do { if (LOG_LEVEL <= LOG_LEVEL_ERROR) { if (!NO_LOG_FILE_LINE) LOG_FILE_LINE; log_print(__VA_ARGS__); } } while(0)
#define LOG_FATAL(...)  do { if (LOG_LEVEL <= LOG_LEVEL_FATAL) { if (!NO_LOG_FILE_LINE) LOG_FILE_LINE; log_print(__VA_ARGS__); abort(); } } while(0)

#define LOG_LEVEL   LOG_LEVEL_DEBUG

#define debug LOG_DEBUG
#define info  LOG_INFO
#define warn  LOG_WARN
#define err   LOG_ERROR
#define fatal LOG_FATAL

#define error err
#define always log_print

#define LOG_DEBUG_NO_FILE_LINE(...)  do { if (!NO_DEBUG && LOG_LEVEL <= LOG_LEVEL_DEBUG)  { log_print(__VA_ARGS__); } } while(0)
#define LOG_INFO_NO_FILE_LINE(...)   do { if (LOG_LEVEL <= LOG_LEVEL_INFO)  { log_print(__VA_ARGS__); } } while(0)
#define LOG_WARN_NO_FILE_LINE(...)   do { if (LOG_LEVEL <= LOG_LEVEL_WARN)  { log_print(__VA_ARGS__); } } while(0)
#define LOG_ERROR_NO_FILE_LINE(...)  do { if (LOG_LEVEL <= LOG_LEVEL_ERROR) { log_print(__VA_ARGS__); } } while(0)
#define LOG_FATAL_NO_FILE_LINE(...)  do { if (LOG_LEVEL <= LOG_LEVEL_FATAL) { log_print(__VA_ARGS__); abort(); } } while(0)

#define debug_nofl LOG_DEBUG_NO_FILE_LINE
#define info_nofl  LOG_INFO_NO_FILE_LINE
#define warn_nofl  LOG_WARN_NO_FILE_LINE
#define err_nofl   LOG_ERROR_NO_FILE_LINE
#define fatal_nofl LOG_FATAL_NO_FILE_LINE

#define error_nofl err_nofl

#define LH LOG_HERE
#define LHD LOG_HERE_DEBUG

#define LOGD(val)  do { if (!NO_LOG_FILE_LINE) LOG_FILE_LINE; log_print(#val "%d\n", (val)); } while(0)

#endif