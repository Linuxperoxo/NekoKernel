/*
 * 
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : str.h                         |
 *    |  SRC MOD   : 19/12/2024                    | 
 *    |                                            |
 *    O--------------------------------------------/
 *    
 *
 */

#ifndef __STD_STR__
#define __STD_STR__

#include <std/int.h>

__attribute__((always_inline)) inline __u32 strlen(const char* __src__)
{
  __u32 __str_size = 0x00;

  while(*__src__ != '\0')
  {
    ++__str_size;
    ++__src__;
  }
  return __str_size;
}

__attribute__((always_inline)) inline __u8 strcmp(const char* __restrict __str1__, const char* __restrict __str2__)
{
  while(*__str1__ != '\0' && *__str2__ != '\0')
  {
    if(*__str1__ != *__str2__)
      return 1;

    ++__str1__;
    ++__str2__;
  }
  return 0;
}

__attribute__((always_inline)) inline void strcpy(char* __restrict __dest__, const char* __restrict __src__)
{
  while(*__src__ != '\0')
  {
    *__dest__ = *__src__;
    
    ++__dest__;
    ++__src__;
  }
}

__attribute__((always_inline)) inline void strncpy(char* __restrict __dest__, const char* __restrict __src__, __u32 __size__)
{
  while(__size__--)
  {
    *__dest__ = *__src__;
    
    ++__dest__;
    ++__src__;
  }
}

__attribute__((always_inline)) inline void strcat(char* __restrict __dest__, const char* __restrict __to_cat__)
{
  char* __tmp = __dest__ + strlen(__dest__);

  while(*__to_cat__ != '\0')
  {
    *__tmp = *__to_cat__;

    ++__to_cat__;
    ++__tmp;
  }
  *__tmp = '\0';
}

#endif
