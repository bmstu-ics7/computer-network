#ifndef __QUEUE_H
#define __QUEUE_H

#include <stdlib.h>

typedef struct list list_t;
typedef struct queue queue_t;

struct list {
    list_t *next;
    int *client_socket;
};

struct queue {
    list_t *head;
    list_t *tail;
};

void queue_push(queue_t *queue , int *client_socket);
int *queue_pop(queue_t *queue);
void queue_free(queue_t **queue);

#endif // __QUEUE_H
