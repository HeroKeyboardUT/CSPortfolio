
#include "cpu.h"
#include "mem.h"
#include "mm.h"
#include "syscall.h"
#include "libmem.h"

#include <pthread.h>
static pthread_mutex_t queue_lock;

int calc(struct pcb_t *proc)
{
	return ((unsigned long)proc & 0UL);
}

int alloc(pcb_t *proc, uint32_t size, uint32_t reg_index)
{
	addr_t addr = alloc_mem(size, proc);
	if (addr == 0)
	{
		return 1;
	}
	else
	{
		proc->regs[reg_index] = addr;
		return 0;
	}
}

int free_data(pcb_t *proc, uint32_t reg_index)
{
	return free_mem(proc->regs[reg_index], proc);
}

int read(
	pcb_t *proc, // Process executing the instruction
	uint32_t source,	// Index of source register
	uint32_t offset,	// Source address = [source] + [offset]
	uint32_t destination)
{ // Index of destination register

	BYTE data;
	if (read_mem(proc->regs[source] + offset, proc, &data))
	{
		proc->regs[destination] = data;
		return 0;
	}
	else
	{
		return 1;
	}
}

int write(
	pcb_t *proc,	// Process executing the instruction
	BYTE data,		// Data to be wrttien into memory
	uint32_t destination, // Index of destination register
	uint32_t offset)
{ // Destination address =
	// [destination] + [offset]
	return write_mem(proc->regs[destination] + offset, proc, data);
}

int run(pcb_t *proc)
{
	/* Check if Program Counter point to the proper instruction */
	if (proc->pc >= proc->code->size)
	{
		return 1;
	}

	inst_t ins = proc->code->text[proc->pc];
	proc->pc++;
	int stat = 1;
switch (ins.opcode)
	{
	case CALC:
		stat = calc(proc);
		break;
	case ALLOC:
#ifdef MM_PAGING
		pthread_mutex_lock(&queue_lock);
		stat = liballoc(proc, ins.arg_0, ins.arg_1);
		pthread_mutex_unlock(&queue_lock);
#else
		stat = alloc(proc, ins.arg_0, ins.arg_1);
#endif
		break;
	case FREE:
#ifdef MM_PAGING
		pthread_mutex_lock(&queue_lock);
		stat = libfree(proc, ins.arg_0);
		pthread_mutex_unlock(&queue_lock);
#else
		stat = free_data(proc, ins.arg_0);
#endif
		break;
	case READ:
#ifdef MM_PAGING
		pthread_mutex_lock(&queue_lock);
		stat = libread(proc, ins.arg_0, ins.arg_1, &ins.arg_2);
		pthread_mutex_unlock(&queue_lock);
#else
		stat = read(proc, ins.arg_0, ins.arg_1, ins.arg_2);
#endif
		break;
	case WRITE:
#ifdef MM_PAGING
		pthread_mutex_lock(&queue_lock);
		stat = libwrite(proc, ins.arg_0, ins.arg_1, ins.arg_2);
		pthread_mutex_unlock(&queue_lock);
#else
		stat = write(proc, ins.arg_0, ins.arg_1, ins.arg_2);
#endif
		break;
	case SYSCALL:
		pthread_mutex_lock(&queue_lock);
		stat = libsyscall(proc, ins.arg_0, ins.arg_1, ins.arg_2, ins.arg_3);
		pthread_mutex_unlock(&queue_lock);
		break;
	default:
		stat = 1;
	}
	return stat;
}
