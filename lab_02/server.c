#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "error.h"
#include "convert_number_system.h"

#define PORT 8080
#define SIZE_BUFFER 100
int sockfd;

void close_app()
{
    if (close(sockfd) < 0)
        exit(error());

    printf("\nSocket closed\n");
    exit(0);
}

int main()
{
    struct sockaddr_in socket_port;
    struct sockaddr_in recv_name;
    socklen_t recv_len;
    char buffer[SIZE_BUFFER];
    char result[SIZE_BUFFER];
    ssize_t size;

    signal(SIGINT, close_app);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
        return error();

    memset(&socket_port, 0, sizeof(socket_port));
    memset(&recv_name, 0, sizeof(recv_name));

    socket_port.sin_family = AF_INET;
    socket_port.sin_addr.s_addr = INADDR_ANY;
    socket_port.sin_port = htons(PORT);

    bind(sockfd, (const struct sockaddr *)&socket_port, sizeof(socket_port));

    if (errno != 0)
        return error();

    printf("Socket is successfully opened\n");
    printf("Press Ctrl+C to close this application\n");

    while (1)
    {
        size = recvfrom(
            sockfd,
            buffer,
            sizeof(buffer),
            MSG_WAITALL,
            (struct sockaddr *)&recv_name,
            &recv_len
        );

        if (size < 0)
            return error();

        buffer[size - 1] = '\0';

        if (is_number(buffer))
        {
            printf("_____________________________\n");
            printf("Getted number - %s\n", buffer);

            convert_number_system(result, buffer, 2);
            printf("2 -  %s\n", result);

            convert_number_system(result, buffer, 8);
            printf("8 -  %s\n", result);

            convert_number_system(result, buffer, 16);
            printf("16 - %s\n", result);

            convert_number_system(result, buffer, 19);
            printf("19 - %s\n", result);
        }
        else
        {
            printf("_____________________________\n");
            printf("Getted message is not a number - %s\n", buffer);
        }
    }

    return 0;
}
