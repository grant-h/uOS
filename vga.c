/*
 * Defines basic VGA text manipulation routines.
 */

#include <common.h>
#include <string.h>
#include <print.h>
#include <vga.h>
#include <kerror.h>

#define VID_MEM 0xb8000
#define COLUMNS 80
#define BYTES_PER_ROW COLUMNS*2
#define ROWS 25
#define TOTAL_BYTES ROWS*BYTES_PER_ROW

#define VGA_COMMAND_PORT 0x3D4
#define VGA_DATA_PORT 0x3D5
#define VGA_LOW_INDEX_REG 0xF
#define VGA_HIGH_INDEX_REG 0xE

static unsigned int vga_row = 0, vga_col = 0;
static uint8 attr_fg = COLOR_LGREY, attr_bg = COLOR_BLUE;

static void update_cursor();
static void clear_row(int row);
static void clear_screen();
static void scroll_up();

void vga_init()
{
  //TODO: check for monochrome monitor (just for fun)

  clear_screen(); //screen is now in a known state
  printf("VGA: Mode %dx%d, %s, attr fg:%#x bg:%#x\n", COLUMNS, ROWS, "color", attr_fg, attr_bg);
}

void vga_set_color(vga_color_t fg, vga_color_t bg)
{
  if(fg < COLOR_DONTCARE)
    attr_fg = fg;
  if(bg < COLOR_DONTCARE)
    attr_bg = bg;
}

void vga_get_color(vga_color_t * fg, vga_color_t * bg)
{
  if(fg)
    *fg = attr_fg;
  if(bg)
    *bg = attr_bg;
}

void vga_kputc(unsigned char c)
{
  //special character handling
  if(c == '\n')
  {
    vga_row++;
    vga_col = 0;

    if(vga_row == ROWS)
      scroll_up();
    update_cursor();
  }
  else if(c == '\r')
  {
    vga_col = 0;
    update_cursor();
  }
  else
  {
    unsigned char *vidmem = (unsigned char *)VID_MEM;
    unsigned char *writePosition = vidmem + (vga_row*COLUMNS + vga_col)*2; //times 2 to skip attributes
    writePosition[0] = c;
    writePosition[1] = (attr_bg << 4) | attr_fg; 

    vga_col++;
    if(vga_col == COLUMNS)
      vga_kputc('\n');
  }
}

void update_cursor()
{
  unsigned short pos = vga_row*COLUMNS + vga_col;
  //TODO do out byte
  pos = pos;
}

void clear_row(int row)
{
  unsigned char *vidmem = (unsigned char *)VID_MEM + BYTES_PER_ROW*row;

  int i;
  for(i = 0; i < BYTES_PER_ROW; i++)
  {
    if(i % 2 == 0) //character
      vidmem[i] = '\0';
    else //attribute
      vidmem[i] = (attr_bg << 4) | attr_fg;
  }
}

void clear_screen()
{
  int i;
  for(i = 0; i < ROWS; i++)
    clear_row(i);

  vga_row = vga_col = 0;
  update_cursor();
}

void scroll_up() 
{
  int row;
  //start reading from the second row and overwrite the first one
  unsigned char * readPos = (unsigned char *)VID_MEM + BYTES_PER_ROW;
  unsigned char * writePos = (unsigned char *)VID_MEM;

  //where row is the row to written to
  //stop before the last row because there is nothing after it to be read
  for(row = 0; row < ROWS-1; row++) //row 0 becomes row 1, row 1 becomes row 2
  {
    memcpy(writePos, readPos, BYTES_PER_ROW);
    readPos += BYTES_PER_ROW;
    writePos += BYTES_PER_ROW;
  }

  clear_row(ROWS-1); //clear last row

  if(vga_row > 0)
  {
    vga_row--;
    update_cursor();
  }
}
