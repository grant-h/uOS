#ifndef VGA_H
#define VGA_H

enum vga_color
{
  COLOR_BLACK = 0,
  COLOR_BLUE,
  COLOR_GREEN,
  COLOR_CYAN,
  COLOR_RED,
  COLOR_MAGENTA,
  COLOR_BROWN,
  COLOR_LGREY,
  COLOR_DGREY,
  COLOR_LBLUE,
  COLOR_LGREEN,
  COLOR_LCYAN,
  COLOR_LRED,
  COLOR_LMAGENTA,
  COLOR_YELLOW,
  COLOR_WHITE,
  COLOR_DONTCARE
};
typedef enum vga_color vga_color_t;

//TODO-WANT: add an attribute system where old colors can be pushed and poped
//just like MS Visual Studio's #pragma push/pop

void vga_init();
void vga_set_color(vga_color_t fg, vga_color_t bg);
void vga_get_color(vga_color_t * fg, vga_color_t * bg);
void vga_kputc(unsigned char c);

#endif
