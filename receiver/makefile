CC = gcc
CFLAGS  = -c

receivefile: rcv.o ack.o sendFrame.o
	$(CC) -o receivefile rcv.o ack.o sendFrame.o 
	rm -f *.o

rcv.o: rcv.c
	$(CC) $(CFLAGS) rcv.c

ack.o: ack.c
	$(CC) $(CFLAGS) ack.c

sendFrame.o: sendFrame.c
	$(CC) $(CFLAGS) sendFrame.c