#include <irq.h>
#include <i8259.h>
#include <print.h>
#include <kerror.h>

void * irq_handlers[IRQ_AMOUNT] = {NULL};

// This gets called from our ASM interrupt handler stub.
void irq_handler(struct registers regs)
{
  ASSERT(regs.int_no >= IRQ_BASE);

  //IRQ_BASE is the start vector of the IRQs
  unsigned char irq_num = regs.int_no - IRQ_BASE; 

  void (* handler)(struct registers) = irq_handlers[irq_num];

  if(handler)
    handler(regs);

  pic_send_eoi(irq_num);
} 

void register_irq_handler(unsigned int irq, void (*handler)(struct registers))
{
  ASSERT(irq < IRQ_AMOUNT);

  printf("IRQ%d: registered\n", irq);

  irq_handlers[irq] = handler;
}

void unregister_irq_handler(unsigned int irq)
{
  ASSERT(irq < IRQ_AMOUNT);
  ASSERT(irq_handlers[irq] != NULL); //prevent strange bugs

  irq_handlers[irq] = NULL;
}
