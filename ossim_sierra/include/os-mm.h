#ifndef OSMM_H
#define OSMM_H


//#define MM_PAGING
#define PAGING_MAX_MMSWP 4 /* max number of supported swapped space */
#define PAGING_MAX_SYMTBL_SZ 30

typedef char BYTE;
typedef uint32_t addr_t;
//typedef unsigned int uint32_t;

typedef struct pgn_t {
   int pgn;
   struct pgn_t *pg_next; 
} pgn_t;

/*
 * Memory region struct
 */
typedef struct vm_rg_struct {
   unsigned long rg_start;
   unsigned long rg_end;

   struct vm_rg_struct *rg_next;
} vm_rg_struct;

/*
 * Memory area struct
 */
typedef struct vm_area_struct {
   unsigned long vm_id;
   unsigned long vm_start;
   unsigned long vm_end;

   unsigned long sbrk;
   /*
    * Derived field
    * unsigned long vm_limit = vm_end - vm_start
    */
   struct mm_struct *vm_mm;
   struct vm_rg_struct *vm_freerg_list;
   struct vm_area_struct *vm_next;
} vm_area_struct;

/* 
 * Memory mapping struct
 */
typedef struct mm_struct {
   uint32_t *pgd;

   vm_area_struct *mmap;

   /* Currently we support a fixed number of symbols */
   vm_rg_struct symrgtbl[PAGING_MAX_SYMTBL_SZ];

   /* List of free pages */
   pgn_t *fifo_pgn;
} mm_struct;

/*
 * FRAME/MEM PHY struct
 */
typedef struct framephy_struct { 
   int fpn;
   struct framephy_struct *fp_next;

   /* Reserved for tracking allocated frames */
   mm_struct* owner;
} framephy_struct;

typedef struct memphy_struct {
   /* Basic field of data and size */
   BYTE *storage;
   int maxsz;
   
   /* Sequential device fields */ 
   int rdmflg;
   int cursor;

   /* Management structure */
   framephy_struct *free_fp_list;
   framephy_struct *used_fp_list;
} memphy_struct;


#endif
