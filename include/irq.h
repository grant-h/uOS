#ifndef IRQ_H
#define IRQ_H

#include <common.h>

void irq_map_isr(uint32 irq, uint32 isr);

#endif
