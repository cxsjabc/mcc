CUR_DIR = .

# 1. *nix: use gcc
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
# $(warning "$(SRCS)")
OBJS := $(patsubst %.c,%.o, $(SRCS))
# $(warning "$(OBJS)")

%.o : %.c
	$(CC) $(C_FLAGS) -c $< -o $@ 

all: $(OBJS) main.o
	$(CC) -o mcc $(OBJS) main.o $(C_FLAGS)

clean:
	rm -rf $(OBJS) main.o

mcc: all

# test
TEST_DIR = test
TEST_SRCS := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS := $(patsubst %.c,%.o, $(TEST_SRCS))

test: $(OBJS) $(TEST_OBJS)
	$(CC) -o mcc_test $(TEST_OBJS) $(OBJS) $(C_FLAGS)

clean_test:
	rm -rf $(TEST_OBJS)