/*
 * 
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : str.c                         |
 *    |  SRC MOD   : 19/01/2024                    | 
 *    |                                            |
 *    O--------------------------------------------/
 *    
 *
 */

#include <std/str.h>
#include <std/int.h>

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

void strcat(char* __restrict __dest__, const char* __restrict __to_cat__)
{
  char* __tmp = __dest__ + strlen(__dest__);

  while(*__to_cat__ != '\0')
    *__tmp++ = *__to_cat__++;
  *__tmp = '\0';
}


