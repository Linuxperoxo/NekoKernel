/*
 * 
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : utils.c                       |
 *    |  SRC MOD   : 19/01/2025                    | 
 *    |                                            |
 *    O--------------------------------------------/
 *    
 *
 */

#include <std/int.h>
#include <std/utils.h>
#include <std/str.h>

void memset(void* __src__, __u8 __value__, __u32 __size__)
{
  while(__size__--)
    *((__u8*)__src__++) = __value__;    
}

void memcpy(void* __dest__, void* __src__, __u32 __size__)
{
  while(__size__--)
    *((__u8*)__dest__++) = *((__u8*)__src__++);
}

__attribute__((always_inline)) inline void numstr(__u32 __num__, char* __dest__)
{
  __u16 __i = 0x00;
  __u16 __j = 0x00;

  char __tmp[16];

  do
  {
    __tmp[__i++] = (__num__ % 10) + '0';
    __num__ = __num__ / 10;
  }while(__num__ > 0);

  while(__i > 0)
  {
    __dest__[__j++] = __tmp[--__i];
  }
  __dest__[__j] = '\0';
}


__attribute__((always_inline)) inline void numhex(int __num__, char* __dest__)
{
  
  /*
   *
   * Esse static é importante para evitar problemas, ele
   * vai servir para forçar o compilador a mandar essa array
   * para a .data
   *
   */

  static const char __hex_chars[16] = {
    '0', '1', '2', '3', 
    '4', '5', '6', '7', 
    '8', '9', 'a', 'b', 
    'c', 'd', 'e', 'f'
  };

  int __i = 2;

  for(int __shift = 28; __shift >= 0; __shift -= 4)
  {
    __dest__[__i++] = __hex_chars[((__num__ >> __shift) & 0x0F)];
  }
  __dest__[0]   = '0';
  __dest__[1]   = 'x';
  __dest__[__i] = '\0';
}

void sprintf(__u32 __num__, char* __restrict __type_to_convert, char* __restrict __dest__)
{
  if(strcmp(__type_to_convert, "%d") == 0)
  {
    numstr(__num__, __dest__);
    return;
  }

  if(strcmp(__type_to_convert, "%x") == 0)
  {
    numhex(__num__, __dest__);
    return;
  }
}
