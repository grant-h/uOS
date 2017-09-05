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
uint32 EARLY_KMALLOC_START = (uint32)&_EARLY_KMALLOC_START;
uint32 EARLY_KMALLOC_END = (uint32)&_EARLY_KMALLOC_END;

// stored Page Directory
static uint32 * pageDirectory = NULL;
// A reverse mapping from physical to virtual
static uint32 ** ptPhysToVirt = NULL;
// Guard against multiple enables of paging and for 
// virtual to physical translation
static int kernelPagingEnabled = 0;

static void page_map_pte(uint32 * pt, uint32 index, uint32 phys, uint32 perm);
static uint32 * _native_get_page_directory();
static void _native_set_page_directory(uint32 * phyDir);
static void _native_paging_enable();
static void _native_paging_disable();

uint32 virt_to_phys(uint32 virt_addr)
{
  uint32 * pd = NULL;
  uint32 * pt = NULL;
  uint32 pde = 0;

  uint32 index = virt_addr / PAGE_SIZE;
  uint32 pageDirI = (index / 1024) % 1024;
  uint32 pageTableI = index % 1024;

  // we are referencing our virtual memory alloc'd page directory
  if(kernelPagingEnabled) {
    pd = pageDirectory; // virtual address
    pde = (uint32)ptPhysToVirt[pageDirI] |
      (uint32)pd[pageDirI] & PAGE_ENTRY_PRESENT; // virtual address with flags
    pt = (uint32*)(pde & PAGE_ALIGN);
  } else {
    pd = _native_get_page_directory(); // identity mapped physical address
    pde = (uint32)pd[pageDirI]; // identity mapped physical address
    pt = (uint32*)(pde & PAGE_ALIGN);
  }

  // TODO: make this more robust
  ASSERT(pde & PAGE_ENTRY_PRESENT);
  ASSERT(pt[pageTableI] & PAGE_ENTRY_PRESENT);

  // return just physical address without flags
  return pt[pageTableI] & PAGE_ALIGN;
}

void paging_init()
{
  uint32 i;
  uint32 addr = 0;

  // 1024 page table entries (pointers)
  pageDirectory = (uint32 *)kmalloc_early_align(PAGE_TABLE_SIZE);
  ptPhysToVirt = (uint32 **)kmalloc_early_align(PAGE_TABLE_SIZE);

  // initialize all the page tables to not present, rw, supervisor
  memset(pageDirectory, 0, PAGE_TABLE_SIZE);
  // initialize reverse mappings
  memset(ptPhysToVirt, 0, PAGE_TABLE_SIZE);

  uint32 * oldDir = _native_get_page_directory();
  printf("paging: PTD at %p (old %p)\n",
      virt_to_phys((uint32)pageDirectory), oldDir);

  // Identity map the 1 megabyte
  for(i = 0; i < PAGES_PER_MB(1); i++) {
    page_ident_map(addr, PAGE_ENTRY_RW);
    addr += PAGE_SIZE;
  }

  // move our kernel to the higher half
  uint32 virtBase = 0xC0000000;
  uint32 virtStart = KERNEL_START;
  uint32 virtEnd = PAGE_ALIGN_UP(KERNEL_END);

  for(addr = virtStart-virtBase; virtStart < virtEnd; virtStart += PAGE_SIZE, addr += PAGE_SIZE) {
    page_map(virtStart, addr, PAGE_ENTRY_RW);
  }

  // set our directory and enable paging
  _native_set_page_directory(virt_to_phys(pageDirectory));
  _native_paging_enable();
  kernelPagingEnabled = 1;
}

/* A direct mapping between the virtual and physical realm 
 */
uint32 page_ident_map(uint32 addr, uint32 perm)
{
  return page_map(addr, addr, perm);
}

void page_ident_map_range(uint32 start, uint32 end, uint32 perm)
{
  ASSERT(start < end);

  end = PAGE_ALIGN_UP(end);

  for(; start < end; start += PAGE_SIZE)
    page_map(start, start, perm);
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

    // Clear all physical addresses and flags
    memset(pageTable, 0, PAGE_TABLE_SIZE);

    // Add the page table to the directory and mark it as present
    // NOTE: PDE's MUST have a physical address
    pageDirectory[pageDirI] = virt_to_phys((uint32)pageTable)
      | PAGE_ENTRY_PRESENT | PAGE_ENTRY_RW;

    printf("page_map: table %u created at %p (%p)\n",
        pageDirI, pageTable, pageDirectory[pageDirI]);

    // store the virtual address of the PTE
    ptPhysToVirt[pageDirI] = pageTable;
  }

  // load our virtual PT address from our reverse mapping
  uint32 * pageTable = ptPhysToVirt[pageDirI];
  page_map_pte(pageTable, pageTableI, phys, perm);

  return (uint32)&pageTable[pageTableI];
}

static void page_map_pte(uint32 * pt, uint32 index, uint32 phys, uint32 perm)
{
  ASSERT(pt);
  ASSERT(index < PAGE_ENTRIES);

  // clear out the page table entry, but keep the flags
  pt[index] &= ~PAGE_ALIGN;

  // OR on our physical address
  pt[index] |= phys;

  // Add permissions
  pt[index] |= perm;

  // mark the entry as present
  pt[index] |= PAGE_ENTRY_PRESENT;
}

static inline void _native_set_page_directory(uint32 * phyDir)
{
  asm volatile("mov %0, %%cr3":: "b"(phyDir));
}

static inline uint32 * _native_get_page_directory()
{
  uint32 ret;
  asm volatile("mov %%cr3, %0": "=b" (ret));

  return (uint32 *)ret;
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
