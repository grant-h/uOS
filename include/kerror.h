#ifndef KERROR_H
#define KERROR_H

#include <common.h>

#define ASSERT(cond) \
if(!(cond)) { \
  printf("Assertion failed at %s:%d\n", __FILE__, __LINE__); \
  panic(); \
}
        
void panic();
void handle_exception(registers_t reg);
void panic_exception(registers_t reg);

#endif
