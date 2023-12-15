# This Makefile is also available for Windows if gcc.exe/clang.exe... are supported(in PATH environment).
# Usage:
#  make
#  make test
#
#  make clean
#  make clean_test

CUR_DIR := .

.PHONY : prepare all clean

$(info "OS: $(OS)")

# ** $(CC) and $(CPP) must use the same version or compatible versions,
# *  or link error and execute error will occur.
# *  i.e gcc 10.2 and g++ 11.4, execute error.
# **

# 0. Use clang (16.0.0)
# CC = clang

# 1. Use gcc (11.4.0)
CC := gcc

# 1.1 Use g++
# CC = g++

# 2. Winows 64bit: use Mingw gcc generate 32bit program
# CC = i686-w64-mingw32-gcc.exe
  # If Windows 32bit, can use mingw32-gcc.exe (This is for 32bit system)

# 3. Winows 64bit: use Mingw gcc generate 64bit program
# CC = x86_64-w64-mingw32-gcc.exe

# g++ 11.4.0
CPP := g++

MCC_ROOT := $(CUR_DIR)/../..
MCC_SRC_PATH := $(CUR_DIR)/../../src
MCC_INCLUDES := $(MCC_ROOT)/inc

C_INCLUDES := -I $(MCC_INCLUDES)
C_FLAGS := $(C_INCLUDES)
C_FLAGS += -Wall -Werror -g
# disable clang secure function warnings on clang-16.0
C_FLAGS += -D_CRT_SECURE_NO_WARNINGS
C_FLAGS += -lstdc++

BUILD_OBJ_DIR := out
BUILD_MCC_OBJ_DIR := out/mcc

DEPENDS_MCC_REAL_SRCS := $(wildcard $(MCC_SRC_PATH)/*.c)
DEPENDS_MCC_SRCS := $(patsubst ./../../src/%.c, src/%.c, $(DEPENDS_MCC_REAL_SRCS))

DEPENDS_MCC_OBJS := $(patsubst %.c, $(BUILD_MCC_OBJ_DIR)/%.o, $(DEPENDS_MCC_SRCS))
$(info DEPENDS_MCC_REAL_SRCS: $(DEPENDS_MCC_REAL_SRCS))
$(info DEPENDS_MCC_SRCS: $(DEPENDS_MCC_SRCS))
$(info DEPENDS_MCC_OBJS: $(DEPENDS_MCC_OBJS))

SRC_DIR := .
SRCS := $(wildcard $(SRC_DIR)/*.cpp)

CPP_TEST_OBJ := cpp_test.o
CPP_TEST_OUT := cpp_test
ifeq ($(OS), Windows_NT)
CPP_TEST_OUT := $(CPP_TEST_OUT).exe
endif

OBJS := $(patsubst %.cpp,%.o, $(SRCS))
BUILD_OBJS := $(patsubst %.o,$(BUILD_OBJ_DIR)/%.o, $(OBJS))
$(info SRCS: $(SRCS))
$(info OBJS: $(OBJS))
$(info BUILD_OBJS: $(BUILD_OBJS))

$(BUILD_OBJ_DIR)/%.o : %.cpp
	$(CPP) $(C_FLAGS) -c $< -o $@

all: prepare mcc_depends $(BUILD_OBJS)
	$(CPP) -o $(CPP_TEST_OUT) $(BUILD_OBJS) $(DEPENDS_MCC_OBJS) $(C_FLAGS)

clean:
	rm -rf $(CPP_TEST_OUT) $(CPP_TEST_OBJ) $(BUILD_OBJS) $(DEPENDS_MCC_OBJS)
	- rmdir out/mcc/src
	- rmdir out/mcc
	- rmdir out

cpp_test: all

prepare:
	- @ mkdir out out/mcc out/mcc/src

$(BUILD_MCC_OBJ_DIR)/src/%.o : $(MCC_SRC_PATH)/%.c
	$(CC) $(C_FLAGS) -c $< -o $@

mcc_depends: $(DEPENDS_MCC_OBJS)

	
