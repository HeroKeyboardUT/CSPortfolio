// From src/sys_xxxhandler.c
#include "common.h"
#include "_syscall.h"
#include "stdio.h"
#include "unistd.h"
/*SYSTEM FOR PAUSING*/
int __sys_sleep(pcb_t *caller, struct sc_regs *regs)
{
    int time = regs->a1;
    if (time < 0) {
        printf("Invalid pause time: %d\n", time);
        return -1;
    }
    printf("Process %d is pausing for %d s\n", caller->pid, time);
    sleep(time);
    return 0;
}