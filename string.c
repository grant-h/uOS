#include <string.h>
#include <kprint.h>

static int printd(int value, int base);
static int printsz(const char * string);

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

static int printd(int value, int base)
{
  char buffer[33]; 
  
  itoa(value, buffer, base);
  return printsz(buffer);
}

static int printsz(const char * string)
{
  while(*string)
    kputc(*string++);

  return strlen(string);
}

int printf(const char * fmt, ...)
{

  /* Interesting pointer "magic"
   * 
   * Before prologue...
   * --STACK--
   * ESP:   fmt
   * ESP+4: arg1
   * ESP+8: arg2
   * ...
   */

  int ** args = (int **)&fmt; //get the address of fmt, which is on the stack
  args++; //skip fmt itself

  int totalWritten = 0;
  int i;

  for(i = 0; i < strlen(fmt); i++)
  {
    char c = fmt[i];

    if(c == '%' && i+1 < strlen(fmt)) //here we go!
    {
      char specifier = fmt[i+1];

      switch(specifier)
      {
        case 's':
          totalWritten += printsz((const char *)*args++);
          break;
        case 'd':
          totalWritten += printd((int)*args++, 10);
          break;
        case 'x':
          totalWritten += printd((int)*args++, 16);
          break;
        case '%': //treat is a normal character
          kputc(specifier);
          totalWritten++;
          break;
        default:
          kputc(c);
          kputc(specifier); //FIXME must be a cleaner way of doing this
          totalWritten += 2;
          break;
      }

      i++;
    }
    else
    {
      kputc(c);
      totalWritten++;
    }
  }

  return totalWritten;
}

char * itoa(int value, char * buffer, int base)
{
  char * front = buffer; //always points to the front
  char * p = buffer;
  char * p1, * p2;
  unsigned int uValue = value; //kill the sign

  if(base < 2 || base > 16) //not accepted
    return front;

  if(base == 10 && value < 0)
  {
    *p++ = '-';
    buffer++; //deny access to first character
    uValue = -value; //restore base 10 sanity
  }

  do
  {
    int remainder = uValue % base;
    *p++ = (remainder < 10) ? remainder + '0' : remainder - 10 + 'a';
  } while(uValue /= base);
  
  *p = 0; //TERMINATED!

  //our algorithm places the characters in reverse order so, reverse the buffer
  p1 = buffer;
  p2 = p - 1; //dont reverse the null terminator 

  while(p1 < p2) //smart
  {
    char tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;

    p1++;
    p2--;
  }

  return front;
}
