#include "stats.h"

TablePlrs tableCreate(void)
{
    TablePlrs pl = (TablePlrs) malloc(sizeof(*pl));
    if (!pl) {
        fprintf(stderr, "ERROR: no memory\n");
        exit(NOT_MEMORY);
    }
    pl->players = (Player) malloc(sizeof(*(pl->players)));
    pl->size = 0;
    pl->freespace = 1;
    return pl;
}

void tableResize(TablePlrs pl)
{
    pl->players = realloc(pl->players, 2 * pl->size * sizeof(*(pl->players)));
    if (!pl->players) {
        fprintf(stderr, "ERROR: no memory\n");
        exit(NOT_MEMORY);
    }
    pl->freespace = pl->size;
}

void tableAdd(TablePlrs pl, char *log)
{
    if (!pl->freespace) {
        tableResize(pl);
    }
    strcpy(pl->players[pl->size].log, log);
    pl->players[pl->size].wins = 0;
    pl->players[pl->size].loses = 0;
    pl->players[pl->size].min_steps = 100;
    pl->size++;
    pl->freespace--;
}

void tableFill(TablePlrs pl, char *log, int win, int lose, int min_step)
{
    Player toFill = tableFind(pl, log);
    if(toFill) {
        toFill->wins = win;
        toFill->loses = lose;
        toFill->min_steps = min_step;
    }
}

void tableUpdate(TablePlrs pl, char *log, int res, int steps)
{
    Player player = tableFind(pl, log);
    if (!player) {
        tableAdd(pl, log);
        player = tableFind(pl, log);
    }
    res == 1 ? player->wins++ : player->loses++;
    if (steps < player->min_steps) {
        player->min_steps = steps;
    }
}

void tablePrint(TablePlrs pl)
{
    if (pl) {
        printf("PLAYER\tWINS\tLOSES\tMIN STEPS\n");
        for (uint32_t i = 0; i < pl->size; ++i) {
            printf("%s\t", pl->players[i].log);
            printf("%d\t", pl->players[i].wins);
            printf("%d\t", pl->players[i].loses);
            printf("%d\n", pl->players[i].min_steps);
        }
    }
}

Player tableFind(TablePlrs pl, char *log)
{
    if (pl) {
        for (uint32_t i = 0; i < pl->size; ++i) {
            if (!strcmp(pl->players[i].log, log)) {
                return &(pl->players[i]);
            }
        }
    }
    return NULL;
}

void tableDestroy(TablePlrs *pl)
{
    free((*pl)->players);
    (*pl)->players = NULL;
    free(*pl);
    *pl = NULL;
}

void tableSort(TablePlrs pl)
{
    struct _player tmp;
    if (pl) {
        for(int i = 1; i < pl->size; ++i) {
            for(int j = i; j > 0; --j) {
                if (pl->players[j - 1].wins < pl->players[j].wins) {
                    tmp = pl->players[j - 1];
                    pl->players[j - 1] = pl->players[j];
                    pl->players[j] = tmp;
                }
            }
        }
    }
}

/* TODO: binary read and write */
int tableSave(TablePlrs pl, FILE *file)
{
    if (file < 0) {
        printf("Cannot open file\n");
        return 1;
    }
    for (int i = 0; i < pl->size; ++i) {
        fprintf(file, "%s %d %d %d ", pl->players[i].log, pl->players[i].wins, pl->players[i].loses, pl->players[i].min_steps);
    }
    return 0;
}

int tableLoad(TablePlrs *pl, FILE *file)
{
    Player tmp;
    if (file < 0) {
        printf("Cannot open file\n");
        return 1;
    }
    while (fscanf(file, "%s %d %d %d", tmp->log, &tmp->wins, &tmp->loses, &tmp->min_steps) == 4) {
        tableAdd((*pl), tmp->log);
        tableFill((*pl), tmp->log, tmp->wins, tmp->loses, tmp->min_steps);
    }
    return 0;
}


