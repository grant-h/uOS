#include <irq.h>
#include <driver.h>
#include <print.h>

static void keyboard_irq(struct registers * regs);

int keyboard_initialize()
{
  printf("Keyboard initialized\n");
  register_irq_handler(IRQ1, keyboard_irq);
  return 0;
}

static void keyboard_irq(struct registers * regs)
{
  printf("IRQ1\n");
}

int keyboard_finalize()
{
  printf("Keyboard unloading\n");
  return 0;
}

struct driver_interface keyboard_driver = {
  .name = "keyboard",
  .functions = {
    .initialize = keyboard_initialize,
    .finalize = keyboard_finalize
  }
};
