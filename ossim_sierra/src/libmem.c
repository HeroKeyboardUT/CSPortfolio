/*
 * Copyright (C) 2025 pdnguyen of HCMC University of Technology VNU-HCM
 */

/* Sierra release
 * Source Code License Grant: The authors hereby grant to Licensee
 * personal permission to use and modify the Licensed Source Code
 * for the sole purpose of studying while attending the course CO2018.
 */

//  #ifdef MM_PAGING
/*
 * System Library
 * Memory Module Library libmem.c 
 */

#include "string.h"
#include "mm.h"
#include "_syscall.h"
#include "libmem.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#ifdef REWORK
int enlist_vm_freerg_list(mm_struct *mm, vm_rg_struct *rg_elmt)
{
  if (rg_elmt->rg_start >= rg_elmt->rg_end)
    return -1;

  vm_rg_struct *curr, *prev, *temp;
  int merged;

  do {
    merged = 0;
    prev = NULL;
    curr = mm->mmap->vm_freerg_list;

    while (curr != NULL) {
      if (curr->rg_end == rg_elmt->rg_start) {
        rg_elmt->rg_start = curr->rg_start;
        if (prev == NULL)
          mm->mmap->vm_freerg_list = curr->rg_next;
        else
          prev->rg_next = curr->rg_next;
        temp = curr;
        curr = curr->rg_next;
        free(temp);
        merged = 1;
        break;
      } else if (curr->rg_start == rg_elmt->rg_end) {
        rg_elmt->rg_end = curr->rg_end;
        if (prev == NULL)
          mm->mmap->vm_freerg_list = curr->rg_next;
        else
          prev->rg_next = curr->rg_next;
        temp = curr;
        curr = curr->rg_next;
        free(temp);
        merged = 1;
        break;
      } else {
        prev = curr;
        curr = curr->rg_next;
      }
    }
  } while (merged);
  vm_rg_struct *new_node = malloc(sizeof(vm_rg_struct));
  if (!new_node)
    return -1;
  new_node->rg_start = rg_elmt->rg_start;
  new_node->rg_end = rg_elmt->rg_end;
  new_node->rg_next = NULL;
  prev = NULL;
  curr = mm->mmap->vm_freerg_list;

  while (curr != NULL && curr->rg_start < new_node->rg_start) {
    prev = curr;
    curr = curr->rg_next;
  }

  if (prev == NULL) {
    new_node->rg_next = mm->mmap->vm_freerg_list;
    mm->mmap->vm_freerg_list = new_node;
  } else {
    new_node->rg_next = curr;
    prev->rg_next = new_node;
  }

  return 0;
}
#else
int enlist_vm_freerg_list(mm_struct *mm, vm_rg_struct *rg_elmt)
{
  vm_rg_struct *rg_node = mm->mmap->vm_freerg_list;
  if (rg_elmt->rg_start >= rg_elmt->rg_end)
    return -1;
  if (rg_node != NULL)
    rg_elmt->rg_next = rg_node;
  mm->mmap->vm_freerg_list = rg_elmt;
  return 0;
}
#endif


vm_rg_struct *get_symrg_byid(mm_struct *mm, int rgid)
{
  if (rgid < 0 || rgid > PAGING_MAX_SYMTBL_SZ)
    return NULL;
  return &mm->symrgtbl[rgid];
}

int liballoc(pcb_t *proc, uint32_t size, uint32_t reg_index)
{
  int addr;
  return __alloc(proc, 0, reg_index, size, &addr);
}
int __alloc(pcb_t *caller, int vmaid, int rgid, int size, int *alloc_addr)
{
  vm_rg_struct rgnode;
  if (get_free_vmrg_area(caller, vmaid, size, &rgnode) == 0)
  {
    caller->mm->symrgtbl[rgid].rg_start = rgnode.rg_start;
    caller->mm->symrgtbl[rgid].rg_end = rgnode.rg_end;

    *alloc_addr = rgnode.rg_start;
  }
  else
  {
  vm_area_struct *cur_vma = get_vma_by_num(caller->mm, vmaid);
  int inc_sz = PAGING_PAGE_ALIGNSZ(size);
  int old_sbrk = cur_vma->sbrk;

  struct sc_regs regs;
  regs.a1 = SYSMEM_INC_OP;
  regs.a2 = vmaid;
  regs.a3 = inc_sz;
  
  _syscall(caller, 17, &regs);

  cur_vma->sbrk = old_sbrk + inc_sz;
  *alloc_addr = old_sbrk;
  
  if (get_free_vmrg_area(caller, vmaid, size, &rgnode) == 0)
  {
    caller->mm->symrgtbl[rgid].rg_start = rgnode.rg_start;
    caller->mm->symrgtbl[rgid].rg_end = rgnode.rg_end;
  }
} 
  caller->regs[rgid] = *alloc_addr;
  printf("===== PHYSICAL MEMORY AFTER ALLOCATION =====\n");
#ifdef IODUMP
  printf("PID=%d - Region=%d - Address=%08lx - Size=%d byte\n", 
         caller->pid, rgid, (unsigned long)*alloc_addr, size);
#ifdef PAGETBL_DUMP
  print_pgtbl(caller, 0, -1);
#endif
#endif
  printf("================================================================\n");
  return 0;
}

int __free(pcb_t *caller, int vmaid, int rgid)
{
  vm_rg_struct* rgnode = get_symrg_byid(caller->mm, rgid);
  if (rgid < 0 || rgid > PAGING_MAX_SYMTBL_SZ || rgnode == NULL)
    return -1;
  caller->regs[rgid] = 0; 
  enlist_vm_freerg_list(caller->mm, rgnode);
#ifdef REWORK
  rgnode->rg_start = 0;
  rgnode->rg_end = 0;
#endif
  printf("===== PHYSICAL MEMORY AFTER DEALLOCATION =====\n");
#ifdef IODUMP
  printf("PID=%d - Region=%d\n", caller->pid, rgid);
#ifdef PAGETBL_DUMP
  print_pgtbl(caller, 0, -1); //print max TBL
#endif
#endif
printf("================================================================\n");
  return 0;
}

int libfree(pcb_t *proc, uint32_t reg_index)
{
  return __free(proc, 0, reg_index);
}

int pg_getpage(mm_struct *mm, int pgn, int *fpn, pcb_t *caller)
{
  uint32_t pte = mm->pgd[pgn];

  if (!PAGING_PAGE_PRESENT(pte))
  {
    int vicpgn, swpfpn; 
    int vicfpn;
    uint32_t vicpte;
    find_victim_page(caller->mm, &vicpgn);
    MEMPHY_get_freefp(caller->active_mswp, &swpfpn);

    vicpte = mm->pgd[vicpgn];
    vicfpn = PAGING_PTE_FPN(vicpte);

    struct sc_regs regs;
    regs.a1 = SYSMEM_SWP_OP;
    regs.a2 = vicfpn;
    regs.a3 = swpfpn;

    _syscall(caller, 17, &regs);
    pte_set_swap(&vicpte, caller->active_mswp_id, swpfpn);
    mm->pgd[vicpgn] = vicpte;

    pte_set_fpn(&pte, vicfpn);          
    mm->pgd[pgn] = pte;
  }

  *fpn = PAGING_FPN(mm->pgd[pgn]);

  return 0;
}

int pg_setval(mm_struct *mm, int addr, BYTE value, pcb_t *caller)
{
  int pgn = PAGING_PGN(addr);
  int off = PAGING_OFFST(addr);
  int fpn;
  
  if (pg_getpage(mm, pgn, &fpn, caller) != 0)
    return -1;

  int phyaddr = fpn * PAGING_PAGESZ + off;
  struct sc_regs regs;
  regs.a1 = SYSMEM_IO_WRITE;
  regs.a2 = phyaddr;
  regs.a3 = value;

  _syscall(caller, 17, &regs);
  return 0;
}

int pg_getval(mm_struct *mm, int addr, BYTE *data, pcb_t *caller)
{
  int pgn = PAGING_PGN(addr);
  int off = PAGING_OFFST(addr);
  int fpn;

  if (pg_getpage(mm, pgn, &fpn, caller) != 0)
    return -1;

  int phyaddr = fpn * PAGING_PAGESZ + off;
  struct sc_regs regs;
  regs.a1 = SYSMEM_IO_READ;
  regs.a2 = phyaddr;
  regs.a3 = *data;

  _syscall(caller, 17, &regs);
  *data = (BYTE)(regs.a3);
  return 0;
}

int __write(pcb_t *caller, int vmaid, int rgid, int offset, BYTE value)
{
  vm_rg_struct *currg = get_symrg_byid(caller->mm, rgid);
  vm_area_struct *cur_vma = get_vma_by_num(caller->mm, vmaid);

  if (currg == NULL || cur_vma == NULL) /* Invalid memory identify */
    return -1;

  pg_setval(caller->mm, currg->rg_start + offset, value, caller);

  return 0;
}

int libwrite(
    struct pcb_t *proc,  
    BYTE data,           
    uint32_t destination,
    uint32_t offset)
{
  int val = __write(proc, 0, destination, offset, data);
  if(val == -1)
    return -1;

  printf("===== PHYSICAL MEMORY AFTER WRITING =====\n");
#ifdef IODUMP
  printf("write PID=%d region=%d offset=%d value=%d\n",proc->pid, destination, offset, data);
#ifdef PAGETBL_DUMP
  print_pgtbl(proc, 0, -1); 
#endif
  MEMPHY_dump(proc->mram);
  printf("================================================================\n");
#endif
  return 0;

}

int __read(pcb_t *caller, int vmaid, int rgid, int offset, BYTE *data)
{
  vm_rg_struct *currg = get_symrg_byid(caller->mm, rgid);
  vm_area_struct *cur_vma = get_vma_by_num(caller->mm, vmaid);

  if (currg == NULL || cur_vma == NULL) /* Invalid memory identify */
    return -1;

  pg_getval(caller->mm, currg->rg_start + offset, data, caller);

  return 0;
}

int libread(
    pcb_t *proc, 
    uint32_t source,    
    uint32_t offset,  
    uint32_t* destination)
{
  BYTE data;
  int val = __read(proc, 0, source, offset, &data);
 *destination = data;

  printf("===== PHYSICAL MEMORY AFTER READING =====\n"); 
#ifdef IODUMP
  printf("read PID=%d region=%d offset=%d value=%d\n", proc->pid, source, offset, data);
#ifdef PAGETBL_DUMP
  print_pgtbl(proc, 0, -1); //print max TBL
#endif
  MEMPHY_dump(proc->mram);
  printf("================================================================\n");
#endif
  return val;
}

int libread_inkill(
    pcb_t *proc, 
    uint32_t source,    
    uint32_t offset,  
    uint32_t* destination)
{
  BYTE data;
  int val = __read(proc, 0, source, offset, &data);
  *destination = data;
  return val;
}

int free_pcb_memph(pcb_t *caller)
{
  int pagenum, fpn;
  uint32_t pte;


  for(pagenum = 0; pagenum < PAGING_MAX_PGN; pagenum++)
  {
    pte= caller->mm->pgd[pagenum];

    if (!PAGING_PAGE_PRESENT(pte))
    {
      fpn = PAGING_PTE_FPN(pte);
      MEMPHY_put_freefp(caller->mram, fpn);
    } else {
      fpn = PAGING_PTE_SWP(pte);
      MEMPHY_put_freefp(caller->active_mswp, fpn);    
    }
  }

  return 0;
}

int find_victim_page(mm_struct *mm, int *retpgn)
{
  if (mm->fifo_pgn == NULL) {
    printf("Error: No pages in FIFO queue to evict.\n");
    return -1;
  }
  pgn_t *pg = mm->fifo_pgn;
  pgn_t *prev_pg = NULL;
  while(pg != NULL) {
    prev_pg = pg;
    pg = pg->pg_next;
  }
  *retpgn = prev_pg->pgn;
  free(prev_pg);

  return 0;
}

int get_free_vmrg_area(pcb_t *caller, int vmaid, int size, vm_rg_struct *newrg)
{
  vm_area_struct *cur_vma = get_vma_by_num(caller->mm, vmaid);

  vm_rg_struct *rgit = cur_vma->vm_freerg_list;

  if (rgit == NULL)
    return -1;

  newrg->rg_start = newrg->rg_end = -1;
  while (rgit != NULL)
  {
    if (rgit->rg_end - rgit->rg_start >= size)
    {
      newrg->rg_start = rgit->rg_start;
      newrg->rg_end = rgit->rg_start + size;
      newrg->rg_next = NULL;
      rgit->rg_start = newrg->rg_end;

      return 0;
    }
    rgit = rgit->rg_next;
  }
  return -1;
}
// #endif


 