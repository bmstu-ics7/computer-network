CC := gcc
FLAGS := -std=c99 -Wall -Werror
SERVER := queue.o server.o
CLIENT := client.o

%.o: %.c *.h
	$(CC) $(FLAGS) -c $<

all: server.out client.out

server.out: $(SERVER)
	$(CC) $(FLAGS) -o $@ $^

client.out: $(CLIENT)
	$(CC) $(FLAGS) -o $@ $^
