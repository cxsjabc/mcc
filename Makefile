# This Makefile is also available for Windows if gcc.exe/clang.exe... are supported(in PATH environment).
# Usage:
#  make
#  make test
#
#  make clean
#  make clean_test

CUR_DIR := .

.PHONY : prepare all clean mcc test clean_test mcc_test

$(info "OS: $(OS)")

# Note 1: can use make CC=g++ to change the default $(CC) compiler.
# Note 2: Sometimes, you installed many compilers and the compilers may be with the same name, you
  #       can use the full path to locate the compiler.
  #       i.e. D:\compiler1\clang.exe

# 0. Use clang (16.0.0)
# CC := clang

# 1. Use gcc (10.2.0)
CC := gcc

# 2. Winows 64bit: use Mingw gcc generate 32bit program (10.2.0)
# CC := i686-w64-mingw32-gcc.exe
  # If Windows 32bit, can use mingw32-gcc.exe (This is for 32bit system)

# 3. Winows 64bit: use Mingw gcc generate 64bit program (10.2.0)
# CC := x86_64-w64-mingw32-gcc.exe

# 4. Please don't use Android NDK GCC toolchain(If use, maybe needs to fix file ending style, CRLF or LF type and other issues)

C_INCLUDES := -I $(CUR_DIR)/inc

# Extra or test flags: -v: verbose output
# TEST_C_FLAGS += -v

C_FLAGS := $(C_INCLUDES)
C_FLAGS += -Wall -Werror -g
# disable clang secure function warnings on clang-16.0
C_FLAGS += -D_CRT_SECURE_NO_WARNINGS
ifeq ($(OS),Windows_NT)
C_FLAGS += -D_WINDOWS
endif
C_FLAGS += $(TEST_C_FLAGS)

SRC_DIR := src
SRCS := $(wildcard $(SRC_DIR)/*.c)

BUILD_OBJ_DIR := out

MAIN_OBJ := main.o
ifeq ($(OS), Windows_NT)
OUT_FILE := mcc.exe
NULL_FILE := NUL
else
OUT_FILE := mcc
NULL_FILE := /dev/null
endif

OBJS := $(patsubst %.c, %.o, $(SRCS))
BUILD_OBJS := $(patsubst %.o, $(BUILD_OBJ_DIR)/%.o, $(OBJS))
BUILD_OBJS_DEPENDS := $(patsubst %.o, %.o.d, $(BUILD_OBJS))

all: srcs_depend prepare $(BUILD_OBJS) $(BUILD_OBJ_DIR)/$(MAIN_OBJ)
	$(CC) -o $(OUT_FILE) $(BUILD_OBJS) $(BUILD_OBJ_DIR)/$(MAIN_OBJ) $(C_FLAGS)

$(BUILD_OBJ_DIR)/%.o : %.c
	$(CC) $(C_FLAGS) -MMD -MF $@.d -MT $@ -c $< -o $@

-include $(BUILD_OBJ_DIR)/$(SRC_DIR)/*.d
-include $(BUILD_OBJ_DIR)/$(MAIN_OBJ).d

clean:
	-rm -rf $(BUILD_OBJS) $(BUILD_OBJ_DIR)/$(MAIN_OBJ) $(BUILD_OBJS_DEPENDS) $(BUILD_OBJ_DIR)/$(MAIN_OBJ).d $(OUT_FILE)
	-rm -rf $(BUILD_OBJ_DIR)/$(SRC_DIR)
	-if [ -d "$(BUILD_OBJ_DIR)" ]; then rmdir --ignore-fail-on-non-empty $(BUILD_OBJ_DIR); fi

srcs_depend:

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
TEST_OUT := mcc_test.exe
else
TEST_OUT := mcc_test
endif

prepare_test:
	-@ if [ ! -d "$(BUILD_OBJ_DIR)" ]; then mkdir $(BUILD_OBJ_DIR); fi
	-@ if [ ! -d "$(BUILD_OBJ_DIR)/$(SRC_DIR)" ]; then mkdir $(BUILD_OBJ_DIR)/$(SRC_DIR); fi
	-@ if [ ! -d "$(BUILD_OBJ_DIR)/$(TEST_DIR)" ]; then mkdir $(BUILD_OBJ_DIR)/$(TEST_DIR); fi

test: prepare_test $(BUILD_OBJS) $(TEST_BUILD_OBJS)
	$(CC) -o $(TEST_OUT) $(TEST_BUILD_OBJS) $(BUILD_OBJS) $(C_FLAGS)

-include $(BUILD_OBJ_DIR)/$(TEST_DIR)/*.d

clean_test:
	-rm -rf $(BUILD_OBJS) $(TEST_BUILD_OBJS) $(TEST_BUILD_OBJS_DEPENDS) $(TEST_OUT)
	-rm -rf $(BUILD_OBJ_DIR)/$(TEST_DIR)
	-rm -rf $(BUILD_OBJ_DIR)/$(SRC_DIR)
	-if [ -d "$(BUILD_OBJ_DIR)" ]; then rmdir --ignore-fail-on-non-empty $(BUILD_OBJ_DIR); fi

tca: clean_test test