#include "queue.h"

void queue_push(queue_t *queue , int *client_socket) {
    list_t *head = malloc(sizeof(list_t));

    head->next = NULL;
    head->client_socket = client_socket;

    if (queue->head == NULL) {
        queue->head = queue->tail = head;
    } else {
        queue->head->next = head;
    }

    queue->head = head;
}

int *queue_pop(queue_t *queue) {
    if (queue == NULL || queue->tail == NULL) {
        return NULL;
    }

    list_t *tail = queue->tail;
    int *ret = tail->client_socket;
    queue->tail = tail->next;
    free(tail);

    if (queue->tail == NULL) {
        queue->head = NULL;
    }

    return ret;
}

void queue_free(queue_t **queue) {
    if (*queue == NULL) {
        return;
    }

    list_t *curr = (*queue)->tail;
    list_t *next = NULL;

    while (curr) {
        next = curr->next;
        free(curr->client_socket);
        free(curr);
        curr = next;
    }

    free(*queue);
    *queue = NULL;
}
