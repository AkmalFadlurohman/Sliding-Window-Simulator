CC = gcc
CFLAGS  = -c

all: sendfile receivefile

sendfile: sendFile.o sender.o sendFrame.o
	$(CC) -o sendfile sendFile.o sender.o sendFrame.o 

receivefile: rcv.o ack.o sendFrame.o
	$(CC) -o receivefile rcv.o ack.o sendFrame.o 
	rm -f *.o

sendFile.o: sendFile.c
	$(CC) $(CFLAGS) sendFile.c

sender.o: sender.c
	$(CC) $(CFLAGS) sender.c

sendFrame1.o: sendFrame.c
	$(CC) $(CFLAGS) sendFrame.c

rcv.o: rcv.c
	$(CC) $(CFLAGS) rcv.c

ack.o: ack.c
	$(CC) $(CFLAGS) ack.c