#include "argument.h"
#include "stats.h"

#define TIME_GAME 1
#define STEP_GAME 2

void fieldCreate(Cell field[][10])
{
    /* initialization */
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            field[i][j].numberOfFoxes = 0;
            field[i][j].cntFoxes = 0;
            field[i][j].fox = 0;
            field[i][j].print = 0;
            field[i][j].check = 0;
        }
    }

    srand(time(NULL));
    for (int k = 0; k < 8; ++k) {
        int i = rand() % 10;
        int j = rand() % 10;
        field[i][j].fox = 1;
        field[i][j].cntFoxes++;
    }
}

void countOfFoxs(Cell field[][10])
{
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            /* horizontal line */
            for (int k = 0; k < 10; ++k) {
                if (field[i][k].fox == 1 && k != j) {
                    field[i][j].numberOfFoxes += field[i][k].cntFoxes;
                }
            }
            /* vertical line */
            for (int k = 0; k < 10; ++k) {
                if (field[k][j].fox == 1 && k != i) {
                    field[i][j].numberOfFoxes += field[k][j].cntFoxes;
                }
            }
            /* diagonal line "\" */
            for (int k = i - 1, l = j - 1; k >= 0 && l >= 0; --k, --l) {
                if (field[k][l].fox == 1) {
                    field[i][j].numberOfFoxes += field[k][l].cntFoxes;
                }
            }
            for (int k = i + 1, l = j + 1; k < 10 && l < 10; ++k, ++l) {
                if (field[k][l].fox == 1) {
                    field[i][j].numberOfFoxes += field[k][l].cntFoxes;
                }
            }
            /* diagonal line "/" */
            for (int k = i + 1, l = j - 1; k < 10 && l >= 0; ++k, --l) {
                if (field[k][l].fox == 1) {
                    field[i][j].numberOfFoxes += field[k][l].cntFoxes;
                }
            }
            for (int k = i - 1, l = j + 1; k >= 0 && l < 10; --k, ++l) {
                if (field[k][l].fox == 1) {
                    field[i][j].numberOfFoxes += field[k][l].cntFoxes;
                }
            }
            if (field[i][j].fox == 1) {
                field[i][j].numberOfFoxes += field[i][j].cntFoxes;
            }
        }
    }
}


void copy(Args *tmp, Args *tmp2)
{
    for(int i = 0; i < 10; ++i) {
        for(int j = 0; j < 10; ++j) {
            tmp2->matrix[i][j] = tmp->matrix[i][j];
        }
    }
}

void sort(Result *table, size_t n)
{
    Result tmp;
    for(int i = 1; i < n; ++i) {
        for(int j = i; j > 0; --j) {
            if(table[j - 1].steps > table[j].steps) {
                tmp = table[j - 1];
                table[j - 1] = table[j];
                table[j] = tmp;
            }
        }
    }
}

void printRes(Result *tmp, size_t n)
{
    for (int i = 0; i < n; ++i) {
        printf("%d) %s: %zu steps\n", i + 1, tmp[i].name, tmp[i].steps);
    }
}

void getTop(TablePlrs db, char *ans)
{
    tableSort(db);
    strcpy(ans, "USERNAME        WINS\tLOSSES\tMIN STEPS");
    int size = (db->size > 5) ? 5 : db->size;
    for (int i = 0; i < size; ++i) {
        sprintf(ans, "%s\n %-16s %d\t %d\t %d", ans, db->players[i].log, db->players[i].wins, db->players[i].loses, db->players[i].min_steps);
    }
}

int main(int argc, char **argv)
{
    printf("Server start.\n");
    size_t countOfPlayers;
    int id[4] = { 0 };
    char answer[MSG_SIZE];
    Result table[4];
    Args game;
    Args tmp;

    /* CONNECTING */
    void *context = zmq_ctx_new();
    if (!context) error("zmq_ctx_new");
    void *responder = zmq_socket(context, ZMQ_REP);
    
    int bind = zmq_bind(responder, "tcp://*:5555");
    if (bind) error("bind");

    /* CHECK ARGUMENTS FROM INPUT */
    if (argc == 3 && !strcmp(argv[1],"-p")) {
        countOfPlayers = atoi(argv[2]);
    } else {
        printf(SERVER_USAGE);
        return 0;
    }
    if (!countOfPlayers || (countOfPlayers < 2 && countOfPlayers > 4)) {
        printf(SERVER_USAGE);
        return 0;
    }

    TablePlrs db = tableCreate();
    FILE *fp = fopen(DB_NAME, "r+");
    printf("Loading data base...\n");
    if(tableLoad(&db, fp)) error("db load");
    printf("Loaded db:\n");
    tablePrint(db);
    fseek(fp, 0, SEEK_SET);

    /* INITIALIZATION AND FILL FOXES */
    fieldCreate(game.matrix);
    countOfFoxs(game.matrix);

    for (size_t ready = 0; ready < countOfPlayers; ++ready) {
        zmq_recv(responder, &tmp, sizeof(Args), 0);
        if (!tmp.players) {
            printf("User %s connected and start to play\n", tmp.log);
            tmp.id1 = ready;
            table[ready].yep = 0;
            tmp.players = countOfPlayers;
            copy(&game, &tmp);
            printf("\t%zu players are ready\n", ready + 1);
        } else {
            --ready;
        }
        zmq_send(responder, &tmp, sizeof(Args), 0);
    }

    size_t num = countOfPlayers;
    printf("Start the game...\n");

    /* WAITING ALL PLAYERS */
    while (num) {
        zmq_recv(responder, &tmp, sizeof(Args), 0);
        if(!id[tmp.id1]) {
            --num;
            tmp.status = 1;
            id[tmp.id1] = 1;
        }
        zmq_send(responder, &tmp, sizeof(Args), 0);
    }

    printf("All players connected\n");

    for (int i = 0; i < countOfPlayers; ++i) {
        id[i] = 0;
    }

    num = countOfPlayers;
    char timeWinner[SIZE_LOG];
    int stepGame = 0;

    while(1) {
        if (!countOfPlayers) {
            printf("Counting of scores...\n");
            sort(table, num);
            printRes(table, num);

            /* STEP MODE -- DB UPDATE */
            if (stepGame) {
                char userWithMinSteps[SIZE_LOG];
                strcpy(userWithMinSteps, table[0].name);
                int modeUserWithMinSteps = table[0].mode;
                int minSteps = table[0].steps;
                /*for (int i = 0; i < num; ++i) {
                    if (table[i].steps < minSteps) {
                        strcpy(userWithMinSteps, table[i].name);
                        modeUserWithMinSteps = table[i].mode;
                    }
                }*/

                if (modeUserWithMinSteps == STEP_GAME) {
                    tableUpdate(db, userWithMinSteps, 1, minSteps);
                }
                for (int i = 0; i < num; ++i) {
                    if (strcmp(userWithMinSteps, table[i].name) && table[i].mode == STEP_GAME) {
                        tableUpdate(db, table[i].name, 0, table[i].steps);
                    }
                }
            }


            table[0].yep = 1;
            countOfPlayers = 1;
            printf("Save data base.\n");
            tablePrint(db);
            tableSave(db, fp);
            fclose(fp);
        }
        zmq_recv(responder, &tmp, sizeof(Args), 0);
        switch (tmp.status) {

            /* FINISH GAME */
            case 0: {
                if(!id[tmp.id1]) {
                    strcpy(table[tmp.id1].name, tmp.log);
                    table[tmp.id1].steps = tmp.result;
                    id[tmp.id1] = 1;
                    table[tmp.id1].mode = tmp.mode;
                    printf("User %s finished the game\n", tmp.log);
                    printf("\this result: %zu steps\n", tmp.result);
                    strcpy(answer, "Your'ar finished. Pls, wait other players");
                    zmq_send(responder, &answer, sizeof(answer), 0);
                    --countOfPlayers;

                    /* TIME MODE -- DB UPDATE */
                    if (tmp.mode == TIME_GAME) {
                        if (countOfPlayers + 1 == num) {
                            tableUpdate(db, tmp.log, 1, tmp.result);
                            strcpy(timeWinner, tmp.log);
                        } else {
                            tableUpdate(db, tmp.log, 0, tmp.result);
                        }
                    } else if (tmp.mode == STEP_GAME) {
                        stepGame = 1;
                    }
                }
                break;
            }

            /* RESULT GAME */
            case 1: {
                if(countOfPlayers) {
                    zmq_send(responder, &table, sizeof(table), 0);
                } else {
                    table[0].yep = 1;
                    zmq_send(responder, &table, sizeof(table), 0);
                }
                break;
            }

            /* PLAYER STATISTICS */
            case 2: {
                Player print;
                print = tableFind(db, tmp.log);
                sprintf(answer, "Username: %s\nWins: %d\nLosses: %d\nMinimum steps: %d\n", print->log, print->wins, print->loses, print->min_steps);
                zmq_send(responder, &answer, sizeof(answer), 0);
                break;
            }

            /* TOP 5 PLAYERS */
            case 3: {
                getTop(db, answer);
                zmq_send(responder, &answer, sizeof(answer), 0);
                break;
            }

            /* EXIT */
            case 4: {
                printf("Player %s disconnect\n", tmp.log);
                strcpy(answer, "GoodBuy");
                zmq_send(responder, &answer, sizeof(answer), 0);
                break;
            }
            default: {
                tmp.players = 0;
                zmq_send(responder, &tmp, sizeof(Args), 0);
                break;
            }
        }

    }

    zmq_close(responder);
    zmq_ctx_destroy(context);
    return 0;
}
