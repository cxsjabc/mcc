#ifndef LOG_H
#define LOG_H

#include <stdlib.h>

#ifndef NO_DEBUG
#define NO_DEBUG 0
#endif

#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO  1
#define LOG_LEVEL_WARN  2
#define LOG_LEVEL_ERROR 3
#define LOG_LEVEL_FATAL 4

#define log_print printf

#define LOG_DEBUG(...)  do { if (!NO_DEBUG && LOG_LEVEL <= LOG_LEVEL_DEBUG)  log_print(__VA_ARGS__); } while(0)
#define LOG_INFO(...)   do { if (LOG_LEVEL <= LOG_LEVEL_INFO)  log_print(__VA_ARGS__); } while(0)
#define LOG_WARN(...)   do { if (LOG_LEVEL <= LOG_LEVEL_WARN)  log_print(__VA_ARGS__); } while(0)
#define LOG_ERROR(...)  do { if (LOG_LEVEL <= LOG_LEVEL_ERROR)  log_print(__VA_ARGS__); } while(0)
#define LOG_FATAL(...)  do { if (LOG_LEVEL <= LOG_LEVEL_FATAL)  log_print(__VA_ARGS__); abort(); } while(0)

#define LOG_LEVEL   LOG_LEVEL_DEBUG

#define debug LOG_DEBUG
#define info  LOG_INFO
#define warn  LOG_WARN
#define err   LOG_ERROR
#define fatal LOG_FATAL

#define error err
#define always log_print

#endif