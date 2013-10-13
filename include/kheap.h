#ifndef KHEAP_H
#define KHEAP_H

#include <common.h>
#include <boot/multiboot.h>
#include <memory.h>

void kmalloc_early_init(uint32 base, uint32 max);
int kmalloc_early_init_grub(struct multiboot_info * mbi);
uint32 kmalloc_early(uint32 size);
uint32 kmalloc_early_align(uint32 size);

#endif
