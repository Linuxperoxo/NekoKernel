/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : keyboard.c                    |
 *    |  SRC MOD   : 17/12/2024                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include <idt.h>
#include <std/io.h>
#include <std/types.h>
#include <std/ports.h>
#include <std/kernel.h>
#include <device/io/keyboard/keyboard.h>

struct Keyboard __keyboard = { 0x00 };

extern void terminal_keyboard_input();

__u8 __keyboard_layout[] = {
  '\0',
  '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\0',
  '\0', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\0',
  '\0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '\0',
  '\0', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 
  '\0', '\0', '\0',               ' ',              '\0', '\0', '\0'
};

void keyboard_handler(struct InterruptRegisters*) 
{
  __keyboard.__scan = inb(KEYBOARD_IN_PORT);
  __keyboard.__code = __keyboard.__scan & 0x7F; 
  __keyboard.__char = __keyboard_layout[__keyboard.__code];
  
  /*
   *
   * Flag KEY_IS_PRESS
   *
   */

  __keyboard.__flags = ((__keyboard.__scan & 0x80) >> 7) == 0x00 ? __keyboard.__flags | 0x01 : __keyboard.__flags & 0xFE;
  
  /*
   *
   * Flags KEY_IS_VISIBLE
   *
  */

  __keyboard.__flags = __keyboard.__char != '\0' ? __keyboard.__flags | 0x02 : __keyboard.__flags & 0xFD;
  
  /*
   *
   * Flag KEY_IS_ESPECIAL
   *
   */
  
  __keyboard.__flags = __keyboard.__char >= KEY_LOW_ALPHA_ASCII_INIT && __keyboard.__char <= KEY_LOW_ALPHA_ASCII_END ||
                       __keyboard.__char >= KEY_NUM_ASCII_INIT && __keyboard.__char <= KEY_NUM_ASCII_END ? __keyboard.__flags | 0x04 : __keyboard.__flags & 0xFB;

  if(KEYBOARD_TERMINAL_BUFFER_ENABLE)
    terminal_keyboard_input();
}

void keyboard_init()
{
  irq_install_isr_handler(IRQ_KEYBOARD_NUM, &keyboard_handler);
}

