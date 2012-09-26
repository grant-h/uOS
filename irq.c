#include <irq.h>
#include <i8259.h>
#include <print.h>
#include <kerror.h>

void * irq_handlers[IRQ_AMOUNT] = {0};

// This gets called from our ASM interrupt handler stub.
void irq_handler(struct registers regs)
{
  ASSERT(regs.int_no >= IRQ_BASE);

  unsigned char irq_num = regs.int_no - IRQ_BASE; //IRQ_BASE is the start vector of the IRQs

  void (* handler)(struct registers) = irq_handlers[regs.int_no - IRQ_BASE];

  if(handler)
    handler(regs);

  pic_send_eoi(irq_num);
} 

void register_irq_handler(uint32 irq, void (*handler)(struct registers))
{
  irq -= IRQ_BASE; //XXX: nasty hack, yuck

  printf("IRQ%d: registered\n", irq);

  irq_handlers[irq] = handler;
}

void unregister_irq_handler(uint32 irq)
{
  ASSERT(irq >= IRQ_AMOUNT);

  irq_handlers[irq] = NULL;
}
