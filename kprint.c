/*
 * Defines basic VGA text manipulation routines.
 */

#include <common.h>
#include <string.h>
#include <kprint.h>

static unsigned int vga_row = 0, vga_col = 0;

void update_vga_cursor()
{
  unsigned short pos = vga_row*COLUMNS + vga_col;
  //TODO do out byte
  pos = pos;

}
void kputc(unsigned char c)
{
  //special character handling
  if(c == '\n')
  {
    vga_row++;
    vga_col = 0;

    if(vga_row == ROWS)
      scroll_up();
    update_vga_cursor();
  }
  else if(c == '\r')
  {
    vga_col = 0;
    update_vga_cursor();
  }
  else
  {
    unsigned char *vidmem = (unsigned char *)VID_MEM;
    unsigned char *writePosition = vidmem + (vga_row*COLUMNS + vga_col)*2; //times 2 to skip attributes
    writePosition[0] = c;
    writePosition[1] = 0x07; //light grey, black bg

    vga_col++;
    if(vga_col == COLUMNS)
      kputc('\n'); //possible? reentrant?
  }
}

void clear_screen()
{
  int i;
  for(i = 0; i < COLUMNS*ROWS*2; i++)
  {
    unsigned char *vidmem = (unsigned char *)VID_MEM;
    vidmem[i] = 0;
  }

  vga_row = vga_col = 0;
  update_vga_cursor();
}

void scroll_up() 
{
  int row;
  unsigned char * readPos = (unsigned char *)VID_MEM + COLUMNS*2; //start reading from the second row and overwrite the first one
  unsigned char * writePos = (unsigned char *)VID_MEM;

  //where row is the row to written to
  //stop before the last row because there is nothing after it to be read
  for(row = 0; row < ROWS-1; row++) //row 0 becomes row 1, row 1 becomes row 2
  {
    memcpy(writePos, readPos, COLUMNS*2);
    readPos += COLUMNS*2;
    writePos += COLUMNS*2;
  }

  memset(writePos, 0, COLUMNS*2); //zero the last row
  
  if(vga_row > 0)
  {
    vga_row--;
    update_vga_cursor();
  }
}
