#ifndef I8259_H
#define I8259_H

void pic_send_eoi(unsigned char irq);
void pic_init();
void pic_mask_irq(unsigned char irq);
void pic_unmask_irq(unsigned char irq);

#endif
