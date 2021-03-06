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
typedef uint32             size_t;

// Calculate the offset in bytes of a struct member
#define offsetof(type, member) ((size_t) &((type *)0)->member)

// Retrieves the container object from a pointer to a struct member
// __memb is not strictly required, but it allows the compiler to typecheck
// It must be converted to a character pointer as offsetof() is in bytes
#define container_of(ptr, type, member) ({              \
  const typeof(((type *)0)->member) * __memb = (ptr);     \
  (type *)((char *)__memb - offsetof(type, member)); })

#endif
