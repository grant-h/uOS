#include <common.h>
#include "kheap.h"

#include <print.h>

uint32 earlyBase = 0x0;

/* kmalloc_early_xxx
 * At this point, all we have is a location of free memory
 * but no paging. No paging means that there is literally no point
 * in handing out memory addresses when we want to enforce memory
 * boundaries through paging.
 *
 * These early functions are just used to bootstrap early code until
 * the point when paging and the real heap can be enabled.
 */

void kmalloc_early_init(uint32 base)
{
  //printf("kmalloc_early base %p\n", base);
  earlyBase = base;
}

uint32 kmalloc_early(uint32 size)
{
  uint32 addr = earlyBase;

  earlyBase += size;

  return addr;
}

uint32 kmalloc_early_align(uint32 size)
{
  // is our placement address not page aligned?
  // mask off everything except the lower 0xfff
  if(earlyBase & ~PAGE_ALIGN)
  {
    earlyBase &= PAGE_ALIGN;
    earlyBase += PAGE_SIZE;
  }

  // just call the regular early kmalloc, which will act on the page 
  // aligned placement address
  return kmalloc_early(size);
}
