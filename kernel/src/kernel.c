/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : kernel.c                      |
 *    |  SRC MOD   : 06/01/2025                    |
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
#include <syscall.h>
#include <device/io/keyboard/keyboard.h>

void k_shutdown()
{
  printf("\nSYSTEM SHUTTING DOWN!...\n");
  sleep_for(2000);
  outw(0x604, 0x2000);
}

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

  irq_install_isr_handler(0x03, &syscall_handler);

  __asm__ volatile(
    "int $0x23\n"
    :
    :
    :
  );

  k_shutdown();
}
