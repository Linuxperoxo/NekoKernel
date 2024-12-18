/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : kernel.h                      |
 *    |  SRC MOD   : 15/12/2024                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#ifndef __KERNEL__
#define __KERNEL__

/*
 *
 * Struct que armazena o estado dos registradores 
 * antes de uma ISR ocorrer
 *
 */

struct InterruptRegisters {
  __u32 __cr2;
  __u32 __ds;
  __u32 __edi;
  __u32 __esi;
  __u32 __ebp;
  __u32 __esp;
  __u32 __ebx;
  __u32 __edx;
  __u32 __ecx;
  __u32 __eax;
  __u32 __int_num;
  __u32 __error_code;
  
  /*
   *
   * Registradores que são empilhados automaticamento
   * quando ocorre uma interrupção que vai ser atendida
   *
   */

  __u32 __eip;
  __u32 __csm;
  __u32 __eflags;
  __u32 __useresp;
  __u32 __ss;
};

#endif
