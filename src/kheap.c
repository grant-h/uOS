#include <kheap.h>

#include <kerror.h>
#include <print.h>

uint8  initialized = 0;
uint32 initialBase = 0x0;
uint32 currentBase = 0x0;
uint32 maxBase = 0x0;

/* kmalloc_early_xxx
 * At this point, all we have is a location of free memory
 * but no paging. No paging means that there is literally no point
 * in handing out memory addresses when we want to enforce memory
 * boundaries through paging.
 *
 * These early functions are just used to bootstrap early code until
 * the point when paging and the real heap can be enabled.
 */

// TODO: reliably choose a free spot in memory
void kmalloc_early_init(uint32 base, uint32 max)
{
  ASSERT(base < max);

  printf("kmalloc_early [%p, %p]\n", base, max);
  initialBase = currentBase = base;
  maxBase = max;
  initialized = 1;
}

/* Returns true on a finding a free memory region. 
 * Uses this to initialize kmalloc_early
 * TODO: depreciate this and just make a function to get a list
 * of memory regions by availability
 */
int kmalloc_early_init_grub(struct multiboot_info * mbi)
{
  if(mbi == NULL)
    return 0;

  if(mbi->flags & MULTIBOOT_INFO_MEM_MAP)
  {
    uint32 * mem_info_ptr = (uint32 *)mbi->mmap_addr; 

    while(mem_info_ptr < (uint32 *)mbi->mmap_addr+mbi->mmap_length)
    {
      multiboot_memory_map_t * cur = (multiboot_memory_map_t *)mem_info_ptr;   

      // choose the first memory location we find. don't worry about the size yet
      if(cur->type == MULTIBOOT_MEMORY_AVAILABLE && cur->len > 0) 
      {
        if(cur->addr == 0x0 && cur->len > PAGE_SIZE)
        {
          cur->addr += PAGE_SIZE;
          cur->len -= PAGE_SIZE;
        }

        kmalloc_early_init(cur->addr, cur->addr+cur->len);
        //kmalloc_early_init(0x0 KERNEL_END, 0xC0000000 + KERNEL_END+0x10000);
        return 1;
      }

      // move to the next structure
      mem_info_ptr += cur->size + sizeof(cur->size);
    }
  }

  return 0;
}

uint32 kmalloc_early(uint32 size)
{
  if(!initialized)
    panic("kmalloc_early: no base address initialized\n");

  if(!size)
    return NULL;

  if(size+currentBase > maxBase)
    panic("kmalloc_early: %u byte allocation caused us to run out of memory (%u bytes left)\n", size, maxBase - currentBase);
  

  uint32 addr = currentBase;

  currentBase += size;

  return addr;
}

uint32 kmalloc_early_align(uint32 size)
{
  if(!initialized)
    panic("kmalloc_early_align: no base address initialized\n");

  // prevent page alignment if the allocation size is zero
  if(!size)
    return NULL;


  // is our placement address not page aligned?
  if(currentBase & ~PAGE_ALIGN)
  {
    currentBase &= PAGE_ALIGN;
    currentBase += PAGE_SIZE;
  }

  // just call the regular early kmalloc, which will act on the page 
  // aligned placement address
  return kmalloc_early(size);
}
