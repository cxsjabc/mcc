# This file is preferred to used for somebody who likes to work on Windows, likes to use make and msvc cl.exe together.
# Seems that it's strange, but it's a good idea for that somebody hates the strange grammar of nmake.exe, so use make.exe instead.
# But, he doesn't like Cygwin/MinGW gcc, so use msvc cl.exe instead.

# Please execute make in msvc command line environment if USE_MSVC(see the variable below).
# Usage:
# Execute the commands in root folder of this project.
#  make -f makefiles/Makefile.win.mk
#  make -f makefiles/Makefile.win.mk test
#
#  make -f makefiles/Makefile.win.mk clean
#  make -f makefiles/Makefile.win.mk clean_test
#
# Also, can add CC config:
#  make -f makefiles/Makefile.win.mk CC=gcc


CUR_DIR := .

ifneq (cl, $(CC))
USE_MSVC := 0
else
USE_MSVC := 1
endif

C_INCLUDES := -I $(CUR_DIR)/inc
C_FLAGS := $(C_INCLUDES)

SRC_DIR := src
SRCS := $(wildcard $(SRC_DIR)/*.c)

BUILD_OBJ_DIR := out

# MSVC
ifeq (1, $(USE_MSVC))
# 0. cl.exe
CC := cl.exe
LINK := link.exe

MAIN_OBJ := main.obj
OUT_FILE := mcc.exe

OBJS := $(patsubst %.c,%.obj, $(SRCS))
BUILD_OBJS := $(patsubst %.obj,$(BUILD_OBJ_DIR)/%.obj, $(OBJS))

# /showIncludes /sourceDependencies:directives $<.json
$(BUILD_OBJ_DIR)/%.obj : %.c
	$(CC) $(C_FLAGS) /c $< /Fo:$@

all: prepare $(BUILD_OBJS) $(BUILD_OBJ_DIR)/$(MAIN_OBJ)
	$(LINK) /out:$(OUT_FILE) $(BUILD_OBJS) $(BUILD_OBJ_DIR)/$(MAIN_OBJ)

clean:
	- rm -rf $(BUILD_OBJS) $(BUILD_OBJ_DIR)/$(MAIN_OBJ) $(OUT_FILE)
	- rmdir $(BUILD_OBJ_DIR)/$(SRC_DIR)
	- if [ -d "$(BUILD_OBJ_DIR)" ]; then rmdir --ignore-fail-on-non-empty $(BUILD_OBJ_DIR); fi

prepare:
	- @ if [ ! -d "$(BUILD_OBJ_DIR)" ]; then mkdir $(BUILD_OBJ_DIR); fi
	- @ if [ ! -d "$(BUILD_OBJ_DIR)/$(SRC_DIR)" ]; then mkdir $(BUILD_OBJ_DIR)/$(SRC_DIR); fi
else
# not MSVC
# 1. use gcc.exe
CC := gcc
# 2. Winows 64bit: use Mingw gcc generate 32bit program (>= 10.2.0)
# CC = i686-w64-mingw32-gcc.exe
  # If Windows 32bit, can use mingw32-gcc.exe (This is for 32bit system)

# 3. Winows 64bit: use Mingw gcc generate 64bit program (>= 10.2.0)
# CC = x86_64-w64-mingw32-gcc.exe

# 4. Please don't use Android NDK GCC toolchain(If use, maybe needs to fix file ending style, CRLF or LF type)

MAIN_OBJ := main.o
OUT_FILE := mcc

C_FLAGS += -Wall -Werror -g

OBJS := $(patsubst %.c,%.o, $(SRCS))
BUILD_OBJS := $(patsubst %.o,$(BUILD_OBJ_DIR)/%.o, $(OBJS))
BUILD_OBJS_DEPENDS := $(patsubst %.o, %.o.d, $(BUILD_OBJS))

all: prepare $(BUILD_OBJS) $(BUILD_OBJ_DIR)/$(MAIN_OBJ)
	$(CC) -o $(OUT_FILE) $(BUILD_OBJS) $(BUILD_OBJ_DIR)/$(MAIN_OBJ) $(C_FLAGS)

$(BUILD_OBJ_DIR)/%.o : %.c
	$(CC) $(C_FLAGS) -MMD -MF $@.d -MT $@ -c $< -o $@

-include $(BUILD_OBJ_DIR)/$(SRC_DIR)/*.d
-include $(BUILD_OBJ_DIR)/$(MAIN_OBJ).d

clean:
	- rm -rf $(BUILD_OBJS) $(BUILD_OBJ_DIR)/$(MAIN_OBJ) $(BUILD_OBJS_DEPENDS) $(BUILD_OBJ_DIR)/$(MAIN_OBJ).d $(OUT_FILE)
	- rmdir $(BUILD_OBJ_DIR)/$(SRC_DIR)
	- if [ -d "$(BUILD_OBJ_DIR)" ]; then rmdir --ignore-fail-on-non-empty $(BUILD_OBJ_DIR); fi

prepare:
	- @ if [ ! -d "$(BUILD_OBJ_DIR)" ]; then mkdir $(BUILD_OBJ_DIR); fi
	- @ if [ ! -d "$(BUILD_OBJ_DIR)/$(SRC_DIR)" ]; then mkdir $(BUILD_OBJ_DIR)/$(SRC_DIR); fi

endif

mcc: all

# test
TEST_DIR := test
TEST_SRCS := $(wildcard $(TEST_DIR)/*.c)

ifeq (1, $(USE_MSVC))
# MSVC
TEST_OBJS := $(patsubst %.c,%.obj, $(TEST_SRCS))
TEST_BUILD_OBJS := $(patsubst %.obj,$(BUILD_OBJ_DIR)/%.obj, $(TEST_OBJS))
TEST_OUT := mcc_test.exe

prepare_test:
	- @ if [ ! -d "$(BUILD_OBJ_DIR)" ]; then mkdir $(BUILD_OBJ_DIR); fi
	- @ if [ ! -d "$(BUILD_OBJ_DIR)/$(SRC_DIR)" ]; then mkdir $(BUILD_OBJ_DIR)/$(SRC_DIR); fi
	- @ if [ ! -d "$(BUILD_OBJ_DIR)/$(TEST_DIR)" ]; then mkdir $(BUILD_OBJ_DIR)/$(TEST_DIR); fi

test: prepare_test $(BUILD_OBJS) $(TEST_BUILD_OBJS)
	$(LINK) /out:$(TEST_OUT) $(TEST_BUILD_OBJS) $(BUILD_OBJS)

clean_test:
	- rm -rf $(TEST_BUILD_OBJS) $(TEST_OUT)
	- rmdir $(BUILD_OBJ_DIR)/$(TEST_DIR)

else
# not MSVC
TEST_OBJS := $(patsubst %.c,%.o, $(TEST_SRCS))
TEST_BUILD_OBJS := $(patsubst %.o,$(BUILD_OBJ_DIR)/%.o, $(TEST_OBJS))
TEST_BUILD_OBJS_DEPENDS := $(patsubst %.o, %.o.d, $(TEST_BUILD_OBJS))

TEST_OUT := mcc_test

prepare_test:
	- @ if [ ! -d "$(BUILD_OBJ_DIR)" ]; then mkdir $(BUILD_OBJ_DIR); fi
	- @ if [ ! -d "$(BUILD_OBJ_DIR)/$(SRC_DIR)" ]; then mkdir $(BUILD_OBJ_DIR)/$(SRC_DIR); fi
	- @ if [ ! -d "$(BUILD_OBJ_DIR)/$(TEST_DIR)" ]; then mkdir $(BUILD_OBJ_DIR)/$(TEST_DIR); fi

test: prepare_test $(BUILD_OBJS) $(TEST_BUILD_OBJS)
	$(CC) -o $(TEST_OUT) $(TEST_BUILD_OBJS) $(BUILD_OBJS) $(C_FLAGS)

clean_test:
	- rm -rf $(TEST_BUILD_OBJS) $(TEST_BUILD_OBJS_DEPENDS) $(TEST_OUT)
	- rm -rf $(BUILD_OBJ_DIR)/$(TEST_DIR)
	- if [ -d "$(BUILD_OBJ_DIR)" ]; then rmdir --ignore-fail-on-non-empty $(BUILD_OBJ_DIR); fi

endif
