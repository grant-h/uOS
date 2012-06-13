#include <irq.h>

void irq_map_isr(uint32 irq, uint32 isr)
{
  ASSERT(irq >= 0 && irq <= 15);
  ASSERT(isr >= 0 && isr <= 255);

  if(irq > 7) //slave
  {
  }
  else //master
  {
  }
}
