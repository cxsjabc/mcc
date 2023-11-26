CUR_DIR = .

CC = gcc
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