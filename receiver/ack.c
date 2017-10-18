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