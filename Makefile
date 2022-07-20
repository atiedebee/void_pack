SRC = src/void_pack.c
OBJ = void_pack.o

CC = gcc-11

FLAGS = -O2 -Wall -Wextra -Wshadow -std=c11 -Wpedantic -fpic
FLAGS += -I include/

default: static shared

.PHONY: debug clean static shared obj test all

debug:
	$(eval FLAGS += -DDEBUG)

all: static shared obj test
	
test:
	$(CC) $(FLAGS) $(SRC) src/tests.c -DDEBUG -fsanitize=undefined

obj: lib
	$(CC) $(FLAGS) $(SRC) -c -o $(addprefix lib/, $(OBJ))

static: lib
	$(CC) $(FLAGS) $(SRC) -c -o $(OBJ)
	ar -rcs lib/libvoid_pack.a $(OBJ)
	@rm $(OBJ)

shared: lib
	$(CC) $(FLAGS) $(SRC) -c -o $(OBJ)
	gcc -shared $(FLAGS) $(OBJ) -o lib/libvoid_pack.so
	@rm $(OBJ)
lib:
	@mkdir lib

clean:
	rm -f $(OBJ) ./a.out ./lib/*
