#include "queue.h"
#include <stdlib.h>

queue queue_create() {
    queue q = (queue) malloc(sizeof(struct __queue));
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    return q;
}

void queue_emplace(queue q, triple value) {
    queue_node newNode = (queue_node) malloc(sizeof(struct __queue_node));
    newNode->value = value;
    newNode->next = NULL;

    if (q->front == NULL) {
        q->front = newNode;
        q->rear = newNode;
        q->size = 1;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
        (q->size)++;
    }
}

triple queue_front(queue q) {
    if (q->size) return q->front->value;
    return (triple) {-1, -1, -1};
}

void queue_pop(queue q) {
    if (q->front == NULL) return;
    if (q->front->next == NULL) {
        free(q->front);
        q->front = NULL;
        q->rear = NULL;
        q->size = 0;
        return;
    }

    queue_node temp = q->front->next;
    free(q->front);
    q->front = temp;
    (q->size)--;
}

void queue_destroy(queue q) {
    while (q->size) {
        queue_pop(q);
    }
}