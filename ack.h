// ack.h
//
//
// By Muhammad Rafid Amrullah / 13515125

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef packet_h
#define packet_h

#define Ack_size 7

typedef struct
{
	uint8_t ack;
	unsigned int nextSeqNum;
	uint8_t aws;
	uint8_t checksum;
} Packet_ACK;

// setter

void setACK(Packet_ACK *packet);
void setNextSeqNum(Packet_ACK *packet, unsigned int new);
void setAws(Packet_ACK *packet, uint8_t new);
void setAckChecksum(Packet_ACK *packet, uint8_t new);

// getter

uint8_t getACK(Packet_ACK packet);
int getNextSeqNum(Packet_ACK packet);
uint8_t getAws(Packet_ACK packet);
uint8_t getAckChecksum(Packet_ACK packet);
uint8_t calculateChecksum(Packet_ACK packet);

/* Byte Conversion */
char* ackToByte(Packet_ACK *A);
void BytesToAck(Packet_ACK *A,char* Bytes);
#endif
