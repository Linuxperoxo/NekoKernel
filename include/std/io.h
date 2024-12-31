/*
 * 
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : io.h                          |
 *    |  SRC MOD   : 31/12/2024                    | 
 *    |                                            |
 *    O--------------------------------------------/
 *    
 *
 */

#ifndef __STD_IO__
#define __STD_IO__

#include <std/int.h>
#include <std/utils.h>
#include <terminal.h>

inline __attribute__((always_inline)) void printf(const char* __text__) 
{
  while(*__text__ != '\0') 
  {
    terminal_out(*__text__++);
  }
}

inline __attribute__((always_inline)) void scanf(char* __dest__, __u32 __size__)
{ 
  terminal_cln_flags()

  while(!TERMINAL_BUFFER_IS_READY);
}

#endif
