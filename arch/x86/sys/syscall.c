/*
 * 
 *
 *    /--------------------------------------------o
 *    |                                            |
 *    |  copyright : (c) 2024 per linuxperoxo.     |
 *    |  author    : linuxperoxo                   |
 *    |  file      : syscall.c                     |
 *    |  src mod   : 13/01/2025                    | 
 *    |                                            |
 *    o--------------------------------------------/
 *    
 *
 */

#include <sys/kernel.h>
#include <terminal.h>
#include <idt.h>

void sys_write()
{
  __asm__ volatile(
    "cmp $0x00, %%ebx\n"
    "jz .Lstdout\n"
    "cmp $0x01, %%ebx\n"
    "jz .Lstdin\n"
    "jmp 3f\n"

    ".Lstdout:\n"
      "1:\n"
        "cmp $0x00, %%edi\n"
        "jz 3f\n"
          
      "2:\n"
        "movb (%%esi), %%al\n"
        "push %%eax\n"
        "call terminal_out\n"
        "pop %%eax\n"
        "inc %%esi\n"
        "dec %%edi\n"
        "jmp 1b\n"
          
      "3:\n"
        "jmp 3f\n"

    ".Lstdin:\n"
      "1:\n"
        "cmp $0x00, %%edi\n"
        "jz 3f\n"
          
      "2:\n"
        "movb (%%esi), %%al\n"
        "push %%eax\n"
        "call terminal_in\n"
        "pop %%eax\n"
        "inc %%esi\n"
        "dec %%edi\n"
        "jmp 1b\n"
        
      "3:\n"
    :
    :
    :
  );
}

void syscall128(struct InterruptRegisters*)
{
  __asm__ volatile(
    "cmp $0x10, %%eax\n"
    "jz .Lsys_write\n"

    ".Lsys_write:\n"
    "call sys_write\n"
    :
    :
    : "%eax", "%ebx", "%esi", "%edi"
  );
}

void syscall177(struct InterruptRegisters*)
{
  printf("syscall177\n");
}

void syscall_init()
{
  idt_install_coop_routine(0x80, &syscall128);
  idt_install_coop_routine(0xB1, &syscall177);
}

