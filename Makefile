CUR_DIR = .

C_INCLUDES = -I $(CUR_DIR)/inc
C_FLAGS = $(C_INCLUDES)

all: src/main.o
	gcc -o mcc src/main.o $(C_FLAGS)

src/main.o: src/main.c
	# echo "CUR_DIR: ${CUR_DIR}, C_FLAGS: $(C_FLAGS)"
	gcc -c src/main.c -o src/main.o $(C_FLAGS)