#ifndef __QUEUE_H__
#define __QUEUE_H__

typedef struct __triple {
    int first, second, third;
} triple;

struct __queue_node {
    triple value;
    struct __queue_node *next;
};

typedef struct __queue_node* queue_node;

struct __queue {
    queue_node front;
    queue_node rear;
    int size;
};

typedef struct __queue* queue;

queue queue_create();

void queue_emplace(queue q, triple value);

triple queue_front(queue q);

void queue_pop(queue q);

void queue_destroy(queue q);

#endif // __QUEUE_H__
