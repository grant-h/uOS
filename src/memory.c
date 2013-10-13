#include <common.h>
#include <kerror.h> // assert
#include <kheap.h>

#include <memory.h>

// initialize the KERNEL_* convience variables
// allows us forget about using address of all the time
uint32 KERNEL_START = &_KERNEL_START;
uint32 KERNEL_END = &_KERNEL_END;

static uint32 * pageDirectory = 0;

void paging_init()
{
  uint32 i;

  // 1024 page table entries (pointers)
  pageDirectory = (uint32 *)kmalloc_early_align(1024);

  printf("paging: directory at %p\n", pageDirectory);

  // initialize all the page tables to not present, rw, supervisor
  for(i = 0; i < 1024; i++)
    pageDirectory[i] = PAGE_ENTRY_RW;

  // create our first page table. use it to identity map low-mem (<1 MB)
  // TODO: and place our kernel in the higher half
  
  // number of pages in 3 MB
  uint32 toMap = PAGES_PER_MB(3);
  uint32 addr = 0;

  for(i = 0; i < toMap; i++)
  {
    page_ident_map(addr);
    addr += PAGE_SIZE;
  }

  unsigned int cr0;
  asm volatile("mov %0, %%cr3":: "b"(pageDirectory));
  asm volatile("mov %%cr0, %0": "=b"(cr0));
  cr0 |= 0x80000000;
  asm volatile("mov %0, %%cr0":: "b"(cr0)); // brace yourself
}

/* A direct mapping between the virtual and physical realm 
 */
uint32 page_ident_map(uint32 addr)
{
  return page_map(addr, addr);
}

/* Allocates a mapping between the requested virtual address
 * and the physical address. Returns the address of the PTE
 */
uint32 page_map(uint32 virt, uint32 phys)
{
  ASSERT(!(virt & NOT_ALIGNED || phys & NOT_ALIGNED));
  ASSERT(pageDirectory);

  uint8 pageDirI = virt / (1024*1024);
  uint8 pageTableI = virt / 1024;

  // if the page table isn't present, create it
  if(!(pageDirectory[pageDirI] & PAGE_ENTRY_PRESENT))
  {
    uint32 * pageTable = (uint32 *)kmalloc_early_align(1024);
    int i;

    // initialize all the page table entries to not present, rw, supervisor
    for(i = 0; i < 1024; i++)
      pageTable[i] = PAGE_ENTRY_RW;

    // clear out the page table address, but keep the flags
    pageDirectory[pageDirI] &= ~PAGE_ALIGN;

    // add the page table to the directory and mark it as present
    // make sure to OR the assignment to preserve the flags
    pageDirectory[pageDirI] |= *pageTable | PAGE_ENTRY_PRESENT;
  }

  // get our page table address
  uint32 * pageTable = (uint32 *)(pageDirectory[pageDirI] & PAGE_ALIGN);

  // clear out the page table entry, but keep the flags
  pageTable[pageTableI] &= ~PAGE_ALIGN;
  // OR on our physical address
  pageTable[pageTableI] |= phys; 

  // if the entry wasn't present, great! just mark it as present and move on
  if((pageTable[pageTableI] & PAGE_ENTRY_PRESENT))
    pageTable[pageTableI] |= PAGE_ENTRY_PRESENT;

  return (uint32)&pageTable[pageTableI];
}




