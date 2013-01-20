#include <common.h>
#include <print.h>
#include <boot/multiboot.h>
#include <kerror.h>
#include <vga.h>
#include <isr.h>
#include <pit.h>
#include <i8259.h>

void banner();
void print_multiboot(void * mbInfo);

void kmain(void * mbd, unsigned int magic)
{
  vga_init(); 
  banner();

  init_desc_tables(); //get exception handling up quickly!

  if(magic != MULTIBOOT_BOOTLOADER_MAGIC)
  {
    panic("Kernel was not loaded using a multiboot compliant bootloader. Cannot continue" \
           " reliably without multiboot information. Halting...\n");
    return; //never reached
  }


  printf("Using standard 80x25 vga text mode\n");
  print_multiboot(mbd);

  pic_init();
  init_pit(100);
  enable_interupts(); //hardware interupts are now enabled

  for(;;)
    halt();
}

void banner()
{
  //this color setting interface is cumbersome
  //for my application, it would be better if 
  //color codes were inline with the text, such
  //as traditional terminal coloring schemes
  vga_set_color(COLOR_LRED, COLOR_DONTCARE);
  printf("              ____  _____\n");
  printf("       __  __/ __ \\/ ___/\n");
  printf("      / / / / / / /\\___ \\\n");
  printf("     / /_/ / /_/ /____/ /\n");
  printf("    / ____/\\____/______/\n");
  printf("   / /\n");
  printf("  /_/  Micro Operating System\n");
  vga_set_color(COLOR_GREEN, COLOR_DONTCARE);
  printf("          By Grant Hernandez\n");
  
  vga_set_color(COLOR_WHITE, COLOR_DONTCARE);
  printf("\nBuilt on %s at %s\n\n", __DATE__, __TIME__);
  vga_set_color(COLOR_LGREY, COLOR_DONTCARE);
}

void print_multiboot(void * mbInfo)
{
  struct multiboot_info * mbi = (struct multiboot_info *)mbInfo;
   
  if(mbi == NULL)
  {
    printf("No multiboot information!\n");
    return;
  }

  if(mbi->flags & MULTIBOOT_INFO_MEMORY)
  {
    unsigned int total_mem = mbi->mem_lower + mbi->mem_upper; 
    printf("Total Memory %d KiB: mem_lower = %d KiB, mem_upper = %d KiB\n", total_mem, mbi->mem_lower, mbi->mem_upper);
  }

  if(mbi->flags & MULTIBOOT_INFO_MEM_MAP)
  {
    unsigned int * mem_info_ptr = (unsigned int *)mbi->mmap_addr; 

    printf("Tentative Memory Map\n");

    while(mem_info_ptr < (unsigned int *)mbi->mmap_addr+mbi->mmap_length)
    {
      multiboot_memory_map_t * cur = (multiboot_memory_map_t *)mem_info_ptr;   

      if(cur->len > 0)
        printf("  [%p-%p %5s]\n", (uint32)cur->addr, (uint32)(cur->addr+cur->len), cur->type == MULTIBOOT_MEMORY_AVAILABLE ? "AVAIL" : "RESVD");

      mem_info_ptr += cur->size + sizeof(cur->size);
    }
  }
}
