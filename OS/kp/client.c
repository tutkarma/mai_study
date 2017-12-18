#include "argument.h"

void help()
{
    printf("1) Result this game\n2) My stats\n3) Top-5 for all time\n4) Exit\n");
}

void printRes(Result *tmp, size_t n)
{
    for (int i = 0; i < n; ++i) {
        printf("%d) %s: %zu steps\n", i + 1, tmp[i].name, tmp[i].steps);
    }
    printf("\n");
}

void selectMode()
{
    printf("Select game mode:\n1) time\n2) minimum steps\n");
}

int main(int argc, char **argv)
{
    int i;
    int j;
    char answer[MSG_SIZE];
    size_t steps = 0;
    Result table[4];
    void *context = zmq_ctx_new();
    if (!context) error("zmq_ctx_new");
    void *requester = zmq_socket(context, ZMQ_REQ);

    /* PLAYER ATTRIBUTES */
    Args myGame;
    myGame.players = 0; 
    myGame.status = -1; 
    int caughtFoxes = 0;

    /* CHECK ARGUMENTS FROM INPUT */
    if (argc == 3 && !strcmp(argv[1],"-l")) {
        strcpy(myGame.log, argv[2]);
    } else {
        printf(CLIENT_USAGE);
        return 0;
    }

    /* CONNECTING */
    int cn = zmq_connect(requester, "tcp://localhost:5555");
    printf("Connect to tcp://localhost:5555\n");
    if (cn) error("connect");

    int time = 1500;
    zmq_setsockopt(requester, ZMQ_RCVTIMEO, &time, sizeof(time));

    zmq_send(requester, &myGame, sizeof(Args), 0);
    zmq_recv(requester, &myGame, sizeof(Args), 0);

    if (myGame.players == 0) {
        printf("There are no more places\n");
        return 0;
    }

    /* 2 MODES: TIME AND STEPS */
    selectMode();
    while(1) {
        scanf("%d", &myGame.mode);
        if (myGame.mode != 1 && myGame.mode != 2) {
            printf("Unknown mode. Pls, select 1 or 2.\n");
        } else {
            break;
        }
    }


    /* WAITING ALL PLAYERS */
    printf("Wait...\n");
    while(1) {
        zmq_send(requester, &myGame, sizeof(Args), 0);
        zmq_recv(requester, &myGame, sizeof(Args), 0);
        if(myGame.status == 1) {
            break;
        }
    }

    /* GAME */
    while (caughtFoxes < 8) {
        printf("Enter the coordinates (x & y), pls: (from 1 to 10)\n");
        printf("x = ");
        scanf("%d", &i);
        if(i == -1) {
            printf("Hey admin!!!\n");
            break;
        }
        if (i < 1 || i > 10) {
            printf("Pls, from 1 to 10.\n");
            continue;
        }
        printf("y = ");
        scanf("%d", &j);
        if (j < 1 || j > 10) {
            printf("Pls, from 1 to 10.\n");
            continue;
        }
        --i;
        --j;
        if (myGame.matrix[i][j].check == 1) {
            printf("You have checked this cell.\n");
            continue;
        } else {
            myGame.matrix[i][j].check = 1;
        }

        if (myGame.matrix[i][j].fox == 1) {
            printf("\nGrats, you caught %d fox(es).\n", myGame.matrix[i][j].cntFoxes);
            caughtFoxes += myGame.matrix[i][j].cntFoxes;
        } else {
            printf("\nOh, fox is not here\n");
        }

        myGame.matrix[i][j].print = 1;
        printf("\n");
        for(int k = 0; k < 10; ++k) {
            for(int l = 0; l < 10; ++l) {
                if(myGame.matrix[k][l].print == 0) {
                    printf("x ");
                } else {
                    if(myGame.matrix[k][l].fox != 1) {
                        printf("%d ", myGame.matrix[k][l].numberOfFoxes);
                    } else {
                        printf("F ");
                    }
                }
            }
            printf("\n");
        }
        printf("\n");
        printf("Have to catch %d fox(es)\n", 8 - caughtFoxes);
        ++steps;
    }

    printf("Congratulations, you catch all the foxes.\n");
    printf("     |\\/|  /\\  /\n  .__.. \\_/  \\/ \n   \\____/_ __/  \n      /_/_/     \n\n");
    myGame.result = steps;
    myGame.status = 0;
    zmq_send(requester, &myGame, sizeof(Args), 0);
    zmq_recv(requester, &answer, sizeof(answer), 0);
    printf("%s\n", answer);
    while(1) {
        int cmd;
        help();
        scanf("%d", &cmd);
        switch(cmd) {
            /* GET RESULT GAME */
            case 1: {
                myGame.status = cmd;
                zmq_send(requester, &myGame, sizeof(Args), 0);
                zmq_recv(requester, &table, sizeof(Result) * 4, 0);
                if(table[0].yep) {
                    printRes(table, myGame.players);
                } else {
                    printf("Pls, wait other players.\n");
                }
                break;
            }

            /* GET MY STATISTICS */
            case 2: {
                myGame.status = cmd;
                zmq_send(requester, &myGame, sizeof(Args), 0);
                zmq_recv(requester, &answer, sizeof(answer), 0);
                printf("%s\n", answer);
                break;
            }

            /* GET TOP-5 PLAYERS */
            case 3: {
                myGame.status = cmd;
                zmq_send(requester, &myGame, sizeof(Args), 0);
                zmq_recv(requester, &answer, sizeof(answer), 0);
                printf("%s\n", answer);
                break;
            }

            /* EXIT */
            case 4: {
                myGame.status = cmd;
                zmq_send(requester, &myGame, sizeof(Args), 0);
                zmq_recv(requester, &answer, sizeof(answer), 0);
                printf("%s\n", answer);
                zmq_close(requester);
                zmq_ctx_term(context);
                return 0;
                break;
            }

            default: {
                printf("Unknown command\n");
                break;
            }
        }
    }
    zmq_close(requester);
    zmq_ctx_destroy(context);
    return 0;
}
