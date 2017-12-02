#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "zmq.h"
#include "bank.h"
#include "message.h"

int main(void)
{
    int code = 0;
    ClientDB clientBase = ClientDBCreate();
    void *context = zmq_ctx_new();
    void *responsSocket = zmq_socket(context, ZMQ_REP);

    char adress[25];
    printf("Enter bank's adress: ");
    ID bank;
    scanf("%d", &bank);
    sprintf(adress, "%s%d", "tcp://*:", bank);

    zmq_bind(responsSocket, adress);

    //int counter = 0;

    while (1) {

        zmq_msg_t message;

        zmq_msg_init(&message);
        zmq_msg_recv(&message, responsSocket, 0);
        MsgData *md = (MsgData *) zmq_msg_data(&message);
        zmq_msg_close(&message);

        char info[STR_SIZE];

        switch (md->action) {
            case 1:
                printf("Put money into the account id: %d\n", md->client);
                MoneyPut(md->client, md->sum, clientBase);
                
                strcpy(info, "Operation was completed successfully\0");
                memcpy(md->message, info, strlen(info) + 1);

                zmq_msg_init_size(&message, sizeof(MsgData));
                memcpy(zmq_msg_data(&message), md, sizeof(MsgData));
                zmq_msg_send(&message, responsSocket, 0);
                zmq_msg_close(&message);

                break;
            case 2:
                printf("Get money from the account id: %d\n", md->client);

                code = MoneyGet(md->client, md->sum, clientBase);
                if (code == SUCCESS) {
                    printf("Success\n");
                    strcpy(info, "Operation was completed successfully\0");
                } else if (code == NOT_ENOUGH_MONEY) {
                    printf("Not enough money\n");
                    strcpy(info, "You not enough money\0");
                } else {
                    printf("Not bank client\n");
                    strcpy(info, "You aren't a client of bank\0");
                }

                memcpy(md->message, info, strlen(info) + 1);
                zmq_msg_init_size(&message, sizeof(MsgData));
                memcpy(zmq_msg_data(&message), md, sizeof(MsgData));
                zmq_msg_send(&message, responsSocket, 0);
                zmq_msg_close(&message);

                break;

            case 3:
                printf("Send money from account id: %d to account id: %d\n", md->client, md->receiverClient);
                code = MoneySend(md->client, md->receiverClient, md->sum, clientBase);
                if (code == SUCCESS) {
                    printf("Success\n");
                    strcpy(info, "Operation was completed successfully\0");
                } else if (code == NOT_ENOUGH_MONEY) {
                    printf("Not enought money\n");
                    strcpy(info, "You not enough money");
                } else if (code == RECEIVER_NOT_CLIENT) {
                    printf("Receiver not bank client\n");
                    strcpy(info, "Receiver is not a client of bank\0");
                }

                memcpy(md->message, info, strlen(info) + 1);
                zmq_msg_init_size(&message, sizeof(MsgData));
                memcpy(zmq_msg_data(&message), md, sizeof(MsgData));
                zmq_msg_send(&message, responsSocket, 0);
                zmq_msg_close(&message);
                break;

            case 4:
                printf("Check account id: %d\n", md->client);

                code = CheckAccount(md->client, clientBase);
                if (code == NOT_CLIENT) {
                    printf("Not bank client\n");
                    //strcpy(info, "You aren't client of bank\0");
                    char info[] = "You aren't a client of bank\0";
                } else {
                    printf("Client sum is %d\n", code);
                    sprintf(info, "%s%d%c", "Your account is ", code, '\0');
                }

                memcpy(md->message, info, strlen(info) + 1);
                zmq_msg_init_size(&message, sizeof(MsgData));
                memcpy(zmq_msg_data(&message), md, sizeof(MsgData));
                zmq_msg_send(&message, responsSocket, 0);
                zmq_msg_close(&message);

                break;
        }
        zmq_msg_close(&message);
        ClientDBPrint(clientBase);

    }
    zmq_close(responsSocket);
    zmq_ctx_destroy(context);

    ClientDBDestroy(&clientBase);


}