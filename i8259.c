/* i8259.c
 * Control code for the interrupt controller
 */

#include <i8259.h>
#include <print.h>
#include <assembly.h>
#include <kerror.h>

/* PIC0: master, PIC1: slave */
#define PIC_MASTER_CMD  0x20
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_CMD   0xA0
#define PIC_SLAVE_DATA  0xA1

#define PIC_EOI 0x20

#define ICW1_ICW4 0x01    /* ICW4 (not) needed */
#define ICW1_SINGLE 0x02    /* Single (cascade) mode */
#define ICW1_INTERVAL4  0x04    /* Call address interval 4 (8) */
#define ICW1_LEVEL  0x08    /* Level triggered (edge) mode */
#define ICW1_INIT 0x10    /* Initialization - required! */
 
#define ICW4_8086 0x01    /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO 0x02    /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE  0x08    /* Buffered mode/slave */
#define ICW4_BUF_MASTER 0x0C    /* Buffered mode/master */
#define ICW4_SFNM 0x10    /* Special fully nested (not) */

void pic_send_eoi(unsigned char irq)
{
  if(irq >= 8)
    outb(PIC_SLAVE_CMD, PIC_EOI);
  
  outb(PIC_MASTER_CMD, PIC_EOI);
}

void pic_init()
{
  unsigned char a1, a2;

  //a1 = inb(PIC_MASTER_DATA);                        // save masks
  //a2 = inb(PIC_SLAVE_DATA);

  outb(PIC_MASTER_CMD, ICW1_INIT+ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
  outb(PIC_SLAVE_CMD, ICW1_INIT+ICW1_ICW4);
  outb(PIC_MASTER_DATA, 0x20);                 // ICW2: Master PIC vector offset
  outb(PIC_SLAVE_DATA, 0x28);                 // ICW2: Slave PIC vector offset
  outb(PIC_MASTER_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
  outb(PIC_SLAVE_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)

  outb(PIC_MASTER_DATA, ICW4_8086);        // ICW4: 8086 extra hint
  outb(PIC_SLAVE_DATA, ICW4_8086);

  outb(PIC_MASTER_DATA, 0);
  outb(PIC_SLAVE_DATA, 0);

  printf("i8259 init completion\n"); 
}

void pic_mask_irq(unsigned char irq)
{
  ASSERT(irq <= 15);

  unsigned char mask;
  unsigned char port;

  if(irq > 7)
  {
    port = PIC_SLAVE_DATA;
    irq -= 8;
  }
  else 
  {
    port = PIC_MASTER_DATA;
  }

  mask = inb(port);
  mask |= 1 << irq;

  outb(port, mask);
}

void pic_unmask_irq(unsigned char irq)
{
  ASSERT(irq <= 15);

  unsigned char mask;
  unsigned char port;

  if(irq > 7)
  {
    port = PIC_SLAVE_DATA;
    irq -= 8;
  }
  else 
  {
    port = PIC_MASTER_DATA;
  }

  mask = inb(port);

  mask &= ~(1 << irq);

  outb(port, mask);
}
