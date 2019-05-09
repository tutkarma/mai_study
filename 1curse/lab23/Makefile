CC = gcc
FLAGS = -g -std=c99 -w -pipe -O3 -Wextra -Werror -pedantic

all: run

run: tree.o main.o
	$(CC) $(FLAGS) $^ -o $@

clean:
	rm -f *.o run