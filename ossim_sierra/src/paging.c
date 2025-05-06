
#include "mem.h"
#include "cpu.h"
#include "loader.h"
#include <stdio.h>

int main() {
	pcb_t * ld = load("input/p0");
	pcb_t * proc = load("input/p0");
	unsigned int i;
	for (i = 0; i < proc->code->size; i++) {
		run(proc);
		run(ld);
	}
	dump();
	return 0;
}

