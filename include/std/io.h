/*
 * 
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : io.h                          |
 *    |  SRC MOD   : 30/11/2024                    | 
 *    |                                            |
 *    O--------------------------------------------/
 *    
 *
 */

#ifndef __STDIO__
#define __STDIO__

#include <std/types.h>
#include <video/vga/vga.h>

inline __attribute__((always_inline)) void printf(const char* __text__)
{
  while(*__text__ != '\0')
  {
    switch(*__text__)
    {
      case '\n':
        __vga.__current_row += 1;
        __vga.__current_col = 0;
        
        ++__text__;
      break;

      case '\r':
        __vga.__current_col = 0;

        ++__text__;
      break;

      default:
        vga_print_char(*__text__);
      break;
    }
  }
}

#endif
