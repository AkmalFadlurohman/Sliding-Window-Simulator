//
//  sender.c
//  
//
//  Created by Akmal Fadlurohman on 10/16/17.
//
//
#include <stdlib.h>
#include <sys/socket.h>
#include "sender.h"

void initSender(Sender *S,int SWS,int sendBufferSize) {
    S->SWS = SWS;
    S->seqNum = 0;
    S->LAR = -1;
    S->LFS = -1;
    //int size = sendBufferSize;
    S->sendBuffer = (sendFrame*) calloc((sendBufferSize/sizeof(sendFrame)),sizeof(sendFrame));
}
void fillSendBuffer(Sender *S,char* msgBuffer,int msgLength) {
    for (int i=0;i<msgLength;i++) {
        initFrame(&S->sendBuffer[i]);
        setSeqNum(&S->sendBuffer[i],i);
        setData(&S->sendBuffer[i],msgBuffer[i]);
        setCheckSum(&S->sendBuffer[i],checkSum(S->sendBuffer[i]));
    }
}
