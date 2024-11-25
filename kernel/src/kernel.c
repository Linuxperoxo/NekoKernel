/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : kernel.c                      |
 *    |  SRC MOD   : 24/11/2024                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include "gdt.h"
#include <video/vga/vga.h>

void k_main(void)
{
  init_vga();
  clear_screen();
  initgdt();
  print_screen("GDT is done! ");
  print_screen("Hello, World! :D ");
  print_screen("Hello, From Kernel! :D");
  while(1);
}
