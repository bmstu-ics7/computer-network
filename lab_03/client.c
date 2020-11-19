#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define COUNT_ARG_ERROR (-1)
#define BUFSIZE 255
#define HEADSIZE 255
#define RECVSIZE 4096

typedef struct sockaddr SA;
typedef struct sockaddr_in SA_IN;

void check_error(const char *name_function);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <servername> <port>\n", argv[0]);
        return COUNT_ARG_ERROR;
    }

    struct hostent *server = gethostbyname(argv[1]);
    check_error("gethostname");

    int port = atoi(argv[2]);
    check_error("atoi");

    SA_IN server_addr;
    int server_socket;
    char *buffer = NULL;
    char *send_buffer = NULL;
    char server_buffer[RECVSIZE];

    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    check_error("socket");

    server_addr.sin_family = AF_INET;
    memcpy(
        &server_addr.sin_addr,
        server->h_addr_list[0],
        server->h_length
    );
    server_addr.sin_port = htons(port);

    connect(server_socket, (SA*)&server_addr, sizeof(server_addr));
    check_error("connect");

    printf("Type file:\n");
    buffer = malloc(BUFSIZE);
    fgets(buffer, BUFSIZE, stdin);
    buffer[strlen(buffer) - 1] = 0;

    send_buffer = malloc(HEADSIZE);
    sprintf(send_buffer,
        "GET /%s HTTP/1.1\r\n"
        "Host: %s:%s\r\n"
        "User-Agent: Client#%d\r\n\r\n",
        buffer, argv[1], argv[2], getpid()
    );

    write(server_socket, send_buffer, strlen(send_buffer));
    memset(server_buffer, 0, RECVSIZE);
    read(server_socket, server_buffer, RECVSIZE);

    printf("%s\n", server_buffer);

    close(server_socket);
    check_error("close");

    free(buffer);
    free(send_buffer);

    return 0;
}

void check_error(const char *name_function) {
    if (errno != 0) {
        fprintf(stderr, "%s: %d\n", name_function, errno);
        exit(errno);
    }
}
