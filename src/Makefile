SRC = void_pack.c
OBJ = $(SRC:.c=.o)

CC = gcc

FLAGS = -ansi -Wall -Wextra -Wpedantic -O2

default:
	$(CC) $(FLAGS) -c $(SRC) -o $(OBJ)


.PHONY: clean

clean:
	rm $(OBJ)
