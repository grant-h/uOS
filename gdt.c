#include <gdt.h>
#include <string.h>

// Lets us access our ASM functions from our C code.
extern void gdt_flush(unsigned int ptr);
static void gdt_set_gate(int num, unsigned int base, unsigned int limit, unsigned char access, unsigned char gran);

gdt_entry_t gdt_entries[5];
gdt_ptr_t   gdt_ptr;

void init_gdt()
{
   gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
   gdt_ptr.base  = (unsigned int)&gdt_entries;

   unsigned char gran = GDT_GRAN_DEFAULT_OPER | GDT_GRAN_BIT;
   unsigned char accessCode = GDT_ACCESS_TYPE_CODE | GDT_ACCESS_CODE_READ;
   unsigned char accessData = GDT_ACCESS_DATA_WRITE;
   accessCode |= GDT_ACCESS_DESC_TYPE | GDT_ACCESS_SEGPRES;
   accessData |= GDT_ACCESS_DESC_TYPE | GDT_ACCESS_SEGPRES;

   /*printf("gran 0x%x, accessCode 0x%x, accessData 0x%x, aCR3 0x%x, aDR3 0x%x\n", gran,
                                                                                 accessCode,
                                                                                 accessData,
                                                                                 accessCode|GDT_ACCESS_RING3,
                                                                                 accessData|GDT_ACCESS_RING3);*/
                                                                                

   gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
   gdt_set_gate(1, 0, 0xFFFFFFFF, accessCode, gran); // Kernel code segment
   gdt_set_gate(2, 0, 0xFFFFFFFF, accessData, gran); // Kernel data segment
   gdt_set_gate(3, 0, 0xFFFFFFFF, accessCode | GDT_ACCESS_RING3, gran); // User mode code segment
   gdt_set_gate(4, 0, 0xFFFFFFFF, accessData | GDT_ACCESS_RING3, gran); // User mode data segment

   gdt_flush((unsigned int)&gdt_ptr);
}

// Set the value of one GDT entry.
static void gdt_set_gate(int num, unsigned int base, unsigned int limit, unsigned char access, unsigned char gran)
{
   gdt_entries[num].base_low    = (base & 0xFFFF);     //16 - 1
   gdt_entries[num].base_middle = (base >> 16) & 0xFF; //bits 24 - 16
   gdt_entries[num].base_high   = (base >> 24) & 0xFF; //32 -24

   gdt_entries[num].limit_low   = (limit & 0xFFFF);
   gdt_entries[num].granularity = (limit >> 16) & 0x0F; //the low nibble of limit

   gdt_entries[num].granularity |= gran & 0xF0; //the high nibble of gran
   gdt_entries[num].access      = access;
}
