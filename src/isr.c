#include <common.h>
#include <kerror.h>
#include <print.h>
#include <isr.h>
#include <irq.h>
#include <gdt.h>
#include <idt.h>

void isr_handler(struct registers * regs)
{
  int int_no = regs->int_no;

  if(int_no <= 19) //an exception has occured, panic!
    handle_exception(regs);
  else if(int_no >= IRQ_BASE)
    irq_handler(regs);
  else
    printf("[INFO] Unhandled interrupt %d\n", int_no);
}

void init_desc_tables()
{
  init_gdt();
  init_idt();
}
