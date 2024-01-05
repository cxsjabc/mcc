# This makefile is purely for Windows msvc programmers.
# This file is used by nmake.bat, nmake.bat will call nmake.exe if possible.

# Please execute make in msvc command line environment if use MSVC.

# Usage:
#  nmake.exe -f Makefile.msvc.mk
#  nmake.exe -f Makefile.msvc.mk test
#
#  nmake.exe -f Makefile.msvc.mk clean
#  nmake.exe -f Makefile.msvc.mk clean_test

CUR_DIR = .

USE_MSVC = 1

WIN_GEN_SRC_FILES = scripts\windows\tmp_src_files.txt
WIN_GEN_SRC_DEPS = out\src\src.deps.win
WIN_GEN_TEST_DEPS = out\test\test.deps.win

!IF EXISTS($(WIN_GEN_SRC_DEPS))
!INCLUDE $(WIN_GEN_SRC_DEPS)
!ENDIF

!IF EXISTS($(WIN_GEN_TEST_DEPS))
!INCLUDE $(WIN_GEN_TEST_DEPS)
!ENDIF

!ifdef COMSPEC
!message "nmake is: $(MAKE)"
!INCLUDE $(WIN_GEN_SRC_FILES)
!else
!message "make: $(MAKE)"
!error "No msvc environment!"
!endif

!message "MSVC_ARCH: $(MSVC_ARCH)"

C_INCLUDES = /I $(CUR_DIR)/inc /I $(CUR_DIR)/external/pthreads-w32-2-9-1/include
C_INCLUDES = $(C_INCLUDES) /I $(CUR_DIR)/external/inc

C_DEFINES = /DPTHREAD_WIN32

C_LIBFLAG = /LIBPATH:$(CUR_DIR)/external/pthreads-w32-2-9-1/lib/$(MSVC_ARCH) pthreadVC2.lib

C_FLAGS = $(C_INCLUDES) $(C_DEFINES)
C_LINKFLAGS = $(C_LIBFLAG)

SRC_DIR = src

# MSVC
!ifdef USE_MSVC
# 0. cl.exe
CC = cl.exe
LINK = link.exe
AWK = awk.exe

MAIN_OBJ = main.obj
OUT_FILE = m.exe

#SRCS = src\type.c
#OBJS = src\type.obj src\args.obj

BUILD_OBJ_DIR = out

# /showIncludes
{src\}.c{$(BUILD_OBJ_DIR)\src\}.obj:
	@ echo "compile $< --> $@ (rule 1)"
	$(CC) $(C_FLAGS) /c $< /Fo:$@

{src\win\}.c{$(BUILD_OBJ_DIR)\src\win\}.obj:
	@ echo "compile $< --> $@ (rule 1)"
	$(CC) $(C_FLAGS) /c $< /Fo:$@

{test\}.c{$(BUILD_OBJ_DIR)\test\}.obj:
	@ echo "compile $< --> $@ (rule 1.1)"
	$(CC) $(C_FLAGS) /c $< /Fo:$@

.c{$(BUILD_OBJ_DIR)\}.obj:
	@ echo "compile $< --> $@ (rule 2)"
	$(CC) $(C_FLAGS) /c $< /Fo:$@

prepare:
	-@ mkdir $(BUILD_OBJ_DIR) >nul 2>nul
	-@ mkdir $(BUILD_OBJ_DIR)\src $(BUILD_OBJ_DIR)\src\win >nul 2>nul

all: prepare $(LINK_OBJS) $(BUILD_OBJ_DIR)\$(MAIN_OBJ) inc\mcc\env.h
	$(LINK) /out:$(OUT_FILE) $(LINK_OBJS) $(BUILD_OBJ_DIR)\$(MAIN_OBJ) $(C_LINKFLAGS)

clean:
	-@ del $(LINK_OBJS) $(BUILD_OBJ_DIR)\$(MAIN_OBJ) $(OUT_FILE) >nul 2>nul
	-@ rmdir $(BUILD_OBJ_DIR)\src >nul 2>nul
	-@ rmdir $(BUILD_OBJ_DIR) >nul 2>nul
!endif

mcc: all

# test
TEST_DIR = test

!ifdef USE_MSVC
# MSVC
TEST_SRCS = test\test.c
#TEST_OBJS = test\test.obj #$(patsubst %.c,%.obj, $(TEST_SRCS))
TEST_OUT = t.exe

prepare_test:
	-@ mkdir $(BUILD_OBJ_DIR) >nul 2>nul
	-@ mkdir $(BUILD_OBJ_DIR)\src >nul 2>nul
	-@ mkdir $(BUILD_OBJ_DIR)\src\win
	-@ mkdir $(BUILD_OBJ_DIR)\test >nul 2>nul

test: prepare_test $(LINK_OBJS) $(LINK_TEST_OBJS)
	$(LINK) /out:$(TEST_OUT) $(LINK_TEST_OBJS) $(LINK_OBJS) $(C_LINKFLAGS)

clean_test:
	-@ del /F /Q $(TEST_OUT) $(LINK_TEST_OBJS) $(LINK_OBJS) >nul 2>nul
	-@ del /S /Q $(BUILD_OBJ_DIR)
	-@ rmdir $(BUILD_OBJ_DIR)\test >nul 2>nul
	-@ rmdir $(BUILD_OBJ_DIR)\src >nul 2>nul
	-@ rmdir $(BUILD_OBJ_DIR) >nul 2>nul

!endif
