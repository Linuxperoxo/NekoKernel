/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : kernel.c                      |
 *    |  SRC MOD   : 25/11/2024                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include <gdt.h>
#include <video/vga/vga.h>

void k_main(void)
{
  
  /*
   *
   * Kernel start
   *
   */

  init_vga();
  clear_screen();
  print_screen("Booting kernel...\n\n");
  print_screen("Starting VGA...   ");
  print_screen("[ OK ]\n");

  print_screen("Starting GDT...   ");
  initgdt();
  print_screen("[ OK ]\n");

  /*
   *
   * Kernel started
   *
   */

  print_screen("\n");
  print_screen("GDT is done!\n");
  print_screen("Hello, World!\n");
  print_screen("Hello, From Kernel!\n");
  print_screen("Welcome to my kernel :D\n");
  while(1);
}
