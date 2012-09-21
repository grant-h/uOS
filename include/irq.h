#ifndef IRQ_H
#define IRQ_H

#include <common.h>
#include <assembly.h>

#define IRQ_BASE 32

enum irq_vectors
{
IRQ0 = IRQ_BASE,
IRQ1, IRQ2, IRQ3, IRQ4, IRQ5, IRQ6, IRQ7, IRQ8, IRQ9, IRQ10, IRQ11,
IRQ12, IRQ13, IRQ14, IRQ15
};

void irq_handler(struct registers regs);

#endif
