#include <pit.h>
#include <i8259.h>
#include <irq.h>
#include <print.h>
#include <kerror.h>
#include <assembly.h>

uint32 tick_count = 0;
uint32 tick_hz = 0;

void timer_tick(struct registers reg)
{
  tick_count++;

  if(tick_count % tick_hz == 0)
  {
    int secondsElapsed = tick_count / tick_hz; 

    if(secondsElapsed % 10 == 0)
      printf("%d", secondsElapsed);
    else
      printf(".");

    if(secondsElapsed == 7)
    {
      //int die = 10 / 0;
    }
  }
}

uint32 get_tick_count()
{
  return tick_count;
}

void init_pit(uint32 freq)
{
  uint32 divisor = 1193180 / freq;

  tick_hz = freq;

  // Send the command byte.
  outb(0x43, 0x36);

  // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
  uint8 l = (uint8)(divisor & 0xFF);
  uint8 h = (uint8)( (divisor>>8) & 0xFF );

  // Send the frequency divisor.
  outb(0x40, l);
  outb(0x40, h);

  register_irq_handler(IRQ0, timer_tick);
}

