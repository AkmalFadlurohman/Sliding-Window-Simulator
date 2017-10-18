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
#include "ack.h"

/* Global */
struct sockaddr_in senderAddr,receiverAddr;

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
void die(char *s)
{
    perror(s);
    exit(1);
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
    
        initSender(&S,SWS,sendBufferSize);
        
        int currentSendBuffer;
        fillSendBuffer(&S,frameBuffer,&currentFrameBuffer,&currentSendBuffer,&msgLength);
        
        int senderSocket;
        if ((senderSocket = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP)) == -1)
        {
            die("socket");
        }
        
        memset((char *) &receiverAddr, 0, sizeof(receiverAddr));
        receiverAddr.sin_family = AF_INET;
        receiverAddr.sin_port = htons(port);
        
        if (inet_aton(destIP , &receiverAddr.sin_addr) == 0)
        {
            fprintf(stderr, "inet_aton() failed\n");
            exit(1);
        }
        
        if( bind(senderSocket , (struct sockaddr*)&receiverAddr, sizeof(receiverAddr) ) == -1)
        {
            die("bind");
        }
        int frameSent = 0;
        int ackReceived = 0;
        while(1) {
            if (currentSendBuffer == 0) {
                break;
            } else if (currentSendBuffer < S.SWS) {
                int sent;
                for (int i=0;i<currentSendBuffer;i++) {
                    if ((sent = sendto(senderSocket, S.sendBuffer[i], 9, 0 , (struct sockaddr *) &receiverAddr, sizeof(receiverAddr)))==-1)
                    {
                        die("sendto()");
                    }
                    sendFrame F;
                    BytesToFrame(&F,S.sendBuffer[i]);
                    S.LFS = getSeqNum(F);
                    frameSent += sent;
                }
                //deleteFromSendBuffer(&S,0,currentSendBuffer,&currentSendBuffer);
                break;
            } else {
                int sent;
                for (int i=0;i<S.SWS;i++) {
                    if ((sent = sendto(senderSocket, S.sendBuffer[i], 9, 0 , (struct sockaddr *) &receiverAddr, sizeof(receiverAddr)))==-1)
                    {
                        die("sendto()");
                    }
                    sendFrame F;
                    BytesToFrame(&F,S.sendBuffer[i]);
                    S.LFS = getSeqNum(F);
                    frameSent += sent;
                }
                int received;
                socklen_t len = sizeof receiverAddr;
                for (int i=0;0<S.SWS;i++) {
                    if ((received = recvfrom(senderSocket, msg, 7, 0, (struct sockaddr *) &receiverAddr,&len)) == -1)
                    {
                        die("recvfrom()");
                    }
                    Packet_ACK A;
                    BytesToAck(&A,msg);
                    printf("%d\n",getNextSeqNum(A));
                    ackReceived += received;
                }
                deleteFromSendBuffer(&S,0,S.SWS-1,&currentSendBuffer);
            }
        }
        int socketStats = shutdown(senderSocket,2);

        return 0;
    }
}
