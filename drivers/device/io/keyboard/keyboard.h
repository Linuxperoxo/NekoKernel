/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : keryboard.h                   |
 *    |  SRC MOD   : 17/12/2024                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#ifndef __DRIVERS_KEYBOARD__
#define __DRIVERS_KEYBOARD__

#include <std/types.h>

#define IRQ_KEYBOARD_NUM 0x01

#define KEYBOARD_IN_PORT 0x60
#define KEYBOARD_COMMAND_PORT 0x64
#define KEYBOARD_RESET_COMMAND 0xFF

#define KEYBOARD_KEY_FLAGS_STATUS (__keyboard.__flags & 0xFF)

/*
 *
 * Keys Code
 *
 */

#define KEY_ESC    0x01
#define KEY_1      0x02
#define KEY_2      0x03
#define KEY_3      0x04
#define KEY_4      0x05
#define KEY_5      0x06
#define KEY_6      0x07
#define KEY_7      0x08
#define KEY_8      0x09
#define KEY_9      0x0A
#define KEY_0      0x0B
#define KEY_SUB    0x0C
#define KEY_EQU    0x0D
#define KEY_BACK   0x0E

#define KEY_TAB    0x0F
#define KEY_Q      0x10
#define KEY_W      0x11
#define KEY_E      0x12
#define KEY_R      0x13
#define KEY_T      0x14
#define KEY_Y      0x15
#define KEY_U      0x16
#define KEY_I      0x17
#define KEY_O      0x18
#define KEY_P      0x19
#define KEY_OBRACK 0x1A
#define KEY_CBRACK 0x1B
#define KEY_ENTER  0x1C

/*
 *
 * Keys Alpha
 *
 */

#define KEY_LOW_ALPHA_ASCII_INIT 97
#define KEY_LOW_ALPHA_ASCII_END  122

#define KEY_UP_ALPHA_ASCII_INIT 65
#define KEY_UP_ALPHA_ASCII_END  90

/*
 *
 * Keys Num
 *
 */

#define KEY_NUM_ASCII_INIT 48
#define KEY_NUM_ASCII_END  57

/*
 *
 * Cond for keyboard
 *
 */

#define KEY_IS_PRESS (KEYBOARD_KEY_FLAGS_STATUS & 0x01)
#define KEY_IS_VISIBLE ((KEYBOARD_KEY_FLAGS_STATUS & 0x02) >> 1)
#define KEY_IS_ESPECIAL ((KEYBOARD_KEY_FLAGS_STATUS & 0x04) >> 2)
#define KEYBOARD_TERMINAL_BUFFER_ENABLE ((KEYBOARD_KEY_FLAGS_STATUS & 0x08) >> 3)

/*
 *
 * Buffer Bit Flags
 *
 */

#define KEYBOARD_FLAG_VISIBLE ((KEYBOARD_KEY_FLAGS_STATUS & 16) >> 4)
#define KEYBOARD_FLAG_ESPECIAL ((KEYBOARD_KEY_FLAGS_STATUS & 32) >> 5)
#define KEYBOARD_FLAG_CODE ((KEYBOARD_KEY_FLAGS_STATUS & 64) >> 6)
#define KEYBOARD_FLAG_RELEASE ((KEYBOARD_KEY_FLAGS_STATUS & 128) >> 7)

/*
 *
 * Buffer Flags
 *
 */

struct Keyboard {
  __u8 __scan; // Tem todo o scan da instrução IN
  __u8 __code; // Tecla que foi pressionada
  __u8 __char; // Caractere a ser impresso
  __u8 __flags; // Flags da tecla
};

/*
 * __full_scan:
 *  
 *  * Tem o byte completo sem nenhuma manipulação da instrução IN
 *
 * __key: 
 *  
 *  * Código da tecla pressionada
 *
 * __char:
 *  
 *  * Aqui fica o caractere que representa essa tecla, isso se for uma tecla de um caractere
 *
 * __flags:
 *  
 *  * Cada bit de __flags representa uma coisa:
 *
 *  * bit 0: Representa se a tecla foi pressionada ou solta, 1 para pressionada e 0 para solta;
 *  * bit 1: Representa se a tecla é visível, 1 para visível e 0 para não visível (caracteres)
 *  * bit 2: É um caractere especial ou não, 1 para especial, 0 para não especial
 *  * bit 3: Buffer Habilitado, 1 para sim, 0 para não
 *
 */

extern struct Keyboard __keyboard;

extern void keyboard_init();

#endif
