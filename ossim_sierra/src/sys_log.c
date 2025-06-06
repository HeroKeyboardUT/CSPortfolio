#include "common.h"
#include "_syscall.h"
#include "stdio.h"

int __sys_log(pcb_t *caller)
{
    int size = caller->code->size;
    int i;
    for(i = 0; i < size; i++)
    {
        if(caller->code->text[i].opcode == ALLOC)
        {
            printf("Instruction %d: Allocating %d bytes\n", i, caller->code->text[i].arg_0);
        }
        else if(caller->code->text[i].opcode == FREE)
        {
            printf("Instruction %d: Freeing\n", i);
        }
        else if(caller->code->text[i].opcode == READ)
        {
            printf("Instruction %d: Reading\n", i);
        }
        else if(caller->code->text[i].opcode == WRITE)
        {
            printf("Instruction %d: Writing %d\n", i, caller->code->text[i].arg_0);
        }
        else if(caller->code->text[i].opcode == SYSCALL)
        {
            printf("Instruction %d: System call with args %d %d %d %d\n", i, caller->code->text[i].arg_0,
             caller->code->text[i].arg_1, caller->code->text[i].arg_2, caller->code->text[i].arg_3);
        }
        else
        {
            printf("Instruction %d: Unknown operation\n", i);
        }
    }
    return 0;
}