/*
 * 
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : io.h                          |
 *    |  SRC MOD   : 21/12/2024                    | 
 *    |                                            |
 *    O--------------------------------------------/
 *    
 *
 */

#ifndef __STD_IO__
#define __STD_IO__

#include <std/int.h>
#include <std/utils.h>
#include <device/io/keyboard/keyboard.h>
#include <terminal.h>

inline __attribute__((always_inline)) void printf(const char* __text__) 
{
  while(*__text__ != '\0') 
  {
    terminal_out_write(*__text__++);
  }
}

inline __attribute__((always_inline)) void cleanf()
{
  terminal_output_clear();
}

inline __attribute__((always_inline)) void scanf(char* __dest__, __u32 __size__)
{ 
  terminal_input_clear();

  while(terminal_get_last_key() != KEY_ENTER || KEY_IS_PRESS);

  terminal_cpy_input(__dest__, __size__);
}

#endif
