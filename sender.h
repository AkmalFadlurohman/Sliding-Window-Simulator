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
void fillFrameBuffer(sendFrame* frameBuffer,char* msgBuffer,int *currentFrameBuffer,int msgLength);
void deleteFromFrameBuffer(sendFrame* frameBuffer,int first, int last,int *currentFrameBuffer);
void fillSendBuffer(Sender *S,sendFrame* frameBuffer,int *currentFrameBuffer,int *currentSendBuffer,int *msgLength);
void deleteFromSendBuffer(Sender *S,int first, int last,int* currentSendBuffer);

#endif
