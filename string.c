#include <string.h>
#include <kprint.h>

void * memcpy(void * dst, void * src, unsigned int size)
{
  unsigned int i;

  for(i = 0; i < size; i++)
  {
    *((unsigned char *)dst+i) = *((unsigned char *)src+i);
  }

  return dst;
}

void * memset(void * ptr, int value, unsigned int size)
{
  unsigned int i;
  for(i = 0; i < size; i++)
    *((unsigned char *)ptr+i) = (unsigned char)value;

  return ptr; 
}

int strlen(const char * string)
{
  int len = 0;
  while(*string++)
    len++;
  
  return len;
}
