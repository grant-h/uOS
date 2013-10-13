#ifndef ASSEMBLY_H
#define ASSEMBLY_H

struct registers
{
   unsigned int ds;                  // Data segment selector
   unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
   unsigned int int_no, err_code;    // Interrupt number and error code (if applicable)
   unsigned int eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
};

#define FORCEINLINE __attribute__((always_inline))

void outb(unsigned short port, unsigned char value) FORCEINLINE;
unsigned char inb(unsigned short port);
unsigned short inw(unsigned short port);
void disable_interupts() FORCEINLINE;
void enable_interupts() FORCEINLINE;
void halt() FORCEINLINE;

#endif
