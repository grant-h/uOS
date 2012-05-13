#ifndef GDT_H
#define GDT_H

void init_gdt();

// This structure contains the value of one GDT entry.
// We use the attribute 'packed' to tell GCC not to change
// any of the alignment in the structure.
struct gdt_entry_struct
{
   unsigned short limit_low;           // The lower 16 bits of the limit.
   unsigned short base_low;            // The lower 16 bits of the base.
   unsigned char  base_middle;         // The next 8 bits of the base.
   unsigned char  access;              // Access flags, determine what ring this segment can be used in.
   unsigned char  granularity;
   unsigned char  base_high;           // The last 8 bits of the base.
} __attribute__((packed));             //DO NOT ALIGN
typedef struct gdt_entry_struct gdt_entry_t; 

struct gdt_ptr_struct
{
   unsigned short limit;               // The upper 16 bits of all selector limits.
   unsigned int base;                // The address of the first gdt_entry_t struct.
} __attribute__((packed));
typedef struct gdt_ptr_struct gdt_ptr_t; 

#define GDT_ACCESS_TYPE_CODE       (1 << 3) //if set, executable
  #define GDT_ACCESS_DATA_WRITE (1 << 1)
  
  #define GDT_ACCESS_CODE_READ (1 << 1)
#define GDT_ACCESS_DESC_TYPE  (1 << 4) //if set, code/data
#define GDT_ACCESS_DPL_LOW    (1 << 5)
#define GDT_ACCESS_DPL_HIGH   (1 << 6)
  #define GDT_ACCESS_RING1    GDT_ACCESS_DPL_LOW
  #define GDT_ACCESS_RING2    GDT_ACCESS_DPL_HIGH
  #define GDT_ACCESS_RING3    (GDT_ACCESS_RING1 | GDT_ACCESS_RING2)
#define GDT_ACCESS_SEGPRES    (1 << 7)

#define GDT_GRAN_DEFAULT_OPER (1 << 6) //if set, 32 bit
#define GDT_GRAN_BIT          (1 << 7) //if set, gdt_limit is scaled by 4KiB

#endif
