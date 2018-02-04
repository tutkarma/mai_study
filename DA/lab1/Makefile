CC = gcc
FLAGS = -std=c99 -w -pipe -O3 -Wextra -Werror -Wall -Wno-sign-compare -pedantic -lm

all: da1

da1: vector.o main.o
	$(CC) $(FLAGS) $^ -o $@

clean:
	rm -f *.o da1