//
//  sendPacket.c
//  
//
//  Created by Akmal Fadlurohman on 10/16/17.
//
//

#include "sendFrame.h"

/* Initialization Method */
void initFrame(sendFrame *F) {
    F->SOH = 0x1;
    F->STX = 0x2;
    F->ETX = 0x3;
}
void BytesToFrame(sendFrame *F,char* Bytes) {
    F->SOH = Bytes[0];
    unsigned int seqNum = (unsigned int) ((unsigned char)(Bytes[1] << 24) |
                              (unsigned char)(Bytes[2] << 16) |
                              (unsigned char)(Bytes[3] << 8) |
                              (unsigned char)(Bytes[4]));
    setSeqNum(F,seqNum);
    F->STX = Bytes[5];
    setData(F,Bytes[6]);
    F->ETX = Bytes[7];
    unsigned int checksum = (unsigned int) Bytes[8];
    setCheckSum(F,checksum);
}

/* Getter Method */
unsigned int getSeqNum(sendFrame F) {
    return F.seqNum;
}
uint8_t getData(sendFrame F) {
    return F.data;
}
unsigned int getChecksum(sendFrame F) {
    return F.checksum;
}

/* Setter Method */
void setSeqNum(sendFrame *F,unsigned int seqNum) {
    F->seqNum = seqNum;
}

void setData(sendFrame *F,char data) {
    F->data = (uint8_t) data;
}

void setCheckSum(sendFrame *F,unsigned int checksum) {
    F->checksum = checksum;
}

/* Frame and Byte Conversion Method */
char* sendFrameToByte(sendFrame *F) {
    char* Bytes = (char *) malloc(sendFrame_size);
    Bytes[0] = F->SOH;
    unsigned int seqNum = F->seqNum;
    Bytes[1] = (seqNum >> 24) & 0xFF;
    Bytes[2] = (seqNum >> 16) & 0xFF;
    Bytes[3] = (seqNum >> 8) & 0xFF;
    Bytes[4] = seqNum & 0xFF;
    Bytes[5] = F->STX;
    Bytes[6] = F->data;
    Bytes[7] = F->ETX;
    unsigned int checksum = F->checksum;
    Bytes[8] = checksum & 0xFF;
    return Bytes;
}
void printBytes(char* Bytes) {
    for(int i = 0; i < sendFrame_size; i++) {
        printf("%02hhX ", Bytes[i]);
    }
    printf("\n");
}

/* Other Method */
unsigned int checkSum(sendFrame F) {
    return ((F.SOH)+F.seqNum+(F.STX)+(F.data)+(F.ETX));
}


