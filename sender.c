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
    S->sendBuffer = (char**) calloc((sendBufferSize/sizeof(char*)),sizeof(char*));
    for (int i=0;i<sendBufferSize;i++) {
        S->sendBuffer[i] = (char *) malloc(sendFrame_size);
    }
}

void fillSendFrameBuffer(sendFrame* sendFrameBuffer,char* msgBuffer,int msgLength) {
    for (int i=0;i<msgLength;i++) {
        initFrame(&sendFrameBuffer[i]);
        setSeqNum(&sendFrameBuffer[i],i);
        setData(&sendFrameBuffer[i],msgBuffer[i]);
        setCheckSum(&sendFrameBuffer[i],checkSum(sendFrameBuffer[i]));
    }
}

void fillSendBuffer(Sender *S,sendFrame* sendFrameBuffer,int currentSendBuffer) {
    for (int i=0;i<currentSendBuffer;i++) {
        for (int j=0;j<sendFrame_size;j++) {
            S->sendBuffer[i][j] = sendFrameToByte(&sendFrameBuffer[i])[j];
        }
    }
}