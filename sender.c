//
//  sender.c
//  
//
//  Created by Akmal Fadlurohman on 10/16/17.
//
//
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include "sender.h"

void initSender(Sender *S,int SWS,int sendBufferSize) {
    S->SWS = SWS;
    S->sendBufferSize = sendBufferSize;
    S->seqNum = 0;
    S->LAR = -1;
    S->LFS = -1;
    int Size = sizeof(char*);
    S->sendBuffer = malloc(sendBufferSize*Size);
    for (int i=0;i<sendBufferSize;i++) {
        S->sendBuffer[i] = malloc(sendFrame_size*sizeof(char));
    }
}

void fillFrameBuffer(sendFrame* frameBuffer,char* msgBuffer,int *currentFrameBuffer,int msgLength) {
    *currentFrameBuffer = msgLength;
    for (int i=0;i<*currentFrameBuffer;i++) {
        initFrame(&frameBuffer[i]);
        setSeqNum(&frameBuffer[i],i);
        setData(&frameBuffer[i],msgBuffer[i]);
        setCheckSum(&frameBuffer[i],checkSum(frameBuffer[i]));
    }
}

void deleteFromFrameBuffer(sendFrame* frameBuffer,int first, int last,int *currentFrameBuffer) {
    for (int i=first;i<=last;i++) {
        frameBuffer[i] = frameBuffer[last+i];
    }
    *currentFrameBuffer = *currentFrameBuffer - (last-first);
}

void fillSendBuffer(Sender *S,sendFrame* frameBuffer,int *currentFrameBuffer,int *currentSendBuffer,int *msgLength) {
    if (*msgLength <= (S->sendBufferSize)) {
        *currentSendBuffer = *msgLength;
    } else {
        *currentSendBuffer = S->sendBufferSize;
    }
    for (int i=0;i<*currentSendBuffer;i++) {
        for (int j=0;j<sendFrame_size;j++) {
            S->sendBuffer[i][j] = sendFrameToByte(&frameBuffer[i])[j];
        }
    }
}

void deleteFromSendBuffer(Sender *S,int first, int last,int* currentSendBuffer) {
    for (int i=first;i<=last;i++) {
        for (int j=0;j<sendFrame_size;j++) {
            S->sendBuffer[i][j] = S->sendBuffer[last+i][j];
        }
    }
    *currentSendBuffer = *currentSendBuffer - (last-first);
}
