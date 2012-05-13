#include <common.h>
#include <kerror.h>
#include <isr.h>
#include <gdt.h>
#include <idt.h>

void isr_handler(registers_t regs)
{
  int int_no = regs.int_no;

  if(int_no <= 18) //an exception has occured, panic!
    panic_exception(regs);
}

void init_desc_tables()
{
  init_gdt();
  init_idt();
}
