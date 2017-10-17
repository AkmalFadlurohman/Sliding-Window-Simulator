//
//  sendPacket.h
//  
//
//  Created by Akmal Fadlurohman on 10/16/17.
//
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef sendFrame_h
#define sendFrame_h

#define sendFrame_size 9

typedef struct {
    uint8_t SOH;
    unsigned int  seqNum;
    uint8_t STX;
    uint8_t data;
    uint8_t ETX;
    unsigned int checksum;
} sendFrame;

/* Initialization Method */
void initFrame(sendFrame *F);
void BytesToFrame(sendFrame *F,char* Bytes);

/* Getter Method */
unsigned int getSeqNum(sendFrame F);
uint8_t getData(sendFrame F);
unsigned int getChecksum(sendFrame F);

/* Setter Method */
void setSeqNum(sendFrame *F,unsigned int seqNum);
void setData(sendFrame *F,char c);
void setCheckSum(sendFrame *F,unsigned int checksum);

/* Frame and Byte Conversion Method */
char* sendFrameToByte(sendFrame *F);
void printBytes(char* Bytes);
/* Other Method */
unsigned int checkSum(sendFrame F);

#endif
