#include <common.h>
#include <print.h>
#include <boot/multiboot.h>
#include <kprint.h>
#include <isr.h>

void kmain(void* mbd, unsigned int magic)
{
  clear_screen();

  for(;;) ;

  if(magic != MULTIBOOT_BOOTLOADER_MAGIC)
  {
    printf("Kernel was not loaded using a multiboot compliant bootloader. Cannot continue" \
           " reliably without multiboot information. Halting...\n");
    return;
  }

  init_desc_tables(); //get exception handling up quickly!

  printf("[TestKernel INIT]\n\nVideo memory: 0x%x\n", VID_MEM);
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

    while(mem_info_ptr < (unsigned int *)mbi->mmap_addr+mbi->mmap_length)
    {
      multiboot_memory_map_t * cur = (multiboot_memory_map_t *)mem_info_ptr;   

      unsigned int addr_low = cur->addr & 0xFFFFFFFF;
      unsigned int len_low = cur->len & 0xFFFFFFFF;

      printf("0x%x - 0x%x    %s,  ", addr_low, addr_low+len_low, cur->type == MULTIBOOT_MEMORY_AVAILABLE ? "AVAIL" : "RESV");

      mem_info_ptr += cur->size + sizeof(cur->size);
    }
  }
  //asm volatile ("int $8");  
}
