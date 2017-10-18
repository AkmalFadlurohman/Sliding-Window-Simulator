// receiver.c
//
//
// By Muhammad Rafid Amrullah / 13515125

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "ack.h"
#include "sendFrame.h"

/* Global */
struct sockaddr_in senderAddr,receiverAddr;

int strToInt(char* str) {
    int numb = str[0] - '0';
    for (int i=1;i<strlen(str);i++) {
        int temp = numb;
        temp *= 10;
        numb = temp + (str[i] - '0');
    }
    return numb;
}

int validChecksum(sendFrame F, uint8_t rcv_sum){
    return (F.checksum == rcv_sum)?1:0;
}

void createACK(sendFrame F,Packet_ACK* A) {
    setACK(A);
    setNextSeqNum(A, F.seqNum + 1);
    setAws(A,0x1);
    setAckChecksum(A,calculateChecksum(A));
}

void die(char *s)
{
    perror(s);
    exit(1);
}

int main(int argc, char *argv[]){
    
    int rws = strToInt(argv[2]);
    int recvBufferSize = strToInt(argv[3]);
    int port = strToInt(argv[4]);

    sendFrame *F = malloc(recvBufferSize*sizeof(sendFrame));
    
    senderAddr.sin_family = AF_INET;
    senderAddr.sin_port = htons(port);
    senderAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    int receiverSocket;
    if ((receiverSocket=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }
    if( bind(receiverSocket , (struct sockaddr*)&senderAddr, sizeof(senderAddr) ) == -1)
    {
        die("bind");
    }
    char *msg = malloc(sendFrame_size*sizeof(char));
    
    int recv_len;
    socklen_t len = sizeof receiverAddr;
    while(1) {
        if ((recv_len = recvfrom(receiverSocket, msg, 9, 0, (struct sockaddr *) &receiverAddr,&len)) == -1)
        {
            die("recvfrom()");
        }
        sendFrame F;
        BytesToFrame(&F,msg);
        Packet_ACK A;
        createACK(F,&A);
        if ((sent = sendto(receiverSocket,ackToByte(&A), 7, 0 , (struct sockaddr *) &senderAddr, sizeof(senderAddr)))==-1)
        {
            die("sendto()");
        }
    }
    int socketStats = shutdown(receiverSocket,2);

	return 0;
}
