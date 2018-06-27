CC = g++
FLAGS = -std=c++14 -O2 -pedantic -lm
FILE_PROG = main.cpp
FILE_BENCH = bench.cpp
PROG = da9
BENCH = dfs

all: da9 dfs

da9:
	$(CC) $(FLAGS) -o $(PROG) $(FILE_PROG)

dfs:
	$(CC) $(FLAGS) -o $(BENCH) $(FILE_BENCH)

clean:
	rm -f *.o dfs da9