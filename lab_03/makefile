CC := gcc
FLAGS := -std=c99 -Wall -Werror
SERVER := queue.o server.o

%.o: %.c *.h
	$(CC) $(FLAGS) -c $<

server.out: $(SERVER)
	$(CC) $(FLAGS) -o $@ $^
