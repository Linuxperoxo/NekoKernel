/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : kernel.c                      |
 *    |  SRC MOD   : 13/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include <gdt.h>
#include <idt.h>
#include <std/io.h>
#include <sys/ports.h>
#include <terminal.h>
#include <std/str.h>
#include <sys/kernel.h>
#include <media/ata.h>
#include <shell.h>
#include <timer.h>
#include <task.h>
#include <syscall.h>
#include <device/io/keyboard/keyboard.h>

void k_init()
{
  terminal_init();
  keyboard_init();
  idt_init();
  gdt_init();
  syscall_init();
  task_init();
  timer_init();

  printf("==========================================\n");
  printf("==          Neko Operating System       ==\n");
  printf("==      Copyright Linuxperoxo 2024      ==\n");
  printf("==========================================\n");
}

void k_main()
{ 
  k_init();

  /*
   *
   * Esse código assembly foi apenas por testes
   *
   * __asm__ volatile(
   * "movw $0b00001011, %%ax\n"
   * "movw %%ax, %%cs\n"
   * :
   * :
   * : "%eax"
   * );
   * 
   */

  printf("\n=== NEKO WELCOME: \n\n");
  
  printf("    (\\_/)\n");
  printf("  >( o.o )<\n");
  printf("   /  |  \\ <- This is Neko! Say 'Hi' to Neko\n");
  printf("  /   |   \\\n");
  printf(" (    |    )\n\n");

  printf("\nNEKO: Welcome to Neko Kernel! :D\n\n");

  const char* __sla = "Hello, World!\n";

  /*
  
  __asm__ volatile(
    "movl $0x10, %%eax\n" // sys_write
    "movl $0x00, %%ebx\n" // stdout 
    "movl %0, %%esi\n"    // Ponteiro para o conteúdo de escrita
    "movl $0x0E, %%edi\n" // Bytes a ser escritos
    "int $0x80\n"         // syscall
    :
    :"p"(__sla)
    :"%eax", "%ebx", "%esi", "%edi"
  );

  */
  shell_init();
}
