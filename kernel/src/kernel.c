/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : kernel.c                      |
 *    |  SRC MOD   : 03/01/2025                    |
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
#include <device/io/keyboard/keyboard.h>

void k_main()
{ 
  terminal_init();

  printf("KEYBOARD...   ");
  keyboard_init();
  printf("[ OK ]\n");

  printf("GDT...        ");
  gdtinit();
  printf("[ OK ]\n");

  printf("IDT...        ");
  idt_init(); 
  printf("[ OK ]\n");

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

  printf("TIMER...      ");
  timer_init();
  printf("[ OK ]\n");

  printf("\n=== NEKO WELCOME: \n\n");
  
  printf("    (\\_/)\n");
  printf("  >( o.o )<\n");
  printf("   /  |  \\ <- This is Neko! Say 'Hi' to Neko\n");
  printf("  /   |   \\\n");
  printf(" (    |    )\n\n");

  printf("\nSYSNEKO: Welcome to Neko Kernel! :D\n\n");
}
