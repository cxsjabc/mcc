CUR_DIR = .

CC = gcc
C_INCLUDES = -I $(CUR_DIR)/inc
C_FLAGS = $(C_INCLUDES)
C_FLAGS += -Wall -Werror -g

SRCDIR = src

SRCS := $(wildcard $(SRCDIR)/*.c)
# $(warning "$(SRCS)")
OBJS := $(patsubst %.c,%.o, $(SRCS))
# $(warning "$(OBJS)")

%.o : %.c
	$(CC) $(C_FLAGS) -c $< -o $@ 

all: $(OBJS)
	$(CC) -o mcc $(OBJS) $(C_FLAGS)

mcc: all