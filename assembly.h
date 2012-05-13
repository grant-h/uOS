#ifndef ASSEMBLY_H
#define ASSEMBLY_H

void outb(unsigned short port, unsigned char value);
unsigned char inb(unsigned short port);
unsigned short inw(unsigned short port);
void clear_interupts();
void restore_interupts();
void halt();
#endif
