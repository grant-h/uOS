#ifndef KHEAP_H
#define KHEAP_H

#define PAGE_SIZE 0x1000
#define PAGE_ALIGN 0xfffff000

void kmalloc_early_init(uint32 base);
uint32 kmalloc_early(uint32 size);
uint32 kmalloc_early_align(uint32 size);

#endif
