/*
 * 
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : utils.h                       |
 *    |  SRC MOD   : 24/11/2024                    | 
 *    |                                            |
 *    O--------------------------------------------/
 *    
 *
 */

#ifndef __UTILS__
#define __UTILS__

#include "types.h"

inline __attribute__((always_inline)) void memset(void* __dest__, __u8 __value__, __u32 __size__)
{
  __u8* __tmp_dest = (__u8*) __dest__;
  
  while(__size__--)
  {
    *__tmp_dest = __value__;
    ++__tmp_dest;
  }
}

inline __attribute__((always_inline)) void memcpy(void* __dest__, void* __src__, __u32 __size__)
{
  __u8* __tmp_dest = (__u8*) __dest__;
  __u8* __tmp_src = (__u8*) __src__;

  while(__size__--)
  {
    *__tmp_dest = *__tmp_src;
    ++__tmp_dest;
    ++__tmp_src;
  }
}

#endif
