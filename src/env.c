#define NO_DEBUG 1
#define NO_LOG_FILE_LINE 1

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "mcc/string.h"
#include "mcc/error.h"
#include "mcc/log.h"
#include "mcc/size.h"
#include "mcc/env.h"

/**
 * GCC '-march=' switch are:
 *	  nocona core2 nehalem corei7 westmere sandybridge corei7-avx ivybridge core-avx-i
 *	  haswell core-avx2 broadwell skylake skylake-avx512 cannonlake icelake-client icelake-server
 *	  cascadelake tigerlake cooperlake bonnell atom silvermont slm goldmont goldmont-plus tremont
 *	  knl knm x86-64 eden-x2 nano nano-1000 nano-2000 nano-3000 nano-x2 eden-x4 nano-x4 k8 k8-sse3
 *	  opteron opteron-sse3 athlon64 athlon64-sse3 athlon-fx amdfam10 barcelona bdver1 bdver2 bdver3
 *	  bdver4 znver1 znver2 znver3 btver1 btver2 native
 * 
 */
/*
 * Windows GCC 64bit (GCC 10.2.0 -march=x86-64 or x86_64-w64-mingw32-gcc 10.2.0)
 *   __GNUC__ is defined
 *   _WIN32 is defined.
 *   _WIN64 is defined.
 *   __x86_64__ is defined.
 *   __amd64__ is defined.
 *   _M_AMD64 is defined.
 *   _M_X64 is defined.
 *   __SIZEOF_POINTER__: 8
 *   SIZE_MAX: ffffffffffffffff
 *
 * Windows GCC 32bit (i686-w64-mingw32-gcc 10.2.0)
 *   __GNUC__ is defined
 *   _WIN32 is defined.
 *   __i386__ is defined.
 *   __SIZEOF_POINTER__: 4
 *   SIZE_MAX: ffffffff
 * 
 * Windows Clang (8.0.1 x86_64-unknown-windows-cygnus)
 *   __GNUC__ is defined
 *   unix is defined.
 *   -- _WIN32 and _WIN64 are both not defined.
 *   __x86_64__ is defined.
 *   __amd64__ is defined.
 *   __clang_version__ is: 8.0.1 (tags/RELEASE_801/final)
 *   __clang_major__ is: 8
 *   __WORDSIZE: 64
 *   __SIZEOF_POINTER__: 8
 *   __LP64__ is defined.
 * 
 * Windows clang (Swift toolchains 16.0.0 x86_64-unknown-windows-msvc)
 *   _MSC_VER is defined.
 *   _WIN32 is defined.
 *   _WIN64 is defined.
 *   __x86_64__ is defined.
 *   __amd64__ is defined.
 *   _M_AMD64 is defined.
 *   _M_X64 is defined.
 *   __clang_version__ is: 16.0.0
 *   __clang_major__ is: 16
 *   __SIZEOF_POINTER__: 8
 *   SIZE_MAX: ffffffffffffffff
 *
 * Windows MSVC cl 32bit (VS2019 19.29.30148)
 *   _MSC_VER is defined.
 *   _WIN32 is defined.
 *   SIZE_MAX: ffffffff
 * 
 * Windows MSVC cl 64bit (VS2019 19.29.30148)
 *   _MSC_VER is defined.
 *   _WIN32 is defined.
 *   _WIN64 is defined.
 *   _M_AMD64 is defined.
 *   _M_X64 is defined.
 *   SIZE_MAX: ffffffffffffffff
 *
 * Ubuntu 22.04 (WSL 1.0 or 2.0) (GCC 11.4.0)
 *   __GNUC__ is defined
 *   linux is defined.
 *   unix is defined.
 *   -- _WIN32 and _WIN64 are both not defined.
 *   __x86_64__ is defined.
 *   __amd64__ is defined.
 *   __WORDSIZE: 64
 *   __SIZEOF_POINTER__: 8
 *   __LP64__ is defined.
 * 
 * Ubuntu 22.04 (WSL 2.O) (GCC 11.4.0 -m32)
 *   __GNUC__ is defined
 *   linux is defined.
 *   unix is defined.
 *   -- _WIN32 and _WIN64 are both not defined.
 *   __i386__ is defined.
 *   __WORDSIZE: 32
 *   __SIZEOF_POINTER__: 4
 * 
 * macOS 10.13.6 (Apple LLVM 10.0.0 clang-1000.10.44.4)(VM)
 *   __GNUC__ is defined
 *   -- _WIN32 and _WIN64 are both not defined.
 *   __x86_64__ is defined.
 *   __amd64__ is defined.
 *   __WORDSIZE: 64
 *   __SIZEOF_POINTER__: 8
 *   SIZE_MAX: ffffffffffffffff
 *   __LP64__ is defined.
 */

__BEGIN_DECLS

const char *get_clang_clangpp()
{
#ifdef __cplusplus
	return "clang++";
#else
	return "clang";
#endif
}

const char *get_gcc_gpp()
{
#ifdef __cplusplus
	return "g++";
#else
	return "gcc";
#endif
}

void check_compiler()
{
#ifdef __clang__
	info_nofl("%s version: %s\n", get_clang_clangpp(), __clang_version__);
#elif defined(__GNUC__)
	info_nofl("%s version: %d\n", get_gcc_gpp(), __GNUC__);
#else
	info_nofl("not clang or gcc, what compiler?\n")
#endif	
}

int check_build_envionment()
{
// OS: Windows or Linux ...
#if defined(_MSC_VER)
	info_nofl("OS: Windows, msvc: %d\n", _MSC_VER);
#elif defined(_WINDOWS) // passed from Makefile $(0S)
	info_nofl("OS: Windows, ");
	check_compiler();
#elif defined(__linux__)
	info("OS: Linux, ");
	check_compiler();
#elif defined(__APPLE__)
	info("OS: macOS, ");
	check_compiler();
#elif defined(__unix__)
	info("OS: Unix\n");
#else
	info("OS: Unknown\n");
#endif

#if defined(__GNUC__)
	debug("__GNUC__ is defined\n");
#endif

#if __GNUC__
	debug("__GNUC__: %d\n", __GNUC__);
#endif

#ifdef __clang__
	debug("__clang__ is defined\n");
#endif

#ifdef _MSC_VER
	debug("_MSC_VER is defined.\n");
#endif

#ifdef linux
	debug("linux is defined.\n");
#endif

#ifdef unix
	debug("unix is defined.\n");
#endif

// System 32bit or 64bit
#ifdef _WIN32
	debug("_WIN32 is defined.\n");
#endif

#ifdef _WIN64
	debug("_WIN64 is defined.\n");
#endif

#if !defined(_WIN32) && !defined(_WIN64)
	debug("-- _WIN32 and _WIN64 are both not defined.\n");
#endif

#ifdef __i386__
	debug("__i386__ is defined.\n");
#endif

#ifdef __x86_64__
	debug("__x86_64__ is defined.\n");
#endif

#ifdef __amd64__
	debug("__amd64__ is defined.\n");
#endif

#ifdef _M_AMD64
	debug("_M_AMD64 is defined.\n");
#endif

#ifdef _M_X64
	debug("_M_X64 is defined.\n");
#endif

// Compiler version
#ifdef __clang_version__
	debug("__clang_version__ is: %s\n", __clang_version__);
#endif

#ifdef __clang_major__
	debug("__clang_major__ is: %d\n", __clang_major__);
#endif

// Type Size
#ifdef __WORDSIZE
	// Win GCC(not found)
	debug("__WORDSIZE: %d\n", __WORDSIZE);
#endif

#ifdef __SIZEOF_POINTER__
	debug("__SIZEOF_POINTER__: %d\n", __SIZEOF_POINTER__);
#endif

#ifdef SIZE_MAX
	debug("SIZE_MAX: %llx\n", (unsigned long long)SIZE_MAX);
#endif

#ifdef __LP64__
	debug("__LP64__ is defined.\n");
#endif

	return 1;
}

__END_DECLS