#ifndef COMMON_H
#define COMMON_H

#ifndef NULL
#define NULL 0
#endif
#define FALSE 0
#define TRUE 1

/*
 * Minimal vararg (stdarg.h) support. 
 * Copied from M$ support code
 */

typedef char * va_list;
#define _INTSIZEOF(n)   ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))
#define va_start(ap,v)  (ap = (va_list)&v + _INTSIZEOF(v))
#define va_arg(ap,t)    (*(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)))
#define va_end(ap)      (ap = (va_list)0)

/*
 * Bitwise Helpers
 */

#define ISSET(field, var) ((var) & (field))
#define ISSET_BIT(bit, var) ((var) & (1 << (bit)))

/*
 * Minimal stdint.h for "portable" ints
 */

typedef unsigned long long uint64;
typedef          long long int64;
typedef unsigned int       uint32;
typedef          int       int32;
typedef unsigned short     uint16;
typedef          short     int16;
typedef unsigned char      uint8;
typedef          char      int8;

typedef struct registers
{
   unsigned int ds;                  // Data segment selector
   unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
   unsigned int int_no, err_code;    // Interrupt number and error code (if applicable)
   unsigned int eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} registers_t;

#endif
