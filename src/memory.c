#include <common.h>
#include <kerror.h> // assert
#include <kheap.h>
#include <print.h>
#include <string.h>

#include <memory.h>

// initialize the KERNEL_* convience variables
// allows us forget about using & all the time
uint32 KERNEL_START = (uint32)&_KERNEL_START;
uint32 KERNEL_END = (uint32)&_KERNEL_END;

// stored Page Directory
static uint32 * pageDirectory = 0;

static void _native_set_page_directory(uint32 * phyDir);
static void _native_paging_enable();
static void _native_paging_disable();

void paging_init()
{
  uint32 i;
  uint32 addr = 0;

  // 1024 page table entries (pointers)
  pageDirectory = (uint32 *)kmalloc_early_align(PAGE_TABLE_SIZE);

  printf("paging: directory at %p\n", pageDirectory);

  // initialize all the page tables to not present, rw, supervisor
  memset(pageDirectory, 0, PAGE_TABLE_SIZE);

  // create our first page table. use it to identity map low-mem
  // TODO: and place our kernel in the higher half

  for(i = 0; i < PAGES_PER_MB(2); i++)
  {
    page_ident_map(addr, PAGE_ENTRY_RW);
    addr += PAGE_SIZE;
  }

  // move our kernel to the higher half
  uint32 virtBase = 0xC0000000;
  uint32 virtEnd = PAGE_ALIGN_UP((KERNEL_END-KERNEL_START)+virtBase);

  for(addr = KERNEL_START; virtBase < virtEnd; virtBase += PAGE_SIZE, addr += PAGE_SIZE)
    page_map(virtBase, addr, PAGE_ENTRY_RW);

  // set our directory and enable paging
  _native_set_page_directory(pageDirectory);
  _native_paging_enable();

  //printf("%p\n", *(uint32 *)(KERNEL_START));
  //printf("%p\n", *(uint32 *)(0xc0000000));
}

/* A direct mapping between the virtual and physical realm 
 */
uint32 page_ident_map(uint32 addr, uint32 perm)
{
  return page_map(addr, addr, perm);
}

/* Allocates a mapping between the requested virtual address
 * and the physical address, using the requested permissions.
 * Returns the address of the PTE
 */
uint32 page_map(uint32 virt, uint32 phys, uint32 perm)
{
  ASSERT(!(virt & NOT_ALIGNED || phys & NOT_ALIGNED));
  ASSERT(pageDirectory);

  //printf("page_map: %p -> %p\n", virt, phys);

  uint32 index = virt / PAGE_SIZE;
  uint32 pageDirI = (index / 1024) % 1024;
  uint32 pageTableI = index % 1024;

  // if the page table isn't present, create it
  if(!(pageDirectory[pageDirI] & PAGE_ENTRY_PRESENT))
  {
    uint32 * pageTable = (uint32 *)kmalloc_early_align(PAGE_TABLE_SIZE);
    int i;

    // initialize all the page table entries to permissions 
    for(i = 0; i < PAGE_ENTRIES; i++)
      pageTable[i] = perm & PAGE_ALIGN;

    // clear out the page table address, but keep the flags
    pageDirectory[pageDirI] &= ~PAGE_ALIGN;

    // add the page table to the directory and mark it as present
    // make sure to OR the assignment to preserve the flags
    pageDirectory[pageDirI] |= (uint32)pageTable | PAGE_ENTRY_PRESENT;

    //printf("page_map: table %u created at %p\n", pageDirI, pageTable);
  }

  // get our page table address
  uint32 * pageTable = (uint32 *)(pageDirectory[pageDirI] & PAGE_ALIGN);

  // clear out the page table entry, but keep the flags
  pageTable[pageTableI] &= ~PAGE_ALIGN;
  // OR on our physical address
  pageTable[pageTableI] |= phys; 

  // mark the entry as present
  pageTable[pageTableI] |= PAGE_ENTRY_PRESENT;

  //printf("page_map: entry %u present at %p\n", pageTableI, (pageTable + pageTableI));

  return (uint32)&pageTable[pageTableI];
}

static inline void _native_set_page_directory(uint32 * phyDir)
{
  asm volatile("mov %0, %%cr3":: "b"(phyDir));
}

static inline void _native_paging_enable()
{
  uint32 cr0;

  asm volatile("mov %%cr0, %0": "=b"(cr0));
  cr0 |= 0x80000000;
  asm volatile("mov %0, %%cr0":: "b"(cr0)); // brace yourself
}

static inline void _native_paging_disable()
{
  uint32 cr0;

  asm volatile("mov %%cr0, %0": "=b"(cr0));
  cr0 &= ~(0x80000000U);
  asm volatile("mov %0, %%cr0":: "b"(cr0));
}
