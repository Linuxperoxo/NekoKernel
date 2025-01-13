/*
 * 
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : io.h                          |
 *    |  SRC MOD   : 12/01/2025                    | 
 *    |                                            |
 *    O--------------------------------------------/
 *    
 *
 */

#ifndef __STD_IO__
#define __STD_IO__

#include <std/int.h>
#include <std/utils.h>
#include <timer.h>
#include <terminal.h>

inline __attribute__((always_inline)) void printf(const char* __text__) 
{
  while(*__text__ != '\0')
  {
    terminal_out(*__text__++);
  }
}

inline __attribute__((always_inline)) void scanf(char* __dest__, __u16 __size__)
{
  terminal_cln_flag();

  while(!TERMINAL_BUFFER_IS_READY)
  {
    sleep_for(10);
  }
  terminal_cpy_in(__dest__, __size__);
}

#endif
