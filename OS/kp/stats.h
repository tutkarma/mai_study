#ifndef _STATS_H_
#define _STATS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include "argument.h"

#define SUCCESS 1
#define NOT_MEMORY -1
#define NOT_PLAYER -2

#define SIZE_LOG 100
#define DB_NAME "playersDB"

/* TODO: use tree for db, not vector */

typedef struct _player {
    char log[SIZE_LOG];
    int wins;
    int loses;
    int min_steps;
} *Player;

typedef struct _tablePlayers {
    Player players;
    uint32_t size;
    uint32_t freespace;
} *TablePlrs;

TablePlrs tableCreate(void);
void tableAdd(TablePlrs pl, char *log);
void tablePrint(TablePlrs pl);
Player tableFind(TablePlrs pl, char *log);
void tableDestroy(TablePlrs *pl);
void tableSort(TablePlrs pl);

int tableSave(TablePlrs pl, FILE *file);
int tableLoad(TablePlrs *pl, FILE *file);

void tableFill(TablePlrs pl, char *log, int wins, int loses, int min_steps);
void tableUpdate(TablePlrs pl, char *log, int res, int steps);

#endif