#include <common.h>
#include <string.h>
#include <boot/multiboot.h>
#include <kprint.h>
#include <isr.h>

void kmain(void* mbd, unsigned int magic)
{
  clear_screen();

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
    printf("mem_lower = %dKB, mem_upper = %dKB\n", mbi->mem_lower, mbi->mem_upper);
  }

  asm volatile("int  $9");
}
