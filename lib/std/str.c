/*
 * 
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : str.c                         |
 *    |  SRC MOD   : 20/01/2025                    | 
 *    |                                            |
 *    O--------------------------------------------/
 *    
 *
 */

#include <std/str.h>
#include <std/int.h>
#include <sys/kmem.h>

__u32 strlen(const char* __src__)
{
  __u32 __str_size = 0x00;

  while(*__src__++ != '\0')
    ++__str_size;
  return __str_size;
}

__u8 strcmp(const char* __restrict __str1__, const char* __restrict __str2__)
{
  if(*__str1__ == '\0' || *__str2__ == '\0')
    return 2;

  while(*__str1__ != '\0' && *__str2__ != '\0')
  {
    if(*__str1__++ != *__str2__++)
      return 1;
  }
  
  if(*__str1__ == '\0' && *__str2__ != '\0' ||
     *__str1__ != '\0' && *__str2__ == '\0')
    return 1; 
  return 0;
}

void strcpy(char* __restrict __dest__, const char* __restrict __src__)
{
  while(*__src__ != '\0')
    *__dest__++ = *__src__++;
}

char* strcat(const char* __restrict __str1__, const char* __restrict __str2__)
{
  __u32 __str1_size = strlen(__str1__);
  __u32 __str2_size = strlen(__str2__);

  char* __buffer = (char*)kmalloc(__str1_size + __str2_size + 1);

  strcpy(__buffer, __str1__);
  strcpy(__buffer + __str1_size, __str2__);

  __buffer[__str1_size + __str2_size] = '\0';

  return __buffer;
}


