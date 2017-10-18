//
//  sendfile.c
//  
//
//  Created by Akmal Fadlurohman on 10/17/17.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "sender.h"


int strToInt(char* str) {
    int numb = str[0] - '0';
    for (int i=1;i<strlen(str);i++) {
        int temp = numb;
        temp *= 10;
        numb = temp + (str[i] - '0');
    }
    return numb;
}
void deleteFromSendBuffer(Sender *S,int first, int last,int* currentSendBuffer) {
    for (int i=first;i<=last;i++) {
        for (int j=0;j<sendFrame_size;j++) {
            S->sendBuffer[i][j] = S->sendBuffer[last+i][j];
        }
    }
    *currentSendBuffer = *currentSendBuffer - (last-first);
}

int main(int argc, char* argv[]) {
    if (argc == 6) {
        
        Sender S;
        char* msgFileName = (char*) malloc(sizeof(argv[1]));
        int SWS = strToInt(argv[2]);
        int sendBufferSize = strToInt(argv[3]);
        char* destIP = (char*) malloc(sizeof(argv[4]));
        int port = strToInt(argv[5]);
        
        strcpy(msgFileName,argv[1]);
        strcpy(destIP,argv[4]);

        FILE *msgFile = fopen(msgFileName,"r");
        if (msgFile == NULL) {
            printf("Error cannot open the specified file\n");
            return 0;
        }
        
        char* msgBuffer =  malloc(sendBufferSize*sizeof(char));
        int msgLength = 0;
        int flag = 0;
       
        while (flag != 1)
        {
            char c;
            fscanf(msgFile,"%c",&c);
            if (c == '.') {
                flag = 1;
            } else {
                msgBuffer[msgLength] = c;
                msgLength++;
            }
        }
        fclose(msgFile);
        sendFrame* sendFrameBuffer =  (sendFrame*) calloc((sendBufferSize/sizeof(sendFrame)),sizeof(sendFrame));
        fillSendFrameBuffer(sendFrameBuffer,msgBuffer,msgLength);
    
        initSender(&S,SWS,sendBufferSize);
        
        int currentSendBuffer = msgLength;
        int maxSendBuffer = sendBufferSize;
        fillSendBuffer(&S,sendFrameBuffer,currentSendBuffer);
        free(msgBuffer);
        
        struct sockaddr_in receiverAddr;
        
        receiverAddr.sin_family = AF_INET;
        receiverAddr.sin_port = htons(port);
        receiverAddr.sin_addr.s_addr = inet_addr(destIP);
        memset(receiverAddr.sin_zero, '\0', sizeof receiverAddr.sin_zero);
        int udpSocket = socket(PF_INET,SOCK_DGRAM,0);
        
        for (int i=0;i<currentSendBuffer;i++) {
            /*printf("SOH: %#x |",sendFrameBuffer[i].SOH);
            printf("seqNum: %d %#x |",sendFrameBuffer[i].seqNum,sendFrameBuffer[i].seqNum);
            printf("STX: %#x |",sendFrameBuffer[i].STX);
            printf("Data: %c %#x |",sendFrameBuffer[i].data,sendFrameBuffer[i].data);
            printf("ETX: %#x|",sendFrameBuffer[i].ETX);
            printf("Checksum: %#x|\n",sendFrameBuffer[i].checksum);*/
            printBytes(S.sendBuffer[i]);
        }
        printf("\n");
        
        int frameSent = 0;
        int ackReceived = 0;
        while(1) {
            if (currentSendBuffer == 0) {
                break;
            } else if (currentSendBuffer < S.SWS) {
                for (int i=0;i<currentSendBuffer;i++) {
                    sendto(udpSocket,S.sendBuffer[i],9,0,(struct sockaddr *)&receiverAddr,sizeof(receiverAddr));
                }
                deleteFromSendBuffer(&S,0,currentSendBuffer,&currentSendBuffer);
                break;
            } else {
                for (int i=0;i<S.SWS;i++) {
                    int sent = sendto(udpSocket,S.sendBuffer[i],9,0,(struct sockaddr *)&receiverAddr,sizeof(receiverAddr));
                    frameSent += sent;
                }
                deleteFromSendBuffer(&S,0,S.SWS-1,&currentSendBuffer);
                for (int i=0;i<currentSendBuffer;i++) {
                    printBytes(S.sendBuffer[i]);
                }
            }
        }
        printf("%d\n",frameSent);
        int socketStats = shutdown(udpSocket,2);
        free(S.sendBuffer);
        
    }
    return 0;
}
