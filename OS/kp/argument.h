#ifndef ARG_H
#define ARG_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "zmq.h"

#define SERVER_USAGE "usage: ./server -p <number of players (from 2 to 4)>\n"
#define CLIENT_USAGE "usage: ./client -l <login>\n"

#define SIZE_LOG 100
#define MSG_SIZE 2048

#define error(msg)                             \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

typedef struct _cell {
    int numberOfFoxes;
    int cntFoxes;
    int fox;
    int print;
    int check;
} Cell;

typedef struct _result {
    char name[SIZE_LOG];
    size_t steps;
    int yep;
    int mode;
} Result;

typedef struct _args{
    Cell matrix[10][10];
    char log[SIZE_LOG];
    int id1;
    size_t players;
    void *requester;
    int status;
    int mode;
    size_t result;
} Args;

#endif