#include <irq.h>
#include <i8259.h>
#include <print.h>
#include <kerror.h>

// This gets called from our ASM interrupt handler stub.
void irq_handler(struct registers regs)
{
  ASSERT(regs.int_no >= IRQ_BASE);

  printf("IRQ%d\n", regs.int_no);

  unsigned char irq_num = regs.int_no - IRQ_BASE; //IRQ_BASE is the start interrupt number of the IRQs
  pic_send_eoi(irq_num);
} 
