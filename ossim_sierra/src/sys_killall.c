// /*
//  * Copyright (C) 2025 pdnguyen of HCMC University of Technology VNU-HCM
//  */

// /* Sierra release
//  * Source Code License Grant: The authors hereby grant to Licensee
//  * personal permission to use and modify the Licensed Source Code
//  * for the sole purpose of studying while attending the course CO2018.
//  */

// #include "common.h"
// #include "syscall.h"
// #include "stdio.h"
// #include "queue.h"
// #include <pthread.h>
// #include <string.h>
// #include <stdlib.h>
// static pthread_mutex_t queue_lock;
// /*
// killall sends a signal to all processes running the specified command(s).
// The command or program names are specified by a memory region associated with REGIONID.

// If no signal is specified, the default terminate signal is sent.

// killall is an incomplete system call that requires further implementation
// by the student to terminate the execution of all processes whose program
// name matches the string(s) stored in REGIONID.
// */
// int __sys_killall(pcb_t *caller, sc_regs* regs)
// {
//     char proc_name[100];
//     uint32_t data;

//     //hardcode for demo only
//     uint32_t memrg = regs->a1;
    
//     /* TODO: Get name of the target proc */
//     //proc_name = libread..
//     int i = 0;
//     data = 0;
//     while(data != -1){
//         libread(caller, memrg, i, &data);
//         proc_name[i]= data;
//         if(data == -1) {
//             proc_name[i]='\0';
//             break;
//         }
//         i++;
//     }
//     printf("The procname retrieved from memregionid %d is \"%s\"\n", memrg, proc_name);

//     /* TODO: Traverse proclist to terminate the proc
//      *       stcmp to check the process match proc_name
//      */
//     pcb_t *proc = NULL;
//     // Traverse the running list
//     pthread_mutex_lock(&queue_lock);
//     for (int i = 0; i < caller->running_list->size; i++) {
//         proc = caller->running_list->proc[i];
//         if (strcmp(proc->path, proc_name) == 0) {
//             printf("Terminating process %d with name \"%s\"\n", proc->pid, proc->path);
//             proc->pc = proc->code->size;
//         }
//     }
//     pthread_mutex_unlock(&queue_lock);
//     #ifdef MLQ_SCHED
//     // Traverse the MLQ ready queue for each priority level
//     for (int prio = 0; prio < MAX_PRIO; prio++) 
//     {
//         pthread_mutex_lock(&queue_lock);
//         for (int i = 0; i < caller->mlq_ready_queue[prio].size; i++) {
//             proc = caller->mlq_ready_queue[prio].proc[i];
//             if (strcmp(proc->path, proc_name) == 0) {
//                 printf("Terminating process %d with name \"%s\" from priority %d queue\n", proc->pid, proc->path, prio);
//                 proc->pc = proc->code->size;
//             } 
//         }
//         pthread_mutex_unlock(&queue_lock);
//     }
//     #endif
//     return 0; 
// }
/*
 * Copyright (C) 2025 pdnguyen of HCMC University of Technology VNU-HCM
 */

/* Sierra release
 * Source Code License Grant: The authors hereby grant to Licensee
 * personal permission to use and modify the Licensed Source Code
 * for the sole purpose of studying while attending the course CO2018.
 */

 #include "common.h"
 #include "_syscall.h"
 #include "stdio.h"
 #include <string.h>
 #include "libmem.h"
 #include "queue.h"
 #include "pthread.h"

 static pthread_mutex_t queue_lock ;
 
 void remove_proc_from_list(struct queue_t *queue, struct pcb_t *proc ) {
     if (queue == NULL || proc == NULL) return;
     int found = 0;
     for (int i = 0; i < queue->size; i++) {
         if (queue->proc[i] == proc) {
             found = 1;
             for (int j = i; j < queue->size - 1; j++) {
                 queue->proc[j] = queue->proc[j + 1];
             }
 
             queue->proc[queue->size - 1] = NULL; 
             queue->size--; 
             break;
         }
     }
     if (found) {
         printf("Process PID %d removed from queue.\n", proc->pid);
     } else {
         printf("Process not found in queue.\n");
     }
 }
 
 int __sys_killall(struct pcb_t *caller, struct sc_regs* regs)
 {
     char proc_name[100];
     uint32_t data;
     uint32_t memrg = regs->a1;
 
     int i = 0;
     data = 0;
     while (data != -1) {
         libread_inkill(caller, memrg, i, &data);
         proc_name[i] = data;
         if (data == -1) proc_name[i] = '\0';
         i++;
     }
     printf("The procname retrieved from memregionid %d is \"%s\"\n", memrg, proc_name);
 
     struct pcb_t *proc = NULL;
 
     pthread_mutex_lock(&queue_lock);
     for (int i = 0; i < caller->running_list->size; ) {
         proc = caller->running_list->proc[i];
         char *last_slash = strrchr(proc->path, '/');
         char *current_name = last_slash ? last_slash + 1 : proc->path;
 
         if (strcmp(current_name, proc_name) == 0) {
             printf("Terminating process %d with name \"%s\"\n", proc->pid, proc->path);
             proc->pc = proc->code->size;
             remove_proc_from_list(caller->running_list, proc); 
             continue;
         }
         i++;
     }
     pthread_mutex_unlock(&queue_lock);
 
     #ifdef MLQ_SCHED
     for (int prio = 0; prio < MAX_PRIO; prio++) {
         pthread_mutex_lock(&queue_lock);
         for (int i = 0; i < caller->mlq_ready_queue[prio].size; ) {
             proc = caller->mlq_ready_queue[prio].proc[i];
             char *last_slash = strrchr(proc->path, '/');
             char *current_name = last_slash ? last_slash + 1 : proc->path;
 
             if (strcmp(current_name, proc_name) == 0) {
                 printf("Terminating process %d with name \"%s\" from priority %d queue\n", proc->pid, proc->path, prio);
                 proc->pc = proc->code->size;
                 remove_proc_from_list(&caller->mlq_ready_queue[prio], proc);
                 continue; 
             }
             i++;
         }
         pthread_mutex_unlock(&queue_lock);
     }
     #endif
 
     return 0; 
 }
 