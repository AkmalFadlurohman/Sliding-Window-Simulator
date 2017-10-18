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

void printFrameBuffer(sendFrame* frameBuffer,int currentFrameBuffer) {
    for (int i=0;i<currentFrameBuffer;i++) {
        printf("SOH: %#x |",frameBuffer[i].SOH);
        printf("seqNum: %d %#x |",frameBuffer[i].seqNum,frameBuffer[i].seqNum);
        printf("STX: %#x |",frameBuffer[i].STX);
        printf("Data: %c %#x |",frameBuffer[i].data,frameBuffer[i].data);
        printf("ETX: %#x|",frameBuffer[i].ETX);
        printf("Checksum: %#x|\n",frameBuffer[i].checksum);
    }
}

void printSendBuffer(Sender S,int currentSendBuffer) {
    for (int i=0;i<currentSendBuffer;i++) {
        printBytes(S.sendBuffer[i]);
    }
}

int strToInt(char* str) {
    int numb = str[0] - '0';
    for (int i=1;i<strlen(str);i++) {
        int temp = numb;
        temp *= 10;
        numb = temp + (str[i] - '0');
    }
    return numb;
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
        
        int currentFrameBuffer;
        sendFrame* frameBuffer =  (sendFrame*) calloc((sendBufferSize/sizeof(sendFrame)),sizeof(sendFrame));
        fillFrameBuffer(frameBuffer,msgBuffer,&currentFrameBuffer,msgLength);
        printf("CurrentFrameBuffer:%d\n",currentFrameBuffer);
    
        initSender(&S,SWS,sendBufferSize);
        
        int currentSendBuffer;
        fillSendBuffer(&S,frameBuffer,&currentFrameBuffer,&currentSendBuffer,&msgLength);
        printf("CurrentSendBuffer: %d\n",currentSendBuffer);
        
        printFrameBuffer(frameBuffer,currentFrameBuffer);
        printSendBuffer(S,currentSendBuffer);
        
        struct sockaddr_in receiverAddr;
        
        receiverAddr.sin_family = AF_INET;
        receiverAddr.sin_port = htons(port);
        receiverAddr.sin_addr.s_addr = inet_addr(destIP);
        memset(receiverAddr.sin_zero, '\0', sizeof receiverAddr.sin_zero);
        int udpSocket = socket(PF_INET,SOCK_DGRAM,0);
        
        int frameSent = 0;
        int ackReceived = 0;
        /*while(1) {
            if (currentSendBuffer == 0) {
                break;
            } else if (currentSendBuffer < S.SWS) {
                for (int i=0;i<currentSendBuffer;i++) {
                    int sent = sendto(udpSocket,S.sendBuffer[i],9,0,(struct sockaddr *)&receiverAddr,sizeof(receiverAddr));
                    frameSent += sent;
                }
                //deleteFromSendBuffer(&S,0,currentSendBuffer,&currentSendBuffer);
                //printf("%d\n",currentSendBuffer);
                break;
            } else {
                for (int i=0;i<S.SWS;i++) {
                    int sent = sendto(udpSocket,S.sendBuffer[i],9,0,(struct sockaddr *)&receiverAddr,sizeof(receiverAddr));
                    frameSent += sent;
                }
                //deleteFromSendBuffer(&S,0,S.SWS-1,&currentSendBuffer);
            }
        }*/
        int socketStats = shutdown(udpSocket,2);
        //free(S.sendBuffer);
        //free(msgBuffer);
        return 0;
    }
}
