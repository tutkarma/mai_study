#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <inttypes.h>

#define STR_SIZE 256

typedef int32_t ID;

typedef struct _msg {
    ID client;
    int sum;
    int action;
    ID receiverClient;
    void *requester;
    char message[STR_SIZE];
    char fileName[STR_SIZE];
} MsgData;

#endif