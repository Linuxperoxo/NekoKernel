/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : kernel.c                      |
 *    |  SRC MOD   : 20/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include <neko/kernel.h>
#include <std/int.h>

void sys_cpu(char* __cpu_id_dest__)
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
