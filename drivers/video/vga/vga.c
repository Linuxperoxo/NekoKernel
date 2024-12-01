/*
 * 
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : vga.c                         |
 *    |  SRC MOD   : 30/11/2024                    | 
 *    |                                            |
 *    O--------------------------------------------/
 *    
 *
 */

#include <std/types.h>
#include <video/vga/vga.h>

struct VGAState __vga = {
  0, 0, 0, 0, 0
};

void vga_init()
{
  __vga.__framebuffer = (__u8*) VGA_FRAMEBUFFER_ADDRS;
  __vga.__current_row = DEFAULT_HEIGHT;
  __vga.__current_col = DEFAULT_WIDTH;
  __vga.__bc_color = DEFAULT_BC_COLOR;
  __vga.__ch_color = DEFAULT_CHAR_COLOR;
}

void vga_reset()
{
  __vga.__framebuffer = (__u8*) VGA_FRAMEBUFFER_ADDRS;
  __vga.__current_row = 0;
  __vga.__current_col = 0;
  __vga.__bc_color = DEFAULT_BC_COLOR;
  __vga.__ch_color = DEFAULT_CHAR_COLOR;

  vga_clean();
}

void vga_set_color(__u16 __color__)
{
  __vga.__bc_color = __color__ >> 4 & 0xFF;
  __vga.__ch_color = __color__ & 0xFF;
}

void vga_clean()
{
  __u8* __framebuffer = (__u8*) VGA_FRAMEBUFFER_ADDRS;
  __u16 __size = 0;

  while(__size)
  {
    __framebuffer[__size++] = 0x00;
    __framebuffer[__size++] = __vga.__bc_color << 4 | __vga.__ch_color;
  }
}

void vga_print_char(__u8 __ch__)
{
  if(__vga.__current_col + 1 >= DEFAULT_WIDTH)
  {
    __vga.__current_row += 1;
    __vga.__current_col = 0;
  }
  __vga.__framebuffer[DEFAULT_WIDTH * __vga.__current_row * 2 + __vga.__current_col * 2] = __ch__;
}
