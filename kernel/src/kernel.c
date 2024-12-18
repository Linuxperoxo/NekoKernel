/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : kernel.c                      |
 *    |  SRC MOD   : 18/12/2024                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include <gdt.h>
#include <idt.h>
#include <std/io.h>
#include <std/ports.h>
#include <terminal.h>
#include <device/io/keyboard/keyboard.h>
#include <std/str.h>

void k_main()
{
  cleanf();

  terminal_init_vga();
  
  printf("=== KERNEL INIT: \n\n");
  
  printf("TERMINAL...   ");
  terminal_init();
  printf("[ OK ]\n");
  
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
  
  printf("\n=== USER LOGIN:\n\n");
  printf("Login: ");
}
