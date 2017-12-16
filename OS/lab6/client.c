#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <pthread.h>

#include "zmq.h"
#include "message.h"

void menuAdmin()
{
    printf("1) Stop server \n");
    printf("2) Run server \n");
    printf("3) Exit \n");
}

void menuUser()
{
    printf("1) Put money into account\n");
    printf("2) Get money from account\n");
    printf("3) Send money to another account\n");
    printf("4) Check balance\n");
    printf("5) Leave the bank\n");
}

void *SendRecv(void *arg)
{
    MsgData *md = (MsgData *) arg;
    zmq_msg_t message;
    zmq_msg_init_size(&message, sizeof(MsgData));
    memcpy(zmq_msg_data(&message), md, sizeof(MsgData));
    zmq_msg_send(&message, md->requester, 0);
    zmq_msg_close(&message);

    zmq_msg_init(&message);
    zmq_msg_recv(&message, md->requester, 0);
    md = (MsgData *) zmq_msg_data(&message);
    printf("%s\n", md->message);
    zmq_msg_close(&message);
    pthread_exit(NULL);
    return 0;
}

int main(int argc, char **argv)
{
    void *context = zmq_ctx_new();
    int admin = 0;

    ID client, bank;
    if (argc == 2 && !strcmp(argv[1], "admin")) {
        admin = 1;
    } else {
        printf("Enter client's login: ");
        scanf("%d", &client);
    }

    char adress[25];
    printf("Enter bank's adress: ");
    scanf("%d", &bank);

    sprintf(adress, "%s%d", "tcp://localhost:", bank);

    printf("tcp://localhost:%d \n", bank);

    void *sendSocket = zmq_socket(context, ZMQ_REQ);
    zmq_connect(sendSocket, adress);

    if (admin) {
        int act = 0;
        menuAdmin();
        do {
            scanf("%d", &act);
            MsgData md;
            md.action = act + 10;
            switch (act) {
                /*case 1: {
                    pthread_t th;
                    printf("Enter the file name: ");
                    scanf("%s", md.fileName);
                    md.requester = sendSocket;
                    pthread_create(&th, NULL, SendRecv, &md);
                    pthread_detach(th);
                    break;
                }

                case 2: {
                    pthread_t th;
                    printf("Enter the file name: ");
                    scanf("%s", md.fileName);
                    md.requester = sendSocket;
                    pthread_create(&th, NULL, SendRecv, &md);
                    pthread_detach(th);
                    break;
                }*/

                case 1: {
                    pthread_t th;
                    md.requester = sendSocket;
                    pthread_create(&th, NULL, SendRecv, &md);
                    pthread_detach(th);
                    break;
                }

                case 2: {
                    pthread_t th;
                    md.requester = sendSocket;
                    pthread_create(&th, NULL, SendRecv, &md);
                    pthread_detach(th);
                    break;
                }

                case 3:
                    break;

                default: {
                    printf("Inccorect command\n");
                    break;
                }
            }
        } while (act != 3);


    } else {
        int act = 0, sum = 0;
        menuUser();
        do {
            scanf("%d", &act);

            MsgData md;
            md.action = act;
            md.client = client;

            switch (act) {
                case 1: {
                    printf("Enter the sum: ");
                    scanf("%d", &sum);

                    md.sum = sum;
                    pthread_t th;
                    md.requester = sendSocket;
                    pthread_create(&th, NULL, SendRecv, &md);
                    pthread_detach(th);
                    break;
                }

                case 2: {
                    printf("Enter the sum: ");
                    scanf("%d", &sum);

                    md.sum = sum;
                    pthread_t th;
                    md.requester = sendSocket;
                    pthread_create(&th, NULL, SendRecv, &md);
                    pthread_detach(th);

                    break;
                }

                case 3: {
                    int receiverClient;
                    printf("Enter receiver id: ");
                    scanf("%d", &receiverClient);

                    printf("Enter the sum: ");
                    scanf("%d", &sum);

                    md.sum = sum;
                    md.receiverClient = receiverClient;

                    pthread_t th;
                    md.requester = sendSocket;
                    pthread_create(&th, NULL, SendRecv, &md);
                    pthread_detach(th);

                    break;
                }

                case 4: {
                    pthread_t th;
                    md.requester = sendSocket;
                    pthread_create(&th, NULL, SendRecv, &md);
                    pthread_detach(th);

                    break;
                }

                case 5:
                    break;

                default: {
                    printf("Inccorect command\n");
                    break;
                }
            }
        } while (act != 5);
    }

    zmq_close(sendSocket);
    zmq_ctx_destroy(context);

    return 0;
}