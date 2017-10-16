CC = gcc
CFLAGS  = -c -Wall


sendfile: sender.o sendPacket.o
	$(CC) -o sendfile sender.o sendPacket.o 

sender.o: sender.c
	$(CC) $(CFLAGS) sender.c

sendPacket.o: sendPacket.c
	$(CC) $(CFLAGS) sendPacket.c

clean:
	rm -rf *o SlidingWindow a.out