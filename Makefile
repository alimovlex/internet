CC = gcc
CFLAGS = -g -Wall
all: server client

server: server.c
	$(CC) $(CFLAGS) -o server server.c
client:
	$(CC) $(CFLAGS) -o client client.c
clean:
	rm -f server client
