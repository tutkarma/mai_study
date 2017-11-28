CC = gcc
FLAGS = -std=c99 -pthread -w -pipe -O2 -Wextra -Werror -Wall -Wno-sign-compare -pedantic -lm

all: run

run: libbst.so mainStat.o mainDyn.o
	$(CC) $(FLAGS) -o run-stat mainStat.o -L. -lbst -Wl,-rpath,.
	$(CC) $(FLAGS) -o run-dyn mainDyn.o -ldl

mainStat.o: mainStat.c
	$(CC) -c $(FLAGS) mainStat.c

mainDyn.o: mainDyn.c
	$(CC) -c $(FLAGS) mainDyn.c

bst.o: bst.c
	$(CC) -c -fPIC $(FLAGS) bst.c

libbst.so: bst.o
	$(CC) $(FLAGS) -shared -o libbst.so bst.o

clean:
	rm -f *.o run-stat run-dyn *.so