/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : kernel.c                      |
 *    |  SRC MOD   : 11/12/2024                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include <gdt.h>
#include <idt.h>
#include <std/io.h>
#include <std/ports.h>

void keyboard_handler(struct InterruptRegisters* __regs__)
{
  printf("Key Press!");
  inb(0x60);
}

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
  
  printf("IDT...   ");
  idtinit(); 
  printf("[ OK ]\n");

  printf("\nNeko Say -> Welcome to Neko Kernel! :D\n");

  irq_install_isr_handler(0x01, &keyboard_handler);

  printf("OK\n");
}
