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

int libsyscall (pcb_t *caller,
             uint32_t syscall_idx,
             uint32_t a1,
             uint32_t a2,
             uint32_t a3)
{
   struct sc_regs regs;

   regs.a1 = a1;
   regs.a2 = a2;
   regs.a3 = a3;
   printf("libsyscall: syscall_idx = %d, a1 = %d, a2 = %d, a3 = %d\n",
          syscall_idx, a1, a2, a3);
   return _syscall(caller, syscall_idx, &regs);
}
