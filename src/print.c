#include <print.h>
#include <common.h>

/*
 * Internal definitions
 */

#define ISDIGIT(x) ((x) >= '0' && (x) <= '9')
#define PRINT_BUF_LEN 65

//Flags
#define LEFT_JUST 0x1
#define SHOW_SIGN 0x2
#define HIDE_SIGN 0x4
#define DISP_HEXD 0x8
#define PADW_ZERO 0x10

//Internal flags
#define DATA_LONG 0x1
#define DATA_LLGN 0x2
#define DATA_SHRT 0x4

/*
 * Internal Function Prototypes
 */
static int outchar(char ** out, char c);
static int print(char ** out, const char * fmt, va_list args);
static int prints(char ** out, const char * string, int32 width, int32 flags);
static int printi(char ** out, int64 num, int32 base, int32 sign, int32 width, int32 flags, char convbase);

/*
 * Function Definitions
 */

int printf(const char * fmt, ...)
{
  if (!fmt)
    return 0;

  va_list list;
  va_start(list, fmt);

  int32 cw = print(NULL, fmt, list);

  va_end(list);

  return cw;
}

int vprintf(const char * fmt, va_list args)
{
  if (!fmt || !args)
    return 0;

  va_list list = args;

  int32 cw = print(NULL, fmt, list);

  va_end(list);

  return cw;
}

int sprintf(char * buf, const char * fmt, ...)
{
  if (!fmt || !buf)
    return 0;

  va_list list;
  va_start(list, fmt);

  int32 cw = print(&buf, fmt, list);

  va_end(list);

  return cw;
}

int vsprintf(char * buf, const char * fmt, va_list args)
{
  if (!fmt || !buf || !args)
    return 0;

  va_list list = args;

  int32 cw = print(&buf, fmt, list);

  va_end(list);

  return cw;
}

/*
 * Internal function definitions
 */

static int outchar(char ** out, char c)
{
  extern void vga_kputc(unsigned char);

  if (out)
  {
    **out = c;
    (*out)++;
  }
  else
    vga_kputc(c);

  return 1;
}

static int print(char ** out, const char * fmt, va_list args)
{
  int32 cw = 0;

  for (; *fmt; ++fmt)
  {
    int32 parsing = FALSE, 
          flags = 0, 
          internal_flags = 0,
          width = 0;

    if (*fmt != '%')
    {
      cw += outchar(out, *fmt);
      continue;
    }

    ++fmt;

    if (*fmt == '\0')
      break;
    if (*fmt == '%')
    {
      cw += outchar(out, *fmt);
      continue;
    }

    /* FLAGS STAGE
     * '-' left, '+' show sign, ' ' no sign
     * '#' 0x, '0' pad using 0s
     */

    parsing = TRUE;
    while (parsing)
    {
      switch (*fmt++)
      {
        case '-':
          flags |= LEFT_JUST;
          break;
        case '+':
          flags |= SHOW_SIGN;
          break;
        case ' ':
          flags |= HIDE_SIGN;
          break;
        case '#':
          flags |= DISP_HEXD;
          break;
        case '0':
          flags |= PADW_ZERO;
          break;
        default:
          parsing = FALSE;
          fmt--;
          break;
      }
    }

    /* WIDTH STAGE
     * '0'-'9'
     */

    while (ISDIGIT(*fmt))
    {
      width += *fmt - '0';
      ++fmt;

      if (ISDIGIT(*fmt))
        width *= 10;
    }

    /* BIT LENGTH STAGE
     * 'h' short, 'l' long
     * two longs ("ll") is long long
     */

    parsing = TRUE;
    while (parsing)
    {
      switch (*fmt++)
      {
        case 'h':
          internal_flags |= DATA_SHRT;
          break;
        case 'l':
          if(internal_flags & DATA_LONG)
            internal_flags |= DATA_LLGN;
          else
            internal_flags |= DATA_LONG;
          break;
        default:
          parsing = FALSE;
          fmt--;
          break;
      }
    }

    /* Sanity check the flags for mutually exclusive ones
     * If there is conflict, resolve it by mistrusting the user
     * and disable all the conflicts
     */

    if (flags & HIDE_SIGN && flags & SHOW_SIGN)
    {
      flags &= ~HIDE_SIGN;
      flags &= ~SHOW_SIGN;
    }

    if (internal_flags & DATA_SHRT && (internal_flags & DATA_LONG || internal_flags & DATA_LLGN))
      internal_flags = 0;

    if (flags & DISP_HEXD && flags & SHOW_SIGN)
    {
      flags &= ~DISP_HEXD;
      flags &= ~SHOW_SIGN;
    }

    /* FORMAT SPECIFIER STAGE
     * 'c' print char, 'd' or 'i' print signed int type
     * 's' print null terminated string or '(null)' if pointer NULL
     * 'u' print unsigned int type
     * 'x' prints unsigned in as hex 'X' capitalized
     * 'p' print using the pointer type
     */

    char buf[2] = {0};
    char * str = 0;

    switch (*fmt)
    {
      case 's':
        flags &= LEFT_JUST; //the only flag used
        str = (char *)va_arg(args, int32);
        cw += prints(out, str ? str : "(null)", width, flags);
        break;
      case 'c':
        flags &= LEFT_JUST;
        buf[0] = va_arg(args, int32);
        buf[1] = '\0';
        cw += prints(out, buf, width, flags);
        break;
      case 'd':
      case 'i':
        if (internal_flags & DATA_LLGN)
          cw += printi(out, va_arg(args, int64), 10, TRUE, width, flags, 'a');
        else if (internal_flags & DATA_SHRT)
          cw += printi(out, va_arg(args, int16), 10, TRUE, width, flags, 'a');
        else
          cw += printi(out, va_arg(args, int32), 10, TRUE, width, flags, 'a');
        break;
      case 'u':
        if (internal_flags & DATA_LLGN)
          cw += printi(out, va_arg(args, uint64), 10, FALSE, width, flags, 'a');
        else if (internal_flags & DATA_SHRT)
          cw += printi(out, va_arg(args, uint16), 10, FALSE, width, flags, 'a');
        else
          cw += printi(out, va_arg(args, uint32), 10, FALSE, width, flags, 'a');
        break;
      case 'x':
        if (internal_flags & DATA_LLGN)
          cw += printi(out, va_arg(args, uint64), 16, FALSE, width, flags, 'a');
        else if (internal_flags & DATA_SHRT)
          cw += printi(out, va_arg(args, uint16), 16, FALSE, width, flags, 'a');
        else
          cw += printi(out, va_arg(args, uint32), 16, FALSE, width, flags, 'a');
        break;
      case 'p':
        flags = 0, width = 0; //reset flags
        flags = DISP_HEXD | PADW_ZERO;

        if (internal_flags & DATA_LLGN)
          width = sizeof(int64)*2;
        else if (internal_flags & DATA_SHRT)
          width = sizeof(int16)*2;
        else
          width = sizeof(int32)*2;
      case 'X': //fall through
        if (internal_flags & DATA_LLGN)
          cw += printi(out, va_arg(args, uint64), 16, FALSE, width, flags, 'A');
        else if (internal_flags & DATA_SHRT)
          cw += printi(out, va_arg(args, uint16), 16, FALSE, width, flags, 'A');
        else
          cw += printi(out, va_arg(args, uint32), 16, FALSE, width, flags, 'A');
        break;
      default:
        cw += outchar(out, *fmt);
        break;
    }

  }

  return cw;
}

static int prints(char ** out, const char * string, int32 width, int32 flags)
{
  int32 cw = 0, len = 0; 
  char padc = ' ';
  char * hexd = "0x";
  const char * ptr = string;

  while(*ptr++) len++; //string length
  
  if(len >= width) //dont care
    width = 0;
  else
  {
    if(flags & PADW_ZERO)
      padc = '0';
    width -= len; //how much padding we need
  }

  if (flags & LEFT_JUST)
  {
    if (flags & DISP_HEXD)
      while (*hexd)
        cw += outchar(out, *hexd++);

    if (width)
    {
      if (flags & PADW_ZERO) //padding comes first
      {
        while (width--)
          cw += outchar(out, padc);

        while (*string)
          cw += outchar(out, *string++);

      }
      else //padding comes after everything
      {
        while (*string)
          cw += outchar(out, *string++);

        while (width--)
          cw += outchar(out, padc);
      }
    }
    else //no padding
      while (*string)
        cw += outchar(out, *string++);
  }
  else //right justified
  {
    if (width && !(flags & PADW_ZERO))
    {
      while (width--)
        cw += outchar(out, padc);

      if (flags & DISP_HEXD)
        while (*hexd)
          cw += outchar(out, *hexd++);
    }
    else if(width && flags & PADW_ZERO)
    {
      if(flags & DISP_HEXD)
        while (*hexd)
          cw += outchar(out, *hexd++);

      while (width--)
        cw += outchar(out, padc);
    }
    else //no padding
      if(flags & DISP_HEXD)
        while (*hexd)
          cw += outchar(out, *hexd++);

    while (*string)
      cw += outchar(out, *string++);
  }

  return cw;
}

static int printi(char ** out, int64 num, int32 base, int32 sign, int32 width, int32 flags, char convbase)
{
  int32 cw = 0, neg = FALSE;
  uint64 u;
  char buffer[PRINT_BUF_LEN] = {0}; //longest representation (base 2, 64 bits)
  char * iter;

  u = num;

  if(base < 2) //error check
    return 0;

  if (base != 16) //show hexd only for base 16
    flags &= ~DISP_HEXD;

  if (num == 0) //short circuit and prevents errors
  {
    buffer[0] = '0';
    buffer[1] = '\0';
    return prints(out, buffer, width, flags);
  }

  if (sign && base == 10 && num < 0)
  {
    neg = TRUE;
    u = -num; //restore sanity
  }


  iter = buffer + PRINT_BUF_LEN-1;
  *iter-- = '\0';

  while (u) //everything is going in reverse
  {
    int32 rem = u % base;
    char c;

    if (rem >= 10)
      c = rem - 10 + convbase;
    else
      c = rem + '0';

    *iter-- = c;

    u /= base;
  }

  if (!(flags & HIDE_SIGN) && base == 10)
  {
    if (flags & PADW_ZERO) //sign always first
    {
      if (flags & SHOW_SIGN && !neg)
      {
        cw += outchar(out, '+');
        width--;
      }

      if (neg)
      {
        cw += outchar(out, '-');
        width--;
      }
    }
    else
    {
      if (flags & SHOW_SIGN && !neg)
        *iter-- = '+';
      
      if (neg)
        *iter-- = '-';
    }
  }

  return cw + prints(out, ++iter, width, flags);
}
