/*
 * 
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : utils.h                       |
 *    |  SRC MOD   : 18/12/2024                    | 
 *    |                                            |
 *    O--------------------------------------------/
 *    
 *
 */

#ifndef __STD_UTILS__
#define __STD_UTILS__

#include <std/types.h>

#define NULL (void*)0x00

__attribute__((always_inline)) inline void memset(void* __src__, __u8 __value__, __u32 __size__)
{
  while(__size__--)
  {
    *((__u8*)__src__) = __value__;
  }
}

__attribute__((always_inline)) inline void memcpy(void* __dest__, void* __src__, __u32 __size__)
{
  while(__size__--)
  {
    *((__u8*)__dest__) = *((__u8*)__src__);

    __dest__++;
    __src__++;
  }
}

#endif
