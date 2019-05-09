CC = gcc
CFLAGS = -g -std=c99 -w -pipe -O3

all: generate print execute

generate: owner.o io.o generate.o
	$(CC) $^ -o $@

print:    io.o print.o
	$(CC) $^ -o $@

execute:  io.o execute.o
	$(CC) $^ -o $@

clean:
	rm -f *.o

cleanall:
	rm -f generate print execute *.o

