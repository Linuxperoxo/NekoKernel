/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : kernel.c                      |
 *    |  SRC MOD   : 19/01/2025                    |
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

  printf("\n=== NEKO WELCOME: \n\n");
  
  printf("    (\\_/)\n");
  printf("  >( o.o )<\n");
  printf("   /  |  \\ <- This is Neko! Say 'Hi' to Neko\n");
  printf("  /   |   \\\n");
  printf(" (    |    )\n\n");

  printf("\nNEKO: Welcome to Neko Kernel! :D\n\n");
}
