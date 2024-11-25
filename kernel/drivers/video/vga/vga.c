/*
 * 
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : vga.c                         |
 *    |  SRC MOD   : 24/11/2024                    | 
 *    |                                            |
 *    O--------------------------------------------/
 *    
 *
 */

#include <types.h>
#include "vga.h"

struct {
  __u8 __vga_background_color;
  __u8 __vga_char_color;
  __u8 __vga_screen_height, __vga_screen_width;
  __u8* __current;
}__kvga;


void init_vga()
{
  __kvga.__vga_char_color = DEFAULT_CHAR_COLOR;
  __kvga.__vga_background_color = DEFAULT_BC_COLOR;
  __kvga.__current = (__u8*) FRAME_BUFFER_ADDRS;
  __kvga.__vga_screen_height = DEFAULT_HEIGHT;
  __kvga.__vga_screen_width = DEFAULT_WIDTH;
}

void print_screen(const char* __text__)
{
  for(unsigned int __i = 0; *__text__ != '\0'; __i++)
  {

    /*
     *
     * Colocando o caractere
     *
     */

    *__kvga.__current = *__text__;
    ++__kvga.__current;
    /*
     *
     * Movendo os bits para o lugar certo
     *
     */

    *__kvga.__current = (__kvga.__vga_background_color << 4) | __kvga.__vga_char_color;
    ++__kvga.__current;
    ++__text__;
  }
}

void clear_screen()
{
  __kvga.__current = (__u8*) FRAME_BUFFER_ADDRS;
  __u32 __i = 0;
  
  while(__i++ < __kvga.__vga_screen_height * __kvga.__vga_screen_width * 2)
  {
    *__kvga.__current = 0;
    ++__kvga.__current;
  }
  __kvga.__current = (__u8*) FRAME_BUFFER_ADDRS;
}
