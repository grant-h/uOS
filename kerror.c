#include <kerror.h>
#include <assembly.h>
#include <string.h>

void panic_exception(registers_t reg)
{
  clear_interupts();

  printf("[PANIC] Due to an unrecoverable exception\n\n");
  printf("Exception details:\n");
  printf("Panic called from ISR%d with error code %d\n", reg.int_no, reg.err_code);
  printf("Fault occured at EIP 0x%x, CS 0x%x\n", reg.eip, reg.cs);

  printf("Halting...");

  for(;;) 
    halt();
}
