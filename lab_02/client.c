#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "error.h"

#define PORT 8080
#define SIZE_BUFFER 100

int sockfd;

void close_app()
{
    if (close(sockfd) < 0)
        exit(error());

    exit(0);
}

int main()
{
    size_t len;
    struct sockaddr_in socket_srvr;
    char *buffer;

    signal(SIGINT, close_app);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
        return error();

    memset(&socket_srvr, 0, sizeof(socket_srvr));

    socket_srvr.sin_family = AF_INET;
    socket_srvr.sin_port = htons(PORT);
    socket_srvr.sin_addr.s_addr = INADDR_ANY;

    while(1)
    {
        printf("Type number: ");
        getline(&buffer, &len, stdin);

        sendto(
            sockfd,
            buffer,
            strlen(buffer),
            0,
            (struct sockaddr *)&socket_srvr,
            sizeof(socket_srvr)
        );
    }

    return 0;
}
