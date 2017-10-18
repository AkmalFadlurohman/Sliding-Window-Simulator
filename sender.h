#ifndef sender_h
#define sender_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sendFrame.h"

typedef struct {
    int SWS;
    int sendBufferSize;
    unsigned char seqNum;
    unsigned int LAR;
    unsigned char LFS;
    char** sendBuffer;
} Sender;

void initSender(Sender *S,int SWS,int sendBufferSize);
void fillSendFrameBuffer(sendFrame* sendFrameBuffer,char* msgBuffer,int msgLength);
void fillSendBuffer(Sender *S,sendFrame* sendFrameBuffer,int currentSendBuffer);

#endif
