#ifndef KERROR_H
#define KERROR_H

#include <assembly.h>

#define ASSERT(cond) \
if(!(cond)) { \
  panic("Assertion failed in %s:%s() at line %d", __FILE__, __func__, __LINE__); \
}

#define BREAKPOINT asm volatile("int $0x3")
        
void panic(char * reason, ...);
void handle_exception(struct registers * reg);

#endif
