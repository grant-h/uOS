#include <common.h>
#include <print.h>
#include <boot/multiboot.h>
#include <kerror.h>
#include <kprint.h>
#include <isr.h>

void banner();

void kmain(void* mbd, unsigned int magic)
{
  clear_screen();
  banner();

  init_desc_tables(); //get exception handling up quickly!

  if(magic != MULTIBOOT_BOOTLOADER_MAGIC)
  {
    printf("Kernel was not loaded using a multiboot compliant bootloader. Cannot continue" \
           " reliably without multiboot information. Halting...\n");
    return;
  }


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


}

void banner()
{
  printf("            ____  _____\n");
  printf("     __  __/ __ \\/ ___/\n");
  printf("    / / / / / / /\\___ \\\n");
  printf("   / /_/ / /_/ /____/ /\n");
  printf("  / ____/\\____/______/\n");
  printf(" / /\n");
  printf("/_/  Micro Operating System\n");
  printf("        By Grant Hernandez\n");
  
  printf("\nBuilt on %s at %s\n\n", __DATE__, __TIME__);
}
