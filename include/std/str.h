/*
 * 
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : str.h                         |
 *    |  SRC MOD   : 17/01/2024                    | 
 *    |                                            |
 *    O--------------------------------------------/
 *    
 *
 */

#ifndef __STD_STR__
#define __STD_STR__

#include <std/int.h>

__u32 strlen(const char* __src__);
__u8 strcmp(const char* __restrict __str1__, const char* __restrict __str2__);
void strcpy(char* __restrict __dest__, const char* __restrict __src__);
void strncpy(char* __restrict __dest__, const char* __restrict __src__, __u32 __size__);
void strcat(char* __restrict __dest__, const char* __restrict __to_cat__);

#endif
