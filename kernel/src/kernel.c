/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : kernel.c                      |
 *    |  SRC MOD   : 09/12/2024                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include <gdt.h>
#include <std/io.h>

void k_main(void)
{
  vga_init();
  vga_clean();

  printf("Booting Kernel...\n");
  
  printf("\n    (\\_/)\n");
  printf("  >( o.o )<\n");
  printf("   /  |  \\ <- This is Neko! Say 'Hi' to Neko\n");
  printf("  /   |   \\\n");
  printf(" (    |    )\n\n");
  
  printf("VGA...   [ OK ]\n");

  printf("GDT...   ");
  gdtinit();
  printf("[ OK ]\n");

  printf("\nNeko Say -> Welcome to Neko Kernel! :D\n");
}
