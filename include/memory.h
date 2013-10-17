#ifndef MEMORY_H
#define MEMORY_H

// helper constants
#define PAGE_SIZE 0x1000
#define PAGE_ALIGN 0xfffff000
#define NOT_ALIGNED ~(PAGE_ALIGN)
#define PAGE_ALIGN_UP(addr) ((addr & NOT_ALIGNED) ? ((addr & PAGE_ALIGN) + PAGE_SIZE) \
                                                  : (addr))
// 1024 DWORD entries per PD and PT
#define PAGE_ENTRIES 1024
#define PAGE_TABLE_SIZE (sizeof(uint32)*PAGE_ENTRIES)

// SI units to number of pages
#define PAGES_PER_KB(kb) (PAGE_ALIGN_UP(kb*1024) / PAGE_SIZE)
#define PAGES_PER_MB(mb) (PAGE_ALIGN_UP(mb*1024*1024) / PAGE_SIZE)
#define PAGES_PER_GB(gb) (PAGE_ALIGN_UP(gb*1024*1024*1024) / PAGE_SIZE)

// page flags
#define PAGE_ENTRY_PRESENT 0x1
#define PAGE_ENTRY_RW      0x2

// Information about the Kernel from the linker
extern uint32 _KERNEL_START;
extern uint32 _KERNEL_END;
// Simplified storage varables (see memory.c)
extern uint32 KERNEL_START;
extern uint32 KERNEL_END;

void paging_init();
uint32 page_ident_map(uint32 addr, uint32 perm);
uint32 page_map(uint32 virt, uint32 phys, uint32 perm);
static void _native_set_page_directory(uint32 * phyDir);
static void _native_paging_enable();
static void _native_paging_disable();

#endif
