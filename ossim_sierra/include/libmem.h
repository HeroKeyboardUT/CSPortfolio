/*
 * Copyright (C) 2025 pdnguyen of HCMC University of Technology VNU-HCM
 */

/* Sierra release
 * Source Code License Grant: The authors hereby grant to Licensee
 * personal permission to use and modify the Licensed Source Code
 * for the sole purpose of studying while attending the course CO2018.
 */

#include "common.h"

#define SYSMEM_MAP_OP 1
#define SYSMEM_INC_OP 2
#define SYSMEM_SWP_OP 3
#define SYSMEM_IO_READ 4
#define SYSMEM_IO_WRITE 5

extern vm_area_struct *get_vma_by_num(mm_struct *mm, int vmaid);
int inc_vma_limit(pcb_t*, int, int);
int __mm_swap_page(pcb_t*, int, int);
int liballoc(pcb_t *, uint32_t, uint32_t);
int libfree(pcb_t *, uint32_t);
int libread(pcb_t*, uint32_t, uint32_t, uint32_t*);
int libwrite(pcb_t*, BYTE, uint32_t, uint32_t);
int libsyscall(pcb_t*, uint32_t, uint32_t, uint32_t, uint32_t);
int libread_inkill(pcb_t*, uint32_t, uint32_t, uint32_t*);
