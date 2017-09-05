#include <common.h>
#include <print.h>
#include <boot/multiboot.h>
#include <kerror.h>
#include <kheap.h>
#include <vga.h>
#include <isr.h>
#include <pit.h>
#include <i8259.h>

void banner();
void print_multiboot(struct multiboot_info * mbi);

void kmain(struct multiboot_info * mbi, uint32 magic)
{
  vga_init();
  banner();

  if(magic != MULTIBOOT_BOOTLOADER_MAGIC)
    panic("Kernel was not loaded using a multiboot compliant bootloader. Cannot continue" \
           " reliably without multiboot information. Halting...\n");

  ////////////////////

  // Initializes our IDT and GDT to get exception handling up ASAP
  init_desc_tables();

  // print some diagnostic info on grub's memory map
  print_multiboot(mbi);
  printf("  [%p-%p KERNL]\n", KERNEL_START, KERNEL_END);

  // initialize our early kmalloc
  kmalloc_early_init(EARLY_KMALLOC_START, EARLY_KMALLOC_END);

  // transition from early page tables to kernel tables
  paging_init();

  // Initialize the interrupt controller
  pic_init();
  // interface with the Programmable Interval Timer
  init_pit(100); // 100 Hz
  enable_interupts(); // hardware interrupts are now enabled

  // become interrupt driven
  for(;;) halt();
}

void banner()
{
  // make the header stand out
  vga_set_color(COLOR_WHITE, COLOR_DONTCARE);
  printf("              ____  _____\n");
  printf("       __  __/ __ \\/ ___/\n");
  printf("      / / / / / / /\\___ \\\n");
  printf("     / /_/ / /_/ /____/ /\n");
  printf("    / ____/\\____/______/\n");
  printf("   / /\n");
  printf("  /_/  Micro Operating System\n");
  printf("          By Grant Hernandez\n");

  printf("\nBuilt on %s at %s\n\n", __DATE__, __TIME__);
  vga_set_color(COLOR_LGREY, COLOR_DONTCARE);
}

void print_multiboot(struct multiboot_info * mbi)
{
  if(mbi == NULL)
  {
    printf("No multiboot information!\n");
    return;
  }

  if(mbi->flags & MULTIBOOT_INFO_MEMORY)
  {
    unsigned int total_mem = mbi->mem_lower + mbi->mem_upper;
    printf("Total Memory %d KiB: mem_lower = %d KiB, mem_upper = %d KiB\n", total_mem,
                                                                            mbi->mem_lower,
                                                                            mbi->mem_upper);
  }

  if(mbi->flags & MULTIBOOT_INFO_MEM_MAP)
  {
    unsigned int * mem_info_ptr = (unsigned int *)mbi->mmap_addr;

    printf("Tentative Memory Map\n");

    while(mem_info_ptr < (unsigned int *)mbi->mmap_addr+mbi->mmap_length)
    {
      multiboot_memory_map_t * cur = (multiboot_memory_map_t *)mem_info_ptr;

      if(cur->len > 0)
        printf("  [%p-%p %5s]\n", (uint32)cur->addr,
                                  (uint32)(cur->addr+cur->len),
                                  cur->type == MULTIBOOT_MEMORY_AVAILABLE ? "AVAIL" : "RESVD");

      mem_info_ptr += cur->size + sizeof(cur->size);
    }
  }
}
