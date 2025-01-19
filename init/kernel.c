/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : kernel.c                      |
 *    |  SRC MOD   : 18/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include <neko/gdt.h>
#include <neko/idt.h>
#include <std/io.h>
#include <sys/ports.h>
#include <sys/tty.h>
#include <std/str.h>
#include <neko/kernel.h>
#include <media/ata.h>
#include <sys/shell.h>
#include <sys/timer.h>
#include <sys/task.h>
#include <sys/syscall.h>
#include <sys/vfs.h>
#include <sys/kmem.h>
#include <device/io/keyboard/keyboard.h>
#include <video/vga/vga.h>

void k_init()
{
  vfs_init();
  vga_init();
  tty_init();

  keyboard_t* __keyboard = (keyboard_t*)kmalloc(sizeof(keyboard_t));
  keyboard_init(__keyboard);

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
   */

  /*

  __asm__ volatile(
    "movw $0b00001011, %%ax\n"
    "movw %%ax, %%cs\n"
    :
    :
    : "%eax"
  );
  
   */

  printf("\n=== NEKO WELCOME: \n\n");
  
  printf("    (\\_/)\n");
  printf("  >( o.o )<\n");
  printf("   /  |  \\ <- This is Neko! Say 'Hi' to Neko\n");
  printf("  /   |   \\\n");
  printf(" (    |    )\n\n");

  printf("\nNEKO: Welcome to Neko Kernel! :D\n\n");

  //const char* __sla = "Hello, World!\n";

  /*
   *
   * Syscall Teste 
   *
   */
  
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
}
