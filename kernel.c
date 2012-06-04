#include <common.h>
#include <print.h>
#include <boot/multiboot.h>
#include <kprint.h>
#include <isr.h>

void kmain(void* mbd, unsigned int magic)
{
  init_desc_tables(); //get exception handling up quickly!

  clear_screen();

  if(magic != MULTIBOOT_BOOTLOADER_MAGIC)
  {
    printf("Kernel was not loaded using a multiboot compliant bootloader. Cannot continue" \
           " reliably without multiboot information. Halting...\n");
    return;
  }


  printf("[TestKernel INIT]\n\nVideo memory: %p\n", VID_MEM);
  printf("Using standard 80x25 vga text mode\n");
  
  struct multiboot_info * mbi = (struct multiboot_info *)mbd; //initialize our multiboot information structure

  if(ISSET(MULTIBOOT_INFO_MEMORY, mbi->flags)) 
  {
    unsigned int total_mem = mbi->mem_lower + mbi->mem_upper; 
    printf("Total Memory %d KiB: mem_lower = %d KiB, mem_upper = %d KiB\n", total_mem, mbi->mem_lower, mbi->mem_upper);
  }

  if(ISSET(MULTIBOOT_INFO_MEM_MAP, mbi->flags))
  {
    unsigned int * mem_info_ptr = (unsigned int *)mbi->mmap_addr; 

    printf("GRUB Memory Map\n");

    while(mem_info_ptr < (unsigned int *)mbi->mmap_addr+mbi->mmap_length)
    {
      multiboot_memory_map_t * cur = (multiboot_memory_map_t *)mem_info_ptr;   

      if(cur->len > 0)
        printf("  [%p-%p %5s]\n", (uint32)cur->addr, (uint32)(cur->addr+cur->len), cur->type == MULTIBOOT_MEMORY_AVAILABLE ? "AVAIL" : "RESVD");

      mem_info_ptr += cur->size + sizeof(cur->size);
    }
  }
  //asm volatile ("int $8");  
}
