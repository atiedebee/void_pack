SRC = src/void_pack.c
OBJ = void_pack.o

CC = gcc-11

FLAGS = -O2 -Wall -Wextra -Wshadow -std=c11 -Wpedantic -fpic
FLAGS += -I include/

default:
	@printf '\n\e[1;31mInvalid usage\n\e[0;37m\n'
	@printf '\e[1;34mmake static - create a static object file\n'
	@printf '\e[1;34mmake shared - create a dynamically linked object file\n'
	@printf '\e[1;34mmake clean  - remove all binaries\e[0;37m\n'

.PHONY: clean static shared

static: lib
	$(CC) $(FLAGS) $(SRC) -c -o $(OBJ)
	ar -rcs lib/libvoid_pack.a $(OBJ)
	rm $(OBJ)

shared: lib
	$(CC) $(FLAGS) $(SRC) -c -o $(OBJ)
	gcc -shared $(FLAGS) $(OBJ) -o lib/libvoid_pack.so
	rm $(OBJ)

clean:
	rm -f $(OBJ) ./a.out ./lib/*
