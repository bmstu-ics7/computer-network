CC := gcc
FLAGS := -std=c99 -Wall -Werror
OBJ_SERVER := error.o convert_number_system.o server.o
OBJ_CLIENT := error.o client.o

%.o: %.c *.h
	$(CC) $(FLAGS) -c $<

all: client server

client: $(OBJ_CLIENT)
	$(CC) $(FLAGS) $^ -o client.out

server: $(OBJ_SERVER)
	$(CC) $(FLAGS) $^ -o server.out

clean:
	rm *.o
