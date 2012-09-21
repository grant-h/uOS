#include <pit.h>
#include <i8259.h>
#include <print.h>
#include <assembly.h>

void init_pit(uint32 freq)
{
  uint32 divisor = 1193180 / freq;

  // Send the command byte.
  outb(0x43, 0x36);

  // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
  uint8 l = (uint8)(divisor & 0xFF);
  uint8 h = (uint8)( (divisor>>8) & 0xFF );

  // Send the frequency divisor.
  outb(0x40, l);
  outb(0x40, h);

  printf("PIT frequency %d Hz (divisor %d)\n", freq, divisor);
}
