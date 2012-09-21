#ifndef PRINT_H
#define PRINT_H

#include <common.h>

int printf(const char * fmt, ...);
int vprintf(const char * fmt, va_list args);

int sprintf(char * buf, const char * fmt, ...);
int vsprintf(char * buf, const char * fmt, va_list args);

#endif
