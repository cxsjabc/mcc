# Usage:
#  make
#  make test
#
#  make clean
#  make clean_test

CUR_DIR = .

# not MSVC
# 0. Use clang
# CC = clang
# 1. Use gcc.exe
CC = gcc
# 2. Winows 64bit: use Mingw gcc generate 32bit program (>= 10.2.0)
# CC = i686-w64-mingw32-gcc.exe
  # If Windows 32bit, can use mingw32-gcc.exe (This is for 32bit system)

# 3. Winows 64bit: use Mingw gcc generate 64bit program (>= 10.2.0)
# CC = x86_64-w64-mingw32-gcc.exe

# 4. Winows: Please don't use Cygwin gcc

# 5. Please don't use Android NDK GCC toolchain(If use, maybe needs to fix file ending style, CRLF or LF type)

C_INCLUDES = -I $(CUR_DIR)/inc
C_FLAGS = $(C_INCLUDES)
C_FLAGS += -Wall -Werror -g

SRC_DIR = src
SRCS := $(wildcard $(SRC_DIR)/*.c)

MAIN_OBJ = main.o
OUT_FILE = mcc

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
TEST_OUT = mcc_test

test: $(OBJS) $(TEST_OBJS)
	$(CC) -o $(TEST_OUT) $(TEST_OBJS) $(OBJS) $(C_FLAGS)

clean_test:
	rm -rf $(TEST_OBJS) $(TEST_OUT)
