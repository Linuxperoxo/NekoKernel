/*
 * 
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : io.h                          |
 *    |  SRC MOD   : 13/12/2024                    | 
 *    |                                            |
 *    O--------------------------------------------/
 *    
 *
 */

#ifndef __STD_IO__
#define __STD_IO__

#include <std/types.h>
#include <video/vga/vga.h>

inline __attribute__((always_inline)) void printf(const char* __text__) {
  while(*__text__ != '\0') {
    switch(*__text__) {
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
        vga_print_char(*__text__++);
      break;
    }
  }
  vga_set_ptr(__vga.__current_row, __vga.__current_col);
}

#endif
