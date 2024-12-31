/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : keyboard.c                    |
 *    |  SRC MOD   : 31/12/2024                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include <idt.h>
#include <std/io.h>
#include <std/int.h>
#include <sys/ports.h>
#include <sys/kernel.h>
#include <terminal.h>
#include <device/io/keyboard/keyboard.h>

struct Keyboard* __current_keyboard = NULL;

__u8 __keyboard_layout[] = {
  '\0',
  '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\0',
  '\0', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\0',
  '\0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '\0',
  '\0', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 
  '\0', '\0', '\0',               ' ',              '\0', '\0', '\0'
};

/*
 *
 * Funções internas
 *
 */

void keyboard_handler(struct InterruptRegisters*) 
{
  __current_keyboard->__scan = inb(KEYBOARD_IN_PORT);
  __current_keyboard->__code = __current_keyboard->__scan & 0x7F; 
  __current_keyboard->__char = __keyboard_layout[__current_keyboard->__code];
  
  /*
   *
   * Flag KEY_IS_PRESS
   *
   */

  __current_keyboard->__flags = ((__current_keyboard->__scan & 0x80) >> 7) == 0x00 ? __current_keyboard->__flags | 0x01 : __current_keyboard->__flags & 0xFE;
  
  /*
   *
   * Flags KEY_IS_VISIBLE
   *
  */

  __current_keyboard->__flags = __current_keyboard->__char != '\0' ? __current_keyboard->__flags | 0x02 : __current_keyboard->__flags & 0xFD;
  
  /*
   *
   * Flag KEY_IS_ESPECIAL
   *
   */
  
  __current_keyboard->__flags = __current_keyboard->__char >= KEY_LOW_ALPHA_ASCII_INIT && __current_keyboard->__char <= KEY_LOW_ALPHA_ASCII_END ||
                                __current_keyboard->__char >= KEY_NUM_ASCII_INIT && __current_keyboard->__char <= KEY_NUM_ASCII_END ? __current_keyboard->__flags | 0x04 : __current_keyboard->__flags & 0xFB;

  if(KEYBOARD_BUFFER_ENABLE)
    __current_keyboard->__buffer_func(__current_keyboard->__code);
}

/*
 *
 * Funções keyboard.h
 *
 */

void keyboard_init()
{
  irq_install_isr_handler(IRQ_KEYBOARD_NUM, &keyboard_handler);
}

void keyboard_switch(struct Keyboard* __new_keyboard__)
{
  __current_keyboard = __new_keyboard__;
}

