# This Makefile is also available for Windows if gcc.exe/clang.exe... are supported(in PATH environment).
# Usage:
#  make
#  make test
#
#  make clean
#  make clean_test

CUR_DIR := .

.PHONY : prepare all clean mcc test clean_test mcc_test

# Default use gcc
CC := gcc

# Note 1: can use make CC=g++ to change the default $(CC) compiler.
# Note 2: Sometimes, you installed many compilers and the compilers may be with the same name, you
  #       can use the full path to locate the compiler.
  #       i.e. D:\compiler1\clang.exe

# 0. Use clang (16.0.0)
ifeq ($(CC),clang)
CC := clang
endif

# 1. Use gcc (Ubuntu 11.4.0, Windows 10.2.0)
ifeq ($(CC),gcc)
CC := gcc
endif

# 1.1. Winows 64bit: use Mingw gcc generate 32bit program (10.2.0)
# CC := i686-w64-mingw32-gcc.exe
  # If Windows 32bit, can use mingw32-gcc.exe (This is for 32bit system)

# 1.2. Winows 64bit: use Mingw gcc generate 64bit program (10.2.0)
# CC := x86_64-w64-mingw32-gcc.exe

# 2. Use g++ (Ubuntu 11.4.0)
#      Note: Windows 11.4.0: build successfully and can't execute
ifeq ($(CC),g++)
CC := g++
endif

# 3. clang++ 16.0.0
ifeq ($(CC),clang++)
CC := clang++
#TEST_C_FLAGS := -Wdeprecated
DISABLE_WERROR := 1
endif

# 4. Please don't use Android NDK GCC or CLANG toolchain directly.
#    If use, may need to fix file ending style, CRLF or LF type and other issues.
#    i.e. include path not found and so on.

# header path
C_INCLUDES := -I $(CUR_DIR)/inc

# Extra or test flags: -v: verbose output
# TEST_C_FLAGS += -v

C_FLAGS := $(C_INCLUDES)
C_FLAGS += -Wall -g
ifneq ($(DISABLE_WERROR),1)
C_FLAGS += -Werror
endif
# disable clang secure function warnings on clang-16.0
C_FLAGS += -D_CRT_SECURE_NO_WARNINGS
ifeq ($(OS),Windows_NT)
C_FLAGS += -D_WINDOWS
endif
C_FLAGS += $(TEST_C_FLAGS)

SRC_DIR := src
SRCS := $(wildcard $(SRC_DIR)/*.c)

BUILD_OBJ_DIR := out

# check current and previous compilers
CURR_COMP := $(CC)
PREV_COMP := $(shell cat $(BUILD_OBJ_DIR)/previous_build)
$(info CURR_COMP: $(CURR_COMP), PREV_COMP: $(PREV_COMP))

MAIN_OBJ := main.o
ifeq ($(OS), Windows_NT)
OUT_FILE := m.exe
NULL_FILE := NUL
else
OUT_FILE := m
NULL_FILE := /dev/null
endif

OBJS := $(patsubst %.c, %.o, $(SRCS))
BUILD_OBJS := $(patsubst %.o, $(BUILD_OBJ_DIR)/%.o, $(OBJS))
BUILD_OBJS_DEPENDS := $(patsubst %.o, %.o.d, $(BUILD_OBJS))

TOUCH_VER_UPDATE := 0
VER_FILE := src/ver.c

all: os_check srcs_depend prepare $(BUILD_OBJS) $(BUILD_OBJ_DIR)/$(MAIN_OBJ) $(OUT_FILE)

$(OUT_FILE): $(BUILD_OBJS) $(BUILD_OBJ_DIR)/$(MAIN_OBJ)
	$(CC) -o $(OUT_FILE) $(BUILD_OBJS) $(BUILD_OBJ_DIR)/$(MAIN_OBJ) $(C_FLAGS)
	echo $(CC) > $(BUILD_OBJ_DIR)/previous_build

$(BUILD_OBJ_DIR)/%.o : %.c
	$(CC) $(C_FLAGS) -MMD -MF $@.d -MT $@ -c $< -o $@
	@# update version if compiled other files
	@ if [ "$<" != "$(VER_FILE)" ] && [ $(TOUCH_VER_UPDATE) -eq 0 ]; then touch $(VER_FILE); set TOUCH_VER_UPDATE=1; fi

-include $(BUILD_OBJ_DIR)/$(SRC_DIR)/*.d
-include $(BUILD_OBJ_DIR)/$(MAIN_OBJ).d

clean:
	-@ rm -rf $(BUILD_OBJS) $(BUILD_OBJ_DIR)/$(MAIN_OBJ) $(BUILD_OBJS_DEPENDS) $(BUILD_OBJ_DIR)/$(MAIN_OBJ).d $(OUT_FILE)
	-@ rm -rf $(BUILD_OBJ_DIR)/$(SRC_DIR)
	-@ if [ -d "$(BUILD_OBJ_DIR)" ]; then rmdir --ignore-fail-on-non-empty $(BUILD_OBJ_DIR); fi

os_check:
	@ echo "OS: $(OS)"

srcs_depend:
	-@ if [ "$(CURR_COMP)" != "$(PREV_COMP)" ] && [ -e $(BUILD_OBJ_DIR)/previous_build ]; then rm -rf $(BUILD_OBJ_DIR); fi

prepare:
	-@ if [ ! -d "$(BUILD_OBJ_DIR)" ]; then mkdir $(BUILD_OBJ_DIR); fi
	-@ if [ ! -d "$(BUILD_OBJ_DIR)/$(SRC_DIR)" ]; then mkdir $(BUILD_OBJ_DIR)/$(SRC_DIR); fi

mcc: all

ca: clean mcc

# test
TEST_DIR := test
TEST_SRCS := $(wildcard $(TEST_DIR)/*.c)

TEST_OBJS := $(patsubst %.c,%.o, $(TEST_SRCS))
TEST_BUILD_OBJS := $(patsubst %.o,$(BUILD_OBJ_DIR)/%.o, $(TEST_OBJS))
TEST_BUILD_OBJS_DEPENDS := $(patsubst %.o, %.o.d, $(TEST_BUILD_OBJS))

ifeq ($(OS), Windows_NT)
TEST_OUT := t.exe
else
TEST_OUT := t
endif

prepare_test:
	-@ if [ ! -d "$(BUILD_OBJ_DIR)" ]; then mkdir $(BUILD_OBJ_DIR); fi
	-@ if [ ! -d "$(BUILD_OBJ_DIR)/$(SRC_DIR)" ]; then mkdir $(BUILD_OBJ_DIR)/$(SRC_DIR); fi
	-@ if [ ! -d "$(BUILD_OBJ_DIR)/$(TEST_DIR)" ]; then mkdir $(BUILD_OBJ_DIR)/$(TEST_DIR); fi

test: os_check prepare_test $(BUILD_OBJS) $(TEST_BUILD_OBJS)
	$(CC) -o $(TEST_OUT) $(TEST_BUILD_OBJS) $(BUILD_OBJS) $(C_FLAGS)
	echo $(CC) > $(BUILD_OBJ_DIR)/previous_build

-include $(BUILD_OBJ_DIR)/$(TEST_DIR)/*.d

clean_test:
	-@ rm -rf $(BUILD_OBJS) $(TEST_BUILD_OBJS) $(TEST_BUILD_OBJS_DEPENDS) $(TEST_OUT)
	-@ rm -rf $(BUILD_OBJ_DIR)/$(TEST_DIR)
	-@ rm -rf $(BUILD_OBJ_DIR)/$(SRC_DIR)
	-@ if [ -d "$(BUILD_OBJ_DIR)" ]; then rmdir --ignore-fail-on-non-empty $(BUILD_OBJ_DIR); fi

tca: clean_test test