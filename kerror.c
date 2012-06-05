#include <kerror.h>
#include <assembly.h>
#include <print.h>

enum exception_codes
{
  EXCEPT_DIV = 0,
  EXCEPT_RESVD,
  EXCEPT_NMI,
  EXCEPT_BP,
  EXCEPT_OVERFLOW,
  EXCEPT_BOUND,
  EXCEPT_INVOP,
  EXCEPT_DEVNA,
  EXCEPT_DOUBLE,
  EXCEPT_COSEG,
  EXCEPT_INVTSS,
  EXCEPT_SEGNP,
  EXCEPT_SSFAULT,
  EXCEPT_GFP,
  EXCEPT_PAGEFAULT,
  EXCEPT_RESVD2,
  EXCEPT_X87FPU,
  EXCEPT_ALIGN,
  EXCEPT_MACHINECK,
  EXCEPT_SIMD
};

char * exception_strings[] = { "Divide Error",
                               "Reserved",
                               "NMI Interrupt",
                               "Breakpoint",
                               "Overflow",
                               "BOUND Range Exceeded",
                               "Invalid Opcode",
                               "Device Not Available",
                               "Double Fault",
                               "Coprocessor Segment Overrun",
                               "Invalid TSS",
                               "Segment Not Present",
                               "Stack-Segment Fault",
                               "General Protection Fault (GPF)",
                               "Page Fault",
                               "Reserved",
                               "x87 FPU Error",
                               "Alignment Check",
                               "Machine Check",
                               "SIMD Floating-Point Exception" };

void panic()
{
  clear_interupts();

  printf("[KERNEL PANIC] Stop drop and roll!\n");
  printf("Halting system...\n");

  for(;;)
    halt();
}

void handle_exception(registers_t reg)
{
  int exCode = reg.int_no;

  ASSERT(exCode >= EXCEPT_DIV && exCode <= EXCEPT_SIMD);

  printf("Exception: %s (INT %d)\n", exception_strings[exCode], exCode);

  switch (exCode)
  {
    case EXCEPT_DIV:
      break;
    case EXCEPT_RESVD:
      break;
    case EXCEPT_NMI:
      break;
    case EXCEPT_BP:
      break;
    case EXCEPT_OVERFLOW:
      break;
    case EXCEPT_BOUND:
      break;
    case EXCEPT_INVOP:
      break;
    case EXCEPT_DEVNA:
      break;
    case EXCEPT_DOUBLE:
      break;
    case EXCEPT_COSEG:
      break;
    case EXCEPT_INVTSS:
      break;
    case EXCEPT_SEGNP:
      break;
    case EXCEPT_SSFAULT:
      break;
    case EXCEPT_GFP:
      break;
    case EXCEPT_PAGEFAULT:
      break;
    case EXCEPT_RESVD2:
      break;
    case EXCEPT_X87FPU:
      break;
    case EXCEPT_ALIGN:
      break;
    case EXCEPT_MACHINECK:
      break;
    case EXCEPT_SIMD:
      break;
  }

  panic_exception(reg);
}


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
