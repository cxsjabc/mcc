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

# 0. Use clang (>= 16.0.0)
# CC = clang

# 1. Use gcc (>= 10.3.1)
CC := gcc

# 2. Winows 64bit: use Mingw gcc generate 32bit program (>= 10.2.0)
# CC = i686-w64-mingw32-gcc.exe
  # If Windows 32bit, can use mingw32-gcc.exe (This is for 32bit system)

# 3. Winows 64bit: use Mingw gcc generate 64bit program (>= 10.2.0)
# CC = x86_64-w64-mingw32-gcc.exe

# 4. Winows: Please don't use Cygwin gcc

# 5. Please don't use Android NDK GCC toolchain(If use, maybe needs to fix file ending style, CRLF or LF type and other issues)

C_INCLUDES := -I $(CUR_DIR)/inc
C_FLAGS := $(C_INCLUDES)
C_FLAGS += -Wall -Werror -g
# disable clang secure function warnings on >= clang-16.0
C_FLAGS += -D_CRT_SECURE_NO_WARNINGS

SRC_DIR := src
SRCS := $(wildcard $(SRC_DIR)/*.c)

BUILD_OBJ_DIR := out

MAIN_OBJ := main.o
ifeq ($(OS), Windows_NT)
OUT_FILE := mcc.exe
else
OUT_FILE := mcc
endif

OBJS := $(patsubst %.c,%.o, $(SRCS))
BUILD_OBJS := $(patsubst %.o,$(BUILD_OBJ_DIR)/%.o, $(OBJS))

$(BUILD_OBJ_DIR)/%.o : %.c
	$(CC) $(C_FLAGS) -c $< -o $@

all: prepare $(BUILD_OBJS) $(BUILD_OBJ_DIR)/$(MAIN_OBJ)
	$(CC) -o $(OUT_FILE) $(BUILD_OBJS) $(BUILD_OBJ_DIR)/$(MAIN_OBJ) $(C_FLAGS)

clean:
	rm -rf $(BUILD_OBJS) $(BUILD_OBJ_DIR)/$(MAIN_OBJ) $(OUT_FILE)
	rm -rf $(BUILD_OBJ_DIR)/$(SRC_DIR)
	- rmdir $(BUILD_OBJ_DIR)

mcc: all

prepare:
	- @mkdir $(BUILD_OBJ_DIR) 2>&1 > /dev/null
	- @mkdir $(BUILD_OBJ_DIR)/$(SRC_DIR) 2>&1 >/dev/null

# test
TEST_DIR := test
TEST_SRCS := $(wildcard $(TEST_DIR)/*.c)

TEST_OBJS := $(patsubst %.c,%.o, $(TEST_SRCS))
TEST_BUILD_OBJS := $(patsubst %.o,$(BUILD_OBJ_DIR)/%.o, $(TEST_OBJS))

ifeq ($(OS), Windows_NT)
TEST_OUT := mcc_test.exe
else
TEST_OUT := mcc_test
endif

prepare_test:
	- mkdir $(BUILD_OBJ_DIR)
	- mkdir $(BUILD_OBJ_DIR)/$(SRC_DIR)
	- mkdir $(BUILD_OBJ_DIR)/$(TEST_DIR)

test: prepare_test $(BUILD_OBJS) $(TEST_BUILD_OBJS)
	$(CC) -o $(TEST_OUT) $(TEST_BUILD_OBJS) $(BUILD_OBJS) $(C_FLAGS)

clean_test:
	rm -rf $(BUILD_OBJS) $(TEST_BUILD_OBJS) $(TEST_OUT)
	rm -rf $(BUILD_OBJ_DIR)/$(TEST_DIR)
	- rmdir $(BUILD_OBJ_DIR)
