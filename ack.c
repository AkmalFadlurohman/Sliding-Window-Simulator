// ack.c
//
//
// By Muhammad Rafid Amrullah / 13515125

#include "ack.h"

// setter

void setACK(Packet_ACK *packet){
	packet->ack = 0x6;
}

void setNextSeqNum(Packet_ACK *packet, unsigned int new){
	packet->nextSeqNum = new;
}

void setAws(Packet_ACK *packet, uint8_t new){
	packet->aws = new;
}

void setAckChecksum(Packet_ACK *packet, uint8_t new){
	packet->checksum = new;
}

// getter

uint8_t getACK(Packet_ACK packet){
	return packet.ack;
}

int getNextSeqNum(Packet_ACK packet){
	return packet.nextSeqNum;
}

uint8_t getAws(Packet_ACK packet){
	return packet.aws;
}

uint8_t getAckChecksum(Packet_ACK packet){
	return packet.checksum;
}

uint8_t calculateChecksum(Packet_ACK packet){
	return (uint8_t) ((packet.ack) + packet.nextSeqNum + (packet.aws));
}

char* ackToByte(Packet_ACK *A) {
    char* Bytes = (char *) malloc(sendFrame_size);
    Bytes[0] = A->ack;
    unsigned int seqNum = A->nextSeqNum;
    Bytes[1] = (seqNum >> 24) & 0xFF;
    Bytes[2] = (seqNum >> 16) & 0xFF;
    Bytes[3] = (seqNum >> 8) & 0xFF;
    Bytes[4] = seqNum & 0xFF;
    Bytes[5] = A->aws;
    Bytes[6] = A->checksum;
    return Bytes;
}
void BytesToAck(Packet_ACK *A,char* Bytes) {
    A->ack = Bytes[0];
    unsigned int seqNum = (unsigned int) ((unsigned char)(Bytes[1] << 24) |
                                          (unsigned char)(Bytes[2] << 16) |
                                          (unsigned char)(Bytes[3] << 8) |
                                          (unsigned char)(Bytes[4]));
    setNextSeqNum(A,seqNum);
    A->aws = Bytes[5];
    A->checksum = Bytes[6];
}
