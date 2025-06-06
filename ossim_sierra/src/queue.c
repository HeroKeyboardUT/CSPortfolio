#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int empty(queue_t * q) {
        if (q == NULL) return 1;
	return (q->size == 0);
}

void enqueue(queue_t * q, pcb_t * proc) {
        if (q == NULL || proc == NULL) {
                fprintf(stderr, "Queue or process is NULL\n");
                return;
        }
        if(q->size == MAX_QUEUE_SIZE) {
                return;
        }
        q->proc[q->size] = proc;
        q->size++;
}

pcb_t * dequeue(queue_t * q) {
        if (empty(q)) return NULL;
        int highest_pos = 0;
        for (int i = 1; i < q->size; i++) {
                if(q->proc[i]->priority < q->proc[highest_pos]->priority) highest_pos = i;
        }
        struct pcb_t* pcb_return = q->proc[highest_pos]; 
        for (int i = highest_pos; i < q->size-1; i++) {
          q->proc[i] = q->proc[i+1];
        }
        q->size--;
	return pcb_return;
}

