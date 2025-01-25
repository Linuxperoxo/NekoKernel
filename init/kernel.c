/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : kernel.c                      |
 *    |  SRC MOD   : 25/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include <device/io/keyboard/keyboard.h>
#include <media/ata.h>
#include <neko/gdt.h>
#include <neko/idt.h>
#include <neko/kernel.h>
#include <std/io.h>
#include <std/str.h>
#include <sys/kmem.h>
#include <sys/ports.h>
#include <sys/shell.h>
#include <sys/syscall.h>
#include <sys/task.h>
#include <sys/timer.h>
#include <sys/tty.h>
#include <sys/vfs.h>
#include <video/vga/vga.h>

void k_init() {
  vfs_init();
  vga_init();
  tty_init();
  keyboard_init();
  idt_init();

  __asm__ volatile("cli\n" : : :);

  gdt_init();
  syscall_init();
  timer_init();
  shell_init();

  printf("==========================================\n");
  printf("==          Neko Operating System       ==\n");
  printf("==      Copyright Linuxperoxo 2024      ==\n");
  printf("==========================================\n");
}

void k_main() {
  k_init();

  printf("\n=== NEKO WELCOME: \n\n");

  printf("    (\\_/)\n");
  printf("  >( o.o )<\n");
  printf("   /  |  \\ <- This is Neko! Say 'Hi' to Neko\n");
  printf("  /   |   \\\n");
  printf(" (    |    )\n\n");

  printf("\nNEKO: Welcome to Neko Kernel! :D\n\n");

  __asm__ volatile("sti\n" : : :);
}
