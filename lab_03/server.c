#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <signal.h>
#include "queue.h"

#define COUNT_ARG_ERROR (-1)
#define SERVER_BACKLOG 1
#define BUFSIZE 255
#define MAX_LEN 4096
#define THREAD_POOL_SIZE 4
#define REQUESTSIZE 128

typedef struct sockaddr SA;
typedef struct sockaddr_in SA_IN;

struct request {
    char *method;
    char *filename;
    char *username;
};

int server_socket;
pthread_t thread_pool[THREAD_POOL_SIZE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_file = PTHREAD_MUTEX_INITIALIZER;
queue_t *clients_queue;

void check_error(const char *name_function);
void close_server(int code);
struct request parse_request(char *request_message);
void handle_connection(int client_socket);
void *thread_function(void *arg);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        return COUNT_ARG_ERROR;
    }

    int port = atoi(argv[1]);
    check_error("atoi");

    signal(SIGINT, close_server);

    clients_queue = malloc(sizeof(queue_t));

    for (int i = 0; i < THREAD_POOL_SIZE; ++i) {
        pthread_create(&thread_pool[i], NULL, thread_function, NULL);
    }

    int client_socket;
    socklen_t addr_size;
    SA_IN server_addr, client_addr;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    check_error("socket");

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    bind(server_socket, (SA*)&server_addr, sizeof(server_addr));
    check_error("bind");

    listen(server_socket, SERVER_BACKLOG);
    check_error("listen");

    printf("server listen on port %d\n", port);
    printf("click Ctrl+C to close the server\n");

    while (true) {
        addr_size = sizeof(SA_IN);
        client_socket = accept(server_socket, (SA*)&client_addr, &addr_size);
        check_error("accept");

        int *pclient = malloc(sizeof(int));
        *pclient = client_socket;

        pthread_mutex_lock(&mutex);
        queue_push(clients_queue, pclient);
        pthread_mutex_unlock(&mutex);
    }

    return 0;
}

void check_error(const char *name_function) {
    if (errno != 0) {
        fprintf(stderr, "%s: %d\n", name_function, errno);
        exit(errno);
    }
}

void close_server(int code) {
    close(server_socket);
    check_error("close");

    queue_free(&clients_queue);

    printf("server closed\n");
    exit(0);
}

struct request parse_request(char *request_message) {
    struct request request;
    request.method = strtok(request_message, " ");
    request.filename = strtok(NULL, " ");
    memmove(request.filename, request.filename + 1, strlen(request.filename));

    do {
        request.username = strtok(NULL, " \n\r");
    } while (strcmp(request.username, "User-Agent:") && request.username != NULL);
    request.username = strtok(NULL, "\n\r");

    return request;
}

void *thread_function(void *arg) {
    while (true) {
        pthread_mutex_lock(&mutex);
        int *client_socket = queue_pop(clients_queue);
        pthread_mutex_unlock(&mutex);

        if (client_socket != NULL) {
            handle_connection(*client_socket);
        }
    }
}

void handle_connection(int client_socket) {
    char buffer[BUFSIZE];
    char request_buffer[BUFSIZE];
    char *content = calloc(MAX_LEN, 1);
    char *all_content = calloc(MAX_LEN, 1);
    char *not_found_msg = "<h1>404 Not Found</h1>\n";
    size_t message_size;
    int code = 0;

    printf("\n===\n");

    message_size = read(client_socket, request_buffer, BUFSIZE);
    check_error("read");

    buffer[message_size - 1] = 0;
    printf("request: %s\n", request_buffer);

    struct request request = parse_request(request_buffer);
    printf("username: %s\n", request.username);

    if (strcmp(request.method, "GET") == 0) {
        FILE *file = fopen(request.filename, "r");
        if (file == NULL) {
            errno = 0;
            printf("connection error: open file %s\n", request.filename);
            sprintf(
                all_content,
                "HTTP/1.1 404 Not Found\r\n"
                "connection: closed\r\n"
                "content-type: text/html\r\n"
                "content-length: %lu\r\n\r\n"
                "%s\n", strlen(not_found_msg), not_found_msg
            );
            code = 404;
        } else {
            do {
                message_size = fread(buffer, 1, BUFSIZE, file);
                buffer[message_size - 1] = 0;

                if (message_size > 0)
                    strcat(content, buffer);
            } while(message_size > 0);

            fclose(file);

            sprintf(
                all_content,
                "HTTP/1.1 200 OK\r\n"
                "connection: closed\r\n"
                "content-type: text/html\r\n"
                "content-length: %lu\r\n\r\n"
                "%s", strlen(content), content
            );
            code = 200;
        }
    } else {
        sprintf(
            all_content,
            "HTTP/1.1 404 Not Found\r\n"
            "connection: closed\r\n"
            "content-type: text/html\r\n"
            "content-length: %lu\r\n\r\n"
            "%s\n", strlen(not_found_msg), not_found_msg
        );
        code = 404;
    }

    write(client_socket, all_content, strlen(all_content));
    free(content);
    free(all_content);

    pthread_mutex_lock(&mutex_file);
    FILE *log = fopen("users.log", "a");
    fprintf(log, "%s : %s (%d)\n",
        request.username,
        request.filename,
        code
    );
    fclose(log);
    pthread_mutex_unlock(&mutex_file);

    printf("closing connection\n");
}
