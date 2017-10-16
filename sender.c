//
//  sender.c
//  
//
//  Created by Akmal Fadlurohman on 10/16/17.
//
//
#include <stdlib.h>
#include <sys/socket.h>
#include "sendPacket.h"

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
        
        
        int SWS = strToInt(argv[2]);
        int sendBufferSize = strToInt(argv[3]);
        char* destIP = (char*) malloc(sizeof(argv[4]));
        char* destPort = (char*) malloc(sizeof(argv[5]));
        
        char* msgFileName = (char*) malloc(sizeof(argv[1]));
        strcpy(msgFileName,argv[1]);
        FILE *msgFile = fopen(msgFileName,"r");
        if (msgFile == NULL) {
            printf("Error cannot open the specified file\n");
            return 0;
        }
        char* msgBuffer =  malloc(1*sizeof(char));
        msgBuffer[0] = '\0';
        int msgBufferSize = 0;
        int flag = 0;
        while (flag != 1)
        {
            char c;
            fscanf(msgFile,"%c",&c);
            if (c == '.') {
                flag = 1;
            } else {
                msgBuffer[msgBufferSize] = c;
                char* temp = malloc((msgBufferSize+1)*sizeof(char));
                strcpy(temp,msgBuffer);
                free(msgBuffer);
                msgBuffer = malloc((msgBufferSize+1)*sizeof(char));
                for (int i=0; i <= msgBufferSize-1; i++) {
                    msgBuffer[i] = temp[i];
                }
                msgBufferSize++;
                free(temp);
            }
        }
        //char* sendBuffer = (char*) malloc(sizeof(sendBufferSize));
        
        strcpy(destIP,argv[4]);
        strcpy(destPort,argv[5]);
        
        printf("%d\n",SWS);
        printf("%d\n",sendBufferSize);
        printf("%lu\n",sizeof(msgBuffer));//Size);
        printf("%s\n",destIP);
        printf("%s\n",destPort);
        printf("%s\n",msgBuffer);
        fclose(msgFile);
        
    }
    return 0;
}
