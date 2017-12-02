#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include "zmq.h"
#include "message.h"

int main(void)
{
    void *context = zmq_ctx_new();

    ID client, bank;
    printf("Enter client's login: ");
    scanf("%d", &client);

    char adress[25];
    printf("Enter bank's adress: ");
    scanf("%d", &bank);

    sprintf(adress, "%s%d", "tcp://localhost:", bank);

    printf("tcp://localhost:%d", bank);

    void *sendSocket = zmq_socket(context, ZMQ_REQ);
    zmq_connect(sendSocket, adress);

    printf("1) Put money into account\n");
    printf("2) Get money from account\n");
    printf("3) Send money to another account\n");
    printf("4) Check balance\n");
    printf("5) Leave the bank\n");

    int act = 0, sum = 0;
    zmq_msg_t message;
    do {
        printf("Choose action: ");
        scanf("%d", &act);

        MsgData md;
        md.action = act;
        md.client = client;

        switch(act) {
            case 1: {
                printf("Enter the sum: ");
                scanf("%d", &sum);

                md.sum = sum;
                //zmq_msg_t message;
                zmq_msg_init_size(&message, sizeof(MsgData));
                memcpy(zmq_msg_data(&message), &md, sizeof(MsgData));
                zmq_msg_send(&message, sendSocket, 0);
                zmq_msg_close(&message);

                zmq_msg_init(&message);
                zmq_msg_recv(&message, sendSocket, 0);
                MsgData *md = (MsgData *) zmq_msg_data(&message);
                printf("%s\n", md->message);
                zmq_msg_close(&message);
                break;
            }

            case 2: {
                printf("Enter the sum: ");
                scanf("%d", &sum);

                md.sum = sum;

                //zmq_msg_t message;
                zmq_msg_init_size(&message, sizeof(MsgData));
                memcpy(zmq_msg_data(&message), &md, sizeof(MsgData));
                zmq_msg_send(&message, sendSocket, 0);
                zmq_msg_close(&message);

                zmq_msg_init(&message);
                zmq_msg_recv(&message, sendSocket, 0);
                MsgData *md = (MsgData *) zmq_msg_data(&message);
                printf("%s\n", md->message);
                zmq_msg_close(&message);

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

                //zmq_msg_t message;
                zmq_msg_init_size(&message, sizeof(MsgData));
                memcpy(zmq_msg_data(&message), &md, sizeof(MsgData));
                zmq_msg_send(&message, sendSocket, 0);
                zmq_msg_close(&message);

                zmq_msg_init(&message);
                zmq_msg_recv(&message, sendSocket, 0);
                MsgData *md = (MsgData *) zmq_msg_data(&message);
                printf("%s\n", md->message);
                zmq_msg_close(&message);

                break;
            }

            case 4: {
                //zmq_msg_t message;
                zmq_msg_init_size(&message, sizeof(MsgData));
                memcpy(zmq_msg_data(&message), &md, sizeof(MsgData));
                zmq_msg_send(&message, sendSocket, 0);
                zmq_msg_close(&message);

                zmq_msg_init(&message);
                zmq_msg_recv(&message, sendSocket, 0);
                MsgData *md = (MsgData *) zmq_msg_data(&message);
                printf("%s\n", md->message);
                zmq_msg_close(&message);

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

    zmq_close(sendSocket);
    zmq_ctx_destroy(context);

    return 0;
}