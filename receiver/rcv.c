// receiver.c
//
//
// By Muhammad Rafid Amrullah / 13515125

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "ack.h"
#include "sendFrame.h"

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

void sendACK(sendFrame F){
    Packet_ACK A;
    setACK(&A);
    setNextSeqNum(&A, F.seqNum + 1);
    setAws(&A,0x1);
    setAckChecksum(&A,calculateChecksum(A));

    printf("ACK Header : %#x | ", A.ack);
    printf("NextSN: %d | ", A.nextSeqNum);
    printf("AWS: %#x | ", A.aws);
    printf("Checksum: %x\n", A.checksum);
}

int main(int argc, char *argv[]){

    sendFrame *F = malloc(3*sizeof(sendFrame));

    for(int i = 0; i < 3; i++){
        initFrame(&(F[i]));
        setSeqNum(&(F[i]),i);
        if(i==0){
            setData(&(F[i]),'a');
        }else if(i==1){
            setData(&(F[i]),'l');
        }else{
            setData(&(F[i]),'o');
        }
        setCheckSum(&(F[i]),checkSum(F[i]));
    }
    
    for(int i = 0; i < 3; i++){
        if(validChecksum(F[i],checkSum(F[i]))){
            sendACK(F[i]);
        }
    }

    int rws = strToInt(argv[2]);
   	int recvBufferSize = strToInt(argv[3]);
    int port = strToInt(argv[4]);

    printf("%d %d %d\n", rws, recvBufferSize, port);

	return 0;
}