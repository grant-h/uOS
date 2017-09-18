#include <gdt.h>
#include <string.h>
#include <common.h>

// Lets us access our ASM functions from our C code.
extern void gdt_flush(uint32 ptr);
static void gdt_set_gate(uint32 num, uint32 base, uint32 limit, uint16 flags);

gdt_entry_t gdt_entries[5];
gdt_ptr_t   gdt_ptr;

void init_gdt()
{
   gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
   gdt_ptr.base  = (uint32)&gdt_entries;

   gdt_set_gate(0, 0, 0, 0);                     // Null segment
   gdt_set_gate(1, 0, 0x000FFFFF, GDT_CODE_PL0); // Kernel code segment
   gdt_set_gate(2, 0, 0x000FFFFF, GDT_DATA_PL0); // Kernel data segment
   gdt_set_gate(3, 0, 0x000FFFFF, GDT_CODE_PL3); // User mode code segment
   gdt_set_gate(4, 0, 0x000FFFFF, GDT_DATA_PL3); // User mode data segment

   gdt_flush((uint32)&gdt_ptr); 
}

// Set the value of one GDT entry.
// FROM_BIT_VAR:TO_BIT
static void gdt_set_gate(uint32 num, uint32 base, uint32 limit, uint16 flags) 
{
  uint64 descriptor = 0;

  //start with the top
  descriptor =  base         & 0xFF000000; //base direct map
  descriptor |= (base >> 16) & 0x000000FF; //base 23-16:7-0
  descriptor |= (flags << 8) & 0x00F0FF00; //flags 16-11:24-19 7-0:15-8
  descriptor |= limit        & 0x000F0000; //limit direct map

  //end with the bottom
  descriptor <<= 32;

  descriptor |= (base << 16) & 0xFFFF0000; //base 15-0:31-16
  descriptor |= limit        & 0x0000FFFF; //limit direct map

  //gdt_entries[num].limit_low = limit & 0x0000FFFF;
  //gdt_entries[num].base_low = base & 0x0000FFFF;
  //gdt_entries[num].base_middle = base & 0x00FF0000;

  memcpy(&gdt_entries[num], &descriptor, sizeof(uint64));
}
