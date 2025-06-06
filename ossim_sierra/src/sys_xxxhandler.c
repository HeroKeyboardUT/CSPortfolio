#include "common.h"
#include "_syscall.h"
#include "stdio.h"

/*SYSTEM FOR BOOTING*/
int __sys_xxxhandler(pcb_t *caller, struct sc_regs *regs)
{
    printf("The first system call parameter %d\n", regs->a1);
    return 0;
}
