/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : kernel.h                      |
 *    |  SRC MOD   : 10/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#ifndef __KERNEL__
#define __KERNEL__

#include <std/int.h>
#include <std/io.h>

/*
 *
 * Struct que armazena o estado dos registradores 
 * antes de uma ISR ocorrer
 *
 */

struct InterruptRegisters 
{
  __u32 __cs;
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

__attribute__((always_inline)) inline void sys_cpu(char* __cpu_id_dest__)
{
  __asm__ volatile(
    "movl $0x80000002, %%eax\n"
    "cpuid\n"
    "movl %%eax, (%0)\n"
    "movl %%ebx, 4(%0)\n"
    "movl %%ecx, 8(%0)\n"
    "movl %%edx, 12(%0)\n"

    "movl $0x80000003, %%eax\n"
    "cpuid\n"
    "movl %%eax, 16(%0)\n"
    "movl %%ebx, 20(%0)\n"
    "movl %%ecx, 24(%0)\n"
    "movl %%edx, 28(%0)\n"
    
    "movl $0x80000004, %%eax\n"
    "cpuid\n"
    "movl %%eax, 32(%0)\n"
    "movl %%ebx, 36(%0)\n"
    "movl %%ecx, 40(%0)\n"
    "movl %%edx, 44(%0)\n"

    "movl $0x00, 48(%0)\n"
    :
    : "p"(__cpu_id_dest__)
    : "%eax", "%ebx", "%ecx", "%edx"
  );
}

#endif
