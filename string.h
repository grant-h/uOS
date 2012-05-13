#ifndef STRING_H
#define STRING_H

void * memcpy(void * dst, void * src, unsigned int size);
void * memset(void * ptr, int value, unsigned int size);
int strlen(const char * string);
int printf(const char * fmt, ...);
char * itoa(int value, char * buffer, int base); //TODO: make compatable with unsigned ints

#endif
