/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : kernel.h                      |
 *    |  SRC MOD   : 25/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#ifndef __KERNEL__
#define __KERNEL__

#include <std/int.h>

/*
 *
 * Struct que armazena o estado dos registradores
 * quando ocorre uma interrupção
 *
 */

typedef struct int_regs_t {

  /*
   *
   * Registradores que são salvos manualmente
   *
   */

  __u16 __ds;
  __u16 __ss;
  __u16 __fs;
  __u16 __es;
  __u16 __gs;
  __u32 __eax;
  __u32 __ebx;
  __u32 __ecx;
  __u32 __edx;
  __u32 __esi;
  __u32 __edi;

  /*
   *
   * Armazena o número da interrupção e o código de erro
   *
   */

  __u32 __interrupt;
  __u32 __errcode;

  /*
   *
   * Registradores que são empilhados automaticamente
   * quando ocorre uma interrupção que vai ser atendida
   *
   */

  __u32 __esp;
  __u32 __eip;
  __u32 __cs;
  __u32 __eflags;
} __attribute__((packed)) int_regs_t;

void sys_cpu(char *__cpu_id_dest__);

#endif
