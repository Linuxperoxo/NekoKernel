/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : keyboard.c                    |
 *    |  SRC MOD   : 02/01/2025                    |
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

/*
 *
 * Macros
 *
 */

#define KEYBOARD __current_keyboard

static struct Keyboard* KEYBOARD = NULL;

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
  if(KEYBOARD == NULL)
    return;

  KEYBOARD->__scan = inb(KEYBOARD_IN_PORT);
  KEYBOARD->__code = KEYBOARD->__scan & 0x7F; 
  KEYBOARD->__char = __keyboard_layout[KEYBOARD->__code];
  
  /*
   *
   * Flag KEY_IS_PRESS
   *
   */

  KEYBOARD->__flags = ((KEYBOARD->__scan & 0x80) >> 7) == 0x00 ? KEYBOARD->__flags | 0x01 : KEYBOARD->__flags & 0xFE;
  
  /*
   *
   * Flags KEY_IS_VISIBLE
   *
  */

  KEYBOARD->__flags = KEYBOARD->__char != '\0' ? KEYBOARD->__flags | 0x02 : KEYBOARD->__flags & 0xFD;
  
  /*
   *
   * Flag KEY_IS_ESPECIAL
   *
   */
  
  KEYBOARD->__flags = KEYBOARD->__char >= KEY_LOW_ALPHA_ASCII_INIT && KEYBOARD->__char <= KEY_LOW_ALPHA_ASCII_END ||
                                KEYBOARD->__char >= KEY_NUM_ASCII_INIT && KEYBOARD->__char <= KEY_NUM_ASCII_END ? KEYBOARD->__flags | 0x04 : KEYBOARD->__flags & 0xFB;

  if(KEYBOARD_BUFFER_ENABLE)
    KEYBOARD->__buffer_func(KEYBOARD->__code);
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
  KEYBOARD = __new_keyboard__;
}

__u8 keyboard_flags()
{
  return KEYBOARD != NULL ? KEYBOARD->__flags : 0x00;
}
