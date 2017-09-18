#include <kerror.h>
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

static void panic_exception(struct registers * reg);

void panic(char * reason, ...)
{
  //we will never leave this function...
  disable_interupts();

  va_list args;

  va_start(args, reason);

  printf("\n\n[PANIC] Halt and catch fire\n");
  printf("Reason: ");

  vprintf(reason, args);

  printf("\n");

  for(;;)
    halt(); //goodbye
}

void handle_exception(struct registers * reg)
{
  int exCode = reg->int_no;

  ASSERT(exCode >= EXCEPT_DIV && exCode <= EXCEPT_SIMD);

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


void panic_exception(struct registers * reg)
{
  disable_interupts();

  printf("\n\n[PANIC] Exception '%s' (%d)\n\n",
      exception_strings[reg->int_no], reg->int_no);

  printf("Exception details:\n");
  printf("Panic called from ISR%d with error code %d\n", reg->int_no, reg->err_code);
  printf("Fault occured at EIP %p, CS 0x%04X\n\n", reg->eip, reg->cs);

  uint32 cr2;
  asm volatile("mov %%cr2, %0": "=b" (cr2));


  printf("===== Register Dump =====\n"
    "EAX=%08x EBX=%08x ECX=%08x EDX=%08x\n"
    "ESI=%08x EDI=%08x EBP=%08x ESP=%08x\n"
    "EIP=%08x CR2=%08x\n",
    reg->eax, reg->ebx, reg->ecx, reg->edx,
    reg->esi, reg->edi, reg->ebp, reg->esp,
    reg->eip, cr2);

  printf("Halting...");

  for(;;) halt();
}
