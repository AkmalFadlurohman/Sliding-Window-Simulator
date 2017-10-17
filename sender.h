#ifndef sender_h
#define sender_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sendFrame.h"

typedef struct {
    int SWS;
    unsigned char seqNum;
    unsigned int LAR;
    unsigned char LFS;
    sendFrame *sendBuffer;
} Sender;

void initSender(Sender *S,int SWS,int sendBufferSize);
void fillSendBuffer(Sender *S,char* msgBuffer,int msgLength);

#endif
