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

!ifdef COMSPEC
!message "nmake is: $(MAKE)"
!INCLUDE $(WIN_GEN_SRC_FILES)
!else
!message "make: $(MAKE)"
!error "No msvc environment!"
!endif

C_INCLUDES = -I $(CUR_DIR)/inc
C_FLAGS = $(C_INCLUDES)

SRC_DIR = src

# MSVC
!ifdef USE_MSVC
# 0. cl.exe
CC = cl.exe
LINK = link.exe

MAIN_OBJ = main.obj
OUT_FILE = mcc.exe

#SRCS = src\type.c
#OBJS = src\type.obj src\args.obj

BUILD_OBJ_DIR = out

{src\}.c{$(BUILD_OBJ_DIR)\src\}.obj:
	@ echo "compile $< --> $@ (rule 1)"
	echo $@: > $@.d
	cl.exe -I ./inc /c /showIncludes $< /nologo /Fo:NUL | findstr "inc\mcc" | awk '{print $$3}' >> $@.d
	$(CC) $(C_FLAGS) /c /showIncludes $< /Fo:$@

{test\}.c{$(BUILD_OBJ_DIR)\test\}.obj:
	@ echo "compile $< --> $@ (rule 1.1)"
	$(CC) $(C_FLAGS) /c $< /Fo:$@

.c{$(BUILD_OBJ_DIR)\}.obj:
	@ echo "compile $< --> $@ (rule 2)"
	$(CC) $(C_FLAGS) /c $< /Fo:$@

prepare:
	-@ mkdir $(BUILD_OBJ_DIR) >nul 2>nul
	-@ mkdir $(BUILD_OBJ_DIR)\src >nul 2>nul

all: prepare $(LINK_OBJS) $(BUILD_OBJ_DIR)\$(MAIN_OBJ)
	$(LINK) /out:$(OUT_FILE) $(LINK_OBJS) $(BUILD_OBJ_DIR)\$(MAIN_OBJ)

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
TEST_OUT = mcc_test.exe

prepare_test:
	-@ mkdir $(BUILD_OBJ_DIR) >nul 2>nul
	-@ mkdir $(BUILD_OBJ_DIR)\src >nul 2>nul
	-@ mkdir $(BUILD_OBJ_DIR)\test >nul 2>nul

test: prepare_test $(LINK_OBJS) $(LINK_TEST_OBJS)
	$(LINK) /out:$(TEST_OUT) $(LINK_TEST_OBJS) $(LINK_OBJS)

clean_test:
	-@ del /F /Q $(TEST_OUT) $(LINK_TEST_OBJS) $(LINK_OBJS) >nul 2>nul
	-@ del /S /Q $(BUILD_OBJ_DIR)
	-@ rmdir $(BUILD_OBJ_DIR)\test >nul 2>nul
	-@ rmdir $(BUILD_OBJ_DIR)\src >nul 2>nul
	-@ rmdir $(BUILD_OBJ_DIR) >nul 2>nul

!endif
