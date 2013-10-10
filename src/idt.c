#include <idt.h>
#include <string.h>
#include <irq.h>

extern void idt_flush(unsigned int);

static void idt_set_gate(unsigned char,unsigned int,unsigned short,unsigned char);

idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;

extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

void (* INT_LIST[])(void) = {isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7, 
                             isr8, isr9, isr10, isr11, isr12, isr13, isr14, 
                             isr15, isr16, isr17, isr18, isr19,isr20, isr21, 
                             isr22, isr23, isr24, isr25, isr26, isr27, isr28, 
                             isr29, isr30, isr31, irq0, irq1, irq2, irq3, irq4,
                             irq6, irq6, irq7, irq8, irq9, irq10, irq11, irq12,
                             irq13, irq14, irq15};

#define INT_LIST_LEN (sizeof(INT_LIST)/sizeof(INT_LIST[0]))

void init_idt()
{
   unsigned int isr;

   idt_ptr.limit = sizeof(idt_entry_t) * 256 -1;
   idt_ptr.base  = (unsigned int)&idt_entries;

   memset(&idt_entries, 0, sizeof(idt_entry_t)*256);

   for(isr = 0; isr < INT_LIST_LEN; isr++)
     idt_set_gate(isr, (unsigned int)INT_LIST[isr], 0x08, 0x8E);

   idt_flush((unsigned int)&idt_ptr);
}

static void idt_set_gate(unsigned char num, unsigned int base, unsigned short sel, unsigned char flags)
{
   idt_entries[num].base_lo = base & 0xFFFF;
   idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

   idt_entries[num].sel     = sel;
   idt_entries[num].always0 = 0;
   // We must uncomment the OR below when we get to using user-mode.
   // It sets the interrupt gate's privilege level to 3.
   idt_entries[num].flags   = flags /* | 0x60 */;
} 

