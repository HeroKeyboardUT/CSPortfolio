
#include "queue.h"
#include "sched.h"
#include <pthread.h>

#include <stdlib.h>
#include <stdio.h>
static queue_t ready_queue;
static queue_t run_queue;
static pthread_mutex_t queue_lock;

static queue_t running_list;
#ifdef MLQ_SCHED
static queue_t mlq_ready_queue[MAX_PRIO];
static int slot[MAX_PRIO];
#endif

int queue_empty(void) {
#ifdef MLQ_SCHED
	unsigned long prio;
	for (prio = 0; prio < MAX_PRIO; prio++)
		if(!empty(&mlq_ready_queue[prio])) 
			return -1;
#endif
	return (empty(&ready_queue) && empty(&run_queue));
}

void init_scheduler(void) {
#ifdef MLQ_SCHED
    int i ;

	for (i = 0; i < MAX_PRIO; i++) {
		mlq_ready_queue[i].size = 0;
		slot[i] = MAX_PRIO - i; 
	}
#endif
	ready_queue.size = 0;
	run_queue.size = 0;
	pthread_mutex_init(&queue_lock, NULL);
}

#ifdef MLQ_SCHED
pcb_t * get_mlq_proc(void) {
	pcb_t * proc = NULL;
    pthread_mutex_lock(&queue_lock);
	for (int round = 0; round < 2; round++) { 
        for (int i = 0; i < MAX_PRIO; i++) {
            if (slot[i] > 0) {
                proc = dequeue(&mlq_ready_queue[i]);
                if (proc != NULL) {
                    slot[i]--;
                    pthread_mutex_unlock(&queue_lock);
                    return proc;
                }
            }
        }

        for (int i = 0; i < MAX_PRIO; i++) {
            slot[i] = MAX_PRIO - i;
        }
    }
	pthread_mutex_unlock(&queue_lock);
    return proc;
}

void put_mlq_proc(pcb_t * proc) {
	pthread_mutex_lock(&queue_lock);
	enqueue(&mlq_ready_queue[proc->prio], proc);
	pthread_mutex_unlock(&queue_lock);
}

void add_mlq_proc(pcb_t * proc) {
	pthread_mutex_lock(&queue_lock);
	enqueue(&mlq_ready_queue[proc->prio], proc);
	pthread_mutex_unlock(&queue_lock);	
}

pcb_t * get_proc(void) {
	return get_mlq_proc();
}

void put_proc(pcb_t * proc) {
	proc->ready_queue = &ready_queue;
	proc->mlq_ready_queue = mlq_ready_queue;
	proc->running_list = & running_list;

	pthread_mutex_lock(&queue_lock);
    enqueue(&running_list, proc); 
    pthread_mutex_unlock(&queue_lock);

	return put_mlq_proc(proc);
}

void add_proc(pcb_t * proc) {
	proc->ready_queue = &ready_queue;
	proc->mlq_ready_queue = mlq_ready_queue;
	proc->running_list = & running_list;

	pthread_mutex_lock(&queue_lock);
	enqueue(&running_list, proc);
	pthread_mutex_unlock(&queue_lock);

	return add_mlq_proc(proc);
}
#else
pcb_t * get_proc(void) {
	pcb_t * proc = NULL;
	/*TODO: get a process from [ready_queue].
	 * Remember to use lock to protect the queue.
	 * */
	pthread_mutex_lock(&queue_lock);
	proc = dequeue(&ready_queue);
	if (proc == NULL) {
		proc = dequeue(&run_queue);
	}
	pthread_mutex_unlock(&queue_lock);
	if (proc == NULL) {
		return NULL;
	}
	proc->ready_queue = &ready_queue;
	proc->running_list = & running_list;
	return proc;
}

void put_proc(pcb_t * proc) {
	proc->ready_queue = &ready_queue;
	proc->running_list = & running_list;

	/* TODO: put running proc to running_list */

	pthread_mutex_lock(&queue_lock);
	enqueue(&run_queue, proc);
	pthread_mutex_unlock(&queue_lock);
}

void add_proc(pcb_t * proc) {
	proc->ready_queue = &ready_queue;
	proc->running_list = & running_list;

	/* TODO: put running proc to running_list */

	pthread_mutex_lock(&queue_lock);
	enqueue(&ready_queue, proc);
	pthread_mutex_unlock(&queue_lock);	
}
#endif



