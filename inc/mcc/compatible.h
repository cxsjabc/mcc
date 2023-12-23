#ifndef COMPATIBLE_H
#define COMPATIBLE_H

__BEGIN_DECLS

#ifdef PTHREAD_WIN32
#define PTHREAD_T_TO_LONG(pthread) ((unsigned long)(intptr_t)((pthread).p))
#else
#define PTHREAD_T_TO_LONG(pthread) ((unsigned long)(pthread))
#endif

#undef __WEAK
#ifdef _MSC_VER
#define __WEAK
#else
#define __WEAK 	__attribute__((weak))
#endif

// Because BOOL is often defined by Windows or some 3rd party headers,
// use LOGIC to avoid conflict.
typedef unsigned char LOGIC;

__END_DECLS

#endif