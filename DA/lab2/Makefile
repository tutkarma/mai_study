CC = gcc
FLAGS = -std=c99 -w -pipe -O2 -Wextra -Werror -Wall -Wno-sign-compare -pedantic -lm -Werror=return-type 

all: da2

da2: TPatricia.o dict.o serialization.o main.o
	$(CC) $(FLAGS) $^ -o $@

clean:
	rm -f *.o da2