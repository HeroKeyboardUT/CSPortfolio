
#ifndef QUEUE_H
#define QUEUE_H

#include "common.h"

#define MAX_QUEUE_SIZE 10

typedef struct queue_t {
	pcb_t * proc[MAX_QUEUE_SIZE];
	int size;
} queue_t;

void enqueue(queue_t * q, pcb_t * proc);

pcb_t * dequeue(queue_t * q);

int empty(queue_t * q);

#endif

