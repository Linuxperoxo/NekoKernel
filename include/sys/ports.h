/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : ports.h                       |
 *    |  SRC MOD   : 18/12/2024                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#ifndef __STD_PORTS__
#define __STD_PORTS__

/*
 *
 * Manipulação direta com I/O Ports
 *
 */

#include <std/int.h>

/*
 *
 * Fiz Funções para cada tipo.
 *
 * Byte, Word e Dword, para 8, 16 e 32 bits 
 *
 */

/*
 *
 * Usei __asm__ volatile para garantir que o compilador
 * não otimize nem remova esse assembly inline, isso é 
 * importante para garantir a integridade desse código
 *
 */

inline __attribute__((always_inline)) void outb(__u16 __port__, __u8 __value__)
{
  __asm__ volatile(
    "outb %%al, %%dx\n"
    :
    : "d"(__port__), "a"(__value__)
    :
  );
}

inline __attribute__((always_inline)) void outw(__u16 __port__, __u16 __value__) 
{
  __asm__ volatile(
    "outw %%ax, %%dx\n"
    :
    : "d"(__port__), "a"(__value__)
    : 
  );  
}

inline __attribute__((always_inline)) void outl(__u16 __port__, __u32 __value__) 
{ 
  __asm__ volatile(
    "outl %%eax, %%dx\n"
    :
    : "d"(__port__), "a"(__value__)
    :
  );
}

inline __attribute__((always_inline)) __u8 inb(__u16 __port__) 
{
  __u8 __inb_return = 0;
  
  __asm__ volatile(
    "inb %%dx, %%al\n"
    : "=a"(__inb_return)
    : "d"(__port__)
    :
  );
  return __inb_return;
}

inline __attribute__((always_inline)) __u16 inw(__u16 __port__) 
{
  __u16 __inb_return = 0;
  
  __asm__ volatile(
    "inw %%dx, %%al\n"
    : "=a"(__inb_return)
    : "d"(__port__)
    :
  );
  return __inb_return;
}

inline __attribute__((always_inline)) __u32 inl(__u16 __port__) 
{
  __u32 __inb_return = 0;
  
  __asm__ volatile(
    "inl %%dx, %%al\n"
    : "=a"(__inb_return)
    : "d"(__port__)
    :
  );
  return __inb_return;
}

#endif
