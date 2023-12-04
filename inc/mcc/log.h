#ifndef LOG_H
#define LOG_H

#include <stdlib.h>

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

#define LH LOG_HERE
#define LHD LOG_HERE_DEBUG

#define LOGD(val)  do { if (!NO_LOG_FILE_LINE) LOG_FILE_LINE; log_print(#val "%d\n", (val)); } while(0)

#endif