/*
 * 
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : utils.h                       |
 *    |  SRC MOD   : 19/01/2025                    | 
 *    |                                            |
 *    O--------------------------------------------/
 *    
 *
 */

#ifndef __STD_UTILS__
#define __STD_UTILS__

#include <std/int.h>

#define NULL (void*)0x00

extern void memset(void* __src__, __u8 __value__, __u32 __size__);
extern void memcpy(void* __dest__, void* __src__, __u32 __size__);
extern void sprintf(__u32 __num__, char* __restrict __type_to_convert, char* __restrict __dest__);
extern void numstr(__u32 __num__, char* __dest__);
extern void numhex(int __num__, char* __dest__);

#endif
