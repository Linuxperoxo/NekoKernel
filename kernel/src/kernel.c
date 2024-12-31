/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : kernel.c                      |
 *    |  SRC MOD   : 22/12/2024                    |
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
#include <device/io/keyboard/keyboard.h>
#include <std/str.h>
#include <sys/kernel.h>
#include <shell.h>

void k_main()
{
  struct Terminal __first_session;
  terminal_init(&__first_session);
  
  printf("KEYBOARD...   ");
  keyboard_init();
  printf("[ OK ]\n");

  printf("GDT...        ");
  gdtinit();
  printf("[ OK ]\n");

  printf("IDT...        ");
  idtinit(); 
  printf("[ OK ]\n");

  printf("\n=== NEKO WELCOME: \n\n");
  
  printf("    (\\_/)\n");
  printf("  >( o.o )<\n");
  printf("   /  |  \\ <- This is Neko! Say 'Hi' to Neko\n");
  printf("  /   |   \\\n");
  printf(" (    |    )\n\n");
  
  printf("\nNeko Say -> Welcome to Neko Kernel! :D\n");
  
  printf("\n=== USER SPACE:\n\n");
}
