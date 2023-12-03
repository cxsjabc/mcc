# This Makefile is also available for Windows if gcc.exe/clang.exe... are supported(in PATH environment).
# Usage:
#  make
#  make test
#
#  make clean
#  make clean_test

CUR_DIR = .

$(info "OS: $(OS)")

# 0. Use clang (>= 16.0.0)
# CC = clang

# 1. Use gcc (>= 10.3.1)
CC = gcc

# 2. Winows 64bit: use Mingw gcc generate 32bit program (>= 10.2.0)
# CC = i686-w64-mingw32-gcc.exe
  # If Windows 32bit, can use mingw32-gcc.exe (This is for 32bit system)

# 3. Winows 64bit: use Mingw gcc generate 64bit program (>= 10.2.0)
# CC = x86_64-w64-mingw32-gcc.exe

# 4. Winows: Please don't use Cygwin gcc

# 5. Please don't use Android NDK GCC toolchain(If use, maybe needs to fix file ending style, CRLF or LF type and other issues)

C_INCLUDES = -I $(CUR_DIR)/inc
C_FLAGS = $(C_INCLUDES)
C_FLAGS += -Wall -Werror -g
# disable clang secure function warnings on >= clang-16.0
C_FLAGS += -D_CRT_SECURE_NO_WARNINGS

SRC_DIR = src
SRCS := $(wildcard $(SRC_DIR)/*.c)

MAIN_OBJ = main.o
ifeq ($(OS), Windows_NT)
OUT_FILE = mcc.exe
else
OUT_FILE = mcc
endif

OBJS := $(patsubst %.c,%.o, $(SRCS))

%.o : %.c
	$(CC) $(C_FLAGS) -c $< -o $@ 

all: $(OBJS) $(MAIN_OBJ)
	$(CC) -o $(OUT_FILE) $(OBJS) $(MAIN_OBJ) $(C_FLAGS)

clean:
	rm -rf $(OBJS) $(MAIN_OBJ) $(OUT_FILE)

mcc: all

# test
TEST_DIR = test
TEST_SRCS := $(wildcard $(TEST_DIR)/*.c)

TEST_OBJS := $(patsubst %.c,%.o, $(TEST_SRCS))
ifeq ($(OS), Windows_NT)
TEST_OUT = mcc_test.exe
else
TEST_OUT = mcc_test
endif

test: $(OBJS) $(TEST_OBJS)
	$(CC) -o $(TEST_OUT) $(TEST_OBJS) $(OBJS) $(C_FLAGS)

clean_test:
	rm -rf $(TEST_OBJS) $(TEST_OUT)
