/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : keyboard.c                    |
 *    |  SRC MOD   : 19/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include <neko/idt.h>
#include <std/io.h>
#include <std/int.h>
#include <sys/ports.h>
#include <sys/vfs.h>
#include <neko/kernel.h>
#include <sys/kmem.h>
#include <sys/tty.h>
#include <std/utils.h>
#include <device/io/keyboard/keyboard.h>

extern void tty_keyboard_in(keyboard_t* __keyboard__);

static char __keyboard_layout[61] = {
  '\0',
  '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\0',
  '\0', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\0',
  '\0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '\0',
  '\0', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 
  '\0', '\0', '\0',               ' ',              '\0', '\0', '\0'
};

static keyboard_t* __keyboard = NULL;

/*
 *
 *  Internal Functions
 *
 */

static void keyboard_handler(struct InterruptRegisters*) 
{
  if(__keyboard == NULL)
    return;

  __keyboard->__scan = inb(KEYBOARD_IN_PORT);
  __keyboard->__code = __keyboard->__scan & 0x7F; 
  __keyboard->__char = __keyboard_layout[__keyboard->__code];
  
  /*
   *
   * Flag KEY_IS_PRESS
   *
   */

  __keyboard->__flags = ((__keyboard->__scan & 0x80) >> 7) == 0x00 ? __keyboard->__flags | 0x01 : __keyboard->__flags & 0xFE;
  
  /*
   *
   * Flags KEY_IS_VISIBLE
   *
  */

  __keyboard->__flags = __keyboard->__char != '\0' ? __keyboard->__flags | 0x02 : __keyboard->__flags & 0xFD;
  
  /*
   *
   * Flag KEY_IS_ESPECIAL
   *
   */
  
  __keyboard->__flags = __keyboard->__char >= KEY_LOW_ALPHA_ASCII_INIT && __keyboard->__char <= KEY_LOW_ALPHA_ASCII_END ||
                                __keyboard->__char >= KEY_NUM_ASCII_INIT && __keyboard->__char <= KEY_NUM_ASCII_END ? __keyboard->__flags | 0x04 : __keyboard->__flags & 0xFB;

  tty_keyboard_in(__keyboard);
}

static __u8 keyboard_read(offset_t __offset, void* __buffer__)
{
  memcpy(__buffer__, __keyboard, sizeof(keyboard_t));
  return 0;
}

/*
 *
 *  Functions keyboard.h
 *
 */

void keyboard_init()
{
  __keyboard = (keyboard_t*)kmalloc(sizeof(keyboard_t));

  vfs_mkchfile("/dev/input0", ROOT_UID, ROOT_GID, READ_O | WRITE_O, &keyboard_read, NULL);
  idt_install_coop_routine(KEYBOARD_INT_NUM, &keyboard_handler);
}

void keyboard_switch(keyboard_t* __keyboard__)
{
  __keyboard = __keyboard__;
}

