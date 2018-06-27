CC = g++
FLAGS = -std=c++14 -O2 -pedantic -lm
FILE_PROG = main.cpp
FILE_BENCH = bench.cpp
PROG = da8
BENCH = naive

all: da8 naive

da8:
	$(CC) $(FLAGS) -o $(PROG) $(FILE_PROG)

naive:
	$(CC) $(FLAGS) -o $(BENCH) $(FILE_BENCH)

clean:
	rm -f *.o naive da8