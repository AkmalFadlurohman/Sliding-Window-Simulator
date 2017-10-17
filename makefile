CC = gcc
CFLAGS  = -c

sendfile: sendFile.o sender.o sendFrame.o
	$(CC) -o sendfile sendFile.o sender.o sendFrame.o 

sendFile.o: sendFile.c
	$(CC) $(CFLAGS) sendFile.c

sender.o: sender.c
	$(CC) $(CFLAGS) sender.c

sendFrame.o: sendFrame.c
	$(CC) $(CFLAGS) sendFrame.c

clean:
	rm -f *.o sendfile